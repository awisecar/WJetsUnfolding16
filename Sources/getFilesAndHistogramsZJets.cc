#include <iostream>
#include <sstream>
#include <TFile.h>
#include <TString.h>
#include <TSystem.h>
#include <algorithm>
#include <iostream>
#include <cassert>
#include "getFilesAndHistogramsZJets.h"
#include "ConfigVJets.h"

using namespace std;

extern ConfigVJets cfg;

//////////////////////////////////////////////////////////////////////////////////////////////////

TFile* getFile(TString histoDir, TString lepSel, TString energy, TString Name, int jetPtMin, int jetEtaMax, TString closureTest, TString syst){

    TString fileName = histoDir; // TString to contain the name of the file
    if (!fileName.EndsWith("/")) fileName += "/";

    //--- make sure lepSel is short version ---
    if (lepSel == "Muons" || lepSel == "DMu_") lepSel = "DMu";
    else if (lepSel == "Electrons" || lepSel == "DE_") lepSel = "DE";

    fileName += lepSel + "_" + energy + "_" + Name; // update fileName with lepton information
    //-----------------------------------------------


    //--- deal with efficiency correction applied or not ---
    TString trigCorr = "0";
    if (Name.Index("Data") == 0 || energy == "8TeV" || energy == "13TeV") trigCorr = "1"; // trigger correction is applied to data and MC at 8TeV but only to data at 7TeV 

    //--- special case for the generator comparison ---
    if (Name.Index("Powheg") >= 0 || Name.Index("Sherpa") >= 0) trigCorr = "0"; 
    //-------------------------------------------------

    //--- update fileName with trigger correction ----------
    fileName += "_TrigCorr_" + trigCorr;
    //------------------------------------------------------

    //--- update fileName for a bunch of other things ---
    fileName += "_Syst_" + syst;
    fileName += "_JetPtMin_";
    fileName += jetPtMin;
    fileName += "_JetEtaMax_";
    fileName += jetEtaMax; 
    if (closureTest != "") fileName += closureTest;
    //---------------------------------------------------

    //--- fileName is complete: just add the extension and open it ---
    fileName += ".root";
    std::cout << "Opening file with name: " << fileName << std::endl; //<< "   --->   Opened ? " << File->IsOpen() << std::endl;
    TFile *File = new TFile(fileName, "READ");
    if (!File->IsOpen()) {
      std::cerr << "Please check that you produced the following file. I was not able to open it." << std::endl;
      std::cerr << "\t\033[031m " << fileName << "\033[0m " << std::endl;
      abort();
      return NULL;
    }
    else return File;
    //----------------------------------------------------------------
}

