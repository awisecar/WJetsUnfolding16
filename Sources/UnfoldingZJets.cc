//-*- mode: c++; c-basic-offset: 4 -*-
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <memory>
#include <math.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TArrow.h>
#include <TLine.h>
#include <RooUnfoldBayes.h>
#include <RooUnfoldBinByBin.h>
#include <RooUnfoldSvd.h>
#include <TSVDUnfold.h>
#include <TParameter.h>
#include "fileNamesZJets.h"
#include "getFilesAndHistogramsZJets.h"
#include "variablesOfInterestZJets.h"
#include "UnfoldingZJets.h"
#include "PlotSettings.h"
#include "fixYscale.C"
#include "ConfigVJets.h"

//#define FAST_BAYES
//#define NEW_CHI2

extern ConfigVJets cfg;

const static bool isdatabug = false;

static int verbosity = 1;

using namespace std;
//void createInclusivePlots(bool doNormalized, TString outputFileName, TString lepSel, TH1D *hUnfData, TH2D *hCov[], TH1D *hMadGenCrossSection, TH1D *hSheGenCrossSection, TH1D *hPowGenCrossSection);
void createInclusivePlots(bool doNormalized, TString outputFileName, TString lepSel, TH1D *hUnfData, TH2D *hCov[], TH1D *hMadGenCrossSection, TH1D *hSheGenCrossSection);
//void createInclusivePlots(bool doNormalized, TString outputFileName, TString lepSel, TH1D *hUnfData, TH2D *hCov[], TH1D *hMadGenCrossSection);

void plotRespMat(TH2D *hResp, TString variable, TString unfoldDir);
void normalizeTH2D(TH2D *h);

