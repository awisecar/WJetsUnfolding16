//-*- mode: c++; c-basic-offset: 4 -*-
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <memory>
#include <math.h>
#include <algorithm>
#include <TStyle.h>
#include <TCanvas.h>
#include <TArrow.h>
#include <TLine.h>
#include <TSVDUnfold.h>
#include <TParameter.h>
#include "fileNamesZJets.h"
#include "getFilesAndHistogramsZJets.h"
#include "variablesOfInterestZJets.h"
#include "UnfoldingZJets.h"
#include "PlotSettings.h"
#include "fixYscale.C"
#include "ConfigVJets.h"
#include "functions.h"
#include "TRandom3.h"
#include <sys/time.h>
#include "TVectorD.h"
#include "TMatrixD.h"
#include "TDecompSVD.h"
#include <regex>
#include "TVectorT.h"
#include "TMatrixT.h"

//headers for TUnfold
#include "TUnfold.h"
#include "TUnfoldDensity.h"
#include "TUnfoldBinning.h"
#include "TUnfoldSys.h"

extern ConfigVJets cfg;
const static bool isdatabug = false;
static int verbosity = 1;

using namespace std;

void UnfoldingZJets(TString lepSel, int year, TString algo, TString histoDir, TString unfoldDir, int jetPtMin, int jetEtaMax, TString variable, bool doNormalized, int whichSyst)
{
    gStyle->SetOptStat(0);

    //--- Create output directory if does not exist ---
    system("mkdir -p " + unfoldDir);

    int start = 0;
    int end = NVAROFINTERESTZJETS;

    //--- Choosing distribution to unfold
    if (variable != "") {
        start = findVariable(variable);
        if (start >= 0) {
            end = start + 1;
            cout << "\n ============> Processing only variable: " << variable << " <============ " << endl;
        }
        else {
            cerr << "\nError: variable " << variable << " is not defined." << endl;
            cerr << "See below the list of defined variables:" << endl;
            for (unsigned int i = 0; i < NVAROFINTERESTZJETS; ++i) {
                cerr << "\t" << i << ": " << VAROFINTERESTZJETS[i].name << "\n" << endl;
            }
            return;
        }
    }

    double integratedLumi(0.);
    if (year == 2016) integratedLumi = 35916.982;
    else if (year == 2017) integratedLumi = 41524.945;
    else integratedLumi = 59717.255;

    std::cout << "\n year = " << year << std::endl;
    std::cout << " integratedLumi = " << integratedLumi << std::endl; 

    //--- Here we declare the different arrays of TFiles (nominal/central and systematic variations)
    // fData is for the 3 data files: 
    // 0 - central, 1 - JES up, 2 - JES down
    TFile *fData[3] = {NULL}; 
    // fDYJets is for the 9 signal files:
    // 0 - central, 1 - PU up, 2 - PU down, 3 - JER up, 4 - JER down, 5 - LepSF up, 6 - LepSF down, 7 - BtagSF up, 8 - BtagSF down 
    TFile *fDYJets[9] = {NULL};
    // fBg is for the NBGDYJETS x 7 systematics files:
    // 0 - central, 1 - PU up, 2 - PU down, 3 - XSEC up, 4 - XSEC down, 5 - BtagSF up, 6 - BtagSF down
    TFile *fBg[NBGDYJETS][7] = {{NULL}};

    //--- Open all files!
    std::cout << "\n<=========================================================================================>" << std::endl;
    std::cout << "\nOpening all files from reco-level analysis code..." << std::endl;
    getAllFiles(histoDir, lepSel, "13TeV", jetPtMin, jetEtaMax, fData, fDYJets, fBg, NBGDYJETS);
    
    //--- These are the names of the different WJets theory files (defined in fileNamesZJets.h)
    //--- Grabbed in the following lines below
    std::cout << "\nGrabbing names, files for some additional GEN/theory predictions..." << std::endl;
    cout << "DYSHERPA14FILENAME = " << DYSHERPA14FILENAME << endl;
    cout << "DYMGPYTHIA8FILENAME = " << DYMGPYTHIA8FILENAME << endl;
    cout << "NNLO1JFILENAME = " << NNLO1JFILENAME << endl;

    TFile *fSheUnf = 0;
    TFile *fGen1 = NULL;
    TString gen1;
    TString gen1File;
    TFile *fGen2 = NULL;
    TString gen2;
    TString gen2File;
    std::map<TString, vector<TString> > generatorNames;

    std::cout << std::endl;
    //--- Use this Flag for Resp Syst (sherpa unfolding in 8TeV) ---
    if (DYSHERPA14FILENAME.Length() > 0 ){
        //--- Open additional generator files -----------------------------------------------------
        TString genSherpaFile = histoDir + lepSel + "_13TeV_" + DYSHERPA14FILENAME + "_dR_TrigCorr_0_Syst_0_JetPtMin_30_JetEtaMax_24.root";
        std::cout << "Opening file with name: " << genSherpaFile << std::endl;
        fSheUnf = new TFile(histoDir + lepSel + "_13TeV_" + DYSHERPA14FILENAME + "_dR_TrigCorr_0_Syst_0_JetPtMin_30_JetEtaMax_24.root");
        vector<TString> sherpa14;
        sherpa14.push_back(DYSHERPA14FILENAME);
        sherpa14.push_back(DYSHERPA14LEGEND);
        generatorNames["sherpa14"] = sherpa14;
    }   
    else std::cout << "No filename for fSheUnf specified! " << std::endl;   
    //--- Use this Flag for the 2nd Theory -----------------------
    if (DYMGPYTHIA8FILENAME.Length() > 0 ){
        vector<TString> mgpythia8;
        mgpythia8.push_back(DYMGPYTHIA8FILENAME);
        mgpythia8.push_back(DYMGPYTHIA8LEGEND);
        
        generatorNames["mgpythia8"] = mgpythia8;
        gen1 = "mgpythia8";
        
        gen1File = histoDir + lepSel + "_13TeV_" + generatorNames[gen1][0] + "_dR_TrigCorr_1_Syst_0_JetPtMin_";
        gen1File += jetPtMin;
        gen1File += "_JetEtaMax_";
        gen1File += jetEtaMax;
        gen1File += ".root";
        std::cout << "Opening file with name: " << gen1File << std::endl;
        fGen1 = new TFile(gen1File);
    }
    else std::cout << "No filename for fGen1 specified! " << std::endl; 
    //--- Use this Flag for the 3rd Theory -----------------------
    if (NNLO1JFILENAME.Length() > 0 ){
        vector<TString> nnlo1j;
        nnlo1j.push_back(NNLO1JFILENAME);
        nnlo1j.push_back(NNLO1JLEGEND);
        
        generatorNames["nnlo1j"] = nnlo1j;
        gen2 = "nnlo1j";
        
        gen2File = histoDir + lepSel + "_13TeV_" + generatorNames[gen2][0] + ".root";
        std::cout << "Opening file with name: " << gen2File << std::endl;
        fGen2 = new TFile(gen2File);
    }
    else std::cout << "No filename for fGen2 specified! " << std::endl; 

    std::cout << "\n<=========================================================================================>" << std::endl;
    //----------------------------------------------------------------------------------------- 

    //----------------------------------------------------------------------------------------- 

    // --- All necessary files have now been opened ---
    // --- Now run on the different variables ---

    for (int i = start; i < end; ++i) {
        variable = VAROFINTERESTZJETS[i].name;

        //--- Create output file  
        TString outputFileName = unfoldDir + lepSel; 
        outputFileName += "_unfolded_" + variable + "_" + algo;
        outputFileName += "_JetPtMin_";
        outputFileName += jetPtMin;
        outputFileName += "_JetEtaMax_";
        outputFileName += jetEtaMax;
        outputFileName += "_MGPYTHIA6_";
        outputFileName += doNormalized ? "_normalized" : "";   
        TFile *outputRootFile = new TFile(outputFileName + ".root", "RECREATE");
    
        //--- Setting up arrays for histograms (for reco-level, fakes, purities, etc.)

        // 1) data
        //--- rec Data histograms ---
        TH1D *hRecData[3] = {NULL};

        // 2) signal
        //--- rec DYJets histograms ---
        TH1D *hRecDYJets[13] = {NULL};
        //--- gen DYJets histograms ---
        TH1D *hGenDYJets[11] = {NULL};
        //--- res DYJets histograms ---
        TH2D *hResDYJets[13] = {NULL};

        // 3) BG
        //--- rec Bg histograms ---
        TH1D *hRecBg[NBGDYJETS][11] = {{NULL}};
        //--- rec Sum Bg histograms ---
        TH1D *hRecSumBg[11] = {NULL};

        // 4) other
        //--- fake DYJets histograms ---
        TH1D *hFakDYJets[18] = {NULL};
        //--- purity "histograms" ---
        TH1D *hPurity[18] = {NULL};
        
        //--- Get all histograms needed for unfolding! ---
        // Note: andrew -- ttbar scaling done inside getAllHistos
        std::cout << "\nGrabbing all needed histograms from files opened above..." << std::endl;
        getAllHistos(variable, hRecData, fData, hRecDYJets, hGenDYJets, hResDYJets, fDYJets, hRecBg, hRecSumBg, fBg, NBGDYJETS, hFakDYJets, hPurity);
        
        //--- Get histos from theory files (if files opened above)
        TH1D *hGen1 = NULL;
        TH1D *hGen2 = NULL;
        // if (DYSHERPA14FILENAME.Length() > 0 ) respDYJets[17] = getResp(fSheUnf, variable); // Sherpa Unfolding response (Resp Syst)
        if (DYMGPYTHIA8FILENAME.Length() > 0 ) hGen1 = getHisto(fGen1, "gen" + variable);
        if (NNLO1JFILENAME.Length() > 0 ){
            if (variable == "FirstJetPt_Zinc1jet" || variable == "JetsHT_Zinc1jet" || variable == "FirstJetAbsRapidity_Zinc1jet" || variable == "dRptmin100LepCloseJetCo300dR04_Zinc1jet" || variable == "dPhiLepJet1_Zinc1jet"){
                //--- Additional (3rd) theory comparison ---
                hGen2 = getHisto(fGen2, "gen" + variable);
            }
        }

        outputRootFile->cd();

        //write out w+jets gen distributions before lumi and bin width scaling
        hGenDYJets[0]->Write("hMadGenDYJetsCrossSection_NoScale");
        hGen1->Write("hGen1DYJetsCrossSection_NoScale");

        //this is the signal NLO FxFx W+jets MC (used for building migration/response matrix for unfolding)
        TH1D *hMadGenCrossSection = makeCrossSectionHist(hGenDYJets[0], integratedLumi);
        hMadGenCrossSection->SetZTitle("MG_aMC FxFx + PY8 (#leq 2j NLO + PS)");

        //this is the signal LO MLM W+jets MC
        TH1D *hGen1CrossSection = makeCrossSectionHist(hGen1, integratedLumi);
        hGen1CrossSection->SetZTitle(generatorNames[gen1][1]);

        //this is the tag for another theory prediciton (NNLO prediction in 2015 W+jets)
        TH1D *hGen2CrossSection;
        if (variable == "FirstJetPt_Zinc1jet" || variable == "JetsHT_Zinc1jet" || variable == "FirstJetAbsRapidity_Zinc1jet" || variable == "dRptmin100LepCloseJetCo300dR04_Zinc1jet" || variable == "dPhiLepJet1_Zinc1jet"){
            hGen2CrossSection = makeCrossSectionHist(hGen2, 0);
            hGen2CrossSection->SetZTitle(generatorNames[gen2][1]);
        }

        std::cout << "\n<=========================================================================================>" << std::endl;

        //--- Here is an array of TH1D to store the various unfolded data:
        // 0 - Central, 
        // 1 - JES up, 2 - JES down, 
        // 3 - PU up, 4 - PU down, 
        // 5 - JER up, 6 - JER down, 
        // 7 - XSEC up, 8 - XSEC down
        // 9 - , 10 - 
        // 11 - BtagSF up, 12 - BtagSF down 
        // 13 - Lumi up, 14 - Lumi down
        // 15 - SF up, 16 - SF down
        // 17 - 

        //andrew -- re-check that the following order of systematics is replicated in the order that the histos are read in from the files
        //Lumi and SF are not read in from files; they are clones of the Central histo that's globally scaled by a single factor
        //as of 6 March 2019
        TString name[] = {"Central", "JESUp", "JESDown", "PUUp", "PUDown", "JERUp", "JERDown", "XSECUp", "XSECDown", 
                            "blank1", "blank2", "BtagSFUp", "BtagSFDown", "LumiUp", "LumiDown", "SFUp", "SFDown", "blank5"};

        TH1D *hUnfData[18] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,};
        TH1D *hUnfDataNoScale[18] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,};
        TH2D *hUnfDataStatCov[18] =  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,};
        TH2D *hUnfDataStatCovNoScale[18] =  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,};
        TH2D *hUnfMCStatCov[18] =  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,};
        TH2D *hUnfMCStatCovNoScale[18] =  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,};
        int nIter[18] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; 

        int nSysts = DYSHERPA14FILENAME.Length()? 18 : 17;
        bool logy = VAROFINTERESTZJETS[i].log;

        //--- Unfold the Data histograms for each systematic ---
        std::cout << "\nEntering iSyst loop to unfold for Central and every variation for each systematic..." << std::endl;
        std::cout << "nSysts = " << nSysts << std::endl;
        std::cout << "whichSyst = " << whichSyst << std::endl;
        for (unsigned short iSyst = 0; iSyst < nSysts; ++iSyst){
            std::cout << "\n >>>>>>>>>> Start iSyst = " << iSyst << "! <<<<<<<<<<" << std::endl;
            if(iSyst != 0 && whichSyst >= 0 && iSyst != whichSyst) continue;



            // ALW 27 Jan 20
            // temporary, while working on code...
            if (iSyst != 0) continue;



            //--- This section selects the correct hRecData file and hRecSumBg files to use for background subtraction, and then unfolding
            //For JES (iSyst=1,2), select correctly varied data file; otherwise, use Central
            unsigned short iData = (iSyst == 1 || iSyst == 2) ? iSyst : 0;
            //Select correct BG sum for BG-based systematics
            unsigned short iBg = 0;
            //for central, JES, JER, LER, Sherpa unf --> use central background distribution
            if (iSyst == 0 || iSyst == 1 || iSyst == 2 || iSyst == 5 || iSyst == 6 || iSyst == 9 || iSyst == 10 || iSyst == 17) iBg = 0;
            //for PU, use iBg=1,2
            else if (iSyst == 3 || iSyst == 4) iBg = iSyst - 2;
            //for XSEC, use iBg=3,4;
            else if (iSyst == 7 || iSyst == 8) iBg = iSyst - 4;
            //for BtagSF, use iBg=5,6; for Lumi, use iBg=7,8; for SF, use iBg=9,10
            else if (iSyst == 11 || iSyst == 12 || iSyst == 13 || iSyst == 14 || iSyst == 15 || iSyst == 16) iBg = iSyst - 6;

            //Grab reco data
            std::cout << "\nFetching main reco data histo hRecData[iData], " << "iData = " << iData << std::endl;
            TH1D *hRecDataMinusFakes = (TH1D*) hRecData[iData]->Clone();
            //Subtract backgrounds from data to obtain signal
            std::cout << "From reco data, subtracting MC BG histo hRecSumBg[iBg], " << "iBg = " << iBg << std::endl;
            hRecDataMinusFakes->Add(hRecSumBg[iBg], -1);
            //Subtract fakes for background-subtracted data
            std::cout << "Removing fakes using hFakDYJets[iSyst], hPurity[iSyst]!" << std::endl;
            RemoveFakes(hRecDataMinusFakes, hFakDYJets[iSyst], hPurity[iSyst]);



            //This section picks the correct hGenDYJets histogram for calculating efficiencies with respect to the response matrix (inside TUnfoldData)
            unsigned short iGen = 0;
            unsigned short iResp = 0; //for picking correct response matrix from hResDYJets
            //Central, JES, XSEC, SF, Alt. Unfolding
            if (iSyst == 0 || iSyst == 1 || iSyst == 2 || iSyst == 7 || iSyst == 8 || iSyst == 15 || iSyst == 16 || iSyst == 17){
                iGen = 0;
                iResp = 0;
                if (iSyst == 15) iResp = 11;
                else if (iSyst == 16) iResp = 12;
            }
            //PU, JER
            else if (iSyst == 3 || iSyst == 4 || iSyst == 5 || iSyst == 6){
                iGen = iSyst - 2;
                iResp = iSyst - 2;
            }
            //blank1, blank2, BtagSF, Lumi
            else if (iSyst == 9 || iSyst == 10 || iSyst == 11 || iSyst == 12 || iSyst == 13 || iSyst == 14){
                iGen = iSyst - 4;
                iResp = iSyst - 4;
            }
            //Output indices for correct response and gen signal histos
            std::cout << "Fetching gen signal histo hGenDYJets[iGen], " << "iGen = " << iGen << std::endl;
            std::cout << "Fetching response matrix histo hResDYJets[iResp], " << "iResp = " << iResp << std::endl;



            std::cout << "\n ===> Unfold: " << variable << ", " << name[iSyst] << " for " << lepSel << " channel <===" << "\n";
            if(hRecDataMinusFakes->GetEntries() == 0){
                std::cerr << "Warning: histogram " << hRecDataMinusFakes->GetName() << " has no entries. Its unfolding will be skipped.\n";
                continue;
            }
            if (algo == "TUnfold") {
                //use the original hResDYJets histograms as the response matrix
                nIter[iSyst] = TUnfoldData(lepSel, algo, hResDYJets[iResp], hRecDataMinusFakes, hUnfData[iSyst], hUnfDataNoScale[iSyst],
                    hUnfDataStatCov[iSyst], hUnfDataStatCovNoScale[iSyst], hUnfMCStatCov[iSyst], hUnfMCStatCovNoScale[iSyst], name[iSyst], integratedLumi, hGenDYJets[iGen], logy, false);
            }
    


            // Save the hRecDataMinusFakes i.e. detector level histograms for building 
            // correlation matrix, before loop goes to next iSyst
            outputRootFile->cd();
            if (hRecDataMinusFakes){
                TString detecName = "hRecDataMinusFakes_"+name[iSyst];
                TString detecTitle = variable;
                detecTitle += "_detectorlevel";
                hRecDataMinusFakes->SetNameTitle(detecName, detecTitle);
                hRecDataMinusFakes->Write();
            }

        } // END loop over all systematic variations

        //-----------------------------------------------------------------------------------------
        
        // ALW -- do I need to keep this section???
        if (doNormalized) {
            std::cout << "doNormalized = " << doNormalized << std::endl;
            for(int i = 0; i < nSysts; i++) {
                if(!hUnfData[i]) continue;
                double totUnfData = hUnfData[i]->Integral("width"); // normalize to central or itself? 
                hUnfData[i]->Scale(1.0/totUnfData);
                if (i == 0) {
                    hUnfDataStatCov[0]->Scale(1.0/pow(totUnfData, 2));
                    hUnfMCStatCov[0]->Scale(1.0/pow(totUnfData, 2));
                }
                //the "NoScale" here refers to not scaling by lumi and bin width for xsec
                //we normalize the dist. the same here as with the original ones, however
                //original reson for NoScale dist. is to do ratio calculations, and we need integral number of counts in bins
                double totUnfDataNoScale = hUnfDataNoScale[i]->Integral("width");
                hUnfDataNoScale[i]->Scale(1.0/totUnfDataNoScale);
                if (i == 0) {
                    hUnfDataStatCovNoScale[0]->Scale(1.0/pow(totUnfDataNoScale, 2));
                    hUnfMCStatCovNoScale[0]->Scale(1.0/pow(totUnfDataNoScale, 2));
                }
            }
        }

        std::cout << "\n<=========================================================================================>" << std::endl;
        
        //--- Now create the covariance matrices ---
        int nCovs = nSysts > 17 ? 11 : 10;
        
        TH2D *hCov[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,};
        // std::cout <<  __FILE__ << ", "  << __LINE__ << std::endl;
 //     //andrew -- 16 jan 2019 -- problem with cov matrix settings
 //     if(hUnfDataStatCov[0]) {
 //         std::cout << "Cloning hUnfDataStatCov[0]" << std::endl;
 //         hCov[0] = (TH2D*) hUnfDataStatCov[0]->Clone("CovDataStat");
 //     }
 //     std::cout <<  __FILE__ << ", "  << __LINE__ << std::endl;
 //     if(hUnfMCStatCov[0]) {
 //         std::cout << "Cloning hUnfMCStatCov[0]" << std::endl;
 //         hCov[1] = (TH2D*) hUnfMCStatCov[0]->Clone("CovMCStat");
 //     }
 //     std::cout <<  __FILE__ << ", "  << __LINE__ << std::endl;
 //     if(hUnfData[1])  hCov[2] = makeCovFromUpAndDown(hUnfData[0], hUnfData[1], hUnfData[2], "CovJES");
 //     if(hUnfData[3])  hCov[3] = makeCovFromUpAndDown(hUnfData[0], hUnfData[3], hUnfData[4], "CovPU");
 //     if(hUnfData[5])  hCov[4] = makeCovFromUpAndDown(hUnfData[0], hUnfData[5], hUnfData[6], "CovJER");
 //     if(hUnfData[7])  hCov[5] = makeCovFromUpAndDown(hUnfData[0], hUnfData[7], hUnfData[8], "CovXSec");
 //     if(hUnfData[9])  hCov[6] = makeCovFromUpAndDown(hUnfData[0], hUnfData[9], hUnfData[10], "CovLES");
 //     if(hUnfData[11]) hCov[7] = makeCovFromUpAndDown(hUnfData[0], hUnfData[11], hUnfData[12], "CovLER");
 //     if(hUnfData[13]) hCov[8] = makeCovFromUpAndDown(hUnfData[0], hUnfData[13], hUnfData[14], "CovLumi");
 //     if(hUnfData[15]) hCov[9] = makeCovFromUpAndDown(hUnfData[0], hUnfData[15], hUnfData[16], "CovSF");
 //     if(hUnfData[17]) hCov[10] = makeCovFromUpAndDown(hUnfData[0], hUnfData[17], hUnfData[0], "CovSherpaUnf");
 //     if(hUnfMCStatCov[0]) hCov[11] = (TH2D*) hUnfMCStatCov[0]->Clone("CovTotSyst");
 //     
 //     std::cout <<  __FILE__ << ", "  << __LINE__ << std::endl;
 //    
 //     if(hCov[11]){
 //         for (int i = 2; i < nCovs; ++i){
 //             if(hCov[i]) hCov[11]->Add(hCov[i]);
 //         }
 //     }
 //       
 //       //andrew
 //       //Create covariance matrices not scaled for cross section
        TH2D *hCovNoScale[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,};
 //      if(hUnfDataStatCovNoScale[0]) hCovNoScale[0] = (TH2D*) hUnfDataStatCovNoScale[0]->Clone("CovDataStat_NoScale");
 //      if(hUnfMCStatCovNoScale[0]) hCovNoScale[1] = (TH2D*) hUnfMCStatCovNoScale[0]->Clone("CovMCStat_NoScale");
 //      if(hUnfDataNoScale[1])  hCovNoScale[2] = makeCovFromUpAndDown(hUnfData[0], hUnfDataNoScale[1], hUnfDataNoScale[2], "CovJES_NoScale");
 //      if(hUnfDataNoScale[3])  hCovNoScale[3] = makeCovFromUpAndDown(hUnfData[0], hUnfDataNoScale[3], hUnfDataNoScale[4], "CovPU_NoScale");
 //      if(hUnfDataNoScale[5])  hCovNoScale[4] = makeCovFromUpAndDown(hUnfData[0], hUnfDataNoScale[5], hUnfDataNoScale[6], "CovJER_NoScale");
 //      if(hUnfDataNoScale[7])  hCovNoScale[5] = makeCovFromUpAndDown(hUnfData[0], hUnfDataNoScale[7], hUnfDataNoScale[8], "CovXSec_NoScale");
 //      if(hUnfDataNoScale[9])  hCovNoScale[6] = makeCovFromUpAndDown(hUnfData[0], hUnfDataNoScale[9], hUnfDataNoScale[10], "CovLES_NoScale");
 //      if(hUnfDataNoScale[11]) hCovNoScale[7] = makeCovFromUpAndDown(hUnfData[0], hUnfDataNoScale[11], hUnfDataNoScale[12], "CovLER_NoScale");
 //      if(hUnfDataNoScale[13]) hCovNoScale[8] = makeCovFromUpAndDown(hUnfData[0], hUnfDataNoScale[13], hUnfDataNoScale[14], "CovLumi_NoScale");
 //      if(hUnfDataNoScale[15]) hCovNoScale[9] = makeCovFromUpAndDown(hUnfData[0], hUnfDataNoScale[15], hUnfDataNoScale[16], "CovSF_NoScale");
 //      if(hUnfDataNoScale[17]) hCovNoScale[10] = makeCovFromUpAndDown(hUnfData[0], hUnfDataNoScale[17], hUnfDataNoScale[0], "CovSherpaUnf_NoScale");
 //      if(hUnfMCStatCovNoScale[0]) hCovNoScale[11] = (TH2D*) hUnfMCStatCovNoScale[0]->Clone("CovTotSyst_NoScale");
 //         
 //      if(hCovNoScale[11]){
 //          for (int i = 2; i < nCovs; ++i){
 //              if(hCovNoScale[i]) hCovNoScale[11]->Add(hCovNoScale[i]);
 //          }
 //      }
 //   
 //      std::cout <<  __FILE__ << ", "  << __LINE__ << std::endl;

 //     if (doNormalized) {
 //         double Madtot = hMadGenCrossSection->Integral("width");
 //         //double gen1tot = hGen1CrossSection->Integral("width");
 //         //double gen2tot = hGen2CrossSection->Integral("width");
 //         hMadGenCrossSection->Scale(1.0/Madtot);
 //         //hGen1CrossSection->Scale(1.0/gen1tot);    
 //         //hGen2CrossSection->Scale(1.0/gen2tot);
 //     }

 //     
 //     //TCanvas *crossSectionPlot = makeCrossSectionPlot(lepSel, variable, doNormalized, hUnfData[0], hCov[11], hMadGenCrossSection, hGen1CrossSection, hGen2CrossSection);
 //       std::cout <<  __FILE__ << ", "  << __LINE__ << std::endl;
 //     //TCanvas *crossSectionPlot = makeCrossSectionPlot(lepSel, variable, doNormalized, hUnfData[0], hCov[11], hMadGenCrossSection, hGen1CrossSection);  
 //       TCanvas *crossSectionPlot;
 //       if (variable == "FirstJetPt_Zinc1jet" || variable == "JetsHT_Zinc1jet" || variable == "FirstJetAbsRapidity_Zinc1jet" || variable == "dRptmin100LepCloseJetCo300dR04_Zinc1jet" || variable == "dPhiLepJet1_Zinc1jet"){
 //           crossSectionPlot = makeCrossSectionPlot(lepSel, variable, doNormalized, hUnfData[0], hCov[11], hMadGenCrossSection, hGen1CrossSection, hGen2CrossSection);
 //       }
 //       else {
 //           crossSectionPlot = makeCrossSectionPlot(lepSel, variable, doNormalized, hUnfData[0], hCov[11], hMadGenCrossSection, hGen1CrossSection);
 //       }  //add nnlo here

 //       std::cout <<  __FILE__ << ", "  << __LINE__ << std::endl;

 //     //TCanvas *crossSectionPlot = makeCrossSectionPlot(lepSel, variable, doNormalized, hUnfData[0], hCov[11], hMadGenCrossSection); 
 //     crossSectionPlot->Draw();
 //     crossSectionPlot->SaveAs(outputFileName + ".png");
 //     crossSectionPlot->SaveAs(outputFileName + ".pdf");
 //     crossSectionPlot->SaveAs(outputFileName + ".eps");
 //     crossSectionPlot->SaveAs(outputFileName + ".ps");
 //     crossSectionPlot->SaveAs(outputFileName + ".C");
 //     crossSectionPlot->SaveAs(outputFileName + "_canvas.root");

        //Plot response matrix for central distribution
        plotRespMat(hResDYJets[0], variable, unfoldDir, 0, hGenDYJets[0]);
        //if #recobins = 2 * #genbins, then generate alternate plot of response matrix (done for TUnfold)
        if (variable.Contains("_2_Zinc")){
            plotRespMat(hResDYJets[0], variable, unfoldDir, 1, hGenDYJets[0]);
        }
        
    //      if(whichSyst < 0){
    //          createSystPlots(outputFileName, variable, lepSel, hUnfData, logy);
    //          //--- print out break down of errors ---
    //          for (int i = 2; i <= nCovs; ++i) {
    //              cout << hUnfData[0]->GetBinContent(i);
    //              for (int j = 0; j <= 11; ++j) {
    //                  if(hCov[j]){
    //                  cout << " +/- " << sqrt(hCov[j]->GetBinContent(i,i))*100./hUnfData[0]->GetBinContent(i) << "%";
    //                  }
    //               }
    //               cout << endl;
    //           }
    //           createTable(outputFileName, lepSel, variable, doNormalized, hUnfData[0], hCov);
    //      }
    //      
    //      
    //      if (variable.Index("ZNGoodJets_Zexc") >= 0 || variable.Index("ZNGoodJetsFull_Zexc") >= 0) {
    //      //createInclusivePlots(doNormalized, outputFileName, lepSel, hUnfData[0], hCov, hMadGenCrossSection, hGen1CrossSection, hGen2CrossSection);
    //        createInclusivePlots(doNormalized, outputFileName, lepSel, hUnfData[0], hCov, hMadGenCrossSection, hGen1CrossSection);
    //      //createInclusivePlots(doNormalized, outputFileName, lepSel, hUnfData[0], hCov, hMadGenCrossSection);
    //      }
    //
    //      //--------------------------------------
    //

        std::cout << "\n<=========================================================================================>" << std::endl;

        //--- Save other things --- 
        outputRootFile->cd();
        std::cout <<  __FILE__ << ", "  << __LINE__ << std::endl;
        hRecData[0]->Write("hRecDataCentral"); //reco data before BG subtraction and fakes removal
        hRecSumBg[0]->Write("hRecSumBgCentral"); //sum of all BGs to be subtracted from reco data
        hRecDYJets[0]->Write("hRecDYJetsCentral"); //reco MC for W+jets signal
        hGenDYJets[0]->Write("hGenDYJetsCentral"); //gen MC for W+jets NLO FxFx signal
        hMadGenCrossSection->Write("hMadGenDYJetsCrossSection"); //gen MC for W+jets NLO FxFx signal (also)
        hGen1CrossSection->Write("hGen1DYJetsCrossSection"); //gen MC for W+jets LO MLM signal
        //hGen2CrossSection->Write("hGen2DYJetsCrossSection");
        hResDYJets[0]->Write("hResDYJetsCentral"); //hResDYJets is the TH2 used to form response objects respDYJets
        std::cout <<  __FILE__ << ", "  << __LINE__ << std::endl;
      //write out unfolded histograms
    //    for (unsigned short iSyst = 0; iSyst < nSysts; ++iSyst) {
    //        outputRootFile->cd();
    //        if(hUnfData[iSyst]) hUnfData[iSyst]->Write();
    //        if(hUnfDataNoScale[iSyst]) hUnfDataNoScale[iSyst]->Write();
    //    }
        std::cout <<  __FILE__ << ", "  << __LINE__ << std::endl;       
    //      //write out covariance matrices (includes stat. for central (data and MC) and systematics from "up-down" dist.)
    //      for (int i = 0; i <= 11; ++i) {
    //          if(hCov[i]) hCov[i]->Write();
    //          if(hCovNoScale[i]) hCovNoScale[i]->Write();
    //      }
    //        
    //      TH1D *h_TotalUncer = (TH1D*) hUnfData[0]->Clone();
    //      for (int i = 1; i <= hUnfData[0]->GetNbinsX(); ++i) {
    //          h_TotalUncer->SetBinContent(i, sqrt(hCov[0]->GetBinContent(i,i) + hCov[11]->GetBinContent(i,i)));
    //          h_TotalUncer->SetBinError(i, 0.);
    //      }
    //      h_TotalUncer->Write("totalUncertainty");
    //        
    //      TParameter<double> pIntegratedLumi("integratedLumi", integratedLumi);
    //      pIntegratedLumi.Write();
    //      TParameter<int> pNIter("nIter", nIter[0]);
    //      pNIter.Write();
    //
    //      //crossSectionPlot is the canvas showing the main/central unfolded dist. (hUnfData[0])
    //      crossSectionPlot->Write();
    //      //----------------------------------------------------------------------------------------- 
    //
        std::cout <<  __FILE__ << ", "  << __LINE__ << std::endl;
        outputRootFile->Close();
      
      //if (end == start + 1) system("display " + outputFileName + ".png &");
      //if (end == start + 1 && variable == "ZNGoodJets_Zexc") system("display " + outputFileName.ReplaceAll("ZNGoodJets_Zexc", "ZNGoodJets_Zinc") + ".png &");
      
    } // END loop over all variables to unfold

    //--- Close all files ----------------------------------------------------------------------
    closeAllFiles(fData, fDYJets, fBg, NBGDYJETS);
    if(fSheUnf) fSheUnf->Close();
    if(fGen1) fGen1->Close();
    if(fGen2) fGen2->Close();
    //------------------------------------------------------------------------------------------ 

}