void getFiles(TString histoDir, TFile *Files[], TString lepSel, TString energy, TString Name, int jetPtMin, int jetEtaMax){

    vector<TString> Syst;

    // ==============================================================
    
   // Full systematics ---
   // Data
   if (Name.Index("Data") >= 0 ){
       Syst.push_back("0");         // 0:      Central
       Syst.push_back("2_Up");      // 2 up:   JES up
       Syst.push_back("2_Down");    // 2 down: JES down
   }
   // Signal (W+jets)
   else if (Name.Index("UNFOLDING") >= 0){
       Syst.push_back("0");         // 0:       Central
       Syst.push_back("1_Up");      // 1 up:    PU up
       Syst.push_back("1_Down");    // 1 down:  PU down
       Syst.push_back("4_Up");      // 4 up:    JER up
       Syst.push_back("4_Down");    // 4 down:  JER down
       Syst.push_back("5_Up");      // 5 up:    LepSF up
       Syst.push_back("5_Down");    // 5 down:  LepSF down
       // Syst.push_back("6_Up");      // 6 up:    BtagSF up
       // Syst.push_back("6_Down");    // 6 down:  BtagSF down
       Syst.push_back("0");      // 6 up:    BtagSF up
       Syst.push_back("0");    // 6 down:  BtagSF down
       Syst.push_back("11_Up");     // 11 up:   L1Prefire up
       Syst.push_back("11_Down");   // 11 down: L1Prefire down
   }
   // Background
   else{
       Syst.push_back("0");         // 0:       Central
       Syst.push_back("1_Up");      // 1 up:    PU up
       Syst.push_back("1_Down");    // 1 down:  PU down
       Syst.push_back("3_Up");      // 3 up:    XSEC up
       Syst.push_back("3_Down");    // 3 down:  XSEC down
       Syst.push_back("5_Up");      // 5 up:    LepSF up
       Syst.push_back("5_Down");    // 5 down:  LepSF down
       // Syst.push_back("6_Up");      // 6 up:    BtagSF up
       // Syst.push_back("6_Down");    // 6 down:  BtagSF down
       Syst.push_back("0");      // 6 up:    BtagSF up
       Syst.push_back("0");    // 6 down:  BtagSF down
       Syst.push_back("11_Up");     // 11 up:   L1Prefire up
       Syst.push_back("11_Down");   // 11 down: L1Prefire down
   };

    // ==============================================================

   //  // JES only ---
   //  // Data
   //  if (Name.Index("Data") >= 0 ){
   //      Syst.push_back("0");         // 0:      Central
   //      Syst.push_back("2_Up");      // 2 up:   JES up
   //      Syst.push_back("2_Down");    // 2 down: JES down
   //  }
   //  // Signal (W+jets)
   //  else if (Name.Index("UNFOLDING") >= 0){
   //      Syst.push_back("0");   // 0:       Central
   //      Syst.push_back("0");   // 1 up:    PU up
   //      Syst.push_back("0");   // 1 down:  PU down
   //      Syst.push_back("0");   // 4 up:    JER up
   //      Syst.push_back("0");   // 4 down:  JER down
   //      Syst.push_back("0");   // 5 up:    LepSF up
   //      Syst.push_back("0");   // 5 down:  LepSF down
   //      Syst.push_back("0");   // 6 up:    BtagSF up
   //      Syst.push_back("0");   // 6 down:  BtagSF down
   //      Syst.push_back("0");   // 11 up:   L1Prefire up
   //      Syst.push_back("0");   // 11 down: L1Prefire down
   //  }
   //  // Background
   //  else{
   //      Syst.push_back("0");   // 0:       Central
   //      Syst.push_back("0");   // 1 up:    PU up
   //      Syst.push_back("0");   // 1 down:  PU down
   //      Syst.push_back("0");   // 3 up:    XSEC up
   //      Syst.push_back("0");   // 3 down:  XSEC down
   //      Syst.push_back("0");   // 5 up:    LepSF up
   //      Syst.push_back("0");   // 5 down:  LepSF down
   //      Syst.push_back("0");   // 6 up:    BtagSF up
   //      Syst.push_back("0");   // 6 down:  BtagSF down
   //      Syst.push_back("0");   // 11 up:   L1Prefire up
   //      Syst.push_back("0");   // 11 down: L1Prefire down
   //  };

    // ==============================================================

//    // No systematics ---
//    // Data
//    if (Name.Index("Data") >= 0 ){
//        Syst.push_back("0");   // 0:      Central
//        Syst.push_back("0");   // 2 up:   JES up
//        Syst.push_back("0");   // 2 down: JES down
//    }
//    // Signal (W+jets)
//    else if (Name.Index("UNFOLDING") >= 0){
//        Syst.push_back("0");   // 0:       Central
//        Syst.push_back("0");   // 1 up:    PU up
//        Syst.push_back("0");   // 1 down:  PU down
//        Syst.push_back("0");   // 4 up:    JER up
//        Syst.push_back("0");   // 4 down:  JER down
//        Syst.push_back("0");   // 5 up:    LepSF up
//        Syst.push_back("0");   // 5 down:  LepSF down
//        Syst.push_back("0");   // 6 up:    BtagSF up
//        Syst.push_back("0");   // 6 down:  BtagSF down
//        Syst.push_back("0");   // 11 up:   L1Prefire up
//        Syst.push_back("0");   // 11 down: L1Prefire down
//    }
//    // Background
//    else{
//        Syst.push_back("0");   // 0:       Central
//        Syst.push_back("0");   // 1 up:    PU up
//        Syst.push_back("0");   // 1 down:  PU down
//        Syst.push_back("0");   // 3 up:    XSEC up
//        Syst.push_back("0");   // 3 down:  XSEC down
//        Syst.push_back("0");   // 5 up:    LepSF up
//        Syst.push_back("0");   // 5 down:  LepSF down
//        Syst.push_back("0");   // 6 up:    BtagSF up
//        Syst.push_back("0");   // 6 down:  BtagSF down
//        Syst.push_back("0");   // 11 up:   L1Prefire up
//        Syst.push_back("0");   // 11 down: L1Prefire down
//    };

    // ==============================================================
    
    // Determine how many files we have and open them all
    int nSyst(Syst.size());
    std::cout << "Size of nSyst: " << nSyst << std::endl;
    for (int i(0); i < nSyst; i++) {
        //std::cout << "Syst[i]: " << Syst[i] << std::endl;
        Files[i] = getFile(histoDir, lepSel, energy, Name, jetPtMin, jetEtaMax, "", Syst[i]);
    }
    //----------------------------------------------------------
}

