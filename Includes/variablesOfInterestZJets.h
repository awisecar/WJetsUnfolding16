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

//--- Z Jets ------------------------------------

const variableStruct VAROFINTERESTZJETS[] = {
    //--  Name  ------------------ log - decrease - Mu SVD kterm - Mu Bayes kterm - E SVD kterm - E Bayes kterm  - E Bayes kterm COMBINE
    //-- jet Pt distribution
    {"FirstJetPt_Zinc1jet",         1,      1,          11,              5,             9,            9 ,               6    }, // 1
    {"SecondJetPt_Zinc2jet",        1,      1,          14,              8,            11,            7 ,               7    }, // 2
    {"ThirdJetPt_Zinc3jet",         1,      1,           6,              9,             8,            7 ,               7    }, // 3
    {"FourthJetPt_Zinc4jet",        1,      1,           4,              4,             6,            5 ,               4    }, // 4
    {"FifthJetPt_Zinc5jet",         1,      1,           3,              2,             4,            3 ,               2    }, // 5 
    //-- jets HT distribution
    {"JetsHT_Zinc1jet",             1,      1,           9,              4,             7,            3 ,               3    }, // 6
    {"JetsHT_Zinc2jet",             1,      1,           5,              3,             9,            4 ,               2    }, // 7
    {"JetsHT_Zinc3jet",             1,      1,           6,              3,             6,            6 ,               3    }, // 8
    {"JetsHT_Zinc4jet",             1,      1,           5,              5,             4,            4 ,               3    }, // 9
    {"JetsHT_Zinc5jet",             1,      1,           3,              2,             4,            3 ,               1    }, // 10       
    //-- jet Eta distribution
//    {"FirstJetEta_Zinc1jet",        0,      1,          10,              4,            18,            4 ,               5    }, // 11
//    {"SecondJetEta_Zinc2jet",       0,      1,           6,              5,            16,            5 ,               5    }, // 12
//    {"ThirdJetEta_Zinc3jet",        0,      1,           4,              4,             8,            4 ,               5    }, // 13
//    {"FourthJetEta_Zinc4jet",       0,      1,           4,              4,             4,            5 ,               4    }, // 14
//    {"FifthJetEta_Zinc5jet",        0,      1,           3,              2,             2,            2 ,               1    }, // 15    
    //-- jet Rapidity distribution
    {"FirstJetAbsRapidity_Zinc1jet",        0,      1,          10,              4,            18,            4 ,               5    }, // 11
    {"SecondJetAbsRapidity_Zinc2jet",       0,      1,           6,              5,            16,            5 ,               5    }, // 12
    {"ThirdJetAbsRapidity_Zinc3jet",        0,      1,           4,              4,             8,            4 ,               5    }, // 13
    {"FourthJetAbsRapidity_Zinc4jet",       0,      1,           4,              4,             4,            5 ,               4    }, // 14
    {"FifthJetAbsRapidity_Zinc5jet",        0,      1,           3,              2,             2,            2 ,               1    }, // 15    
    //-- Jets Mass distribution
    {"JetsMass_Zinc2jet",           0,      1,           9,              5,            13,           11 ,               5    }, // 16
//    {"ZPt_Zinc0jet",                0,      1,           9,              5,            13,           11 ,               5    }, // 17
    {"ZAbsRapidity_Zinc1jet",       0,      1,           9,              5,            13,           11 ,               5    }, // 18
    {"SumZJetRapidity_Zinc1jet",    0,      1,           9,              5,            13,           11 ,               5    }, // 19
    {"DifZJetRapidity_Zinc1jet",    0,      1,           9,              5,            13,           11 ,               5    }, // 20
    {"FirstJetAbsRapidity_Zinc1jet",0,      1,           9,              5,            13,           11 ,               5    }, // 21
    //-- jet Pt distribution
    {"AbsZRapidity_Zinc1jet",        1,      1,           3,              3,             3,            3 ,               3    }, // 1
    {"AbsFirstJetRapidity_Zinc1jet",         1,      1,           3,              3,             3,            3 ,               3    }, // 1
    {"SumZFirstJetRapidity_Zinc1jet",         1,      1,           3,              3,             3,            3 ,               3    }, // 1
    {"DifZFirstJetRapidity_Zinc1jet",         1,      1,           3,              3,             3,            3 ,               3    }, // 1
    {"SumZFirstJetEta_Zinc1jet",         1,      1,           3,              3,             3,            3 ,               3    }, // 1
    {"DifZFirstJetEta_Zinc1jet",         1,      1,           3,              3,             3,            3 ,               3    }, // 1
 //   {"AbsZRapidity_Zexc1jet",         1,      1,           3,              3,             3,            3 ,               3    }, // 1
 //   {"AbsJetRapidity_Zexc1jet",         1,      1,           3,              3,             3,            3 ,               3    }, // 1
 //   {"SumZJetRapidity_Zexc1jet",         1,      1,           3,              3,             3,            3 ,               3    }, // 1
 //   {"DifZJetRapidity_Zexc1jet",         1,      1,           3,              3,             3,            3 ,               3    }, // 1
    {"AbsZRapidity_Zinc2jet",        1,      1,           3,              3,             3,            3 ,               3    }, // 1
    {"AbsFirstJetRapidity_Zinc2jet",         1,      1,           3,              3,             3,            3 ,               3    }, // 1
    {"SumZFirstJetRapidity_Zinc2jet",         1,      1,           3,              3,             3,            3 ,               3    }, // 1
    {"DifZFirstJetRapidity_Zinc2jet",         1,      1,           3,              3,             3,            3 ,               3    }, // 1
    {"AbsSecondJetRapidity_Zinc2jet",         1,      1,           3,              3,             3,            3 ,               3    }, // 1
    {"SumZSecondJetRapidity_Zinc2jet",         1,      1,           3,              3,             3,            3 ,               3    }, // 1
    {"DifZSecondJetRapidity_Zinc2jet",         1,      1,           3,              3,             3,            3 ,               3    }, // 1
    {"SumFirstSecondJetRapidity_Zinc2jet",         1,      1,           3,              3,             3,            3 ,               3    }, // 1
    {"DifFirstSecondJetRapidity_Zinc2jet",         1,      1,           3,              3,             3,            3 ,               3    }, // 1
    {"SumZTwoJetsRapidity_Zinc2jet",         1,      1,           3,              3,             3,            3 ,               3    }, // 1
    {"DifZTwoJetsRapidity_Zinc2jet",         1,      1,           3,              3,             3,            3 ,               3    }, // 1
    {"DPhiZFirstJet_Zinc1jet",         1,      1,           3,              3,             3,            3 ,               3    }, // 1
    {"DPhiZFirstJet_Zinc2jet",         1,      1,           3,              3,             3,            3 ,               3    }, // 1
    {"DPhiZFirstJet_Zinc3jet",         1,      1,           3,              3,             3,            3 ,               3    }, // 1
    {"DPhiZSecondJet_Zinc3jet",         1,      1,           3,              3,             3,            3 ,               3    }, // 1
    {"DPhiZThirdJet_Zinc3jet",         1,      1,           3,              3,             3,            3 ,               3    }, // 1
    {"DPhiFirstSecondJet_Zinc3jet",         1,      1,           3,              3,             3,            3 ,               3    }, // 1
    {"DPhiFirstThirdJet_Zinc3jet",         1,      1,           3,              3,             3,            3 ,               3    }, // 1
    {"DPhiSecondThirdJet_Zinc3jet",         1,      1,           3,              3,             3,            3 ,               3    }, // 1
    {"DPhiZFirstJet_ZPt150_Zinc1jet",         1,      1,           3,              3,             3,            3 ,               3    }, // 1
    {"DPhiZFirstJet_ZPt150_Zinc2jet",         1,      1,           3,              3,             3,            3 ,               3    }, // 1
    {"DPhiZFirstJet_ZPt150_Zinc3jet",         1,      1,           3,              3,             3,            3 ,               3    }, // 1
    {"DPhiZSecondJet_ZPt150_Zinc3jet",         1,      1,           3,              3,             3,            3 ,               3    }, // 1
    {"DPhiZThirdJet_ZPt150_Zinc3jet",         1,      1,           3,              3,             3,            3 ,               3    }, // 1
    {"DPhiFirstSecondJet_ZPt150_Zinc3jet",         1,      1,           3,              3,             3,            3 ,               3    }, // 1
    {"DPhiFirstThirdJet_ZPt150_Zinc3jet",         1,      1,           3,              3,             3,            3 ,               3    }, // 1
    {"DPhiSecondThirdJet_ZPt150_Zinc3jet",         1,      1,           3,              3,             3,            3 ,               3    }, // 1
    {"DPhiZFirstJet_ZPt300_Zinc1jet",         1,      1,           3,              3,             3,            3 ,               3    }, // 1
    {"DPhiZFirstJet_ZPt300_Zinc2jet",         1,      1,           3,              3,             3,            3 ,               3    }, // 1
    {"DPhiZFirstJet_ZPt300_Zinc3jet",         1,      1,           3,              3,             3,            3 ,               3    }, // 1
    {"DPhiZSecondJet_ZPt300_Zinc3jet",         1,      1,           3,              3,             3,            3 ,               3    }, // 1
    {"DPhiZThirdJet_ZPt300_Zinc3jet",         1,      1,           3,              3,             3,            3 ,               3    }, // 1
    {"DPhiFirstSecondJet_ZPt300_Zinc3jet",         1,      1,           3,              3,             3,            3 ,               3    }, // 1
    {"DPhiFirstThirdJet_ZPt300_Zinc3jet",         1,      1,           3,              3,             3,            3 ,               3    }, // 1
    {"DPhiSecondThirdJet_ZPt300_Zinc3jet",         1,      1,           3,              3,             3,            3 ,               3    }, // 1
    {"DPhiZFirstJet_ZPt150_HT300_Zinc3jet",         1,      1,           3,              3,             3,            3 ,               3    }, // 1
    {"DPhiZSecondJet_ZPt150_HT300_Zinc3jet",         1,      1,           3,              3,             3,            3 ,               3    }, // 1
    {"DPhiZThirdJet_ZPt150_HT300_Zinc3jet",         1,      1,           3,              3,             3,            3 ,               3    }, // 1
    //different Z boson PT cuts/////////
//    {"AbsZRapidity_ZPt100_Zinc1jet",        1,      1,           3,              3,             3,            3 ,               3    }, // 1
//    {"AbsFirstJetRapidity_ZPt100_Zinc1jet",         1,      1,           3,              3,             3,            3 ,               3    }, // 1
//    {"SumZFirstJetRapidity_ZPt100_Zinc1jet",         1,      1,           3,              3,             3,            3 ,               3    }, // 1
//    {"DifZFirstJetRapidity_ZPt100_Zinc1jet",         1,      1,           3,              3,             3,            3 ,               3    }, // 1
//    {"AbsZRapidity_ZPt100_Zinc2jet",        1,      1,           3,              3,             3,            3 ,               3    }, // 1
//    {"AbsSecondJetRapidity_ZPt100_Zinc2jet",         1,      1,           3,              3,             3,            3 ,               3    }, // 1
//    {"SumZSecondJetRapidity_ZPt100_Zinc2jet",         1,      1,           3,              3,             3,            3 ,               3    }, // 1
//    {"DifZSecondJetRapidity_ZPt100_Zinc2jet",         1,      1,           3,              3,             3,            3 ,               3    }, // 1
    {"AbsZRapidity_ZPt150_Zinc1jet",        1,      1,           3,              3,             3,            3 ,               3    }, // 1
    {"AbsFirstJetRapidity_ZPt150_Zinc1jet",         1,      1,           3,              3,             3,            3 ,               3    }, // 1
    {"SumZFirstJetRapidity_ZPt150_Zinc1jet",         1,      1,           3,              3,             3,            3 ,               3    }, // 1
    {"DifZFirstJetRapidity_ZPt150_Zinc1jet",         1,      1,           3,              3,             3,            3 ,               3    }, // 1
    {"AbsZRapidity_ZPt150_Zinc2jet",        1,      1,           3,              3,             3,            3 ,               3    }, // 1
    {"AbsSecondJetRapidity_ZPt150_Zinc2jet",         1,      1,           3,              3,             3,            3 ,               3    }, // 1
    {"SumZSecondJetRapidity_ZPt150_Zinc2jet",         1,      1,           3,              3,             3,            3 ,               3    }, // 1
    {"DifZSecondJetRapidity_ZPt150_Zinc2jet",         1,      1,           3,              3,             3,            3 ,               3    }, // 1
    {"AbsZRapidity_ZPt300_Zinc1jet",        1,      1,           3,              3,             3,            3 ,               3    }, // 1
    {"AbsFirstJetRapidity_ZPt300_Zinc1jet",         1,      1,           3,              3,             3,            3 ,               3    }, // 1
    {"SumZFirstJetRapidity_ZPt300_Zinc1jet",         1,      1,           3,              3,             3,            3 ,               3    }, // 1
    {"DifZFirstJetRapidity_ZPt300_Zinc1jet",         1,      1,           3,              3,             3,            3 ,               3    }, // 1
    {"ZPt_Zinc1jet",         1,      1,           3,              3,             3,            3 ,               3    }, // 1
//    {"ZPt_Zinc2jet",         1,      1,           3,              3,             3,            3 ,               3    }, // 1
    //different JetPt cuts////////
//    {"AbsZRapidity_FirstJetPt50_Zinc1jet",        1,      1,           3,              3,             3,            3 ,               3    }, // 1
//    {"AbsFirstJetRapidity_FirstJetPt50_Zinc1jet",         1,      1,           3,              3,             3,            3 ,               3    }, // 1
//    {"SumZFirstJetRapidity_FirstJetPt50_Zinc1jet",         1,      1,           3,              3,             3,            3 ,               3    }, // 1
//    {"DifZFirstJetRapidity_FirstJetPt50_Zinc1jet",         1,      1,           3,              3,             3,            3 ,               3    }, // 1
//    {"AbsZRapidity_FirstJetPt80_Zinc1jet",        1,      1,           3,              3,             3,            3 ,               3    }, // 1
//    {"AbsFirstJetRapidity_FirstJetPt80_Zinc1jet",         1,      1,           3,              3,             3,            3 ,               3    }, // 1
//    {"SumZFirstJetRapidity_FirstJetPt80_Zinc1jet",         1,      1,           3,              3,             3,            3 ,               3    }, // 1
//    {"DifZFirstJetRapidity_FirstJetPt80_Zinc1jet",         1,      1,           3,              3,             3,            3 ,               3    }, // 1
    //Set jet rapidity discriminator////
//    {"AbsZRapidity_DifJetRapidityl2_Zinc2jet",        1,      1,           3,              3,             3,            3 ,               3    }, // 1
//    {"AbsFirstJetRapidity_DifJetRapidityl2_Zinc2jet",         1,      1,           3,              3,             3,            3 ,               3    }, // 1
//    {"SumZFirstJetRapidity_DifJetRapidityl2_Zinc2jet",         1,      1,           3,              3,             3,            3 ,               3    }, // 1
//    {"DifZFirstJetRapidity_DifJetRapidityl2_Zinc2jet",         1,      1,           3,              3,             3,            3 ,               3    }, // 1
//    {"AbsZRapidity_DifJetRapiditys2_Zinc2jet",        1,      1,           3,              3,             3,            3 ,               3    }, // 1
//    {"AbsFirstJetRapidity_DifJetRapiditys2_Zinc2jet",         1,      1,           3,              3,             3,            3 ,               3    }, // 1
//    {"SumZFirstJetRapidity_DifJetRapiditys2_Zinc2jet",         1,      1,           3,              3,             3,            3 ,               3    }, // 1
//    {"DifZFirstJetRapidity_DifJetRapiditys2_Zinc2jet",         1,      1,           3,              3,             3,            3 ,               3    }, // 1
    {"ZNGoodJets_Zexc",             1,      1,           7,              4,             3,            3 ,               3    }, // 0
    {"ZNGoodJets_Zinc",             1,      1,           7,              4,             3,            3 ,               3    }, // 0
    {"ZNGoodJetsFull_Zexc",             1,      1,           7,              4,             3,            3 ,               3    }, // 0
    {"ZNGoodJetsFull_Zinc",             1,      1,           7,              4,             3,            3 ,               3    }, // 0
    {"dPhiLepJet1_Zinc1jet",         1,      1,           3,              3,             3,            3 ,               3    }, // 1
    {"dPhiLepJet2_Zinc2jet",         1,      1,           3,              3,             3,            3 ,               3    }, // 1
    {"dPhiLepJet3_Zinc3jet",         1,      1,           3,              3,             3,            3 ,               3    }, // 1
    {"dPhiLepJet4_Zinc4jet",         1,      1,           3,              3,             3,            3 ,               3    }, // 1
    {"dRptmin100LepCloseJetCo300dR04_Zinc1jet",         1,      1,           3,              3,             3,            3 ,               3    }, // 1
    {"dRptmin100LepCloseDiJetCo300dR04_Zinc2jet",         1,      1,           3,              3,             3,            3 ,               3    }, // 1

//andrew
    {"LeadingJetPt_Zinc1jet",         1,      1,          1,              4,             1,            1 ,               1    },// 1
    {"LeadingJetPt_Zinc2jet",         1,      1,          1,              4,             1,            1 ,               1    },// 1
    {"LeadingJetPt_Zinc3jet",         1,      1,          1,              4,             1,            1 ,               1    },// 1
    {"LeadingJetPt_Zinc4jet",         1,      1,          1,              4,             1,            1 ,               1    },// 1
    {"HTover2_Zinc2jet",         1,      1,          1,              4,             1,            1 ,               1    },// 1
    {"HTover2_Zinc3jet",         1,      1,          1,              4,             1,            1 ,               1    },// 1
    {"HTover2_Zinc4jet",         1,      1,          1,              4,             1,            1 ,               1    },// 1
    {"LepPtPlusLeadingJetPt_Zinc1jet",         1,      1,          1,              4,             1,            1 ,               1    },
    {"LepPtPlusLeadingJetPt_Zinc2jet",         1,      1,          1,              4,             1,            1 ,               1    },
    {"LepPtPlusLeadingJetPt_Zinc3jet",         1,      1,          1,              4,             1,            1 ,               1    },
    {"LepPtPlusLeadingJetPt_Zinc4jet",         1,      1,          1,              4,             1,            1 ,               1    },
    {"LepPtPlusHTover2_Zinc2jet",         1,      1,          1,              4,             1,            1 ,               1    },// 1
    {"LepPtPlusHTover2_Zinc3jet",         1,      1,          1,              4,             1,            1 ,               1    },// 1
    {"LepPtPlusHTover2_Zinc4jet",         1,      1,          1,              4,             1,            1 ,               1    },// 1
    {"ZPt_Zinc1jet",      1,      1,          1,              4,             1,            1 ,               1    },
    {"ZPt_Zinc2jet",      1,      1,          1,              4,             1,            1 ,               1    },
    {"ZPt_Zinc3jet",      1,      1,          1,              4,             1,            1 ,               1    },
    {"ZPt_Zinc4jet",      1,      1,          1,              4,             1,            1 ,               1    },
    {"ZPtPlusLeadingJetPt_Zinc1jet",      1,      1,          1,              4,             1,            1 ,               1    },
    {"ZPtPlusLeadingJetPt_Zinc2jet",      1,      1,          1,              4,             1,            1 ,               1    },
    {"ZPtPlusLeadingJetPt_Zinc3jet",      1,      1,          1,              4,             1,            1 ,               1    },
    {"ZPtPlusLeadingJetPt_Zinc4jet",      1,      1,          1,              4,             1,            1 ,               1    },
    {"ZPtPlusHTover2_Zinc2jet",      1,      1,          1,              4,             1,            1 ,               1    },
    {"ZPtPlusHTover2_Zinc3jet",      1,      1,          1,              4,             1,            1 ,               1    },
    {"ZPtPlusHTover2_Zinc4jet",      1,      1,          1,              4,             1,            1 ,               1    },
    ////
    {"FirstJetPhi_Zinc1jet",      1,      1,          1,              4,             1,            1 ,               1    },
    {"MT_Zinc0jet",      1,      1,          1,              4,             1,            1 ,               1    },
    {"MET_Zinc0jet",      1,      1,          1,              4,             1,            1 ,               1    },
    {"lepPt_Zinc0jet",      1,      1,          1,              4,             1,            1 ,               1    },
    ////
    /// following distributions have different binning for reco and gen (done for TUnfold)
    {"LeadingJetPt_2_Zinc1jet",         1,      1,          1,              4,             1,            1 ,               1    },// 1
    {"LeadingJetPt_2_Zinc2jet",         1,      1,          1,              4,             1,            1 ,               1    },// 1
    {"LeadingJetPt_2_Zinc3jet",         1,      1,          1,              4,             1,            1 ,               1    },// 1
    {"LeadingJetPt_2_Zinc4jet",         1,      1,          1,              4,             1,            1 ,               1    },// 1
    {"HTover2_2_Zinc2jet",         1,      1,          1,              4,             1,            1 ,               1    },// 1
    {"HTover2_2_Zinc3jet",         1,      1,          1,              4,             1,            1 ,               1    },// 1
    {"HTover2_2_Zinc4jet",         1,      1,          1,              4,             1,            1 ,               1    },// 1
    {"LepPtPlusLeadingJetPt_2_Zinc1jet",         1,      1,          1,              4,             1,            1 ,               1    },
    {"LepPtPlusLeadingJetPt_2_Zinc2jet",         1,      1,          1,              4,             1,            1 ,               1    },
    {"LepPtPlusLeadingJetPt_2_Zinc3jet",         1,      1,          1,              4,             1,            1 ,               1    },
    {"LepPtPlusLeadingJetPt_2_Zinc4jet",         1,      1,          1,              4,             1,            1 ,               1    },
    {"LepPtPlusHTover2_2_Zinc2jet",      1,      1,          1,              4,             1,            1 ,               1    },
    {"LepPtPlusHTover2_2_Zinc3jet",      1,      1,          1,              4,             1,            1 ,               1    },
    {"LepPtPlusHTover2_2_Zinc4jet",      1,      1,          1,              4,             1,            1 ,               1    },
    {"ZPt_2_Zinc1jet",      1,      1,          1,              4,             1,            1 ,               1    },
    {"ZPt_2_Zinc2jet",      1,      1,          1,              4,             1,            1 ,               1    },
    {"ZPt_2_Zinc3jet",      1,      1,          1,              4,             1,            1 ,               1    },
    {"ZPt_2_Zinc4jet",      1,      1,          1,              4,             1,            1 ,               1    },
    {"ZPtPlusLeadingJetPt_2_Zinc1jet",      1,      1,          1,              4,             1,            1 ,               1    },
    {"ZPtPlusLeadingJetPt_2_Zinc2jet",      1,      1,          1,              4,             1,            1 ,               1    },
    {"ZPtPlusLeadingJetPt_2_Zinc3jet",      1,      1,          1,              4,             1,            1 ,               1    },
    {"ZPtPlusLeadingJetPt_2_Zinc4jet",      1,      1,          1,              4,             1,            1 ,               1    },
    {"ZPtPlusHTover2_2_Zinc2jet",      1,      1,          1,              4,             1,            1 ,               1    },
    {"ZPtPlusHTover2_2_Zinc3jet",      1,      1,          1,              4,             1,            1 ,               1    },
    {"ZPtPlusHTover2_2_Zinc4jet",      1,      1,          1,              4,             1,            1 ,               1    },
    // new
    {"LepPtPlusLeadingJetPt_TUnfold_Zinc1jet",      1,      1,          1,              4,             1,            1 ,               1    },
    {"LepPtPlusLeadingJetPt_TUnfold_Zinc2jet",      1,      1,          1,              4,             1,            1 ,               1    },
    {"LepPtPlusLeadingJetPt_TUnfold_Zinc3jet",      1,      1,          1,              4,             1,            1 ,               1    },
    {"LepPtPlusLeadingJetPt_TUnfold_Zinc4jet",      1,      1,          1,              4,             1,            1 ,               1    },
    {"FirstJetPt_Zinc1jet_TUnfold",      1,      1,          1,              4,             1,            1 ,               1    },
    {"FirstJetAbsRapidity_Zinc1jet_TUnfold",      1,      1,          1,              4,             1,            1 ,               1    },
    {"dPhiLepJet1_Zinc1jet_TUnfold",      1,      1,          1,              4,             1,            1 ,               1    },
    // for alpha-s ratios
    {"LepPtPlusLeadingJetPt_Zinc1jet_TUnfold",      1,      1,          1,              4,             1,            1 ,               1    },
    {"LepPtPlusLeadingJetPt_Zinc2jet_TUnfold",      1,      1,          1,              4,             1,            1 ,               1    },
    {"LepPtPlusLeadingJetPt_Zinc3jet_TUnfold",      1,      1,          1,              4,             1,            1 ,               1    },
    {"LepPtPlusLeadingJetPt_Zexc1jet_TUnfold",      1,      1,          1,              4,             1,            1 ,               1    },
    {"LepPtPlusLeadingJetPt_Zexc2jet_TUnfold",      1,      1,          1,              4,             1,            1 ,               1    },
    {"LepPtPlusLeadingJetPt_Zexc3jet_TUnfold",      1,      1,          1,              4,             1,            1 ,               1    },
    {"LepPtPlusLeadingJetAK8Pt_Zinc1jet_TUnfold",      1,      1,          1,              4,             1,            1 ,               1    },
    {"LepPtPlusLeadingJetAK8Pt_Zinc2jet_TUnfold",      1,      1,          1,              4,             1,            1 ,               1    },
    {"LepPtPlusLeadingJetAK8Pt_Zinc3jet_TUnfold",      1,      1,          1,              4,             1,            1 ,               1    },
    {"LepPtPlusLeadingJetAK8Pt_Zexc1jet_TUnfold",      1,      1,          1,              4,             1,            1 ,               1    },
    {"LepPtPlusLeadingJetAK8Pt_Zexc2jet_TUnfold",      1,      1,          1,              4,             1,            1 ,               1    },
    {"LepPtPlusLeadingJetAK8Pt_Zexc3jet_TUnfold",      1,      1,          1,              4,             1,            1 ,               1    },
    // for closure test using W+jets reco MC
    {"FirstJetPt_Zinc1jet_TUnfold_NOEFFWEIGHTS",      1,      1,          1,              4,             1,            1 ,               1    },
    {"FirstJetAbsRapidity_Zinc1jet_TUnfold_NOEFFWEIGHTS",      1,      1,          1,              4,             1,            1 ,               1    },
    {"dPhiLepJet1_Zinc1jet_TUnfold_NOEFFWEIGHTS",      1,      1,          1,              4,             1,            1 ,               1    },
}; 

const unsigned int NVAROFINTERESTZJETS = sizeof(VAROFINTERESTZJETS)/sizeof(VAROFINTERESTZJETS[0]);

int findVariable(const TString &variable);
#endif