int TUnfoldData(const TString lepSel, const TString algo, TH2D* resp, TH1D* hRecDataMinusFakes, TH1D* &hUnfData, TH1D* &hUnfDataNoScale, 
TH2D* &hUnfDataStatCov, TH2D* &hUnfDataStatCovNoScale, TH2D* &hUnfMCStatCov, TH2D* &hUnfMCStatCovNoScale, TString name, double integratedLumi, TH1D* hGenDYJets, 
bool logy, bool doRatioUnfold){

    printf("=========================================================================\n");
    printf("Beginning TUnfoldData function...\n");
    printf("Name of systematic: %s\n", name.Data());
    std::cout << "----------------------------" << std::endl;

    // Create output file ---
    TString variable = TString(hRecDataMinusFakes->GetName());
    TString unfoldOutputDir = "UnfoldingCheck";
    if (doRatioUnfold) unfoldOutputDir += "Ratio";
    unfoldOutputDir += "/";
    system("mkdir -p "+unfoldOutputDir);
    TFile *f = new TFile(unfoldOutputDir + lepSel + "_" + variable + "_" + name + ".root", "RECREATE");
    f->cd();

    // ---------------------------------------------------------------------------

    // BG subtracted, fakes removed from reco data distribution
    TH1D * recoData = (TH1D*) hRecDataMinusFakes->Clone(); 
    TH1D * genDY = (TH1D*) hGenDYJets->Clone();
    TH2D * responseMatrix = (TH2D*) resp->Clone();

    // ----- RECO DISTRIBUTION ----- 
    printf("\n >>>>> recoData->GetNbinsX=%d\n", recoData->GetNbinsX());
    double recoEventCounter = 0.;
    for(int i = 1; i <= recoData->GetNbinsX(); i++){
        recoEventCounter += recoData->GetBinContent(i);
        printf("Bin %i, (%F, %F): %F\n", i, recoData->GetBinLowEdge(i), recoData->GetXaxis()->GetBinUpEdge(i), recoData->GetBinContent(i));
    }
    printf("#Events in input reco dist.: %F\n", recoEventCounter);

    // ----- GEN DISTRIBUTION ----- 
    printf("\n >>>>> genDY->GetNbinsX=%d\n", genDY->GetNbinsX());
    double genEventCounter = 0.;
    for(int i = 1; i <= genDY->GetNbinsX(); i++){
        genEventCounter += genDY->GetBinContent(i);
        printf("Bin %i, (%F, %F): %F\n", i, genDY->GetBinLowEdge(i), genDY->GetXaxis()->GetBinUpEdge(i), genDY->GetBinContent(i));
    }
    printf("#Events in input gen dist.: %F\n", genEventCounter);

    // ----- RESPONSE MATRIX ----- 
    int nBinsRespX = responseMatrix->GetNbinsX();
    int nBinsRespY = responseMatrix->GetNbinsY();
    printf("\n >>>>> responseMatrix->GetNbinsX=%d, responseMatrix->GetNbinsY=%d\n", nBinsRespX, nBinsRespY);
    // Add non-reconstructed fiducial events (of gen origin) in underflow reco bins
    // (b/c response matrix is filled with events that pass gen and reco cuts)
    // this is necessary for proper normalization of response matrix within TUnfold
    printf(" --- Normalizing response matrix taking non-reconstructed gen events into account...\n");
    std::vector<double> dataEfficiency;
    for(int i=1; i <= nBinsRespY; i++) { // GEN
        double genBin = genDY->GetBinContent(i);
        double recBin = 0;
        for(int j=1; j <= nBinsRespX+1; j++) { // RECO (include overflow)
            recBin += responseMatrix->GetBinContent(j,i);
        }
        //printf("Gen bin content = %F, Reco bin content (integrated over gen row) = %F, Efficiency = %F\n",genBin,recBin,recBin/genBin);
        dataEfficiency.push_back(recBin/genBin);
        //if(genBin-recBin <= 0) printf("Bin %d totally efficient (%f/%f/%f)\n",i,genBin,recBin,genBin-recBin);
        //if(i==1) printf("Bin %d totally efficient (%f/%f/%f)\n",i,genBin,recBin,genBin-recBin);
        printf(" - Efficiency of reconstruction for gen bin #%d: %F\n", i, dataEfficiency[i-1]);
        printf("Before: %F  After: %F\n",responseMatrix->GetBinContent(0,i),TMath::Max(genBin-recBin,0.0));
        responseMatrix->SetBinContent(0,i,TMath::Max(genBin-recBin,0.0));
    }
    //Set some gen overflow bins to 0 in response matrix:
    for(int i=1; i<=responseMatrix->GetNbinsX(); i++) {
        responseMatrix->SetBinContent(i, responseMatrix->GetNbinsY()+1, 0.0);
    }
    //and the reco overflow bins to 0 as well
    for(int i=1; i<=responseMatrix->GetNbinsY(); i++) {
        responseMatrix->SetBinContent(responseMatrix->GetNbinsX()+1, i, 0.0);
    }

  //  //print overflow and underflow bins
  //  std::cout << "Printing response matrix bin content: " << std::endl;
  //  printf("responseMatrix->GetNbinsX=%d, responseMatrix->GetNbinsY=%d\n", nBinsRespX, nBinsRespY);
  //  for(int i=0; i < nBinsRespX+2; i++){
  //      for(int j=0; j < nBinsRespY+2; j++){
  //          printf("Value in Bin(%i,%i): %f\n", i, j, responseMatrix->GetBinContent(i,j));
  //      }
  //  }
    
    //This should be done in the unfolding (check on this), keeping for reference.
    /*
    for(int i=1;i<=recoData->GetNbinsX();i++){
  printf("recoData->GetBinContent(%d)=%F -> recoData->GetBinContent(%d)=%F\n",i,
         recoData->GetBinContent(i), i, recoData->GetBinContent(i)/dataEfficiency[i-1]);
  recoData->SetBinContent(i,recoData->GetBinContent(i)/dataEfficiency[i-1]);
    }
    */
    
    recoData->Write();
    genDY->Write();
    responseMatrix->Write();

    // ---------------------------------------------------------------------------
    
    printf("\n-----> Doing the unfolding \n");
    TUnfold::ERegMode regMode = TUnfold::kRegModeCurvature;
    TUnfold::EConstraint constraintMode = TUnfold::kEConstraintArea; // preserve the area (constrain)
    TUnfoldDensity::EDensityMode densityFlags = TUnfoldDensity::kDensityModeBinWidth;
    //Define constructor and use SetInput to input the measurement that is to be unfolded
    TUnfoldDensity unfold(responseMatrix, TUnfold::kHistMapOutputVert, regMode, constraintMode, densityFlags);
    unfold.SetInput(recoData);

    //--- Manual unfolding ---
    //Double_t tau=1.E-4;
    Double_t tau=0.0;
    // Double_t biasScale=0.0;
    //--- ---

    int regParam_=-3;
    //int regParam_=0;
    //int regParam_=1;
    int iBest=regParam_;

    Int_t nScan=30;
    //Int_t nScan=100;
    TSpline *logTauX,*logTauY;
    TSpline *rhoLogTau=0;
    TGraph *lCurve=0;
    
    if (regParam_ < 0){
        printf("\n-----> Doing ScanLcurve method\n");
        // this method scans the parameter tau and finds the kink in the L curve
        //   // finally, the unfolding is done for the best choice of tau
        iBest = unfold.ScanLcurve(nScan,0.,0.,&lCurve,&logTauX,&logTauY);
        //iBest=unfold.ScanTau(nScan,0.,0.,&rhoLogTau,
        //           TUnfoldDensity::kEScanTauRhoMax);
    }
    else if(regParam_ > 0){
        printf("\n-----> Doing ScanTau method\n");
        iBest=unfold.ScanTau(50,0.0,0.0,&rhoLogTau,TUnfoldDensity::kEScanTauRhoAvg);
    }
    else{
        printf("\n-----> Doing manual DoUnfold method\n");
        std::cout << "manually set tau value = " << tau << std::endl;
        unfold.DoUnfold(tau);
    }
    
    if(lCurve) {
        Double_t t[1],x[1],y[1];
        TGraph *bestLcurve;
        logTauX->GetKnot(iBest,t[0],x[0]);
        logTauY->GetKnot(iBest,t[0],y[0]);
        bestLcurve = new TGraph(1,x,y);
        printf("Writing out L-curve plot...\n");
        TCanvas *canLCurve = new TCanvas("LCurveScan", "LCurveScan", 500, 500);
        canLCurve->cd();
        lCurve->Draw("AL");
        bestLcurve->SetMarkerColor(kRed);
        bestLcurve->Draw("*");
        canLCurve->Write();
    }
    Double_t tauFinal = unfold.GetTau();
    printf("Final value of tau*10^8: %f\n", tauFinal*100000000.);
    
    // ---------------------------------------------------------------------------

    // Get output from unfolding ---
    hUnfData = (TH1D*)unfold.GetOutput("UnfData_"+name)->Clone();
    hUnfData->SetTitle(hRecDataMinusFakes->GetTitle());
    hUnfData->GetXaxis()->SetTitle(hRecDataMinusFakes->GetXaxis()->GetTitle());
    hUnfData->GetYaxis()->SetTitle(hRecDataMinusFakes->GetYaxis()->GetTitle());
    printf("\nUnfolded data:");
    printf("\n >>>>> hUnfData->GetNbinsX=%d\n", hUnfData->GetNbinsX());
    double unfEventCounter = 0;
    for(int i = 1; i <= hUnfData->GetNbinsX(); i++){
        unfEventCounter += hUnfData->GetBinContent(i);
        printf("Bin %i, (%F, %F): %F\n", i, hUnfData->GetBinLowEdge(i), hUnfData->GetXaxis()->GetBinUpEdge(i), hUnfData->GetBinContent(i));
    }
    printf("#Events in unfolded dist.: %F\n", unfEventCounter);
    hUnfDataNoScale = (TH1D*) hUnfData->Clone("UnfData_"+name+"_NoScale");
    hUnfDataNoScale->Write();

    //Get covariance contribution from the input uncertainties (data statistical uncertainties) 
    hUnfDataStatCov = (TH2D*)unfold.GetEmatrixInput("dataStatCov", "dataStatCov", 0, 0, kTRUE)->Clone(); //Last 3 inputs are default;
    hUnfDataStatCov->SetName("UnfDataStatCov_" + name);
    hUnfDataStatCov->SetTitle(hRecDataMinusFakes->GetTitle());
    hUnfDataStatCovNoScale = (TH2D*) hUnfDataStatCov->Clone("UnfDataStatCov_"+name+"_NoScale");
    hUnfDataStatCovNoScale->Write();
    
    //Retreive covariance contribution from uncorrelated (statistical) uncertainties of the response matrix. 
    //Equivalent to RooUnfold::Ereco(RooUnfold::kCovariance) with IncludeSystematics(2). 
    hUnfMCStatCov = (TH2D*)unfold.GetEmatrixSysUncorr("respStatCov", "respStatCov", 0, 0, kTRUE)->Clone(); //Last 3 inputs are default;
    hUnfMCStatCov->SetName("UnfMCStatCov_" + name);
    hUnfMCStatCov->SetTitle(hRecDataMinusFakes->GetTitle());
    hUnfMCStatCovNoScale = (TH2D*) hUnfMCStatCov->Clone("UnfMCStatCov_"+name+"_NoScale");
    hUnfMCStatCovNoScale->Write();

    printf("\nChi2A = %F\n",unfold.GetChi2A());
    printf("Number of degrees of freedom = %d\n",unfold.GetNdf());
    //TH2D *probabilityMatrix = (TH2D*)unfold.GetProbabilityMatrix("probabilityMatrix")->Clone();
    //probabilityMatrix->Write();
    TH2D *corrCoeffMatrix = (TH2D*)unfold.GetRhoIJtotal("corrCoeffMatrix")->Clone();
    corrCoeffMatrix->Write();

    TH1D *foldedData = (TH1D*)unfold.GetFoldedOutput("foldedData");
    //printf("---> Input data:\n");
    //for(int i=1;i<=recoData->GetNbinsX();i++){
    //    printf("bin %i: %F\n",i,recoData->GetBinContent(i));
    //}
    if(foldedData){
        foldedData->Write();
        //printf("---> Folded result:\n");
        //for(int i=1;i<=foldedData->GetNbinsX();i++){
        //    printf("bin %i: %F\n",i,foldedData->GetBinContent(i));
        //}
        printf("\n >>>>> Ratio of RecoData/Folded:\n"); 
        TH1D *foldedClosureRatio = (TH1D*) recoData->Clone();
        foldedClosureRatio->SetName("foldedClosureRatio");
        foldedClosureRatio->SetTitle("foldedClosureRatio");
        foldedClosureRatio->GetXaxis()->SetTitle(hRecDataMinusFakes->GetXaxis()->GetTitle());
        foldedClosureRatio->GetYaxis()->SetTitle("RecoData/Folded");
        foldedClosureRatio->Divide(foldedData);
        for(int i = 1; i <= foldedData->GetNbinsX(); i++){
            printf("Bin %i: %F\n", i, foldedClosureRatio->GetBinContent(i));
        }
        foldedClosureRatio->Write();
    }

    // Get Unfolded/Reco to quantify the effects of the unfolding ---
    // 1) Have to first merge the split bins of the reco-level distribution
    // to match the binning of the unfolded dist (which should be the same as gen-level)

    //set bin content of this new histo using bin contents from original
    TH1D *recoDataNominal = (TH1D*) recoData->Clone();
    int numBinsXRecoNominal = recoDataNominal->GetNbinsX();

    //create the resized histo using binning info from unfolded dist
    int numBinsUnf =  hUnfData->GetNbinsX();
    // double xminUnf = hUnfData->GetXaxis()->GetXmin();
    // double xmaxUnf = hUnfData->GetXaxis()->GetXmax();
    Double_t xBinsUnf[numBinsUnf+1]; //array of bin edges from unfolded dist.
    for (int i(1); i < numBinsUnf + 2; i++){ //use low bin edge of overflow bin for upper bin edge of last real bin
        double binlowUnf = hUnfData->GetBinLowEdge(i);
        // double binhighUnf = hUnfData->GetXaxis()->GetBinUpEdge(i);
        xBinsUnf[i-1] = binlowUnf;
    }

    // create new reco-level histogram with binning of unfolded distribution
    // logic below only works because the num. of reco bins is even
    // because #reco bins = 2 * #gen bins...
    TH1D *recoData_binsMerged = new TH1D("", "", numBinsUnf, xBinsUnf);
    int newHistoCounter = 1;
    for (int i(1); i < numBinsXRecoNominal + 1; i += 2){
        double bincount1 = recoDataNominal->GetBinContent(i);
        double bincount2 = recoDataNominal->GetBinContent(i+1);
        recoData_binsMerged->SetBinContent(newHistoCounter, (bincount1 + bincount2));
        newHistoCounter++;
    }

    // 2) Then take the ratio
    if(hUnfData){
        printf("\nRatio of Unfolded/RecoData:"); 
        TH1D *UnfRecoRatio = (TH1D*) hUnfData->Clone();
        UnfRecoRatio->Divide(recoData_binsMerged);
        printf("\n >>>>> UnfRecoRatio->GetNbinsX=%d\n", UnfRecoRatio->GetNbinsX());
        for(int i = 1; i <= UnfRecoRatio->GetNbinsX(); i++){
            printf("Bin %i: %F\n", i, UnfRecoRatio->GetBinContent(i));
        }
        UnfRecoRatio->SetName("unfRecoRatio");
        UnfRecoRatio->SetTitle("unfRecoRatio");
        UnfRecoRatio->GetXaxis()->SetTitle(hRecDataMinusFakes->GetXaxis()->GetTitle());
        UnfRecoRatio->GetYaxis()->SetTitle("Unfolded/Reco");
        // set y-axis bounds between 0.5 and 1.5...
        UnfRecoRatio->Write();
    }

    // ---------------------------------------------------------------------------
    
    //For single-diff. xsecs, need to scale unfolded event counts by int. lumi and bin widths to get xsec
    if (!doRatioUnfold){
        //--- divide by luminosity ---
        double lumiUnc = cfg.getD("lumiUnc", 0.027);
        printf("\nScaling data %s by int. lumi = %F... \n",hUnfData->GetName(), integratedLumi);
        hUnfData->Scale(1./integratedLumi);
        if(hUnfDataStatCov) hUnfDataStatCov->Scale(1./(integratedLumi*integratedLumi));
        if(hUnfMCStatCov) hUnfMCStatCov->Scale(1./(integratedLumi*integratedLumi));
        if ("LumiUp" == name) {
            printf("With lumiUnc = %F\n",lumiUnc);
            hUnfData->Scale(1./(1+lumiUnc));
            if(hUnfDataStatCov) hUnfDataStatCov->Scale(1./((1+lumiUnc)*(1+lumiUnc)));
            if(hUnfMCStatCov) hUnfMCStatCov->Scale(1./((1+lumiUnc)*(1+lumiUnc)));
        }
        else if ("LumiDown" == name) {
            printf("With lumiUnc = %F\n",lumiUnc);
            hUnfData->Scale(1./(1-lumiUnc));
            if(hUnfDataStatCov) hUnfDataStatCov->Scale(1./((1-lumiUnc)*(1-lumiUnc)));
            if(hUnfMCStatCov) hUnfMCStatCov->Scale(1./((1-lumiUnc)*(1-lumiUnc)));
        }
        //--- divide by bin width to get cross section ---
        printf("Normalizing to bin widths to get cross section...\n");
        int nBins = hUnfData->GetNbinsX();
        for (int i = 1; i <= nBins; ++i) {
            double binWidth = hUnfData->GetBinWidth(i);
            hUnfData->SetBinContent(i, hUnfData->GetBinContent(i)*(1./binWidth));
            hUnfData->SetBinError(i, hUnfData->GetBinError(i)*(1./binWidth));
            for (int j = 1; j <= nBins; ++j) {
                double binWidth2 = hUnfData->GetBinWidth(j);
                if(hUnfDataStatCov){
                    hUnfDataStatCov->SetBinContent(i, j, hUnfDataStatCov->GetBinContent(i, j)*(1./(binWidth*binWidth2)));
                    hUnfDataStatCov->SetBinError(i, j, hUnfDataStatCov->GetBinError(i, j)*(1./(binWidth*binWidth2)));
                }
                if(hUnfMCStatCov){
                    hUnfMCStatCov->SetBinContent(i, j, hUnfMCStatCov->GetBinContent(i, j)*(1./(binWidth*binWidth2)));
                    hUnfMCStatCov->SetBinError(i, j, hUnfMCStatCov->GetBinError(i, j)*(1./(binWidth*binWidth2)));
                }
            }
        }
    }

    hUnfData->Write();
    hUnfDataStatCov->Write();
    hUnfMCStatCov->Write();

    // ---------------------------------------------------------------------------

    f->Close();
    printf("\nExiting TUnfoldData function...");
    printf("\n=========================================================================\n");
    return 0;
}