//void UnfoldingZJets(TString lepSel, TString algo, TString histoDir, TString unfoldDir, 
//        int jetPtMin, int jetEtaMax, TString gen1, TString gen2, TString variable, bool doNormalized)
void UnfoldingZJets(TString lepSel, TString algo, TString histoDir, TString unfoldDir, 
		    int jetPtMin, int jetEtaMax, TString variable, bool doNormalized,
		    int whichSyst)
{
    gStyle->SetOptStat(0);
    //--- create output directory if does not exist ---
    system("mkdir -p " + unfoldDir);

    int start = 0;
    int end = NVAROFINTERESTZJETS;

    if (variable != "") {
        start = findVariable(variable);
        if (start >= 0) {
            end = start + 1;
            cout << "Processing only variable: " << variable << endl;
        }
        else {
            cerr << "\nError: variable " << variable << " is not interesting." << endl;
            cerr << "See below the list of interesting variables:" << endl;
            for (unsigned int i = 0; i < NVAROFINTERESTZJETS; ++i) {
                cerr << "\t" << i << ": " << VAROFINTERESTZJETS[i].name << "\n" << endl;
            }
            return;
        }
    }

    // Here we declare the different arrays of TFiles. 
    // fData is for the three data files: 
    // 0 - central, 1 - JES up, 2 - JES down
    TFile *fData[3] = {NULL}; 
    // fDYJets is for the five DYJets files:
    // 0 - central, 1 - PU up, 2 - PU down, 3 - JER up, 4 - JER down, 5 - LES up, 6 - LES down, 7 - LER up, 8 - LER down 
    TFile *fDYJets[9] = {NULL};
    // fBg is for the NBGDYJETS x 5 systematics files:
    // 0 - central, 1 - PU up, 2 - PU down, 3 - XSEc up, 4 - XSEC down, 5 - LES up, 6 - LES down 
    TFile *fBg[NBGDYJETS][7] = {{NULL}};

    //--- Open all files ---------------------------------------------------------------------- 
    getAllFiles(histoDir, lepSel, "13TeV", jetPtMin, jetEtaMax, fData, fDYJets, fBg, NBGDYJETS);
    //----------------------------------------------------------------------------------------- 

//    //reads integrated luminosity
//    double integratedLumi = -1;
//    TH1* Lumi = 0;
//    if(fData[0]) fData[0]->GetObject("Lumi", Lumi);
//    if(Lumi) integratedLumi = Lumi->GetBinContent(1);
//    else {
//      cerr << "Error: Lumi histogram was not found.\n";
//      return;
//    }

    //double integratedLumi = 2239.351439352; 
    double integratedLumi(35916.637); // 80X 2016 data bonzai 23Sep2016ReReco golden json - andrew - 6.3.2018
    
    cout << " DYSHERPA14FILENAME " << DYSHERPA14FILENAME << endl;
    cout << " DYMGPYTHIA8FILENAME " << DYMGPYTHIA8FILENAME << endl;
    TFile *fSheUnf = 0;
    TFile *fGen1 = NULL;
    TString gen1;
    TString gen1File;
    
    TFile *fGen2 = NULL;
    TString gen2;
    TString gen2File;

    std::map<TString, vector<TString> > generatorNames;

    //--- Use this Flag for the 2nd Theory -----------------------
    if (DYMGPYTHIA8FILENAME.Length() > 0 ){
        vector<TString> mgpythia8;
        mgpythia8.push_back(DYMGPYTHIA8FILENAME);
        mgpythia8.push_back(DYMGPYTHIA8LEGEND);
        
        generatorNames["mgpythia8"] = mgpythia8;
        gen1 = "mgpythia8";
        
        gen1File = histoDir + lepSel + "_13TeV_" + generatorNames[gen1][0] + "_dR_TrigCorr_1_Syst_0_JetPtMin_";
        gen1File += jetPtMin;
        gen1File += "_JetEtaMax_";
        gen1File += jetEtaMax;
        gen1File += ".root";
        fGen1 = new TFile(gen1File);
    }
    
    //--- Use this Flag for the 3rd Theory -----------------------
    if (NNLO1JFILENAME.Length() > 0 ){
        vector<TString> nnlo1j;
        nnlo1j.push_back(NNLO1JFILENAME);
        nnlo1j.push_back(NNLO1JLEGEND);
        
        generatorNames["nnlo1j"] = nnlo1j;
        gen2 = "nnlo1j";
        
        gen2File = histoDir + lepSel + "_13TeV_" + generatorNames[gen2][0] + ".root";
        fGen2 = new TFile(gen2File);
    }  //add nnlo here
    //--- Use this Flag for Resp Syst (sherpa unfolding in 8TeV) ---
    if (DYSHERPA14FILENAME.Length() > 0 ){
	//--- Open additional generator files -----------------------------------------------------
	fSheUnf = new TFile(histoDir + lepSel + "_13TeV_" + DYSHERPA14FILENAME + "_dR_TrigCorr_0_Syst_0_JetPtMin_30_JetEtaMax_24.root");

	//std::map<TString, vector<TString> > generatorNames;
      
	vector<TString> sherpa14;
	sherpa14.push_back(DYSHERPA14FILENAME);
	sherpa14.push_back(DYSHERPA14LEGEND);
	//vector<TString> sherpa2;
	//sherpa2.push_back(DYSHERPA2FILENAME);
	//sherpa2.push_back(DYSHERPA2LEGEND);
	//vector<TString> amcatnlo;
	//amcatnlo.push_back(DYAMCATNLOFILENAME);
	//amcatnlo.push_back(DYAMCATNLOLEGEND);
      
	generatorNames["sherpa14"] = sherpa14;
	//generatorNames["sherpa2"] = sherpa2;
	//generatorNames["amcatnlo"] = amcatnlo; 

    }      
    //----------------------------------------------------------------------------------------- 

    //----------------------------------------------------------------------------------------- 
    //--- Now run on the different variables ---
    for (int i = start; i < end; ++i) {
      variable = VAROFINTERESTZJETS[i].name;
      
      TString outputFileName = unfoldDir + lepSel; 
      outputFileName += "_unfolded_" + variable + "_" + algo;
      outputFileName += "_JetPtMin_";
      outputFileName += jetPtMin;
      outputFileName += "_JetEtaMax_";
      outputFileName += jetEtaMax;
      //outputFileName += "_MGPYTHIA6_" + gen1 + "_" + gen2;
      outputFileName += "_MGPYTHIA6_";
      outputFileName += doNormalized ? "_normalized" : "";
	
      TFile *outputRootFile = new TFile(outputFileName + ".root", "RECREATE");
	
	
      //--- rec Data histograms ---
      TH1D *hRecData[3] = {NULL};
      TH1D* hRecDataOdd = 0;
      TH1D* hRecDataEven = 0;
      
      //--- rec DYJets histograms ---
      TH1D *hRecDYJets[13] = {NULL};
      //--- fake DYJets histograms ---
      TH1D *hFakDYJets[18] = {NULL};
      //--- purity "histograms" ---
      TH1D *hPurity[18] = {NULL};
      //--- gen DYJets histograms ---
      TH1D *hGenDYJets[11] = {NULL};
      //--- res DYJets histograms ---
      TH2D *hResDYJets[13] = {NULL};
      //--- rec Bg histograms ---
      TH1D *hRecBg[NBGDYJETS][11] = {{NULL}};
      //--- rec Sum Bg histograms ---
      TH1D *hRecSumBg[11] = {NULL};
      //--- response DYJets objects ---
      RooUnfoldResponse *respDYJets[18] = {NULL};

      //Get split histogram of data used for cross-validation
      TString variable_odd = variable + "_Odd";
      TString variable_even = variable + "_Even";

      if(fData[0]){
	fData[0]->cd();
	hRecDataOdd = (TH1D*) fData[0]->Get(variable_odd);
	hRecDataEven = (TH1D*) fData[0]->Get(variable_even);
      }
      
      //--- Get all histograms ---
      std::cerr << __FILE__ << ":"  << __LINE__ << ". " << variable << "\n";
      getAllHistos(variable, hRecData, fData, 
		   hRecDYJets, hGenDYJets, hResDYJets, fDYJets,
		   hRecBg, hRecSumBg, fBg, NBGDYJETS, respDYJets, hFakDYJets, hPurity);
        
            
//        //--- Apichart : This ccalculation does not work, but keep the code for futher development--
//        //----------- for calculating resp syst --------
//        //--- rec Data histograms ---
//        TH1D *Fine_hRecData[3] = {NULL};
//        //--- rec DYJets histograms ---
//        TH1D *Fine_hRecDYJets[13] = {NULL};
//        //--- fake DYJets histograms ---
//        TH1D *Fine_hFakDYJets[18] = {NULL};
//        //--- purity "histograms" ---
//        TH1D *Fine_hPurity[18] = {NULL};
//        //--- gen DYJets histograms ---
//        TH1D *Fine_hGenDYJets[11] = {NULL};
//        //--- res DYJets histograms ---
//        TH2D *Fine_hResDYJets[13] = {NULL};
//        //--- rec Bg histograms ---
//        TH1D *Fine_hRecBg[NBGDYJETS][11] = {{NULL}};
//        //--- rec Sum Bg histograms ---
//        TH1D *Fine_hRecSumBg[11] = {NULL};
//        //--- response DYJets objects ---
//        RooUnfoldResponse *Fine_respDYJets[18] = {NULL};
//        //===================================================
        TH1D *hGen1 = NULL;
        TH1D *hGen2 = NULL;
        if (DYMGPYTHIA8FILENAME.Length() > 0 ){
            hGen1 = getHisto(fGen1, "gen" + variable);
        }
        if (NNLO1JFILENAME.Length() > 0 ){
        if (variable == "FirstJetPt_Zinc1jet" || variable == "JetsHT_Zinc1jet" || variable == "FirstJetAbsRapidity_Zinc1jet" || variable == "dRptmin100LepCloseJetCo300dR04_Zinc1jet" || variable == "dPhiLepJet1_Zinc1jet"){
            //--- Additional (3rd) theory comparison ---
            hGen2 = getHisto(fGen2, "gen" + variable);
            }
        }  //add nnlo here
        if (DYSHERPA14FILENAME.Length() > 0 ){
          //--- Get Sherpa Unfolding response (Resp Syst)---
          respDYJets[17] = getResp(fSheUnf, variable);
            
          
               
//        //--- Apichart : This ccalculation does not work, but keep the code for futher development--
//        //----------- for calculating resp syst --------  
//          TString variableStd = variable;
//          TString variableFine = variable;
//          variableFine.Replace(variable.Index("_"), 1,"_2_");
//          if (variable == "ZNGoodJets_Zexc") variableFine = variable;
//          cout << " " << variableStd << " " << variableFine << endl;
//          
//          getAllHistos(variableFine, Fine_hRecData, fData,
//                       Fine_hRecDYJets, Fine_hGenDYJets, Fine_hResDYJets, fDYJets,
//                       Fine_hRecBg, Fine_hRecSumBg, fBg, NBGDYJETS, Fine_respDYJets, Fine_hFakDYJets, Fine_hPurity);
//          
//          TH1D *measSubBG = (TH1D*) Fine_hRecData[0]->Clone();
//          measSubBG->Add(Fine_hRecSumBg[0], -1);
//          
//          //--- Bins definition : nominal binning ---
//          const int nBinsStd(hRecData[0]->GetNbinsX());
//          double BinsArray[nBinsStd+1];
//          for (int i = 0; i <= nBinsStd; i++){
//              if (i == 0) BinsArray[i] = hRecData[0]->GetXaxis()->GetBinLowEdge(i+1);
//              else BinsArray[i] = hRecData[0]->GetXaxis()->GetBinUpEdge(i);
//          }
//          
//          cout << " Number of bin used = " << measSubBG->GetNbinsX()  << "   "  << nBinsStd << endl;
//
//          
//          TH2D *hResponse      = (TH2D*) Fine_hResDYJets[0]->Clone();
//          TH1D *histoMCtruth   = (TH1D*) Fine_hGenDYJets[0]->Clone();
//          TH1D *histoMCreco    = (TH1D*) Fine_hRecDYJets[0]->Clone();
//          
//          cout << " integral " << measSubBG->Integral() << " " << histoMCtruth->Integral() << " " << histoMCreco->Integral() << " " << hResponse->Integral() << endl;
//          
//          //--- calculate scaling factor
//          const int nBinsFine(histoMCreco->GetNbinsX());
//          double fwFirst[nBinsFine];
//          cout << "----------- calculate scaling factor fw -------------" << endl;
//          for (int i = 1; i <= nBinsFine; i++){
//              fwFirst[i] = 1;
//              if ((histoMCreco->GetBinContent(i) > 0) & (measSubBG->Integral() > 0)){
//                  fwFirst[i] = (measSubBG->GetBinContent(i) / histoMCreco->GetBinContent(i)) * (histoMCreco->Integral() / measSubBG->Integral());
//              }
//              if (fwFirst[i] < 0)fwFirst[i] =1;
//              cout << fwFirst[i] << endl;
//          }
//
//          double TruthToProjectY[nBinsFine];
//          for (int j = 1; j <= hResponse->GetYaxis()->GetNbins(); j++){
//              TruthToProjectY[j] = 1;
//              if(hResponse->ProjectionY()->GetBinContent(j) > 0){
//                  TruthToProjectY[j]  = (histoMCtruth->GetBinContent(j) / hResponse->ProjectionY()->GetBinContent(j));
//              }
//              cout << " TruthToProjectY   " << TruthToProjectY[j] << endl;
//          }
//
//          
//          TH2D *hResponseTemp = (TH2D*) hResponse->Clone();
//          TH1D *MCtruthTemp = (TH1D*) histoMCtruth->Clone();
//          TH1D *MCrecoTemp = (TH1D*) histoMCreco->Clone();
//          //--- hResponseTemp
//          for (int j = 1; j <= nBinsFine; j++){
//              for (int i = 1; i <= nBinsFine; i++){
//                  hResponseTemp->SetBinContent(i, j, fwFirst[i] * hResponse->GetBinContent(i, j));
//              }
//          }
//          //--- MCtruthTemp, MCrecoTemp
//          for (int j = 1; j <= nBinsFine; j++){
//              MCtruthTemp->SetBinContent(j, TruthToProjectY[j] * hResponseTemp->ProjectionY()->GetBinContent(j));
//              MCrecoTemp->SetBinContent(j, fwFirst[j] * histoMCreco->GetBinContent(j));
//          }
//
//
//          //--- Produce the response and unfolded with nominal binning.
//          //--- Note that measSubBG should be taken from filling the nominal binning.
//          //--- Only response-relevant should be produced here.
//          TH1D *MCtruthTempNominal    = (TH1D*) MCtruthTemp ->Rebin(nBinsStd,"MCtruthNorminal",   BinsArray);
//          TH1D *MCrecoTempNominal     = (TH1D*) MCrecoTemp  ->Rebin(nBinsStd,"MCrecoNorminal",    BinsArray);
//          //--- Rebin 2D response matrix
//          TH2D *hResponseTempNominal = new TH2D("responseNorminal", hResponse->GetTitle(), nBinsStd, BinsArray, nBinsStd, BinsArray);
//          for (int j = 1; j <= hResponseTemp->GetYaxis()->GetNbins(); j++){
//              for (int i = 1; i <= hResponseTemp->GetXaxis()->GetNbins(); i++){
//                  hResponseTempNominal->Fill(hResponseTemp->GetXaxis()->GetBinCenter(i), hResponseTemp->GetYaxis()->GetBinCenter(j), hResponseTemp->GetBinContent(i,j));
//              }
//          }
//          
//          //---- print out ----
//          for (int j = 1; j <= hResponseTempNominal->GetYaxis()->GetNbins(); j++){
//              for (int i = 1; i <= hResponseTempNominal->GetXaxis()->GetNbins(); i++){
//                  
//                  if (hResDYJets[0]->GetBinContent(i,j) > 0){
//                      hResponseTempNominal->SetBinError(i,j, (hResponseTempNominal->GetBinContent(i,j)*hResDYJets[0]->GetBinError(i,j))/hResDYJets[0]->GetBinContent(i,j));
//                  }
//                  else{
//                      hResponseTempNominal->SetBinError(i,j, 0.);
//                  }
//                  
//                  
//                  cout << "i " << i << " j " << j << " " << hResponseTempNominal->GetBinContent(i,j) << " " <<  hResDYJets[0]->GetBinContent(i,j) << "   " << hResponseTempNominal->GetBinError(i,j) << " " <<  hResDYJets[0]->GetBinError(i,j) << endl;
//              }
//          }
//          
//          MCrecoTempNominal->Reset();
//
//          RooUnfoldResponse *responseReweightedNominal = new RooUnfoldResponse (MCrecoTempNominal, MCtruthTempNominal, hResponseTempNominal);
//          responseReweightedNominal->UseOverflow();
//
//          //TH2D * tepmResp = getResp(fSheUnf, variable);
//          //respDYJets[17] = getResp(fSheUnf, variable);
//          respDYJets[17] = (RooUnfoldResponse*) responseReweightedNominal->Clone();
//          //-----------------------------------------------------------------------------------------
//          //---End: for calculating resp syst -------------------------------------------------------
      }

      //andrew -- write out gen distributions before lumi scaling to get num of entries
      outputRootFile->cd();
      hGenDYJets[0]->Write("hMadGenDYJetsCrossSection_NoScale");
      hGen1->Write("hGen1DYJetsCrossSection_NoScale");

      TH1D *hMadGenCrossSection = makeCrossSectionHist(hGenDYJets[0], integratedLumi);
      //andrew -- need to double check that this is pulling from the wjets source MC that's listed in the Samples structure in fileNamesZJets.h
      hMadGenCrossSection->SetZTitle("MG_aMC FxFx + PY8 (#leq 2j NLO + PS)");
    TH1D *hGen1CrossSection = makeCrossSectionHist(hGen1, integratedLumi);
      //andrew -- this should be the wjets signal mc mlm
      hGen1CrossSection->SetZTitle(generatorNames[gen1][1]);
       TH1D *hGen2CrossSection;
        if (variable == "FirstJetPt_Zinc1jet" || variable == "JetsHT_Zinc1jet" || variable == "FirstJetAbsRapidity_Zinc1jet" || variable == "dRptmin100LepCloseJetCo300dR04_Zinc1jet" || variable == "dPhiLepJet1_Zinc1jet"){
            hGen2CrossSection = makeCrossSectionHist(hGen2, 0);
            hGen2CrossSection->SetZTitle(generatorNames[gen2][1]);
        }  //add nnlo here
//        TH1D *genNNLO;
//        vector<double> myVecNNLO;
//        myVecNNLO = getNNLO1jxsec(variable);
//        genNNLO = (TH1D*) hGenDYJets[0]->Clone();
//        for(int i = 1; i<= hGenDYJets[0]->GetNbinsX(); i++){
//            genNNLO->SetBinContent(i, myVecNNLO.at(i-1));
//            //cout << " genShe01 " << genNNLO->GetBinContent(i) << endl;
//            genNNLO->SetBinError  (i, 0.000000000000000000001);
//        }
//        genNNLO->SetZTitle("NNLO1j ");
        
//        TH1D *genNLO2;
//        vector<double> myVecNLO2;
//        myVecNLO2 = getNLO1jxsec(variable);
//        genNLO2 = (TH1D*) hGenDYJets[0]->Clone();
//        for(int i = 1; i<= hGenDYJets[0]->GetNbinsX(); i++){
//            genNLO2->SetBinContent(i, myVecNLO2.at(i-1));
//            cout << " genNLO2 " << genNLO2->GetBinContent(i) << endl;
//            genNLO2->SetBinError  (i, 0.000000000000000000001);
//        }
//        genNLO2->SetZTitle("NLO1j ");
        
        
      //if (gen1 == "sherpa2" && lepSel == "DMu") hGen1CrossSection->Scale(1./3.9731e+09);
      //if (gen1 == "sherpa2" && lepSel == "DE") hGen1CrossSection->Scale(1./3.9731e+09); 
      //if (gen2 == "sherpa2" && lepSel == "DMu") hGen2CrossSection->Scale(1./3.9731e+09); 
      //if (gen2 == "sherpa2" && lepSel == "DE") hGen2CrossSection->Scale(1./3.9731e+09); 
      // this is for MLM
      //if (lepSel == "DE")  hPowGenCrossSection->Scale(9.68984524447932094e+06/1.26965652647094554e+18); 
      //if (lepSel == "DMu") hSheGenCrossSection->Scale(9.67682344591264986e+06/1.56860892310320000e+14); 
      //if (gen1 == "amcatnlo" && lepSel == "DMu") hGen1CrossSection->Scale(9.67682344591264986e+06/1.93588665458e+18); 
      //if (gen1 == "amcatnlo" && lepSel == "DE")  hGen1CrossSection->Scale(9.68984524447932094e+06/2.01906738616e+18); 
      //if (gen2 == "amcatnlo" && lepSel == "DMu") hGen2CrossSection->Scale(9.67682344591264986e+06/1.93588665458e+18); 
      //if (gen2 == "amcatnlo" && lepSel == "DE")  hGen2CrossSection->Scale(9.68984524447932094e+06/2.01906738616e+18); 

      // Here is an array of TH1D to store the various unfolded data:
      // 0 - Central, 
      // 1 - JES up, 2 - JES down, 
      // 3 - PU up, 4 - PU down, 
      // 5 - JER up, 6 - JER down, 
      // 7 - XSEC up, 8 - XSEC down
      // 9 - LES up, 10 - LES down
      // 11 - LER up, 12 -LER down
      // 13 - Lumi up, 14 - Lumi down
      // 15 - SF up, 16 - SF down
      // 17 - SherpaUnf
      TString name[] = {"Central", "JESUp", "JESDown", "PUUp", "PUDown", "JERUp", "JERDown", 
			"XSECUp", "XSECDown", "LESUp", "LESDown", "LERUp", "LERDown",
			"LumiUp", "LumiDown", "SFUp", "SFDown", "SherpaUnf"};
      TH1D *hUnfData[18] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,};
      TH1D *hUnfDataNoScale[18] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,};
      TH2D *hUnfDataStatCov[18] =  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,};
      TH2D *hUnfDataStatCovNoScale[18] =  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,};
      TH2D *hUnfMCStatCov[18] =  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,};
      TH2D *hUnfMCStatCovNoScale[18] =  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,};
      
        
      int nIter[18] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; 
      int svdKterm(0);
      //if (lepSel == "DMu")
      if (lepSel == "DMu" || lepSel == "SMu")
	svdKterm = VAROFINTERESTZJETS[i].MuSVDkterm;
      else if (lepSel == "DE")   
	svdKterm = VAROFINTERESTZJETS[i].ESVDkterm;
      else {
	cerr << "Error: algo " << algo << " or lepSel " << lepSel << "invalid\n"; 
	cerr << "Aborting...\n";
	return;
      }
      bool logy = VAROFINTERESTZJETS[i].log;

      int nSysts = DYSHERPA14FILENAME.Length()? 18 : 17;
      //int nSysts = DYSHERPA14FILENAME.Length()? 17 : 17;
      //--- Unfold the Data histograms for each systematic ---
      for (unsigned short iSyst = 0; iSyst < nSysts; ++iSyst) {

	if(iSyst != 0 && whichSyst >= 0 && iSyst != whichSyst) continue;
	//--- only JES up and down (iSyst = 1 and 2) is applied on data ---
	unsigned short iData = (iSyst == 1 || iSyst == 2) ? iSyst : 0;
	unsigned short iBg = 0;
	if (iSyst == 0 || iSyst == 1 || iSyst == 2 || iSyst == 5 || iSyst == 6 || iSyst == 11 || iSyst == 12 || iSyst == 17) iBg = 0; // Central, JES, JER, LER, Sherpa
	else if (iSyst == 3 || iSyst == 4) iBg = iSyst - 2; // PU
	else if (iSyst == 7 || iSyst == 8 || iSyst == 9 || iSyst == 10) iBg = iSyst - 4; // XSec, LES
	else if (iSyst == 13 || iSyst == 14 || iSyst == 15 || iSyst == 16) iBg = iSyst - 6;  // Lumi, SF

	TH1D *hRecDataMinusFakes = (TH1D*) hRecData[iData]->Clone();
	if(hRecDataMinusFakes->GetBinContent(1) > 0){
	  std::cout <<  __FILE__ << __LINE__ << ": "
		    << "hRec: " << hRecDataMinusFakes->GetName()
		    <<  " " << hRecDataMinusFakes->GetBinError(1)
	    / sqrt(hRecDataMinusFakes->GetBinContent(1)) << "\n";
	}
//	std::cerr << "DEBUG: hRecData[" << iData << "]->GetEntries() = "
//		  << hRecDataMinusFakes->GetEntries()
//		  << ", nbins: " << hRecDataMinusFakes->GetNbinsX()
//		  <<"\n"
//		  << "DEBUG: hRecSumBg[" << iData << "]->GetEntries() = "
//		  << hRecSumBg[iData]->GetEntries()
//		  << ", nbins: " << hRecSumBg[iData]->GetNbinsX()
//		  <<"\n"
//		  << "DEBUG: hPurity[" << iData << "]->GetEntries() = "
//		  << hPurity[iData]->GetEntries()
//		  << ", nbins: " << hPurity[iData]->GetNbinsX()
//		  <<"\n"
//		  << "hRecDYJets[iData]->GetNbins() = " << hRecDYJets[iData]->GetNbinsX()
//		  << std::endl;
	
	hRecDataMinusFakes->Add(hRecSumBg[iBg], -1);
	if(hRecDataMinusFakes->GetBinContent(1) > 0){
	  std::cout <<  __FILE__ << __LINE__ << ": "
		    << "hRecMinusBg: " << hRecDataMinusFakes->GetName()
		    << " " <<  hRecDataMinusFakes->GetBinError(1)
	    / sqrt(hRecDataMinusFakes->GetBinContent(1)) << "\n";
	}

	//Applies purity corrections (aka fake corrections):
	RemoveFakes(hRecDataMinusFakes, hFakDYJets[iSyst], hPurity[iSyst]);

	//FIXME: use independent samples for subtraction
	TH1D *hRecDataMinusFakesOdd;
	TH1D *hRecDataMinusFakesEven;
	if(iData==0 && hRecDataOdd && hRecDataEven){
	  hRecDataMinusFakesOdd = (TH1D*) hRecDataOdd->Clone();
	  hRecDataMinusFakesOdd->Add(hRecSumBg[iBg], -0.5);
	  RemoveFakes(hRecDataMinusFakesOdd, hFakDYJets[iSyst], hPurity[iSyst]);
      hRecDataMinusFakesEven = (TH1D*) hRecDataEven->Clone();
	  hRecDataMinusFakesEven->Add(hRecSumBg[iBg], -0.5);
	  RemoveFakes(hRecDataMinusFakesEven, hFakDYJets[iSyst], hPurity[iSyst]);
	} else{
	  hRecDataMinusFakesEven = hRecDataMinusFakesOdd = 0;
	}
	if(hRecDataMinusFakes->GetBinContent(1) > 0){
	  std::cout <<  __FILE__ << __LINE__ << ": "
		    << "hRecMinusBgMinusFake: " << hRecDataMinusFakes->GetName()
		    << " " << hRecDataMinusFakes->GetBinError(1)
	    / sqrt(hRecDataMinusFakes->GetBinContent(1))
		    << "\t" << hRecDataMinusFakes->GetBinContent(1) /  hRecData[iData]->GetBinContent(1)
	    	    << "\t" << hRecDataMinusFakes->GetBinError(3)
	    / sqrt(hRecDataMinusFakes->GetBinContent(3))
		    << "\t" << hRecDataMinusFakes->GetBinContent(3) /  hRecData[iData]->GetBinContent(3)
		    << "\n";
	}

	
	if (iSyst == 17) cout << "SHERPAUNFOLDING" << endl;
	std::cout << "Starting unfolding of " << variable << " "
		  << name[iSyst] << " for  "<< lepSel << " channel." << "\n";
	if(hRecDataMinusFakes->GetEntries() == 0){
	  std::cerr << "Warning: histogram " << hRecDataMinusFakes->GetName()
		    << " has no entries. Its unfolding will be skipped.\n";
	  continue;
	}
	nIter[iSyst] = UnfoldData(lepSel, algo, svdKterm, respDYJets[iSyst], hRecDataMinusFakes, hUnfData[iSyst], hUnfDataNoScale[iSyst],
				  hUnfDataStatCov[iSyst], hUnfDataStatCovNoScale[iSyst], hUnfMCStatCov[iSyst], hUnfMCStatCovNoScale[iSyst], name[iSyst], integratedLumi, logy,
				  hRecDataMinusFakesOdd, hRecDataMinusFakesEven);

	//--- save the unfolded histograms ---
	//outputRootFile->cd();
	//if(hUnfData[iSyst]) hUnfData[iSyst]->Write();
	
        //andrew
        //save the hRecDataMinusFakes i.e. detector level histograms for building correlation matrix,
        //before loop goes to next iSyst
        //here we grab the "central" detec.-level histo
        outputRootFile->cd();
        if (hRecDataMinusFakes && iSyst == 0){
        TString detecName = "hDetecData"+name[iSyst];
        TString detecTitle = variable;
        detecTitle += "_detectorlevel";
        hRecDataMinusFakes->SetNameTitle(detecName, detecTitle);
        hRecDataMinusFakes->Write();
        }
    }
      //-----------------------------------------------------------------------------------------
        
      if (doNormalized) {
	for(int i = 0; i < nSysts; i++)
	  {
	    if(!hUnfData[i]) continue;

	    double totUnfData = hUnfData[i]->Integral("width"); // normalize to central or itself? 
	    hUnfData[i]->Scale(1.0/totUnfData);
	    if (i == 0) {
	      hUnfDataStatCov[0]->Scale(1.0/pow(totUnfData, 2));
	      hUnfMCStatCov[0]->Scale(1.0/pow(totUnfData, 2));
	    }
 
            //the "NoScale" here refers to not scaling by lumi and bin width for xsec
            //we normalize the dist. the same here as with the original ones, however
            //original reson for NoScale dist. is to do ratio calculations, and we need integral number of counts in bins
	    double totUnfDataNoScale = hUnfDataNoScale[i]->Integral("width");
	    hUnfDataNoScale[i]->Scale(1.0/totUnfDataNoScale);
	    if (i == 0) {
	      hUnfDataStatCovNoScale[0]->Scale(1.0/pow(totUnfDataNoScale, 2));
	      hUnfMCStatCovNoScale[0]->Scale(1.0/pow(totUnfDataNoScale, 2));
	    }

	  }
      }
        
      //--- Now create the covariance matrices ---
      int nCovs = nSysts > 17 ? 11 : 10;
        
      TH2D *hCov[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,};
      if(hUnfDataStatCov[0]) hCov[0] = (TH2D*) hUnfDataStatCov[0]->Clone("CovDataStat");
      if(hUnfMCStatCov[0]) hCov[1] = (TH2D*) hUnfMCStatCov[0]->Clone("CovMCStat");
      if(hUnfData[1])  hCov[2] = makeCovFromUpAndDown(hUnfData[0], hUnfData[1], hUnfData[2], "CovJES");
      if(hUnfData[3])  hCov[3] = makeCovFromUpAndDown(hUnfData[0], hUnfData[3], hUnfData[4], "CovPU");
      if(hUnfData[5])  hCov[4] = makeCovFromUpAndDown(hUnfData[0], hUnfData[5], hUnfData[6], "CovJER");
      if(hUnfData[7])  hCov[5] = makeCovFromUpAndDown(hUnfData[0], hUnfData[7], hUnfData[8], "CovXSec");
      if(hUnfData[9])  hCov[6] = makeCovFromUpAndDown(hUnfData[0], hUnfData[9], hUnfData[10], "CovLES");
      if(hUnfData[11]) hCov[7] = makeCovFromUpAndDown(hUnfData[0], hUnfData[11], hUnfData[12], "CovLER");
      if(hUnfData[13]) hCov[8] = makeCovFromUpAndDown(hUnfData[0], hUnfData[13], hUnfData[14], "CovLumi");
      if(hUnfData[15]) hCov[9] = makeCovFromUpAndDown(hUnfData[0], hUnfData[15], hUnfData[16], "CovSF");
      if(hUnfData[17]) hCov[10] = makeCovFromUpAndDown(hUnfData[0], hUnfData[17], hUnfData[0], "CovSherpaUnf");
      if(hUnfMCStatCov[0]) hCov[11] = (TH2D*) hUnfMCStatCov[0]->Clone("CovTotSyst");
      
      if(hCov[11]){
	      for (int i = 2; i < nCovs; ++i){
	          if(hCov[i]) hCov[11]->Add(hCov[i]);
          }
      }
        
        //andrew
        //Create covariance matrices not scaled for cross section
       TH2D *hCovNoScale[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,};
       if(hUnfDataStatCovNoScale[0]) hCovNoScale[0] = (TH2D*) hUnfDataStatCovNoScale[0]->Clone("CovDataStat_NoScale");
       if(hUnfMCStatCovNoScale[0]) hCovNoScale[1] = (TH2D*) hUnfMCStatCovNoScale[0]->Clone("CovMCStat_NoScale");
       if(hUnfDataNoScale[1])  hCovNoScale[2] = makeCovFromUpAndDown(hUnfData[0], hUnfDataNoScale[1], hUnfDataNoScale[2], "CovJES_NoScale");
       if(hUnfDataNoScale[3])  hCovNoScale[3] = makeCovFromUpAndDown(hUnfData[0], hUnfDataNoScale[3], hUnfDataNoScale[4], "CovPU_NoScale");
       if(hUnfDataNoScale[5])  hCovNoScale[4] = makeCovFromUpAndDown(hUnfData[0], hUnfDataNoScale[5], hUnfDataNoScale[6], "CovJER_NoScale");
       if(hUnfDataNoScale[7])  hCovNoScale[5] = makeCovFromUpAndDown(hUnfData[0], hUnfDataNoScale[7], hUnfDataNoScale[8], "CovXSec_NoScale");
       if(hUnfDataNoScale[9])  hCovNoScale[6] = makeCovFromUpAndDown(hUnfData[0], hUnfDataNoScale[9], hUnfDataNoScale[10], "CovLES_NoScale");
       if(hUnfDataNoScale[11]) hCovNoScale[7] = makeCovFromUpAndDown(hUnfData[0], hUnfDataNoScale[11], hUnfDataNoScale[12], "CovLER_NoScale");
       if(hUnfDataNoScale[13]) hCovNoScale[8] = makeCovFromUpAndDown(hUnfData[0], hUnfDataNoScale[13], hUnfDataNoScale[14], "CovLumi_NoScale");
       if(hUnfDataNoScale[15]) hCovNoScale[9] = makeCovFromUpAndDown(hUnfData[0], hUnfDataNoScale[15], hUnfDataNoScale[16], "CovSF_NoScale");
       if(hUnfDataNoScale[17]) hCovNoScale[10] = makeCovFromUpAndDown(hUnfData[0], hUnfDataNoScale[17], hUnfDataNoScale[0], "CovSherpaUnf_NoScale");
       if(hUnfMCStatCovNoScale[0]) hCovNoScale[11] = (TH2D*) hUnfMCStatCovNoScale[0]->Clone("CovTotSyst_NoScale");
          
       if(hCovNoScale[11]){
           for (int i = 2; i < nCovs; ++i){
               if(hCovNoScale[i]) hCovNoScale[11]->Add(hCovNoScale[i]);
           }
       }
    

      if (doNormalized) {
	double Madtot = hMadGenCrossSection->Integral("width");
	//double gen1tot = hGen1CrossSection->Integral("width");
	//double gen2tot = hGen2CrossSection->Integral("width");
	hMadGenCrossSection->Scale(1.0/Madtot);
	//hGen1CrossSection->Scale(1.0/gen1tot);	
	//hGen2CrossSection->Scale(1.0/gen2tot);
      }

      
      //TCanvas *crossSectionPlot = makeCrossSectionPlot(lepSel, variable, doNormalized, hUnfData[0], hCov[11], hMadGenCrossSection, hGen1CrossSection, hGen2CrossSection);

      //TCanvas *crossSectionPlot = makeCrossSectionPlot(lepSel, variable, doNormalized, hUnfData[0], hCov[11], hMadGenCrossSection, hGen1CrossSection);  
        TCanvas *crossSectionPlot;
        if (variable == "FirstJetPt_Zinc1jet" || variable == "JetsHT_Zinc1jet" || variable == "FirstJetAbsRapidity_Zinc1jet" || variable == "dRptmin100LepCloseJetCo300dR04_Zinc1jet" || variable == "dPhiLepJet1_Zinc1jet"){
            crossSectionPlot = makeCrossSectionPlot(lepSel, variable, doNormalized, hUnfData[0], hCov[11], hMadGenCrossSection, hGen1CrossSection, hGen2CrossSection);
        }
        else {
            crossSectionPlot = makeCrossSectionPlot(lepSel, variable, doNormalized, hUnfData[0], hCov[11], hMadGenCrossSection, hGen1CrossSection);
        }  //add nnlo here

      //TCanvas *crossSectionPlot = makeCrossSectionPlot(lepSel, variable, doNormalized, hUnfData[0], hCov[11], hMadGenCrossSection); 
      crossSectionPlot->Draw();
      crossSectionPlot->SaveAs(outputFileName + ".png");
      crossSectionPlot->SaveAs(outputFileName + ".pdf");
      crossSectionPlot->SaveAs(outputFileName + ".eps");
      crossSectionPlot->SaveAs(outputFileName + ".ps");
      crossSectionPlot->SaveAs(outputFileName + ".C");
      crossSectionPlot->SaveAs(outputFileName + "_canvas.root");

      plotRespMat(hResDYJets[0], variable, unfoldDir);
        
      if(whichSyst < 0){
	  createSystPlots(outputFileName, variable, lepSel, hUnfData, logy);
      //--- print out break down of errors ---
	  for (int i = 2; i <= nCovs; ++i) {
	  cout << hUnfData[0]->GetBinContent(i);
	  for (int j = 0; j <= 11; ++j) {
	    if(hCov[j]){
	      cout << " +/- " << sqrt(hCov[j]->GetBinContent(i,i))*100./hUnfData[0]->GetBinContent(i) << "%";
	    }
	}
	  cout << endl;
	}

	createTable(outputFileName, lepSel, variable, doNormalized, hUnfData[0], hCov);
      }
      
      
      if (variable.Index("ZNGoodJets_Zexc") >= 0 || variable.Index("ZNGoodJetsFull_Zexc") >= 0) {
      //createInclusivePlots(doNormalized, outputFileName, lepSel, hUnfData[0], hCov, hMadGenCrossSection, hGen1CrossSection, hGen2CrossSection);
        createInclusivePlots(doNormalized, outputFileName, lepSel, hUnfData[0], hCov, hMadGenCrossSection, hGen1CrossSection);
      //createInclusivePlots(doNormalized, outputFileName, lepSel, hUnfData[0], hCov, hMadGenCrossSection);
      }

      //--------------------------------------

      //--- Save other things --- 
      outputRootFile->cd();

      //andrew - reco data before BG subtraction and fakes removal
      hRecData[0]->Write("hRecDataCentral");
      //andrew - sum of all BGs to be subtracted from reco data
      hRecSumBg[0]->Write("hRecSumBgCentral");
      hRecDYJets[0]->Write("hRecDYJetsCentral");
      hGenDYJets[0]->Write("hGenDYJetsCentral");
      hMadGenCrossSection->Write("hMadGenDYJetsCrossSection");
      hGen1CrossSection->Write("hGen1DYJetsCrossSection");
      //hGen2CrossSection->Write("hGen2DYJetsCrossSection");
      respDYJets[0]->Write("respDYJetsCentral");
      //andrew - hResDYJets is the TH2 used to form response objects respDYJets
      hResDYJets[0]->Write("hResDYJetsCentral");

      //write out unfolded histograms
      for (unsigned short iSyst = 0; iSyst < nSysts; ++iSyst) {
          outputRootFile->cd();
          if(hUnfData[iSyst]) hUnfData[iSyst]->Write();
          if(hUnfDataNoScale[iSyst]) hUnfDataNoScale[iSyst]->Write();
      }
        
      //write out covariance matrices (includes stat. for central (data and MC) and systematics from "up-down" dist.)
      for (int i = 0; i <= 11; ++i) {
          if(hCov[i]) hCov[i]->Write();
          if(hCovNoScale[i]) hCovNoScale[i]->Write();
      }
        
      TH1D *h_TotalUncer = (TH1D*) hUnfData[0]->Clone();
      for (int i = 1; i <= hUnfData[0]->GetNbinsX(); ++i) {
          h_TotalUncer->SetBinContent(i, sqrt(hCov[0]->GetBinContent(i,i) + hCov[11]->GetBinContent(i,i)));
          h_TotalUncer->SetBinError(i, 0.);
      }
      h_TotalUncer->Write("totalUncertainty");
        
        
      TParameter<double> pIntegratedLumi("integratedLumi", integratedLumi);
      pIntegratedLumi.Write();
      TParameter<int> pNIter("nIter", nIter[0]);
      pNIter.Write();

      //crossSectionPlot is the canvas showing the main/central unfolded dist. (hUnfData[0])
      crossSectionPlot->Write();
      //----------------------------------------------------------------------------------------- 

      outputRootFile->Close();

      //if (end == start + 1) system("display " + outputFileName + ".png &");
      //if (end == start + 1 && variable == "ZNGoodJets_Zexc") system("display " + outputFileName.ReplaceAll("ZNGoodJets_Zexc", "ZNGoodJets_Zinc") + ".png &");
      
    }

    //--- Close all files ----------------------------------------------------------------------
    closeAllFiles(fData, fDYJets, fBg, NBGDYJETS);
    if(fSheUnf) fSheUnf->Close();
    //fGen1->Close();
    //fGen2->Close();
    //------------------------------------------------------------------------------------------ 

}

