#include "PlotSettings.h"
#include "TStyle.h"
#include "TFile.h"
#include "TMath.h"
#include "ConfigVJets.h"

extern ConfigVJets cfg;

void setAndDrawTPad(TString canvasName, TPad *plot, int plotNumber, int numbOfGenerator, bool isRatio){

    if (numbOfGenerator == 1) {
        if (plotNumber == 1) {
            plot->SetPad(0.01, 0.35, 0.99, 0.99);
            plot->SetTopMargin(0.11);
            plot->SetBottomMargin(0.005);
        }
        else if (plotNumber == 2) {
            plot->SetPad(0.01, 0.01, 0.99, 0.35);
            plot->SetTopMargin(0.0);
            plot->SetBottomMargin(0.3);
        }
    }
    else if (numbOfGenerator == 2) {
        if (plotNumber == 1) {
            plot->SetPad(0.01, 0.45, 0.99, 0.99);
            plot->SetTopMargin(0.11);
            plot->SetBottomMargin(0.005);
        }
        else if (plotNumber == 2) {
            plot->SetPad(0.01, 0.27, 0.99, 0.45);
            plot->SetTopMargin(0.0);
            plot->SetBottomMargin(0.0);
        }
        else if (plotNumber == 3) {
            plot->SetPad(0.01, 0.01, 0.99, 0.27);
            plot->SetTopMargin(0.0);
            plot->SetBottomMargin(0.3);
        }

    }
    else if (numbOfGenerator == 3) {
        if (plotNumber == 1) {
            plot->SetPad(0.01, 0.55, 0.99, 0.99);
            plot->SetTopMargin(0.11);
            plot->SetBottomMargin(0.005);
        }
        else if (plotNumber == 2) {
            plot->SetPad(0.01, 0.39, 0.99, 0.55);
            plot->SetTopMargin(0.0);
            plot->SetBottomMargin(0.0);
        }
        else if (plotNumber == 3) {
            plot->SetPad(0.01, 0.23, 0.99, 0.39);
            plot->SetTopMargin(0.0);
            plot->SetBottomMargin(0.0);
        }
        else if (plotNumber == 4) {
            plot->SetPad(0.01, 0.01, 0.99, 0.23);
            plot->SetTopMargin(0.0);
            plot->SetBottomMargin(0.3);
        }
    }

    if (plotNumber == 1 && (canvasName.Index("Eta") < 0 && canvasName.Index("AbsRapidity") < 0 && canvasName.Index("dPhiLepJet2") < 0 && !isRatio)) plot->SetLogy();

    plot->SetLeftMargin(0.13);
    plot->SetRightMargin(0.07);
    plot->SetFillStyle(0);
    plot->Draw();
    plot->cd();
}

void customizeLegend(TLegend *legend, int numbOfGenerator){
    legend->SetFillColor(0);
    legend->SetFillStyle(1001);
    legend->SetBorderSize(1);
    if (numbOfGenerator == 1) {
        legend->SetX1(0.39);
        legend->SetY1(0.84);
        legend->SetX2(0.96);
        legend->SetY2(0.98);
        legend->SetTextSize(.029);
    }
    else if (numbOfGenerator == 2) {
        legend->SetX1(0.39);
        legend->SetY1(0.77);
        legend->SetX2(0.96);
        legend->SetY2(0.98);
        legend->SetTextSize(.034);
    }
    else if (numbOfGenerator == 3) {
        legend->SetX1(0.44);
        legend->SetY1(0.7);
        legend->SetX2(0.96);
        legend->SetY2(0.98);
        legend->SetTextSize(.042);
    }
}

void customizeLegend(TLegend *legend, int genNumb, int numbOfGenerator){

    legend->SetFillColor(0);
    legend->SetFillStyle(ZJetsFillStyle);
    legend->SetBorderSize(0);
    //legend->SetTextSize(.075);
    legend->SetTextSize(.12);

    if (genNumb == numbOfGenerator) {
        if (numbOfGenerator == 1) {
            legend->SetY1(0.35);
            legend->SetX2(0.43);
            legend->SetY2(0.45);
            //legend->SetTextSize(0.06);
            legend->SetTextSize(0.08);
        }

        if (numbOfGenerator == 2) {
            legend->SetY1(0.34);
            legend->SetX2(0.43);
            legend->SetY2(0.45);
            //legend->SetTextSize(0.06);
            legend->SetTextSize(0.08);
        }
        if (numbOfGenerator == 3) {
            legend->SetY1(0.34);
            legend->SetX2(0.43);
            legend->SetY2(0.45);
            //legend->SetTextSize(0.06);
            legend->SetTextSize(0.09);
        }
    }

}

void customizeCentral(TGraphAsymmErrors *grCentral, bool ratio){
    customizeCentral(grCentral, (TLegend*) NULL);
    if (ratio) grCentral->SetMarkerSize(0);
}

void customizeCentral(TGraphAsymmErrors *grCentral, TLegend *legend, TString legText){
    grCentral->SetLineColor(kBlack);
    grCentral->SetLineWidth(2);
    grCentral->SetMarkerStyle(20);
    grCentral->SetFillColor(12);
    gStyle->SetHatchesSpacing(1.5);
    gStyle->SetHatchesLineWidth(2);
    grCentral->SetFillStyle(3354);
    grCentral->SetMarkerColor(kBlack);

    grCentral->GetXaxis()->SetTitleOffset(1.0);
    grCentral->GetXaxis()->SetTitleSize(0.05);
    grCentral->GetXaxis()->SetLabelSize(0.0);
    grCentral->GetXaxis()->SetLabelFont(42);
    grCentral->GetXaxis()->SetTitleFont(42);

    grCentral->GetYaxis()->SetTitleOffset(1.1);
    grCentral->GetYaxis()->SetTitleSize(0.07);
    grCentral->GetYaxis()->SetLabelSize(0.05);
    grCentral->GetYaxis()->SetLabelFont(42);
    grCentral->GetYaxis()->SetTitleFont(42);

    grCentral->SetTitle();
    grCentral->GetXaxis()->SetTitle();
    if (legend) legend->AddEntry(grCentral, legText, "PLEF");

}

TGraphAsymmErrors* createGrFromHist(const TH1D *h){
    int nPoints = h->GetNbinsX();
    double *xCoor = new double[nPoints];
    double *yCoor = new double[nPoints];
    double *xErr  = new double[nPoints];
    double *yErr = new double[nPoints];

    for (int i(0); i < nPoints; i++) {
        xCoor[i] = h->GetBinCenter(i+1);
        xErr[i]  = 0.5*h->GetBinWidth(i+1);
        yCoor[i] = h->GetBinContent(i+1);
        yErr[i] = h->GetBinError(i+1);
    }

    TGraphAsymmErrors *gr = new TGraphAsymmErrors(nPoints, xCoor, yCoor, xErr, xErr, yErr, yErr);

    delete [] xCoor; delete [] yCoor; delete [] xErr; delete [] yErr; 
    return gr;
}

//======================================================================
// This function creates a TGraphAsymmErrors from a TGraphAsymmErrors.
// The output TGraph has all yCoor set to 1. and y low and high errors
// representing the relative errors of the input TGraph.
//======================================================================
TGraphAsymmErrors* createRatioGraph(const TGraphAsymmErrors* grCentral){       
    int nPoints = grCentral->GetN();
    double *xCoor = new double[nPoints];
    double *yCoor = new double[nPoints];
    double *xErr  = new double[nPoints];
    double *yErrL = new double[nPoints];
    double *yErrH = new double[nPoints];

    for (int i(0); i < nPoints; i++) {
        grCentral->GetPoint(i, xCoor[i], yCoor[i]);
        xErr[i] = grCentral->GetErrorXlow(i);
        yErrL[i] = grCentral->GetErrorYlow(i)/yCoor[i];
        yErrH[i] = grCentral->GetErrorYhigh(i)/yCoor[i];
        yCoor[i] = 1.;
    }   

    TGraphAsymmErrors *grCentralRatio = new TGraphAsymmErrors(nPoints, xCoor, yCoor, xErr, xErr, yErrL, yErrH);

    delete [] xCoor; delete [] yCoor; delete [] xErr; delete [] yErrL; delete [] yErrH; 
    return grCentralRatio;
}

//======================================================================
// This function creates a TGraphErrors from a TGraphErrors.
// The output TGraph has all yCoor set to 1. and y errors
// representing the relative errors of the input TGraph.
//======================================================================
TGraphErrors* createRatioGraph(const TGraphErrors* grCentral){       
    int nPoints = grCentral->GetN();
    double *xCoor = new double[nPoints];
    double *yCoor = new double[nPoints];
    double *xErr  = new double[nPoints];
    double *yErr = new double[nPoints];

    for (int i(0); i < nPoints; i++) {
        grCentral->GetPoint(i, xCoor[i], yCoor[i]);
        xErr[i] = grCentral->GetErrorX(i);
        yErr[i] = grCentral->GetErrorY(i)/yCoor[i];
        yCoor[i] = 1.;
    }   

    TGraphErrors *grCentralRatio = new TGraphErrors(nPoints, xCoor, yCoor, xErr, yErr);

    delete [] xCoor; delete [] yCoor; delete [] xErr; delete [] yErr; 
    return grCentralRatio;
}

//================================================================================
// This function creates a TGraphAsymmErrors from a TH1D and a TGraphAsymmErrors.
// The output TGraph is the ratio of the TH1D by the TGraphAsymmErrors.
//================================================================================
TGraphAsymmErrors *createGenToCentral(const TH1D *gen, const TGraphAsymmErrors *grCentral){
    int nPoints = grCentral->GetN();
    double *xCoor = new double[nPoints];
    double *yCoor = new double[nPoints];
    double *xErr  = new double[nPoints];
    double *yErr  = new double[nPoints];

    for (int i(0); i < nPoints; i++) {
        grCentral->GetPoint(i, xCoor[i], yCoor[i]);
        xErr[i] = grCentral->GetErrorXlow(i);
        yErr[i] = 0.;
        if (yCoor[i] != 0) {
            yErr[i]  = gen->GetBinError(i+1)/yCoor[i];
            yCoor[i] = gen->GetBinContent(i+1)/yCoor[i];
        }
    }

    TGraphAsymmErrors *grGenToCentral = new TGraphAsymmErrors(nPoints, xCoor, yCoor, xErr, xErr, yErr, yErr);
    delete [] xCoor; delete [] yCoor; delete [] xErr; delete [] yErr; 
    return grGenToCentral;
}

