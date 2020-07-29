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
using namespace std;

void UnfoldingZJetsRatios(TString lepSel, int year, TString algo, TString histoDir, TString unfoldDir, int jetPtMin, int jetEtaMax, TString variableNUM, TString variableDENOM, bool doNormalized, int whichSyst){
    gStyle->SetOptStat(0);

    std::cout << "\n----- Begin ratio unfolding! -----" << std::endl;
    cout << "\nNumerator variable: " << variableNUM << endl;
    cout << "Denominator variable: " << variableDENOM << endl;

    //--- Get input histogram directory for specific year ---
    histoDir += "_";
    histoDir += year;
    
    //--- Create output directory if does not exist ---
    unfoldDir += "Ratio";
    unfoldDir += "_";
    unfoldDir += year;
    system("mkdir -p " + unfoldDir);

    double integratedLumi(0.);
    if (year == 2016)      integratedLumi = 35916.982;
    else if (year == 2017) integratedLumi = 41524.945;
    else                   integratedLumi = 59717.255;

    std::cout << "\n year = " << year << std::endl;
    std::cout << " integratedLumi = " << integratedLumi << std::endl; 

    std::cout << "\n<=========================================================================================>" << std::endl;

    //--- Here we declare the different arrays of TFiles (nominal/central and systematic variations)
    TFile *fData[3] = {NULL}; 
    TFile *fDYJets[11] = {NULL};
    TFile *fBg[NBGDYJETS][11] = {{NULL}};

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
    TH1D *hGenDYJetsNUM[13] = {NULL};
    //--- res DYJets histograms ---
    TH2D *hResDYJetsNUM[13] = {NULL};
    //--- "fakes" DYJets histograms ---
    TH1D *hFakDYJetsNUM[18] = {NULL};
    //--- "accept" DYJets histograms" ---
    TH2D *hAccepDYJetsNUM[13] = {NULL};
    //--- rec Bg histograms ---
    TH1D *hRecBgNUM[NBGDYJETS][13] = {{NULL}};
    //--- rec Sum Bg histograms ---
    TH1D *hRecSumBgNUM[13] = {NULL};

    //--- Get all numerator histograms ---
    std::cout << "\nGrabbing all needed histograms from files opened above for... " << variableNUM << std::endl;
    getAllHistos(variableNUM, year, hRecDataNUM, fData, hRecDYJetsNUM, hGenDYJetsNUM, hResDYJetsNUM, fDYJets, 
        hRecBgNUM, hRecSumBgNUM, fBg, NBGDYJETS, hFakDYJetsNUM, hAccepDYJetsNUM, false);

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
    TH1D *hGenDYJetsDENOM[13] = {NULL};
    //--- res DYJets histograms ---
    TH2D *hResDYJetsDENOM[13] = {NULL};
    //--- "fakes" DYJets histograms ---
    TH1D *hFakDYJetsDENOM[18] = {NULL};
    //--- "accept" DYJets histograms ---
    TH2D *hAccepDYJetsDENOM[11] = {NULL};
    //--- rec Bg histograms ---
    TH1D *hRecBgDENOM[NBGDYJETS][13] = {{NULL}};
    //--- rec Sum Bg histograms ---
    TH1D *hRecSumBgDENOM[13] = {NULL};

    //--- Get all denominator histograms ---
    std::cout << "\nGrabbing all needed histograms from files opened above for... " << variableDENOM << std::endl;
    getAllHistos(variableDENOM, year, hRecDataDENOM, fData, hRecDYJetsDENOM, hGenDYJetsDENOM, hResDYJetsDENOM, fDYJets, 
        hRecBgDENOM, hRecSumBgDENOM, fBg, NBGDYJETS, hFakDYJetsDENOM, hAccepDYJetsDENOM, false);

    // Theory histograms for denominator
    TH1D *hGen1DENOM = NULL;
    if (DYMGPYTHIA8FILENAME.Length() > 0 ) hGen1DENOM = getHisto(fGen1, "gen" + variableDENOM);

    std::cout << "\n<=========================================================================================>" << std::endl;

    //  --- Naming convention for central+systematic variations ---
    //  0 - Central, 
    //  1 - JES up,        2 - JES down, 
    //  3 - PU up,         4 - PU down, 
    //  5 - XSEC up,       6 - XSEC down
    //  7 - JER up,        8 - JER down, 
    //  9 - LepSF up,     10 - LepSF down
    // 11 - BtagSF up,    12 - BtagSF down 
    // 13 - L1Prefire up, 14 - L1Prefire down
    // 15 - Lumi up,      16 - Lumi down
    // 17 - (Blank)  

    int nSysts = 18;
    TString name[] = {"Central", "JESUp", "JESDown", "PUUp", "PUDown", "XSECUp", "XSECDown", "JERUp", "JERDown", 
        "LepSFUp", "LepSFDown", "BtagSFUp", "BtagSFDown", "L1PrefireUp", "L1PrefireDown", "LumiUp", "LumiDown", "Blank"};
    
    // unfolded distributions and matrices for numerator and denominator
    // numerator
    TH1D *hRecDataBinsMergedNUM[nSysts]       = {}; 
    TH1D *hUnfDataNUM[nSysts]                 = {}; 
    TH1D *hUnfDataNoScaleNUM[nSysts]          = {};
    TH2D *hUnfDataStatCovNUM[nSysts]          = {};
    TH2D *hUnfDataStatCovNoScaleNUM[nSysts]   = {};
    TH2D *hUnfMCStatCovNUM[nSysts]            = {};
    TH2D *hUnfMCStatCovNoScaleNUM[nSysts]     = {};
    // denominator
    TH1D *hRecDataBinsMergedDENOM[nSysts]     = {};
    TH1D *hUnfDataDENOM[nSysts]               = {};
    TH1D *hUnfDataNoScaleDENOM[nSysts]        = {};
    TH2D *hUnfDataStatCovDENOM[nSysts]        = {};
    TH2D *hUnfDataStatCovNoScaleDENOM[nSysts] = {};
    TH2D *hUnfMCStatCovDENOM[nSysts]          = {};
    TH2D *hUnfMCStatCovNoScaleDENOM[nSysts]   = {};

    bool logy = false;

    std::cout << "\n<=========================================================================================>" << std::endl;

    std::cout << "\n----- Unfolding NUMERATOR! ----- " << std::endl;
    std::cout << "Entering iSyst loop to unfold for Central and every variation for each systematic for... " << variableNUM << std::endl;
    std::cout << "nSysts = " << nSysts << std::endl;
    std::cout << "whichSyst = " << whichSyst << std::endl;
    for (int iSyst = 0; iSyst < nSysts; ++iSyst){
        std::cout << "\n >>>>>>>>>> Start iSyst = " << iSyst << "! <<<<<<<<<<" << std::endl;
        if (iSyst != 0 && whichSyst >= 0 && iSyst != whichSyst) continue;

        int iData(0), iBg(0), iGen(0), iResp(0); 
        unfoldingFileSelector(iSyst, iData, iBg, iGen, iResp);

        std::cout << "Fetching main reco data histo hRecDataNUM[iData], " << "iData = " << iData << std::endl;
        TH1D *hRecDataMinusFakesNUM = (TH1D*) hRecDataNUM[iData]->Clone();
        std::cout << "From reco data, subtracting MC BG histo hRecSumBgNUM[iBg], " << "iBg = " << iBg << std::endl;
        hRecDataMinusFakesNUM->Add(hRecSumBgNUM[iBg], -1);
        std::cout << "Removing fakes using hFakDYJetsNUM[iSyst]!" << std::endl;
        RemoveFakes(hRecDataMinusFakesNUM, hFakDYJetsNUM[iSyst]);

        std::cout << "Fetching gen signal histo hGenDYJetsNUM[iGen] and acceptance histo hAccepDYJetsNUM[iGen], " << "iGen = " << iGen << std::endl;
        std::cout << "Fetching response matrix histo hResDYJetsNUM[iResp], " << "iResp = " << iResp << std::endl;

        std::cout << "\n ===> Unfold: " << variableNUM << ", " << name[iSyst] << "\n";
        if(hRecDataMinusFakesNUM->GetEntries() == 0){
            std::cerr << "Warning: histogram " << hRecDataMinusFakesNUM->GetName() << " has no entries. Unfolding will be skipped.\n";
            continue;
        }

        if (algo == "TUnfold"){
            TUnfoldData(lepSel, algo, hResDYJetsNUM[iResp], hRecDataMinusFakesNUM, hRecDataBinsMergedNUM[iSyst], hUnfDataNUM[iSyst], hUnfDataNoScaleNUM[iSyst],
                hUnfDataStatCovNUM[iSyst], hUnfDataStatCovNoScaleNUM[iSyst], hUnfMCStatCovNUM[iSyst], hUnfMCStatCovNoScaleNUM[iSyst], 
                name[iSyst], integratedLumi, hGenDYJetsNUM[iGen], hAccepDYJetsNUM[iGen], logy, true, year);
        }
    }

    std::cout << "\n<=========================================================================================>" << std::endl;

    std::cout << "\n----- Unfolding DENOMINATOR! ----- " << std::endl;
    std::cout << "Entering iSyst loop to unfold for Central and every variation for each systematic for... " << variableDENOM << std::endl;
    std::cout << "nSysts = " << nSysts << std::endl;
    std::cout << "whichSyst = " << whichSyst << std::endl;
    for (int iSyst = 0; iSyst < nSysts; ++iSyst){
        std::cout << "\n >>>>>>>>>> Start iSyst = " << iSyst << "! <<<<<<<<<<" << std::endl;
        if (iSyst != 0 && whichSyst >= 0 && iSyst != whichSyst) continue;

        int iData(0), iBg(0), iGen(0), iResp(0); 
        unfoldingFileSelector(iSyst, iData, iBg, iGen, iResp);

        std::cout << "Fetching main reco data histo hRecDataDENOM[iData], " << "iData = " << iData << std::endl;
        TH1D *hRecDataMinusFakesDENOM = (TH1D*) hRecDataDENOM[iData]->Clone();
        std::cout << "From reco data, subtracting MC BG histo hRecSumBgDENOM[iBg], " << "iBg = " << iBg << std::endl;
        hRecDataMinusFakesDENOM->Add(hRecSumBgDENOM[iBg], -1);
        std::cout << "Removing fakes using hFakDYJetsDENOM[iSyst]!" << std::endl;
        RemoveFakes(hRecDataMinusFakesDENOM, hFakDYJetsDENOM[iSyst]);
        
        std::cout << "Fetching gen signal histo hGenDYJetsDENOM[iGen] and acceptance histo hAccepDYJetsDENOM[iGen], " << "iGen = " << iGen << std::endl;
        std::cout << "Fetching response matrix histo hResDYJetsDENOM[iResp], " << "iResp = " << iResp << std::endl;

        std::cout << "\n ===> Unfold: " << variableDENOM << ", " << name[iSyst] << "\n";
        if(hRecDataMinusFakesDENOM->GetEntries() == 0){
            std::cerr << "Warning: histogram " << hRecDataMinusFakesDENOM->GetName() << " has no entries. Unfolding will be skipped.\n";
            continue;
        }

        if (algo == "TUnfold"){
            TUnfoldData(lepSel, algo, hResDYJetsDENOM[iResp], hRecDataMinusFakesDENOM, hRecDataBinsMergedDENOM[iSyst], hUnfDataDENOM[iSyst], hUnfDataNoScaleDENOM[iSyst],
                hUnfDataStatCovDENOM[iSyst], hUnfDataStatCovNoScaleDENOM[iSyst], hUnfMCStatCovDENOM[iSyst], hUnfMCStatCovNoScaleDENOM[iSyst], 
                name[iSyst], integratedLumi, hGenDYJetsDENOM[iGen], hAccepDYJetsDENOM[iGen], logy, true, year);
        }
        
    }

    std::cout << "\n<=========================================================================================>" << std::endl;

    // Take ratio of separately-unfolded numerator and denominator
    std::cout << "\n----- Take ratio of unfolded & reco distributions! ----- " << std::endl;

    TH1D *hUnfDataRATIO[nSysts]  = {};
    TH1D *hRecoDataRATIO[nSysts] = {};

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
        hRecoDataRATIO[iSyst] = (TH1D*) hRecDataBinsMergedNUM[iSyst]->Clone();
        hRecoDataRATIO[iSyst]->Divide(hRecDataBinsMergedNUM[iSyst], hRecDataBinsMergedDENOM[iSyst], 1, 1, "B");
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
    hGenSignalNLOFxFxRATIO->SetTitle("MG_aMC FxFx + PY8 (#leq 2j NLO + PS)");
    hGenSignalNLOFxFxRATIO->GetYaxis()->SetTitle("Ratio");
    hGenSignalNLOFxFxRATIO->Write();
    // LO MLM (Madgraph)
    TH1D *hGenSignalLOMLMNUM = makeCrossSectionHist(hGen1NUM, integratedLumi);
    TH1D *hGenSignalLOMLMDENOM = makeCrossSectionHist(hGen1DENOM, integratedLumi);
    TH1D *hGenSignalLOMLMRATIO = (TH1D*) hGenSignalLOMLMNUM->Clone();
    hGenSignalLOMLMRATIO->Divide(hGenSignalLOMLMNUM, hGenSignalLOMLMDENOM, 1, 1, "B");
    hGenSignalLOMLMRATIO->SetName("GenSignalXSecRatio_LOMLM");
    hGenSignalLOMLMRATIO->SetTitle("MG_aMC + PY8 (#leq 4j LO + PS)");
    hGenSignalLOMLMRATIO->GetYaxis()->SetTitle("Ratio");
    hGenSignalLOMLMRATIO->Write();

    std::cout << "\n<=========================================================================================>" << std::endl;

    // Calculate covariance matrices for RATIO ---
    TH2D *hCovRATIO[12] = {};
    int nCovs = 10;
    // // -- Covariance matrix for the data statistical uncertainties (input to unfolding)
    // if (hUnfDataStatCov[0]){
    //     std::cout << "Retrieving CovDataStat..." << std::endl;
    //     hCovRATIO[0] = (TH2D*) hUnfDataStatCov[0]->Clone("CovDataStat");
    // }
    // // -- Covariance matrix for the (uncorrelated) statistical uncertainties of the response matrix
    // if (hUnfMCStatCov[0]){
    //     std::cout << "Retrieving CovMCStat..." << std::endl;
    //     hCovRATIO[1] = (TH2D*) hUnfMCStatCov[0]->Clone("CovMCStat");
    // }
    // -- Covariance matrices for systematic variations
    if (hUnfDataRATIO[1])  hCovRATIO[2] = makeCovFromUpAndDown(hUnfDataRATIO[0], hUnfDataRATIO[1], hUnfDataRATIO[2], "CovJES");
    if (hUnfDataRATIO[3])  hCovRATIO[3] = makeCovFromUpAndDown(hUnfDataRATIO[0], hUnfDataRATIO[3], hUnfDataRATIO[4], "CovPU");
    if (hUnfDataRATIO[5])  hCovRATIO[4] = makeCovFromUpAndDown(hUnfDataRATIO[0], hUnfDataRATIO[5], hUnfDataRATIO[6], "CovXSEC");
    if (hUnfDataRATIO[7])  hCovRATIO[5] = makeCovFromUpAndDown(hUnfDataRATIO[0], hUnfDataRATIO[7], hUnfDataRATIO[8], "CovJER");
    if (hUnfDataRATIO[9])  hCovRATIO[6] = makeCovFromUpAndDown(hUnfDataRATIO[0], hUnfDataRATIO[9], hUnfDataRATIO[10], "CovLepSF");
    // if (hUnfDataRATIO[11]) hCovRATIO[7] = makeCovFromUpAndDown(hUnfDataRATIO[0], hUnfDataRATIO[11], hUnfDataRATIO[12], "CovBtagSF"); // not currently using
    if (hUnfDataRATIO[13]) hCovRATIO[8] = makeCovFromUpAndDown(hUnfDataRATIO[0], hUnfDataRATIO[13], hUnfDataRATIO[14], "CovL1Prefire");
    if (hUnfDataRATIO[15]) hCovRATIO[9] = makeCovFromUpAndDown(hUnfDataRATIO[0], hUnfDataRATIO[15], hUnfDataRATIO[16], "CovLumi");
    // if (hUnfDataRATIO[17]) hCovRATIO[10] = makeCovFromUpAndDown(hUnfDataRATIO[0], hUnfDataRATIO[17], hUnfDataRATIO[0], "CovBlank"); // not currently using
    // -- Covariance matrix for all systematics summed
    // if (hCovRATIO[1]) hCovRATIO[11] = (TH2D*) hCovRATIO[1]->Clone("CovTotSyst");
    if (hCovRATIO[2]) hCovRATIO[11] = (TH2D*) hCovRATIO[2]->Clone("CovTotSyst");
    if (hCovRATIO[11]){
        std::cout << "Calculating CovTotSyst from all systematic contributions!" << std::endl;
        // for (int i = 2; i <= nCovs; ++i){
        for (int i = 3; i <= nCovs; ++i){
            if (hCovRATIO[i]) hCovRATIO[11]->Add(hCovRATIO[i]);
        }
    }

    std::cout << "\n<=========================================================================================>" << std::endl;

    // Plot central unfolded distribution (w/ stat & syst errors), theory predictions ---
    TString variableRATIO = variableNUM + "_TO_" + variableDENOM;
    TCanvas *crossSectionPlot = makeCrossSectionPlot(lepSel, year, variableRATIO, true, doNormalized, hUnfDataRATIO[0], hCovRATIO[11], hGenSignalNLOFxFxRATIO, hGenSignalLOMLMRATIO);
    crossSectionPlot->Draw();
    crossSectionPlot->SaveAs(outputFileName + ".pdf");

    //Plot response matrices ---
    // Numerator
    plotRespMat(hResDYJetsNUM[0], variableNUM, unfoldDir, 0, hGenDYJetsNUM[0]);
    //if #recobins = 2 * #genbins, then generate alternate plot of response matrix (done for TUnfold)
    if (variableNUM.Contains("_TUnfold")) plotRespMat(hResDYJetsNUM[0], variableNUM, unfoldDir, 1, hGenDYJetsNUM[0]);
    std::cout << std::endl;
    // Denominator
    plotRespMat(hResDYJetsDENOM[0], variableDENOM, unfoldDir, 0, hGenDYJetsDENOM[0]);
    //if #recobins = 2 * #genbins, then generate alternate plot of response matrix (done for TUnfold)
    if (variableDENOM.Contains("_TUnfold")) plotRespMat(hResDYJetsDENOM[0], variableDENOM, unfoldDir, 1, hGenDYJetsDENOM[0]);
    std::cout << std::endl;


    std::cout << "\n<=========================================================================================>" << std::endl;

    outputRootFile->Close();
    closeAllFiles(fData, fDYJets, fBg, NBGDYJETS);
    if(fGen1) fGen1->Close();

    std::cout << "\nFinished!\n" << std::endl;
}