void createSystPlots(TString outputFileName, TString variable, TString lepSel, TH1D *hUnfData[], bool logy){

  // 0 - Central, 
  // 1 - JES up, 2 - JES down, 
  // 3 - PU up, 4 - PU down, 
  // 5 - JER up, 6 - JER down, 
  // 7 - XSEC up, 8 - XSEC down
  // 9 - LES up, 10 - LES down
  // 11 - LER up, 12 - LER down
  // 13 - Lumi up, 14 - Lumi down
  // 15 - SF up, 16 - SF down
  // 17 - SherpaUnf
  TString syst[] = {"JES", "PU", "JER", "XSec", "LES", "LER", "Lumi", "S.F."};
  //TString syst[] = {"JES", "PU", "JER", "XSec", "BtagSF", "LER", "Lumi", "S.F."};
  for (int i = 1; i < 16; i += 2) {

    TH1D *hCent = (TH1D*) hUnfData[0]->Clone();
    hCent->SetMarkerColor(kBlack);
    hCent->SetMarkerStyle(20);
    if (variable == "ZNGoodJets_Zexc") hCent->GetXaxis()->SetRangeUser(1, 8);
    if (variable.Index("JetPt_Zinc") >= 0) hCent->GetXaxis()->SetRangeUser(30, hCent->GetXaxis()->GetXmax());
    hCent->GetXaxis()->SetLabelSize(0);
    hCent->GetYaxis()->SetTitle("d#sigma");
    hCent->GetYaxis()->SetTitleSize(0.05);
    hCent->GetYaxis()->SetTitleOffset(0.9);

    TH1D *hUp = (TH1D*) hUnfData[i]->Clone();
    hUp->SetLineColor(kGreen+2);
    hUp->SetLineWidth(2);
    TH1D *hDown = (TH1D*) hUnfData[i+1]->Clone();
    hDown->SetLineColor(kBlue);
    hDown->SetLineWidth(2);

    TCanvas *c = new TCanvas(variable + " - " + syst[i/2], variable + " - " + syst[i/2], 700, 900);
    c->cd();

    TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1);
    pad1->SetTopMargin(0.11);
    pad1->SetBottomMargin(0.02);
    pad1->SetRightMargin(0.03);
    pad1->SetLeftMargin(0.15);
    pad1->SetTicks();
    pad1->SetLogy(logy ? kTRUE : kFALSE);
    pad1->Draw();
    pad1->cd();

    TLegend *leg = new TLegend(0.8, 0.7, 0.95, 0.86);
    leg->SetBorderSize(0);
    leg->SetFillStyle(0);
    hCent->DrawCopy("e");
    hUp->DrawCopy("samehist");
    hDown->DrawCopy("samehist");
    leg->AddEntry(hUp, syst[i/2] + " Up", "l");
    leg->AddEntry(hCent, "Central", "lp");
    leg->AddEntry(hDown, syst[i/2] + " Down", "l");
    leg->Draw();
    pad1->Draw();
    fixYscale();
    c->cd();

    TPad *pad2 = new TPad("pad2", "pad2", 0, 0, 1, 0.3);
    pad2->SetTopMargin(0.02);
    pad2->SetBottomMargin(0.3);
    pad2->SetRightMargin(0.03);
    pad2->SetLeftMargin(0.15);
    pad2->SetGridy();
    pad2->SetTicks();
    pad2->Draw();
    pad2->cd();
    hUp->Divide(hCent);
    hDown->Divide(hCent);
    hCent->Divide(hCent);

    double maxRatio = max(max(hCent->GetMaximum(), hUp->GetMaximum()), hDown->GetMaximum());
    double minRatio = min(min(hCent->GetMinimum(), hUp->GetMinimum()), hDown->GetMinimum());
    int nBins = hCent->GetNbinsX();
    for (int j = 1; j <= nBins; j++) {
        maxRatio = max(maxRatio, 1+hCent->GetBinError(j));
        minRatio = min(minRatio, 1-hCent->GetBinError(j));
    }
    hCent->GetYaxis()->SetRangeUser(minRatio-0.02*(maxRatio-minRatio), maxRatio+0.02*(maxRatio-minRatio));
    hCent->GetYaxis()->SetRangeUser(0.82, 1.18);
    if (variable == "ZNGoodJets_Zexc") hCent->GetXaxis()->SetRangeUser(1, 8);
    hCent->SetTitle(" ");
    hCent->GetYaxis()->SetTitle("Ratio to Central");
    hCent->GetYaxis()->SetTitleSize(0.08);
    hCent->GetYaxis()->SetTitleOffset(0.9);
    hCent->GetYaxis()->CenterTitle();
    hCent->GetYaxis()->SetLabelSize(0.08);
    hCent->GetYaxis()->SetLabelOffset(0.014);
    hCent->GetXaxis()->SetTitleSize(0.13);
    //hCent->GetXaxis()->SetLabelSize(0.13);
    hCent->GetXaxis()->SetLabelSize(0.08);
    hCent->GetXaxis()->SetLabelOffset(0.012);

    hCent->DrawCopy("e");
    hUp->Draw("histsame");
    hDown->Draw("histsame");
    c->cd();
    c->Update();
    c->Draw();

    TString systStr = syst[i/2];
    if (systStr == "S.F.") systStr = "SF";
    system("mkdir SystPlot");
    c->SaveAs("SystPlot/" + lepSel + "_" + variable + "_" + systStr + ".png");
    c->SaveAs("SystPlot/" + lepSel + "_" + variable + "_" + systStr + ".ps");
    c->SaveAs("SystPlot/" + lepSel + "_" + variable + "_" + systStr + ".eps");
    c->SaveAs("SystPlot/" + lepSel + "_" + variable + "_" + systStr + ".pdf");
    c->SaveAs("SystPlot/" + lepSel + "_" + variable + "_" + systStr + ".C");
    c->SaveAs("SystPlot/" + lepSel + "_" + variable + "_" + systStr + ".root");
    }
}