TGraphAsymmErrors* createScaleSystGraph(TString lepSel, TString variable, const TGraphAsymmErrors *grGenToCentral){
    int nPoints = grGenToCentral->GetN();
    double *xCoor    = new double[nPoints];
    double *yCoor    = new double[nPoints];
    double *xErr     = new double[nPoints];
    double *yErrUp   = new double[nPoints];
    double *yErrDown = new double[nPoints];

    TFile *fDE[7];

    TString histoDir    = cfg.getS("histoDir");

    if (lepSel == "DE" || lepSel == "") {
        fDE[0] = new TFile(histoDir + "/DE_8TeV_DYJetsToLL_M-50_TuneCUETP8M1_8TeV-amcatnloFXFX-Bonzai_fixed_allWeights_dR_TrigCorr_1_Syst_0_JetPtMin_30_JetEtaMax_24_muR_1_muF_1.root");
        fDE[1] = new TFile(histoDir + "/DE_8TeV_DYJetsToLL_M-50_TuneCUETP8M1_8TeV-amcatnloFXFX-Bonzai_fixed_allWeights_dR_TrigCorr_1_Syst_0_JetPtMin_30_JetEtaMax_24_muR_0.5_muF_0.5.root");
        fDE[2] = new TFile(histoDir + "/DE_8TeV_DYJetsToLL_M-50_TuneCUETP8M1_8TeV-amcatnloFXFX-Bonzai_fixed_allWeights_dR_TrigCorr_1_Syst_0_JetPtMin_30_JetEtaMax_24_muR_0.5_muF_1.root");
        fDE[3] = new TFile(histoDir + "/DE_8TeV_DYJetsToLL_M-50_TuneCUETP8M1_8TeV-amcatnloFXFX-Bonzai_fixed_allWeights_dR_TrigCorr_1_Syst_0_JetPtMin_30_JetEtaMax_24_muR_1_muF_0.5.root");
        fDE[4] = new TFile(histoDir + "/DE_8TeV_DYJetsToLL_M-50_TuneCUETP8M1_8TeV-amcatnloFXFX-Bonzai_fixed_allWeights_dR_TrigCorr_1_Syst_0_JetPtMin_30_JetEtaMax_24_muR_1_muF_2.root");
        fDE[5] = new TFile(histoDir + "/DE_8TeV_DYJetsToLL_M-50_TuneCUETP8M1_8TeV-amcatnloFXFX-Bonzai_fixed_allWeights_dR_TrigCorr_1_Syst_0_JetPtMin_30_JetEtaMax_24_muR_2_muF_1.root");
        fDE[6] = new TFile(histoDir + "/DE_8TeV_DYJetsToLL_M-50_TuneCUETP8M1_8TeV-amcatnloFXFX-Bonzai_fixed_allWeights_dR_TrigCorr_1_Syst_0_JetPtMin_30_JetEtaMax_24_muR_2_muF_2.root");
    }

    TFile *fDMu[7];
    if (lepSel == "DMu" || lepSel == "") {
        fDMu[0] = new TFile(histoDir + "/DMu_8TeV_DYJetsToLL_M-50_TuneCUETP8M1_8TeV-amcatnloFXFX-Bonzai_fixed_allWeights_dR_TrigCorr_1_Syst_0_JetPtMin_30_JetEtaMax_24_muR_1_muF_1.root");
        fDMu[1] = new TFile(histoDir + "/DMu_8TeV_DYJetsToLL_M-50_TuneCUETP8M1_8TeV-amcatnloFXFX-Bonzai_fixed_allWeights_dR_TrigCorr_1_Syst_0_JetPtMin_30_JetEtaMax_24_muR_0.5_muF_0.5.root");
        fDMu[2] = new TFile(histoDir + "/DMu_8TeV_DYJetsToLL_M-50_TuneCUETP8M1_8TeV-amcatnloFXFX-Bonzai_fixed_allWeights_dR_TrigCorr_1_Syst_0_JetPtMin_30_JetEtaMax_24_muR_0.5_muF_1.root");
        fDMu[3] = new TFile(histoDir + "/DMu_8TeV_DYJetsToLL_M-50_TuneCUETP8M1_8TeV-amcatnloFXFX-Bonzai_fixed_allWeights_dR_TrigCorr_1_Syst_0_JetPtMin_30_JetEtaMax_24_muR_1_muF_0.5.root");
        fDMu[4] = new TFile(histoDir + "/DMu_8TeV_DYJetsToLL_M-50_TuneCUETP8M1_8TeV-amcatnloFXFX-Bonzai_fixed_allWeights_dR_TrigCorr_1_Syst_0_JetPtMin_30_JetEtaMax_24_muR_1_muF_2.root");
        fDMu[5] = new TFile(histoDir + "/DMu_8TeV_DYJetsToLL_M-50_TuneCUETP8M1_8TeV-amcatnloFXFX-Bonzai_fixed_allWeights_dR_TrigCorr_1_Syst_0_JetPtMin_30_JetEtaMax_24_muR_2_muF_1.root");
        fDMu[6] = new TFile(histoDir + "/DMu_8TeV_DYJetsToLL_M-50_TuneCUETP8M1_8TeV-amcatnloFXFX-Bonzai_fixed_allWeights_dR_TrigCorr_1_Syst_0_JetPtMin_30_JetEtaMax_24_muR_2_muF_2.root");
    }

    TH1D *hDE[7], *hDMu[7];
    for (int i(0); i < 7; ++i) {
        if (lepSel == "DE" || lepSel == "") hDE[i] = (TH1D*) fDE[i]->Get("gen" + variable);
        if (lepSel == "DMu" || lepSel == "") hDMu[i] = (TH1D*) fDMu[i]->Get("gen" + variable);
    }

    for (int i(1); i < 7; ++i) {
        if (lepSel == "DE" || lepSel == "") hDE[i]->Divide(hDE[0]);
        if (lepSel == "DMu" || lepSel == "") hDMu[i]->Divide(hDMu[0]);
    }

    int nBins = (lepSel == "DE" || lepSel == "") ? hDE[0]->GetNbinsX() : hDMu[0]->GetNbinsX();
    TH1D *hErrorsUp = (lepSel == "DE" || lepSel == "") ? (TH1D*) hDE[0]->Clone("hErrorsUp") : (TH1D*) hDMu[0]->Clone("hErrorsUp");
    hErrorsUp->Reset();
    TH1D *hErrorsDown = (lepSel == "DE" || lepSel == "") ? (TH1D*) hDE[0]->Clone("hErrorsDown") : (TH1D*) hDMu[0]->Clone("hErrorsDown");
    hErrorsDown->Reset();
    for (int i(1); i <= nBins; ++i) {
        hErrorsUp->SetBinContent(i, 1);
        hErrorsDown->SetBinContent(i, 1);
        for (int j(1); j < 7; ++j) {
            if (lepSel == "DE") {
                hErrorsUp->SetBinContent(i, TMath::Max(hErrorsUp->GetBinContent(i), hDE[j]->GetBinContent(i)));
                hErrorsDown->SetBinContent(i, TMath::Min(hErrorsDown->GetBinContent(i), hDE[j]->GetBinContent(i)));
            }
            else if (lepSel == "DMu") {
                hErrorsUp->SetBinContent(i, TMath::Max(hErrorsUp->GetBinContent(i), hDMu[j]->GetBinContent(i)));
                hErrorsDown->SetBinContent(i, TMath::Min(hErrorsDown->GetBinContent(i), hDMu[j]->GetBinContent(i)));
            }
            else if (lepSel == "") {
                hErrorsUp->SetBinContent(i, TMath::Max(hErrorsUp->GetBinContent(i), 0.5*(hDE[j]->GetBinContent(i)+hDMu[j]->GetBinContent(i))));
                hErrorsDown->SetBinContent(i, TMath::Min(hErrorsDown->GetBinContent(i), 0.5*(hDE[j]->GetBinContent(i)+hDMu[j]->GetBinContent(i))));
            }
        }
    }

    for (int i(0); i < nPoints; i++) {
        grGenToCentral->GetPoint(i, xCoor[i], yCoor[i]);

        xErr[i] = grGenToCentral->GetErrorXlow(i);

        yErrUp[i] = pow(grGenToCentral->GetErrorYhigh(i), 2);
        std::cout << "X coor: " << xCoor[i] << "  scale uncertainty up: " << fabs(hErrorsUp->GetBinContent(i+1) - 1) << std::endl;
        yErrUp[i] += pow(fabs(hErrorsUp->GetBinContent(i+1) - 1), 2);
        yErrUp[i] = sqrt(yErrUp[i]);

        yErrDown[i] = pow(grGenToCentral->GetErrorYlow(i), 2);
        std::cout << "X coor: " << xCoor[i] << "  scale uncertainty down: " << fabs(hErrorsDown->GetBinContent(i+1) - 1) << std::endl;
        yErrDown[i] += pow(fabs(1 - hErrorsDown->GetBinContent(i+1)), 2);
        yErrDown[i] = sqrt(yErrDown[i]);

    }

    TGraphAsymmErrors *grScaleSyst = new TGraphAsymmErrors(nPoints, xCoor, yCoor, xErr, xErr, yErrDown, yErrUp);
    delete [] xCoor; delete [] yCoor; delete [] xErr; delete [] yErrDown; delete [] yErrUp;
    if (lepSel == "DE" || lepSel == "") {
        for (int i(0); i < 7; ++i) fDE[i]->Close();
    }
    if (lepSel == "DMu" || lepSel == "") {
        for (int i(0); i < 7; ++i) fDMu[i]->Close();
    }
    return grScaleSyst;

}

TGraphAsymmErrors* createPDFSystGraph(const TH1D *hPDFUp, const TH1D *hPDFDown, const TGraphAsymmErrors *grGenToCentral){
    int nPoints = grGenToCentral->GetN();
    double *xCoor    = new double[nPoints];
    double *yCoor    = new double[nPoints];
    double *xErr     = new double[nPoints];
    double *yErrUp   = new double[nPoints];
    double *yErrDown = new double[nPoints];

    for (int i(0); i < nPoints; i++) {
        grGenToCentral->GetPoint(i, xCoor[i], yCoor[i]);

        xErr[i] = grGenToCentral->GetErrorXlow(i);

        yErrUp[i] = pow(grGenToCentral->GetErrorYhigh(i), 2);
        yErrUp[i] += pow(hPDFUp->GetBinContent(i+1)*yCoor[i], 2); 
        yErrUp[i] = sqrt(yErrUp[i]);

        yErrDown[i] = pow(grGenToCentral->GetErrorYlow(i), 2);
        yErrDown[i] += pow(hPDFDown->GetBinContent(i+1)*yCoor[i], 2); 
        yErrDown[i] = sqrt(yErrDown[i]);
    }

    TGraphAsymmErrors *grPDFSyst = new TGraphAsymmErrors(nPoints, xCoor, yCoor, xErr, xErr, yErrDown, yErrUp);
    delete [] xCoor; delete [] yCoor; delete [] xErr; delete [] yErrDown; delete [] yErrUp;
    return grPDFSyst;
}

//void customizeGenGraph(TGraphAsymmErrors *gen, TGraphAsymmErrors *gPDF, int genNum, TString yTitle, int numbOfGenerator, TLegend *legend)
void customizeGenGraph(TH1D *hSyst, TGraphAsymmErrors *gen, TGraphAsymmErrors *gPDF, int genNum, TString yTitle, int numbOfGenerator, TLegend *legend, bool isClosureTest){
    //hSyst->GetYaxis()->SetRangeUser(0.2, 1.8);



    // y-axis range on the theory/data ratio subplots
    if (!isClosureTest) hSyst->GetYaxis()->SetRangeUser(0.01, 2.15); // NOMINAL
    else hSyst->GetYaxis()->SetRangeUser(0.851, 1.149); // zoomed in
    



    hSyst->GetYaxis()->SetNdivisions(507);
    hSyst->GetYaxis()->SetLabelSize(0.154);
    hSyst->GetYaxis()->SetTitle(yTitle);
    hSyst->GetYaxis()->SetTitleSize(0.116);
    hSyst->GetYaxis()->SetTitleOffset(0.45);
    hSyst->GetYaxis()->CenterTitle();
    //hSyst->SetTitle();
    gen->SetFillColor(ZJetsFillColor[genNum-1]);
    gen->SetFillStyle(ZJetsFillStyle);
    gen->SetLineColor(ZJetsLineColor[genNum-1]);
    gen->SetLineWidth(2);
    gen->SetMarkerColor(ZJetsLineColor[genNum-1]);
    gen->SetMarkerStyle(ZJetsMarkerStyle[genNum-1]);

    if (genNum == numbOfGenerator) {
        hSyst->GetYaxis()->SetLabelSize(0.09);
        if (numbOfGenerator == 2) hSyst->GetYaxis()->SetLabelSize(0.105);
        if (numbOfGenerator == 3) hSyst->GetYaxis()->SetLabelSize(0.113);
        hSyst->GetYaxis()->SetTitleSize(0.08);
        if (numbOfGenerator == 2) hSyst->GetYaxis()->SetTitleSize(0.08);
        if (numbOfGenerator == 3) hSyst->GetYaxis()->SetTitleSize(0.09);
        hSyst->GetYaxis()->SetTitleOffset(0.8);
        if (numbOfGenerator == 2) hSyst->GetYaxis()->SetTitleOffset(0.63);
        if (numbOfGenerator == 3) hSyst->GetYaxis()->SetTitleOffset(0.60);
        hSyst->GetXaxis()->SetLabelSize(0.12);
        hSyst->GetXaxis()->SetTitleSize(0.14);
        hSyst->GetXaxis()->SetTitleOffset(0.95);
    }
    else {
        hSyst->GetXaxis()->SetTitle();
    }

    if (legend) {
        TLegendEntry *leEntry;
        if(genNum == 3 || genNum==1) leEntry = legend->AddEntry(gen, "Syst. + stat. unc. (gen)", "f");
        //if(genNum == 3) leEntry = legend->AddEntry(gen, "Syst. + Stat. unc. (gen)", "f");
        else leEntry = legend->AddEntry(gen, "Stat. unc. (gen)", "f");
        leEntry->SetFillColor(ZJetsFillColor[genNum-1]);
        leEntry->SetFillStyle(ZJetsFillStyle);
    }


}

void customizeGenHist(TH1D *gen, int genNumb, TLegend *legend, TString legText){

    //--- Customize gen Sherpa ---
    gen->SetFillColor(ZJetsFillColor[genNumb-1]);
    gen->SetFillStyle(ZJetsFillStyle);
    gen->SetLineColor(ZJetsLineColor[genNumb-1]);
    gen->SetLineWidth(2);
    gen->SetMarkerColor(ZJetsMarkerColor[genNumb-1]);
    gen->SetMarkerStyle(ZJetsMarkerStyle[genNumb-1]);
    TLegendEntry *le = legend->AddEntry(gen, legText, "pefl");
    le->SetFillColor(ZJetsFillColor[genNumb-1]);
    le->SetFillStyle(ZJetsFillStyle);
    le->SetLineColor(ZJetsLineColor[genNumb-1]);
    le->SetMarkerColor(ZJetsMarkerColor[genNumb-1]);
    le->SetMarkerStyle(ZJetsMarkerStyle[genNumb-1]);
}


void configYaxis(TH1D *grCentralSyst, TH1D *gen1, TH1D *gen2, TH1D *gen3, bool isRatio){

    //--- Configure y-axis of the plot ---
    double minimumToPlot  = grCentralSyst->GetMaximum();
    double minimumToPlot2 = gen1->GetMaximum();
    
    for (int i = 1; i <= grCentralSyst->GetNbinsX(); i++){
        if (grCentralSyst->GetBinContent(i) <= 0) continue;
        if (grCentralSyst->GetBinContent(i) < minimumToPlot){
            minimumToPlot = grCentralSyst->GetBinContent(i);
        }
    }
    
    for (int i = 1; i <= gen1->GetNbinsX(); i++){
        if (gen1->GetBinContent(i) <= 0) continue;
        if (gen1->GetBinContent(i) < minimumToPlot2){
            minimumToPlot2 = gen1->GetBinContent(i);
        }
    }

    minimumToPlot = TMath::Min(minimumToPlot, minimumToPlot2);
    //if (gen2) minimumToPlot = TMath::Min(minimumToPlot, gen2->GetBinContent(gen2->GetMinimumBin()));
    //if (gen3) minimumToPlot = TMath::Min(minimumToPlot, gen3->GetBinContent(gen3->GetMinimumBin()));

    double maximumToPlot = grCentralSyst->GetMaximum();
    maximumToPlot = TMath::Max(maximumToPlot, gen1->GetBinContent(gen1->GetMaximumBin()));
    //if (gen2) maximumToPlot = TMath::Max(maximumToPlot, gen2->GetBinContent(gen2->GetMaximumBin()));
    //if (gen3) maximumToPlot = TMath::Max(maximumToPlot, gen3->GetBinContent(gen3->GetMaximumBin()));

    if (isRatio) grCentralSyst->GetYaxis()->SetRangeUser(0.5*minimumToPlot, 1.3*maximumToPlot);
    else grCentralSyst->GetYaxis()->SetRangeUser(0.2*minimumToPlot, 5.*maximumToPlot);

}

