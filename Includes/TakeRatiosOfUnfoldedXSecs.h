#ifndef _TAKERATIOSOFUNFOLDEDXSECS_h_
#define _TAKERATIOSOFUNFOLDEDXSECS_h_

#include <tuple>
#include <RooUnfold.h>
#include <TCanvas.h>
#include <TString.h>

void TakeRatiosOfUnfoldedXSecs(TString lepSel, int year, TString algo, TString histoDir, TString unfoldDir, int jetPtMin, int jetEtaMax, TString variableNUM = "", TString variableDENOM = "", bool doNormalized = false, int whichSyst = -1);

#endif