void createInclusivePlots(bool doNormalized, TString outputFileName, TString lepSel, TH1D *hUnfData, TH2D *hCov[], TH1D *hMadGenCrossSection, TH1D *hSheGenCrossSection){

    TH1D *hInc = (TH1D*) hUnfData->Clone("ZNGoodJets_Zinc");
    TH1D *hIncMad = (TH1D*) hMadGenCrossSection->Clone("ZNGoodJets_Zinc_Mad");
    TH1D *hIncShe = (TH1D*) hSheGenCrossSection->Clone("ZNGoodJets_Zinc_She");
    TH2D *hCovInc[12] = {NULL};
    if(hCov[0]) hCovInc[0] = (TH2D*) hCov[0]->Clone("CovDataStat");
    if(hCov[1]) hCovInc[1] = (TH2D*) hCov[1]->Clone("CovMCStat");
    if(hCov[2]) hCovInc[2] = (TH2D*) hCov[2]->Clone("CovJES");
    if(hCov[3]) hCovInc[3] = (TH2D*) hCov[3]->Clone("CovPU");
    if(hCov[4]) hCovInc[4] = (TH2D*) hCov[4]->Clone("CovJER");
    if(hCov[5]) hCovInc[5] = (TH2D*) hCov[5]->Clone("CovXSec");
    if(hCov[6]) hCovInc[6] = (TH2D*) hCov[6]->Clone("CovLES");
    if(hCov[7]) hCovInc[7] = (TH2D*) hCov[7]->Clone("CovLER");
    if(hCov[8]) hCovInc[8] = (TH2D*) hCov[8]->Clone("CovLumi");
    if(hCov[9]) hCovInc[9] = (TH2D*) hCov[9]->Clone("CovSF");
    if(hCov[10]) hCovInc[10] = (TH2D*) hCov[10]->Clone("CovSherpaUnf");
    else hCovInc[10] = 0;
    if(hCov[11]) hCovInc[11] = (TH2D*) hCov[11]->Clone("CovTotSyst");

    int nBins = hInc->GetNbinsX();
    for (int i = 1; i <= nBins; i++) {
    double binSum = 0;
    double binSumMad = 0;
    double binSumShe = 0;
    double binStatError2 = 0;
    double binStatMadError2 = 0;
    double binStatSheError2 = 0;
    double binCov[12] = {0};
    for (int j = i; j <= nBins; j++) {
        binSum += hInc->GetBinContent(j);
        binSumMad += hIncMad->GetBinContent(j);
        binSumShe += hIncShe->GetBinContent(j);
        binStatError2 += pow(hInc->GetBinError(j), 2);
        binStatMadError2 += pow(hIncMad->GetBinError(j), 2);
        binStatSheError2 += pow(hIncShe->GetBinError(j), 2);
        for (int k = 0; k < 12; k++) {
          if(hCovInc[k]) binCov[k] += hCovInc[k]->GetBinContent(j, j);
        }
    }
    hInc->SetBinContent(i, binSum);
    hIncMad->SetBinContent(i, binSumMad);
    hIncShe->SetBinContent(i, binSumShe);
    hInc->SetBinError(i, sqrt(binStatError2));
    hIncMad->SetBinError(i, sqrt(binStatMadError2));
    hIncShe->SetBinError(i, sqrt(binStatSheError2));
    for (int k = 0; k < 12; k++) {
      if(hCovInc[k]) hCovInc[k]->SetBinContent(i, i, binCov[k]);
    }
    }

    TCanvas *crossSectionPlot = makeCrossSectionPlot(lepSel, TString("ZNGoodJets_Zinc"), doNormalized, hInc, hCovInc[11], hIncMad, hIncShe);
    outputFileName.ReplaceAll("_Zexc", "_Zinc");
    crossSectionPlot->Draw();
    crossSectionPlot->SaveAs(outputFileName + ".png");
    crossSectionPlot->SaveAs(outputFileName + ".pdf");
    crossSectionPlot->SaveAs(outputFileName + ".eps");
    crossSectionPlot->SaveAs(outputFileName + ".ps");
    crossSectionPlot->SaveAs(outputFileName + ".C");
    crossSectionPlot->SaveAs(outputFileName + "_canvas.root");
    createTable(outputFileName, lepSel, TString("ZNGoodJets_Zinc"), doNormalized, hInc, hCovInc);
}