void configXaxis(TH1D *grCentralSyst, TH1D *gen1, TString variable){
    
    if (variable.Index("ZNGoodJets_Zexc") >= 0){
	    std::cout << __FILE__ << ":" << __LINE__ << ". Range of ZNGoodJets_Zexc x-axis is being modified.!\n";
        grCentralSyst->GetXaxis()->SetBinLabel(2, "= 1");
        grCentralSyst->GetXaxis()->SetBinLabel(3, "= 2");
        grCentralSyst->GetXaxis()->SetBinLabel(4, "= 3");
        grCentralSyst->GetXaxis()->SetBinLabel(5, "= 4");
        grCentralSyst->GetXaxis()->SetBinLabel(6, "= 5");
	    grCentralSyst->GetXaxis()->SetBinLabel(7, "= 6");
        grCentralSyst->GetXaxis()->SetBinLabel(8, "= 7");
	    grCentralSyst->GetXaxis()->SetBinLabel(9, "= 8");
        grCentralSyst->GetXaxis()->SetLabelSize(0.15);
        grCentralSyst->GetXaxis()->SetLabelOffset(0.01);
    }
    if (variable.Index("ZNGoodJets_Zinc") >= 0){
	    std::cout << __FILE__ << ":" << __LINE__ << ". Range of ZNGoodJets_Zinc x-axis is being modified.!\n";
        grCentralSyst->GetXaxis()->SetBinLabel(2, "#geq 1");
        grCentralSyst->GetXaxis()->SetBinLabel(3, "#geq 2");
        grCentralSyst->GetXaxis()->SetBinLabel(4, "#geq 3");
	    grCentralSyst->GetXaxis()->SetBinLabel(5, "#geq 4");
        grCentralSyst->GetXaxis()->SetBinLabel(6, "#geq 5");
	    grCentralSyst->GetXaxis()->SetBinLabel(7, "#geq 6");
        grCentralSyst->GetXaxis()->SetBinLabel(8, "#geq 7");
        grCentralSyst->GetXaxis()->SetBinLabel(9, "#geq 8");
        grCentralSyst->GetXaxis()->SetLabelSize(0.15);
        grCentralSyst->GetXaxis()->SetLabelOffset(0.01);
    }
    
    TString xtitle = gen1->GetXaxis()->GetTitle();
    if ( (variable.Index("diJetAK8Mass") >= 0) || (variable.Index("diJetMass") >= 0) ) xtitle += " [GeV]";
    if (variable.Index("LepPtPlusLeadingJetAK8Pt_Z") >= 0) xtitle = "p_{T}(#mu)+p_{T}(j_{1}) [GeV]";
    
    grCentralSyst->GetXaxis()->SetTitle(xtitle);
    grCentralSyst->GetXaxis()->SetTitleSize(0.14);

}

std::string getYaxisTitle(bool doNormalized, const TH1D *gen1, bool isRatio){

    std::string title = "";
    std::string xtitle = gen1->GetXaxis()->GetTitle();
    std::string shortVar = xtitle.substr(0, xtitle.find(" "));
    std::string unit = "";

    std::cout << "\ngetYaxisTitle function: xtitle = " << xtitle << std::endl;
    std::cout << "getYaxisTitle function: shortVar = " << shortVar << std::endl;
    
    if (xtitle.find("#Delta#phi(j") != std::string::npos)           shortVar = "#Delta#phi(j_{1 }j_{2})";
    if (xtitle.find("#Deltay(j") != std::string::npos)              shortVar = "#Deltay(j_{1 }j_{2})";
    if (xtitle.find("#DeltaR(j") != std::string::npos)              shortVar = "#DeltaR(j_{1 }j_{2})";
    if (xtitle.find("M(j") != std::string::npos)                    shortVar = "M(j_{1 }j_{2})";
    if (xtitle.find("#DeltaR(#mu") != std::string::npos)            shortVar = "#DeltaR(#mu,closest jet)";
    if (xtitle.find("p_{T}(#mu)+p_{T}(j_{l") != std::string::npos)  shortVar = "p_{T}(#mu)+p_{T}(j_{1})";

    title = "d#sigma/d" + shortVar;
    if (xtitle.find("[") != std::string::npos){
        size_t begin = xtitle.find("[") + 1;
        unit = xtitle.substr(begin);
        unit = unit.substr(0, unit.find("]"));
    }
    else{
        if (xtitle.find("M(j") != std::string::npos) unit = "GeV";
    }
    if (doNormalized) title = "1/#sigma " + title;
    else{
        title += "  [pb";
        if (unit != "" ) title += "/" + unit;
        title += "]";
    }
    if (isRatio) title = "Ratio ";

    std::cout << "getYaxisTitle function: title = " << title << "\n" << std::endl;

    return title;
}