void createSystPlots(TString outputFileName, TString variable, TString lepSel, TH1D *hUnfData[], bool logy)
{

  // 0 - Central, 
  // 1 - JES up, 2 - JES down, 
  // 3 - PU up, 4 - PU down, 
  // 5 - JER up, 6 - JER down, 
  // 7 - XSEC up, 8 - XSEC down
  // 9 - LES up, 10 - LES down
  // 11 - LER up, 12 - LER down
  // 13 - Lumi up, 14 - Lumi down
  // 15 - SF up, 16 - SF down
  // 17 - SherpaUnf
  TString syst[] = {"JES", "PU", "JER", "XSec", "LES", "LER", "Lumi", "S.F."};
  //TString syst[] = {"JES", "PU", "JER", "XSec", "BtagSF", "LER", "Lumi", "S.F."};
  for (int i = 1; i < 16; i += 2) {

    TH1D *hCent = (TH1D*) hUnfData[0]->Clone();
    hCent->SetMarkerColor(kBlack);
    hCent->SetMarkerStyle(20);
    if (variable == "ZNGoodJets_Zexc") hCent->GetXaxis()->SetRangeUser(1, 8);
    if (variable.Index("JetPt_Zinc") >= 0) hCent->GetXaxis()->SetRangeUser(30, hCent->GetXaxis()->GetXmax());
	hCent->GetXaxis()->SetLabelSize(0);
	hCent->GetYaxis()->SetTitle("d#sigma");
	hCent->GetYaxis()->SetTitleSize(0.05);
	hCent->GetYaxis()->SetTitleOffset(0.9);

	TH1D *hUp = (TH1D*) hUnfData[i]->Clone();
	hUp->SetLineColor(kGreen+2);
	hUp->SetLineWidth(2);
	TH1D *hDown = (TH1D*) hUnfData[i+1]->Clone();
	hDown->SetLineColor(kBlue);
	hDown->SetLineWidth(2);

	TCanvas *c = new TCanvas(variable + " - " + syst[i/2], variable + " - " + syst[i/2], 700, 900);
	c->cd();

	TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1);
	pad1->SetTopMargin(0.11);
	pad1->SetBottomMargin(0.02);
	pad1->SetRightMargin(0.03);
	pad1->SetLeftMargin(0.15);
	pad1->SetTicks();
	pad1->SetLogy(logy ? kTRUE : kFALSE);
	pad1->Draw();
	pad1->cd();

	TLegend *leg = new TLegend(0.8, 0.7, 0.95, 0.86);
	leg->SetBorderSize(0);
	leg->SetFillStyle(0);
	hCent->DrawCopy("e");
	hUp->DrawCopy("samehist");
	hDown->DrawCopy("samehist");
	leg->AddEntry(hUp, syst[i/2] + " Up", "l");
	leg->AddEntry(hCent, "Central", "lp");
	leg->AddEntry(hDown, syst[i/2] + " Down", "l");
	leg->Draw();
	pad1->Draw();
	fixYscale();
	c->cd();

	TPad *pad2 = new TPad("pad2", "pad2", 0, 0, 1, 0.3);
	pad2->SetTopMargin(0.02);
	pad2->SetBottomMargin(0.3);
	pad2->SetRightMargin(0.03);
	pad2->SetLeftMargin(0.15);
	pad2->SetGridy();
	pad2->SetTicks();
	pad2->Draw();
	pad2->cd();
	hUp->Divide(hCent);
	hDown->Divide(hCent);
	hCent->Divide(hCent);

	double maxRatio = max(max(hCent->GetMaximum(), hUp->GetMaximum()), hDown->GetMaximum());
	double minRatio = min(min(hCent->GetMinimum(), hUp->GetMinimum()), hDown->GetMinimum());
	int nBins = hCent->GetNbinsX();
	for (int j = 1; j <= nBins; j++) {
	    maxRatio = max(maxRatio, 1+hCent->GetBinError(j));
	    minRatio = min(minRatio, 1-hCent->GetBinError(j));
	}
	hCent->GetYaxis()->SetRangeUser(minRatio-0.02*(maxRatio-minRatio), maxRatio+0.02*(maxRatio-minRatio));
	hCent->GetYaxis()->SetRangeUser(0.82, 1.18);
	if (variable == "ZNGoodJets_Zexc") hCent->GetXaxis()->SetRangeUser(1, 8);
	hCent->SetTitle(" ");
	hCent->GetYaxis()->SetTitle("Ratio to Central");
	hCent->GetYaxis()->SetTitleSize(0.08);
	hCent->GetYaxis()->SetTitleOffset(0.9);
	hCent->GetYaxis()->CenterTitle();
	hCent->GetYaxis()->SetLabelSize(0.08);
	hCent->GetYaxis()->SetLabelOffset(0.014);
	hCent->GetXaxis()->SetTitleSize(0.13);
	//hCent->GetXaxis()->SetLabelSize(0.13);
	hCent->GetXaxis()->SetLabelSize(0.08);
	hCent->GetXaxis()->SetLabelOffset(0.012);

	hCent->DrawCopy("e");
	hUp->Draw("histsame");
	hDown->Draw("histsame");
	c->cd();
	c->Update();
	c->Draw();

	TString systStr = syst[i/2];
	if (systStr == "S.F.") systStr = "SF";
	system("mkdir SystPlot");
	c->SaveAs("SystPlot/" + lepSel + "_" + variable + "_" + systStr + ".png");
	c->SaveAs("SystPlot/" + lepSel + "_" + variable + "_" + systStr + ".ps");
	c->SaveAs("SystPlot/" + lepSel + "_" + variable + "_" + systStr + ".eps");
	c->SaveAs("SystPlot/" + lepSel + "_" + variable + "_" + systStr + ".pdf");
	c->SaveAs("SystPlot/" + lepSel + "_" + variable + "_" + systStr + ".C");
	c->SaveAs("SystPlot/" + lepSel + "_" + variable + "_" + systStr + ".root");
    }
}