void createTable(TString outputFileName, TString lepSel, TString variable, bool doNormalized, TH1D *hUnfData, TH2D *hCov[]){
    cout << "Hello" << endl;    
    //--- print out break down of errors ---
    TString title = hUnfData->GetTitle();
    int nBins = hUnfData->GetNbinsX();
    TString var = "";
    TString dSigma = "";
    TString xtitle = hUnfData->GetXaxis()->GetTitle();
    createTitleVariableAnddSigma(variable, doNormalized, xtitle, title, var, dSigma);
    cout << "Title: " << title << endl;
    cout << "Var: " << var << endl;
    cout << "dSig: " << dSigma << endl;
    cout << "nBins: " << nBins << endl;

    TString table = "\\begin{table}[htb!]\n\\begin{center}\n";
    table += "\\caption{Differential cross section in " + title;
    table += " and break down of the systematic uncertainties for the ";
    if (lepSel == "DMu") table += "muon decay channel.}\n";
    if (lepSel == "DE") table += "electron decay channel.}\n";
    table += "\\scriptsize{\n";
    table += "\\begin{tabular}{c|cc|ccccccccc}\n";
    table += "\\multicolumn{12}{c}{" + title + "} \\\\\n";
    table += var + " & " + dSigma + " & \\tiny{Tot. Unc [\\%]} & ";
    table += "\\tiny{stat [\\%]} & \\tiny{MC stat [\\%]} & \\tiny{JES [\\%]} & \\tiny{JER [\\%]} & ";
    table += "\\tiny{PU [\\%]} & \\tiny{XSEC [\\%]} & \\tiny{Lumi [\\%]} & ";
    //table += "\\tiny{LES [\\%]} & \\tiny{LER [\\%]} & ";
    table += "\\tiny{BtagSF [\\%]} & \\tiny{LER [\\%]} & ";
    table += "\\tiny{Unf [\\%]} & \\tiny{Eff [\\%]} \\\\\\hline\n";

    int start = 1;
    if (title.Index("multiplicity", 0, TString::ECaseCompare::kIgnoreCase) >= 0) {
        start = 1; 
        //nBins--;
    }
    if (title.Index("jet $p_{\\text{T}}$", 0, TString::ECaseCompare::kIgnoreCase) >= 0) start = 3; 

    for (int i = start; i <= nBins; ++i) {
    double xs = hUnfData->GetBinContent(i);
    TString numbers;
    if (title.Index("exclusive jet multiplicity", 0,  TString::ECaseCompare::kIgnoreCase) >= 0) {
        numbers.Form("= %d", i - 1);
    }
    else if (title.Index("inclusive jet multiplicity", 0,  TString::ECaseCompare::kIgnoreCase) >= 0) {
        numbers.Form("$\\geq$ %d", i - 1);
    }
    else {
        numbers.Form("$%g \\ -\\ %g$", hUnfData->GetBinLowEdge(i), hUnfData->GetBinLowEdge(i+1));
    }
    table += numbers + " & "; 
    numbers.Form("%#.3g", xs);
    table += numbers + " & ";
    // total uncertainty
    if(hCov[11]) numbers.Form("%#.2g", sqrt(hCov[0]->GetBinContent(i,i) + hCov[11]->GetBinContent(i,i))*100./xs);
    else numbers="N/A";
    table += numbers + " & ";
    // stat uncertainty
    if(hCov[0]) numbers.Form("%#.2g", sqrt(hCov[0]->GetBinContent(i,i))*100./xs);
    else numbers="N/A";
    table += numbers + " & ";
    // MC stat uncertainty
    if(hCov[1]) numbers.Form("%#.2g", sqrt(hCov[1]->GetBinContent(i,i))*100./xs);
    else numbers="N/A";
    table += numbers + " & ";
    // JES uncertainty
    if(hCov[2]) numbers.Form("%#.2g", sqrt(hCov[2]->GetBinContent(i,i))*100./xs);
    else numbers="N/A";
    table += numbers + " & ";
    // JER uncertainty
    if(hCov[4]) numbers.Form("%#.2g", sqrt(hCov[4]->GetBinContent(i,i))*100./xs);
    else numbers="N/A";
    table += numbers + " & ";
    // PU uncertainty
    if(hCov[3]) numbers.Form("%#.2g", sqrt(hCov[3]->GetBinContent(i,i))*100./xs);
    else numbers="N/A";
    table += numbers + " & ";
    // XSec uncertainty
    if(hCov[5]) numbers.Form("%#.2g", sqrt(hCov[5]->GetBinContent(i,i))*100./xs);
    else numbers="N/A";
    table += numbers + " & ";
    // Lumi uncertainty
    if(hCov[8]) numbers.Form("%#.2g", sqrt(hCov[8]->GetBinContent(i,i))*100./xs);
    else numbers="N/A";
    table += numbers + " & ";
    // LES uncertainty
    if(hCov[6]) numbers.Form("%#.2g", sqrt(hCov[6]->GetBinContent(i,i))*100./xs);
    else numbers="N/A";
    table += numbers + " & ";
    // LER uncertainty
    if(hCov[7]) numbers.Form("%#.2g", sqrt(hCov[7]->GetBinContent(i,i))*100./xs);
    else numbers="N/A";
    table += numbers + " & ";
    if (hCov[10]){
      // Unf uncertainty
      numbers.Form("%#.2g", sqrt(hCov[10]->GetBinContent(i,i))*100./xs);
      table += numbers + " & ";
    }
    // SF uncertinaty
    if(hCov[9]) numbers.Form("%#.2g", sqrt(hCov[9]->GetBinContent(i,i))*100./xs);
    else numbers="N/A";
    table += numbers + " \\\\\n";
    }

    table += "\\end{tabular}}\n";
    table += "\\label{tab:" + lepSel + variable + "}\n";
    table += "\\end{center}\\end{table}\n";
    ofstream out(outputFileName + ".tex");
    out << table;
    out.close();
    if(verbosity > 0) cout << table << endl;
}