TCanvas* makeCrossSectionPlot(TString lepSel, int year, TString variable, bool isRatio, bool doNormalized, TH1D *hStat, TH2D *hCovSyst, TH1D *hGen1, TH1D *hGen2, bool isClosureTest, TH1D* hGen3, double integratedLumi){

    // --- Determine how many comparisons we have ---
    int numbOfGenerator = 1;
    if (hGen2) numbOfGenerator = 2;
    if (hGen3) numbOfGenerator = 3;

    // --- ...
    // NOTE: update
    TH1D *hPDFUp = (TH1D*) hStat->Clone("hPDFUp");
    hPDFUp->Scale(1.10);
    TH1D *hPDFDown = (TH1D*) hStat->Clone("hPDFDown");
    hPDFDown->Scale(0.75);

    TH1D *hSyst = (TH1D*) hStat->Clone("hSyst");
    int nBins = hSyst->GetNbinsX();
    for (int i = 1; i <= nBins; ++i){
        hSyst->SetBinError(i, sqrt(pow(hStat->GetBinError(i), 2) + hCovSyst->GetBinContent(i, i)));
    }

    // --- Declare all additional TGraphs ---
    TGraphAsymmErrors *grCentralStat = createGrFromHist(hStat);
    grCentralStat->SetName("gr" + variable + "CentralStatError"); 

    TGraphAsymmErrors *grCentralSyst = createGrFromHist(hSyst);
    grCentralSyst->SetName("gr" + variable + "CentralTotError"); 

    TGraphAsymmErrors *grCentralStatRatio = createRatioGraph(grCentralStat);
    TGraphAsymmErrors *grCentralSystRatio = createRatioGraph(grCentralSyst);
    TGraphAsymmErrors *grGen1ToCentral = createGenToCentral(hGen1, grCentralStat);
    TGraphAsymmErrors *grGen1PDFSyst = createPDFSystGraph(hPDFUp, hPDFDown, grGen1ToCentral); 
   
    //TGraphAsymmErrors *grGen1TotSyst = createTotSystGraphAMCNLO(lepSel, variable, grGen1ToCentral);
    TGraphAsymmErrors *grGen2ToCentral = NULL;
    TGraphAsymmErrors *grGen2PDFSyst = NULL;
    if (hGen2) {
        grGen2ToCentral = createGenToCentral(hGen2, grCentralStat);
        grGen2PDFSyst = createPDFSystGraph(hPDFUp, hPDFDown, grGen2ToCentral); 
    }
    TGraphAsymmErrors *grGen3ToCentral = NULL;
    TGraphAsymmErrors *grGen3PDFSyst = NULL;
    TGraphAsymmErrors *grGen3ScaleSyst = NULL;
    if (hGen3) {
        grGen3ToCentral = createGenToCentral(hGen3, grCentralStat);
        grGen3PDFSyst = createPDFSystGraph(hPDFUp, hPDFDown, grGen3ToCentral); 
        //grGen3ScaleSyst = createScaleSystGraph(lepSel, variable, grGen3ToCentral);
        grGen3ScaleSyst = createScaleSystGraphNNLO1j(lepSel, variable, grGen3ToCentral);
        grGen3ScaleSyst->SetFillColor(kGreen-8);
    }

    // ------------------------------------------------------------------------------------------

    // --- Main Canvas ---
    double maximum = hGen1->GetMaximum();
    double minimum = hGen1->GetMinimum();
    if (!minimum > 0) minimum = 1e-06;
    TString canvasName = "canvas" + variable;
    TCanvas *plots = new TCanvas(canvasName, hStat->GetTitle(), 600, 800);

    // ------------------------------------------------------------------------------------------
    
    // --- First Pad ---
    plots->cd();
    TPad *plot1 = new TPad("plot1", "plot1", 0., 0., 0., 0.);
    setAndDrawTPad(canvasName, plot1, 1, numbOfGenerator, isRatio);

    // --- TLegend ---
    TLegend *legend = new TLegend(0.7, 0.74, 0.99, 0.98);
    customizeLegend(legend, numbOfGenerator);

    customizeCentral(grCentralSyst, legend, "Data");
    customizeCentral(grCentralStat, false);
    customizeCentral(grCentralSystRatio, true);
    customizeCentral(grCentralStatRatio, true);

    hSyst->SetLineColor(kWhite);
    hSyst->SetMarkerColor(kWhite);
    hSyst->SetTitle("");
    hSyst->GetXaxis()->SetLabelSize(0);
    hSyst->GetYaxis()->SetTitle("");
    // if (canvasName.Contains("FirstJetPt_Zinc1jet") || canvasName.Contains("JetsHT_Zinc1jet") || canvasName.Contains("FirstJetAbsRapidity_Zinc1jet") || canvasName.Contains("dPhiLepJet1_Zinc1jet") || canvasName.Contains("dRptmin100LepCloseJetCo300dR04_Zinc1jet")){
    //     hSyst->GetYaxis()->SetLabelSize(0.065);
    // }
    // else hSyst->GetYaxis()->SetLabelSize(0.055);
    hSyst->GetYaxis()->SetLabelSize(0.055);

    configXaxis(hSyst, hGen1, variable);
    configYaxis(hSyst, hGen1, hGen2, hGen3, isRatio);

    // --- Here begins the axis ranges ---
    // --- Set a default x-axis range first
    hSyst->GetXaxis()->SetRangeUser(hSyst->GetXaxis()->GetXmin(), hSyst->GetXaxis()->GetXmax());
    // --- Now can tweak specifically for each distribution
    // Setting y-max bounds for some of these b/c we keep overflow bins (for migrating events) in the unfolding
    if (canvasName.Contains("ZNGoodJets")){
        hSyst->GetXaxis()->SetRangeUser(0.5, 8.5);
        hSyst->GetYaxis()->SetRangeUser(0.011, 5.0*maximum);
    }
    if ( canvasName.Contains("FirstJetPt_Zinc1jet") )        hSyst->GetXaxis()->SetRangeUser(30., 1099.9);
    if ( canvasName.Contains("SecondJetPt_Zinc2jet") )       hSyst->GetXaxis()->SetRangeUser(30., 699.9);
    if ( canvasName.Contains("FirstJetAK8Pt_Zinc1jet") )     hSyst->GetXaxis()->SetRangeUser(50., 1099.9);
    if ( canvasName.Contains("SecondJetAK8Pt_Zinc2jet") )    hSyst->GetXaxis()->SetRangeUser(50., 699.9);
    if ( canvasName.Contains("dRJets_Zinc2jet") )            hSyst->GetXaxis()->SetRangeUser(0.4, 5.6);
    if ( canvasName.Contains("dRJetsAK8_Zinc2jet") )         hSyst->GetXaxis()->SetRangeUser(0.8, 5.6);
    if ( canvasName.Contains("diJetMass_Zinc2jet") )         hSyst->GetXaxis()->SetRangeUser(0., 2999.9);
    if ( canvasName.Contains("diJetAK8Mass_Zinc2jet") )      hSyst->GetXaxis()->SetRangeUser(0., 2999.9);
    if ( canvasName.Contains("dRLepCloseJetCo") )            hSyst->GetXaxis()->SetRangeUser(0.4, 3.999);
    if ( canvasName.Contains("LepPtPlusLeadingJetAK8Pt_Z") ) hSyst->GetXaxis()->SetRangeUser(200., 1399.9);
    if ( canvasName.Contains("LepPtPlusHT2over2AK8_Z") )     hSyst->GetXaxis()->SetRangeUser(200., 1050.);
    // ---
    if ( canvasName.Contains("JetAbsRapidity_Zinc1jet") || canvasName.Contains("JetAK8AbsRapidity_Zinc1jet") ){
        hSyst->GetYaxis()->SetRangeUser(0.1, 1.5*maximum);
    }
    if ( canvasName.Contains("JetAbsRapidity_Zinc2jet") || canvasName.Contains("JetAK8AbsRapidity_Zinc2jet") ){
        hSyst->GetYaxis()->SetRangeUser(0.1, 1.5*maximum);
    }
    if (canvasName.Contains("dPhiLepJet1"))     hSyst->GetYaxis()->SetRangeUser(0.09*minimum, 4.0*maximum);
    if (canvasName.Contains("dPhiLepJet2"))     hSyst->GetYaxis()->SetRangeUser(0.1*minimum, 1.3*maximum);
    if (canvasName.Contains("dPhiJets"))        hSyst->GetYaxis()->SetRangeUser(0.08*minimum, 5.*maximum);
    if (canvasName.Contains("dRJets"))          hSyst->GetYaxis()->SetRangeUser(1.0*minimum, 20.*maximum);
    if (canvasName.Contains("dRLepCloseJetCo")) hSyst->GetYaxis()->SetRangeUser(0.5*minimum, 11.*maximum);

    // --- Now draw histograms, graphs ---
    hSyst->DrawCopy("e");
    grCentralSyst->SetName("grCentralSyst");
    grCentralSyst->Draw("2");
    customizeGenHist(hGen1, 1, legend, hGen1->GetZaxis()->GetTitle());
    hGen1->SetName("hGen1");
    hGen1->DrawCopy("ESAME");
    if (hGen2){
        customizeGenHist(hGen2, 2, legend, hGen2->GetZaxis()->GetTitle());
        hGen2->SetName("hGen2");
        hGen2->DrawCopy("ESAME");
    }
    if (hGen3){
        customizeGenHist(hGen3, 3, legend, hGen3->GetZaxis()->GetTitle());
        hGen3->SetName("hGen3");
        hGen3->DrawCopy("ESAME");
    }
    grCentralStat->SetName("grCentralStat");
    grCentralStat->Draw("p");
    legend->Draw("same");

    // --- TLatex stuff ---
    TLatex *latexLabel = new TLatex(); 
    latexLabel->SetNDC();
    latexLabel->SetTextSize(0.04);
    if (hGen2) latexLabel->SetTextSize(0.04);
    if (hGen3) latexLabel->SetTextSize(0.04);
    latexLabel->SetLineWidth(2);
    latexLabel->SetTextFont(61);

    // --- "CMS" logo ---
    if (isRatio) latexLabel->DrawLatex(0.57, 0.25+0.06, "CMS Preliminary");
    else{
        if ( canvasName.Contains("LepPtPlusLeadingJetAK8Pt_Z") || canvasName.Contains("LepPtPlusHT2over2AK8_Z") || canvasName.Contains("dRLepCloseJetCo") ){
            latexLabel->DrawLatex(0.18, 0.25+0.07, "CMS Preliminary");
        }
        else latexLabel->DrawLatex(0.18, 0.24, "CMS Preliminary");
    }

    latexLabel->SetTextSize(0.044);
    latexLabel->SetLineWidth(2);
    latexLabel->SetTextFont(42);

    // --- integrated luminosity of data sample ---
    if (year == 2016)      latexLabel->DrawLatex(0.13, 0.90, "35.9 fb^{-1} (13 TeV)");
    else if (year == 2017) latexLabel->DrawLatex(0.13, 0.90, "41.5 fb^{-1} (13 TeV)");
    else if (year == 2018) latexLabel->DrawLatex(0.13, 0.90, "59.7 fb^{-1} (13 TeV)");
    else                   latexLabel->DrawLatex(0.13, 0.90, "137.16 fb^{-1} (13 TeV)");

    // --- ...
    if (isRatio){
        if (canvasName.Contains("AK8")){ 
            if ( canvasName.Contains("LepPtPlusLeadingJetAK8Pt_Z") || canvasName.Contains("LepPtPlusHT2over2AK8_Z") ){
                latexLabel->DrawLatex(0.57, 0.21+0.04, "anti-k_{T} (R = 0.8) Jets");
            }
            else latexLabel->DrawLatex(0.57, 0.21-0.03, "anti-k_{T} (R = 0.8) Jets");
        }
        else latexLabel->DrawLatex(0.57, 0.21-0.03, "anti-k_{T} (R = 0.4) Jets");
    }
    else{
        if (canvasName.Contains("AK8")){
            if ( canvasName.Contains("LepPtPlusLeadingJetAK8Pt_Z") || canvasName.Contains("LepPtPlusHT2over2AK8_Z") ){
                latexLabel->DrawLatex(0.18, 0.21+0.04, "anti-k_{T} (R = 0.8) Jets");
            }
            else latexLabel->DrawLatex(0.18, 0.21-0.03, "anti-k_{T} (R = 0.8) Jets");
        }
        else{
            if ( canvasName.Contains("dRLepCloseJetCo") ) latexLabel->DrawLatex(0.18, 0.21+0.05, "anti-k_{T} (R = 0.4) Jets");
            else latexLabel->DrawLatex(0.18, 0.21-0.03, "anti-k_{T} (R = 0.4) Jets");
        }
    }

    // --- ...
    if (isRatio){
        
        if (canvasName.Contains("AK8")){
            if ( canvasName.Contains("LepPtPlusLeadingJetAK8Pt_Z") || canvasName.Contains("LepPtPlusHT2over2AK8_Z") ){
                latexLabel->DrawLatex(0.57, 0.21-0.02,"p_{T}^{jet} > 50 GeV, |y^{jet}| < 2.4");
                latexLabel->DrawLatex(0.57, 0.21-0.09,"p_{T}^{#mu} > 50 GeV, p_{T}^{j_{1}} > 100 GeV");
            }
            else{
                latexLabel->DrawLatex(0.57, 0.21-0.09,"p_{T}^{jet} > 50 GeV, |y^{jet}| < 2.4");
            }
        }
        else latexLabel->DrawLatex(0.57, 0.21-0.09,"p_{T}^{jet} > 30 GeV, |y^{jet}| < 2.4");


        // --- jet multiplicity requirement ---
        latexLabel->DrawLatex(0.57, 0.21-0.15,"W(#mu#nu) + jets");       

    }
    else{
        if (canvasName.Contains("FirstJetPt50")) latexLabel->DrawLatex(0.18,0.21-0.09,"p_{T}^{jet} > 50 GeV, |#eta^{jet}| < 2.4 ");
        else if (canvasName.Contains("FirstJetPt80")) latexLabel->DrawLatex(0.18,0.21-0.09,"p_{T}^{jet} > 80 GeV, |#eta^{jet}| < 2.4 ");
        else if (canvasName.Contains("ZPt150")) latexLabel->DrawLatex(0.18,0.21-0.09,"p_{T}^{Z} > 150 GeV, p_{T}^{jet} > 30 GeV, |#eta^{jet}| < 2.4 ");
        else if (canvasName.Contains("ZPt300")) latexLabel->DrawLatex(0.18,0.21-0.09,"p_{T}^{Z} > 300 GeV, p_{T}^{jet} > 30 GeV, |#eta^{jet}| < 2.4 ");
        else if (canvasName.Contains("DifJetRapidityl2")) latexLabel->DrawLatex(0.18,0.21-0.09,"p_{T}^{jet} > 30 GeV, |#eta^{jet}| < 2.4, |y_{jet1}-y_{jet2}| > 2 ");
        else if (canvasName.Contains("DifJetRapiditys2")) latexLabel->DrawLatex(0.18,0.21-0.09,"p_{T}^{jet} > 30 GeV, |#eta^{jet}| < 2.4, |y_{jet1}-y_{jet2}| < 2 ");
        else if (canvasName.Contains("ZPt150_HT300")) latexLabel->DrawLatex(0.18,0.21-0.09,"p_{T}^{Z} > 150 GeV, p_{T}^{jet} > 30 GeV, |#eta^{jet}| < 2.4, H_{T}^{jet} > 300 GeV ");
        else if (canvasName.Contains("dRptmin100LepCloseJetCo300dR04_Zinc1jet")) latexLabel->DrawLatex(0.18,0.21-0.09,"p_{T}^{jet} > 100 GeV, p_{T}^{lead jet} > 300 GeV, |y^{jet}| < 2.4 ");
        else if (canvasName.Contains("AK8")){
            if ( canvasName.Contains("LepPtPlusLeadingJetAK8Pt_Z") || canvasName.Contains("LepPtPlusHT2over2AK8_Z") ){
                latexLabel->DrawLatex(0.18, 0.21-0.02,"p_{T}^{jet} > 50 GeV, |y^{jet}| < 2.4 ");
                latexLabel->DrawLatex(0.18, 0.21-0.09,"p_{T}^{#mu} > 50 GeV, p_{T}^{j_{1}} > 100 GeV");
            }
            else latexLabel->DrawLatex(0.18, 0.21-0.09,"p_{T}^{jet} > 50 GeV, |y^{jet}| < 2.4 ");
        }
        else if (canvasName.Contains("dRLepCloseJetCo")){
            latexLabel->DrawLatex(0.18, 0.21-0.01,"p_{T}^{jet} > 30 GeV, |y^{jet}| < 2.4 ");
            if (canvasName.Contains("dRLepCloseJetCo300dR04_Zinc1jet_TUnfold")) latexLabel->DrawLatex(0.18, 0.21-0.09,"p_{T}^{j_{1}} > 300 GeV, p_{T}^{j_{2}} > 150 GeV");
            if (canvasName.Contains("dRLepCloseJetCo500dR04_Zinc1jet_TUnfold")) latexLabel->DrawLatex(0.18, 0.21-0.09,"p_{T}^{j_{1}} > 500 GeV, p_{T}^{j_{2}} > 300 GeV");
        }
        else latexLabel->DrawLatex(0.18, 0.21-0.09,"p_{T}^{jet} > 30 GeV, |y^{jet}| < 2.4 ");


        // --- jet multiplicity requirement ---
        // inclusive jet requirement
        if (canvasName.Contains("_Zinc1jet") && !(canvasName.Contains("dRLepCloseJetCo")) ){
            latexLabel->DrawLatex(0.18,0.21-0.15,"W(#mu#nu) + #geq 1-jet");
        }
        else if (canvasName.Contains("_Zinc2jet") || (canvasName.Contains("dRLepCloseJetCo")) ){
            latexLabel->DrawLatex(0.18,0.21-0.15,"W(#mu#nu) + #geq 2-jets");
        }
        else if (canvasName.Contains("_Zinc3jet")) latexLabel->DrawLatex(0.18,0.21-0.15,"W(#mu#nu) + #geq 3-jets");
        else if (canvasName.Contains("_Zinc4jet")) latexLabel->DrawLatex(0.18,0.21-0.15,"W(#mu#nu) + #geq 4-jets");
        // exclusive jet requirement
        else if (canvasName.Contains("_Zexc1jet")) latexLabel->DrawLatex(0.18,0.21-0.15,"W(#mu#nu) + 1-jet");
        else if (canvasName.Contains("_Zexc2jet")) latexLabel->DrawLatex(0.18,0.21-0.15,"W(#mu#nu) + 2-jets");
        // other
        else latexLabel->DrawLatex(0.18,0.21-0.15,"W(#mu#nu) + jets");

    }

    latexLabel->SetName("latexLabel");
    latexLabel->Draw("same");

    // --- y-axis title
    TLatex *ytitle = new TLatex();
    ytitle->SetName("ytitle");
    ytitle->SetTextSize(0.04);
    if (hGen2) ytitle->SetTextSize(0.05);
    if (hGen3) ytitle->SetTextSize(0.06);
    ytitle->SetTextFont(42);
    ytitle->SetLineWidth(2);
    ytitle->SetTextColor(kBlack);
    ytitle->SetNDC();
    ytitle->SetTextAlign(33);
    ytitle->SetTextAngle(90);
    std::string strYtitle = getYaxisTitle(doNormalized, hGen1, isRatio);
    if (strYtitle.find("eta") != std::string::npos){
        size_t first = strYtitle.find("#eta");
        std::string tmp1 = strYtitle.substr(0, first);
        std::string tmp2 = strYtitle.substr(first);
        strYtitle = tmp1 + "|" + tmp2;
        size_t second = strYtitle.find(")");
        tmp1 = strYtitle.substr(0, second+1);
        tmp2 = strYtitle.substr(second+1);
        strYtitle = tmp1 + "|" + tmp2;
    }
    ytitle->DrawLatex(0.008,0.91,strYtitle.c_str());
    // --- End Of first Pad ---

    // ------------------------------------------------------------------------------------------

    // --- Second Pad ---
    plots->cd();
    TPad *plot2 = new TPad("plot2", "plot2", 0., 0., 0., 0.);
    setAndDrawTPad(canvasName, plot2, 2, numbOfGenerator, isRatio);

    // --- TLegend ---
    TLegend *legend2 = new TLegend(0.16, 0.05, 0.42, 0.20);
    customizeLegend(legend2, 1, numbOfGenerator);
    TString generator1 = hGen1->GetZaxis()->GetTitle();
    generator1 = generator1(0, generator1.Index(" "));

    customizeGenGraph(hSyst, grGen1ToCentral, grGen1PDFSyst, 1, "MG_aMC FxFx/Data", numbOfGenerator, legend2, isClosureTest);
    
    configXaxis(hSyst, hGen1, variable);
    grGen1PDFSyst->SetFillStyle(1001);
    grGen1PDFSyst->SetFillColor(kBlue-6);
       
    //grGen1TotSyst->SetFillStyle(1001);
    //grGen1TotSyst->SetFillColor(kBlue-10);
    hSyst->DrawCopy("e");
    grGen1ToCentral->SetName("grGen1ToCentral");
    grGen1ToCentral->Draw("2");
    //grGen1PDFSyst->Draw("2");
       
    //grGen1TotSyst->Draw("2");
    grCentralSystRatio->SetName("grCentralSystRatio");
    grCentralSystRatio->Draw("2");
    grCentralStatRatio->Draw("p");
    grGen1ToCentral->Draw("p");
    legend2->Draw("same");
    //if (canvasName.Contains("JetPt_Zinc")) {
    //    grGen1ToCentral->GetXaxis()->SetRangeUser(30, x + ex);
    //}
    plot2->RedrawAxis();
    // --- End of Second Pad ---


    // ------------------------------------------------------------------------------------------

    if (hGen2){

        // --- Third Pad ---
        plots->cd();
        TPad *plot3 = new TPad("plot3", "plot3", 0., 0., 0., 0.);
        setAndDrawTPad(canvasName, plot3, 3, numbOfGenerator, isRatio);

        // --- TLegend ---
        TLegend *legend3 = new TLegend(0.16, 0.05, 0.42, 0.20);
        customizeLegend(legend3, 2, numbOfGenerator);
        TString generator2 = hGen2->GetZaxis()->GetTitle();
        generator2 = generator2(0, generator2.Index(" "));

        customizeGenGraph(hSyst, grGen2ToCentral, grGen2PDFSyst, 2, "MG_aMC/Data", numbOfGenerator, legend3, isClosureTest);

        configXaxis(hSyst, hGen2, variable);
        grGen2PDFSyst->SetFillStyle(ZJetsFillStyle);
        grGen2PDFSyst->SetFillColor(ZJetsPdfFillColor[2]);
        hSyst->DrawCopy("e");
        grGen2ToCentral->SetName("grGen2ToCentral");
        grGen2ToCentral->Draw("2");
        //grGen2PDFSyst->Draw("2");
        grGen2ToCentral->Draw("2");
        grCentralSystRatio->SetName("grCentralSystRatio");
        grCentralSystRatio->Draw("2");
        grCentralStatRatio->Draw("p");
        grGen2ToCentral->Draw("p");
        legend3->Draw("same");
        //if (canvasName.Contains("JetPt_Zinc")) {
        //    grGen2ToCentral->GetXaxis()->SetRangeUser(30, x + ex);
        //}
        plot3->RedrawAxis();
        // --- End of Third Pad ---
    }

    if (hGen3){

        // --- Fourth Pad ---
        plots->cd();
        TPad *plot4 = new TPad("plot4", "plot4", 0., 0., 0., 0.);
        setAndDrawTPad(canvasName, plot4, 4, numbOfGenerator, isRatio);

        // --- TLegend ---
        TLegend *legend4 = new TLegend(0.16, 0.05, 0.42, 0.20);
        customizeLegend(legend4, 3, numbOfGenerator);
        TString generator3 = hGen3->GetZaxis()->GetTitle();
        generator3 = generator3(0, generator3.Index(" "));

        customizeGenGraph(hSyst, grGen3ToCentral, grGen3PDFSyst, 3,"NNLO/Data", numbOfGenerator, legend4, isClosureTest);        

        configXaxis(hSyst, hGen3, variable);
        grGen3PDFSyst->SetFillStyle(ZJetsFillStyle);
        grGen3PDFSyst->SetFillColor(ZJetsPdfFillColor[1]);
        hSyst->DrawCopy("e");
        grGen3ToCentral->SetName("grGen3ToCentral");
        grGen3ToCentral->Draw("2");
        //grGen3PDFSyst->Draw("2");
        grGen3ScaleSyst->Draw("2");
        grGen3ToCentral->Draw("2");
        grCentralSystRatio->SetName("grCentralSystRatio");
        grCentralSystRatio->Draw("2");
        grCentralStatRatio->Draw("p");
        grGen3ToCentral->Draw("p");
        legend4->Draw("same");
        //if (canvasName.Contains("JetPt_Zinc")) {
        //    grGen3ToCentral->GetXaxis()->SetRangeUser(30, x + ex);
        //}
        plot4->RedrawAxis();
        // --- End of Fourth Pad ---
    }

    plots->Update();
    return plots;
}

