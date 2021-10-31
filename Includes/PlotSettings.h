#ifndef _PLOTSETTINGS_H_
#define _PLOTSETTINGS_H_

#include <iostream>
#include <TGraphAsymmErrors.h>
#include <TGraphErrors.h>
#include <TH1.h>
#include <TH2.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TLegendEntry.h>
#include <TMath.h>
#include <TAxis.h>
#include <TLatex.h>

const int ZJetsFillColor[3] = {kBlue-10, kOrange-2, kGreen-8};
const int ZJetsPdfFillColor[3] = {kBlue-6, kOrange-5, kGreen-2};
const int ZJetsLineColor[3] = {kBlue, kOrange+10, kGreen+3};;
const int ZJetsMarkerColor[3] = {ZJetsLineColor[0], ZJetsLineColor[1], ZJetsLineColor[2]};
const int ZJetsMarkerStyle[3] = {24, 25, 26};
const int ZJetsFillStyle = 1001;
//const double TheoScaleExclusive[11] = {0.062,0.108,0.147,0.168,0.195,0.198,0.237,0.306,0.0,0.0,0.0}; //exclusive (0-7 Jet scales)
//const double TheoScaleInclusive[11] = {0.04,0.062,0.089,0.118,0.119,0.155,0.123,0.203,0.0,0.0,0.0}; //inclusive (0-7 Jet scales)


void setAndDrawTPad(TString canvasName, TPad *plot, int plotNumber, int numbOfGenerator, bool isRatio);
std::string getYaxisTitle(bool doNormalized, const TH1D *gen1, bool isRatio);
void customizeLegend(TLegend *legend, int numbOfGenerator);
void customizeLegend(TLegend *legend, int genNumb, int numbOfGenerator);
void configYaxis(TH1D *grCentralSyst, TH1D *gen1, TH1D *gen2 = NULL, TH1D *gen3 = NULL, bool isRatio = false);
void configXaxis(TH1D *grCentralSyst, TH1D *gen1, TString variable);
void customizeCentral(TGraphAsymmErrors *grCentral, bool ratio);
void customizeCentral(TGraphAsymmErrors *grCentral, TLegend *legend, TString legText = "");
void customizeGenHist(TH1D *gen, int genNumb, TLegend *legend, TString legText);
void customizeGenGraph(TH1D *hSyst, TGraphAsymmErrors *gen, TGraphAsymmErrors *gPDF, int genNumb, TString yTitle, int numbOfGenerator, TLegend *legend = NULL, bool isClosureTest = false, bool isRatio = false);
TGraphAsymmErrors* createGrFromHist(const TH1D *h);
TGraphAsymmErrors* createRatioGraph(const TGraphAsymmErrors* grCentral);
TGraphErrors* createRatioGraph(const TGraphErrors* grCentral);
TGraphAsymmErrors *createGenToCentral(const TH1D *gen, const TGraphAsymmErrors *grCentral);
TGraphAsymmErrors* createPDFSystGraph(const TH1D *hPDFUp, const TH1D *hPDFDown, const TGraphAsymmErrors *grGenToCentral);
TGraphAsymmErrors* createScaleSystGraph(TString lepSel, TString variable, const TGraphAsymmErrors *grGenToCentral);
TGraphAsymmErrors* createScaleSystGraphNNLO1j(TString lepSel, TString variable, const TGraphAsymmErrors *grGenToCentral);
TGraphAsymmErrors* createTotSystGraphAMCNLO(TString lepSel, TString variable, const TGraphAsymmErrors *grGenToCentral, bool isRatio);
TCanvas* makeCrossSectionPlot(TString lepSel, int year, TString variable, bool isRatio, bool doNormalized, TH1D *hData, TH2D *hCovSyst, TH1D *hGen, TH1D *hGen1 = NULL, bool isClosureTest = false, TH1D *hGen2 = NULL, double integratedLumi = -1);
void createTitleVariableAnddSigma(TString variable, bool doNormalized, TString xtitle, TString &title, TString &var, TString &dSigma) ;
#endif
