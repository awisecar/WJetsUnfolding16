#include <iostream>
#include <TROOT.h>
#include <TString.h>
#include "ArgParser.h"
#include "ConfigVJets.h"
#include "getFilesAndHistogramsZJets.h"
#include "RecoComparison.h"
//#include "variablesOfInterest.h"

ConfigVJets cfg;

int main(int argc, char **argv)
{
    gROOT->SetBatch();
    gErrorIgnoreLevel = kError;

    //--- Loads configuration -----------------------------------------------------
    TString histoDir    = cfg.getS("histoDir");
    TString recoCompDir = cfg.getS("recoCompDir");
    TString lepSel      = cfg.getS("lepSel");
    int jetPtMin        = cfg.getI("jetPtMin");
    int jetEtaMax  = cfg.getI("jetEtaMax");
    bool doPASPlots     = cfg.getB("doPASPlots");
    
    //-----------------------------------------------------------------------------

    //--- Parse the arguments -----------------------------------------------------
    if (argc > 1) {
        for (int i = 1; i < argc; ++i) {
            TString currentArg = argv[i];
            //--- possible options ---
            if (currentArg.BeginsWith("histoDir=")) {
                getArg(currentArg, histoDir);
		cfg.set("histoDir", histoDir);
            }
            else if (currentArg.BeginsWith("recoCompDir=")) {
                getArg(currentArg, recoCompDir);
		cfg.set("recoCompDir", recoCompDir);
            }
            else if (currentArg.BeginsWith("lepSel=")) {
                getArg(currentArg, lepSel);
		cfg.set("lepSel", lepSel);
            }
            else if (currentArg.BeginsWith("jetPtMin=")) {
                getArg(currentArg, jetPtMin);
		cfg.set("jetPtMin", jetPtMin);
            }
            else if (currentArg.BeginsWith("jetEtaMax=")) {
                getArg(currentArg, jetEtaMax);
		cfg.set("jetEtaMax", jetEtaMax);
            }
            else if (currentArg.BeginsWith("doPASPlots=")) {
                getArg(currentArg, doPASPlots);
		cfg.set("doPASPlots", doPASPlots);
            }
            //--- asking for help ---
            else if (currentArg.Contains("help") || currentArg.BeginsWith("-h")) {
                std::cout << "\nUsage: \n\t./runRecoComparison [lepSel=(DMu, DE)] [jetPtMin=(int)] [jetEtaMax=(int*10)] [histoDir=(path)] [recoCompDir=(path)] [doPASPlots=(0,1)] [--help]" << std::endl;
                std::cout << "\neg: ./runRecoComparison lepSel=DMu jetEtaMax=24" << std::endl;
                std::cout << "\nunspecified options will be read from vjets.cfg\n" << std::endl;
                return 0;
            }
            //--- bad option ---
            else {
                std::cerr << "Warning: unknown option \"" << currentArg << "\"" << std::endl;
                std::cerr << "Please issue ./runRecoComparison --help for more information on possible options" << std::endl;
                return 0;
            }
        }
    }
    std::cout << "\n";
    
    if (!histoDir.EndsWith("/")) histoDir += "/";
    if (!recoCompDir.EndsWith("/")) recoCompDir += "/";

    RecoComparison(doPASPlots, lepSel, histoDir, recoCompDir, jetPtMin, jetEtaMax);

    std::cout << "Produce event yield table for p_T(jet) > " << jetPtMin << " GeV, |y(jet)| < " << jetEtaMax
	      << "\n";

    
    //for(int ivar = 0; ivar < NVAROFINTEREST; ++ivar){
    // getStatistics(lepSel, jetPtMin, jetEtaMax, VAROFINTEREST[ivar].name);
    //}
    getStatistics(lepSel, jetPtMin, jetEtaMax, "ZNGoodJets_Zexc");
    getStatistics(lepSel, jetPtMin, jetEtaMax, "ZNGoodJets_Zinc");
    getStatistics(lepSel, jetPtMin, jetEtaMax, "FirstJetPt_Zinc1jet");
    getStatistics(lepSel, jetPtMin, jetEtaMax, "SecondJetPt_Zinc2jet");
    getStatistics(lepSel, jetPtMin, jetEtaMax, "ThirdJetPt_Zinc3jet");
    
    //getStatistics("Electrons", 30);
    //getStatistics("Muons", 30);
    //getStatistics("Muons", 30,0,0,1,0,1);
    //getStatistics("Electrons", 30,0,0,1,0,1);


    return 0;
}