void createTitleVariableAnddSigma(TString variable, bool doNormalized, TString xtitle, TString& title, TString& var, TString& dSigma) {

    // jet multiplicity
    if (variable.Index("ZNGoodJets_Zexc") >= 0) {
        title = "Exclusive jet multiplicity";
        var = "$N_{\\text{jets}}$";
        dSigma = "$\\frac{d\\sigma}{dN_{\\text{jets}}} \\tiny{\\left[\\text{pb}\\right]}$";
    }
    if (variable.Index("ZNGoodJets_Zinc") >= 0) {
        title = "Inclusive jet multiplicity";
        var = "$N_{\\text{jets}}$";
        dSigma = "$\\frac{d\\sigma}{dN_{\\text{jets}}} \\tiny{\\left[\\text{pb}\\right]}$";
    }

    if (xtitle.Index("p_{T}(Z)") >= 0) {
        title = "$p_{T}^{Z}$";
        var = "$p_{\\text{T}}(Z)$ \\tiny{[GeV]}";
        dSigma = "$\\frac{d\\sigma}{dp_{\\text{T}}(Z)} \\tiny{\\left[\\frac{\\text{pb}}{\\text{GeV}}\\right]}$";
    }

    // Z Boson and Jet rapidity
    if (variable.Index("AbsZRapidity_Zexc1jet") >= 0) {
        title = "$|y_\\text{Z}|$ ($N_{\\text{jets}} = 1$)";
        var = "$|y_{\\text{Z}}|$";
        dSigma = "$\\frac{d\\sigma}{d|y_{\\text{Z}}|} \\tiny{\\left[\\text{pb}\\right]}$";
    }

    if (variable.Index("AbsZRapidity_Zinc1jet") >= 0) {
        title = "$|y_\\text{Z}|$ ($N_{\\text{jets}} \\geq 1$)";
        var = "$|y_{\\text{Z}}|$";
        dSigma = "$\\frac{d\\sigma}{d|y_{\\text{Z}}|} \\tiny{\\left[\\text{pb}\\right]}$";
    }

    if (variable.Index("AbsJetRapidity_Zexc1jet") >= 0) {
        title = "$|y_\\text{jet}|$ ($N_{\\text{jets}} = 1$)";
        var = "$|y_{\\text{jet}}|$";
        dSigma = "$\\frac{d\\sigma}{d|y_{\\text{jet}}|} \\tiny{\\left[\\text{pb}\\right]}$";
    }

    if (variable.Index("AbsFirstJetRapidity_Zinc1jet") >= 0) {
        title = "$|y_\\text{jet1}|$ ($N_{\\text{jets}} \\geq 1$)";
        var = "$|y_{\\text{jet1}}|$";
        dSigma = "$\\frac{d\\sigma}{d|y_{\\text{jet1}}|} \\tiny{\\left[\\text{pb}\\right]}$";
    }

    if (variable.Index("SumZJetRapidity_Zexc1jet") >= 0) {
        title = "$y_\\text{sum}$ ($N_{\\text{jets}} = 1$)";
        var = "$y_{\\text{sum}}$";
        dSigma = "$\\frac{d\\sigma}{dy_{\\text{sum}}} \\tiny{\\left[\\text{pb}\\right]}$";
    }

    if (variable.Index("SumZFirstJetRapidity_Zinc1jet") >= 0) {
        title = "$y_\\text{sum(Z,jet1)}$ ($N_{\\text{jets}} \\geq 1$)";
        var = "$y_{\\text{sum(Z,jet1)}}$";
        dSigma = "$\\frac{d\\sigma}{dy_{\\text{sum(Z,jet1)}}} \\tiny{\\left[\\text{pb}\\right]}$";
    }

    if (variable.Index("DifZJetRapidity_Zexc1jet") >= 0) {
        title = "$y_\\text{diff}$ ($N_{\\text{jets}} = 1$)";
        var = "$y_{\\text{diff}}$";
        dSigma = "$\\frac{d\\sigma}{dy_{\\text{diff}}} \\tiny{\\left[\\text{pb}\\right]}$";
    }

    if (variable.Index("DifZFirstJetRapidity_Zinc1jet") >= 0) {
        title = "$y_\\text{diff(Z,jet1)}$ ($N_{\\text{jets}} \\geq 1$)";
        var = "$y_{\\text{diff(Z,jet1)}}$";
        dSigma = "$\\frac{d\\sigma}{dy_{\\text{diff(Z,jet1)}}} \\tiny{\\left[\\text{pb}\\right]}$";
    }

    if (variable.Index("AbsZRapidity_Zinc2jet") >= 0) {
        title = "$|y_\\text{Z}|$ ($N_{\\text{jets}} \\geq 2$)";
        var = "$|y_{\\text{Z}}|$";
        dSigma = "$\\frac{d\\sigma}{d|y_{\\text{Z}}|} \\tiny{\\left[\\text{pb}\\right]}$";
    }

    if (variable.Index("AbsFirstJetRapidity_Zinc2jet") >= 0) {
        title = "$|y_\\text{jet1}|$ ($N_{\\text{jets}} \\geq 2$)";
        var = "$|y_{\\text{jet1}}|$";
        dSigma = "$\\frac{d\\sigma}{d|y_{\\text{jet1}}|} \\tiny{\\left[\\text{pb}\\right]}$";
    }

    if (variable.Index("AbsSecondJetRapidity_Zinc2jet") >= 0) {
        title = "$|y_\\text{jet2}|$ ($N_{\\text{jets}} \\geq 2$)";
        var = "$|y_{\\text{jet2}}|$";
        dSigma = "$\\frac{d\\sigma}{d|y_{\\text{jet2}}|} \\tiny{\\left[\\text{pb}\\right]}$";
    }

    if (variable.Index("SumZFirstJetRapidity_Zinc2jet") >= 0) {
        title = "$y_\\text{sum(Z,jet1)}$ ($N_{\\text{jets}} \\geq 2$)";
        var = "$y_{\\text{sum(Z,jet1)}}$";
        dSigma = "$\\frac{d\\sigma}{dy_{\\text{sum(Z,jet1)}}} \\tiny{\\left[\\text{pb}\\right]}$";
    }

    if (variable.Index("SumZSecondJetRapidity_Zinc2jet") >= 0) {
        title = "$y_\\text{sum(Z,jet2)}$ ($N_{\\text{jets}} \\geq 2$)";
        var = "$y_{\\text{sum(Z,jet2)}}$";
        dSigma = "$\\frac{d\\sigma}{dy_{\\text{sum(Z,jet2)}}} \\tiny{\\left[\\text{pb}\\right]}$";
    }

    if (variable.Index("SumFirstSecondJetRapidity_Zinc2jet") >= 0) {
        title = "$y_\\text{sum(jet1,jet2)}$ ($N_{\\text{jets}} \\geq 2$)";
        var = "$y_{\\text{sum(jet1,jet2)}}$";
        dSigma = "$\\frac{d\\sigma}{dy_{\\text{sum(jet1,jet2)}}} \\tiny{\\left[\\text{pb}\\right]}$";
    }

    if (variable.Index("DifZFirstJetRapidity_Zinc2jet") >= 0) {
        title = "$y_\\text{diff(Z,jet1)}$ ($N_{\\text{jets}} \\geq 2$)";
        var = "$y_{\\text{diff(Z,jet1)}}$";
        dSigma = "$\\frac{d\\sigma}{dy_{\\text{diff(Z,jet1)}}} \\tiny{\\left[\\text{pb}\\right]}$";
    }

    if (variable.Index("DifZSecondJetRapidity_Zinc2jet") >= 0) {
        title = "$y_\\text{diff(Z,jet2)}$ ($N_{\\text{jets}} \\geq 2$)";
        var = "$y_{\\text{diff(Z,jet2)}}$";
        dSigma = "$\\frac{d\\sigma}{dy_{\\text{diff(Z,jet2)}}} \\tiny{\\left[\\text{pb}\\right]}$";
    }

    if (variable.Index("DifFirstSecondJetRapidity_Zinc2jet") >= 0) {
        title = "$y_\\text{diff(jet1,jet2)}$ ($N_{\\text{jets}} \\geq 2$)";
        var = "$y_{\\text{diff(jet1,jet2)}}$";
        dSigma = "$\\frac{d\\sigma}{dy_{\\text{diff(jet1,jet2)}}} \\tiny{\\left[\\text{pb}\\right]}$";
    }

    if (variable.Index("SumZTwoJetsRapidity_Zinc2jet") >= 0) {
        title = "$y_\\text{sum(Z,jet1+jet2)}$ ($N_{\\text{jets}} \\geq 2$)";
        var = "$y_{\\text{sum(Z,jet1+jet2)}}$";
        dSigma = "$\\frac{d\\sigma}{dy_{\\text{sum(Z,jet1+jet2)}}} \\tiny{\\left[\\text{pb}\\right]}$";
    }

    if (variable.Index("DifZTwoJetsRapidity_Zinc2jet") >= 0) {
        title = "$y_\\text{diff(Z,jet1+jet2)}$ ($N_{\\text{jets}} \\geq 2$)";
        var = "$y_{\\text{diff(Z,jet1+jet2)}}$";
        dSigma = "$\\frac{d\\sigma}{dy_{\\text{diff(Z,jet1+jet2)}}} \\tiny{\\left[\\text{pb}\\right]}$";
    }

    if (variable.Index("AbsZRapidity_ZPt150_Zinc1jet") >= 0) {
        title = "$|y_\\text{Z}|$ ($N_{\\text{jets}} \\geq 1, p_{\\text{T}}^{\\text{Z}} \\geq 150 \\text{GeV}$)";
        var = "$|y_{\\text{Z}}|$";
        dSigma = "$\\frac{d\\sigma}{d|y_{\\text{Z}}|} \\tiny{\\left[\\text{pb}\\right]}$";
    }

    if (variable.Index("AbsZRapidity_ZPt300_Zinc1jet") >= 0) {
        title = "$|y_\\text{Z}|$ ($N_{\\text{jets}} \\geq 1, p_{\\text{T}}^{\\text{Z}} \\geq 300 \\text{GeV}$)";
        var = "$|y_{\\text{Z}}|$";
        dSigma = "$\\frac{d\\sigma}{d|y_{\\text{Z}}|} \\tiny{\\left[\\text{pb}\\right]}$";
    }

    if (variable.Index("AbsFirstJetRapidity_ZPt150_Zinc1jet") >= 0) {
        title = "$|y_\\text{jet1}|$ ($N_{\\text{jets}} \\geq 1, p_{\\text{T}}^{\\text{Z}} \\geq 150 \\text{GeV}$)";
        var = "$|y_{\\text{jet1}}|$";
        dSigma = "$\\frac{d\\sigma}{d|y_{\\text{jet1}}|} \\tiny{\\left[\\text{pb}\\right]}$";
    }

    if (variable.Index("AbsFirstJetRapidity_ZPt300_Zinc1jet") >= 0) {
        title = "$|y_\\text{jet1}|$ ($N_{\\text{jets}} \\geq 1, p_{\\text{T}}^{\\text{Z}} \\geq 300 \\text{GeV}$)";
        var = "$|y_{\\text{jet1}}|$";
        dSigma = "$\\frac{d\\sigma}{d|y_{\\text{jet1}}|} \\tiny{\\left[\\text{pb}\\right]}$";
    }

    if (variable.Index("SumZFirstJetRapidity_ZPt150_Zinc1jet") >= 0) {
        title = "$y_\\text{sum(Z,jet1)}$ ($N_{\\text{jets}} \\geq 1, p_{\\text{T}}^{\\text{Z}} \\geq 150 \\text{GeV}$)";
        var = "$y_{\\text{sum(Z,jet1)}}$";
        dSigma = "$\\frac{d\\sigma}{dy_{\\text{sum(Z,jet1)}}} \\tiny{\\left[\\text{pb}\\right]}$";
    }

    if (variable.Index("SumZFirstJetRapidity_ZPt300_Zinc1jet") >= 0) {
        title = "$y_\\text{sum(Z,jet1)}$ ($N_{\\text{jets}} \\geq 1, p_{\\text{T}}^{\\text{Z}} \\geq 300 \\text{GeV}$)";
        var = "$y_{\\text{sum(Z,jet1)}}$";
        dSigma = "$\\frac{d\\sigma}{dy_{\\text{sum(Z,jet1)}}} \\tiny{\\left[\\text{pb}\\right]}$";
    }

    if (variable.Index("DifZFirstJetRapidity_ZPt150_Zinc1jet") >= 0) {
        title = "$y_\\text{diff(Z,jet1)}$ ($N_{\\text{jets}} \\geq 1, p_{\\text{T}}^{\\text{Z}} \\geq 150 \\text{GeV}$)";
        var = "$y_{\\text{diff(Z,jet1)}}$";
        dSigma = "$\\frac{d\\sigma}{dy_{\\text{diff(Z,jet1)}}} \\tiny{\\left[\\text{pb}\\right]}$";
    }

    if (variable.Index("DifZFirstJetRapidity_ZPt300_Zinc1jet") >= 0) {
        title = "$y_\\text{diff(Z,jet1)}$ ($N_{\\text{jets}} \\geq 1, p_{\\text{T}}^{\\text{Z}} \\geq 300 \\text{GeV}$)";
        var = "$y_{\\text{diff(Z,jet1)}}$";
        dSigma = "$\\frac{d\\sigma}{dy_{\\text{diff(Z,jet1)}}} \\tiny{\\left[\\text{pb}\\right]}$";
    }

    if (variable.Index("SumZFirstJetRapidity_FirstJetPt50_Zinc1jet") >= 0) {
        title = "$y_\\text{sum(Z,jet1)}$ ($N_{\\text{jets}} \\geq 1, p_{\\text{T}}^{\\text{jet1}} \\geq 50 \\text{GeV}$)";
        var = "$y_{\\text{sum(Z,jet1)}}$";
        dSigma = "$\\frac{d\\sigma}{dy_{\\text{sum(Z,jet1)}}} \\tiny{\\left[\\text{pb}\\right]}$";
    }

    if (variable.Index("SumZFirstJetRapidity_FirstJetPt80_Zinc1jet") >= 0) {
        title = "$y_\\text{sum(Z,jet1)}$ ($N_{\\text{jets}} \\geq 1, p_{\\text{T}}^{\\text{jet1}} \\geq 80 \\text{GeV}$)";
        var = "$y_{\\text{sum(Z,jet1)}}$";
        dSigma = "$\\frac{d\\sigma}{dy_{\\text{sum(Z,jet1)}}} \\tiny{\\left[\\text{pb}\\right]}$";
    }

    if (variable.Index("DifZFirstJetRapidity_FirstJetPt50_Zinc1jet") >= 0) {
        title = "$y_\\text{diff(Z,jet1)}$ ($N_{\\text{jets}} \\geq 1, p_{\\text{T}}^{\\text{jet1}} \\geq 50 \\text{GeV}$)";
        var = "$y_{\\text{diff(Z,jet1)}}$";
        dSigma = "$\\frac{d\\sigma}{dy_{\\text{diff(Z,jet1)}}} \\tiny{\\left[\\text{pb}\\right]}$";
    }

    if (variable.Index("DifZFirstJetRapidity_FirstJetPt80_Zinc1jet") >= 0) {
        title = "$y_\\text{diff(Z,jet1)}$ ($N_{\\text{jets}} \\geq 1, p_{\\text{T}}^{\\text{jet1}} \\geq 80 \\text{GeV}$)";
        var = "$y_{\\text{diff(Z,jet1)}}$";
        dSigma = "$\\frac{d\\sigma}{dy_{\\text{diff(Z,jet1)}}} \\tiny{\\left[\\text{pb}\\right]}$";
    }

    if (variable.Index("AbsZRapidity_DifJetRapiditys2_Zinc2jet") >= 0) {
        title = "$|y_\\text{Z}|$ ($N_{\\text{jets}} \\geq 2, |y_{jet1} - y_{jet2}| \\leq 2 $)";
        var = "$|y_{\\text{Z}}|$";
        dSigma = "$\\frac{d\\sigma}{d|y_{\\text{Z}}|} \\tiny{\\left[\\text{pb}\\right]}$";
    }

    if (variable.Index("AbsZRapidity_DifJetRapidityl2_Zinc2jet") >= 0) {
        title = "$|y_\\text{Z}|$ ($N_{\\text{jets}} \\geq 2, |y_{jet1} - y_{jet2}| \\geq 2 $)";
        var = "$|y_{\\text{Z}}|$";
        dSigma = "$\\frac{d\\sigma}{d|y_{\\text{Z}}|} \\tiny{\\left[\\text{pb}\\right]}$";
    }

    if (variable.Index("AbsFirstJetRapidity_DifJetRapiditys2_Zinc2jet") >= 0) {
        title = "$|y_\\text{jet1}|$ ($N_{\\text{jets}} \\geq 2, |y_{jet1} - y_{jet2}| \\leq 2 $)";
        var = "$|y_{\\text{jet1}}|$";
        dSigma = "$\\frac{d\\sigma}{d|y_{\\text{jet1}}|} \\tiny{\\left[\\text{pb}\\right]}$";
    }

    if (variable.Index("AbsFirstJetRapidity_DifJetRapidityl2_Zinc2jet") >= 0) {
        title = "$|y_\\text{jet1}|$ ($N_{\\text{jets}} \\geq 2, |y_{jet1} - y_{jet2}| \\geq 2 $)";
        var = "$|y_{\\text{jet1}}|$";
        dSigma = "$\\frac{d\\sigma}{d|y_{\\text{jet1}}|} \\tiny{\\left[\\text{pb}\\right]}$";
    }

    if (variable.Index("SumZFirstJetRapidity_DifJetRapiditys2_Zinc2jet") >= 0) {
        title = "$y_\\text{sum(Z,jet1)}$ ($N_{\\text{jets}} \\geq 2, |y_{jet1} - y_{jet2}| \\leq 2 $)";
        var = "$y_{\\text{sum(Z,jet1)}}$";
        dSigma = "$\\frac{d\\sigma}{dy_{\\text{sum(Z,jet1)}}} \\tiny{\\left[\\text{pb}\\right]}$";
    }

    if (variable.Index("SumZFirstJetRapidity_DifJetRapidityl2_Zinc2jet") >= 0) {
        title = "$y_\\text{sum(Z,jet1)}$ ($N_{\\text{jets}} \\geq 2, |y_{jet1} - y_{jet2}| \\geq 2 $)";
        var = "$y_{\\text{sum(Z,jet1)}}$";
        dSigma = "$\\frac{d\\sigma}{dy_{\\text{sum(Z,jet1)}}} \\tiny{\\left[\\text{pb}\\right]}$";
    }

    if (variable.Index("DifZFirstJetRapidity_DifJetRapiditys2_Zinc2jet") >= 0) {
        title = "$y_\\text{diff(Z,jet1)}$ ($N_{\\text{jets}} \\geq 2, |y_{jet1} - y_{jet2}| \\leq 2 $)";
        var = "$y_{\\text{diff(Z,jet1)}}$";
        dSigma = "$\\frac{d\\sigma}{dy_{\\text{diff(Z,jet1)}}} \\tiny{\\left[\\text{pb}\\right]}$";
    }

    if (variable.Index("DifZFirstJetRapidity_DifJetRapidityl2_Zinc2jet") >= 0) {
        title = "$y_\\text{diff(Z,jet1)}$ ($N_{\\text{jets}} \\geq 2, |y_{jet1} - y_{jet2}| \\geq 2 $)";
        var = "$y_{\\text{diff(Z,jet1)}}$";
        dSigma = "$\\frac{d\\sigma}{dy_{\\text{diff(Z,jet1)}}} \\tiny{\\left[\\text{pb}\\right]}$";
    }

    if (variable.Index("SumZFirstJetEta_Zinc1jet") >= 0) {
        title = "$\\eta_\\text{sum(Z,jet1)}$ ($N_{\\text{jets}} \\geq 1 $)";
        var = "$\\eta_{\\text{sum(Z,jet1)}}$";
        dSigma = "$\\frac{d\\sigma}{d\\eta_{\\text{sum(Z,jet1)}}} \\tiny{\\left[\\text{pb}\\right]}$";
    }

    if (variable.Index("DifZFirstJetEta_Zinc1jet") >= 0) {
        title = "$\\eta_\\text{diff(Z,jet1)}$ ($N_{\\text{jets}} \\geq 1 $)";
        var = "$\\eta_{\\text{diff(Z,jet1)}}$";
        dSigma = "$\\frac{d\\sigma}{d\\eta_{\\text{diff(Z,jet1)}}} \\tiny{\\left[\\text{pb}\\right]}$";
    }

    if (variable.Index("DPhiZFirstJet_Zinc1jet") >= 0) {
        title = "$\\Delta\\phi_\\text{Z,jet1}$ ($N_{\\text{jets}} \\geq 1 $)";
        var = "$\\Delta\\phi_{\\text{Z,jet1}}$";
        dSigma = "$\\frac{d\\sigma}{d\\Delta\\phi_{\\text{Z,jet1}}} \\tiny{\\left[\\text{pb}\\right]}$";
    }

    if (variable.Index("DPhiZFirstJet_Zinc2jet") >= 0) {
        title = "$\\Delta\\phi_\\text{Z,jet1}$ ($N_{\\text{jets}} \\geq 2 $)";
        var = "$\\Delta\\phi_{\\text{Z,jet1}}$";
        dSigma = "$\\frac{d\\sigma}{d\\Delta\\phi_{\\text{Z,jet1}}} \\tiny{\\left[\\text{pb}\\right]}$";
    }

    if (variable.Index("DPhiZFirstJet_Zinc3jet") >= 0) {
        title = "$\\Delta\\phi_\\text{Z,jet1}$ ($N_{\\text{jets}} \\geq 3 $)";
        var = "$\\Delta\\phi_{\\text{Z,jet1}}$";
        dSigma = "$\\frac{d\\sigma}{d\\Delta\\phi_{\\text{Z,jet1}}} \\tiny{\\left[\\text{pb}\\right]}$";
    }

    if (variable.Index("DPhiZFirstJet_ZPt150_Zinc1jet") >= 0) {
        title = "$\\Delta\\phi_\\text{Z,jet1}$ ($N_{\\text{jets}} \\geq 1, p_{\\text{T}}^{\\text{Z}} \\geq 150 \\text{GeV}$)";
        var = "$\\Delta\\phi_{\\text{Z,jet1}}$";
        dSigma = "$\\frac{d\\sigma}{d\\Delta\\phi_{\\text{Z,jet1}}} \\tiny{\\left[\\text{pb}\\right]}$";
    }

    if (variable.Index("DPhiZFirstJet_ZPt150_Zinc2jet") >= 0) {
        title = "$\\Delta\\phi_\\text{Z,jet1}$ ($N_{\\text{jets}} \\geq 2, p_{\\text{T}}^{\\text{Z}} \\geq 150 \\text{GeV}$)";
        var = "$\\Delta\\phi_{\\text{Z,jet1}}$";
        dSigma = "$\\frac{d\\sigma}{d\\Delta\\phi_{\\text{Z,jet1}}} \\tiny{\\left[\\text{pb}\\right]}$";
    }

    if (variable.Index("DPhiZFirstJet_ZPt150_Zinc3jet") >= 0) {
        title = "$\\Delta\\phi_\\text{Z,jet1}$ ($N_{\\text{jets}} \\geq 3, p_{\\text{T}}^{\\text{Z}} \\geq 150 \\text{GeV}$)";
        var = "$\\Delta\\phi_{\\text{Z,jet1}}$";
        dSigma = "$\\frac{d\\sigma}{d\\Delta\\phi_{\\text{Z,jet1}}} \\tiny{\\left[\\text{pb}\\right]}$";
    }

    if (variable.Index("DPhiZFirstJet_ZPt300_Zinc1jet") >= 0) {
        title = "$\\Delta\\phi_\\text{Z,jet1}$ ($N_{\\text{jets}} \\geq 1, p_{\\text{T}}^{\\text{Z}} \\geq 300 \\text{GeV}$)";
        var = "$\\Delta\\phi_{\\text{Z,jet1}}$";
        dSigma = "$\\frac{d\\sigma}{d\\Delta\\phi_{\\text{Z,jet1}}} \\tiny{\\left[\\text{pb}\\right]}$";
    }

    if (variable.Index("DPhiZFirstJet_ZPt300_Zinc2jet") >= 0) {
        title = "$\\Delta\\phi_\\text{Z,jet1}$ ($N_{\\text{jets}} \\geq 2, p_{\\text{T}}^{\\text{Z}} \\geq 300 \\text{GeV}$)";
        var = "$\\Delta\\phi_{\\text{Z,jet1}}$";
        dSigma = "$\\frac{d\\sigma}{d\\Delta\\phi_{\\text{Z,jet1}}} \\tiny{\\left[\\text{pb}\\right]}$";
    }

    if (variable.Index("DPhiZFirstJet_ZPt300_Zinc3jet") >= 0) {
        title = "$\\Delta\\phi_\\text{Z,jet1}$ ($N_{\\text{jets}} \\geq 3, p_{\\text{T}}^{\\text{Z}} \\geq 300 \\text{GeV}$)";
        var = "$\\Delta\\phi_{\\text{Z,jet1}}$";
        dSigma = "$\\frac{d\\sigma}{d\\Delta\\phi_{\\text{Z,jet1}}} \\tiny{\\left[\\text{pb}\\right]}$";
    }

    if (variable.Index("DPhiZSecondJet_Zinc3jet") >= 0) {
        title = "$\\Delta\\phi_\\text{Z,jet2}$ ($N_{\\text{jets}} \\geq 3 $)";
        var = "$\\Delta\\phi_{\\text{Z,jet2}}$";
        dSigma = "$\\frac{d\\sigma}{d\\Delta\\phi_{\\text{Z,jet2}}} \\tiny{\\left[\\text{pb}\\right]}$";
    }

    if (variable.Index("DPhiZThirdJet_Zinc3jet") >= 0) {
        title = "$\\Delta\\phi_\\text{Z,jet3}$ ($N_{\\text{jets}} \\geq 3 $)";
        var = "$\\Delta\\phi_{\\text{Z,jet3}}$";
        dSigma = "$\\frac{d\\sigma}{d\\Delta\\phi_{\\text{Z,jet3}}} \\tiny{\\left[\\text{pb}\\right]}$";
    }

    if (variable.Index("DPhiZSecondJet_ZPt150_Zinc3jet") >= 0) {
        title = "$\\Delta\\phi_\\text{Z,jet2}$ ($N_{\\text{jets}} \\geq 3, p_{\\text{T}}^{\\text{Z}} \\geq 150 \\text{GeV}$)";
        var = "$\\Delta\\phi_{\\text{Z,jet2}}$";
        dSigma = "$\\frac{d\\sigma}{d\\Delta\\phi_{\\text{Z,jet2}}} \\tiny{\\left[\\text{pb}\\right]}$";
    }

    if (variable.Index("DPhiZThirdJet_ZPt150_Zinc3jet") >= 0) {
        title = "$\\Delta\\phi_\\text{Z,jet3}$ ($N_{\\text{jets}} \\geq 3, p_{\\text{T}}^{\\text{Z}} \\geq 150 \\text{GeV}$)";
        var = "$\\Delta\\phi_{\\text{Z,jet3}}$";
        dSigma = "$\\frac{d\\sigma}{d\\Delta\\phi_{\\text{Z,jet3}}} \\tiny{\\left[\\text{pb}\\right]}$";
    }

    if (variable.Index("DPhiZSecondJet_ZPt300_Zinc3jet") >= 0) {
        title = "$\\Delta\\phi_\\text{Z,jet2}$ ($N_{\\text{jets}} \\geq 3, p_{\\text{T}}^{\\text{Z}} \\geq 300 \\text{GeV}$)";
        var = "$\\Delta\\phi_{\\text{Z,jet2}}$";
        dSigma = "$\\frac{d\\sigma}{d\\Delta\\phi_{\\text{Z,jet2}}} \\tiny{\\left[\\text{pb}\\right]}$";
    }

    if (variable.Index("DPhiZThirdJet_ZPt300_Zinc3jet") >= 0) {
        title = "$\\Delta\\phi_\\text{Z,jet3}$ ($N_{\\text{jets}} \\geq 3, p_{\\text{T}}^{\\text{Z}} \\geq 300 \\text{GeV}$)";
        var = "$\\Delta\\phi_{\\text{Z,jet3}}$";
        dSigma = "$\\frac{d\\sigma}{d\\Delta\\phi_{\\text{Z,jet3}}} \\tiny{\\left[\\text{pb}\\right]}$";
    }

    if (variable.Index("DPhiFirstSecondJet_Zinc3jet") >= 0) {
        title = "$\\Delta\\phi_\\text{jet1,jet2}$ ($N_{\\text{jets}} \\geq 3 $)";
        var = "$\\Delta\\phi_{\\text{jet1,jet2}}$";
        dSigma = "$\\frac{d\\sigma}{d\\Delta\\phi_{\\text{jet1,jet2}}} \\tiny{\\left[\\text{pb}\\right]}$";
    }

    if (variable.Index("DPhiFirstThirdJet_Zinc3jet") >= 0) {
        title = "$\\Delta\\phi_\\text{jet1,jet3}$ ($N_{\\text{jets}} \\geq 3 $)";
        var = "$\\Delta\\phi_{\\text{jet1,jet3}}$";
        dSigma = "$\\frac{d\\sigma}{d\\Delta\\phi_{\\text{jet1,jet3}}} \\tiny{\\left[\\text{pb}\\right]}$";
    }

    if (variable.Index("DPhiSecondThirdJet_Zinc3jet") >= 0) {
        title = "$\\Delta\\phi_\\text{jet2,jet3}$ ($N_{\\text{jets}} \\geq 3 $)";
        var = "$\\Delta\\phi_{\\text{jet2,jet3}}$";
        dSigma = "$\\frac{d\\sigma}{d\\Delta\\phi_{\\text{jet2,jet3}}} \\tiny{\\left[\\text{pb}\\right]}$";
    }

    if (variable.Index("DPhiFirstSecondJet_ZPt150_Zinc3jet") >= 0) {
        title = "$\\Delta\\phi_\\text{jet1,jet2}$ ($N_{\\text{jets}} \\geq 3, p_{\\text{T}}^{\\text{Z}} \\geq 150 \\text{GeV}$)";
        var = "$\\Delta\\phi_{\\text{jet1,jet2}}$";
        dSigma = "$\\frac{d\\sigma}{d\\Delta\\phi_{\\text{jet1,jet2}}} \\tiny{\\left[\\text{pb}\\right]}$";
    }

    if (variable.Index("DPhiFirstThirdJet_ZPt150_Zinc3jet") >= 0) {
        title = "$\\Delta\\phi_\\text{jet1,jet3}$ ($N_{\\text{jets}} \\geq 3, p_{\\text{T}}^{\\text{Z}} \\geq 150 \\text{GeV}$)";
        var = "$\\Delta\\phi_{\\text{jet1,jet3}}$";
        dSigma = "$\\frac{d\\sigma}{d\\Delta\\phi_{\\text{jet1,jet3}}} \\tiny{\\left[\\text{pb}\\right]}$";
    }

    if (variable.Index("DPhiSecondThirdJet_ZPt150_Zinc3jet") >= 0) {
        title = "$\\Delta\\phi_\\text{jet2,jet3}$ ($N_{\\text{jets}} \\geq 3, p_{\\text{T}}^{\\text{Z}} \\geq 150 \\text{GeV}$)";
        var = "$\\Delta\\phi_{\\text{jet2,jet3}}$";
        dSigma = "$\\frac{d\\sigma}{d\\Delta\\phi_{\\text{jet2,jet3}}} \\tiny{\\left[\\text{pb}\\right]}$";
    }

    if (variable.Index("DPhiFirstSecondJet_ZPt300_Zinc3jet") >= 0) {
        title = "$\\Delta\\phi_\\text{jet1,jet2}$ ($N_{\\text{jets}} \\geq 3, p_{\\text{T}}^{\\text{Z}} \\geq 300 \\text{GeV}$)";
        var = "$\\Delta\\phi_{\\text{jet1,jet2}}$";
        dSigma = "$\\frac{d\\sigma}{d\\Delta\\phi_{\\text{jet1,jet2}}} \\tiny{\\left[\\text{pb}\\right]}$";
    }

    if (variable.Index("DPhiFirstThirdJet_ZPt300_Zinc3jet") >= 0) {
        title = "$\\Delta\\phi_\\text{jet1,jet3}$ ($N_{\\text{jets}} \\geq 3, p_{\\text{T}}^{\\text{Z}} \\geq 300 \\text{GeV}$)";
        var = "$\\Delta\\phi_{\\text{jet1,jet3}}$";
        dSigma = "$\\frac{d\\sigma}{d\\Delta\\phi_{\\text{jet1,jet3}}} \\tiny{\\left[\\text{pb}\\right]}$";
    }

    if (variable.Index("DPhiSecondThirdJet_ZPt300_Zinc3jet") >= 0) {
        title = "$\\Delta\\phi_\\text{jet2,jet3}$ ($N_{\\text{jets}} \\geq 3, p_{\\text{T}}^{\\text{Z}} \\geq 300 \\text{GeV}$)";
        var = "$\\Delta\\phi_{\\text{jet2,jet3}}$";
        dSigma = "$\\frac{d\\sigma}{d\\Delta\\phi_{\\text{jet2,jet3}}} \\tiny{\\left[\\text{pb}\\right]}$";
    }

    if (variable.Index("DPhiZFirstJet_ZPt150_HT300_Zinc3jet") >= 0) {
        title = "$\\Delta\\phi_\\text{Z,jet1}$ ($N_{\\text{jets}} \\geq 3, p_{\\text{T}}^{\\text{Z}} \\geq 150 \\text{GeV}, H_{\\text{T}} \\geq 300 \\text{GeV}$)";
        var = "$\\Delta\\phi_{\\text{Z,jet1}}$";
        dSigma = "$\\frac{d\\sigma}{d\\Delta\\phi_{\\text{Z,jet1}}} \\tiny{\\left[\\text{pb}\\right]}$";
    }

    if (variable.Index("DPhiZSecondJet_ZPt150_HT300_Zinc3jet") >= 0) {
        title = "$\\Delta\\phi_\\text{Z,jet2}$ ($N_{\\text{jets}} \\geq 3, p_{\\text{T}}^{\\text{Z}} \\geq 150 \\text{GeV}, H_{\\text{T}} \\geq 300 \\text{GeV}$)";
        var = "$\\Delta\\phi_{\\text{Z,jet2}}$";
        dSigma = "$\\frac{d\\sigma}{d\\Delta\\phi_{\\text{Z,jet2}}} \\tiny{\\left[\\text{pb}\\right]}$";
    }

    if (variable.Index("DPhiZThirdJet_ZPt150_HT300_Zinc3jet") >= 0) {
        title = "$\\Delta\\phi_\\text{Z,jet3}$ ($N_{\\text{jets}} \\geq 3, p_{\\text{T}}^{\\text{Z}} \\geq 150 \\text{GeV}, H_{\\text{T}} \\geq 300 \\text{GeV}$)";
        var = "$\\Delta\\phi_{\\text{Z,jet3}}$";
        dSigma = "$\\frac{d\\sigma}{d\\Delta\\phi_{\\text{Z,jet3}}} \\tiny{\\left[\\text{pb}\\right]}$";
    }

    if (variable.Index("dPhiLepJet1_Zinc1jet") >= 0) {
        title = "$\\Delta\\phi(\\text{mu, j_{1}})$ ($N_{\\text{jets}} \\geq 1$)";
        var = "$\\Delta\\phi(\\text{mu, j_{1}}})$";
        dSigma = "$\\frac{d\\sigma}{d\\Delta\\phi(\\text{mu, j_{1}})} \\tiny{\\left[\\text{pb}\\right]}$";
    }

    if (variable.Index("dRptmin100LepCloseJetCo300dR04_Zinc1jet") >= 0) {
        title = "$\\Delta\\R(\\text{mu, closest jet})$ ($N_{\\text{jets}} \\geq 1$)";
        var = "$\\Delta\\R(\\text{mu, closest jet})$";
        dSigma = "$\\frac{d\\sigma}{d\\Delta\\R(\\text{mu, jet})} \\tiny{\\left[\\text{pb}\\right]}$";
    }

    // jet pt distributions
    if (xtitle.Index("p_{T}(j_{1})") >= 0) {
        title = "$1^{\\text{st}}$ jet $p_{\\text{T}}$ ($N_{\\text{jets}} \\geq 1$)";
        var = "$p_{\\text{T}}(j_{1})$ \\tiny{[GeV]}";
        dSigma = "$\\frac{d\\sigma}{dp_{\\text{T}}(j_{1})} \\tiny{\\left[\\frac{\\text{pb}}{\\text{GeV}}\\right]}$";
    }
    if (xtitle.Index("p_{T}(j_{2})") >= 0) {
        title = "$2^{\\text{nd}}$ jet $p_{\\text{T}}$ ($N_{\\text{jets}} \\geq 2$)";
        var = "$p_{\\text{T}}(j_{2})$ \\tiny{[GeV]}";
        dSigma = "$\\frac{d\\sigma}{dp_{\\text{T}}(j_{2})} \\tiny{\\left[\\frac{\\text{pb}}{\\text{GeV}}\\right]}$";
    }
    if (xtitle.Index("p_{T}(j_{3})") >= 0) {
        title = "$3^{\\text{rd}}$ jet $p_{\\text{T}}$ ($N_{\\text{jets}} \\geq 3$)";
        var = "$p_{\\text{T}}(j_{3})$ \\tiny{[GeV]}";
        dSigma = "$\\frac{d\\sigma}{dp_{\\text{T}}(j_{3})} \\tiny{\\left[\\frac{\\text{pb}}{\\text{GeV}}\\right]}$";
    }
    if (xtitle.Index("p_{T}(j_{4})") >= 0) {
        title = "$4^{\\text{th}}$ jet $p_{\\text{T}}$ ($N_{\\text{jets}} \\geq 4$)";
        var = "$p_{\\text{T}}(j_{4})$ \\tiny{[GeV]}";
        dSigma = "$\\frac{d\\sigma}{dp_{\\text{T}}(j_{4})} \\tiny{\\left[\\frac{\\text{pb}}{\\text{GeV}}\\right]}$";
    }
    if (xtitle.Index("p_{T}(j_{5})") >= 0) {
        title = "$5^{\\text{th}}$ jet $p_{\\text{T}}$ ($N_{\\text{jets}} \\geq 5$)";
        var = "$p_{\\text{T}}(j_{5})$ \\tiny{[GeV]}";
        dSigma = "$\\frac{d\\sigma}{dp_{\\text{T}}(j_{5})} \\tiny{\\left[\\frac{\\text{pb}}{\\text{GeV}}\\right]}$";
    }
    if (xtitle.Index("p_{T}(j_{6})") >= 0) {
        title = "$6^{\\text{th}}$ jet $p_{\\text{T}}$ ($N_{\\text{jets}} \\geq 6$)";
        var = "$p_{\\text{T}}(j_{6})$ \\tiny{[GeV]}";
        dSigma = "$\\frac{d\\sigma}{dp_{\\text{T}}(j_{6})} \\tiny{\\left[\\frac{\\text{pb}}{\\text{GeV}}\\right]}$";
    }

    // jet HT distributions
    if (xtitle.Index("H_{T}") >= 0 && title.Index("N_{jets} #geq 1") >= 0) {
        title = "$H_{\\text{T}}$ ($N_{\\text{jets}} \\geq 1$)";
        var = "$H_{\\text{T}}$ \\tiny{[GeV]}";
        dSigma = "$\\frac{d\\sigma}{dH_{\\text{T}}} \\tiny{\\left[\\frac{\\text{pb}}{\\text{GeV}}\\right]}$";
    }
    if (xtitle.Index("H_{T}") >= 0 && title.Index("N_{jets} #geq 2") >= 0) {
        title = "$H_{\\text{T}}$ ($N_{\\text{jets}} \\geq 2$)";
        var = "$H_{\\text{T}}$ \\tiny{[GeV]}";
        dSigma = "$\\frac{d\\sigma}{dH_{\\text{T}}} \\tiny{\\left[\\frac{\\text{pb}}{\\text{GeV}}\\right]}$";
    }
    if (xtitle.Index("H_{T}") >= 0 && title.Index("N_{jets} #geq 3") >= 0) {
        title = "$H_{\\text{T}}$ ($N_{\\text{jets}} \\geq 3$)";
        var = "$H_{\\text{T}}$ \\tiny{[GeV]}";
        dSigma = "$\\frac{d\\sigma}{dH_{\\text{T}}} \\tiny{\\left[\\frac{\\text{pb}}{\\text{GeV}}\\right]}$";
    }
    if (xtitle.Index("H_{T}") >= 0 && title.Index("N_{jets} #geq 4") >= 0) {
        title = "$H_{\\text{T}}$ ($N_{\\text{jets}} \\geq 4$)";
        var = "$H_{\\text{T}}$ \\tiny{[GeV]}";
        dSigma = "$\\frac{d\\sigma}{dH_{\\text{T}}} \\tiny{\\left[\\frac{\\text{pb}}{\\text{GeV}}\\right]}$";
    }
    if (xtitle.Index("H_{T}") >= 0 && title.Index("N_{jets} #geq 5") >= 0) {
        title = "$H_{\\text{T}}$ ($N_{\\text{jets}} \\geq 5$)";
        var = "$H_{\\text{T}}$ \\tiny{[GeV]}";
        dSigma = "$\\frac{d\\sigma}{dH_{\\text{T}}} \\tiny{\\left[\\frac{\\text{pb}}{\\text{GeV}}\\right]}$";
    }
    if (xtitle.Index("H_{T}") >= 0 && title.Index("N_{jets} #geq 6") >= 0) {
        title = "$H_{\\text{T}}$ ($N_{\\text{jets}} \\geq 6$)";
        var = "$H_{\\text{T}}$ \\tiny{[GeV]}";
        dSigma = "$\\frac{d\\sigma}{dH_{\\text{T}}} \\tiny{\\left[\\frac{\\text{pb}}{\\text{GeV}}\\right]}$";
    }

    // jet eta distributions
    if (xtitle.Index("eta(j_{1})") >= 0) {
        title = "$1^{\\text{st}}$ jet $\\vert\\eta\\vert$ ($N_{\\text{jets}} \\geq 1$)";
        var = "$\\eta(j_{1})$";
        dSigma = "$\\frac{d\\sigma}{d\\eta(j_{1})} \\tiny{\\left[\\text{pb}\\right]}$";
    }
    if (xtitle.Index("eta(j_{2})") >= 0) {
        title = "$2^{\\text{nd}}$ jet $\\vert\\eta\\vert$ ($N_{\\text{jets}} \\geq 2$)";
        var = "$\\eta(j_{2})$";
        dSigma = "$\\frac{d\\sigma}{d\\eta(j_{2})} \\tiny{\\left[\\text{pb}\\right]}$";
    }
    if (xtitle.Index("eta(j_{3})") >= 0) {
        title = "$3^{\\text{rd}}$ jet $\\vert\\eta\\vert$ ($N_{\\text{jets}} \\geq 3$)";
        var = "$\\eta(j_{3})$";
        dSigma = "$\\frac{d\\sigma}{d\\eta(j_{3})} \\tiny{\\left[\\text{pb}\\right]}$";
    }
    if (xtitle.Index("eta(j_{4})") >= 0) {
        title = "$4^{\\text{th}}$ jet $\\vert\\eta\\vert$ ($N_{\\text{jets}} \\geq 4$)";
        var = "$\\eta(j_{4})$";
        dSigma = "$\\frac{d\\sigma}{d\\eta(j_{4})} \\tiny{\\left[\\text{pb}\\right]}$";
    }
    if (xtitle.Index("eta(j_{5})") >= 0) {
        title = "$5^{\\text{th}}$ jet $\\vert\\eta\\vert$ ($N_{\\text{jets}} \\geq 5$)";
        var = "$\\eta(j_{5})$";
        dSigma = "$\\frac{d\\sigma}{d\\eta(j_{5})} \\tiny{\\left[\\text{pb}\\right]}$";
    }
    if (xtitle.Index("eta(j_{6})") >= 0) {
        title = "$6^{\\text{th}}$ jet $\\vert\\eta\\vert$ ($N_{\\text{jets}} \\geq 6$)";
        var = "$\\eta(j_{6})$";
        dSigma = "$\\frac{d\\sigma}{d\\eta(j_{6})} \\tiny{\\left[\\text{pb}\\right]}$";
    }

    // dijet mass distribution
    if (xtitle.Index("M_{j_{1}j_{2}}") >= 0) {
        title = "dijet mass $M_{jj}$ ($N_{\\text{jets}} \\geq 2$)";
        var = "$M_{jj}$ \\tiny{[GeV]}";
        dSigma = "$\\frac{d\\sigma}{dM_{jj}} \\tiny{\\left[\\frac{\\text{pb}}{\\text{GeV}}\\right]}$";
    }

    if (doNormalized) {
        dSigma = "$\\frac{1}{\\sigma}$ " + dSigma;
        dSigma.ReplaceAll("\\frac{\\text{pb}}", "\\frac{1}");
        dSigma.ReplaceAll("\\tiny{\\left[\\text{pb}\\right]}", "");
    }

}


