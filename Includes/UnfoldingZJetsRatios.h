#ifndef _UNFOLDINGZJETSRATIOS_h_
#define _UNFOLDINGZJETSRATIOS_h_

#include <tuple>
#include <RooUnfold.h>
#include <TCanvas.h>
#include <TString.h>

void UnfoldingZJetsRatios(TString lepSel, int year, TString algo, TString histoDir, TString unfoldDir, int jetPtMin, int jetEtaMax, TString variableNUM = "", TString variableDENOM = "", bool noramlized = false, int whichSyst = -1);

#endif