TH2D* makeCovFromUpAndDown(const TH1D* hUnfDataCentral, const TH1D* hUnfDataUp, const TH1D* hUnfDataDown, TString name){
    int nBins = hUnfDataCentral->GetNbinsX();
    TH2D* h = new TH2D(name, name, nBins, 0, nBins, nBins, 0, nBins);

    for (int i = 1; i <= nBins; ++i) {
        double sigmaMeani = 0.5*fabs(hUnfDataUp->GetBinContent(i) - hUnfDataDown->GetBinContent(i)); 
        if (name.Index("Sherpa") >= 0) sigmaMeani *= 2;
        int signi = (hUnfDataUp->GetBinContent(i) - hUnfDataDown->GetBinContent(i) < 0) ? -1 : 1;
        if (i > 1 && i < nBins) {
            if ((hUnfDataUp->GetBinContent(i-1) - hUnfDataDown->GetBinContent(i-1)) * (hUnfDataUp->GetBinContent(i+1) - hUnfDataDown->GetBinContent(i+1)) < 0) {
                sigmaMeani = 0.5*(0.5*fabs(hUnfDataUp->GetBinContent(i-1) - hUnfDataDown->GetBinContent(i-1)) + 0.5*fabs(hUnfDataUp->GetBinContent(i+1) - hUnfDataDown->GetBinContent(i+1)));
                if (name.Index("Sherpa") >= 0) sigmaMeani *= 2;
            }
        }

        for (int j = 1; j <= nBins; ++j) {
            double sigmaMeanj = 0.5*fabs(hUnfDataUp->GetBinContent(j) - hUnfDataDown->GetBinContent(j)); 
            if (name.Index("Sherpa") >= 0) sigmaMeanj *= 2;
            int signj = (hUnfDataUp->GetBinContent(j) - hUnfDataDown->GetBinContent(j) < 0) ? -1 : 1;
            if (j > 1 && j < nBins) {
                if ((hUnfDataUp->GetBinContent(j-1) - hUnfDataDown->GetBinContent(j-1)) * (hUnfDataUp->GetBinContent(j+1) - hUnfDataDown->GetBinContent(j+1)) < 0) {
                    sigmaMeanj = 0.5*(0.5*fabs(hUnfDataUp->GetBinContent(j-1) - hUnfDataDown->GetBinContent(j-1)) + 0.5*fabs(hUnfDataUp->GetBinContent(j+1) - hUnfDataDown->GetBinContent(j+1)));
                    if (name.Index("Sherpa") >= 0) sigmaMeanj *= 2;
                }
            }

            double correlation = (i == j) ? 1 : 1;
            h->SetBinContent(i, j, correlation * signi * signj * sigmaMeani * sigmaMeanj);

        }//end second loop over nBins
    } //end first loop over nBins

    return h;
}