TGraphAsymmErrors* createScaleSystGraphNNLO1j(TString lepSel, TString variable, const TGraphAsymmErrors *grGenToCentral)
{
	int nPoints = grGenToCentral->GetN();
	double *xCoor    = new double[nPoints];
	double *yCoor    = new double[nPoints];
	double *xErr     = new double[nPoints];
	double *yErrUp   = new double[nPoints];
	double *yErrDown = new double[nPoints];
	
	TString histoDir = cfg.getS("histoDir");
	
	TString inFileName = histoDir + "/" + lepSel + "_13TeV_NNLO1J.root";
	std::cout << " createScaleSystGraphNNLO1j from file: " << inFileName << std::endl;
	
	TH1D *hErrorsUp;
	TH1D *hErrorsDown;
	TH1D *hGenCentral;
	TFile *fNNLO = new TFile(inFileName);
	hErrorsUp = (TH1D*)fNNLO->Get("gen" + variable + "Max"); // this has only scale uncer
	hErrorsDown = (TH1D*)fNNLO->Get("gen" + variable + "Min"); // this has only scale uncer
	hGenCentral = (TH1D*)fNNLO->Get("gen" + variable); //--- the stat err is from non-perturbative corr.
	hErrorsUp->Divide(hGenCentral);
	hErrorsDown->Divide(hGenCentral);
	
	for (int i(0); i < nPoints; i++) {
		grGenToCentral->GetPoint(i, xCoor[i], yCoor[i]);
		
		xErr[i] = grGenToCentral->GetErrorXlow(i);
		
		double fracStat2Up(0);
		if (yCoor[i]>0) fracStat2Up = pow(grGenToCentral->GetErrorYhigh(i)/yCoor[i], 2);
		double fracScal2Up(0);
		if (hGenCentral->GetBinContent(i+1)>0) fracScal2Up = pow(fabs(hErrorsUp->GetBinContent(i+1) - 1), 2);
		double fracTotUp = sqrt(fracStat2Up + fracScal2Up);
		yErrUp[i] = fracTotUp * yCoor[i];
		
		double fracStat2Dn(0);
		if (yCoor[i]>0) fracStat2Dn = pow(grGenToCentral->GetErrorYlow(i)/yCoor[i], 2);
		double fracScal2Dn(0);
		if (hGenCentral->GetBinContent(i+1)>0) fracScal2Dn = pow(fabs(hErrorsDown->GetBinContent(i+1) - 1), 2);
		double fracTotDn = sqrt(fracStat2Dn + fracScal2Dn);
		yErrDown[i] = fracTotDn * yCoor[i];
		
		std::cout << "  X coor: " << xCoor[i] << " Y coor: " << yCoor[i]
		<< "  tot uncertainty up: " << yErrUp[i]
		<< "  tot uncertainty down: " << yErrDown[i]
		<< std::endl;
	}
	
	TGraphAsymmErrors *grScaleSyst = new TGraphAsymmErrors(nPoints, xCoor, yCoor, xErr, xErr, yErrDown, yErrUp);
	delete [] xCoor; delete [] yCoor; delete [] xErr; delete [] yErrDown; delete [] yErrUp;
	fNNLO->Close();
	
	return grScaleSyst;
	
	
    //    for(int i = 1; i<= nBins; i++){
    //        int nBins = genNNLO->GetNbinsX()
    //        nnlo_ySystUp[i-1] = pow( genScErrUp->GetBinContent(i)/genNNLO->GetBinContent(i) , 2);
    //        nnlo_ySystDn[i-1] = pow( genScErrDn->GetBinContent(i)/genNNLO->GetBinContent(i) , 2);
    //        //nnlo_ySystUp[i-1] += pow( hisHCorr->GetBinError(i)/hisHCorr->GetBinContent(i), 2);
    //        //nnlo_ySystDn[i-1] += pow( hisHCorr->GetBinError(i)/hisHCorr->GetBinContent(i), 2);
    //
    //        //--- correction factor if applicable ---
    //        //genNNLO->Scale(1.011);
    //        //genNNLO->Multiply(hisHCorr);
    //
    //        nnlo_yCoor[i-1] = genNNLO->GetBinContent(i);
    //        nnlo_ySystUp[i-1] = sqrt(nnlo_ySystUp[i-1]) * genNNLO->GetBinContent(i);
    //        nnlo_ySystDn[i-1] = sqrt(nnlo_ySystDn[i-1]) * genNNLO->GetBinContent(i);
    //
    //        nnlo_xCoor[i-1]    = genNNLO->GetBinCenter(i);
    //        nnlo_xErr[i-1]     = 0.5 * genNNLO->GetBinWidth(i);
    //    }
    //
    //    //--- Set up for systematic plot on pad 1
    //    genNNLOgrSyst = new TGraphAsymmErrors(nBins, nnlo_xCoor, nnlo_yCoor, nnlo_xErr, nnlo_xErr, nnlo_ySystDn, nnlo_ySystUp);
    //
    //    return genNNLOgrSyst;
}