void getAllFiles(TString histoDir, TString lepSel, TString energy, int jetPtMin, int jetEtaMax, TFile *fData[3], TFile *fDYJets[11], TFile *fBg[][11], int nBg){

    // Open data files ---------------------------------------------------------------------- 
    std::cout << "\nOpening data files: " << Samples[DATA].name << std::endl;
    getFiles(histoDir, fData, lepSel, energy, Samples[DATA].name, jetPtMin, jetEtaMax); 

    // Open DYJets files --------------------------------------------------------------------
    std::cout << "\nOpening signal files: " << Samples[DYJETS].name << std::endl;
    getFiles(histoDir, fDYJets, lepSel, energy, Samples[DYJETS].name, jetPtMin, jetEtaMax); 

    // Open Bg files ------------------------------------------------------------------------
    std::cout << "\nOpening BG files" << std::endl;
    for (unsigned short iBg = 0; iBg < nBg; ++iBg){
	    assert(iBg+1 < (int) NFILESDYJETS);
	    int j = FilesDYJets[iBg+1];
	    assert(j < NSamples);
        std::cout << "Includes background from " << Samples[j].name << "\n";
        getFiles(histoDir, fBg[iBg], lepSel, energy, Samples[j].name, jetPtMin, jetEtaMax);
    }
}

void closeFile(TFile *File){
    if (File){
        if (File->IsOpen()) File->Close();
        cout << "Closing: " << File->GetName() << "   --->   Closed ? " << (!(File->IsOpen())) << endl;
    }
}

void closeFiles(TFile *Files[]){
    if (Files[0]){
        TString fileName = gSystem->BaseName(Files[0]->GetName());
        int nFiles(0);
        if (fileName.Index("Data") >= 0) nFiles = 3; 
	    else if (fileName.Index("UNFOLDING") >=0) nFiles = 11;
        else nFiles = 11; 

        for (int i(0); i < nFiles; i++){
            Files[i]->cd();
            closeFile(Files[i]);
        }
    }
}

void closeFiles(TFile *Files[], int nFiles){
    TString fileName = gSystem->BaseName(Files[0]->GetName());
    for (int i(0); i < nFiles; i++){
        Files[i]->cd();
        closeFile(Files[i]);
        cout << "Closing file: " << Files[i]->GetName() << "   --->   Closed ? " << (!(Files[i]->IsOpen())) << endl;
    }
}

void closeAllFiles(TFile *fData[3], TFile *fDYJets[11], TFile *fBg[][11], int nBg){

    // Close data files
    closeFiles(fData);

    // Close DYJets files
    closeFiles(fDYJets);

    // Close Bg files
    for (unsigned short iBg = 0; iBg < nBg; ++iBg){
        closeFiles(fBg[iBg]);
    }

}

//////////////////////////////////////////////////////////////////////////////////////////////////

