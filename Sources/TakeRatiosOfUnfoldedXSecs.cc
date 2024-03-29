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

void TakeRatiosOfUnfoldedXSecs(TString lepSel, int year, TString algo, TString histoDir, TString unfoldDir, 
    int jetPtMin, int jetEtaMax, TString variableNUM, TString variableDENOM, bool doNormalized, int whichSyst){
    gStyle->SetOptStat(0);

    std::cout << "\n----- Take ratios from unfolded distributions! -----" << std::endl;
    cout << "\nNumerator variable: " << variableNUM << endl;
    cout << "Denominator variable: " << variableDENOM << endl;

    TString inputDir = "UnfoldedFiles";
    if (year == 9999) inputDir += "_Run2";
    else{
        inputDir += "_";
        inputDir += year;
    }
    std::cout << "Input directory: " << inputDir << std::endl;

    unfoldDir += "Ratio";
    unfoldDir += "_";
    if (year == 9999) unfoldDir += "Run2";
    else              unfoldDir += year;
    system("mkdir -p " + unfoldDir);

    // Open the input files ---

    TString inputFileNameNUM = inputDir + "/" + lepSel; 
    inputFileNameNUM += "_unfolded_" + variableNUM + "_" + algo;
    inputFileNameNUM += "_JetPtMin_";
    inputFileNameNUM += jetPtMin;
    inputFileNameNUM += "_JetEtaMax_";
    inputFileNameNUM += jetEtaMax;
    inputFileNameNUM += "_MGPYTHIA6_";
    inputFileNameNUM += doNormalized ? "_normalized" : "";   
    inputFileNameNUM += ".root";
    TFile *inputFileNUM = new TFile(inputFileNameNUM, "READ");

    TString inputFileNameDENOM = inputDir + "/" + lepSel; 
    inputFileNameDENOM += "_unfolded_" + variableDENOM + "_" + algo;
    inputFileNameDENOM += "_JetPtMin_";
    inputFileNameDENOM += jetPtMin;
    inputFileNameDENOM += "_JetEtaMax_";
    inputFileNameDENOM += jetEtaMax;
    inputFileNameDENOM += "_MGPYTHIA6_";
    inputFileNameDENOM += doNormalized ? "_normalized" : "";   
    inputFileNameDENOM += ".root";
    TFile *inputFileDENOM = new TFile(inputFileNameDENOM, "READ");

    // Create output file ---
    TString outputFileName = unfoldDir + "/" + lepSel; 
    outputFileName += "_RATIO_" + variableNUM + "_TO_" + variableDENOM;
    outputFileName += "_JetPtMin_";
    outputFileName += jetPtMin;
    outputFileName += "_JetEtaMax_";
    outputFileName += jetEtaMax;
    outputFileName += "_MGPYTHIA6_";
    outputFileName += doNormalized ? "_normalized" : "";   
    TFile *outputRootFile = new TFile(outputFileName + ".root", "RECREATE");

    // Get relevant histograms ---
    int nSysts = 18;
    TString name[] = {"Central", "JESUp", "JESDown", "PUUp", "PUDown", "XSECUp", "XSECDown", "JERUp", "JERDown", 
        "LepSFUp", "LepSFDown", "BtagSFUp", "BtagSFDown", "L1PrefireUp", "L1PrefireDown", "LumiUp", "LumiDown", "Unfolding"};

    TH1D *hRecDataBinsMergedNUM[nSysts]       = {}; 
    TH1D *hUnfDataNUM[nSysts]                 = {}; 
    TH1D *hRecDataBinsMergedDENOM[nSysts]     = {};
    TH1D *hUnfDataDENOM[nSysts]               = {};

    for (unsigned short iSyst = 0; iSyst < nSysts; ++iSyst){ 
        // -- Numerator
        hRecDataBinsMergedNUM[iSyst] = (TH1D*) inputFileNUM->Get("hRecDataBinsMerged_"+name[iSyst]);
        hRecDataBinsMergedNUM[iSyst]->SetDirectory(0);
        hUnfDataNUM[iSyst] = (TH1D*) inputFileNUM->Get("hUnfData_"+name[iSyst]);
        hUnfDataNUM[iSyst]->SetDirectory(0);
        // -- Denominator
        hRecDataBinsMergedDENOM[iSyst] = (TH1D*) inputFileDENOM->Get("hRecDataBinsMerged_"+name[iSyst]);
        hRecDataBinsMergedDENOM[iSyst]->SetDirectory(0);
        hUnfDataDENOM[iSyst] = (TH1D*) inputFileDENOM->Get("hUnfData_"+name[iSyst]);
        hUnfDataDENOM[iSyst]->SetDirectory(0);
    }

    // NLO FxFx cross sections
    TH1D *hGenSignalNLOFxFxNUM = (TH1D*) inputFileNUM->Get("hMadGenCrossSection");
    hGenSignalNLOFxFxNUM->SetDirectory(0);
    TH1D *hGenSignalNLOFxFxDENOM = (TH1D*) inputFileDENOM->Get("hMadGenCrossSection");
    hGenSignalNLOFxFxDENOM->SetDirectory(0);

    // LO MLM cross sections
    TH1D *hGenSignalLOMLMNUM  = (TH1D*) inputFileNUM->Get("hGen1CrossSection");
    hGenSignalLOMLMNUM->SetDirectory(0);
    TH1D *hGenSignalLOMLMDENOM = (TH1D*) inputFileDENOM->Get("hGen1CrossSection");
    hGenSignalLOMLMDENOM->SetDirectory(0);

    // NLO FxFx inclusive sample
    TH1D *hGenSignalNLOFxFxInclNUM = (TH1D*) inputFileNUM->Get("hGen3CrossSection");
    hGenSignalNLOFxFxInclNUM->SetDirectory(0);
    TH1D *hGenSignalNLOFxFxInclDENOM = (TH1D*) inputFileDENOM->Get("hGen3CrossSection");
    hGenSignalNLOFxFxInclDENOM->SetDirectory(0);

    // Take ratios, write out to file ---
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
    TH1D *hGenSignalNLOFxFxRATIO = (TH1D*) hGenSignalNLOFxFxNUM->Clone();
    hGenSignalNLOFxFxRATIO->Divide(hGenSignalNLOFxFxNUM, hGenSignalNLOFxFxDENOM, 1, 1, "B");
    hGenSignalNLOFxFxRATIO->SetName("GenSignalXSecRatio_NLOFxFx");
    hGenSignalNLOFxFxRATIO->SetTitle("MG_aMC FxFx + PY8 (#leq 2j NLO + PS)");
    hGenSignalNLOFxFxRATIO->GetYaxis()->SetTitle("Ratio");
    hGenSignalNLOFxFxRATIO->Write();
    // LO MLM (Madgraph)
    TH1D *hGenSignalLOMLMRATIO = (TH1D*) hGenSignalLOMLMNUM->Clone();
    hGenSignalLOMLMRATIO->Divide(hGenSignalLOMLMNUM, hGenSignalLOMLMDENOM, 1, 1, "B");
    hGenSignalLOMLMRATIO->SetName("GenSignalXSecRatio_LOMLM");
    hGenSignalLOMLMRATIO->SetTitle("MG_aMC + PY8 (#leq 4j LO + PS)");
    hGenSignalLOMLMRATIO->GetYaxis()->SetTitle("Ratio");
    hGenSignalLOMLMRATIO->Write();
    // NLO FxFx (MG5) inclusive sample
    TH1D *hGenSignalNLOFxFxInclRATIO = (TH1D*) hGenSignalNLOFxFxInclNUM->Clone();
    hGenSignalNLOFxFxInclRATIO->Divide(hGenSignalNLOFxFxInclNUM, hGenSignalNLOFxFxInclDENOM, 1, 1, "B");
    hGenSignalNLOFxFxInclRATIO->SetName("GenSignalXSecRatio_NLOFxFx_InclSample");
    hGenSignalNLOFxFxInclRATIO->SetTitle(NLOFXFXINCLUSIVE_LEGEND);
    hGenSignalNLOFxFxInclRATIO->GetYaxis()->SetTitle("Ratio");
    hGenSignalNLOFxFxInclRATIO->Write();


    std::cout << "\n<=========================================================================================>" << std::endl;

    // Calculate covariance matrices for RATIO ---
    TH2D *hCovRATIO[12] = {};
    int nCovs = 10;

    // -- Covariance matrix for the data statistical uncertainties
    // Get temp histogram for setup
    TH2D *hTemp = (TH2D*) inputFileNUM->Get("CovDataStat");
    hTemp->SetDirectory(0);
    // ---
    std::cout << "Calculating CovDataStat..." << std::endl;
    hCovRATIO[0] = (TH2D*) hTemp->Clone("CovDataStat");
    hCovRATIO[0]->Reset();
    // std::cout << "hCovRATIO[0]->GetNbinsX() = " << hCovRATIO[0]->GetNbinsX() << std::endl;
    // std::cout << "hCovRATIO[0]->GetNbinsY() = " << hCovRATIO[0]->GetNbinsY() << std::endl;
    for (int i(1); i < hCovRATIO[0]->GetNbinsX()+1; i++){
        for (int j(1); j < hCovRATIO[0]->GetNbinsY()+1; j++){
            if (i == j){ // diagonal entries
                double errSquared = pow(hUnfDataRATIO[0]->GetBinError(i), 2.0);
                hCovRATIO[0]->SetBinContent(i, j, errSquared);
            }
            // no off-diagonal entries yet!
            // (these are correlations across bins)
        }
    }
    
    // -- Covariance matrix for the (uncorrelated) statistical uncertainties of the response matrix
    std::cout << "Calculating CovMCStat..." << std::endl;
    hCovRATIO[1] = (TH2D*) hTemp->Clone("CovMCStat");
    hCovRATIO[1]->Reset();
    // get covariance matrices for NUM and DENOM
    TH2D *hMCStatUncertNUM = (TH2D*) inputFileNUM->Get("CovMCStat");
    TH2D *hMCStatUncertDENOM = (TH2D*) inputFileDENOM->Get("CovMCStat");
    hMCStatUncertNUM->SetDirectory(0);
    hMCStatUncertDENOM->SetDirectory(0);
    // now calculate
    for (int i(1); i <= hCovRATIO[1]->GetNbinsX(); i++){
        for (int j(1); j <= hCovRATIO[1]->GetNbinsY(); j++){
            if (i == j){ // diagonal entries
                // assume contributions are uncorrelated for now 
                // (so just use standard error propagation, which is the conservtive estimate)
                double fracErrNUM = sqrt(hMCStatUncertNUM->GetBinContent(i,j))/hUnfDataNUM[0]->GetBinContent(i);
                double fracErrDENOM = sqrt(hMCStatUncertDENOM->GetBinContent(i,j))/hUnfDataDENOM[0]->GetBinContent(i);
                double absValRatio = fabs(hUnfDataRATIO[0]->GetBinContent(i));
                double errRatio = absValRatio * sqrt( pow(fracErrNUM, 2.0) + pow(fracErrDENOM, 2.0) );
                hCovRATIO[1]->SetBinContent( i, j, pow(errRatio, 2.0) );
            }
            // no off-diagonal entries yet!
            // (these are correlations across bins)
        }
    }

    // -- Covariance matrices for systematic variations
    if (hUnfDataRATIO[1])  hCovRATIO[2]  = makeCovFromUpAndDown(hUnfDataRATIO[0], hUnfDataRATIO[1], hUnfDataRATIO[2], "CovJES");
    if (hUnfDataRATIO[3])  hCovRATIO[3]  = makeCovFromUpAndDown(hUnfDataRATIO[0], hUnfDataRATIO[3], hUnfDataRATIO[4], "CovPU");
    if (hUnfDataRATIO[5])  hCovRATIO[4]  = makeCovFromUpAndDown(hUnfDataRATIO[0], hUnfDataRATIO[5], hUnfDataRATIO[6], "CovXSEC");
    if (hUnfDataRATIO[7])  hCovRATIO[5]  = makeCovFromUpAndDown(hUnfDataRATIO[0], hUnfDataRATIO[7], hUnfDataRATIO[8], "CovJER");
    if (hUnfDataRATIO[9])  hCovRATIO[6]  = makeCovFromUpAndDown(hUnfDataRATIO[0], hUnfDataRATIO[9], hUnfDataRATIO[10], "CovLepSF");
    // if (hUnfDataRATIO[11]) hCovRATIO[7] = makeCovFromUpAndDown(hUnfDataRATIO[0], hUnfDataRATIO[11], hUnfDataRATIO[12], "CovBtagSF"); // not currently using
    if (hUnfDataRATIO[13]) hCovRATIO[8]  = makeCovFromUpAndDown(hUnfDataRATIO[0], hUnfDataRATIO[13], hUnfDataRATIO[14], "CovL1Prefire");
    if (hUnfDataRATIO[15]) hCovRATIO[9]  = makeCovFromUpAndDown(hUnfDataRATIO[0], hUnfDataRATIO[15], hUnfDataRATIO[16], "CovLumi");
    if (hUnfDataRATIO[17]) hCovRATIO[10] = makeCovFromUpAndDown(hUnfDataRATIO[0], hUnfDataRATIO[17], hUnfDataRATIO[0], "CovUnfolding"); // ALW: think the argument needs to be: hUnfDataRATIO[0], hUnfDataRATIO[17], hUnfDataRATIO[17]
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

    // Plot central unfolded distribution (w/ stat & syst errors), theory predictions ---
    TString variableRATIO = variableNUM + "_TO_" + variableDENOM;
    TCanvas *crossSectionPlot = makeCrossSectionPlot(lepSel, year, variableRATIO, true, doNormalized, hUnfDataRATIO[0], hCovRATIO[11], hGenSignalNLOFxFxRATIO, hGenSignalLOMLMRATIO);
    crossSectionPlot->Draw();
    crossSectionPlot->SaveAs(outputFileName + ".pdf");

    // Print out break down of errors ---
    std::cout << "\nError Breakdown: " << std::endl;
    for (int i = 1; i <= hUnfDataRATIO[0]->GetNbinsX(); ++i){
        std::cout << "bin #" << i << ": " << hUnfDataRATIO[0]->GetBinContent(i);
        for (int j = 0; j <= 11; ++j){
            if(hCovRATIO[j]) std::cout << " +/- " << sqrt(hCovRATIO[j]->GetBinContent(i,i))*100./hUnfDataRATIO[0]->GetBinContent(i) << "%";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    std::cout << "\n<=========================================================================================>" << std::endl;

    // Save other things --- 
    outputRootFile->cd();
    
    // Write out covariance matrices ---
    for (int i = 0; i <= 11; ++i) if (hCovRATIO[i]) hCovRATIO[i]->Write();

    outputRootFile->Close();
    if (inputFileNUM) inputFileNUM->Close();
    if (inputFileDENOM) inputFileDENOM->Close();

    std::cout << "\nFinished!\n" << std::endl;
}