//void createInclusivePlots(bool doNormalized, TString outputFileName, TString lepSel, TH1D *hUnfData, TH2D *hCov[], TH1D *hMadGenCrossSection, TH1D *hSheGenCrossSection, TH1D *hPowGenCrossSection)
//void createInclusivePlots(bool doNormalized, TString outputFileName, TString lepSel, TH1D *hUnfData, TH2D *hCov[], TH1D *hMadGenCrossSection)
void createInclusivePlots(bool doNormalized, TString outputFileName, TString lepSel, TH1D *hUnfData, TH2D *hCov[], TH1D *hMadGenCrossSection, TH1D *hSheGenCrossSection)
{
    //    std::cerr << "createInclusivePlots disabled!" << __FILE__ << __LINE__ << "\n\n";
    //return;

    TH1D *hInc = (TH1D*) hUnfData->Clone("ZNGoodJets_Zinc");
    TH1D *hIncMad = (TH1D*) hMadGenCrossSection->Clone("ZNGoodJets_Zinc_Mad");
    TH1D *hIncShe = (TH1D*) hSheGenCrossSection->Clone("ZNGoodJets_Zinc_She");
    //TH1D *hIncPow = (TH1D*) hPowGenCrossSection->Clone("ZNGoodJets_Zinc_Pow");
    TH2D *hCovInc[12] = {NULL};
    if(hCov[0]) hCovInc[0] = (TH2D*) hCov[0]->Clone("CovDataStat");
    if(hCov[1]) hCovInc[1] = (TH2D*) hCov[1]->Clone("CovMCStat");
    if(hCov[2]) hCovInc[2] = (TH2D*) hCov[2]->Clone("CovJES");
    if(hCov[3]) hCovInc[3] = (TH2D*) hCov[3]->Clone("CovPU");
    if(hCov[4]) hCovInc[4] = (TH2D*) hCov[4]->Clone("CovJER");
    if(hCov[5]) hCovInc[5] = (TH2D*) hCov[5]->Clone("CovXSec");
    if(hCov[6]) hCovInc[6] = (TH2D*) hCov[6]->Clone("CovLES");
    if(hCov[7]) hCovInc[7] = (TH2D*) hCov[7]->Clone("CovLER");
    if(hCov[8]) hCovInc[8] = (TH2D*) hCov[8]->Clone("CovLumi");
    if(hCov[9]) hCovInc[9] = (TH2D*) hCov[9]->Clone("CovSF");
    if(hCov[10]) hCovInc[10] = (TH2D*) hCov[10]->Clone("CovSherpaUnf");
    else hCovInc[10] = 0;
    if(hCov[11]) hCovInc[11] = (TH2D*) hCov[11]->Clone("CovTotSyst");

    int nBins = hInc->GetNbinsX();
    for (int i = 1; i <= nBins; i++) {
	double binSum = 0;
	double binSumMad = 0;
	double binSumShe = 0;
	//double binSumPow = 0;
	double binStatError2 = 0;
	double binStatMadError2 = 0;
	double binStatSheError2 = 0;
	//double binStatPowError2 = 0;
	double binCov[12] = {0};
	for (int j = i; j <= nBins; j++) {
	    binSum += hInc->GetBinContent(j);
	    binSumMad += hIncMad->GetBinContent(j);
	    binSumShe += hIncShe->GetBinContent(j);
	    //binSumPow += hIncPow->GetBinContent(j);
	    binStatError2 += pow(hInc->GetBinError(j), 2);
	    binStatMadError2 += pow(hIncMad->GetBinError(j), 2);
	    binStatSheError2 += pow(hIncShe->GetBinError(j), 2);
	    //binStatPowError2 += pow(hIncPow->GetBinError(j), 2);
	    for (int k = 0; k < 12; k++) {
	      if(hCovInc[k]) binCov[k] += hCovInc[k]->GetBinContent(j, j);
	    }
	}
	hInc->SetBinContent(i, binSum);
	hIncMad->SetBinContent(i, binSumMad);
	hIncShe->SetBinContent(i, binSumShe);
	//hIncPow->SetBinContent(i, binSumPow);
	hInc->SetBinError(i, sqrt(binStatError2));
	hIncMad->SetBinError(i, sqrt(binStatMadError2));
	hIncShe->SetBinError(i, sqrt(binStatSheError2));
	//hIncPow->SetBinError(i, sqrt(binStatPowError2));
	for (int k = 0; k < 12; k++) {
	  if(hCovInc[k]) hCovInc[k]->SetBinContent(i, i, binCov[k]);
	}
    }

    //TCanvas *crossSectionPlot = makeCrossSectionPlot(lepSel, TString("ZNGoodJets_Zinc"), doNormalized, hInc, hCovInc[11], hIncMad, hIncShe, hIncPow); 
    TCanvas *crossSectionPlot = makeCrossSectionPlot(lepSel, TString("ZNGoodJets_Zinc"), doNormalized, hInc, hCovInc[11], hIncMad, hIncShe);
    //TCanvas *crossSectionPlot = makeCrossSectionPlot(lepSel, TString("ZNGoodJets_Zinc"), doNormalized, hInc, hCovInc[11], hIncMad); 
    outputFileName.ReplaceAll("_Zexc", "_Zinc");
    crossSectionPlot->Draw();
    crossSectionPlot->SaveAs(outputFileName + ".png");
    crossSectionPlot->SaveAs(outputFileName + ".pdf");
    crossSectionPlot->SaveAs(outputFileName + ".eps");
    crossSectionPlot->SaveAs(outputFileName + ".ps");
    crossSectionPlot->SaveAs(outputFileName + ".C");
    crossSectionPlot->SaveAs(outputFileName + "_canvas.root");
    createTable(outputFileName, lepSel, TString("ZNGoodJets_Zinc"), doNormalized, hInc, hCovInc);
}

