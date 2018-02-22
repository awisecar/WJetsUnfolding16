#include "ArgParser.h"

void getArg(TString fullArg, TString &arg) {
    arg = fullArg(fullArg.Index("=")+1, fullArg.Length());
    std::cout << "Using command line option " << fullArg << std::endl;
}

void getArg(TString fullArg, int &arg) {
    arg = TString(fullArg(fullArg.Index("=")+1, fullArg.Length())).Atoi();
    std::cout << "Using command line option " << fullArg << std::endl;
}

void getArg(TString fullArg, Long_t &arg) {
    arg = TString(fullArg(fullArg.Index("=")+1, fullArg.Length())).Atoll();
    std::cout << "Using command line option " << fullArg << std::endl;
}

void getArg(TString fullArg, double &arg) {
    arg = TString(fullArg(fullArg.Index("=")+1, fullArg.Length())).Atof();
    std::cout << "Using command line option " << fullArg << std::endl;
}

void getArg(TString fullArg, bool &arg) {
    TString tmp = fullArg(fullArg.Index("=")+1, fullArg.Length());
    tmp.ToUpper();
    if (tmp == "TRUE" || tmp == "1") {
        arg = true; 
        std::cout << "Using command line option " << fullArg << std::endl;
    }
    else if (tmp == "FALSE" || tmp == "0") {
        arg = false;
        std::cout << "Using command line option " << fullArg << std::endl;
    }
    else {
        std::cerr << "Non boolean expression in " << fullArg << std::endl;
        std::cerr << "Taking vjets.cfg value for this option." << std::endl;
    }
}