void setNegBinZero(TH2D *histograms2D){
    int nBinsX(histograms2D->GetNbinsX());
    int nBinsY(histograms2D->GetNbinsY());
    
    for (int i = 1; i <= nBinsX; i++){
        for (int j = 1; j <= nBinsY; j++){
            if ( histograms2D->GetBinContent(i,j) < 0. ){
                histograms2D->SetBinContent(i, j, 0.);
                histograms2D->SetBinError(i, j, 0.);
            }
        }
    }
    
}

//////////////////////////////////////////////////////////////////////////////////////////////////

TH1D* getHisto(TFile *File, const TString variable){

    TH1D *histo = (TH1D*) File->Get(variable);

    //We had for an unknown reason some histo with the CanExtendAxis option active,
    //which causes troubles for the unfolding code: setting the overflow bin of
    //fake histogram was doubling the number of bins. Following line ensures
    //that the option is disabled:
    if (histo) histo->SetCanExtend(kFALSE);
    if (histo) histo->SetDirectory(0);

    return histo;
}

void getHistos(TH1D *histograms[], TFile *Files[], TString variable, int year){

    TString fileName = gSystem->BaseName(Files[0]->GetName());
    bool isData   = (fileName.Index("Data") >= 0);
    bool isSignal = (fileName.Index("UNFOLDING") >= 0);
    bool isTTbar  = (fileName.Index("SMu_13TeV_TT_") >= 0); // for ttbar scaling 

    int nFiles(0);
    if (isData) nFiles = 3;
    else if (isSignal) nFiles = 11;
    else nFiles = 11; 

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // List ttBar SFs as arrays --
    // 2016
    float ttBarSF_2016exc[7] = {1.058563,	1.004681,	0.995014,	0.972899,	0.948714,	0.939270,	0.947502};
    float ttBarSF_2016inc[7] = {1.002029,	0.991079,	0.981770,	0.963568,	0.946061,	0.939943,	0.941690};
    // 2017
    float ttBarSF_2017exc[7] = {1.363999,	1.113553,	1.070007,	1.071000,	1.094611,	1.132046,	1.198218};
    float ttBarSF_2017inc[7] = {1.138375,	1.091542,	1.076260,	1.085047,	1.112406,	1.154916,	1.217493};
    // 2018
    float ttBarSF_2018exc[7] = {1.123496,	0.997161,	0.955899,	0.950875,	0.947921,	0.981318,	1.032589};
    float ttBarSF_2018inc[7] = {0.997299,	0.972467,	0.955640,	0.955278,	0.963813,	1.001809,	1.058206};

    // Select ttBar SF set based on year
    float ttBarSF_exc[7];
    float ttBarSF_inc[7];
    for (int i = 0; i < 7; i++){
        if (year == 2016){
            ttBarSF_exc[i] = ttBarSF_2016exc[i];
            ttBarSF_inc[i] = ttBarSF_2016inc[i];
        }
        else if (year == 2017){
            ttBarSF_exc[i] = ttBarSF_2017exc[i];
            ttBarSF_inc[i] = ttBarSF_2017inc[i];
        }
        else if (year == 2018){
            ttBarSF_exc[i] = ttBarSF_2018exc[i];
            ttBarSF_inc[i] = ttBarSF_2018inc[i];
        }
        else isTTbar = false; // just turn off scaling if year not specified
    }
    

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Getting histograms from files
    for (int i(0); i < nFiles; i++){
        if (Files[i] == 0) abort();
        Files[i]->cd();
        std::cout << "#" << i << ": Doing filename " << Files[i]->GetName() << std::endl;
        histograms[i] = (TH1D*) Files[i]->Get(variable);	

        // tt-bar rescaling
	    if (isTTbar){
            std::cout << " ----- Re-scaling ttbar BG! " << std::endl;
            std::cout << " ----- year: " << year << ", variable: " << variable << std::endl;

            // inclusive distributions
            if (variable.Index("Zinc2jet") >= 0)      histograms[i]->Scale(ttBarSF_inc[0]);
            else if (variable.Index("Zinc3jet") >= 0) histograms[i]->Scale(ttBarSF_inc[1]);
            else if (variable.Index("Zinc4jet") >= 0) histograms[i]->Scale(ttBarSF_inc[2]);
            else if (variable.Index("Zinc5jet") >= 0) histograms[i]->Scale(ttBarSF_inc[3]);
            else if (variable.Index("Zinc6jet") >= 0) histograms[i]->Scale(ttBarSF_inc[4]);
            else if (variable.Index("Zinc7jet") >= 0) histograms[i]->Scale(ttBarSF_inc[5]);
            else if (variable.Index("Zinc8jet") >= 0) histograms[i]->Scale(ttBarSF_inc[6]);
            // exclusive distributions
            else if (variable.Index("Zexc2jet") >= 0) histograms[i]->Scale(ttBarSF_exc[0]);
            else if (variable.Index("Zexc3jet") >= 0) histograms[i]->Scale(ttBarSF_exc[1]);
            else if (variable.Index("Zexc4jet") >= 0) histograms[i]->Scale(ttBarSF_exc[2]);
            else if (variable.Index("Zexc5jet") >= 0) histograms[i]->Scale(ttBarSF_exc[3]);
            else if (variable.Index("Zexc6jet") >= 0) histograms[i]->Scale(ttBarSF_exc[4]);
            else if (variable.Index("Zexc7jet") >= 0) histograms[i]->Scale(ttBarSF_exc[5]);
            else if (variable.Index("Zexc8jet") >= 0) histograms[i]->Scale(ttBarSF_exc[6]);
            // if neither
            else histograms[i]->Scale(1.0);

            // inclusive jet multiplicity distribution
		    if (variable.Index("ZNGoodJets_Zinc") >= 0){
                // bin content
                histograms[i]->SetBinContent(3, histograms[i]->GetBinContent(3)*ttBarSF_inc[0]);
                histograms[i]->SetBinContent(4, histograms[i]->GetBinContent(4)*ttBarSF_inc[1]);
                histograms[i]->SetBinContent(5, histograms[i]->GetBinContent(5)*ttBarSF_inc[2]);
                histograms[i]->SetBinContent(6, histograms[i]->GetBinContent(6)*ttBarSF_inc[3]);
                histograms[i]->SetBinContent(7, histograms[i]->GetBinContent(7)*ttBarSF_inc[4]);
                histograms[i]->SetBinContent(8, histograms[i]->GetBinContent(8)*ttBarSF_inc[5]);
                histograms[i]->SetBinContent(9, histograms[i]->GetBinContent(9)*ttBarSF_inc[6]);
                // bin error
                histograms[i]->SetBinError(3, histograms[i]->GetBinError(3)*ttBarSF_inc[0]);
                histograms[i]->SetBinError(4, histograms[i]->GetBinError(4)*ttBarSF_inc[1]);
                histograms[i]->SetBinError(5, histograms[i]->GetBinError(5)*ttBarSF_inc[2]);
                histograms[i]->SetBinError(6, histograms[i]->GetBinError(6)*ttBarSF_inc[3]);
                histograms[i]->SetBinError(7, histograms[i]->GetBinError(7)*ttBarSF_inc[4]);
                histograms[i]->SetBinError(8, histograms[i]->GetBinError(8)*ttBarSF_inc[5]);
                histograms[i]->SetBinError(9, histograms[i]->GetBinError(9)*ttBarSF_inc[6]);
		    }

            // exclusive jet multiplicity distribution
		    if (variable.Index("ZNGoodJets_Zexc") >= 0){
                // bin content
                histograms[i]->SetBinContent(3, histograms[i]->GetBinContent(3)*ttBarSF_exc[0]);
                histograms[i]->SetBinContent(4, histograms[i]->GetBinContent(4)*ttBarSF_exc[1]);
                histograms[i]->SetBinContent(5, histograms[i]->GetBinContent(5)*ttBarSF_exc[2]);
                histograms[i]->SetBinContent(6, histograms[i]->GetBinContent(6)*ttBarSF_exc[3]);
                histograms[i]->SetBinContent(7, histograms[i]->GetBinContent(7)*ttBarSF_exc[4]);
                histograms[i]->SetBinContent(8, histograms[i]->GetBinContent(8)*ttBarSF_exc[5]);
                histograms[i]->SetBinContent(9, histograms[i]->GetBinContent(9)*ttBarSF_exc[6]);
                // bin error
                histograms[i]->SetBinError(3, histograms[i]->GetBinError(3)*ttBarSF_exc[0]);
                histograms[i]->SetBinError(4, histograms[i]->GetBinError(4)*ttBarSF_exc[1]);
                histograms[i]->SetBinError(5, histograms[i]->GetBinError(5)*ttBarSF_exc[2]);
                histograms[i]->SetBinError(6, histograms[i]->GetBinError(6)*ttBarSF_exc[3]);
                histograms[i]->SetBinError(7, histograms[i]->GetBinError(7)*ttBarSF_exc[4]);
                histograms[i]->SetBinError(8, histograms[i]->GetBinError(8)*ttBarSF_exc[5]);
                histograms[i]->SetBinError(9, histograms[i]->GetBinError(9)*ttBarSF_exc[6]);
		    }
		
	    }//end ttbar rescaling section	

        //We had for an unknown reason some histo with the CanExtendAxis option active,
        //which causes troubles for the unfolding code: setting the overflow bin of
        //fake histogram was doubling the number of bins. Following line ensures
        //that the option is disabled:
        if (histograms[i]) histograms[i]->SetCanExtend(kFALSE);

    }//end loop over files

    // Cloning central histos for lumi systematics
    if (!isData && histograms[0]){
        Files[0]->cd();
        histograms[11] = (TH1D*) histograms[0]->Clone(); // lumi up
        histograms[12] = (TH1D*) histograms[0]->Clone(); // lumi down
    }

}

