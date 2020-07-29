#!/bin/sh

# 28 July 20 --
./runUnfoldingZJetsRatios  year=2016  variableNUM=LepPtPlusLeadingJetPt_Zinc2jet_TUnfold     variableDENOM=LepPtPlusLeadingJetPt_Zinc1jet_TUnfold    # AK4, R21
./runUnfoldingZJetsRatios  year=2016  variableNUM=LepPtPlusLeadingJetPt_Zinc2jet_TUnfold     variableDENOM=LepPtPlusLeadingJetPt_Zexc1jet_TUnfold    # AK4, R2inc/1exc
./runUnfoldingZJetsRatios  year=2016  variableNUM=LepPtPlusLeadingJetAK8Pt_Zinc2jet_TUnfold  variableDENOM=LepPtPlusLeadingJetAK8Pt_Zinc1jet_TUnfold # AK8, R21
./runUnfoldingZJetsRatios  year=2016  variableNUM=LepPtPlusLeadingJetAK8Pt_Zinc2jet_TUnfold  variableDENOM=LepPtPlusLeadingJetAK8Pt_Zexc1jet_TUnfold # AK8, R2inc/1exc

# ## 19 May 20 ---
# # AK4 jets
# ./runUnfoldingZJetsRatios  year=2016  variableNUM=LepPtPlusLeadingJetPt_Zinc2jet_TUnfold  variableDENOM=LepPtPlusLeadingJetPt_Zinc1jet_TUnfold # R21
# ./runUnfoldingZJetsRatios  year=2016  variableNUM=LepPtPlusLeadingJetPt_Zinc3jet_TUnfold  variableDENOM=LepPtPlusLeadingJetPt_Zinc2jet_TUnfold # R32
# # ./runUnfoldingZJetsRatios  year=2016  variableNUM=LepPtPlusLeadingJetPt_Zexc2jet_TUnfold  variableDENOM=LepPtPlusLeadingJetPt_Zexc1jet_TUnfold # R21 exc
# # ./runUnfoldingZJetsRatios  year=2016  variableNUM=LepPtPlusLeadingJetPt_Zexc3jet_TUnfold  variableDENOM=LepPtPlusLeadingJetPt_Zexc2jet_TUnfold # R32 exc
# ./runUnfoldingZJetsRatios  year=2016  variableNUM=LepPtPlusLeadingJetPt_Zinc2jet_TUnfold  variableDENOM=LepPtPlusLeadingJetPt_Zexc1jet_TUnfold # R2inc/1exc
# # AK8 jets
# ./runUnfoldingZJetsRatios  year=2016  variableNUM=LepPtPlusLeadingJetAK8Pt_Zinc2jet_TUnfold  variableDENOM=LepPtPlusLeadingJetAK8Pt_Zinc1jet_TUnfold # R21
# # ./runUnfoldingZJetsRatios  year=2016  variableNUM=LepPtPlusLeadingJetAK8Pt_Zinc3jet_TUnfold  variableDENOM=LepPtPlusLeadingJetAK8Pt_Zinc2jet_TUnfold # R32
# # ./runUnfoldingZJetsRatios  year=2016  variableNUM=LepPtPlusLeadingJetAK8Pt_Zexc2jet_TUnfold  variableDENOM=LepPtPlusLeadingJetAK8Pt_Zexc1jet_TUnfold # R21 exc
# # ./runUnfoldingZJetsRatios  year=2016  variableNUM=LepPtPlusLeadingJetAK8Pt_Zexc3jet_TUnfold  variableDENOM=LepPtPlusLeadingJetAK8Pt_Zexc2jet_TUnfold # R32 exc
# ./runUnfoldingZJetsRatios  year=2016  variableNUM=LepPtPlusLeadingJetAK8Pt_Zinc2jet_TUnfold  variableDENOM=LepPtPlusLeadingJetAK8Pt_Zexc1jet_TUnfold # R2inc/1exc