void createTable(TString outputFileName, TString lepSel, TString variable, bool doNormalized, TH1D *hUnfData, TH2D *hCov[])
{
    cout << "Hello" << endl;    
    //--- print out break down of errors ---
    TString title = hUnfData->GetTitle();
    int nBins = hUnfData->GetNbinsX();
    TString var = "";
    TString dSigma = "";
    TString xtitle = hUnfData->GetXaxis()->GetTitle();
    createTitleVariableAnddSigma(variable, doNormalized, xtitle, title, var, dSigma);
    cout << "Title: " << title << endl;
    cout << "Var: " << var << endl;
    cout << "dSig: " << dSigma << endl;
    cout << "nBins: " << nBins << endl;

    TString table = "\\begin{table}[htb!]\n\\begin{center}\n";
    table += "\\caption{Differential cross section in " + title;
    table += " and break down of the systematic uncertainties for the ";
    if (lepSel == "DMu") table += "muon decay channel.}\n";
    if (lepSel == "DE") table += "electron decay channel.}\n";
    table += "\\scriptsize{\n";
    table += "\\begin{tabular}{c|cc|ccccccccc}\n";
    table += "\\multicolumn{12}{c}{" + title + "} \\\\\n";
    table += var + " & " + dSigma + " & \\tiny{Tot. Unc [\\%]} & ";
    table += "\\tiny{stat [\\%]} & \\tiny{MC stat [\\%]} & \\tiny{JES [\\%]} & \\tiny{JER [\\%]} & ";
    table += "\\tiny{PU [\\%]} & \\tiny{XSEC [\\%]} & \\tiny{Lumi [\\%]} & ";
    //table += "\\tiny{LES [\\%]} & \\tiny{LER [\\%]} & ";
    table += "\\tiny{BtagSF [\\%]} & \\tiny{LER [\\%]} & ";
    table += "\\tiny{Unf [\\%]} & \\tiny{Eff [\\%]} \\\\\\hline\n";

    int start = 1;
    if (title.Index("multiplicity", 0, TString::ECaseCompare::kIgnoreCase) >= 0) {
        start = 1; 
        //nBins--;
    }
    if (title.Index("jet $p_{\\text{T}}$", 0, TString::ECaseCompare::kIgnoreCase) >= 0) start = 3; 

    for (int i = start; i <= nBins; ++i) {
	double xs = hUnfData->GetBinContent(i);
	TString numbers;
	if (title.Index("exclusive jet multiplicity", 0,  TString::ECaseCompare::kIgnoreCase) >= 0) {
	    numbers.Form("= %d", i - 1);
	}
	else if (title.Index("inclusive jet multiplicity", 0,  TString::ECaseCompare::kIgnoreCase) >= 0) {
	    numbers.Form("$\\geq$ %d", i - 1);
	}
	else {
	    numbers.Form("$%g \\ -\\ %g$", hUnfData->GetBinLowEdge(i), hUnfData->GetBinLowEdge(i+1));
	}
	table += numbers + " & "; 
	numbers.Form("%#.3g", xs);
	table += numbers + " & ";
	// total uncertainty
	if(hCov[11]) numbers.Form("%#.2g", sqrt(hCov[0]->GetBinContent(i,i) + hCov[11]->GetBinContent(i,i))*100./xs);
	else numbers="N/A";
	table += numbers + " & ";
	// stat uncertainty
	if(hCov[0]) numbers.Form("%#.2g", sqrt(hCov[0]->GetBinContent(i,i))*100./xs);
	else numbers="N/A";
	table += numbers + " & ";
	// MC stat uncertainty
	if(hCov[1]) numbers.Form("%#.2g", sqrt(hCov[1]->GetBinContent(i,i))*100./xs);
	else numbers="N/A";
	table += numbers + " & ";
	// JES uncertainty
	if(hCov[2]) numbers.Form("%#.2g", sqrt(hCov[2]->GetBinContent(i,i))*100./xs);
	else numbers="N/A";
	table += numbers + " & ";
	// JER uncertainty
	if(hCov[4]) numbers.Form("%#.2g", sqrt(hCov[4]->GetBinContent(i,i))*100./xs);
	else numbers="N/A";
	table += numbers + " & ";
	// PU uncertainty
	if(hCov[3]) numbers.Form("%#.2g", sqrt(hCov[3]->GetBinContent(i,i))*100./xs);
	else numbers="N/A";
	table += numbers + " & ";
	// XSec uncertainty
	if(hCov[5]) numbers.Form("%#.2g", sqrt(hCov[5]->GetBinContent(i,i))*100./xs);
	else numbers="N/A";
	table += numbers + " & ";
	// Lumi uncertainty
	if(hCov[8]) numbers.Form("%#.2g", sqrt(hCov[8]->GetBinContent(i,i))*100./xs);
	else numbers="N/A";
	table += numbers + " & ";
	// LES uncertainty
	if(hCov[6]) numbers.Form("%#.2g", sqrt(hCov[6]->GetBinContent(i,i))*100./xs);
	else numbers="N/A";
	table += numbers + " & ";
	// LER uncertainty
	if(hCov[7]) numbers.Form("%#.2g", sqrt(hCov[7]->GetBinContent(i,i))*100./xs);
	else numbers="N/A";
	table += numbers + " & ";
	if (hCov[10]){
	  // Unf uncertainty
	  numbers.Form("%#.2g", sqrt(hCov[10]->GetBinContent(i,i))*100./xs);
	  table += numbers + " & ";
	}
	// SF uncertinaty
	if(hCov[9]) numbers.Form("%#.2g", sqrt(hCov[9]->GetBinContent(i,i))*100./xs);
	else numbers="N/A";
	table += numbers + " \\\\\n";
    }

    table += "\\end{tabular}}\n";
    table += "\\label{tab:" + lepSel + variable + "}\n";
    table += "\\end{center}\\end{table}\n";
    ofstream out(outputFileName + ".tex");
    out << table;
    out.close();
    if(verbosity > 0) cout << table << endl;
}


