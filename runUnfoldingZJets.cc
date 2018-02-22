#include <iostream>
#include <TString.h>
#include "ArgParser.h"
#include "ConfigVJets.h"
#include "UnfoldingZJets.h"

ConfigVJets cfg;

int main(int argc, char **argv)
{
    //--- Loads configuration -----------------------------------------------------

    TString histoDir   = cfg.getS("histoDir");
    TString unfoldDir  = cfg.getS("unfoldDir");
    TString lepSel     = cfg.getS("lepSel");
    TString algo       = cfg.getS("algo");
    int jetPtMin       = cfg.getI("jetPtMin");
    int jetEtaMax      = cfg.getI("jetEtaMax");
    int whichSyst      = cfg.getI("whichSyst");
    //TString generator1 = cfg.getS("generator1", "sherpa2");
    //TString generator2 = cfg.getS("generator2", "amcatnlo");

    TString variable = "";
    bool doNormalized(false);

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
            else if (currentArg.BeginsWith("unfoldDir=")) {
                getArg(currentArg, unfoldDir);
		cfg.set("unfoldDir", unfoldDir);
            }
            else if (currentArg.BeginsWith("lepSel=")) {
                getArg(currentArg, lepSel);
		cfg.set("lepSel", lepSel);
            }
            else if (currentArg.BeginsWith("algo=")) {
                getArg(currentArg, algo);
		cfg.set("algo", algo);
            }
            /*else if (currentArg.BeginsWith("generator1=")) {
                getArg(currentArg, generator1);
		cfg.set("generator1", generator1);
            }
            else if (currentArg.BeginsWith("generator2=")) {
                getArg(currentArg, generator2);
		cfg.set("generator2", generator2);
            }*/
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
            //--- asking for help ---
            else if (currentArg.Contains("help") || currentArg.BeginsWith("-h")) {
                std::cout << "\nUsage: \n\t./runUnfolding [lepSel=(DMu, DE)] [algo=(Bayes, SVD)] [jetPtMin=(int)] [jetEtaMax=(int*10)] [histoDir=(path)] [unfoldDir=(path)] [variable=(variableName)] [doNormalized=(0, 1)] [whichSyst=(-1)] [--help]" << std::endl;
                std::cout << "\neg: ./runUnfolding lepSel=DMu jetEtaMax=24" << std::endl;
                std::cout << "\nunspecified options will be read from vjets.cfg\n" << std::endl;
                return 0;
            }
            //--- bad option ---
            else {
                std::cerr << "Warning: unknown option \"" << currentArg << "\"" << std::endl;
                std::cerr << "Please issue ./runUnfolding --help for more information on possible options" << std::endl;
                return 0;
            }
        }
    }

    if (!histoDir.EndsWith("/")) histoDir += "/";
    if (!unfoldDir.EndsWith("/")) unfoldDir += "/";

    std::cout << "\n executing UnfoldingZJets(\"" << lepSel << "\", \"" <<  algo << "\", \"" << histoDir << "\", \"" << unfoldDir << "\", " << jetPtMin << ", " << jetEtaMax << ", &argc, argv);" << std::endl;
    //-----------------------------------------------------------------------------

    //UnfoldingZJets(lepSel, algo, histoDir, unfoldDir, jetPtMin, jetEtaMax, generator1, generator2, variable, doNormalized);
    UnfoldingZJets(lepSel, algo, histoDir, unfoldDir, jetPtMin, jetEtaMax, variable, doNormalized, whichSyst);

    return 0;
}
