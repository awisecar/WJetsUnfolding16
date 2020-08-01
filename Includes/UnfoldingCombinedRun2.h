#ifndef _UNFOLDINGCOMBINEDRUN2_h_
#define _UNFOLDINGCOMBINEDRUN2_h_

#include <tuple>
#include <RooUnfold.h>
#include <TCanvas.h>
#include <TString.h>

void UnfoldingCombinedRun2(TString lepSel, int year, TString algo, TString histoDir, TString unfoldDir, int jetPtMin, int jetEtaMax, TString variable = "", bool doNormalized = false, int whichSyst = -1, bool isClosureTest = false);

#endif