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
#include "PlotSettings.h"
#include "ConfigVJets.h"
#include "functions.h"

#include "CombineUnfoldedRun2.h"

extern ConfigVJets cfg;

using namespace std;

void CombineUnfoldedRun2(TString lepSel, int year, TString algo, TString histoDir, TString unfoldDir, int jetPtMin, int jetEtaMax, TString variable, bool doNormalized, int whichSyst){
    gStyle->SetOptStat(0);

    //--- Create output directory if does not exist ---
    unfoldDir += "_Run2";
    system("mkdir -p " + unfoldDir);

    // Integrated luminosity for each year
    double intLumi_2016 = 35916.982;
    double intLumi_2017 = 41524.945;
    double intLumi_2018 = 59717.255;
    double intLumi_Run2 = intLumi_2016 + intLumi_2017 + intLumi_2018;

    //--- Choosing variable of interest
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

    // Loop over all variables
    for (int i = start; i < end; ++i){
        variable = VAROFINTERESTZJETS[i].name;
        // TH1::SetDefaultSumw2();
        // TH2::SetDefaultSumw2();

        //--- Create output file
        TString outputFileName = unfoldDir + "/" + lepSel + "_" + variable + "_" + "Central"; 
        TFile *outputRootFile = new TFile(outputFileName + ".root", "RECREATE");

        // Open the files with unfolded distributions for each year
        // Currently working with a one file per variable, and also per systematic (but just doing Central for now)
        // "Central" is the name of the systematic variation (this changes as we vary systematics)
        // TODO: will loop over systematics here later....
        TString fileIn = lepSel + "_" + variable + "_" + "Central" + ".root";
        TFile *fUnf2016 = new TFile("UnfoldingCheck_2016/" + fileIn, "read");
        TFile *fUnf2017 = new TFile("UnfoldingCheck_2017/" + fileIn, "read");
        TFile *fUnf2018 = new TFile("UnfoldingCheck_2018/" + fileIn, "read");

        ////////////////////////////////////////////////////////////////////////////////////////////

        // Grab unfolded histograms that have not been scaled yet for diff xsec's
        TH1D *hUnfData_2016 = (TH1D*) fUnf2016->Get("hUnfDataNoScale");
        TH1D *hUnfData_2017 = (TH1D*) fUnf2017->Get("hUnfDataNoScale");
        TH1D *hUnfData_2018 = (TH1D*) fUnf2018->Get("hUnfDataNoScale");

        // Add all three years together for unfolded
        TH1D *hUnfData_Run2 = (TH1D*) hUnfData_2016->Clone();
        hUnfData_Run2->Add(hUnfData_2017);
        hUnfData_Run2->Add(hUnfData_2018);

        // Now scale all unfolded to get xsec (lumi & bin widths)
        // 1) Lumi
        hUnfData_2016->Scale(1./intLumi_2016);
        hUnfData_2017->Scale(1./intLumi_2017);
        hUnfData_2018->Scale(1./intLumi_2018);
        hUnfData_Run2->Scale(1./intLumi_Run2);
        // 2) Bin widths
        int nBins = hUnfData_Run2->GetNbinsX();
        for (int i = 1; i <= nBins; ++i){
            double binWidth = hUnfData_Run2->GetBinWidth(i);
            hUnfData_2016->SetBinContent(i, hUnfData_2016->GetBinContent(i)/binWidth);
            hUnfData_2016->SetBinError(i, hUnfData_2016->GetBinError(i)/binWidth);
            hUnfData_2017->SetBinContent(i, hUnfData_2017->GetBinContent(i)/binWidth);
            hUnfData_2017->SetBinError(i, hUnfData_2017->GetBinError(i)/binWidth);
            hUnfData_2018->SetBinContent(i, hUnfData_2018->GetBinContent(i)/binWidth);
            hUnfData_2018->SetBinError(i, hUnfData_2018->GetBinError(i)/binWidth);
            hUnfData_Run2->SetBinContent(i, hUnfData_Run2->GetBinContent(i)/binWidth);
            hUnfData_Run2->SetBinError(i, hUnfData_Run2->GetBinError(i)/binWidth);
        }

        // Reset names
        hUnfData_2016->SetName("hUnfData_2016");
        hUnfData_2017->SetName("hUnfData_2017");
        hUnfData_2018->SetName("hUnfData_2018");
        hUnfData_Run2->SetName("hUnfData_Run2");

        // Write to output file
        outputRootFile->cd();
        hUnfData_2016->Write();
        hUnfData_2017->Write();
        hUnfData_2018->Write();
        hUnfData_Run2->Write();
    
        // Print canvas w/ comparison among all three years and Run 2 combined
        TCanvas *canAllUnfXSecs = plotXSecComparison(variable, hUnfData_2016, hUnfData_2017, hUnfData_2018, hUnfData_Run2);
        canAllUnfXSecs->Draw();
        canAllUnfXSecs->SaveAs(outputFileName + "_UnfXSecComparison" + ".pdf");
        outputRootFile->cd();
        canAllUnfXSecs->Write();
        delete canAllUnfXSecs; // deallocate heap memory

        ////////////////////////////////////////////////////////////////////////////////////////////

        // also get reco-level and gen-level for all three years
        TH1D *recoData_binsMerged_2016 = (TH1D*) fUnf2016->Get("recoData_binsMerged");
        TH1D *recoData_binsMerged_2017 = (TH1D*) fUnf2017->Get("recoData_binsMerged");
        TH1D *recoData_binsMerged_2018 = (TH1D*) fUnf2018->Get("recoData_binsMerged");
        TH1D *genDY_2016 = (TH1D*) fUnf2016->Get("genDY");
        TH1D *genDY_2017 = (TH1D*) fUnf2017->Get("genDY");
        TH1D *genDY_2018 = (TH1D*) fUnf2018->Get("genDY");

        // NOTE: omitting the errors right now, because I think that the way
        // I've re-binned the reco distribution the errors are not properly set
        // TEMPORARY: manually set the bin errors for the reco'd dist's to 0
        for (int i = 1; i <= nBins; ++i){
            recoData_binsMerged_2016->SetBinError(i, 0.000001);
            recoData_binsMerged_2017->SetBinError(i, 0.000001);
            recoData_binsMerged_2018->SetBinError(i, 0.000001);
        }

        // scale for xsec
        // 1) lumi
        recoData_binsMerged_2016->Scale(1./intLumi_2016);
        recoData_binsMerged_2017->Scale(1./intLumi_2017);
        recoData_binsMerged_2018->Scale(1./intLumi_2018);
        genDY_2016->Scale(1./intLumi_2016);
        genDY_2017->Scale(1./intLumi_2017);
        genDY_2018->Scale(1./intLumi_2018);
        // 2) bin widths
        for (int i = 1; i <= nBins; ++i){
            double binWidth = recoData_binsMerged_2016->GetBinWidth(i);

            recoData_binsMerged_2016->SetBinContent(i, recoData_binsMerged_2016->GetBinContent(i)/binWidth);
            recoData_binsMerged_2016->SetBinError(i, recoData_binsMerged_2016->GetBinError(i)/binWidth);
            recoData_binsMerged_2017->SetBinContent(i, recoData_binsMerged_2017->GetBinContent(i)/binWidth);
            recoData_binsMerged_2017->SetBinError(i, recoData_binsMerged_2017->GetBinError(i)/binWidth);
            recoData_binsMerged_2018->SetBinContent(i, recoData_binsMerged_2018->GetBinContent(i)/binWidth);
            recoData_binsMerged_2018->SetBinError(i, recoData_binsMerged_2018->GetBinError(i)/binWidth);

            genDY_2016->SetBinContent(i, genDY_2016->GetBinContent(i)/binWidth);
            genDY_2016->SetBinError(i, genDY_2016->GetBinError(i)/binWidth);
            genDY_2017->SetBinContent(i, genDY_2017->GetBinContent(i)/binWidth);
            genDY_2017->SetBinError(i, genDY_2017->GetBinError(i)/binWidth);
            genDY_2018->SetBinContent(i, genDY_2018->GetBinContent(i)/binWidth);
            genDY_2018->SetBinError(i, genDY_2018->GetBinError(i)/binWidth);
        }

        // Reset names
        recoData_binsMerged_2016->SetName("recoData_binsMerged_scaled_2016");
        recoData_binsMerged_2017->SetName("recoData_binsMerged_scaled_2017");
        recoData_binsMerged_2018->SetName("recoData_binsMerged_scaled_2018");
        genDY_2016->SetName("genDY_scaled_2016");
        genDY_2017->SetName("genDY_scaled_2017");
        genDY_2018->SetName("genDY_scaled_2018");

        // Write to output file
        outputRootFile->cd();
        recoData_binsMerged_2016->Write();
        recoData_binsMerged_2017->Write();
        recoData_binsMerged_2018->Write();
        genDY_2016->Write();
        genDY_2017->Write();
        genDY_2018->Write();

        // Print canvas w/ comparison of reco and unfolded level for all three years
        TCanvas *canUnfReco_2016 = plotUnfRecoComparison(variable, 2016, hUnfData_2016, recoData_binsMerged_2016, genDY_2016); // 2016
        canUnfReco_2016->Draw();
        canUnfReco_2016->SaveAs(outputFileName + "_UnfRecoComparison_2016" + ".pdf");
        canUnfReco_2016->cd();
        canUnfReco_2016->Write();
        delete canUnfReco_2016;

        TCanvas *canUnfReco_2017 = plotUnfRecoComparison(variable, 2017, hUnfData_2017, recoData_binsMerged_2017, genDY_2017); // 2017
        canUnfReco_2017->Draw();
        canUnfReco_2017->SaveAs(outputFileName + "_UnfRecoComparison_2017" + ".pdf");
        canUnfReco_2017->cd();
        canUnfReco_2017->Write();
        delete canUnfReco_2017;

        TCanvas *canUnfReco_2018 = plotUnfRecoComparison(variable, 2018, hUnfData_2018, recoData_binsMerged_2018, genDY_2018); // 2018
        canUnfReco_2018->Draw();
        canUnfReco_2018->SaveAs(outputFileName + "_UnfRecoComparison_2018" + ".pdf");
        canUnfReco_2018->cd();
        canUnfReco_2018->Write();
        delete canUnfReco_2018;

        ////////////////////////////////////////////////////////////////////////////////////////////

        // Close input files and the output file
        fUnf2016->Close();
        fUnf2017->Close();
        fUnf2018->Close();
        outputRootFile->Close();
    } // end loop over all variables

    std::cout << std::endl;
}

