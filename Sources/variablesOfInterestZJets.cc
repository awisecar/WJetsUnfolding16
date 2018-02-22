#include <variablesOfInterestZJets.h>

int findVariable(const TString &variable) {
    int index = -1;
    for (unsigned int i = 0; i < NVAROFINTERESTZJETS; ++i) {
        if (VAROFINTERESTZJETS[i].name == variable) {
            index = i;
            break;
        }
    }

    return index;
}