int UnfoldData(const TString lepSel, const TString algo, int svdKterm, RooUnfoldResponse *resp,
	       TH1D* hRecDataMinusFakes,
	       TH1D* &hUnfData, TH1D* &hUnfDataNoScale, TH2D* &hUnfDataStatCov, TH2D* &hUnfDataStatCovNoScale, TH2D* &hUnfMCStatCov, TH2D* &hUnfMCStatCovNoScale, TString name,
	       double integratedLumi, bool logy,
	       TH1D *hRecDataMinusFakesOdd, TH1D *hRecDataMinusFakesEven)
{
    //--- make sure we use OverFlow (should already be set to true) ---
    resp->UseOverflow();

    //--- Set the required unfolding algorithm ---
    RooUnfold::Algorithm alg;
    if (algo == "Bayes") {
	alg = RooUnfold::kBayes;
    }
    else if (algo == "SVD") {
	alg = RooUnfold::kSVD;
    }
    else {
	cerr << "Error: the specified algo: " << algo << " is not implemented!" << endl;
	cerr << "       I will proceed with kBayes algo" << endl;
	alg = RooUnfold::kBayes;
    }

    std::cout << "-----------------------" << std::endl;
    TString variable = TString(hRecDataMinusFakes->GetName());
    system("mkdir UnfoldingCheck/");
    TFile *f = new TFile("UnfoldingCheck/" + lepSel + "_" + variable + "_" + name + ".root", "RECREATE");
    f->cd();

    TH2D* hresp = (TH2D*) resp->Hresponse()->Clone(TString::Format("hResp%s%s", variable.Data(), name.Data()));
    hresp->Write();
    
    int finalNIter = -1;
    int nIter = 99;
    int finalNIterXval = -1;
    int nIterXval = 99;

    int nBinsTmp = hRecDataMinusFakes->GetNbinsX();
    //int nBinsTmp = 50;

    bool svd_unfold = cfg.getB("svdUnfold", false);
    bool tsvd_unfold = cfg.getB("tsvdUnfold", false);
    bool binByBin_unfold = cfg.getB("binByBinUnfold", false);
    bool xvalIter = cfg.getB("xvalIter", false);
    int minIter = cfg.getI("minIter", 2);
    int maxIter = cfg.getI("maxIter", 20);
    int nSkipFirstJetPtBins = cfg.getI("nSkipFirstJetPtBins", 2);
    bool useFlatPrior = cfg.getB("useFlatPrior", false);
    double lumiUnc = cfg.getD("lumiUnc", 0.023);
    verbosity = cfg.getI("unfoldingVerbosity", 1);
        
    TH1D *hchi2 = new TH1D("hchi2", "hchi2", nBinsTmp + 1, -0.5, nBinsTmp + .5);
    hchi2->SetTitle(TString::Format("#chi^{2}/ndf of reco vs folded-unfolded for %s %s",
				    variable.Data(), name.Data()));
    hchi2->GetYaxis()->SetTitle("#chi^{2}/ndf");
    hchi2->GetYaxis()->SetTitleOffset(1.40);
    hchi2->GetXaxis()->SetTitle("number of iterations of the Bayes method");
    hchi2->GetXaxis()->CenterTitle();
    hchi2->GetXaxis()->SetNdivisions(nBinsTmp, 0, 0);
    hchi2->GetXaxis()->SetLabelSize(0.03);
    hchi2->SetLineWidth(2);    
    TH1D* hchi2Xval = 0;
    TH1* hResMaxXval = 0;
    if(hRecDataMinusFakesOdd && hRecDataMinusFakesEven){
	hchi2Xval = (TH1D*) hchi2->Clone("hchi2Xval");
	hchi2Xval->Reset();
	hResMaxXval = new TH1D("hResMaxXval",
			       TString::Format("Max(res) %s %s;Iter;max(res) cross-validation",
					       variable.Data(), name.Data()),
			       nBinsTmp + 1, -.5, nBinsTmp +  0.5);
	
    }
    
#ifdef FAST_BAYES
    
//FAST..    RooUnfoldResponse *respBis = (RooUnfoldResponse*) resp->Clone();
//FAST..    TH1D *hRecDataMinusFakesBis = (TH1D*) hRecDataMinusFakes->Clone();
//FAST..
//FAST..    std::cerr << "========= > " << hRecDataMinusFakes->GetBinError(3) << "\n";
//FAST..    
//FAST..    RooUnfold *RObjectForDataTmp = RooUnfold::New(alg, respBis, hRecDataMinusFakesBis, nBinsTmp);
//FAST..    RObjectForDataTmp->SetVerbose(verbosity);
//FAST..    //RObjectForDataTmp->UseFlatPrior(true);
//FAST..    int nBinsToSkip = (TString(hRecDataMinusFakesBis->GetName()).Index("JetPt_Zinc") > 0) ? 2 : 0;
//FAST..    //nBinsToSkip = 0;
//FAST..    RObjectForDataTmp->IncludeSystematics(0); // new version of RooUnfold: will compute Cov based on Data Statistics only
//FAST..    std::auto_ptr<std::vector<TH1*> > hUnfs (new std::vector<TH1*>);
//FAST..
//FAST..    //Unfolds the data:
//FAST..    TH1D* hUnfDataBis = (TH1D*) RObjectForDataTmp->Hreco(RooUnfold::kCovariance, hUnfs.get());
//FAST..    //RObjectForDataTmp->Hreco(RooUnfold::kCovariance, hUnfs.get());
//FAST..
//FAST..    std::cerr << "========= > " << (*hUnfs)[4]->GetBinError(3) << "\n";
//FAST..    std::cerr << "========= > " << (*hUnfs)[nBinsTmp]->GetBinError(3) << "\n";
//FAST..    std::cerr << "========= > " << hUnfDataBis->GetBinError(3) << "\n";
//FAST..    
//FAST..    //    TString tmpStr2 = TString::Format("%s residuals;Iter;Residual", name.Data());
//FAST..    TH1* hResMax = new TH1D("hResMax", TString::Format("Max(res) %s %s;Iter;max(res)", variable.Data(), name.Data()),
//FAST..			    hUnfs->size(), -.5, hUnfs->size() - 0.5);
//FAST..    
//FAST..    for (unsigned i = 0; i < hUnfs->size(); ++i) {
//FAST..      (*hUnfs)[i]->SetName(TString::Format("hUnf%d", i));
//FAST..      if(i==0){
//FAST..	(*hUnfs)[i]->SetTitle(TString::Format("Prior for %s %s %d iters", variable.Data(), name.Data(), i));
//FAST..      } else {
//FAST..	(*hUnfs)[i]->SetTitle(TString::Format("Unfolded %s %s %d iters", variable.Data(), name.Data(), i));
//FAST..      }
//FAST..      (*hUnfs)[i]->Write();
//FAST..      TH1* hfoldUnfData = foldUnfData((*hUnfs)[i], respBis);
//FAST..      hfoldUnfData->SetName(TString::Format("datafoldedBack%s_%d", name.Data(), i));
//FAST..      hfoldUnfData->SetTitle(TString::Format("dataFoldedBack%s_%d", name.Data(), i));
//FAST..      hfoldUnfData->Write();
//FAST..
//FAST..      TH1* hRes = (TH1*) hRecDataMinusFakes->Clone(TString::Format("hRes%s_%d", name.Data(), i+1));
//FAST..      hRes->Reset();
//FAST..      hRes->SetTitle(TString::Format("%s %s residuals - iteration %d",
//FAST..				     variable.Data(), name.Data(), i+1));
//FAST..      hRes->GetYaxis()->SetTitle("Residuals");
//FAST..      const int nRes = hfoldUnfData->GetXaxis()->GetLast() - hfoldUnfData->GetXaxis()->GetFirst() + 1;
//FAST..      Double_t res[nRes];
//FAST..      double mychi2 = MyChi2Test(hRecDataMinusFakesBis, hfoldUnfData, nBinsToSkip, res);
//FAST..      if(verbosity) std::cout << "Chi2/nbins of data / folded-unfolded distributions: " << mychi2 << "\n"; 
//FAST..      if(i > 0) hchi2->SetBinContent(i + 1, mychi2);
//FAST..      if (mychi2 < 1./sqrt(2) && finalNIter < 0) {
//FAST..	nIter = i;
//FAST..	finalNIter = i;
//FAST..	std::cout << "We will use " << nIter << " iterations with a final Chi2/ndf of: " << mychi2 << std::endl;
//FAST..      }      
//FAST..      double maxRes = -1.;
//FAST..      for(int j = 0; j < nRes; ++j){
//FAST..	double x = fabs(res[j]);
//FAST..	  hRes->SetBinContent(hfoldUnfData->GetXaxis()->GetFirst() + j, x);
//FAST..	  if(x > maxRes) maxRes = x;
//FAST..      }
//FAST..      hRes->Write();
//FAST..      if(i>0) hResMax->Fill(i + 1, maxRes);
//FAST..    }
//FAST..    hResMax->Write();
//FAST..    TH1D *hgen = (TH1D*) respBis->Htruth();
//FAST..    TString tmpName = "mcGen" + name;
//FAST..    hgen->SetName(tmpName);
//FAST..    hgen->SetTitle(tmpName);
//FAST..    hgen->Write();
//FAST..    
//FAST..    TH1D *hfoldgen = foldUnfData(hgen, respBis);
//FAST..    tmpName = "mcGenFolded" + name;
//FAST..    hfoldgen->SetName(tmpName);
//FAST..    hfoldgen->SetTitle(tmpName);
//FAST..    hfoldgen->Write();
//FAST..
//FAST..    TH1D *hmes = (TH1D*) respBis->Hmeasured();
//FAST..    tmpName = "mcReco" + name;
//FAST..    hmes->SetName(tmpName);
//FAST..    hmes->SetTitle(tmpName);
//FAST..    hmes->Write();
//FAST..    
//FAST..    hRecDataMinusFakes->Write("Unf" + name + "_0");
//FAST..
//FAST..    if(hRecDataMinusFakesOdd && hRecDataMinusFakesEven){
//FAST..
//FAST..	//hchi2Xval = (TH1D*) hchi2->Clone("hchi2Xval");
//FAST..	//hchi2Xval->Reset();
//FAST..      
//FAST..      RooUnfoldResponse *respBis2 = (RooUnfoldResponse*) resp->Clone();
//FAST..      TH1D *hRecDataMinusFakesBisOdd = (TH1D*) hRecDataMinusFakesOdd->Clone();
//FAST..      TH1D *hRecDataMinusFakesBisEven = (TH1D*) hRecDataMinusFakesEven->Clone();
//FAST..
//FAST..      //std::cout << "Chi2/ndf odd/even: " << MyChi2Test(hRecDataMinusFakesOdd, hRecDataMinusFakesEven) << "\n";
//FAST..      //      std::cout << "ROOT Chi2/ndf odd/even: "
//FAST..      //		<< hRecDataMinusFakesOdd->Chi2Test(hRecDataMinusFakesEven, "WW,P,CHI2/NDF") << "\n";
//FAST..      
//FAST..      RooUnfold *RObjectForDataTmp2 = RooUnfold::New(alg, respBis2, hRecDataMinusFakesBisOdd, nBinsTmp);
//FAST..      RObjectForDataTmp2->SetVerbose(verbosity);
//FAST..      //      RObjectForDataTmp2->UseFlatPrior(true);
//FAST..      //    int nBinsToSkip = (TString(hRecDataMinusFakesBis->GetName()).Index("JetPt_Zinc") > 0) ? 2 : 0;
//FAST..      int nBinsToSkip = 0;
//FAST..      RObjectForDataTmp2->IncludeSystematics(0); // new version of RooUnfold: will compute Cov based on Data Statistics only
//FAST..      std::auto_ptr<std::vector<TH1*> > hUnfs2 (new std::vector<TH1*>);
//FAST..      
//FAST..      //Unfolds the data:
//FAST..      //TH1D* hUnfDataBis = (TH1D*) RObjectForDataTmp->Hreco(RooUnfold::kCovariance, hUnfs.get());
//FAST..      RObjectForDataTmp2->Hreco(RooUnfold::kCovariance, hUnfs2.get());
//FAST..      
//FAST..      TH1* hResMaxXval = new TH1D("hResMaxXval",
//FAST..				  TString::Format("Max(res) %s %s;Iter;max(res) cross-validation",
//FAST..						  variable.Data(), name.Data()),
//FAST..				  hUnfs->size(), -.5, hUnfs->size() - 0.5);
//FAST..      
//FAST..      for (unsigned i = 0; i < hUnfs2->size(); ++i) {
//FAST..	(*hUnfs2)[i]->SetName(TString::Format("hUnfXval%d", i));
//FAST..	if(i==0){
//FAST..	  (*hUnfs2)[i]->SetTitle(TString::Format("Prior for %s %s", variable.Data(), name.Data()));
//FAST..	} else {
//FAST..	  (*hUnfs2)[i]->SetTitle(TString::Format("Unfolded %s %s %d iters, cross-validation", variable.Data(), name.Data(), i));
//FAST..	}
//FAST..	(*hUnfs2)[i]->Write();
//FAST..	TH1* hfoldUnfDataOdd = foldUnfData((*hUnfs2)[i], respBis);
//FAST..	hfoldUnfDataOdd->SetName(TString::Format("dataOddfoldedBack%s_%d", name.Data(), i));
//FAST..	hfoldUnfDataOdd->SetTitle(TString::Format("dataOddFoldedBack%s_%d", name.Data(), i));
//FAST..	hfoldUnfDataOdd->Write();
//FAST..	
//FAST..	TH1* hResXval = (TH1*) hRecDataMinusFakes->Clone(TString::Format("hResXval%s_%d", name.Data(), i+1));
//FAST..	hResXval->Reset();
//FAST..	hResXval->SetTitle(TString::Format("%s %s residuals - iteration %d, cross validation",
//FAST..				       variable.Data(), name.Data(), i+1));
//FAST..	hResXval->GetYaxis()->SetTitle("Residuals");
//FAST..	
//FAST..	const int nRes = hfoldUnfDataOdd->GetXaxis()->GetLast() - hfoldUnfDataOdd->GetXaxis()->GetFirst() + 1;
//FAST..	Double_t resXval[nRes];
//FAST..	double mychi2 = MyChi2Test(hfoldUnfDataOdd, hRecDataMinusFakesBisEven, nBinsToSkip, resXval);
//FAST..	if(verbosity) std::cout << "Chi2/nbins of data / folded-unfolded distributions: " << mychi2 << "\n"; 
//FAST..
//FAST..	if(i > 0) hchi2Xval->SetBinContent(i + 1, mychi2);
//FAST..	
//FAST..	if (mychi2 < 1. && finalNIterXval < 0) {
//FAST..	  nIterXval = i;
//FAST..	  finalNIterXval = i;
//FAST..	  std::cout << "We will use " << nIter << " iterations with a final Chi2/ndf of: " << mychi2 << std::endl;
//FAST..	}      
//FAST..	double maxRes = -1.;
//FAST..	for(int j = 0; j < nRes; ++j){
//FAST..	  double x = fabs(resXval[j]);
//FAST..	  hResXval->SetBinContent(hfoldUnfDataOdd->GetXaxis()->GetFirst() + j, x);
//FAST..	  if(x > maxRes) maxRes = x;
//FAST..	}
//FAST..	hResXval->Write();
//FAST..	if(i > 0) hResMaxXval->Fill(i+1, maxRes);
//FAST..      } //next hUnfs2
//FAST..      
//FAST..      hResMaxXval->Write();
//FAST..      TH1D *hgen = (TH1D*) respBis->Htruth();
//FAST..      TString tmpName = "mcGen" + name;
//FAST..      hgen->SetName(tmpName);
//FAST..      hgen->SetTitle(tmpName);
//FAST..      hgen->Write();
//FAST..      
//FAST..      TH1D *hfoldgen = foldUnfData(hgen, respBis);
//FAST..      tmpName = "mcGenFolded" + name;
//FAST..      hfoldgen->SetName(tmpName);
//FAST..      hfoldgen->SetTitle(tmpName);
//FAST..      hfoldgen->Write();
//FAST..      
//FAST..      TH1D *hmes = (TH1D*) respBis->Hmeasured();
//FAST..      tmpName = "mcReco" + name;
//FAST..      hmes->SetName(tmpName);
//FAST..      hmes->SetTitle(tmpName);
//FAST..      hmes->Write();
//FAST..      hRecDataMinusFakesOdd->Write("UnfOdd" + name + "_0");
//FAST..      hRecDataMinusFakesEven->Write("UnfEven" + name + "_0");
//FAST..    }//cross-validation
    
#else
    TH1* hResMax = new TH1D("hResMax", TString::Format("Max(res) %s %s;Iter;max(res)", variable.Data(), name.Data()),
			    nBinsTmp+1, -.5, nBinsTmp + 0.5);
    
    int nBinsToSkip = (TString(hRecDataMinusFakes->GetName()).Index("JetPt_Zinc") > 0) ? nSkipFirstJetPtBins : 0;

    std::cout << "Bayes unfolding, number of first bins to skip: " << nBinsToSkip << "\n";

    //Test different regularisation values (=number of iterations for Bayes case)
    for (int i = 1; i <= nBinsTmp; ++i) {
	RooUnfoldResponse *respBis = (RooUnfoldResponse*) resp->Clone();
	TH1D *hRecDataMinusFakesBis = (TH1D*) hRecDataMinusFakes->Clone();
	RooUnfold *RObjectForDataTmp = RooUnfold::New(alg, respBis, hRecDataMinusFakesBis, i);
	RObjectForDataTmp->SetVerbose(verbosity);
	RObjectForDataTmp->UseFlatPrior(useFlatPrior);
	RObjectForDataTmp->IncludeSystematics(0); // new version of RooUnfold: will compute Cov based on Data Statistics only
	std::cout << "niter = " << i << std::endl;
	TH1D* hUnfDataBis = (TH1D*) RObjectForDataTmp->Hreco(RooUnfold::kCovariance);
	TString tmpName = "Unf" + name + "_"; 
	tmpName += i; 
	hUnfDataBis->SetName(tmpName);
	hUnfDataBis->SetTitle(tmpName);
	tmpName = "fol" + name + "_"; 
	tmpName += i; 
	TH1D *hfoldUnfData = foldUnfData(hUnfDataBis, respBis);
	hfoldUnfData->SetName(tmpName);
	hfoldUnfData->SetTitle(tmpName);
	TH1D *hgen = (TH1D*) respBis->Htruth();
	tmpName = "gen" + name + "_";
	tmpName += i;
	hgen->SetName(tmpName);
	hgen->SetTitle(tmpName);
	hgen->Write();
	TH1D *hfoldgen = foldUnfData(hgen, respBis);
	tmpName = "folgen" + name + "_";
	tmpName += i;
	hfoldgen->SetName(tmpName);
	hfoldgen->SetTitle(tmpName);
	hfoldgen->Write();
	TH1D *hmes = (TH1D*) respBis->Hmeasured();
	tmpName = "mes" + name + "_";
	tmpName += i;
	hmes->SetName(tmpName);
	hmes->SetTitle(tmpName);
	hmes->Write();

	TH1* hRes = (TH1*) hRecDataMinusFakes->Clone(TString::Format("hRes%s_%d", name.Data(), i));
	hRes->Reset();
	hRes->SetTitle(TString::Format("%s %s residuals - iteration %d",
				       variable.Data(), name.Data(), i));
	hRes->GetYaxis()->SetTitle("Residuals");

	if(hRecDataMinusFakesOdd && hRecDataMinusFakesEven){
	    TH1D *hRecDataMinusFakesBisOdd = (TH1D*) hRecDataMinusFakesOdd->Clone();
	    TH1D *hRecDataMinusFakesBisEven = (TH1D*) hRecDataMinusFakesEven->Clone();

	    RooUnfoldResponse *respBis2 = (RooUnfoldResponse*) resp->Clone();
	    RooUnfold *RObjectForDataTmp3 = RooUnfold::New(alg, respBis2, hRecDataMinusFakesBisOdd, i);
	    RObjectForDataTmp3->SetVerbose(verbosity);
	    RObjectForDataTmp3->IncludeSystematics(0); // new version of RooUnfold: will compute Cov based on Data Statistics only

	    TH1* hUnfDataOdd = RObjectForDataTmp3->Hreco(RooUnfold::kCovariance);
	    
	    TH1* hfoldUnfDataOdd = foldUnfData(hUnfDataOdd, respBis);
	    hfoldUnfDataOdd->SetName(TString::Format("dataOddfoldedBack%s_%d", name.Data(), i));
	    hfoldUnfDataOdd->SetTitle(TString::Format("dataOddFoldedBack%s_%d", name.Data(), i));
	    hfoldUnfDataOdd->Write();
	    
	    TH1* hResXval = (TH1*) hRecDataMinusFakes->Clone(TString::Format("hResXval%s_%d", name.Data(), i+1));
	    hResXval->Reset();
	    hResXval->SetTitle(TString::Format("%s %s residuals - iteration %d, cross validation",
					       variable.Data(), name.Data(), i+1));
	    hResXval->GetYaxis()->SetTitle("Residuals");
	    const int nRes = hfoldUnfDataOdd->GetXaxis()->GetLast() - hfoldUnfDataOdd->GetXaxis()->GetFirst() + 1;
	    Double_t resXval[nRes];
	    double mychi2Xval = MyChi2Test(hfoldUnfDataOdd, hRecDataMinusFakesBisEven, nBinsToSkip, resXval);
	    hchi2Xval->SetBinContent(i, mychi2Xval);
	    if (mychi2Xval < 1. && finalNIterXval < 0) {
		nIterXval = i;
		finalNIterXval = i;
		std::cout << "nIterXval: " << nIterXval << "\n";
	    }
	    double maxRes = 0;
	    for(int j = 0; j < nRes; ++j){
		double x = fabs(resXval[j]);
		hResXval->SetBinContent(hfoldUnfDataOdd->GetXaxis()->GetFirst() + j, x);
		if(x > maxRes) maxRes = x;
	    }
	    hResXval->Write();
	    hResMaxXval->Fill(i, maxRes);
	}
	
	
	const int nRes = hfoldUnfData->GetXaxis()->GetLast() - hfoldUnfData->GetXaxis()->GetFirst() + 1;
	Double_t res[nRes];
	double mychi2 = MyChi2Test(hRecDataMinusFakesBis, hfoldUnfData, nBinsToSkip, res);
	if(verbosity) std::cout << "Chi2/nbins of data / folded-unfolded distributions: " << mychi2 << "\n"; 
	hchi2->SetBinContent(i, mychi2);
	if (i==1) hRecDataMinusFakesBis->Write("Unf" + name + "_0"); 
	hUnfDataBis->Write();
	hfoldUnfData->Write();
	if (mychi2 < 1./sqrt(2) && finalNIter < 0) {
	    nIter = i;
	    finalNIter = i;
	    std::cout << "nIter: " << nIter << "\n";
	    //break;
	}

	double maxRes = -1.;
	for(int j = 0; j < nRes; ++j){
	    double x = fabs(res[j]);
	    hRes->SetBinContent(hfoldUnfData->GetXaxis()->GetFirst() + j, x);
	    hRes->SetBinError(hfoldUnfData->GetXaxis()->GetFirst() + j, 0);
	    if(x > maxRes) maxRes = x;
	}
	hRes->Write();
	hResMax->Fill(i, maxRes);
	//if(finalNIter > 0 && nIter > 3) break;
    }
    hResMax->Write();
    if(hResMaxXval) hResMaxXval->Write();
    if(hRecDataMinusFakesOdd) hRecDataMinusFakesOdd->Write("UnfOdd" + name + "_0");
    if(hRecDataMinusFakesEven) hRecDataMinusFakesEven->Write("UnfEven" + name + "_0");

       

#endif
    //    nIter = min(nIter, 20);
    //nIter = max(nIter, 2);
   // nIter = 4;

    if (variable == "ZNGoodJets_Zexc") maxIter = 4;
    int chosenIter = 0;
    if(xvalIter){
	chosenIter = nIterXval;
    } else{
	chosenIter = nIter;
    }
    if(chosenIter < minIter){
	chosenIter = minIter;
    }
    if(chosenIter > maxIter){
	chosenIter = maxIter;
    }

    std::cout << "Bayes unfold, choice of number of iterations\n"
	"\tmin: " << minIter << "\tmax: " << maxIter << "\tmode: " << (xvalIter ? "cross-validation" : "standard")
	      << "\tnIter(std): " << nIter << "\tnIter(xval): " << nIterXval << "\n"
	"\tSelected value: " << chosenIter << "\n";
    

    hchi2->GetYaxis()->SetRangeUser(0, max(1.3, 1.1*hchi2->GetMaximum()));
    TArrow *arrowChi2 = new TArrow(nIter, min(1.2, hchi2->GetMaximum()), nIter, 0.8, 0.02, "|>");
    arrowChi2->SetLineColor(kRed);
    arrowChi2->SetFillColor(kRed);
    arrowChi2->SetLineWidth(2);

    TArrow *arrowChi2_2 = new TArrow(chosenIter, min(1.2, hchi2->GetMaximum()), chosenIter, 0.8, 0.02, "|>");
    arrowChi2_2->SetLineColor(kGreen+2);
    arrowChi2_2->SetFillColor(kGreen+2);
    arrowChi2_2->SetLineWidth(2);

    TLine *line = new TLine(0.5, 0.7, nBinsTmp + 0.5, 0.7);
    line->SetLineColor(kBlack);
    line->SetLineStyle(kDashed);
    line->SetLineWidth(2);
    TCanvas *chchi2 = new TCanvas("chchi2", "chchi2", 600, 600);
    chchi2->cd();
    chchi2->SetGrid();
    hchi2->DrawCopy();
    arrowChi2->Draw();
    arrowChi2_2->Draw();
    line->Draw();
    hchi2->Write();
    chchi2->Write();
    chchi2->Print("UnfoldingCheck/" + lepSel + "_" + variable + "_" + name + "_" + algo + "_chi2.pdf");

    if(hRecDataMinusFakesOdd && hRecDataMinusFakesEven){
      hchi2Xval->GetYaxis()->SetRangeUser(0, max(1.3, 1.1*hchi2Xval->GetMaximum()));
      TArrow *arrowChi2Xval = new TArrow(nIterXval, min(1.2, hchi2Xval->GetMaximum()), nIterXval, 0.8, 0.02, "|>");
      arrowChi2Xval->SetLineColor(kRed);
      arrowChi2Xval->SetFillColor(kRed);
      arrowChi2Xval->SetLineWidth(2);

      TArrow *arrowChi2Xval2 = new TArrow(chosenIter, min(1.2, hchi2Xval->GetMaximum()), chosenIter, 0.8, 0.02, "|>");
      arrowChi2Xval2->SetLineColor(kGreen+2);
      arrowChi2Xval2->SetFillColor(kGreen+2);
      arrowChi2Xval2->SetLineWidth(2);

      TLine *line = new TLine(0.5, 1., nBinsTmp + 0.5, 1.);
      line->SetLineColor(kBlack);
      line->SetLineStyle(kDashed);
      line->SetLineWidth(2);
      TCanvas *chchi2Xval = new TCanvas("chchi2Xval", "chchi2Xval", 600, 600);
      chchi2Xval->cd();
      chchi2Xval->SetGrid();
      hchi2Xval->DrawCopy();
      arrowChi2Xval->Draw();
      arrowChi2Xval2->Draw();
      line->Draw();
      hchi2Xval->Write();
      chchi2Xval->Write();
      chchi2Xval->Print("UnfoldingCheck/" + lepSel + "_" + variable + "_" + name + "_" + algo + "_chi2Xval.pdf");
    }
      
    if(binByBin_unfold){
      RooUnfold *RObjectForDataBinByBin = RooUnfold::New(RooUnfold::kBinByBin, resp, hRecDataMinusFakes);
      RObjectForDataBinByBin->SetVerbose(verbosity);
      TH1D *hUnfDataBinByBin = (TH1D*) RObjectForDataBinByBin->Hreco(RooUnfold::kCovariance);
      hUnfDataBinByBin->SetName("UnfDataBinByBin" + name);
      hUnfDataBinByBin->Write();
    }
    
    RooUnfold *RObjectForDataBayes = RooUnfold::New(RooUnfold::kBayes, resp, hRecDataMinusFakes, chosenIter);
    RObjectForDataBayes->SetVerbose(verbosity);
    TH1D *hUnfDataBayes = (TH1D*) RObjectForDataBayes->Hreco(RooUnfold::kCovariance);
    hUnfDataBayes->SetName("UnfDataBayes" + name);
    hUnfDataBayes->Write();
    
    if(svd_unfold){
      for (int i(1); i <= nBinsTmp; i++) {
	RooUnfold *RObjectForDataSVD = RooUnfold::New(RooUnfold::kSVD, resp, hRecDataMinusFakes, i);
	RObjectForDataSVD->SetVerbose(verbosity);
	TH1D *hUnfDataSVD = (TH1D*) RObjectForDataSVD->Hreco(RooUnfold::kCovariance);
	hUnfDataSVD->SetName("UnfDataSVD_" + TString::Format("%d", i) + "_" + name);
	hUnfDataSVD->Write();
      }
    }

    if(tsvd_unfold){
      TSVDUnfold *unfoldTSVD = new TSVDUnfold(hRecDataMinusFakes, (TH1D*)resp->Htruth(), (TH1D*)resp->Hmeasured(), (TH2D*)resp->Hresponse());
      TH1D *unfresult = (TH1D*) unfoldTSVD->Unfold(1);
      TH1D *hmodDOriginal = (TH1D*) unfoldTSVD->GetD();
      TH1D *hSV       = (TH1D*) unfoldTSVD->GetSV();
      
      TH1D *hmodD = new TH1D("hmodD", "hmodD", nBinsTmp, 0.5, nBinsTmp+0.5);
      for (int i(0); i <= nBinsTmp+1; i++) {
	hmodD->SetBinContent(i, hmodDOriginal->GetBinContent(i));
      }
      hmodD->SetTitle(hmodDOriginal->GetTitle() + TString(" for ") + TString(hRecDataMinusFakes->GetTitle()));
      hmodD->GetXaxis()->SetNdivisions(nBinsTmp, 0, 0);
      hmodD->GetXaxis()->SetLabelSize(0.03);
      hmodD->GetYaxis()->SetTitle("|d_{i}|");
      hmodD->GetYaxis()->SetTitleOffset(1.40);
      hmodD->GetXaxis()->SetTitle("regularization parameter of the SVD method");
      hmodD->GetXaxis()->CenterTitle();
      TArrow *arrowSvd = new TArrow(svdKterm, 20, svdKterm, 1.1*hmodD->GetBinContent(svdKterm), 0.02, "|>");
      arrowSvd->SetLineColor(kRed);
      arrowSvd->SetFillColor(kRed);
      arrowSvd->SetLineWidth(2);
      
      hmodD->SetName("modD" + name);
      TCanvas *chmodD = new TCanvas("chmodD", "chmodD", 600, 600);
      chmodD->cd();
      chmodD->SetGrid();
      chmodD->SetLogy(logy ? kTRUE : kFALSE);
      hmodD->DrawCopy();
      arrowSvd->Draw();
      chmodD->Write();
      hmodD->Write();
      unfresult->Write();
      hSV->Write();
    }

    f->Close();
    delete f;

    std::cout << "\n---------------------------------------------------------------------------------------------------------------\n-" << std::endl;

    //--- Unfold data minus background ---
    RooUnfold *RObjectForData = RooUnfold::New(alg, resp, hRecDataMinusFakes, chosenIter);
    RObjectForData->SetVerbose(verbosity);

    //--- get the unfolded result ---
    RObjectForData->IncludeSystematics(0); // new version of RooUnfold: will compute Cov based on Data Statistics only
    hUnfData = (TH1D*) RObjectForData->Hreco(RooUnfold::kCovariance);
    hUnfData->SetName("UnfData" + name);
    hUnfData->SetTitle(hRecDataMinusFakes->GetTitle());

    //andrew -- write out hUnfData before it gets scaled for xsec
    //Want to construct TEfficiency for hRecDataMinusFakes and hUnfDataNoScale to use Wilson score errors 
    hUnfDataNoScale = (TH1D*) hUnfData->Clone();
    hUnfDataNoScale->SetName("UnfData"+name+"_NoScale");
    hUnfDataNoScale->SetTitle(hUnfData->GetTitle());
    

    if (algo == "Bayes") {
	//--- get covariance from statistics on Data ---
	hUnfDataStatCov = M2H(RObjectForData->Ereco(RooUnfold::kCovariance)); // new version of RooUnfold   
	hUnfDataStatCov->SetName("UnfDataStatCov" + name);
	hUnfDataStatCov->SetTitle(hRecDataMinusFakes->GetTitle());
        
    //andrew
    //Write out hUnfDataStatCov before it gets scaled for xsec to later construct bin-bin correlation matrix
    hUnfDataStatCovNoScale = (TH2D*) hUnfDataStatCov->Clone();
    hUnfDataStatCovNoScale->SetName("UnfDataStatCov" + name + "_NoScale");
    hUnfDataStatCovNoScale->SetTitle(hUnfDataStatCov->GetTitle());


	//--- get covariance from MC stat ---
	RooUnfold *RObjectForMC = RooUnfold::New(alg, resp, hRecDataMinusFakes, chosenIter);
	RObjectForMC->SetVerbose(verbosity);
	RObjectForMC->IncludeSystematics(2); // new version of RooUnfold: will compute Cov based on MC Statistics only
	hUnfMCStatCov = M2H(RObjectForMC->Ereco(RooUnfold::kCovariance)); // new version of RooUnfold
	hUnfMCStatCov->SetName("UnfMCStatCov" + name);
	hUnfMCStatCov->SetTitle(hRecDataMinusFakes->GetTitle());
        
    //andrew
    hUnfMCStatCovNoScale = (TH2D*) hUnfMCStatCov->Clone();
    hUnfMCStatCovNoScale->SetName("UnfMCStatCov" + name + "_NoScale");
    hUnfMCStatCovNoScale->SetTitle(hUnfMCStatCov->GetTitle());
    }


    //--- divide by luminosity ---
    hUnfData->Scale(1./integratedLumi);
    hUnfDataStatCov->Scale(1./(integratedLumi*integratedLumi));
    hUnfMCStatCov->Scale(1./(integratedLumi*integratedLumi));
    //if ("LumiUp" == name) {
    //    hUnfData->Scale(1./1.026);
    //    hUnfDataStatCov->Scale(1./(1.026*1.026));
    //    hUnfMCStatCov->Scale(1./(1.026*1.026));
    //}
    //else if ("LumiDown" == name) {
    //    hUnfData->Scale(1./0.974);
    //    hUnfDataStatCov->Scale(1./(0.974*0.974));
    //    hUnfMCStatCov->Scale(1./(0.974*0.974));
    //}
    if ("LumiUp" == name) {
        hUnfData->Scale(1./(1+lumiUnc));
        hUnfDataStatCov->Scale(1./((1+lumiUnc)*(1+lumiUnc)));
        hUnfMCStatCov->Scale(1./((1+lumiUnc)*(1+lumiUnc)));
    }
    else if ("LumiDown" == name) {
        hUnfData->Scale(1./(1-lumiUnc));
        hUnfDataStatCov->Scale(1./((1-lumiUnc)*(1-lumiUnc)));
        hUnfMCStatCov->Scale(1./((1-lumiUnc)*(1-lumiUnc)));
    }


    //--- divide by bin width to get cross section ---
    int nBins = hUnfData->GetNbinsX();
    for (int i = 1; i <= nBins; ++i) {
	double binWidth = hUnfData->GetBinWidth(i);
	hUnfData->SetBinContent(i, hUnfData->GetBinContent(i)*1./binWidth);
	hUnfData->SetBinError(i, hUnfData->GetBinError(i)*1./binWidth);
	for (int j = 1; j <= nBins; ++j) {
	    hUnfDataStatCov->SetBinContent(i, j, hUnfDataStatCov->GetBinContent(i, j)*1./(binWidth*binWidth));
	    hUnfDataStatCov->SetBinError(i, j, hUnfDataStatCov->GetBinError(i, j)*1./(binWidth*binWidth));
	    hUnfMCStatCov->SetBinContent(i, j, hUnfMCStatCov->GetBinContent(i, j)*1./(binWidth*binWidth));
	    hUnfMCStatCov->SetBinError(i, j, hUnfMCStatCov->GetBinError(i, j)*1./(binWidth*binWidth));
	}
    }
    return chosenIter;
}