TGraphAsymmErrors* createTotSystGraphAMCNLO(TString lepSel, TString variable, const TGraphAsymmErrors *grGenToCentral)
{
	//if (variable == "ZNGoodJetsFull_Zexc") variable = "ZNGoodJets_Zexc";
	int nPoints = grGenToCentral->GetN();
	double *xCoor    = new double[nPoints];
	double *yCoor    = new double[nPoints];
	double *xErr     = new double[nPoints];
	double *yErrUp   = new double[nPoints];
	double *yErrDown = new double[nPoints];
	
	TString histoDir = cfg.getS("histoDir");
	
	TString inFileName = histoDir + "/" + lepSel + "_13TeV_AMCNLO_Syst.root";
	std::cout << " createTotSystGraphAMCNLO from file: " << inFileName << std::endl;
	
	TH1D *hErrorsUp;
	TH1D *hErrorsDown;
	TH1D *hGenCentral;
	TFile *fSyst = new TFile(inFileName);
	hErrorsUp = (TH1D*)fSyst->Get("gen" + variable + "Max");
	hErrorsDown = (TH1D*)fSyst->Get("gen" + variable + "Min");
	hGenCentral = (TH1D*)fSyst->Get("gen" + variable + "Cen");
	hErrorsUp->Divide(hGenCentral);
	hErrorsDown->Divide(hGenCentral);
	
	for (int i(0); i < nPoints; i++) {
		grGenToCentral->GetPoint(i, xCoor[i], yCoor[i]);
		
		xErr[i] = grGenToCentral->GetErrorXlow(i);
		
		double fracTotUp(0);
		if (hGenCentral->GetBinContent(i+1)>0) fracTotUp = fabs(hErrorsUp->GetBinContent(i+1) - 1);
		yErrUp[i] = fracTotUp * yCoor[i];
		
		double fracTotDn(0);
		if (hGenCentral->GetBinContent(i+1)>0) fracTotDn = fabs(hErrorsDown->GetBinContent(i+1) - 1);
		yErrDown[i] = fracTotDn * yCoor[i];
		
		std::cout << "  X coor: " << xCoor[i] << " Y coor: " << yCoor[i]
		<< "  tot uncertainty up: " << yErrUp[i]
		<< "  tot uncertainty down: " << yErrDown[i]
		<< std::endl;
	}
	
	TGraphAsymmErrors *grTotSyst = new TGraphAsymmErrors(nPoints, xCoor, yCoor, xErr, xErr, yErrDown, yErrUp);
	delete [] xCoor; delete [] yCoor; delete [] xErr; delete [] yErrDown; delete [] yErrUp;
	fSyst->Close();
	
	return grTotSyst;
	
}



