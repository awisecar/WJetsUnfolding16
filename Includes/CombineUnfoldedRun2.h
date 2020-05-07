#ifndef _COMBINEUNFOLDEDRUN2_h_
#define _COMBINEUNFOLDEDRUN2_h_

#include <tuple>
#include <RooUnfold.h>
#include <TCanvas.h>
#include <TString.h>

TCanvas* plotXSecComparison(TString variable, TH1D *h2016, TH1D *h2017, TH1D *h2018, TH1D *hRun2);
TCanvas* plotUnfRecoComparison(TString variable, int year, TH1D *hUnf, TH1D *hReco, TH1D *hGen);
void CombineUnfoldedRun2(TString lepSel, int year, TString algo, TString histoDir, TString unfoldDir, int jetPtMin, int jetEtaMax, TString variable = "", bool noramlized = false, int whichSyst = -1);

#endif