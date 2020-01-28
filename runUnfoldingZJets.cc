#include <iostream>
#include <TString.h>
#include "ArgParser.h"
#include "ConfigVJets.h"
#include "UnfoldingZJets.h"

ConfigVJets cfg;

int main(int argc, char **argv)
{
    //--- Loads configuration -----------------------------------------------------

    //andrew -- 5 march 2019 -- current defaults for these options commented to the right
    TString lepSel     = cfg.getS("lepSel");    // SMu
    int year           = cfg.getI("year");      // ...
    TString algo       = cfg.getS("algo");      // TUnfold (can switch to Bayes if want to use RooUnfold Bayes)
    TString histoDir   = cfg.getS("histoDir");  // HistoFiles
    TString unfoldDir  = cfg.getS("unfoldDir"); // UnfoldedFiles
    int jetPtMin       = cfg.getI("jetPtMin");  // 30
    int jetEtaMax      = cfg.getI("jetEtaMax"); // 24
    int whichSyst      = cfg.getI("whichSyst"); // -1 (can set on command line to 1 or higher to select on a syst)

    TString variable = "";
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
            else if (currentArg.BeginsWith("variable=")) {
                getArg(currentArg, variable);
		        cfg.set("variable", variable);
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

    if (!histoDir.EndsWith("/")) histoDir += "/";
    if (!unfoldDir.EndsWith("/")) unfoldDir += "/";
    
    std::cout << "\n >>>>> Executing UnfoldingZJets(\"" << lepSel << "\", \"" <<  year << "\", \"" <<  algo << "\", \"" << histoDir << "\", \"" << unfoldDir << "\", " << jetPtMin << ", " << jetEtaMax << ", " << variable << ", " << doNormalized << ", " << whichSyst << ");" << std::endl;
    std::cout << " >>>>> Where UnfoldingZJets(lepSel, year, algo, histoDir, unfoldDir, jetPtMin, jetEtaMax, variable, doNormalized, whichSyst)" << std::endl;

    UnfoldingZJets(lepSel, year, algo, histoDir, unfoldDir, jetPtMin, jetEtaMax, variable, doNormalized, whichSyst);

    return 0;
}