TH1D* makeCrossSectionHist(TH1D* hGenDYJets, double integratedLumi){
    TH1D *hGenCrossSection = (TH1D*) hGenDYJets->Clone();
    
    if (integratedLumi > 0){
        hGenCrossSection->Scale(1./integratedLumi);
        
        int nBins = hGenCrossSection->GetNbinsX();
        for (int i = 1; i <= nBins; ++i) {
            double binWidth = hGenCrossSection->GetBinWidth(i);
            hGenCrossSection->SetBinContent(i, hGenCrossSection->GetBinContent(i)*1./binWidth);
            hGenCrossSection->SetBinError(i, hGenCrossSection->GetBinError(i)*1./binWidth);
        }
    }

    return hGenCrossSection;
}

void RemoveFakes(TH1* hRecData, TH1* hFakes, TH1* hPurity){
    int fakeMethod = cfg.getI("fakeMethod");
    static bool emitMess = true;
    if(emitMess){
        std::cout << "Fake subtraction method id: " << fakeMethod << std::endl;;
        emitMess = false;
    }
    switch(fakeMethod){
    case 0: //global rescale to data
            std::cout << "fakeMethod = " << fakeMethod << " --> Just subtracting the hFakes histo from the hRecData histo" << std::endl;
            hRecData->Add(hFakes, -1);
        return;
    case 1: //bin-by-bin purity method
        std::cout << "fakeMethod = " << fakeMethod << " --> Doing bin-by-bin purity method " << std::endl;
        for(int ibin = 0; ibin <= hRecData->GetNbinsX(); ++ibin){
            double c = hRecData->GetBinContent(ibin);
            double e = hRecData->GetBinError(ibin);
            double p = hPurity->GetBinContent(ibin);
            hRecData->SetBinContent(ibin, c * p);
            hRecData->SetBinError(ibin, e * p);
        }
        return;
    default:
        std::cerr << __FILE__ <<  ":" << __LINE__ << ". Value " << fakeMethod
              << " for parameter fakeMethod " << " is not supported. Exists.\n";
        exit(1);
    }
}

