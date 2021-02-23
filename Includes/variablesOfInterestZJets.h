#ifndef __VARIABLESOFINTERESTZJETS_H_
#define __VARIABLESOFINTERESTZJETS_H_

#include <TObject.h>
#include <TObjString.h>
#include <TList.h>

struct variableStruct{
    TString name;
    bool log, decrease;  
    int MuSVDkterm, MuBayeskterm, ESVDkterm, EBayeskterm, BayesktermCombine;
};

const variableStruct VAROFINTERESTZJETS[] = {
    //--  Name  ------------------ log - decrease - Mu SVD kterm - Mu Bayes kterm - E SVD kterm - E Bayes kterm  - E Bayes kterm COMBINE
    // Jet multiplicity ---
    {"ZNGoodJets_Zexc_TUnfold",         1,      1,          1,              4,             1,            1 ,               1    },
    {"ZNGoodJetsAK8_Zexc_TUnfold",      1,      1,          1,              4,             1,            1 ,               1    },
    // AK4 jet kinematics ---
    // 1st jet
    {"FirstJetPt_Zinc1jet_TUnfold",               1,      1,          1,              4,             1,            1 ,               1    },
    {"FirstJetAbsRapidity_Zinc1jet_TUnfold",      1,      1,          1,              4,             1,            1 ,               1    },
    {"dPhiLepJet1_Zinc1jet_TUnfold",              1,      1,          1,              4,             1,            1 ,               1    },
    // 2nd jet
    {"SecondJetPt_Zinc2jet_TUnfold",               1,      1,          1,              4,             1,            1 ,               1    },
    {"SecondJetAbsRapidity_Zinc2jet_TUnfold",      1,      1,          1,              4,             1,            1 ,               1    },
    {"dPhiLepJet2_Zinc2jet_TUnfold",               1,      1,          1,              4,             1,            1 ,               1    },
    // other
    {"dRapidityJets_Zinc2jet_TUnfold",               1,      1,          1,              4,             1,            1 ,               1    },
    // AK8 jet kinematics ---
    // 1st jet
    {"FirstJetAK8Pt_Zinc1jet_TUnfold",               1,      1,          1,              4,             1,            1 ,               1    },
    {"FirstJetAK8AbsRapidity_Zinc1jet_TUnfold",      1,      1,          1,              4,             1,            1 ,               1    },
    {"dPhiLepJet1AK8_Zinc1jet_TUnfold",              1,      1,          1,              4,             1,            1 ,               1    },
    // 2nd jet
    {"SecondJetAK8Pt_Zinc2jet_TUnfold",               1,      1,          1,              4,             1,            1 ,               1    },
    {"SecondJetAK8AbsRapidity_Zinc2jet_TUnfold",      1,      1,          1,              4,             1,            1 ,               1    },
    {"dPhiLepJet2AK8_Zinc2jet_TUnfold",               1,      1,          1,              4,             1,            1 ,               1    },
    // other
    {"dRapidityJetsAK8_Zinc2jet_TUnfold",               1,      1,          1,              4,             1,            1 ,               1    },

    // For alpha-s ratios ---
    // Muon Pt + AK4 Jet Pt
    {"LepPtPlusLeadingJetPt_Zinc1jet_TUnfold",         1,      1,          1,              4,             1,            1 ,               1    },
    {"LepPtPlusLeadingJetPt_Zinc2jet_TUnfold",         1,      1,          1,              4,             1,            1 ,               1    },
    {"LepPtPlusLeadingJetPt_Zinc3jet_TUnfold",         1,      1,          1,              4,             1,            1 ,               1    },
    {"LepPtPlusLeadingJetPt_Zexc1jet_TUnfold",         1,      1,          1,              4,             1,            1 ,               1    },
    {"LepPtPlusLeadingJetPt_Zexc2jet_TUnfold",         1,      1,          1,              4,             1,            1 ,               1    },
    {"LepPtPlusLeadingJetPt_Zexc3jet_TUnfold",         1,      1,          1,              4,             1,            1 ,               1    },
    // Muon Pt + AK8 Jet Pt
    {"LepPtPlusLeadingJetAK8Pt_Zinc1jet_TUnfold",      1,      1,          1,              4,             1,            1 ,               1    },
    {"LepPtPlusLeadingJetAK8Pt_Zinc2jet_TUnfold",      1,      1,          1,              4,             1,            1 ,               1    },
    {"LepPtPlusLeadingJetAK8Pt_Zinc3jet_TUnfold",      1,      1,          1,              4,             1,            1 ,               1    },
    {"LepPtPlusLeadingJetAK8Pt_Zexc1jet_TUnfold",      1,      1,          1,              4,             1,            1 ,               1    },
    {"LepPtPlusLeadingJetAK8Pt_Zexc2jet_TUnfold",      1,      1,          1,              4,             1,            1 ,               1    },
    {"LepPtPlusLeadingJetAK8Pt_Zexc3jet_TUnfold",      1,      1,          1,              4,             1,            1 ,               1    },
    // Muon Pt + HT,2/2 (AK8)
    {"LepPtPlusHT2over2AK8_Zinc2jet_TUnfold",      1,      1,          1,              4,             1,            1 ,               1    },
    {"LepPtPlusHT2over2AK8_Zinc3jet_TUnfold",      1,      1,          1,              4,             1,            1 ,               1    },

}; 

const unsigned int NVAROFINTERESTZJETS = sizeof(VAROFINTERESTZJETS)/sizeof(VAROFINTERESTZJETS[0]);

int findVariable(const TString &variable);
#endif
