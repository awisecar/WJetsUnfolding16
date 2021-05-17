//-*- mode: c++; c-basic-offset: 4 -*-
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <memory>
#include <regex>
#include <math.h>
#include <algorithm>
#include <sys/time.h>

#include <TStyle.h>
#include <TCanvas.h>
#include <TArrow.h>
#include <TLine.h>
#include <TSVDUnfold.h>
#include <TParameter.h>
#include "TRandom3.h"
#include "TVectorD.h"
#include "TMatrixD.h"
#include "TDecompSVD.h"
#include "TVectorT.h"
#include "TMatrixT.h"

#include "fileNamesZJets.h"
#include "getFilesAndHistogramsZJets.h"
#include "variablesOfInterestZJets.h"
#include "UnfoldingZJets.h"
#include "PlotSettings.h"
#include "ConfigVJets.h"
#include "functions.h"

//headers for TUnfold
#include "TUnfold.h"
#include "TUnfoldDensity.h"
#include "TUnfoldBinning.h"
#include "TUnfoldSys.h"

extern ConfigVJets cfg;
using namespace std;

void UnfoldingCombinedRun2(TString lepSel, int year, TString algo, TString histoDir, TString unfoldDir, 
    int jetPtMin, int jetEtaMax, TString variable, bool doNormalized, int whichSyst, bool isClosureTest){
    gStyle->SetOptStat(0);

    std::cout << "\n----- Begin unfolding of Run 2 data, combined at reco level! -----" << std::endl;
    std::cout << "\nVariable: " << variable << std::endl;

    //--- Get input histogram directory for specific year ---
    TString histoDir_2016 = histoDir + "_2016";
    TString histoDir_2017 = histoDir + "_2017";
    TString histoDir_2018 = histoDir + "_2018";

    //--- Create output directory if does not exist ---
    unfoldDir += "_Run2";
    system("mkdir -p " + unfoldDir);

    if (isClosureTest){
        std::cout << "\n ///////////////////////////////////////////////////////" << std::endl;
        std::cout << " //                                                   //" << std::endl;
        std::cout << " //      Doing closure test with W+jets reco MC!      //" << std::endl;
        std::cout << " //                                                   //" << std::endl;
        std::cout << " ///////////////////////////////////////////////////////" << std::endl;
    }

    //--- Choosing distribution to unfold
    int start = 0;
    int end = NVAROFINTERESTZJETS;
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

    // Integrated luminosity values, by year
    double integratedLumi_2016 = 35916.982;
    double integratedLumi_2017 = 41524.945;
    double integratedLumi_2018 = 59717.255;
    double integratedLumi_Run2 = integratedLumi_2016 + integratedLumi_2017 + integratedLumi_2018;

    /////////////////////////////////////////////////////////////////////////////////////////////////////

    // Now declare TFile arrays, open all files for each year ---

    // -- 2016 
    TFile *fData_2016[3] = {NULL}; 
    TFile *fDYJets_2016[11] = {NULL};
    TFile *fBg_2016[NBGDYJETS][11] = {{NULL}};
    std::cout << "\n<=========================================================================================>" << std::endl;
    std::cout << "\nOpening all files for 2016" << std::endl;
    getAllFiles(histoDir_2016, lepSel, "13TeV", jetPtMin, jetEtaMax, fData_2016, fDYJets_2016, fBg_2016, NBGDYJETS);

    TFile *fGen1_2016 = NULL;
    TString gen1;
    TString gen1File;
    std::map<TString, vector<TString> > generatorNames;
    if (DYMGPYTHIA8FILENAME.Length() > 0){
        vector<TString> mgpythia8;
        mgpythia8.push_back(DYMGPYTHIA8FILENAME);
        mgpythia8.push_back(DYMGPYTHIA8LEGEND);
        
        generatorNames["mgpythia8"] = mgpythia8;
        gen1 = "mgpythia8";
        
        gen1File = histoDir_2016 + "/" + lepSel + "_13TeV_" + generatorNames[gen1][0] + "_dR_TrigCorr_1_Syst_0_JetPtMin_";
        gen1File += jetPtMin;
        gen1File += "_JetEtaMax_";
        gen1File += jetEtaMax;
        gen1File += ".root";
        std::cout << "\nOpening file with name: " << gen1File << std::endl;
        fGen1_2016 = new TFile(gen1File);
    }
    else std::cout << "\nNo filename for fGen1_2016 specified! " << std::endl;

    // -- 2017
    TFile *fData_2017[3] = {NULL}; 
    TFile *fDYJets_2017[11] = {NULL};
    TFile *fBg_2017[NBGDYJETS][11] = {{NULL}};
    std::cout << "\n<=========================================================================================>" << std::endl;
    std::cout << "\nOpening all files for 2017" << std::endl;
    getAllFiles(histoDir_2017, lepSel, "13TeV", jetPtMin, jetEtaMax, fData_2017, fDYJets_2017, fBg_2017, NBGDYJETS);

    TFile *fGen1_2017 = NULL;
    if (DYMGPYTHIA8FILENAME.Length() > 0){
        vector<TString> mgpythia8;
        mgpythia8.push_back(DYMGPYTHIA8FILENAME);
        mgpythia8.push_back(DYMGPYTHIA8LEGEND);
        
        generatorNames["mgpythia8"] = mgpythia8;
        gen1 = "mgpythia8";
        
        gen1File = histoDir_2017 + "/" + lepSel + "_13TeV_" + generatorNames[gen1][0] + "_dR_TrigCorr_1_Syst_0_JetPtMin_";
        gen1File += jetPtMin;
        gen1File += "_JetEtaMax_";
        gen1File += jetEtaMax;
        gen1File += ".root";
        std::cout << "\nOpening file with name: " << gen1File << std::endl;
        fGen1_2017 = new TFile(gen1File);
    }
    else std::cout << "\nNo filename for fGen1_2017 specified! " << std::endl;
    
    // -- 2018
    TFile *fData_2018[3] = {NULL}; 
    TFile *fDYJets_2018[11] = {NULL};
    TFile *fBg_2018[NBGDYJETS][11] = {{NULL}};
    std::cout << "\n<=========================================================================================>" << std::endl;
    std::cout << "\nOpening all files for 2018" << std::endl;
    getAllFiles(histoDir_2018, lepSel, "13TeV", jetPtMin, jetEtaMax, fData_2018, fDYJets_2018, fBg_2018, NBGDYJETS);

    TFile *fGen1_2018 = NULL;
    if (DYMGPYTHIA8FILENAME.Length() > 0){
        vector<TString> mgpythia8;
        mgpythia8.push_back(DYMGPYTHIA8FILENAME);
        mgpythia8.push_back(DYMGPYTHIA8LEGEND);
        
        generatorNames["mgpythia8"] = mgpythia8;
        gen1 = "mgpythia8";
        
        gen1File = histoDir_2018 + "/" + lepSel + "_13TeV_" + generatorNames[gen1][0] + "_dR_TrigCorr_1_Syst_0_JetPtMin_";
        gen1File += jetPtMin;
        gen1File += "_JetEtaMax_";
        gen1File += jetEtaMax;
        gen1File += ".root";
        std::cout << "\nOpening file with name: " << gen1File << std::endl;
        fGen1_2018 = new TFile(gen1File);
    }
    else std::cout << "\nNo filename for fGen1_2018 specified! " << std::endl;

    std::cout << "\n<=========================================================================================>" << std::endl;

    // All necessary files have now been opened!
    // Begin loop over different variables...
    for (int i = start; i < end; ++i){
        variable = VAROFINTERESTZJETS[i].name;

        // Create output file
        TString outputFileName = unfoldDir + "/" + lepSel; 
        outputFileName += "_unfolded_" + variable + "_" + algo;
        outputFileName += "_JetPtMin_";
        outputFileName += jetPtMin;
        outputFileName += "_JetEtaMax_";
        outputFileName += jetEtaMax;
        outputFileName += "_MGPYTHIA6_";
        outputFileName += doNormalized ? "_normalized" : "";   
        TFile *outputRootFile = new TFile(outputFileName + ".root", "RECREATE");

        // Now declare TH1/TH2 histogram arrays, get all histograms for each year ---

        // -- 2016 
        TH1D *hRecData_2016[3]           = {};
        TH1D *hRecDYJets_2016[13]        = {};
        TH1D *hGenDYJets_2016[13]        = {};
        TH2D *hResDYJets_2016[13]        = {};
        TH1D *hFakDYJets_2016[18]        = {};
        TH2D *hAccepDYJets_2016[13]      = {};
        TH1D *hRecBg_2016[NBGDYJETS][13] = {{}};
        TH1D *hRecSumBg_2016[13]         = {};
        
        std::cout << "\nGetting all histos for 2016" << std::endl;
        getAllHistos(variable, 2016, hRecData_2016, fData_2016, hRecDYJets_2016, hGenDYJets_2016, hResDYJets_2016, fDYJets_2016, 
            hRecBg_2016, hRecSumBg_2016, fBg_2016, NBGDYJETS, hFakDYJets_2016, hAccepDYJets_2016, isClosureTest);

        TH1D *hGen1_2016 = NULL;
        if (DYMGPYTHIA8FILENAME.Length() > 0 ) hGen1_2016 = getHisto(fGen1_2016, "gen" + variable);

        // -- 2017
        TH1D *hRecData_2017[3]           = {};
        TH1D *hRecDYJets_2017[13]        = {};
        TH1D *hGenDYJets_2017[13]        = {};
        TH2D *hResDYJets_2017[13]        = {};
        TH1D *hFakDYJets_2017[18]        = {};
        TH2D *hAccepDYJets_2017[13]      = {};
        TH1D *hRecBg_2017[NBGDYJETS][13] = {{}};
        TH1D *hRecSumBg_2017[13]         = {};
        
        std::cout << "\nGetting all histos for 2017" << std::endl;
        getAllHistos(variable, 2017, hRecData_2017, fData_2017, hRecDYJets_2017, hGenDYJets_2017, hResDYJets_2017, fDYJets_2017, 
            hRecBg_2017, hRecSumBg_2017, fBg_2017, NBGDYJETS, hFakDYJets_2017, hAccepDYJets_2017, isClosureTest);

        TH1D *hGen1_2017 = NULL;
        if (DYMGPYTHIA8FILENAME.Length() > 0 ) hGen1_2017 = getHisto(fGen1_2017, "gen" + variable);

        // -- 2018
        TH1D *hRecData_2018[3]           = {};
        TH1D *hRecDYJets_2018[13]        = {};
        TH1D *hGenDYJets_2018[13]        = {};
        TH2D *hResDYJets_2018[13]        = {};
        TH1D *hFakDYJets_2018[18]        = {};
        TH2D *hAccepDYJets_2018[13]      = {};
        TH1D *hRecBg_2018[NBGDYJETS][13] = {{}};
        TH1D *hRecSumBg_2018[13]         = {};
        
        std::cout << "\nGetting all histos for 2018" << std::endl;
        getAllHistos(variable, 2018, hRecData_2018, fData_2018, hRecDYJets_2018, hGenDYJets_2018, hResDYJets_2018, fDYJets_2018, 
            hRecBg_2018, hRecSumBg_2018, fBg_2018, NBGDYJETS, hFakDYJets_2018, hAccepDYJets_2018, isClosureTest);

        TH1D *hGen1_2018 = NULL;
        if (DYMGPYTHIA8FILENAME.Length() > 0 ) hGen1_2018 = getHisto(fGen1_2018, "gen" + variable);


        // ------------------------------------------------
        // Get fake rates dist.
        TH1D* hFakeRate_Central_Run2 = (TH1D*) hFakDYJets_2016[0]->Clone("hFakeRate_Central_Run2");
        hFakeRate_Central_Run2->Add(hFakDYJets_2017[0]);
        hFakeRate_Central_Run2->Add(hFakDYJets_2018[0]);
        if (isClosureTest){
            TH1D* hRecDYJets_Central_Run2 = (TH1D*) hRecDYJets_2016[0]->Clone("hRecDYJets_Central_Run2");
            hRecDYJets_Central_Run2->Add(hRecDYJets_2017[0]);
            hRecDYJets_Central_Run2->Add(hRecDYJets_2018[0]);
            hFakeRate_Central_Run2->Divide(hRecDYJets_Central_Run2);
        }
        else{
            TH1D *hRecDataMinusBg_Central_Run2 = (TH1D*) hRecData_2016[0]->Clone();
            hRecDataMinusBg_Central_Run2->Add(hRecSumBg_2016[0], -1);
            hRecDataMinusBg_Central_Run2->Add(hRecData_2017[0]);
            hRecDataMinusBg_Central_Run2->Add(hRecSumBg_2017[0], -1);
            hRecDataMinusBg_Central_Run2->Add(hRecData_2018[0]);
            hRecDataMinusBg_Central_Run2->Add(hRecSumBg_2018[0], -1);
            hFakeRate_Central_Run2->Divide(hRecDataMinusBg_Central_Run2);
        }
        hFakeRate_Central_Run2->SetTitle("hFakeRate_Central_Run2");
        hFakeRate_Central_Run2->GetXaxis()->SetTitle(hRecData_2016[0]->GetXaxis()->GetTitle());
        hFakeRate_Central_Run2->GetYaxis()->SetTitle("Fake Rate");
        hFakeRate_Central_Run2->GetYaxis()->SetRangeUser(0., 1.);        
        // ------------------------------------------------


        // Now get theory cross sections using all years ---
        outputRootFile->cd();

        // -- MG5 NLO FxFx prediction
        TH1D *hGenSignalNLOFxFx_Run2 = (TH1D*) hGenDYJets_2016[0]->Clone();
        hGenSignalNLOFxFx_Run2->Add(hGenDYJets_2017[0]);
        hGenSignalNLOFxFx_Run2->Add(hGenDYJets_2018[0]);

        TH1D *hMadGenCrossSection = makeCrossSectionHist(hGenSignalNLOFxFx_Run2, integratedLumi_Run2);
        hMadGenCrossSection->SetZTitle("MG_aMC FxFx + PY8 (#leq 2j NLO + PS)");

        // -- Madgraph LO MLM prediction
        TH1D *hGenSignalLOMLM_Run2 = (TH1D*) hGen1_2016->Clone();
        hGenSignalLOMLM_Run2->Add(hGen1_2017);
        hGenSignalLOMLM_Run2->Add(hGen1_2018);

        TH1D *hGen1CrossSection = makeCrossSectionHist(hGenSignalLOMLM_Run2, integratedLumi_Run2);
        hGen1CrossSection->SetZTitle(generatorNames[gen1][1]);

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

        TH1D *hRecDataBinsMerged[nSysts]     = {};
        TH1D *hUnfData[nSysts]               = {};
        TH1D *hUnfDataNoScale[nSysts]        = {};
        TH2D *hUnfDataStatCov[nSysts]        = {};
        TH2D *hUnfDataStatCovNoScale[nSysts] = {};
        TH2D *hUnfMCStatCov[nSysts]          = {};
        TH2D *hUnfMCStatCovNoScale[nSysts]   = {};

        bool logy = VAROFINTERESTZJETS[i].log;

        // Unfold the data (combined among all years at reco level) ---
        std::cout << "\nEntering iSyst loop to unfold for Central and every variation for each systematic..." << std::endl;
        std::cout << "nSysts = " << nSysts << std::endl;
        std::cout << "whichSyst = " << whichSyst << std::endl;
        for (int iSyst = 0; iSyst < nSysts; ++iSyst){
            std::cout << "\n >>>>>>>>>> Start iSyst = " << iSyst << "! <<<<<<<<<<" << std::endl;
            if (iSyst != 0 && whichSyst >= 0 && iSyst != whichSyst) continue;

            int iData(0), iBg(0), iGen(0), iResp(0); 
            unfoldingFileSelector(iSyst, iData, iBg, iGen, iResp);


            ////////////////////////////////////////////////////////////////////////////////
            // Selecting input distibution to unfold

            TH1D *hRecDataMinusFakes_2016 = NULL;
            TH1D *hRecDataMinusFakes_2017 = NULL;
            TH1D *hRecDataMinusFakes_2018 = NULL;
            if(!isClosureTest){
                // -- Before combining data for all years, subtract BG and fakes
                // 2016 --
                hRecDataMinusFakes_2016 = (TH1D*) hRecData_2016[iData]->Clone();
                hRecDataMinusFakes_2016->Add(hRecSumBg_2016[iBg], -1);
                RemoveFakes(hRecDataMinusFakes_2016, hFakDYJets_2016[iSyst]);
                // 2017 --
                hRecDataMinusFakes_2017 = (TH1D*) hRecData_2017[iData]->Clone();
                hRecDataMinusFakes_2017->Add(hRecSumBg_2017[iBg], -1);
                RemoveFakes(hRecDataMinusFakes_2017, hFakDYJets_2017[iSyst]);
                // 2018 --
                hRecDataMinusFakes_2018 = (TH1D*) hRecData_2018[iData]->Clone();
                hRecDataMinusFakes_2018->Add(hRecSumBg_2018[iBg], -1);
                RemoveFakes(hRecDataMinusFakes_2018, hFakDYJets_2018[iSyst]);
            }
            else{
                // -- Before combining signal reco MC for all years, subtract fakes
                // 2016 --
                hRecDataMinusFakes_2016 = (TH1D*) hRecDYJets_2016[iResp]->Clone();
                RemoveFakes(hRecDataMinusFakes_2016, hFakDYJets_2016[iSyst]);
                // 2017 --
                hRecDataMinusFakes_2017 = (TH1D*) hRecDYJets_2017[iResp]->Clone();
                RemoveFakes(hRecDataMinusFakes_2017, hFakDYJets_2017[iSyst]);
                // 2018 --
                hRecDataMinusFakes_2018 = (TH1D*) hRecDYJets_2018[iResp]->Clone();
                RemoveFakes(hRecDataMinusFakes_2018, hFakDYJets_2018[iSyst]);
            }
            ////////////////////////////////////////////////////////////////////////////////


            // -- Then add all years together, then unfold

            // Reconstructed data (or reco signal MC for closure test)
            // NOTE: THIS IS A FIX FOR A PROBLEM THAT OCCURRED WHEN A NaN IN THE FIRST BIN APPEARED
            std::cout << "hRecDataMinusFakes_2016->Integral() = " << hRecDataMinusFakes_2016->Integral() << std::endl;
            std::cout << "hRecDataMinusFakes_2017->Integral() = " << hRecDataMinusFakes_2017->Integral() << std::endl;
            std::cout << "hRecDataMinusFakes_2018->Integral() = " << hRecDataMinusFakes_2018->Integral() << std::endl;
            for (int i = 1; i <= hRecDataMinusFakes_2016->GetNbinsX(); i++){
                if ( !(hRecDataMinusFakes_2016->GetBinContent(i) > 0.0) ){
                    std::cout << "Fixing bin " << i << " of distribution for 2016" << std::endl;
                    hRecDataMinusFakes_2016->SetBinContent(i, 0.);
                    hRecDataMinusFakes_2016->SetBinError(i, 0.);
                }
                if ( !(hRecDataMinusFakes_2017->GetBinContent(i) > 0.0) ){
                    std::cout << "Fixing bin " << i << " of distribution for 2017" << std::endl;
                    hRecDataMinusFakes_2017->SetBinContent(i, 0.);
                    hRecDataMinusFakes_2017->SetBinError(i, 0.);
                }
                if ( !(hRecDataMinusFakes_2018->GetBinContent(i) > 0.0) ){
                    std::cout << "Fixing bin " << i << " of distribution for 2018" << std::endl;
                    hRecDataMinusFakes_2018->SetBinContent(i, 0.);
                    hRecDataMinusFakes_2018->SetBinError(i, 0.);
                }
            }
            TH1D *hRecDataMinusFakes_Run2 = (TH1D*) hRecDataMinusFakes_2016->Clone();
            hRecDataMinusFakes_Run2->Add(hRecDataMinusFakes_2017);
            hRecDataMinusFakes_Run2->Add(hRecDataMinusFakes_2018);
            std::cout << "hRecDataMinusFakes_Run2->Integral() = " << hRecDataMinusFakes_Run2->Integral() << std::endl;

            // Response matrices (derived from signal MC)
            TH2D *hResDYJets_Run2 = (TH2D*) hResDYJets_2016[iResp]->Clone();
            hResDYJets_Run2->Add(hResDYJets_2017[iResp]);
            hResDYJets_Run2->Add(hResDYJets_2018[iResp]);

            // Acceptance counts (derived from signal MC)
            TH2D *hAccepDYJets_Run2 = (TH2D*) hAccepDYJets_2016[iGen]->Clone();
            hAccepDYJets_Run2->Add(hAccepDYJets_2017[iGen]);
            hAccepDYJets_Run2->Add(hAccepDYJets_2018[iGen]);

            // Gen-level counts (derived from signal MC)
            TH1D *hGenDYJets_Run2 = (TH1D*) hGenDYJets_2016[iGen]->Clone();
            hGenDYJets_Run2->Add(hGenDYJets_2017[iGen]);
            hGenDYJets_Run2->Add(hGenDYJets_2018[iGen]);

            // Use TUnfold package for unfolding
            if (algo == "TUnfold"){
                TUnfoldData(lepSel, algo, hResDYJets_Run2, hRecDataMinusFakes_Run2, hRecDataBinsMerged[iSyst], hUnfData[iSyst], hUnfDataNoScale[iSyst], hUnfDataStatCov[iSyst], hUnfDataStatCovNoScale[iSyst], 
                    hUnfMCStatCov[iSyst], hUnfMCStatCovNoScale[iSyst], name[iSyst], integratedLumi_Run2, hGenDYJets_Run2, hAccepDYJets_Run2, logy, false, 9999);
            }

        } // END loop over all systematic variations

        std::cout << "\n<=========================================================================================>\n" << std::endl;

        // Calculate covariance matrices ---
        TH2D *hCov[12] = {};
        int nCovs = 10;
        // -- Covariance matrix for the data statistical uncertainties (input to unfolding)
        if (hUnfDataStatCov[0]){
            std::cout << "Retrieving CovDataStat..." << std::endl;
            hCov[0] = (TH2D*) hUnfDataStatCov[0]->Clone("CovDataStat");
        }
        // -- Covariance matrix for the (uncorrelated) statistical uncertainties of the response matrix
        if (hUnfMCStatCov[0]){
            std::cout << "Retrieving CovMCStat..." << std::endl;
            hCov[1] = (TH2D*) hUnfMCStatCov[0]->Clone("CovMCStat");
        }
        // -- Covariance matrices for systematic variations
        if (hUnfData[1])  hCov[2] = makeCovFromUpAndDown(hUnfData[0], hUnfData[1], hUnfData[2], "CovJES");
        if (hUnfData[3])  hCov[3] = makeCovFromUpAndDown(hUnfData[0], hUnfData[3], hUnfData[4], "CovPU");
        if (hUnfData[5])  hCov[4] = makeCovFromUpAndDown(hUnfData[0], hUnfData[5], hUnfData[6], "CovXSEC");
        if (hUnfData[7])  hCov[5] = makeCovFromUpAndDown(hUnfData[0], hUnfData[7], hUnfData[8], "CovJER");
        if (hUnfData[9])  hCov[6] = makeCovFromUpAndDown(hUnfData[0], hUnfData[9], hUnfData[10], "CovLepSF");
        // if (hUnfData[11]) hCov[7] = makeCovFromUpAndDown(hUnfData[0], hUnfData[11], hUnfData[12], "CovBtagSF"); // not currently using
        if (hUnfData[13]) hCov[8] = makeCovFromUpAndDown(hUnfData[0], hUnfData[13], hUnfData[14], "CovL1Prefire");
        if (hUnfData[15]) hCov[9] = makeCovFromUpAndDown(hUnfData[0], hUnfData[15], hUnfData[16], "CovLumi");
        // if (hUnfData[17]) hCov[10] = makeCovFromUpAndDown(hUnfData[0], hUnfData[17], hUnfData[0], "CovBlank"); // not currently using
        // -- Covariance matrix for all systematics summed
        if (hCov[1]) hCov[11] = (TH2D*) hCov[1]->Clone("CovTotSyst");
        if (hCov[11]){
            std::cout << "Calculating CovTotSyst from all systematic contributions!" << std::endl;
            for (int i = 2; i <= nCovs; ++i){
                if (hCov[i]) hCov[11]->Add(hCov[i]);
            }
        }

        // Plot central unfolded distribution (w/ stat & syst errors), theory predictions ---
        TCanvas *crossSectionPlot = makeCrossSectionPlot(lepSel, 9999, variable, false, doNormalized, hUnfData[0], hCov[11], hMadGenCrossSection, hGen1CrossSection);
        crossSectionPlot->Draw();
        crossSectionPlot->SaveAs(outputFileName + ".pdf");

        // Plot response matrix for central distribution ---
        // -- First (re-)add the central response matrix and gen-level distributions
        TH2D *hResDYJetsCentral_Run2 = (TH2D*) hResDYJets_2016[0]->Clone();
        hResDYJetsCentral_Run2->Add(hResDYJets_2017[0]);
        hResDYJetsCentral_Run2->Add(hResDYJets_2018[0]);
        TH1D *hGenDYJetsCentral_Run2 = (TH1D*) hGenDYJets_2016[0]->Clone();
        hGenDYJetsCentral_Run2->Add(hGenDYJets_2017[0]);
        hGenDYJetsCentral_Run2->Add(hGenDYJets_2018[0]);
        // -- Run plotting code
        plotRespMat(hResDYJetsCentral_Run2, variable, unfoldDir, 0, hGenDYJetsCentral_Run2);
        //if #recobins = 2 * #genbins, then generate alternate plot of response matrix (done for TUnfold)
        if (variable.Contains("_TUnfold")) plotRespMat(hResDYJetsCentral_Run2, variable, unfoldDir, 1, hGenDYJetsCentral_Run2);

        std::cout << "\n<=========================================================================================>" << std::endl;

        // Save other things --- 
        outputRootFile->cd();

        hFakeRate_Central_Run2->Write("hFakeRate_Central_Run2");
        hMadGenCrossSection->Write("hMadGenCrossSection"); //gen MC for W+jets NLO FxFx signal (also)
        hGen1CrossSection->Write("hGen1CrossSection"); //gen MC for W+jets LO MLM signal

        // Write out unfolded distributions and covariance matrices ---
        for (int iSyst = 0; iSyst < nSysts; ++iSyst) if (hRecDataBinsMerged[iSyst]) hRecDataBinsMerged[iSyst]->Write();
        for (int iSyst = 0; iSyst < nSysts; ++iSyst) if (hUnfData[iSyst]) hUnfData[iSyst]->Write();     
        for (int i = 0; i <= 11; ++i) if (hCov[i]) hCov[i]->Write();

        outputRootFile->Close();
    } // END loop over all variables to unfold

    // Close all files ---
    closeAllFiles(fData_2016, fDYJets_2016, fBg_2016, NBGDYJETS);
    closeAllFiles(fData_2017, fDYJets_2017, fBg_2017, NBGDYJETS);
    closeAllFiles(fData_2018, fDYJets_2018, fBg_2018, NBGDYJETS);
    if(fGen1_2016) fGen1_2016->Close();
    if(fGen1_2017) fGen1_2017->Close();
    if(fGen1_2018) fGen1_2018->Close(); 
}