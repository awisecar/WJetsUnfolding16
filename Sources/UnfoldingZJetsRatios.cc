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
#include <RooUnfoldBayes.h>
#include <RooUnfoldBinByBin.h>
#include <RooUnfoldSvd.h>
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
static int verbosity = 1;

using namespace std;

void UnfoldingZJetsRatios(TString lepSel, TString algo, TString histoDir, TString unfoldDir, int jetPtMin, int jetEtaMax, TString variableNUM, TString variableDENOM, bool doNormalized, int whichSyst)
{
    std::cout << "\n----- Begin ratio unfolding! -----" << std::endl;
    gStyle->SetOptStat(0);
    //--- Create output directory if does not exist ---
    unfoldDir = "UnfoldedFiles_Ratio/";
    system("mkdir -p " + unfoldDir);
    cout << "\nProcessing numerator variable: " << variableNUM << endl;
    cout << "Processing denominator variable: " << variableDENOM << endl;

    //--- Here we declare the different arrays of TFiles (nominal/central and systematic variations)
    TFile *fData[3] = {NULL}; 
    TFile *fDYJets[9] = {NULL};
    TFile *fBg[NBGDYJETS][7] = {{NULL}};
    //--- Open all files!
    std::cout << "\n----- Opening all files from reco-level analysis code! ----- " << std::endl;
    getAllFiles(histoDir, lepSel, "13TeV", jetPtMin, jetEtaMax, fData, fDYJets, fBg, NBGDYJETS);

    double integratedLumi(35916.625); // 80X 2016 data bonzai 23Sep2016ReReco golden json - andrew - 4.11.2018
    std::cout << "\nintegratedLumi = " << integratedLumi << std::endl; 

    //--- Create output file  
    TString outputFileName = unfoldDir + lepSel; 
    outputFileName += "_unfoldedRatio_" + variableNUM + "_" + variableDENOM + "_" + algo;
    outputFileName += "_JetPtMin_";
    outputFileName += jetPtMin;
    outputFileName += "_JetEtaMax_";
    outputFileName += jetEtaMax;
    outputFileName += "_MGPYTHIA6_";
    outputFileName += doNormalized ? "_normalized" : "";   
    TFile *outputRootFile = new TFile(outputFileName + ".root", "RECREATE");

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
    //--- rec Bg histograms ---
    TH1D *hRecBgNUM[NBGDYJETS][11] = {{NULL}};
    //--- rec Sum Bg histograms ---
    TH1D *hRecSumBgNUM[11] = {NULL};
    //--- fake DYJets histograms ---
    TH1D *hFakDYJetsNUM[18] = {NULL};
    //--- purity "histograms" ---
    TH1D *hPurityNUM[18] = {NULL};
    //--- response DYJets objects ---
    RooUnfoldResponse *respDYJetsNUM[18] = {NULL};
    //--- Get all numerator histograms ---
    //note: andrew -- ttbar scaling done inside getAllHistos
    getAllHistos(variableNUM, hRecDataNUM, fData, hRecDYJetsNUM, hGenDYJetsNUM, hResDYJetsNUM, fDYJets, 
        hRecBgNUM, hRecSumBgNUM, fBg, NBGDYJETS, respDYJetsNUM, hFakDYJetsNUM, hPurityNUM);

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
    //--- rec Bg histograms ---
    TH1D *hRecBgDENOM[NBGDYJETS][11] = {{NULL}};
    //--- rec Sum Bg histograms ---
    TH1D *hRecSumBgDENOM[11] = {NULL};
    //--- fake DYJets histograms ---
    TH1D *hFakDYJetsDENOM[18] = {NULL};
    //--- purity "histograms" ---
    TH1D *hPurityDENOM[18] = {NULL};
    //--- response DYJets objects ---
    RooUnfoldResponse *respDYJetsDENOM[18] = {NULL};
    //--- Get all denominator histograms ---
    //note: andrew -- ttbar scaling done inside getAllHistos
    getAllHistos(variableDENOM, hRecDataDENOM, fData, hRecDYJetsDENOM, hGenDYJetsDENOM, hResDYJetsDENOM, fDYJets, 
        hRecBgDENOM, hRecSumBgDENOM, fBg, NBGDYJETS, respDYJetsDENOM, hFakDYJetsDENOM, hPurityDENOM);

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
    TString name[] = {"Central", "JESUp", "JESDown", "PUUp", "PUDown", "JERUp", "JERDown", "XSECUp", "XSECDown", 
         "blank1", "blank2", "BtagSFUp", "BtagSFDown", "LumiUp", "LumiDown", "SFUp", "SFDown", "blank5"};

    int nSysts = 18;
    std::cout << "\n-----> nSysts = " << nSysts << std::endl;
    int nIter[18] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; 
    TH1D *hUnfData[18] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,};
    TH1D *hUnfDataNoScale[18] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,};
    TH2D *hUnfDataStatCov[18] =  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,};
    TH2D *hUnfDataStatCovNoScale[18] =  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,};
    TH2D *hUnfMCStatCov[18] =  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,};
    TH2D *hUnfMCStatCovNoScale[18] =  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,};
    
    bool logy = true;

    TH1D *hRecDataMinusFakesNUM[18] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,};
    TH1D *hRecDataMinusFakesDENOM[18] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,};

    // Subtract BG and fakes before taking ratio
    std::cout << "----- Looping over iSyst to subtract BG, fakes from data -----" << std::endl;
    for (unsigned short iSyst = 0; iSyst < nSysts; ++iSyst) {
        std::cout << "\n >>>>> iSyst = " << iSyst << std::endl;

        //--- This section selects the correct hRecData file and hRecSumBg files to use for background subtraction
        unsigned short iData = (iSyst == 1 || iSyst == 2) ? iSyst : 0;
        unsigned short iBg = 0;
        if (iSyst == 0 || iSyst == 1 || iSyst == 2 || iSyst == 5 || iSyst == 6 || iSyst == 9 || iSyst == 10 || iSyst == 17) iBg = 0;
        else if (iSyst == 3 || iSyst == 4) iBg = iSyst - 2;
        else if (iSyst == 7 || iSyst == 8) iBg = iSyst - 4;
        else if (iSyst == 11 || iSyst == 12 || iSyst == 13 || iSyst == 14 || iSyst == 15 || iSyst == 16) iBg = iSyst - 6;

        std::cout << " ----- Do BG & fakes removal for NUMERATOR ----- " << std::endl;
        std::cout << "Fetching main reco data histo hRecDataNUM[iData], " << "iData = " << iData << std::endl;
        hRecDataMinusFakesNUM[iSyst] = (TH1D*) hRecDataNUM[iData]->Clone();
        std::cout << "From reco data, subtracting MC BG histo hRecSumBgNUM[iBg], " << "iBg = " << iBg << std::endl;
        hRecDataMinusFakesNUM[iSyst]->Add(hRecSumBgNUM[iBg], -1);
        std::cout << "Removing fakes using hFakDYJetsNUM[iSyst], hPurityNUM[iSyst]!" << std::endl;
        RemoveFakes(hRecDataMinusFakesNUM[iSyst], hFakDYJetsNUM[iSyst], hPurityNUM[iSyst]);

        std::cout << " ----- Do BG & fakes removal for DENOMINATOR ----- " << std::endl;
        std::cout << "Fetching main reco data histo hRecDataDENOM[iData], " << "iData = " << iData << std::endl;
        hRecDataMinusFakesDENOM[iSyst] = (TH1D*) hRecDataDENOM[iData]->Clone();
        std::cout << "From reco data, subtracting MC BG histo hRecSumBgDENOM[iBg], " << "iBg = " << iBg << std::endl;
        hRecDataMinusFakesDENOM[iSyst]->Add(hRecSumBgDENOM[iBg], -1);
        std::cout << "Removing fakes using hFakDYJetsDENOM[iSyst], hPurityDENOM[iSyst]!" << std::endl;
        RemoveFakes(hRecDataMinusFakesDENOM[iSyst], hFakDYJetsDENOM[iSyst], hPurityDENOM[iSyst]);
    }

    /// divide input histos into ratio before unfolding
    TH1D *hRecDataMinusFakesRATIO[18] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,};
    for (unsigned short iSyst = 0; iSyst < nSysts; ++iSyst) {
        hRecDataMinusFakesRATIO[iSyst] = (TH1D*) hRecDataMinusFakesNUM[iSyst]->Clone();
        hRecDataMinusFakesRATIO[iSyst]->Divide(hRecDataMinusFakesNUM[iSyst], hRecDataMinusFakesDENOM[iSyst], 1, 1, "B");
        outputRootFile->cd();
        TString nameTemp = hRecDataMinusFakesNUM[iSyst]->GetName();
        nameTemp += "_";
        nameTemp += hRecDataMinusFakesDENOM[iSyst]->GetName();
        TString titleTemp = "Reco Ratio of "+variableNUM+" to "+variableDENOM+", "+name[iSyst];
        hRecDataMinusFakesRATIO[iSyst]->SetNameTitle(nameTemp, titleTemp);
        hRecDataMinusFakesRATIO[iSyst]->Write();
    }

    // Try unfolding using gen distribution and response matrix for denominator for right now !!!!!
    std::cout << "\n----- Looping over iSyst to unfold ratio -----" << std::endl;
    for (unsigned short iSyst = 0; iSyst < nSysts; ++iSyst) {
        std::cout << "\n >>>>> iSyst = " << iSyst << std::endl;

        unsigned short iGen = 0;
        unsigned short iResp = 0;
        if (iSyst == 0 || iSyst == 1 || iSyst == 2 || iSyst == 7 || iSyst == 8 || iSyst == 15 || iSyst == 16 || iSyst == 17){
            iGen = 0;
            iResp = 0;
            if (iSyst == 15) iResp = 11;
            else if (iSyst == 16) iResp = 12;
        }
        else if (iSyst == 3 || iSyst == 4 || iSyst == 5 || iSyst == 6){
            iGen = iSyst - 2;
            iResp = iSyst - 2;
        }
        else if (iSyst == 9 || iSyst == 10 || iSyst == 11 || iSyst == 12 || iSyst == 13 || iSyst == 14){
            iGen = iSyst - 4;
            iResp = iSyst - 4;
        }
        std::cout << "Fetching gen signal histo hGenDYJets[iGen], " << "iGen = " << iGen << std::endl;
        std::cout << "Fetching response matrix histo hResDYJets[iResp], " << "iResp = " << iResp << std::endl;

        std::cout << "Starting unfolding of ratio of " << variableNUM << " to " << variableDENOM << ", " << name[iSyst] << " for " << lepSel << " channel." << "\n";
        if( (hRecDataMinusFakesNUM[iSyst]->GetEntries() == 0) || (hRecDataMinusFakesDENOM[iSyst]->GetEntries() == 0) ) {
            std::cerr << "Warning: either histogram " << hRecDataMinusFakesNUM[iSyst]->GetName() << " or " << hRecDataMinusFakesDENOM[iSyst]->GetName() << " has no entries. Unfolding will be skipped.\n";
            continue;
        }
        //--- Do unfolding here!
        if (algo == "TUnfold") {
            std::cout << "\n-----> Using TUnfold package!" << std::endl;
            nIter[iSyst] = TUnfoldData(lepSel, algo, hResDYJetsDENOM[iResp], hRecDataMinusFakesRATIO[iSyst], hUnfData[iSyst], hUnfDataNoScale[iSyst],
                hUnfDataStatCov[iSyst], hUnfDataStatCovNoScale[iSyst], hUnfMCStatCov[iSyst], hUnfMCStatCovNoScale[iSyst], 
                name[iSyst], integratedLumi, hGenDYJetsDENOM[iGen], logy, true);
            
            // getting problem with segmentation violation with these following lines...
            // possibly because attempting to access the hUnfData[iSyst] and it doesn't work...
            // outputRootFile->cd();
            // TString nameTemp = "hUnfData_"+name[iSyst];
            // TString titleTemp = "Unfolded Ratio of "+variableNUM+" to "+variableDENOM;
            // hRecDataMinusFakesRATIO[iSyst]->SetNameTitle(nameTemp, titleTemp);
            // hUnfData[iSyst]->Write();
        }
        std::cout << "Exiting iSyst = " << iSyst << std::endl;
    }

    // Part with the covariance matrices.....

    //Plot response matrix for central distribution
    plotRespMat(hResDYJetsDENOM[0], variableDENOM, unfoldDir, 0, hGenDYJetsDENOM[0]);
    //if #recobins = 2 * #genbins, then generate alternate plot of response matrix (done for TUnfold)
    if (variableNUM.Contains("_2_Zinc")){
        plotRespMat(hResDYJetsDENOM[0], variableDENOM, unfoldDir, 1, hGenDYJetsDENOM[0]);
    }
    std::cout << "\n" << std::endl;
    
    outputRootFile->Close();
    closeAllFiles(fData, fDYJets, fBg, NBGDYJETS);
}