void getHistos(TH2D *histograms[], TFile *Files[], TString variable, int year){
    // NOTE: this function is only used to import the hresponse distributions, which exist only for signal

    TString fileName = gSystem->BaseName(Files[0]->GetName());
    bool isData = (fileName.Index("Data") >= 0);
    bool isSignal = (fileName.Index("UNFOLDING") >= 0);

    int nFiles(0);
    if (isData) nFiles = 3; 
    else if (isSignal) nFiles = 11;
    else nFiles = 11; 

    std::cout << "Getting histos for variable " << variable << "!" << std::endl;
    for (unsigned short i = 0; i < nFiles; i++){
        Files[i]->cd();
        std::cout << "#" << i << ": Doing filename " << Files[i]->GetName() << std::endl;
        histograms[i] = (TH2D*) Files[i]->Get(variable);

        // For signal MC response matrix, set negative bin counts to zero
        if (isSignal && variable.Index("hresponse") >= 0) setNegBinZero(histograms[i]);
    } 

    // Cloning central histos for lumi systematics
    if (!isData && histograms[0]){
        Files[0]->cd();
        histograms[11] = (TH2D*) histograms[0]->Clone(); // lumi up
        histograms[12] = (TH2D*) histograms[0]->Clone(); // lumi down
    }
}