void plotRespMat(TH2D *hResp, TString variable, TString unfoldDir, bool addSwitch, TH1D *hGenDYJets){
    //Generates a pdf of the TH2 response matrix used for creating the RooUnfoldResponse object that goes into unfolding   
    //This TH2 is related to the hresponse histos   
    
    gStyle->SetOptStat(0);
    gStyle->SetPaintTextFormat("4.0f");
    gStyle->SetHistMinimumZero();   
    //gStyle->SetPalette(57); //kBird
    //gStyle->SetPalette(53); //kDarkBodyRadiator

    TString outputFileName = unfoldDir;
    outputFileName += "ResponseMatrix_" + variable;
    TH2D *hNormResp;

    if(addSwitch == 1){ //if we have the #recobins = 2 * #genbins (for TUnfold)
        outputFileName += "_alt";

        //set bin content of this new histo using bin contents from original response matrix
        TH2D *hRespNominal = (TH2D*) hResp->Clone();
        int numBinsXRespNominal = hRespNominal->GetNbinsX();
        int numBinsYRespNominal = hRespNominal->GetNbinsY();

        //get condition number for matrix before normalization
        TMatrixD respMatrix(hRespNominal->GetNbinsX(), hRespNominal->GetNbinsY());
        for(int i = 1; i <= hRespNominal->GetNbinsX(); i++){
            for(int j = 1; j <= hRespNominal->GetNbinsY(); j++){
                respMatrix[i-1][j-1] = hRespNominal->GetBinContent(i,j);
            }
        }
        TDecompSVD svdResp(respMatrix);
        svdResp.Decompose(); //do the single value decomposition
        TVectorD svdRespVector = svdResp.GetSig(); //returns diagonal elements of diagonal matrix S
        //printf("Condition # = %F / %F = %F\n", svdRespVector[0], svdRespVector[svdRespVector.GetNoElements()-1], svdRespVector[0]/svdRespVector[svdRespVector.GetNoElements()-1]);  

        normalizeTH2D(hRespNominal);

        //create the resized response matrix using binning info from gen distribution
        std::cout << "Creating re-sized response matrix to understand magnitude of bin migrations!" << std::endl;
        int numBinsGen =  hGenDYJets->GetNbinsX();
        // double xminGen = hGenDYJets->GetXaxis()->GetXmin();
        // double xmaxGen = hGenDYJets->GetXaxis()->GetXmax();
        //  printf("\nhGenDYJets Binning --\n");
        //  printf("numBinsGen = %d, xminGen = %F, xmaxGen = %F\n", numBinsGen, xminGen, xmaxGen);
        Double_t xBinsGen[numBinsGen+1];
        for (int i(1); i < numBinsGen + 2; i++){ //use low bin edge of overflow bin for upper bin edge of last real bin
            // double binlowGen = hGenDYJets->GetBinLowEdge(i);
            // double binhighGen = hGenDYJets->GetXaxis()->GetBinUpEdge(i);
            //      printf("Bin %d: %F to %F\n", i, binlowGen, binhighGen);
            //      if(i==numBinsGen + 1) {
            //          printf("^^^^^last one is overflow bin\n");
            //      }
            xBinsGen[i-1] = hGenDYJets->GetBinLowEdge(i);
        }
        //        std::cout << "hGenDYJets Binning" << std::endl;
        //        for(int i(0); i < numBinsGen + 1; i++){
        //            std::cout << xBinsGen[i] << " ";
        //        }
        //        std::cout << std::endl;


        hNormResp = new TH2D("", "", numBinsGen, xBinsGen, numBinsGen, xBinsGen);
        int newHistoCounter = 1;
        for (int i(1); i < numBinsXRespNominal + 1; i+=2){ //x loop over RECO
            for (int j(1); j < numBinsYRespNominal + 1; j++){ //y loop over GEN
                double bincount1 = hRespNominal->GetBinContent(i,j);
                double bincount2 = hRespNominal->GetBinContent(i+1,j);
                double bincountAdded = bincount1 + bincount2;
                hNormResp->SetBinContent(newHistoCounter,j,bincountAdded);
                // printf("Setting bin content for bin(%d,%d) = %F\n",newHistoCounter,j,bincountAdded);
            }
            newHistoCounter++;
        }
        TString titleHisto = hRespNominal->GetTitle();
        titleHisto += "_alt";
        TString nameHisto = hRespNominal->GetTitle();
        nameHisto += "_alt";
        TString YTitle = hRespNominal->GetYaxis()->GetTitle();
        // YTitle = "Generated_" + YTitle;
        YTitle = "Generated";
        TString XTitle = hRespNominal->GetYaxis()->GetTitle();
        // XTitle = "Reconstructed_" + XTitle;
        XTitle = "Reconstructed";
        hNormResp->SetTitle(titleHisto);
        hNormResp->SetName(nameHisto);
        hNormResp->GetYaxis()->SetTitle(YTitle);
        hNormResp->GetXaxis()->SetTitle(XTitle);
        hNormResp->GetZaxis()->SetTitle("");
        hNormResp->GetYaxis()->SetTitleSize(0.04);
        hNormResp->GetYaxis()->SetTitleOffset(1.10);
        hNormResp->GetXaxis()->SetTitleSize(0.04);
        hNormResp->GetXaxis()->SetTitleOffset(1.10);   
    }

    else{ //the nominal case
        hNormResp = (TH2D*) hResp->Clone();
    
        //get condition number for matrix before normalization
        std::cout << "\nGetting condition number from response matrix!" << std::endl;
        TMatrixD respMatrix(hResp->GetNbinsX(), hResp->GetNbinsY());
        for(int i=1; i<=hResp->GetNbinsX();i++){
            for(int j=1; j<=hResp->GetNbinsY();j++){
                respMatrix[i-1][j-1] = hResp->GetBinContent(i,j);
            }
        }
        TDecompSVD svdResp(respMatrix);
        svdResp.Decompose(); //do the single value decomposition
        TVectorD svdRespVector = svdResp.GetSig(); //returns diagonal elements of diagonal matrix S
        std::cout << "Elements of diagonal matrix S: ";
        for(int i=0; i < svdRespVector.GetNoElements(); i++){
            std::cout << svdRespVector[i] << " ";
        }
        std::cout << std::endl;
        printf("Condition # = %F / %F = %F\n", svdRespVector[0], svdRespVector[svdRespVector.GetNoElements()-1], svdRespVector[0]/svdRespVector[svdRespVector.GetNoElements()-1]);  

        normalizeTH2D(hNormResp);
        
        TString YTitle = hNormResp->GetYaxis()->GetTitle();  
        // YTitle = "Generated_" + YTitle;
        YTitle = "Generated";
        TString XTitle = hNormResp->GetYaxis()->GetTitle();  
        // XTitle = "Reconstructed_" + XTitle;
        XTitle = "Reconstructed";
        
        hNormResp->GetYaxis()->SetTitle(YTitle);
        hNormResp->GetYaxis()->SetTitleSize(0.04);
        hNormResp->GetYaxis()->SetTitleOffset(1.10);
        hNormResp->GetXaxis()->SetTitle(XTitle);
        hNormResp->GetXaxis()->SetTitleSize(0.04);
        hNormResp->GetXaxis()->SetTitleOffset(1.10);
        hNormResp->GetZaxis()->SetTitle("");
    }
    
    TCanvas *canRespMatrix = new TCanvas("s", "s", 600, 600);
    canRespMatrix->cd();

    TPad *pad1 = new TPad("a", "a", 0.05, 0.05, 0.95, 0.9);
    //    pad1->SetTopMargin(0.11);
    //    pad1->SetBottomMargin(0);
    pad1->SetRightMargin(0.15);
    //    pad1->SetLeftMargin(0.11);
    pad1->SetTicks();
    pad1->Draw();
    pad1->cd();
    //pad1->SetLogz();
    if (variable != "ZNGoodJetsFull_Zexc"){
        pad1->SetLogy();
        pad1->SetLogx();
    }
    pad1->cd();

    hNormResp->SetMinimum(0.);
    hNormResp->SetMaximum(100.);
    //hNormResp->SetContour(90);
    //    hNormResp->DrawCopy();
    hNormResp->DrawCopy("COLZ2");
    //hNormResp->DrawCopy("LEGO2Z 0");
    //hNormResp->DrawCopy("hist");
    hNormResp->DrawCopy("TEXT SAME");
    pad1->Draw();   
    canRespMatrix->Print(outputFileName + ".pdf");
    delete canRespMatrix;
}

void normalizeTH2D(TH2D *h){
    //printf("--- Normalizing TH2 by y-axis row! ---\n");
    int xbin(h->GetNbinsX()), ybin(h->GetNbinsY());
    for (int i(0); i <= ybin + 1; i++){
        double sum(0.);
        for (int j(0); j <= xbin + 1; j++){
            sum += h->GetBinContent(j,i);
        }
        for (int j(0); j <= xbin + 1; j++){
            if (sum != 0) h->SetBinContent(j, i, h->GetBinContent(j, i)*100 / sum );
        }
    }
}