TCanvas* plotXSecComparison(TString variable, TH1D *h2016, TH1D *h2017, TH1D *h2018, TH1D *hRun2){

    TH1D *hXSec2016 = (TH1D*) h2016->Clone();
    TH1D *hXSec2017 = (TH1D*) h2017->Clone();
    TH1D *hXSec2018 = (TH1D*) h2018->Clone();
    TH1D *hXSecRun2 = (TH1D*) hRun2->Clone();

    // Build canvas
    double maximum = hXSecRun2->GetMaximum();
    double minimum = hXSecRun2->GetMinimum();
    if (! minimum > 0) minimum = 1e-08;

    TString canvasName = "canvas_XSecAllYearsComparison_" + variable;
    TCanvas *plots = new TCanvas(canvasName, variable+", Unfolded", 600, 800); // need to deallocate heap memory for this later?
    plots->cd();

    // Pad 1 --------------------------------------------------------------
    TPad *plot1 = new TPad("plot1", "plot1", 0., 0., 0., 0.);
    plot1->SetPad(0.01, 0.35, 0.99, 0.99);
    plot1->SetTopMargin(0.11);
    plot1->SetBottomMargin(0.005);
    plot1->SetLeftMargin(0.13);
    plot1->SetRightMargin(0.07);
    plot1->SetFillStyle(0);
    plot1->SetTicks();
    plot1->SetLogy();
    plot1->Draw();
    plot1->cd();

    // aesthetics
    hXSec2016->SetLineColor(kOrange+10); // kOrange+9
    hXSec2016->SetLineWidth(2);
    hXSec2016->SetMarkerColor(kOrange+10);
    hXSec2016->SetMarkerStyle(20);

    hXSec2017->SetLineColor(kAzure+7); // kAzure-6
    hXSec2017->SetLineWidth(2);
    hXSec2017->SetMarkerColor(kAzure+7);
    hXSec2017->SetMarkerStyle(20);

    hXSec2018->SetLineColor(kSpring); // kSpring-1
    hXSec2018->SetLineWidth(2);
    hXSec2018->SetMarkerColor(kSpring);
    hXSec2018->SetMarkerStyle(20);

    hXSecRun2->SetLineColor(kBlack);
    hXSecRun2->SetLineWidth(2);
    hXSecRun2->SetMarkerColor(kBlack);
    hXSecRun2->SetMarkerStyle(20);

    // axes, titles
    hXSec2016->SetTitle("");
    hXSec2016->GetYaxis()->SetRangeUser(0.5*minimum, 7.*maximum);
    hXSec2016->GetYaxis()->SetTitle("Diff. Cross Section");
    hXSec2016->GetYaxis()->SetLabelSize(0.048);
    hXSec2016->GetYaxis()->SetLabelOffset(0.002);
    hXSec2016->GetYaxis()->SetTitleSize(0.051);
    hXSec2016->GetYaxis()->SetTitleOffset(1.09);
    // hXSec2016->GetXaxis()->SetTitle("");
    
    // draw
    hXSec2016->Draw("E");
    hXSec2017->Draw("E same");
    hXSec2018->Draw("E same");
    hXSecRun2->Draw("E same");
 
    // legend
    TLegend *legend = new TLegend(0.7, 0.74, 0.99, 0.98);
    legend->SetFillColor(0);
    legend->SetFillStyle(1001);
    legend->SetBorderSize(1);
    legend->SetX1(0.6); //0.44
    legend->SetY1(0.7);
    legend->SetX2(0.96);
    legend->SetY2(0.98);
    legend->SetTextSize(.042);
    legend->AddEntry(hXSec2016, "2016, Unfolded", "lpe");
    legend->AddEntry(hXSec2017, "2017, Unfolded", "lpe");
    legend->AddEntry(hXSec2018, "2018, Unfolded", "lpe");
    legend->AddEntry(hXSecRun2, "Run 2, Unfolded", "lpe");
    legend->Draw("same");

    // Pad 2 --------------------------------------------------------------
    plots->cd();
    TPad *plot2 = new TPad("plot2", "plot2", 0., 0., 0., 0.);
    plot2->SetPad(0.01, 0.01, 0.99, 0.35);
    plot2->SetTopMargin(0.0);
    plot2->SetBottomMargin(0.3);
    plot2->SetLeftMargin(0.13);
    plot2->SetRightMargin(0.07);
    plot2->SetFillStyle(0);
    plot2->SetTicks();
    plot2->Draw();
    plot2->cd();

    // get ratio distributions
    TH1D *ratio2016ToRun2 = (TH1D*) hXSec2016->Clone();
    TH1D *ratio2017ToRun2 = (TH1D*) hXSec2017->Clone();
    TH1D *ratio2018ToRun2 = (TH1D*) hXSec2018->Clone();
    ratio2016ToRun2->Divide(hXSecRun2);
    ratio2017ToRun2->Divide(hXSecRun2);
    ratio2018ToRun2->Divide(hXSecRun2);

    // axes, titles
    ratio2016ToRun2->SetTitle("");
    ratio2016ToRun2->GetYaxis()->SetRangeUser(0.49, 1.51);
    ratio2016ToRun2->GetYaxis()->SetTitle("Ratio to Run 2");
    ratio2016ToRun2->GetYaxis()->CenterTitle();
    ratio2016ToRun2->GetYaxis()->SetLabelSize(0.073); //0.09
    ratio2016ToRun2->GetYaxis()->SetLabelOffset(0.002); 
    ratio2016ToRun2->GetYaxis()->SetTitleSize(0.082); //0.104
    ratio2016ToRun2->GetYaxis()->SetTitleOffset(0.6); //0.48
    ratio2016ToRun2->GetXaxis()->SetTitleSize(0.11);
    ratio2016ToRun2->GetXaxis()->SetTitleOffset(1.2);
    ratio2016ToRun2->GetXaxis()->SetLabelSize(0.082);
    ratio2016ToRun2->GetXaxis()->SetLabelOffset(0.018);

    // draw
    ratio2016ToRun2->Draw("E");
    ratio2017ToRun2->Draw("E same");
    ratio2018ToRun2->Draw("E same");

    // line
    TLine *line1 = new TLine(hXSec2016->GetXaxis()->GetXmin(), 1., hXSec2016->GetXaxis()->GetXmax(), 1.);
    line1->SetLineColor(kBlack);
    line1->SetLineWidth(1);
    line1->SetLineStyle(7);
    line1->Draw("same");

    // plot2->RedrawAxis();

    plots->Update();
    return plots;
}