void getAllHistos(TString variable, int year, TH1D *hRecData[3], TFile *fData[3], TH1D *hRecDYJets[13], TH1D *hGenDYJets[13], TH2D *hResDYJets[13], TFile *fDYJets[11], TH1D *hRecBg[][13], TH1D *hRecSumBg[13], TFile *fBg[][11], int nBg, TH1D* hFakDYJets[18], TH2D *hAccepDYJets[13], bool isClosureTest){

    // DATA -------------------------
    // hRecData will contain Central, JES up/down (3 objects)
    std::cout << "\n-----> Grabbing rec data histos hRecData!" << std::endl;
    getHistos(hRecData, fData, variable, year);

    // SIGNAL -----------------------
    // hRecDYJets will contain histos from the 11 fDYJets files, and then 2 more at the end for Lumi up/down (13 objects)
    std::cout << "\n-----> Grabbing rec signal histos hRecDYJets!" << std::endl;
    getHistos(hRecDYJets, fDYJets, variable, year);
    // hGenDYJets will contain histos from the 11 fDYJets files, and then 2 more at the end for Lumi up/down (13 objects)
    std::cout << "\n-----> Grabbing gen signal histos hGenDYJets!" << std::endl;
    getHistos(hGenDYJets, fDYJets, "gen" + variable, year);
    // hResDYJets will contain histos from the 11 fDYJets files, and then 2 more at the end for Lumi up/down (13 objects)
    std::cout << "\n-----> Grabbing hresponse signal histos hResDYJets!" << std::endl;
    getHistos(hResDYJets, fDYJets, "hresponse" + variable, year);

    // BACKGROUND -------------------
    // hRecBg, hRecSumBg will contain histos from the 11 fBg files, and then 2 more at the end for Lumi up/down (13 objects)
    std::cout << "\n-----> Grabbing rec BG histos hRecBg and hRecSumBg!" << std::endl;
    std::cout << "variable = " << variable << std::endl;
    for (unsigned short iBg = 0; iBg < nBg; ++iBg){

        std::cout << " \nfile = " << fBg[iBg][0]->GetName() << std::endl;
        getHistos(hRecBg[iBg], fBg[iBg], variable, year);

        for (unsigned short iSyst = 0; iSyst < 13; ++iSyst){
            if(hRecBg[iBg][iSyst] == 0){
                std::cerr << __FILE__ << ":" << __LINE__ << ". Missing histogram " << variable
          		<< ", systematic id " << iSyst << " for process with central value file " 
          		<< fBg[iBg][0]->GetName() << ". Exiting.\n";
                exit(1);
            }	  
            if (iBg == 0) hRecSumBg[iSyst] = (TH1D*) hRecBg[0][iSyst]->Clone();
            else{
                if (hRecSumBg[iSyst]->GetXaxis()->GetNbins() != hRecBg[iBg][iSyst]->GetXaxis()->GetNbins()){
          	        std::cerr << __FILE__ << ":" <<  __LINE__ << ". "
          		    << "Histogram " << hRecSumBg[iSyst]->GetName()
          		    << "for systematic index " << iSyst
          		    << " and background index " << iBg
          		    << " has a different bining than the background #0.\n";
                }
                hRecSumBg[iSyst]->Add(hRecBg[iBg][iSyst]);
            }
        }
    }

    // OTHER ------------------------
    // There are 18 hFakDYJets objects
    std::cout << "\n-----> Grabbing fakes objects hFakDYJets!" << std::endl;
    getAllFakes(hFakDYJets, hRecData, hRecSumBg, hRecDYJets, hResDYJets, isClosureTest);
    // There are 13 hAccepDYJets objects
    std::cout << "\n-----> Grabbing acceptance objects hAccepDYJets!" << std::endl;
    getHistos(hAccepDYJets, fDYJets, "hresponse" + variable + "_NOEFFWEIGHTS", year);

}

