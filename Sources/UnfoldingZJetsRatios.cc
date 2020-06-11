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
#include "UnfoldingZJetsRatios.h"
#include "PlotSettings.h"
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

using namespace std;

void UnfoldingZJetsRatios(TString lepSel, int year, TString algo, TString histoDir, TString unfoldDir, int jetPtMin, int jetEtaMax, TString variableNUM, TString variableDENOM, bool doNormalized, int whichSyst){
    gStyle->SetOptStat(0);

    std::cout << "\n----- Begin ratio unfolding! -----" << std::endl;
    cout << "\nProcessing numerator variable: " << variableNUM << endl;
    cout << "Processing denominator variable: " << variableDENOM << endl;

    //--- Get input histogram directory for specific year ---
    histoDir += "_";
    histoDir += year;
    
    //--- Create output directory if does not exist ---
    unfoldDir += "Ratio";
    unfoldDir += "_";
    unfoldDir += year;
    system("mkdir -p " + unfoldDir);

    double integratedLumi(0.);
    if (year == 2016) integratedLumi = 35916.982;
    else if (year == 2017) integratedLumi = 41524.945;
    else integratedLumi = 59717.255;

    std::cout << "\n year = " << year << std::endl;
    std::cout << " integratedLumi = " << integratedLumi << std::endl; 

    std::cout << "\n<=========================================================================================>" << std::endl;

    //--- Here we declare the different arrays of TFiles (nominal/central and systematic variations)
    TFile *fData[3] = {NULL}; 
    TFile *fDYJets[9] = {NULL};
    TFile *fBg[NBGDYJETS][7] = {{NULL}};

    //--- Open all files!
    std::cout << "\nOpening all files from reco-level analysis code..." << std::endl;
    getAllFiles(histoDir, lepSel, "13TeV", jetPtMin, jetEtaMax, fData, fDYJets, fBg, NBGDYJETS);

    //--- Open theory files!
    TFile *fGen1 = NULL;
    TString gen1;
    TString gen1File;
    std::map<TString, vector<TString> > generatorNames;
    if (DYMGPYTHIA8FILENAME.Length() > 0 ){
        vector<TString> mgpythia8;
        mgpythia8.push_back(DYMGPYTHIA8FILENAME);
        mgpythia8.push_back(DYMGPYTHIA8LEGEND);
        
        generatorNames["mgpythia8"] = mgpythia8;
        gen1 = "mgpythia8";
        
        gen1File = histoDir + "/" + lepSel + "_13TeV_" + generatorNames[gen1][0] + "_dR_TrigCorr_1_Syst_0_JetPtMin_";
        gen1File += jetPtMin;
        gen1File += "_JetEtaMax_";
        gen1File += jetEtaMax;
        gen1File += ".root";
        std::cout << "Opening file with name: " << gen1File << std::endl;
        fGen1 = new TFile(gen1File);
    }
    else std::cout << "No filename for fGen1 specified! " << std::endl;

    //--- Create output file  
    TString outputFileName = unfoldDir + "/" + lepSel; 
    outputFileName += "_RATIO_" + variableNUM + "_TO_" + variableDENOM;
    outputFileName += "_JetPtMin_";
    outputFileName += jetPtMin;
    outputFileName += "_JetEtaMax_";
    outputFileName += jetEtaMax;
    outputFileName += "_MGPYTHIA6_";
    outputFileName += doNormalized ? "_normalized" : "";   
    TFile *outputRootFile = new TFile(outputFileName + ".root", "RECREATE");

    std::cout << "\n<=========================================================================================>" << std::endl;

    // NUMERATOR HISTOGRAMS
    std::cout << "\n----- Opening all histograms for NUMERATOR! ----- " << std::endl;
    //--- rec Data histograms ---
    TH1D *hRecDataNUM[3] = {NULL};
    //--- rec DYJets histograms ---
    TH1D *hRecDYJetsNUM[13] = {NULL};
    //--- gen DYJets histograms ---
    TH1D *hGenDYJetsNUM[11] = {NULL};
    //--- res DYJets histograms ---
    TH2D *hResDYJetsNUM[13] = {NULL};
    //--- "fakes" DYJets histograms ---
    TH1D *hFakDYJetsNUM[13] = {NULL};
    //--- "misses" DYJets histograms" ---
    TH1D *hMissDYJetsNUM[11] = {NULL};
    //--- rec Bg histograms ---
    TH1D *hRecBgNUM[NBGDYJETS][11] = {{NULL}};
    //--- rec Sum Bg histograms ---
    TH1D *hRecSumBgNUM[11] = {NULL};
    //--- Get all numerator histograms ---
    //note: andrew -- ttbar scaling done inside getAllHistos
    std::cout << "\nGrabbing all needed histograms from files opened above for... " << variableNUM << std::endl;
    getAllHistos(variableNUM, hRecDataNUM, fData, hRecDYJetsNUM, hGenDYJetsNUM, hResDYJetsNUM, fDYJets, 
        hRecBgNUM, hRecSumBgNUM, fBg, NBGDYJETS, hFakDYJetsNUM, hMissDYJetsNUM);

    // Theory histograms for numerator
    TH1D *hGen1NUM = NULL;
    if (DYMGPYTHIA8FILENAME.Length() > 0 ) hGen1NUM = getHisto(fGen1, "gen" + variableNUM);

    // DENOMINATOR HISTOGRAMS
    std::cout << "\n----- Opening all histograms for DENOMINATOR! ----- " << std::endl;
    //--- rec Data histograms ---
    TH1D *hRecDataDENOM[3] = {NULL};
    //--- rec DYJets histograms ---
    TH1D *hRecDYJetsDENOM[13] = {NULL};
    //--- gen DYJets histograms ---
    TH1D *hGenDYJetsDENOM[11] = {NULL};
    //--- res DYJets histograms ---
    TH2D *hResDYJetsDENOM[13] = {NULL};
    //--- "fakes" DYJets histograms ---
    TH1D *hFakDYJetsDENOM[13] = {NULL};
    //--- "misses" DYJets histograms ---
    TH1D *hMissDYJetsDENOM[11] = {NULL};
    //--- rec Bg histograms ---
    TH1D *hRecBgDENOM[NBGDYJETS][11] = {{NULL}};
    //--- rec Sum Bg histograms ---
    TH1D *hRecSumBgDENOM[11] = {NULL};
    //--- Get all denominator histograms ---
    //note: andrew -- ttbar scaling done inside getAllHistos
    std::cout << "\nGrabbing all needed histograms from files opened above for... " << variableDENOM << std::endl;
    getAllHistos(variableDENOM, hRecDataDENOM, fData, hRecDYJetsDENOM, hGenDYJetsDENOM, hResDYJetsDENOM, fDYJets, 
        hRecBgDENOM, hRecSumBgDENOM, fBg, NBGDYJETS, hFakDYJetsDENOM, hMissDYJetsDENOM);

    // Theory histograms for denominator
    TH1D *hGen1DENOM = NULL;
    if (DYMGPYTHIA8FILENAME.Length() > 0 ) hGen1DENOM = getHisto(fGen1, "gen" + variableDENOM);

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

    int nSysts = 18;
    std::cout << "\nnSysts = " << nSysts << std::endl;
    TString name[] = {"Central", "JESUp", "JESDown", "PUUp", "PUDown", "JERUp", "JERDown", "XSECUp", "XSECDown", 
         "blank1", "blank2", "BtagSFUp", "BtagSFDown", "LumiUp", "LumiDown", "SFUp", "SFDown", "blank5"};
    
    // unfolded distributions and matrices for numerator and denominator
    // numerator
    TH1D *hUnfDataNUM[18]                 = {}; 
    TH1D *hUnfDataNoScaleNUM[18]          = {};
    TH2D *hUnfDataStatCovNUM[18]          = {};
    TH2D *hUnfDataStatCovNoScaleNUM[18]   = {};
    TH2D *hUnfMCStatCovNUM[18]            = {};
    TH2D *hUnfMCStatCovNoScaleNUM[18]     = {};
    // denominator
    TH1D *hUnfDataDENOM[18]               = {};
    TH1D *hUnfDataNoScaleDENOM[18]        = {};
    TH2D *hUnfDataStatCovDENOM[18]        = {};
    TH2D *hUnfDataStatCovNoScaleDENOM[18] = {};
    TH2D *hUnfMCStatCovDENOM[18]          = {};
    TH2D *hUnfMCStatCovNoScaleDENOM[18]   = {};

    // reconstructed level distributions (split bins re-merged)
    TH1D *hRecDataMergedNUM[18]   = {}; 
    TH1D *hRecDataMergedDENOM[18] = {};

    bool logy = true;

    std::cout << "\n<=========================================================================================>" << std::endl;

    std::cout << "\n----- Unfolding NUMERATOR! ----- " << std::endl;
    std::cout << "Entering iSyst loop to unfold for Central and every variation for each systematic for... " << variableNUM << std::endl;
    std::cout << "nSysts = " << nSysts << std::endl;
    std::cout << "whichSyst = " << whichSyst << std::endl;
    for (int iSyst = 0; iSyst < nSysts; ++iSyst){
        std::cout << "\n >>>>>>>>>> Start iSyst = " << iSyst << "! <<<<<<<<<<" << std::endl;
        if(iSyst != 0 && whichSyst >= 0 && iSyst != whichSyst) continue;

        int iData(0), iBg(0), iGen(0), iResp(0); 
        unfoldingFileSelector(iSyst, iData, iBg, iGen, iResp);

        std::cout << "Fetching main reco data histo hRecDataNUM[iData], " << "iData = " << iData << std::endl;
        TH1D *hRecDataMinusFakesNUM = (TH1D*) hRecDataNUM[iData]->Clone();
        std::cout << "From reco data, subtracting MC BG histo hRecSumBgNUM[iBg], " << "iBg = " << iBg << std::endl;
        hRecDataMinusFakesNUM->Add(hRecSumBgNUM[iBg], -1);
        std::cout << "Removing fakes using hRecDYJetsNUM[iResp], hFakDYJetsNUM[iResp]!" << std::endl;
        // RemoveFakes(hRecDataMinusFakesNUM, hRecDYJetsNUM[iResp], hFakDYJetsNUM[iResp]); // ALW 3 JUNE 20 -- TEMPMORARILY COMMENTING OUT
        
        std::cout << "Fetching gen signal histo hGenDYJetsNUM[iGen] and misses histo hMissDYJetsNUM[iGen], " << "iGen = " << iGen << std::endl;
        std::cout << "Fetching response matrix histo hResDYJetsNUM[iResp], " << "iResp = " << iResp << std::endl;

        std::cout << "Starting unfolding of " << variableNUM << ", " << name[iSyst] << " for " << lepSel << " channel." << "\n";
        if(hRecDataMinusFakesNUM->GetEntries() == 0){
            std::cerr << "Warning: histogram " << hRecDataMinusFakesNUM->GetName() << " has no entries. Unfolding will be skipped.\n";
            continue;
        }

        if (algo == "TUnfold"){
            TUnfoldData(lepSel, algo, hResDYJetsNUM[iResp], hRecDataMinusFakesNUM, hRecDataMergedNUM[iSyst], hUnfDataNUM[iSyst], hUnfDataNoScaleNUM[iSyst],
                hUnfDataStatCovNUM[iSyst], hUnfDataStatCovNoScaleNUM[iSyst], hUnfMCStatCovNUM[iSyst], hUnfMCStatCovNoScaleNUM[iSyst], 
                name[iSyst], integratedLumi, hGenDYJetsNUM[iGen], hMissDYJetsNUM[iGen], logy, true, year);
        }
    }

    std::cout << "\n<=========================================================================================>" << std::endl;

    std::cout << "\n----- Unfolding DENOMINATOR! ----- " << std::endl;
    std::cout << "Entering iSyst loop to unfold for Central and every variation for each systematic for... " << variableDENOM << std::endl;
    std::cout << "nSysts = " << nSysts << std::endl;
    std::cout << "whichSyst = " << whichSyst << std::endl;
    for (int iSyst = 0; iSyst < nSysts; ++iSyst){
        std::cout << "\n >>>>>>>>>> Start iSyst = " << iSyst << "! <<<<<<<<<<" << std::endl;
        if(iSyst != 0 && whichSyst >= 0 && iSyst != whichSyst) continue;

        int iData(0), iBg(0), iGen(0), iResp(0); 
        unfoldingFileSelector(iSyst, iData, iBg, iGen, iResp);

        std::cout << "Fetching main reco data histo hRecDataDENOM[iData], " << "iData = " << iData << std::endl;
        TH1D *hRecDataMinusFakesDENOM = (TH1D*) hRecDataDENOM[iData]->Clone();
        std::cout << "From reco data, subtracting MC BG histo hRecSumBgDENOM[iBg], " << "iBg = " << iBg << std::endl;
        hRecDataMinusFakesDENOM->Add(hRecSumBgDENOM[iBg], -1);
        std::cout << "Removing fakes using hRecDYJetsDENOM[iResp], hFakDYJetsDENOM[iResp]!" << std::endl;
        // RemoveFakes(hRecDataMinusFakesDENOM, hRecDYJetsDENOM[iResp], hFakDYJetsDENOM[iResp]); // ALW 3 JUNE 20 -- TEMPMORARILY COMMENTING OUT
        
        std::cout << "Fetching gen signal histo hGenDYJetsDENOM[iGen] and misses histo hMissDYJetsDENOM[iGen], " << "iGen = " << iGen << std::endl;
        std::cout << "Fetching response matrix histo hResDYJetsDENOM[iResp], " << "iResp = " << iResp << std::endl;

        std::cout << "Starting unfolding of " << variableDENOM << ", " << name[iSyst] << " for " << lepSel << " channel." << "\n";
        if(hRecDataMinusFakesDENOM->GetEntries() == 0){
            std::cerr << "Warning: histogram " << hRecDataMinusFakesDENOM->GetName() << " has no entries. Unfolding will be skipped.\n";
            continue;
        }

        if (algo == "TUnfold"){
            TUnfoldData(lepSel, algo, hResDYJetsDENOM[iResp], hRecDataMinusFakesDENOM, hRecDataMergedDENOM[iSyst], hUnfDataDENOM[iSyst], hUnfDataNoScaleDENOM[iSyst],
                hUnfDataStatCovDENOM[iSyst], hUnfDataStatCovNoScaleDENOM[iSyst], hUnfMCStatCovDENOM[iSyst], hUnfMCStatCovNoScaleDENOM[iSyst], 
                name[iSyst], integratedLumi, hGenDYJetsDENOM[iGen], hMissDYJetsDENOM[iGen], logy, true, year);
        }
        
    }

    std::cout << "\n<=========================================================================================>" << std::endl;

    // Take ratio of separately-unfolded numerator and denominator
    std::cout << "\n----- Take ratio of unfolded & reco distributions! ----- " << std::endl;

    TH1D *hUnfDataRATIO[18]  = {};
    TH1D *hRecoDataRATIO[18] = {};

    outputRootFile->cd();
    for (unsigned short iSyst = 0; iSyst < nSysts; ++iSyst){  
        std::cout << "name = " << name[iSyst] << std::endl;
        // Write out unfolded ratios ---
        hUnfDataRATIO[iSyst] = (TH1D*) hUnfDataNUM[iSyst]->Clone();
        hUnfDataRATIO[iSyst]->Divide(hUnfDataNUM[iSyst], hUnfDataDENOM[iSyst], 1, 1, "B");
        hUnfDataRATIO[iSyst]->SetName("UnfXSecRatio_"+name[iSyst]);
        hUnfDataRATIO[iSyst]->SetTitle(variableNUM+"_TO_"+variableDENOM+"_"+name[iSyst]);
        hUnfDataRATIO[iSyst]->GetYaxis()->SetTitle("Ratio");
        hUnfDataRATIO[iSyst]->Write();
    }
    for (unsigned short iSyst = 0; iSyst < nSysts; ++iSyst){ 
        // Write out reco ratios ---
        hRecoDataRATIO[iSyst] = (TH1D*) hRecDataMergedNUM[iSyst]->Clone();
        hRecoDataRATIO[iSyst]->Divide(hRecDataMergedNUM[iSyst], hRecDataMergedDENOM[iSyst], 1, 1, "B");
        hRecoDataRATIO[iSyst]->SetName("RecoXSecRatio_"+name[iSyst]);
        hRecoDataRATIO[iSyst]->SetTitle(variableNUM+"_TO_"+variableDENOM+"_"+name[iSyst]);
        hRecoDataRATIO[iSyst]->GetYaxis()->SetTitle("Ratio");
        hRecoDataRATIO[iSyst]->Write();
    }

    // Write out GEN ratios ---
    // NLO FxFx (MG5)
    TH1D *hGenSignalNLOFxFxNUM = makeCrossSectionHist(hGenDYJetsNUM[0], integratedLumi);
    TH1D *hGenSignalNLOFxFxDENOM = makeCrossSectionHist(hGenDYJetsDENOM[0], integratedLumi);
    TH1D *hGenSignalNLOFxFxRATIO = (TH1D*) hGenSignalNLOFxFxNUM->Clone();
    hGenSignalNLOFxFxRATIO->Divide(hGenSignalNLOFxFxNUM, hGenSignalNLOFxFxDENOM, 1, 1, "B");
    hGenSignalNLOFxFxRATIO->SetName("GenSignalXSecRatio_NLOFxFx");
    hGenSignalNLOFxFxRATIO->SetTitle(variableNUM+"_TO_"+variableDENOM+"_GenSignalNLOFxFx");
    hGenSignalNLOFxFxRATIO->GetYaxis()->SetTitle("Ratio");
    hGenSignalNLOFxFxRATIO->Write();
    // LO MLM (Madgraph)
    TH1D *hGenSignalLOMLMNUM = makeCrossSectionHist(hGen1NUM, integratedLumi);
    TH1D *hGenSignalLOMLMDENOM = makeCrossSectionHist(hGen1DENOM, integratedLumi);
    TH1D *hGenSignalLOMLMRATIO = (TH1D*) hGenSignalLOMLMNUM->Clone();
    hGenSignalLOMLMRATIO->Divide(hGenSignalLOMLMNUM, hGenSignalLOMLMDENOM, 1, 1, "B");
    hGenSignalLOMLMRATIO->SetName("GenSignalXSecRatio_LOMLM");
    hGenSignalLOMLMRATIO->SetTitle(variableNUM+"_TO_"+variableDENOM+"_GenSignalLOMLM");
    hGenSignalLOMLMRATIO->GetYaxis()->SetTitle("Ratio");
    hGenSignalLOMLMRATIO->Write();



    std::cout << "\n<=========================================================================================>" << std::endl;



    //////////////////////////////////////////
    //
    // Part with the covariance matrices.....
    //
    //////////////////////////////////////////



    //Plot response matrices
    // Numerator ---
    plotRespMat(hResDYJetsNUM[0], variableNUM, unfoldDir, 0, hGenDYJetsNUM[0]);
    //if #recobins = 2 * #genbins, then generate alternate plot of response matrix (done for TUnfold)
    if (variableNUM.Contains("_TUnfold")){
        plotRespMat(hResDYJetsNUM[0], variableNUM, unfoldDir, 1, hGenDYJetsNUM[0]);
    }
    std::cout << std::endl;
    // Denominator ---
    plotRespMat(hResDYJetsDENOM[0], variableDENOM, unfoldDir, 0, hGenDYJetsDENOM[0]);
    //if #recobins = 2 * #genbins, then generate alternate plot of response matrix (done for TUnfold)
    if (variableDENOM.Contains("_TUnfold")){
        plotRespMat(hResDYJetsDENOM[0], variableDENOM, unfoldDir, 1, hGenDYJetsDENOM[0]);
    }
    std::cout << std::endl;


    std::cout << "\n<=========================================================================================>" << std::endl;

    outputRootFile->Close();
    closeAllFiles(fData, fDYJets, fBg, NBGDYJETS);
    if(fGen1) fGen1->Close();

    std::cout << "\nFinished!\n" << std::endl;
}