TCanvas* plotUnfRecoComparison(TString variable, int year, TH1D *hUnf, TH1D *hReco, TH1D *hGen){

    TH1D *hXSecUnf = (TH1D*) hUnf->Clone();
    TH1D *hXSecReco = (TH1D*) hReco->Clone();
    TH1D *hXSecGen = (TH1D*) hGen->Clone();

    // Build canvas
    double maximum = hXSecUnf->GetMaximum();
    double minimum = hXSecUnf->GetMinimum();
    if (! minimum > 0) minimum = 1e-08;

    TString yearStrS;
    yearStrS.Form("%d", year);
    TString yearStr = yearStrS.Data();
    TString canvasName = "canvas_UnfRecoComparison_" + variable + "_" + yearStr;
    TCanvas *plots = new TCanvas(canvasName, variable+", "+yearStr, 600, 800);
    plots->cd();

    // Pad 1 --------------------------------------------------------------
    TPad *plot1 = new TPad("plot1", "plot1", 0., 0., 0., 0.);
    plot1->SetPad(0.01, 0.35, 0.99, 0.99);
    plot1->SetTopMargin(0.11);
    plot1->SetBottomMargin(0.005);
    plot1->SetLeftMargin(0.13);
    plot1->SetRightMargin(0.07);
    plot1->SetFillStyle(0);
    plot1->SetTicks();
    plot1->SetLogy();
    plot1->Draw();
    plot1->cd();

    // aesthetics
    hXSecReco->SetLineColor(kRed);
    hXSecReco->SetLineWidth(2);
    hXSecReco->SetMarkerColor(kRed);
    hXSecReco->SetMarkerStyle(20);

    hXSecGen->SetLineColor(kGreen);
    hXSecGen->SetLineWidth(2);
    hXSecGen->SetMarkerColor(kGreen);
    hXSecGen->SetMarkerStyle(20);

    hXSecUnf->SetLineColor(kBlack);
    hXSecUnf->SetLineWidth(2);
    hXSecUnf->SetMarkerColor(kBlack);
    hXSecUnf->SetMarkerStyle(20);

    // axes, titles
    hXSecReco->SetTitle("");
    hXSecReco->GetYaxis()->SetRangeUser(0.5*minimum, 7.*maximum);
    hXSecReco->GetYaxis()->SetTitle("Diff. Cross Section");
    hXSecReco->GetYaxis()->SetLabelSize(0.048);
    hXSecReco->GetYaxis()->SetLabelOffset(0.002);
    hXSecReco->GetYaxis()->SetTitleSize(0.051);
    hXSecReco->GetYaxis()->SetTitleOffset(1.09);
    
    // draw
    hXSecReco->Draw("E");
    hXSecGen->Draw("E same");
    hXSecUnf->Draw("E same");
 
    // legend
    TLegend *legend = new TLegend(0.7, 0.74, 0.99, 0.98);
    legend->SetFillColor(0);
    legend->SetFillStyle(1001);
    legend->SetBorderSize(1);
    legend->SetX1(0.4); //0.6
    legend->SetY1(0.7);
    legend->SetX2(0.96);
    legend->SetY2(0.98);
    legend->SetTextSize(.042);
    legend->AddEntry(hXSecReco, "Reconstructed, "+yearStr, "lpe");
    legend->AddEntry(hXSecGen, "MG5 NLO FxFx GEN, "+yearStr, "lpe");
    legend->AddEntry(hXSecUnf, "Unfolded, "+yearStr, "lpe");
    legend->Draw("same");

    // Pad 2 --------------------------------------------------------------
    plots->cd();
    TPad *plot2 = new TPad("plot2", "plot2", 0., 0., 0., 0.);
    plot2->SetPad(0.01, 0.01, 0.99, 0.35);
    plot2->SetTopMargin(0.0);
    plot2->SetBottomMargin(0.3);
    plot2->SetLeftMargin(0.13);
    plot2->SetRightMargin(0.07);
    plot2->SetFillStyle(0);
    plot2->SetTicks();
    plot2->Draw();
    plot2->cd();

    // get ratio distributions
    TH1D *ratioUnfToReco = (TH1D*) hXSecUnf->Clone();
    ratioUnfToReco->Divide(hXSecReco);
    TH1D *ratioGenToReco = (TH1D*) hXSecGen->Clone();
    ratioGenToReco->Divide(hXSecReco);

    // axes, titles
    ratioGenToReco->SetTitle("");
    ratioGenToReco->GetYaxis()->SetRangeUser(0.49, 5.2);
    ratioGenToReco->GetYaxis()->SetTitle("Ratio to Reco");
    ratioGenToReco->GetYaxis()->CenterTitle();
    ratioGenToReco->GetYaxis()->SetLabelSize(0.073); //0.09
    ratioGenToReco->GetYaxis()->SetLabelOffset(0.002); 
    ratioGenToReco->GetYaxis()->SetTitleSize(0.082); //0.104
    ratioGenToReco->GetYaxis()->SetTitleOffset(0.6); //0.48
    ratioGenToReco->GetXaxis()->SetTitleSize(0.11);
    ratioGenToReco->GetXaxis()->SetTitleOffset(1.2);
    ratioGenToReco->GetXaxis()->SetLabelSize(0.082);
    ratioGenToReco->GetXaxis()->SetLabelOffset(0.018);

    // draw
    ratioGenToReco->Draw("E");
    ratioUnfToReco->Draw("E same");

    // line
    TLine *line1 = new TLine(hXSecReco->GetXaxis()->GetXmin(), 1., hXSecReco->GetXaxis()->GetXmax(), 1.);
    line1->SetLineColor(kBlack);
    line1->SetLineWidth(1);
    line1->SetLineStyle(7);
    line1->Draw("same");

    plots->Update();
    return plots;
}