TH1D* getFakes(TH1D *hRecDYJets, TH1D *hRecData, TH1D *hRecSumBg, TH2D *hResDYJets, bool isVerbose, bool isClosureTest){

    // NOTE:
    // "Fakes" are the events seen at reco-level that do not originate within the fiducially-accepted region at particle-level
    // i.e. they migrate from outside the fiducial region at gen level into the fiducial region at reco-level, but they do not
    // correspond to the "true" counts, so they must be subtracted

    // Fakes distribution has reco binning
    TH1D *hFakDYJets = (TH1D*) hRecDYJets->Clone();

    // Count the #events that pass both gen and reco cuts, then compare this to the #events that pass just reco cuts
    if (isVerbose) std::cout << std::endl;
    for (int i = 1; i <= hResDYJets->GetNbinsX(); i++){ // count over RECO bins

        double nmes(0.0);
        for (int j = 1; j <= hResDYJets->GetNbinsY(); j++){ // count over GEN bins (currently don't include overflow bin)
            nmes += hResDYJets->GetBinContent(i, j);
        }

        // Fake rate is calculated using signal MC
        double fakeRate = 1. - (nmes/hRecDYJets->GetBinContent(i));
        if (isVerbose) std::cout << "Fake rate calculated for bin #" << i << ": " << fakeRate << std::endl;

        // Now get fakes distribution
        // For data, #fakes is (fake rate from signal MC) * (#events in data we expect to be signal events)
        if (!isClosureTest) hFakDYJets->SetBinContent( i, fakeRate * (hRecData->GetBinContent(i) - hRecSumBg->GetBinContent(i)) );
        else hFakDYJets->SetBinContent( i, fakeRate * hRecDYJets->GetBinContent(i) );

        // Derive the error using standard error propagation
        // We take the fake rate here as a constant without error
        double err(0.);
        if (!isClosureTest) err = sqrt( pow(hRecData->GetBinError(i), 2.) +  pow(hRecSumBg->GetBinError(i), 2.) );
        else err = hRecDYJets->GetBinError(i);
        hFakDYJets->SetBinError(i, fakeRate * err); 
    }

    hFakDYJets->SetEntries(hFakDYJets->GetEffectiveEntries());  // 0 entries if 0 fakes

    return hFakDYJets;
}

