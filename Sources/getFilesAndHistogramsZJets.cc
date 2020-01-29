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

TFile* getFile(TString histoDir, TString lepSel, TString energy, TString Name, int jetPtMin, int jetEtaMax, TString closureTest, TString syst)
{

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

void getFiles(TString histoDir, TFile *Files[], TString lepSel, TString energy, TString Name, int jetPtMin, int jetEtaMax)
{

    vector<TString> Syst;

    // ==============================================================
    
    //    //Full systematics
    //     if (Name.Index("Data") >= 0 ) { // for data we have:
    //         Syst.push_back("0");                 //   0: central
    //         Syst.push_back("2_Up");              //   2 up: JES up
    //         Syst.push_back("2_Down");            //   2 down: JES down
    //     }
    //     else if (Name.Index("UNFOLDING") >= 0 && /*Name.Index("DYJets") >= 0 &&*/ Name.Index("Tau") < 0) {
    //         // for DYJets in case of Z+Jets or for WJets in case of W+Jets analysis we have:
    //         Syst.push_back("0");         // 0: central
    //         Syst.push_back("1_Up");      // 1 up: PU up
    //         Syst.push_back("1_Down");    // 1 down: PU down
    //         Syst.push_back("4_Up");      // 4 up: JER up
    //         Syst.push_back("4_Down");    // 4 down: JER down
    //         Syst.push_back("5_Up");      // 5 up: LepSF up
    //         Syst.push_back("5_Down");    // 5 down: LepSF down
    //         Syst.push_back("6_Up");      // 6 up: BtagSF up
    //         Syst.push_back("6_Down");    // 6 down: BtagSF down
    //     }
    //     else { // for background we have
    //         Syst.push_back("0");         // 0: central
    //         Syst.push_back("1_Up");      // 1 up: PU up
    //         Syst.push_back("1_Down");    // 1 down: PU down
    //         Syst.push_back("3_Up");      // 3 up: XSec up
    //         Syst.push_back("3_Down");    // 3 down: Xsec down
    //         Syst.push_back("6_Up");      // 6 up: BtagSF up
    //         Syst.push_back("6_Down");    // 6 down: BtagSF down
    //     };

    // ==============================================================

    // //1=PU, 2=JES, 3=XSEC, 4=JER, 6=BtagSF
    // if (Name.Index("Data") >= 0 ) { // for data we have:
    //     Syst.push_back("0");                 //   0: central
    //     Syst.push_back("2_Up");              //   2 up: JES up
    //     Syst.push_back("2_Down");            //   2 down: JES down
    // }
    // else if (Name.Index("UNFOLDING") >= 0 && /*Name.Index("DYJets") >= 0 &&*/ Name.Index("Tau") < 0) {
    //     // for DYJets in case of Z+Jets or for WJets in case of W+Jets analysis we have:
    //     Syst.push_back("0");         // 0: central
    //     Syst.push_back("1_Up");      // 1 up: PU up
    //     Syst.push_back("1_Down");    // 1 down: PU down
    //     Syst.push_back("4_Up");      // 4 up: JER up
    //     Syst.push_back("4_Down");    // 4 down: JER down
    //     Syst.push_back("0");      // 5 up: LepSF up
    //     Syst.push_back("0");    // 5 down: LepSF down
    //     Syst.push_back("6_Up");      // 6 up: BtagSF up
    //     Syst.push_back("6_Down");    // 6 down: BtagSF down
    // }
    // else { // for background we have
    //     Syst.push_back("0");         // 0: central
    //     Syst.push_back("1_Up");      // 1 up: PU up
    //     Syst.push_back("1_Down");    // 1 down: PU down
    //     Syst.push_back("3_Up");      // 3 up: XSec up
    //     Syst.push_back("3_Down");    // 3 down: Xsec down
    //     Syst.push_back("6_Up");      // 6 up: BtagSF up
    //     Syst.push_back("6_Down");    // 6 down: BtagSF down
    // };

    // ==============================================================

    // No systematics
    if (Name.Index("Data") >= 0) { // for data we have:
       Syst.push_back("0");                 //   0: central
       Syst.push_back("0");              //   2 up: JES up
       Syst.push_back("0");            //   2 down: JES down
    }
    else if (Name.Index("UNFOLDING") >= 0 && /*Name.Index("DYJets") >= 0 &&*/ Name.Index("Tau") < 0) {
       // for DYJets in case of Z+Jets or for WJets in case of W+Jets analysis we have:
       Syst.push_back("0");         // 0: central
       Syst.push_back("0");      // 1 up: PU up
       Syst.push_back("0");    // 1 down: PU down
       Syst.push_back("0");      // 4 up: JER up
       Syst.push_back("0");      // 4 down: JER down
       Syst.push_back("0");      // 5 up: LepSF up
       Syst.push_back("0");    // 5 down: LepSF down
       Syst.push_back("0");      // 6 up: BtagSF up
       Syst.push_back("0");    // 6 down: BtagSF down
    }
    else { // for background we have
       Syst.push_back("0");         // 0: central
       Syst.push_back("0");      // 1 up: PU up
       Syst.push_back("0");    // 1 down: PU down
       Syst.push_back("0");      // 3 up: XSec up
       Syst.push_back("0");    // 3 down: Xsec down
       Syst.push_back("0");      // 6 up: BtagSF up
       Syst.push_back("0");    // 6 down: BtagSF down
    };

    // ==============================================================

    
    //--- determnie how many files we have and open them all ---
    int nSyst(Syst.size());
    std::cout << "Size of nSyst: " << nSyst << std::endl;
    for (int i(0); i < nSyst; i++) {
        //std::cout << "Syst[i]: " << Syst[i] << std::endl;
        Files[i] = getFile(histoDir, lepSel, energy, Name, jetPtMin, jetEtaMax, "", Syst[i]);
    }
    //----------------------------------------------------------
}

void getAllFiles(TString histoDir, TString lepSel, TString energy, int jetPtMin, int jetEtaMax, TFile *fData[3], TFile *fDYJets[9], TFile *fBg[][7], int nBg)
{
    //--- Open data files ---------------------------------------------------------------------- 
    std::cout << "\nOpening data files: " << Samples[DATA].name << std::endl;
    getFiles(histoDir, fData, lepSel, energy, Samples[DATA].name, jetPtMin, jetEtaMax); 
    //------------------------------------------------------------------------------------------ 

    //--- Open DYJets files --------------------------------------------------------------------
    std::cout << "\nOpening signal files: " << Samples[DYJETS].name << std::endl;
    getFiles(histoDir, fDYJets, lepSel, energy, Samples[DYJETS].name, jetPtMin, jetEtaMax); 
    //------------------------------------------------------------------------------------------ 

    //--- Open Bg files ------------------------------------------------------------------------
    std::cout << "\nOpening BG files" << std::endl;
    for (unsigned short iBg = 0; iBg < nBg; ++iBg) {
	assert(iBg+1 < (int)NFILESDYJETS);
	int j = FilesDYJets[iBg+1];
	assert(j < NSamples);
	// std::cout << "-----> "  << __FILE__ << ":" << __LINE__ << ".  Includes background from " << Samples[j].name << "\n";
    std::cout << "Includes background from " << Samples[j].name << "\n";
        getFiles(histoDir, fBg[iBg], lepSel, energy, Samples[j].name, jetPtMin, jetEtaMax);
    }
    //------------------------------------------------------------------------------------------ 
}

void closeFile(TFile *File)
{
    if (File) {
        if (File->IsOpen()) File->Close();
        cout << "Closing: " << File->GetName() << "   --->   Closed ? " << (!(File->IsOpen())) << endl;
    }
}

void closeFiles(TFile *Files[])
{
    if (Files[0]) {
        TString fileName = gSystem->BaseName(Files[0]->GetName());
        int nFiles;
        if (fileName.Index("Data") >= 0 || fileName.Index("data") >= 0 || fileName.Index("DATA") >= 0) {
            nFiles = 3; 
        }
	else if (/*fileName.Index("DYJets") >= 0 &&*/ fileName.Index("UNFOLDING") >=0 && fileName.Index("Tau") < 0){
            nFiles = 9;
        }
        else nFiles = 7; 

        for (int i(0); i < nFiles; i++){
            Files[i]->cd();
            closeFile(Files[i]);
        }
    }
}

void closeFiles(TFile *Files[], int nFiles)
{
    TString fileName = gSystem->BaseName(Files[0]->GetName());
    for (int i(0); i < nFiles; i++){
        Files[i]->cd();
        closeFile(Files[i]);
        cout << "Closing file: " << Files[i]->GetName() << "   --->   Closed ? " << (!(Files[i]->IsOpen())) << endl;
    }
}

void closeAllFiles(TFile *fData[3], TFile *fDYJets[9], TFile *fBg[][7], int nBg)
{
    //--- Close data files ---------------------------------------------------------------------
    closeFiles(fData);
    //------------------------------------------------------------------------------------------ 

    //--- Close DYJets files -------------------------------------------------------------------
    closeFiles(fDYJets);
    //------------------------------------------------------------------------------------------ 

    //--- Close Bg files -----------------------------------------------------------------------
    for (unsigned short iBg = 0; iBg < nBg; ++iBg) {
        closeFiles(fBg[iBg]);
    }
    //----------------------------------l------------------------------------------------------- 
}

//////////////////////////////////////////////////////////////////////////////////////////////////

void setNegBinZero(TH2D *histograms2D)
{
    int nBinsX(histograms2D->GetNbinsX());
    int nBinsY(histograms2D->GetNbinsY());
    
    for (int i = 1; i <= nBinsX; i++){
        for(int j = 1; j <= nBinsY; j++){
            if ( histograms2D->GetBinContent(i,j) < 0 ){
                histograms2D->SetBinContent(i, j, 0.);
                histograms2D->SetBinError(i, j, 0.);
            }
        }
    }
    
}

//////////////////////////////////////////////////////////////////////////////////////////////////

TH1D* getHisto(TFile *File, const TString variable)
{
    TH1D *histo = (TH1D*) File->Get(variable);

    //---- ttbar scaling (starts here) ----------------------------------------------------------
	TString fileName = gSystem->BaseName(File->GetName());
	bool isTTbar = (fileName.Index("SMu_13TeV_TT") >= 0);

        //andrew -- ttBar SFs for 2016 data added -- 3 May 2019
        std::cout << "isTTbar = " << isTTbar << std::endl;
	if (isTTbar){
                std::cout << " ----- Scaling ttbar BG for " << variable << "!" << std::endl;
                // inclusive distributions
            if (variable.Index("Zinc2jet") >= 0) histo->Scale(1.04904374);
            else if (variable.Index("Zinc3jet") >= 0) histo->Scale(0.99624372);
            else if (variable.Index("Zinc4jet") >= 0) histo->Scale(0.97243212);
            else if (variable.Index("Zinc5jet") >= 0) histo->Scale(0.94750793);
            else if (variable.Index("Zinc6jet") >= 0) histo->Scale(0.92506162);
                // exclusive distributions
		else if (variable.Index("Zexc2jet") >= 0) histo->Scale(1.28532061);
		else if (variable.Index("Zexc3jet") >= 0) histo->Scale(1.02880218);
		else if (variable.Index("Zexc4jet") >= 0) histo->Scale(0.98994466);
		else if (variable.Index("Zexc5jet") >= 0) histo->Scale(0.95915763);
		else if (variable.Index("Zexc6jet") >= 0) histo->Scale(0.93695869);
                // jet multiplicity distributions
		else if (variable.Index("ZNGoodJets_Zinc") >= 0 || variable.Index("ZNGoodJetsFull_Zinc") >= 0) {
			histo->SetBinContent(3, histo->GetBinContent(3)*1.04904374);
			histo->SetBinContent(4, histo->GetBinContent(4)*0.99624372);
			histo->SetBinContent(5, histo->GetBinContent(5)*0.97243212);
			histo->SetBinContent(6, histo->GetBinContent(6)*0.94750793);
			histo->SetBinContent(7, histo->GetBinContent(7)*0.92506162);
			
			histo->SetBinError(3, histo->GetBinError(3)*1.04904374);
			histo->SetBinError(4, histo->GetBinError(4)*0.99624372);
			histo->SetBinError(5, histo->GetBinError(5)*0.97243212);
			histo->SetBinError(6, histo->GetBinError(6)*0.94750793);
			histo->SetBinError(7, histo->GetBinError(7)*0.92506162);
		}
		else if (variable.Index("ZNGoodJets_Zexc") >= 0 || variable.Index("ZNGoodJetsFull_Zexc") >= 0) {
			histo->SetBinContent(3, histo->GetBinContent(3)*1.28532061);
			histo->SetBinContent(4, histo->GetBinContent(4)*1.02880218);
			histo->SetBinContent(5, histo->GetBinContent(5)*0.98994466);
			histo->SetBinContent(6, histo->GetBinContent(6)*0.95915763);
			histo->SetBinContent(7, histo->GetBinContent(7)*0.93695869);
			
			histo->SetBinError(3, histo->GetBinError(3)*1.28532061);
			histo->SetBinError(4, histo->GetBinError(4)*1.02880218);
			histo->SetBinError(5, histo->GetBinError(5)*0.98994466);
			histo->SetBinError(6, histo->GetBinError(6)*0.95915763);
			histo->SetBinError(7, histo->GetBinError(7)*0.93695869);
		}
	}	
		
    //---- ttbar scaling (ends here) ----------------------------------------------------------

    //We had for an unknown reason some histo with the CanExtendAxis option active,
    //which causes troubles for the unfolding code: setting the overflow bin of
    //fake histogram was doubling the number of bins. Following line ensures
    //that the option is disabled:
    if(histo) histo->SetCanExtend(kFALSE);
    if(histo) histo->SetDirectory(0);
    return histo;
}

void getHistos(TH1D *histograms[], TFile *Files[], TString variable)
{

    TString fileName = gSystem->BaseName(Files[0]->GetName());
    bool isData = (fileName.Index("Data") >= 0 || fileName.Index("data") >= 0 || fileName.Index("DATA") >= 0);
    bool isSignal = (/*fileName.Index("DYJets") >= 0 &&*/ fileName.Index("UNFOLDING") >=0 && fileName.Index("Tau") < 0);
    bool isTTbar = (fileName.Index("SMu_13TeV_TT") >= 0);  //needed for ttbar scaling 

    int nFiles = 0;
    if (fileName.Index("Data") >= 0 || fileName.Index("data") >= 0 || fileName.Index("DATA") >= 0) nFiles = 3;
    else if (/*fileName.Index("DYJets") >= 0 &&*/ fileName.Index("UNFOLDING") >=0 && fileName.Index("Tau") < 0) nFiles = 9;
    else nFiles = 7; 

    //--- grabbing histograms from files
    // std::cout << "Getting histo for variable " << variable << "!" << std::endl;
    for (int i(0); i < nFiles; i++){
        if(Files[i] == 0) abort();
        Files[i]->cd();
        std::cout << "#" << i << ": Doing filename " << Files[i]->GetName() << std::endl;
        histograms[i] = (TH1D*) Files[i]->Get(variable);	

        //--- ttbar rescaling
        //andrew -- ttBar SFs for 2016 data added -- 3 May 2019
	    if (isTTbar){
            std::cout << " ----- Scaling ttbar BG for " << variable << "!" << std::endl;

            // inclusive distributions
            if (variable.Index("Zinc2jet") >= 0) histograms[i]->Scale(1.04904374);
            else if (variable.Index("Zinc3jet") >= 0) histograms[i]->Scale(0.99624372);
            else if (variable.Index("Zinc4jet") >= 0) histograms[i]->Scale(0.97243212);
            else if (variable.Index("Zinc5jet") >= 0) histograms[i]->Scale(0.94750793);
            else if (variable.Index("Zinc6jet") >= 0) histograms[i]->Scale(0.92506162);

            // exclusive distributions
            else if (variable.Index("Zexc2jet") >= 0) histograms[i]->Scale(1.28532061);
            else if (variable.Index("Zexc3jet") >= 0) histograms[i]->Scale(1.02880218);
            else if (variable.Index("Zexc4jet") >= 0) histograms[i]->Scale(0.98994466);
            else if (variable.Index("Zexc5jet") >= 0) histograms[i]->Scale(0.95915763);
            else if (variable.Index("Zexc6jet") >= 0) histograms[i]->Scale(0.93695869);

            // jet multiplicity distributions
		    else if (variable.Index("ZNGoodJets_Zinc") >= 0 || variable.Index("ZNGoodJetsFull_Zinc") >= 0) {

                histograms[i]->SetBinContent(3, histograms[i]->GetBinContent(3)*1.04904374);
                histograms[i]->SetBinContent(4, histograms[i]->GetBinContent(4)*0.99624372);
                histograms[i]->SetBinContent(5, histograms[i]->GetBinContent(5)*0.97243212);
                histograms[i]->SetBinContent(6, histograms[i]->GetBinContent(6)*0.94750793);
                histograms[i]->SetBinContent(7, histograms[i]->GetBinContent(7)*0.92506162);
                
                histograms[i]->SetBinError(3, histograms[i]->GetBinError(3)*1.04904374);
                histograms[i]->SetBinError(4, histograms[i]->GetBinError(4)*0.99624372);
                histograms[i]->SetBinError(5, histograms[i]->GetBinError(5)*0.97243212);
                histograms[i]->SetBinError(6, histograms[i]->GetBinError(6)*0.94750793);
                histograms[i]->SetBinError(7, histograms[i]->GetBinError(7)*0.92506162);
		    }
		    else if (variable.Index("ZNGoodJets_Zexc") >= 0 || variable.Index("ZNGoodJetsFull_Zexc") >= 0) {

                histograms[i]->SetBinContent(3, histograms[i]->GetBinContent(3)*1.28532061);
                histograms[i]->SetBinContent(4, histograms[i]->GetBinContent(4)*1.02880218);
                histograms[i]->SetBinContent(5, histograms[i]->GetBinContent(5)*0.98994466);
                histograms[i]->SetBinContent(6, histograms[i]->GetBinContent(6)*0.95915763);
                histograms[i]->SetBinContent(7, histograms[i]->GetBinContent(7)*0.93695869);
                
                histograms[i]->SetBinError(3, histograms[i]->GetBinError(3)*1.28532061);
                histograms[i]->SetBinError(4, histograms[i]->GetBinError(4)*1.02880218);
                histograms[i]->SetBinError(5, histograms[i]->GetBinError(5)*0.98994466);
                histograms[i]->SetBinError(6, histograms[i]->GetBinError(6)*0.95915763);
                histograms[i]->SetBinError(7, histograms[i]->GetBinError(7)*0.93695869);
		    }
		
	    }//end ttbar rescaling section	

        //We had for an unknown reason some histo with the CanExtendAxis option active,
        //which causes troubles for the unfolding code: setting the overflow bin of
        //fake histogram was doubling the number of bins. Following line ensures
        //that the option is disabled:
        if(histograms[i]) histograms[i]->SetCanExtend(kFALSE);

    }//end loop over files

    //--- Scaling some histos for lumi statistics, SF statistics
    if (!isData && histograms[0]) {
        Files[0]->cd();
        //--- From central histograms, we simulate the histograms
        //    for lumi up and down systematics. It is just a rescaliing
        //    since it is a global effect. 
        double lumiErr = cfg.getD("lumiUnc");
        std::cout << "lumiErr = " << lumiErr << std::endl;
        if (isSignal) {
            //--- lumi scale up ---
            histograms[9] = (TH1D*) histograms[0]->Clone();
            histograms[9]->Scale(1. + lumiErr);
            //--- lumi scale down ---
            histograms[10] = (TH1D*) histograms[0]->Clone();
            histograms[10]->Scale(1. - lumiErr);
        }
        else{
            //--- lumi scale up ---
            histograms[7] = (TH1D*) histograms[0]->Clone();
            histograms[7]->Scale(1. + lumiErr);
            //--- lumi scale down ---
            histograms[8] = (TH1D*) histograms[0]->Clone();
            histograms[8]->Scale(1. - lumiErr);
        }

        //--- From central histograms, we simulate the histograms
        //    for scale factors up and down systematics. It is just 
        //    a rescaling since the errors are global. The error 
        //    is different for the two channels and are estimated to
        //    2.5% for muons and 0.5% for electron.
        //    This should not be applied to gen histograms however.
        //    That is why errSF = 0 when variable contains "gen"
	    double muEffUnc = cfg.getD("muEffUnc");
        double errSF = muEffUnc;
        std::cout << "errSF = " << errSF << std::endl;
        //Scaling for lepton SFs should only be done for reco MC
        if (variable.Index("gen") < 0) { //will enter brackets variable does not contain "gen"
            if (isSignal) {
                //--- SF up ---
                histograms[11] = (TH1D*) histograms[0]->Clone();
                histograms[11]->Scale(1. + errSF);
                //--- SF down ---
                histograms[12] = (TH1D*) histograms[0]->Clone();
                histograms[12]->Scale(1. - errSF);
            }
            else {
                //--- SF up ---
                histograms[9] = (TH1D*) histograms[0]->Clone();
                histograms[9]->Scale(1. + errSF);
                //--- SF down ---
                histograms[10] = (TH1D*) histograms[0]->Clone();
                histograms[10]->Scale(1. - errSF);
            }
        }
    }//end global scaling for systematics section

}

void getHistos(TH2D *histograms[], TFile *Files[], TString variable)
{
    TString fileName = gSystem->BaseName(Files[0]->GetName());
    bool isData = (fileName.Index("Data") >= 0 || fileName.Index("data") >= 0 || fileName.Index("DATA") >= 0);
    bool isSignal = (/*fileName.Index("DYJets") >= 0 &&*/ fileName.Index("UNFOLDING") >=0 && fileName.Index("Tau") < 0);

    int nFiles = 0;
    if (fileName.Index("Data") >= 0 || fileName.Index("data") >= 0 || fileName.Index("DATA") >= 0) {
        nFiles = 3; 
    }
    else if (/*fileName.Index("DYJets") >= 0 &&*/ fileName.Index("UNFOLDING") >=0 && fileName.Index("Tau") < 0){
        nFiles = 9;
    }
    else nFiles = 7; 

    std::cout << "Getting histo for variable " << variable << "!" << std::endl;
    for (unsigned short i = 0; i < nFiles; i++){
        Files[i]->cd();
        std::cout << "#" << i << ": Doing filename " << Files[i]->GetName() << std::endl;
        histograms[i] = (TH2D*) Files[i]->Get(variable);
        if (variable.Index("hresponse") >= 0 && fileName.Index("UNFOLDING") >=0 && fileName.Index("Tau") < 0){
            // for Signal MC response matrix set negative bin to zero
            setNegBinZero(histograms[i]);
        }
    } 

    if (!isData && histograms[0]) {
        Files[0]->cd();
        //--- From central histograms, we simulate the histograms
        //    for lumi up and down systematics. It is just a rescaliing
        //    since it is a global effect. The error is estimated to
        //    2.6% for 8 TeV.
        double lumiErr = cfg.getD("lumiUnc");
        std::cout << "lumiErr = " << lumiErr << std::endl;
        if (isSignal) {
            //--- lumi scale up ---
            histograms[9] = (TH2D*) histograms[0]->Clone();
            histograms[9]->Scale(1. + lumiErr);
            //--- lumi scale down ---
            histograms[10] = (TH2D*) histograms[0]->Clone();
            histograms[10]->Scale(1. - lumiErr);
        }
        else{
            //--- lumi scale up ---
            histograms[7] = (TH2D*) histograms[0]->Clone();
            histograms[7]->Scale(1. + lumiErr);
            //--- lumi scale down ---
            histograms[8] = (TH2D*) histograms[0]->Clone();
            histograms[8]->Scale(1. - lumiErr);
        }

        //--- From central histograms, we simulate the histograms
        //    for scale factors up and down systematics. It is just 
        //    a rescaliinga since the errors are global. The error 
        //    is different for the two channels and are estimated to
        //    2.5% for muons and 0.5% for electron for 8 TeV
        //    This should not be applied to gen histograms however.
	double muEffUnc = cfg.getD("muEffUnc");
        double errSF = muEffUnc;
        std::cout << "errSF = " << errSF << std::endl;
        //Scaling for lepton SFs should only be done for reco MC
        if (variable.Index("gen") < 0) {
            if (isSignal) {
                //--- SF up ---
                histograms[11] = (TH2D*) histograms[0]->Clone();
                histograms[11]->Scale(1. + errSF);
                //--- SF down ---
                histograms[12] = (TH2D*) histograms[0]->Clone();
                histograms[12]->Scale(1. - errSF);
            }
            else{
                //--- SF up ---
                histograms[9] = (TH2D*) histograms[0]->Clone();
                histograms[9]->Scale(1. + errSF);
                //--- SF down ---
                histograms[10] = (TH2D*) histograms[0]->Clone();
                histograms[10]->Scale(1. - errSF);
            }
        }
    }
}

void getAllHistos(TString variable, TH1D *hRecData[3], TFile *fData[3], TH1D *hRecDYJets[13], TH1D *hGenDYJets[11], TH2D *hResDYJets[13], TFile *fDYJets[9], TH1D *hRecBg[][11], TH1D *hRecSumBg[11], TFile *fBg[][7], int nBg, TH1D* hFakDYJets[18], TH1D *hPurityDYJets[18]){

    //--- get rec Data histograms ---
    //hRecData will contain Central, JES up/down (3 objects)
    std::cout << "\n-----> Grabbing rec data histos hRecData!" << std::endl;
    getHistos(hRecData, fData, variable);

    //--- get rec DYJets histograms ---
    //hRecDYJets will contain histos from the 9 fDYJets files, and then 4 more at the end for Lumi up/down, then SF up/down (13 objects)
    std::cout << "\n-----> Grabbing rec signal histos hRecDYJets!" << std::endl;
    getHistos(hRecDYJets, fDYJets, variable);

    //--- get gen DYJets histograms ---
    //hGenDYJets will contain histos from the 9 fDYJets files, and then 2 more at the end for Lumi up/down (11 objects)
    std::cout << "\n-----> Grabbing gen signal histos hGenDYJets!" << std::endl;
    getHistos(hGenDYJets, fDYJets, "gen" + variable);

    //--- get res DYJets histograms ---
    //hResDYJets will contain histos from the 9 fDYJets files, and then 4 more at the end for Lumi up/down, then SF up/down (13 objects)
    std::cout << "\n-----> Grabbing hresponse signal histos hResDYJets!" << std::endl;
    getHistos(hResDYJets, fDYJets, "hresponse" + variable);

    //--- get rec Bg histograms ---
    //hRecBg, hRecSumBg will contain histos from the 7 fBg files, and then 4 more at the end for Lumi up/down, then SF up/down (11 objects)
    std::cout << "\n-----> Grabbing rec BG histos hRecBg and hRecSumBg!" << std::endl;
    for (unsigned short iBg = 0; iBg < nBg; ++iBg) {
        std::cout << "\n" << __FILE__ << ":" << __LINE__ << ". variable = " << variable << " file = " << fBg[iBg][0]->GetName() << std::endl;
        getHistos(hRecBg[iBg], fBg[iBg], variable);
        for (unsigned short iSyst = 0; iSyst < 11; ++iSyst) {
            if(hRecBg[iBg][iSyst] == 0){
                std::cerr << __FILE__ << ":" << __LINE__ << ". Missing histogram " << variable
          		<< ", systematic id " << iSyst << " for process with central value file " 
          		<< fBg[iBg][0]->GetName() << ". Exiting.\n";
                exit(1);
            }	  
            if (iBg == 0) hRecSumBg[iSyst] = (TH1D*) hRecBg[0][iSyst]->Clone();
            else{
                if(hRecSumBg[iSyst]->GetXaxis()->GetNbins()!=hRecBg[iBg][iSyst]->GetXaxis()->GetNbins()){
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

    //--- get fakes DYJets ---
    //there are 18 hFakDYJets objects
    std::cout << "\n-----> Grabbing fakes objects hFakDYJets!" << std::endl;
    getFakes(hFakDYJets, hRecData, hRecSumBg, hRecDYJets, hResDYJets);

    //--- get purities DYJets ---
    //there are 18 hPurityDYJets objects
    std::cout << "\n-----> Grabbing purities objects hPurityDYJets!" << std::endl;
    std::cout << "^^^ SKIPPING THIS!!! (because it doesnt look like our fake subtraction method uses it anyway)" << std::endl;
    //getPurities(hPurityDYJets, hRecData, hRecSumBg, hRecDYJets, hResDYJets);
}

TH1D* getFakes(TH1D *hRecDYJets, TH1D *hRecData, TH1D *hRecSumBg, TH2D *hResDYJets)
{
    //   printf("\n-----> Getting statistics concerning fakes:\n");
    TH1D *hFakDYJets = (TH1D*) hRecDYJets->Clone();

    //int sm= hRecDYJets->GetSumw2N();
    //int s = hResDYJets->GetSumw2N();
    int nm = hResDYJets->GetNbinsX() + 2; //number of RECO bins in response matrix (plus over- and underflow)
    int nt = hResDYJets->GetNbinsY() + 2; //number of GEN bins in response matrix (plus over- and underflow)
    //   printf("hResDYJets->GetNbinsX() + 2 = %d, hResDYJets->GetNbinsY() + 2 = %d\n", nm, nt);

    double dataIntegral = hRecData->Integral(0, hRecData->GetNbinsX()+1);
    double dyIntegral = hRecDYJets->Integral(0, hRecDYJets->GetNbinsX()+1);
    double bgIntegral = hRecSumBg->Integral(0, hRecSumBg->GetNbinsX()+1);

    double factor = dyIntegral;
    if (factor != 0) factor = (dataIntegral - bgIntegral) / factor;

    //    printf("hRecData Integral: %F, hRecDYJets Integral: %F, hRecSumBg Integral: %F\n", dataIntegral, dyIntegral, bgIntegral);
    //    printf("hRecData - (hRecDYJets+hRecSumBg): %F\n", (dataIntegral - (dyIntegral+bgIntegral)));
    //    printf("Scaling fake counts (estimated with signal MC) by following factor for data histo: %F\n", factor);
    // std::cout << std::endl;

    for (int i= 0; i<nm; i++) { //count over RECO bins
        double nmes= 0.0, wmes= 0.0;
        for (int j= 0; j<nt; j++) { //count over GEN bins
            nmes += hResDYJets->GetBinContent(i, j);
            wmes += pow(hResDYJets->GetBinError(i, j), 2);
            //if (s) wmes += pow(hResDYJets->GetBinError(i, j), 2);
        }
        // nmes is all of the entries summed up in one reco x-column over all gen y-rows
        // this would be all of the reco events that had a corresponding gen event, such that the hresponse histo was filled
        // fakes would be the number of entries in the 1D reco histogram that don't have a corresponding gen-partner in the hresponse histo
        double fake = hRecDYJets->GetBinContent(i) - nmes;
        //       printf("Integral of reco column %d in hResDYJets: %F, Corresponding bin count in hRecDYJets: %F, Estimated fakes: %F\n", i, nmes, (hRecDYJets->GetBinContent(i)), fake);
        // and now the fakes histogram is the number of fakes scaled to makeup for the discrepancy between signal and BG-subtracted data
        hFakDYJets->SetBinContent(i, factor*fake);

        //Error calculation comes from simple error propagation
	    //double err2 = pow(hRecDYJets->GetBinError(i),2) - wmes; //should be a plus here for regular error propagation
        double err2 = pow(hRecDYJets->GetBinError(i),2) + wmes;
	if(err2 < 0) {
	  err2 = 0;
	}
        hFakDYJets->SetBinError(i, sqrt (err2)); //neglecting uncertainty introduced by the scale factor
        //hFakDYJets->SetBinError   (i, sqrt (wmes + (sm ? pow(hRecDYJets->GetBinError(i),2) : hRecDYJets->GetBinContent(i))));
    }
    hFakDYJets->SetEntries(hFakDYJets->GetEffectiveEntries());  // 0 entries if 0 fakes

    return hFakDYJets;

}

void getFakes(TH1D *hFakDYJets[18], TH1D *hRecData[3], TH1D *hRecSumBg[11], TH1D *hRecDYJets[13], TH2D *hResDYJets[13])
{
    // std::cout << __FILE__ << ":" << __LINE__ << std::endl;
    hFakDYJets[0] = getFakes(hRecDYJets[0], hRecData[0], hRecSumBg[0], hResDYJets[0]);
    hFakDYJets[1] = getFakes(hRecDYJets[0], hRecData[1], hRecSumBg[0], hResDYJets[0]);
    hFakDYJets[2] = getFakes(hRecDYJets[0], hRecData[2], hRecSumBg[0], hResDYJets[0]);
    hFakDYJets[3] = getFakes(hRecDYJets[1], hRecData[0], hRecSumBg[1], hResDYJets[1]);
    hFakDYJets[4] = getFakes(hRecDYJets[2], hRecData[0], hRecSumBg[2], hResDYJets[2]);
    hFakDYJets[5] = getFakes(hRecDYJets[3], hRecData[0], hRecSumBg[0], hResDYJets[3]);
    hFakDYJets[6] = getFakes(hRecDYJets[4], hRecData[0], hRecSumBg[0], hResDYJets[4]);
    hFakDYJets[7] = getFakes(hRecDYJets[0], hRecData[0], hRecSumBg[3], hResDYJets[0]);
    hFakDYJets[8] = getFakes(hRecDYJets[0], hRecData[0], hRecSumBg[4], hResDYJets[0]);
    hFakDYJets[9] = getFakes(hRecDYJets[5], hRecData[0], hRecSumBg[0], hResDYJets[5]);
    hFakDYJets[10] = getFakes(hRecDYJets[6], hRecData[0], hRecSumBg[0], hResDYJets[6]);
    hFakDYJets[11] = getFakes(hRecDYJets[7], hRecData[0], hRecSumBg[5], hResDYJets[7]);
    hFakDYJets[12] = getFakes(hRecDYJets[8], hRecData[0], hRecSumBg[6], hResDYJets[8]);
    hFakDYJets[13] = getFakes(hRecDYJets[9], hRecData[0], hRecSumBg[7], hResDYJets[9]);
    hFakDYJets[14] = getFakes(hRecDYJets[10], hRecData[0], hRecSumBg[8], hResDYJets[10]);
    hFakDYJets[15] = getFakes(hRecDYJets[11], hRecData[0], hRecSumBg[9], hResDYJets[11]);
    hFakDYJets[16] = getFakes(hRecDYJets[12], hRecData[0], hRecSumBg[10], hResDYJets[12]);
    hFakDYJets[17] = getFakes(hRecDYJets[0], hRecData[0], hRecSumBg[0], hResDYJets[0]);
}

TH1D* getPurities(TH1D *hRecDYJets, TH1D *hRecData, TH1D *hRecSumBg, TH2D *hResDYJets)
{ 
    TH1D* hSignal = (TH1D*) hResDYJets->ProjectionY("hSignal", 0, -1, "e");
    hSignal->SetDirectory(0);
    TH1D* hPurity = (TH1D*) hRecDYJets->Clone(TString("purity") + hRecDYJets->GetName());
    hPurity->Reset();

    //hRecDYJets contains all events passing the selection cuts including "fakes"
    hPurity->Divide(hSignal, hRecDYJets, 1., 1., "B");
    
    return hPurity;
}

void getPurities(TH1D *hPurityDYJets[18], TH1D *hRecData[3], TH1D *hRecSumBg[11], TH1D *hRecDYJets[13], TH2D *hResDYJets[13])
{

    hPurityDYJets[0] = getPurities(hRecDYJets[0], hRecData[0], hRecSumBg[0], hResDYJets[0]);
    hPurityDYJets[1] = getPurities(hRecDYJets[0], hRecData[1], hRecSumBg[0], hResDYJets[0]);
    hPurityDYJets[2] = getPurities(hRecDYJets[0], hRecData[2], hRecSumBg[0], hResDYJets[0]);
    hPurityDYJets[3] = getPurities(hRecDYJets[1], hRecData[0], hRecSumBg[1], hResDYJets[1]);
    hPurityDYJets[4] = getPurities(hRecDYJets[2], hRecData[0], hRecSumBg[2], hResDYJets[2]);
    hPurityDYJets[5] = getPurities(hRecDYJets[3], hRecData[0], hRecSumBg[0], hResDYJets[3]);
    hPurityDYJets[6] = getPurities(hRecDYJets[4], hRecData[0], hRecSumBg[0], hResDYJets[4]);
    hPurityDYJets[7] = getPurities(hRecDYJets[0], hRecData[0], hRecSumBg[3], hResDYJets[0]);
    hPurityDYJets[8] = getPurities(hRecDYJets[0], hRecData[0], hRecSumBg[4], hResDYJets[0]);
    hPurityDYJets[9] = getPurities(hRecDYJets[5], hRecData[0], hRecSumBg[5], hResDYJets[5]);
    hPurityDYJets[10] = getPurities(hRecDYJets[6], hRecData[0], hRecSumBg[6], hResDYJets[6]);
    hPurityDYJets[11] = getPurities(hRecDYJets[7], hRecData[0], hRecSumBg[0], hResDYJets[7]);
    hPurityDYJets[12] = getPurities(hRecDYJets[8], hRecData[0], hRecSumBg[0], hResDYJets[8]);
    hPurityDYJets[13] = getPurities(hRecDYJets[9], hRecData[0], hRecSumBg[7], hResDYJets[9]);
    hPurityDYJets[14] = getPurities(hRecDYJets[10], hRecData[0], hRecSumBg[8], hResDYJets[10]);
    hPurityDYJets[15] = getPurities(hRecDYJets[11], hRecData[0], hRecSumBg[9], hResDYJets[11]);
    hPurityDYJets[16] = getPurities(hRecDYJets[12], hRecData[0], hRecSumBg[10], hResDYJets[12]);
    hPurityDYJets[17] = getPurities(hRecDYJets[0], hRecData[0], hRecSumBg[0], hResDYJets[0]);
}