//--- This is needed because we use overflow and the matrices have 2 additional bins ---
TH2D* M2H(const TMatrixD m) 
{
    int nBinsY = m.GetNrows();
    int nBinsX = m.GetNrows();

    TH2D *h = new TH2D(m.GetName(), m.GetName(), nBinsX-2, 0, nBinsX-2, nBinsY-2, 0, nBinsY-2);
    for (int i = 0; i < nBinsX; ++i) {
	for (int j = 0; j < nBinsY; ++j) {
	    h->SetBinContent(i, j, m(i, j));
	}
    }

    return h;
}

TH2D* makeCovFromUpAndDown(const TH1D* hUnfDataCentral, const TH1D* hUnfDataUp, const TH1D* hUnfDataDown, TString name)
{
    int nBins = hUnfDataCentral->GetNbinsX();
    TH2D* h = new TH2D(name, name, nBins, 0, nBins, nBins, 0, nBins);

    for (int i = 1; i <= nBins; ++i) {
	double sigmaMeani = 0.5*fabs(hUnfDataUp->GetBinContent(i) - hUnfDataDown->GetBinContent(i)); 
	if (name.Index("Sherpa") >= 0) sigmaMeani *= 2;
	int signi = (hUnfDataUp->GetBinContent(i) - hUnfDataDown->GetBinContent(i) < 0) ? -1 : 1;
	if (i > 1 && i < nBins) {
	    if ((hUnfDataUp->GetBinContent(i-1) - hUnfDataDown->GetBinContent(i-1)) * (hUnfDataUp->GetBinContent(i+1) - hUnfDataDown->GetBinContent(i+1)) < 0) {
		sigmaMeani = 0.5*(0.5*fabs(hUnfDataUp->GetBinContent(i-1) - hUnfDataDown->GetBinContent(i-1)) + 0.5*fabs(hUnfDataUp->GetBinContent(i+1) - hUnfDataDown->GetBinContent(i+1)));
		if (name.Index("Sherpa") >= 0) sigmaMeani *= 2;
	    }
	}

	for (int j = 1; j <= nBins; ++j) {
	    double sigmaMeanj = 0.5*fabs(hUnfDataUp->GetBinContent(j) - hUnfDataDown->GetBinContent(j)); 
	    if (name.Index("Sherpa") >= 0) sigmaMeanj *= 2;
	    int signj = (hUnfDataUp->GetBinContent(j) - hUnfDataDown->GetBinContent(j) < 0) ? -1 : 1;
	    if (j > 1 && j < nBins) {
		if ((hUnfDataUp->GetBinContent(j-1) - hUnfDataDown->GetBinContent(j-1)) * (hUnfDataUp->GetBinContent(j+1) - hUnfDataDown->GetBinContent(j+1)) < 0) {
		    sigmaMeanj = 0.5*(0.5*fabs(hUnfDataUp->GetBinContent(j-1) - hUnfDataDown->GetBinContent(j-1)) + 0.5*fabs(hUnfDataUp->GetBinContent(j+1) - hUnfDataDown->GetBinContent(j+1)));
		    if (name.Index("Sherpa") >= 0) sigmaMeanj *= 2;
		}
	    }

	    double correlation = (i == j) ? 1 : 1;
	    h->SetBinContent(i, j, correlation * signi * signj * sigmaMeani * sigmaMeanj);
	}
    }

    return h;
}