void getAllFakes(TH1D *hFakDYJets[18], TH1D *hRecData[3], TH1D *hRecSumBg[13], TH1D *hRecDYJets[13], TH2D *hResDYJets[13], bool isClosureTest){
    // NOTE: reco signal distributions and hresponse signal distributions MUST have the same index for fakes derivation
    hFakDYJets[0]  = getFakes(hRecDYJets[0],  hRecData[0], hRecSumBg[0],  hResDYJets[0],  true,  isClosureTest);
    hFakDYJets[1]  = getFakes(hRecDYJets[0],  hRecData[1], hRecSumBg[0],  hResDYJets[0],  false, isClosureTest);
    hFakDYJets[2]  = getFakes(hRecDYJets[0],  hRecData[2], hRecSumBg[0],  hResDYJets[0],  false, isClosureTest);
    hFakDYJets[3]  = getFakes(hRecDYJets[1],  hRecData[0], hRecSumBg[1],  hResDYJets[1],  false, isClosureTest);
    hFakDYJets[4]  = getFakes(hRecDYJets[2],  hRecData[0], hRecSumBg[2],  hResDYJets[2],  false, isClosureTest);
    hFakDYJets[5]  = getFakes(hRecDYJets[0],  hRecData[0], hRecSumBg[3],  hResDYJets[0],  false, isClosureTest);
    hFakDYJets[6]  = getFakes(hRecDYJets[0],  hRecData[0], hRecSumBg[4],  hResDYJets[0],  false, isClosureTest);
    hFakDYJets[7]  = getFakes(hRecDYJets[3],  hRecData[0], hRecSumBg[0],  hResDYJets[3],  false, isClosureTest);
    hFakDYJets[8]  = getFakes(hRecDYJets[4],  hRecData[0], hRecSumBg[0],  hResDYJets[4],  false, isClosureTest);
    hFakDYJets[9]  = getFakes(hRecDYJets[5],  hRecData[0], hRecSumBg[5],  hResDYJets[5],  false, isClosureTest);
    hFakDYJets[10] = getFakes(hRecDYJets[6],  hRecData[0], hRecSumBg[6],  hResDYJets[6],  false, isClosureTest);
    hFakDYJets[11] = getFakes(hRecDYJets[7],  hRecData[0], hRecSumBg[7],  hResDYJets[7],  false, isClosureTest);
    hFakDYJets[12] = getFakes(hRecDYJets[8],  hRecData[0], hRecSumBg[8],  hResDYJets[8],  false, isClosureTest);
    hFakDYJets[13] = getFakes(hRecDYJets[9],  hRecData[0], hRecSumBg[9],  hResDYJets[9],  false, isClosureTest);
    hFakDYJets[14] = getFakes(hRecDYJets[10], hRecData[0], hRecSumBg[10], hResDYJets[10], false, isClosureTest);
    hFakDYJets[15] = getFakes(hRecDYJets[11], hRecData[0], hRecSumBg[11], hResDYJets[11], false, isClosureTest);
    hFakDYJets[16] = getFakes(hRecDYJets[12], hRecData[0], hRecSumBg[12], hResDYJets[12], false, isClosureTest);
    hFakDYJets[17] = getFakes(hRecDYJets[0],  hRecData[0], hRecSumBg[0],  hResDYJets[0],  false, isClosureTest);
}
