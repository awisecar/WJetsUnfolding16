#!/bin/sh


# =========================================================================================================


# # calculating ratio distributions with 2016 only
# ./runTakeRatiosOfUnfoldedXSecs year=2016  variableNUM=LepPtPlusLeadingJetAK8Pt_Zinc2jet_TUnfold  variableDENOM=LepPtPlusLeadingJetAK8Pt_Zinc1jet_TUnfold
# ./runTakeRatiosOfUnfoldedXSecs year=2016  variableNUM=LepPtPlusLeadingJetAK8Pt_Zinc3jet_TUnfold  variableDENOM=LepPtPlusLeadingJetAK8Pt_Zinc2jet_TUnfold
# ./runTakeRatiosOfUnfoldedXSecs year=2016  variableNUM=LepPtPlusLeadingJetAK8Pt_Zinc3jet_TUnfold  variableDENOM=LepPtPlusLeadingJetAK8Pt_Zinc1jet_TUnfold
# ./runTakeRatiosOfUnfoldedXSecs year=2016  variableNUM=LepPtPlusHT2over2AK8_Zinc3jet_TUnfold      variableDENOM=LepPtPlusHT2over2AK8_Zinc2jet_TUnfold


# =========================================================================================================

# calculating ratio distributions with full Run 2 data
./runTakeRatiosOfUnfoldedXSecs year=9999  variableNUM=LepPtPlusLeadingJetAK8Pt_Zinc2jet_TUnfold  variableDENOM=LepPtPlusLeadingJetAK8Pt_Zinc1jet_TUnfold
# ./runTakeRatiosOfUnfoldedXSecs year=9999  variableNUM=LepPtPlusLeadingJetAK8Pt_Zinc3jet_TUnfold  variableDENOM=LepPtPlusLeadingJetAK8Pt_Zinc2jet_TUnfold
# ./runTakeRatiosOfUnfoldedXSecs year=9999  variableNUM=LepPtPlusLeadingJetAK8Pt_Zinc3jet_TUnfold  variableDENOM=LepPtPlusLeadingJetAK8Pt_Zinc1jet_TUnfold
# ./runTakeRatiosOfUnfoldedXSecs year=9999  variableNUM=LepPtPlusLeadingJetAK8Pt_Zinc2jet_TUnfold  variableDENOM=LepPtPlusLeadingJetAK8Pt_Zexc1jet_TUnfold
# ./runTakeRatiosOfUnfoldedXSecs year=9999  variableNUM=LepPtPlusLeadingJetAK8Pt_Zexc2jet_TUnfold  variableDENOM=LepPtPlusLeadingJetAK8Pt_Zexc1jet_TUnfold
# ./runTakeRatiosOfUnfoldedXSecs year=9999  variableNUM=LepPtPlusHT2over2AK8_Zinc3jet_TUnfold      variableDENOM=LepPtPlusHT2over2AK8_Zinc2jet_TUnfold











# 13 November 20 --
# unfolding distributions with just 2016 data today
#./runTakeRatiosOfUnfoldedXSecs year=2016  variableNUM=LepPtPlusLeadingJetAK8Pt_Zinc2jet_TUnfold  variableDENOM=LepPtPlusLeadingJetAK8Pt_Zinc1jet_TUnfold # AK8, R21
#./runTakeRatiosOfUnfoldedXSecs year=2016  variableNUM=LepPtPlusLeadingJetAK8Pt_Zinc2jet_TUnfold  variableDENOM=LepPtPlusLeadingJetAK8Pt_Zexc1jet_TUnfold # AK8, R2inc/1exc

# 31 July 20
# # NOTE: THESE ARE THE COMMANDS USED TO UNFOLD FULL RUN2 DATA AND THEN TAKE RATIOS OF UNFOLDED XSECS!!!
# # ----------------------------------------------------------------------------
# ./runTakeRatiosOfUnfoldedXSecs year=9999  variableNUM=LepPtPlusLeadingJetPt_Zinc2jet_TUnfold     variableDENOM=LepPtPlusLeadingJetPt_Zinc1jet_TUnfold    # AK4, R21
# ./runTakeRatiosOfUnfoldedXSecs year=9999  variableNUM=LepPtPlusLeadingJetPt_Zinc2jet_TUnfold     variableDENOM=LepPtPlusLeadingJetPt_Zexc1jet_TUnfold    # AK4, R2inc/1exc
# ./runTakeRatiosOfUnfoldedXSecs year=9999  variableNUM=LepPtPlusLeadingJetAK8Pt_Zinc2jet_TUnfold  variableDENOM=LepPtPlusLeadingJetAK8Pt_Zinc1jet_TUnfold # AK8, R21
# ./runTakeRatiosOfUnfoldedXSecs year=9999  variableNUM=LepPtPlusLeadingJetAK8Pt_Zinc2jet_TUnfold  variableDENOM=LepPtPlusLeadingJetAK8Pt_Zexc1jet_TUnfold # AK8, R2inc/1exc
# # ----------------------------------------------------------------------------

# 28 July 20 --
# ./runUnfoldingZJetsRatios  year=2016  variableNUM=LepPtPlusLeadingJetPt_Zinc2jet_TUnfold     variableDENOM=LepPtPlusLeadingJetPt_Zinc1jet_TUnfold    # AK4, R21
# ./runUnfoldingZJetsRatios  year=2016  variableNUM=LepPtPlusLeadingJetPt_Zinc2jet_TUnfold     variableDENOM=LepPtPlusLeadingJetPt_Zexc1jet_TUnfold    # AK4, R2inc/1exc
# ./runUnfoldingZJetsRatios  year=2016  variableNUM=LepPtPlusLeadingJetAK8Pt_Zinc2jet_TUnfold  variableDENOM=LepPtPlusLeadingJetAK8Pt_Zinc1jet_TUnfold # AK8, R21
# ./runUnfoldingZJetsRatios  year=2016  variableNUM=LepPtPlusLeadingJetAK8Pt_Zinc2jet_TUnfold  variableDENOM=LepPtPlusLeadingJetAK8Pt_Zexc1jet_TUnfold # AK8, R2inc/1exc

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
