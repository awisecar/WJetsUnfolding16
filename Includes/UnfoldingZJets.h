#ifndef _UNFOLDINGZJETS_h_
#define _UNFOLDINGZJETS_h_

#include <tuple>
#include <RooUnfold.h>
#include <TCanvas.h>
#include <TString.h>

void UnfoldingZJets(TString lepSel, int year, TString algo, TString histoDir, TString unfoldDir, int jetPtMin, int jetEtaMax, TString variable = "", bool noramlized = false, int whichSyst = -1);
void TUnfoldData(const TString lepSel, const TString algo, TH2D* resp, TH1D* hRecDataMinusFakes, TH1D* &hRecDataBinsMerged, TH1D* &hUnfData, TH1D* &hUnfDataNoScale, TH2D* &hUnfDataStatCov, TH2D* &hUnfDataStatCovNoScale, TH2D* &hUnfMCStatCov, TH2D* &hUnfMCStatCovNoScale, TString name, double integratedLumi, TH1D* hGenDYJets, TH1D* hMissDYJets, bool logy = false, bool doRatioUnfold = false, int year = 2016);
void unfoldingFileSelector(int iSyst, int &iData, int &iBg, int &iGen, int &iResp);
TH2D* makeCovFromUpAndDown(const TH1D* hUnfDataCentral, const TH1D* hUnfDataUp, const TH1D* hUnfDataDown, TString name);
TH1D* makeCrossSectionHist(TH1D* hGenDYJets, double integratedLumi);
void createTable(TString outputFileName, TString lepSel, TString variable, bool doNormalized, TH1D *hUnfData, TH2D *hCov[]);
void createSystPlots(TString outputFileName, TString lepSel, TString variable, TH1D* hUnf[], bool logy = false);
void RemoveFakes(TH1D* &hRecData, TH1D* hRecDYJets, TH1D* hFakDYJets);
void normalizeTH2D(TH2D *h);
void createInclusivePlots(bool doNormalized, TString outputFileName, TString lepSel, TH1D *hUnfData, TH2D *hCov[], TH1D *hMadGenCrossSection, TH1D *hSheGenCrossSection);
void plotRespMat(TH2D *hResp, TString variable, TString unfoldDir, bool addSwitch, TH1D *hGenDYJets);


#endif