TH1D* makeCrossSectionHist(TH1D* hGenDYJets, double integratedLumi)
{
    TH1D *hGenCrossSection = (TH1D*) hGenDYJets->Clone();
    
    if (integratedLumi > 0){
        hGenCrossSection->Scale(1./integratedLumi);
        
        int nBins = hGenCrossSection->GetNbinsX();
        for (int i = 1; i <= nBins; ++i) {
            double binWidth = hGenCrossSection->GetBinWidth(i);
            hGenCrossSection->SetBinContent(i, hGenCrossSection->GetBinContent(i)*1./binWidth);
            hGenCrossSection->SetBinError(i, hGenCrossSection->GetBinError(i)*1./binWidth);
        }
    }
    //--- divide by luminosity ---

    return hGenCrossSection;
}

TH1D* foldUnfData(TH1 *hUnfData, RooUnfoldResponse *hresp)
{
    TH1D *hfoldUnfData = (TH1D*) hUnfData->Clone();
    RooUnfoldResponse *resp = (RooUnfoldResponse*) hresp->Clone();
    TH2D *hres = (TH2D*) resp->Hresponse();
    TH1D *hgen = (TH1D*) resp->Htruth();


    int nBins = hfoldUnfData->GetNbinsX();

    for (int i = 0; i <= nBins; ++i) {
	double totGen = hgen->GetBinContent(i);
	for (int j = 0; j <= nBins; ++j) {
	    if (totGen != 0.0) {
		hres->SetBinContent(j, i, hres->GetBinContent(j, i)/totGen);
		hres->SetBinError(j, i, hres->GetBinError(j, i)/totGen);
		hres->SetBinError(j, i, 0.0);
	    }
	    else {
		hres->SetBinContent(i, j, 0);
		hres->SetBinError(i, j, hres->GetBinError(i, j));
		hres->SetBinError(i, j, 0.0);
	    }
	}
    }

    for (int i = 0; i <= nBins; ++i) {
	double sum = 0.0;
	double error2 = 0.0;
	for (int j = 0; j <= nBins; ++j) {
	    sum += hres->GetBinContent(i, j) * hUnfData->GetBinContent(j);
	    error2 += pow(hres->GetBinContent(i, j) * hUnfData->GetBinError(j), 2);
	}
	hfoldUnfData->SetBinContent(i, sum);
	hfoldUnfData->SetBinError(i, sqrt(error2));
    }
    return hfoldUnfData;
}


void test()
{
    TH1D *hreco = new TH1D("hreco", "hreco", 2, 0, 2);
    TH1D *hgen = new TH1D("hgen", "hgen", 2, 0, 2);
    TH2D *hresp = new TH2D("hresp", "hresp", 2, 0, 2, 2, 0, 2);

    hreco->Sumw2();
    hgen->Sumw2();
    hresp->Sumw2();

    hreco->Fill(0.5, 3.5);
    hreco->Fill(1.5, 5);

    hgen->Fill(0.5, 4);
    hgen->Fill(1.5, 6);

    hresp->Fill(0.5, 0.5, 3);
    hresp->Fill(0.5,1.5,0.5);
    hresp->Fill(1.5,0.5,0.5);
    hresp->Fill(1.5,1.5,4.5);

    RooUnfoldResponse *resp = new RooUnfoldResponse(NULL, hgen, hresp);

    RooUnfold *RObjectForData = RooUnfold::New(RooUnfold::kBayes, resp, hreco, 3);
    TH1D *hUnfData = (TH1D*) RObjectForData->Hreco(RooUnfold::kCovariance);

    TH1D *hfoldUnfData = foldUnfData(hUnfData, resp);

    std::cout << "reco: " << hreco->GetBinContent(1) << "   " << hreco->GetBinContent(2) << std::endl; 
    std::cout << "gen : " << hgen->GetBinContent(1) << "   " << hgen->GetBinContent(2) << std::endl; 
    std::cout << "unf : " << hUnfData->GetBinContent(1) << "   " << hUnfData->GetBinContent(2) << std::endl; 
    std::cout << "fol : " << hfoldUnfData->GetBinContent(1) << "   " << hfoldUnfData->GetBinContent(2) << std::endl; 
}

#ifdef NEW_CHI2
double MyChi2Test(TH1 *h1, TH1 *h2, int nBinsToSkip, Double_t* res)
{
    TH1 *h1Copy = (TH1*) h1->Clone();
    TH1 *h2Copy = (TH1*) h2->Clone();
    int nbins = h1->GetNbinsX();
    double chi2 = 0;
    int j = 0;
    for (int i = nBinsToSkip + 1; i <= nbins; ++i) {
      double n1 = h1Copy->GetBinContent(i);
      double n2 = h2Copy->GetBinContent(i);
      double s2 = h2Copy->GetBinError(i);
      if(n1 > 0 && n2 > 0){
	res[j] = (n2-n1) / s2;
	chi2 += res[j]*res[j];
      } else{
	res[j] = 0.;
      }
      ++j;
    }
    return chi2 / nbins;
}
#else
double MyChi2Test(TH1 *h1, TH1 *h2, int nBinsToSkip, Double_t* res)
{
    TH1 *h1Copy = (TH1*) h1->Clone();
    TH1 *h2Copy = (TH1*) h2->Clone();
    
    for (int i = 0; i <= nBinsToSkip; ++i) {
	h1Copy->SetBinContent(i, 0.0);
	h2Copy->SetBinContent(i, 0.0);
    }
    return h1Copy->Chi2Test(h2Copy, "WW,P,CHI2/NDF", res);
}
#endif

/** Correct data histogram for signal purity
 * @param hRecData histogram to correct
 * @param hPurity histogram containing the signal purities
 */
void RemoveFakes(TH1* hRecData, TH1* hFakes, TH1* hPurity){
    int fakeMethod = cfg.getI("fakeMethod");
    static bool emitMess = true;
    if(emitMess){
	std::cout << "Fake subtraction method id: " << fakeMethod << "\n";
	emitMess = false;
    }
    switch(fakeMethod){
	case 0: //global rescale to data
            hRecData->Add(hFakes, -1);
	    return;
	case 1: //bin-by-bin purity method
	    for(int ibin = 0; ibin <= hRecData->GetNbinsX(); ++ibin){
		double c = hRecData->GetBinContent(ibin);
		double e = hRecData->GetBinError(ibin);
		double p = hPurity->GetBinContent(ibin);
		//	if(ibin<4) std::cout <<  __FILE__ << __LINE__ << ": p(" << ibin
		//		     << ") = " << p << "\n";
		hRecData->SetBinContent(ibin, c * p);
		hRecData->SetBinError(ibin, e * p);
	    }
	    return;
	default:
	    std::cerr << __FILE__ <<  ":" << __LINE__ << ". Value " << fakeMethod
		      << " for parameter fakeMethod " << " is not supported. Exists.\n";
	    exit(1);
	}
}

void plotRespMat(TH2D *hResp, TString variable, TString unfoldDir){

    //Generates a pdf of the TH2 response matrix used for creating the RooUnfoldResponse object that goes into unfolding   
    //This TH2 is related to the hresponse histos   

    TString outputFileName = unfoldDir;
    outputFileName += "ResponseMatrix_" + variable;
    
    //    for (int i = 1; i < 200; i++){
    //        cout << hResp->GetBinContent(i,40) << endl;
    //    }
    
    
    TH2D *hNormResp = (TH2D*) hResp->Clone();
    normalizeTH2D(hNormResp);
    
    
    gStyle->SetOptStat(0);
    gStyle->SetPaintTextFormat("4.0f");
   
    //andrew
    gStyle->SetHistMinimumZero();   
    
    
    string YTitle = hNormResp->GetYaxis()->GetTitle();  YTitle = "gen " + YTitle;
    string XTitle = hNormResp->GetYaxis()->GetTitle();  XTitle = "reco " + XTitle;
    
    
    
    //    string respTitle = hNormResp->GetTitle();
    //    string niceTitle = respTitle.substr(0, respTitle.find("#right"));
    //    niceTitle = niceTitle.substr(niceTitle.find("Response")+9);
    
    hNormResp->GetYaxis()->SetTitle(YTitle.c_str());
    hNormResp->GetYaxis()->SetTitleSize(0.04);
    hNormResp->GetYaxis()->SetTitleOffset(1.10);
    hNormResp->GetXaxis()->SetTitle(XTitle.c_str());
    hNormResp->GetXaxis()->SetTitleSize(0.04);
    hNormResp->GetXaxis()->SetTitleOffset(1.10);
    hNormResp->GetZaxis()->SetTitle("");
    //hNormResp->GetZaxis()->SetRangeUser(0,100);
    //hNormResp->GetZaxis()->SetRangeUser(0,2300e+3);
    
    
    TCanvas *canRespMatrix = new TCanvas("s", "s", 600, 600);
    canRespMatrix->cd();
    

    TPad *pad1 = new TPad("a", "a", 0.05, 0.05, 0.95, 0.9);
    //    pad1->SetTopMargin(0.11);
    //    pad1->SetBottomMargin(0);
    pad1->SetRightMargin(0.15);
    //    pad1->SetLeftMargin(0.11);
    
    pad1->SetTicks();
    pad1->Draw();
    pad1->cd();
    //pad1->SetLogz();
    if (variable != "ZNGoodJetsFull_Zexc"){
        pad1->SetLogy();
        pad1->SetLogx();
    }
    
    pad1->cd();
    //hNormResp->SetContour(90);
    //    hNormResp->DrawCopy();
    hNormResp->DrawCopy("colz2");
    //hNormResp->DrawCopy("LEGO2Z 0");
    //hNormResp->DrawCopy("hist");
    hNormResp->DrawCopy("TEXT SAME");
    pad1->Draw();
    
    canRespMatrix->Print(outputFileName + ".pdf");
    
    
}

void normalizeTH2D(TH2D *h)
{
    int xbin(h->GetNbinsX()), ybin(h->GetNbinsY());
    for (int i(0); i <= ybin + 1; i++){
        double sum(0.);
        for (int j(0); j <= xbin + 1; j++){
            sum += h->GetBinContent(j,i);
        }
        for (int j(0); j <= xbin + 1; j++){
            if (sum != 0) h->SetBinContent(j, i, h->GetBinContent(j, i)*100 / sum );
        }
    }
}
