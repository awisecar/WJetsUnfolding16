#include <iostream>
#include <TString.h>
#include "ArgParser.h"
#include "ConfigVJets.h"
#include "TakeRatiosOfUnfoldedXSecs.h"

ConfigVJets cfg;

int main(int argc, char **argv){
    //--- Loads configuration -----------------------------------------------------

    TString lepSel     = cfg.getS("lepSel");    //SMu
    int year           = cfg.getI("year");      // ...
    TString algo       = cfg.getS("algo");      //TUnfold (can switch to Bayes if want to use RooUnfold Bayes)
    TString histoDir   = cfg.getS("histoDir");  //HistoFiles
    TString unfoldDir  = cfg.getS("unfoldDir"); //UnfoldedFiles
    int jetPtMin       = cfg.getI("jetPtMin");  //30
    int jetEtaMax      = cfg.getI("jetEtaMax"); //24
    int whichSyst      = cfg.getI("whichSyst"); //-1 (can set on command line to 1 or higher to select on a syst)

    TString variableNUM = "";
    TString variableDENOM = "";
    bool doNormalized(false);

    //--- Parse the arguments -----------------------------------------------------
    if (argc > 1) {
        for (int i = 1; i < argc; ++i) {
            TString currentArg = argv[i];

            //--- possible options ---
            if (currentArg.BeginsWith("lepSel=")) {
                getArg(currentArg, lepSel);
                cfg.set("lepSel", lepSel);
            }
            else if (currentArg.BeginsWith("year=")) {
                getArg(currentArg, year);
		        cfg.set("year", year);
            }
            else if (currentArg.BeginsWith("algo=")) {
                getArg(currentArg, algo);
                cfg.set("algo", algo);
            }
            else if (currentArg.BeginsWith("histoDir=")) {
                getArg(currentArg, histoDir);
		        cfg.set("histoDir", histoDir);
            }
            else if (currentArg.BeginsWith("unfoldDir=")) {
                getArg(currentArg, unfoldDir);
		        cfg.set("unfoldDir", unfoldDir);
            }
            else if (currentArg.BeginsWith("jetPtMin=")) {
                getArg(currentArg, jetPtMin);
		        cfg.set("jetPtMin", jetPtMin);
            }
            else if (currentArg.BeginsWith("jetEtaMax=")) {
                getArg(currentArg, jetEtaMax);
		        cfg.set("jetEtaMax", jetEtaMax);
            }
            else if (currentArg.BeginsWith("variableNUM=")) {
                getArg(currentArg, variableNUM);
		        cfg.set("variableNUM", variableNUM);
            }
            else if (currentArg.BeginsWith("variableDENOM=")) {
                getArg(currentArg, variableDENOM);
		        cfg.set("variableDENOM", variableDENOM);
            }
            else if (currentArg.BeginsWith("doNormalized=")) {
                getArg(currentArg, doNormalized);
		        cfg.set("doNormalized", doNormalized);
            }
	        else if (currentArg.BeginsWith("whichSyst=")) {
                getArg(currentArg, whichSyst);
		        cfg.set("whichSyst", whichSyst);
            }
            else {
                std::cerr << "Warning: unknown option \"" << currentArg << "\"" << std::endl;
                return 0;
            }
        }
    }
    
    std::cout << "\n >>>>> Executing TakeRatiosOfUnfoldedXSecs(\"" << lepSel << "\", \"" <<  year << "\", \"" <<  algo << "\", \"" << histoDir << "\", \"" << unfoldDir << "\", " << jetPtMin << ", " << jetEtaMax << ", " << variableNUM << ", " << variableDENOM << ", " << doNormalized << ", " << whichSyst << ");" << std::endl;
    std::cout << " >>>>> Where TakeRatiosOfUnfoldedXSecs(lepSel, year, algo, histoDir, unfoldDir, jetPtMin, jetEtaMax, variableNUM, variableDENOM, doNormalized, whichSyst)" << std::endl;

    TakeRatiosOfUnfoldedXSecs(lepSel, year, algo, histoDir, unfoldDir, jetPtMin, jetEtaMax, variableNUM, variableDENOM, doNormalized, whichSyst);

    return 0;
}
