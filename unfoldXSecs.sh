#!/bin/sh

# # closure test with full Run 2 data
# # AK4 jet kinematics
# ./runUnfoldingCombinedRun2 variable=FirstJetPt_Zinc1jet_TUnfold isClosureTest=true
# ./runUnfoldingCombinedRun2 variable=FirstJetAbsRapidity_Zinc1jet_TUnfold isClosureTest=true
# ./runUnfoldingCombinedRun2 variable=dPhiLepJet1_Zinc1jet_TUnfold isClosureTest=true
# ./runUnfoldingCombinedRun2 variable=SecondJetPt_Zinc2jet_TUnfold isClosureTest=true
# ./runUnfoldingCombinedRun2 variable=SecondJetAbsRapidity_Zinc2jet_TUnfold isClosureTest=true
# ./runUnfoldingCombinedRun2 variable=dPhiLepJet2_Zinc2jet_TUnfold isClosureTest=true
# ./runUnfoldingCombinedRun2 variable=dRapidityJets_Zinc2jet_TUnfold isClosureTest=true
# ./runUnfoldingCombinedRun2 variable=dRLepCloseJetCo300dR04_Zinc1jet_TUnfold isClosureTest=true
# ./runUnfoldingCombinedRun2 variable=dRLepCloseJetCo500dR04_Zinc1jet_TUnfold isClosureTest=true
# # AK8 jet kinematics
# ./runUnfoldingCombinedRun2 variable=FirstJetAK8Pt_Zinc1jet_TUnfold isClosureTest=true
# ./runUnfoldingCombinedRun2 variable=FirstJetAK8AbsRapidity_Zinc1jet_TUnfold isClosureTest=true
# ./runUnfoldingCombinedRun2 variable=dPhiLepJet1AK8_Zinc1jet_TUnfold isClosureTest=true
# ./runUnfoldingCombinedRun2 variable=SecondJetAK8Pt_Zinc2jet_TUnfold isClosureTest=true
# ./runUnfoldingCombinedRun2 variable=SecondJetAK8AbsRapidity_Zinc2jet_TUnfold isClosureTest=true
# ./runUnfoldingCombinedRun2 variable=dPhiLepJet2AK8_Zinc2jet_TUnfold isClosureTest=true
# ./runUnfoldingCombinedRun2 variable=dRapidityJetsAK8_Zinc2jet_TUnfold isClosureTest=true
# # alpha-s ratios
# ./runUnfoldingCombinedRun2 variable=LepPtPlusLeadingJetAK8Pt_Zinc1jet_TUnfold isClosureTest=true
# ./runUnfoldingCombinedRun2 variable=LepPtPlusLeadingJetAK8Pt_Zinc2jet_TUnfold isClosureTest=true


# ----------------------------------------------------------------------------------------



# # unfolding with 2018 only
# # AK4 jet kinematics
# ./runUnfoldingZJets year=2018 variable=FirstJetPt_Zinc1jet_TUnfold
# ./runUnfoldingZJets year=2018 variable=FirstJetAbsRapidity_Zinc1jet_TUnfold
# ./runUnfoldingZJets year=2018 variable=dPhiLepJet1_Zinc1jet_TUnfold
# ./runUnfoldingZJets year=2018 variable=SecondJetPt_Zinc2jet_TUnfold
# ./runUnfoldingZJets year=2018 variable=SecondJetAbsRapidity_Zinc2jet_TUnfold
# ./runUnfoldingZJets year=2018 variable=dPhiLepJet2_Zinc2jet_TUnfold
# ./runUnfoldingZJets year=2018 variable=dRapidityJets_Zinc2jet_TUnfold
# ./runUnfoldingZJets year=2018 variable=dRLepCloseJetCo300dR04_Zinc1jet_TUnfold
# ./runUnfoldingZJets year=2018 variable=dRLepCloseJetCo500dR04_Zinc1jet_TUnfold
# # AK8 jet kinematics
# ./runUnfoldingZJets year=2018 variable=FirstJetAK8Pt_Zinc1jet_TUnfold
# ./runUnfoldingZJets year=2018 variable=FirstJetAK8AbsRapidity_Zinc1jet_TUnfold
# ./runUnfoldingZJets year=2018 variable=dPhiLepJet1AK8_Zinc1jet_TUnfold
# ./runUnfoldingZJets year=2018 variable=SecondJetAK8Pt_Zinc2jet_TUnfold
# ./runUnfoldingZJets year=2018 variable=SecondJetAK8AbsRapidity_Zinc2jet_TUnfold
# ./runUnfoldingZJets year=2018 variable=dPhiLepJet2AK8_Zinc2jet_TUnfold
# ./runUnfoldingZJets year=2018 variable=dRapidityJetsAK8_Zinc2jet_TUnfold
# # alpha-s ratios
# ./runUnfoldingZJets year=2018 variable=LepPtPlusLeadingJetAK8Pt_Zinc1jet_TUnfold
# ./runUnfoldingZJets year=2018 variable=LepPtPlusLeadingJetAK8Pt_Zinc2jet_TUnfold


# # closure test with 2018 only
# # AK4 jet kinematics
# ./runUnfoldingZJets year=2018 variable=FirstJetPt_Zinc1jet_TUnfold isClosureTest=true
# ./runUnfoldingZJets year=2018 variable=FirstJetAbsRapidity_Zinc1jet_TUnfold isClosureTest=true
# ./runUnfoldingZJets year=2018 variable=dPhiLepJet1_Zinc1jet_TUnfold isClosureTest=true
# ./runUnfoldingZJets year=2018 variable=SecondJetPt_Zinc2jet_TUnfold isClosureTest=true
# ./runUnfoldingZJets year=2018 variable=SecondJetAbsRapidity_Zinc2jet_TUnfold isClosureTest=true
# ./runUnfoldingZJets year=2018 variable=dPhiLepJet2_Zinc2jet_TUnfold isClosureTest=true
# ./runUnfoldingZJets year=2018 variable=dRapidityJets_Zinc2jet_TUnfold isClosureTest=true
# ./runUnfoldingZJets year=2018 variable=dRLepCloseJetCo300dR04_Zinc1jet_TUnfold isClosureTest=true
# ./runUnfoldingZJets year=2018 variable=dRLepCloseJetCo500dR04_Zinc1jet_TUnfold isClosureTest=true
# # AK8 jet kinematics
# ./runUnfoldingZJets year=2018 variable=FirstJetAK8Pt_Zinc1jet_TUnfold isClosureTest=true
# ./runUnfoldingZJets year=2018 variable=FirstJetAK8AbsRapidity_Zinc1jet_TUnfold isClosureTest=true
# ./runUnfoldingZJets year=2018 variable=dPhiLepJet1AK8_Zinc1jet_TUnfold isClosureTest=true
# ./runUnfoldingZJets year=2018 variable=SecondJetAK8Pt_Zinc2jet_TUnfold isClosureTest=true
# ./runUnfoldingZJets year=2018 variable=SecondJetAK8AbsRapidity_Zinc2jet_TUnfold isClosureTest=true
# ./runUnfoldingZJets year=2018 variable=dPhiLepJet2AK8_Zinc2jet_TUnfold isClosureTest=true
# ./runUnfoldingZJets year=2018 variable=dRapidityJetsAK8_Zinc2jet_TUnfold isClosureTest=true
# # alpha-s ratios
# ./runUnfoldingZJets year=2018 variable=LepPtPlusLeadingJetAK8Pt_Zinc1jet_TUnfold isClosureTest=true
# ./runUnfoldingZJets year=2018 variable=LepPtPlusLeadingJetAK8Pt_Zinc2jet_TUnfold isClosureTest=true


# ----------------------------------------------------------------------------------------


# 11 January 21 --
# unfolding distributions with full Run 2 data
# AK4 jet kinematics
./runUnfoldingCombinedRun2 variable=FirstJetPt_Zinc1jet_TUnfold
./runUnfoldingCombinedRun2 variable=FirstJetAbsRapidity_Zinc1jet_TUnfold
./runUnfoldingCombinedRun2 variable=dPhiLepJet1_Zinc1jet_TUnfold
./runUnfoldingCombinedRun2 variable=SecondJetPt_Zinc2jet_TUnfold
./runUnfoldingCombinedRun2 variable=SecondJetAbsRapidity_Zinc2jet_TUnfold
./runUnfoldingCombinedRun2 variable=dPhiLepJet2_Zinc2jet_TUnfold
./runUnfoldingCombinedRun2 variable=dRapidityJets_Zinc2jet_TUnfold
./runUnfoldingCombinedRun2 variable=dRLepCloseJetCo300dR04_Zinc1jet_TUnfold
./runUnfoldingCombinedRun2 variable=dRLepCloseJetCo500dR04_Zinc1jet_TUnfold
# AK8 jet kinematics
./runUnfoldingCombinedRun2 variable=FirstJetAK8Pt_Zinc1jet_TUnfold
./runUnfoldingCombinedRun2 variable=FirstJetAK8AbsRapidity_Zinc1jet_TUnfold
./runUnfoldingCombinedRun2 variable=dPhiLepJet1AK8_Zinc1jet_TUnfold
./runUnfoldingCombinedRun2 variable=SecondJetAK8Pt_Zinc2jet_TUnfold
./runUnfoldingCombinedRun2 variable=SecondJetAK8AbsRapidity_Zinc2jet_TUnfold
./runUnfoldingCombinedRun2 variable=dPhiLepJet2AK8_Zinc2jet_TUnfold
./runUnfoldingCombinedRun2 variable=dRapidityJetsAK8_Zinc2jet_TUnfold
# alpha-s ratios
./runUnfoldingCombinedRun2 variable=LepPtPlusLeadingJetAK8Pt_Zinc1jet_TUnfold
./runUnfoldingCombinedRun2 variable=LepPtPlusLeadingJetAK8Pt_Zinc2jet_TUnfold
# ./runUnfoldingCombinedRun2 variable=LepPtPlusLeadingJetAK8Pt_Zinc3jet_TUnfold
# ./runUnfoldingCombinedRun2 variable=LepPtPlusHT2over2AK8_Zinc2jet_TUnfold
# ./runUnfoldingCombinedRun2 variable=LepPtPlusHT2over2AK8_Zinc3jet_TUnfold


# # 6 December 20 --
# # unfolding with 2016 only
# # AK4 jet kinematics
# ./runUnfoldingZJets year=2016 variable=FirstJetPt_Zinc1jet_TUnfold
# ./runUnfoldingZJets year=2016 variable=FirstJetAbsRapidity_Zinc1jet_TUnfold
# ./runUnfoldingZJets year=2016 variable=dPhiLepJet1_Zinc1jet_TUnfold
# ./runUnfoldingZJets year=2016 variable=SecondJetPt_Zinc2jet_TUnfold
# ./runUnfoldingZJets year=2016 variable=SecondJetAbsRapidity_Zinc2jet_TUnfold
# ./runUnfoldingZJets year=2016 variable=dPhiLepJet2_Zinc2jet_TUnfold
# ./runUnfoldingZJets year=2016 variable=dRapidityJets_Zinc2jet_TUnfold
# ./runUnfoldingZJets year=2016 variable=dRLepCloseJetCo300dR04_Zinc1jet_TUnfold
# ./runUnfoldingZJets year=2016 variable=dRLepCloseJetCo500dR04_Zinc1jet_TUnfold
# # AK8 jet kinematics
# ./runUnfoldingZJets year=2016 variable=FirstJetAK8Pt_Zinc1jet_TUnfold
# ./runUnfoldingZJets year=2016 variable=FirstJetAK8AbsRapidity_Zinc1jet_TUnfold
# ./runUnfoldingZJets year=2016 variable=dPhiLepJet1AK8_Zinc1jet_TUnfold
# ./runUnfoldingZJets year=2016 variable=SecondJetAK8Pt_Zinc2jet_TUnfold
# ./runUnfoldingZJets year=2016 variable=SecondJetAK8AbsRapidity_Zinc2jet_TUnfold
# ./runUnfoldingZJets year=2016 variable=dPhiLepJet2AK8_Zinc2jet_TUnfold
# ./runUnfoldingZJets year=2016 variable=dRapidityJetsAK8_Zinc2jet_TUnfold
# # alpha-s ratios
# ./runUnfoldingZJets year=2016 variable=LepPtPlusLeadingJetAK8Pt_Zinc1jet_TUnfold
# ./runUnfoldingZJets year=2016 variable=LepPtPlusLeadingJetAK8Pt_Zinc2jet_TUnfold
# # ./runUnfoldingZJets year=2016 variable=LepPtPlusLeadingJetAK8Pt_Zinc3jet_TUnfold
# # ./runUnfoldingZJets year=2016 variable=LepPtPlusHT2over2AK8_Zinc2jet_TUnfold
# # ./runUnfoldingZJets year=2016 variable=LepPtPlusHT2over2AK8_Zinc3jet_TUnfold




# ----------------------------------------------------------------------------------------




# # 19 November 20 --
# # unfolding distributions for ratio with full Run 2 data
# # AK4 jet kinematics
# ./runUnfoldingCombinedRun2 variable=FirstJetPt_Zinc1jet_TUnfold
# ./runUnfoldingCombinedRun2 variable=FirstJetAbsRapidity_Zinc1jet_TUnfold
# ./runUnfoldingCombinedRun2 variable=dPhiLepJet1_Zinc1jet_TUnfold
# ./runUnfoldingCombinedRun2 variable=SecondJetPt_Zinc2jet_TUnfold
# ./runUnfoldingCombinedRun2 variable=SecondJetAbsRapidity_Zinc2jet_TUnfold
# ./runUnfoldingCombinedRun2 variable=dPhiLepJet2_Zinc2jet_TUnfold
# # AK8 jet kinematics
# ./runUnfoldingCombinedRun2 variable=FirstJetAK8Pt_Zinc1jet_TUnfold
# ./runUnfoldingCombinedRun2 variable=FirstJetAK8AbsRapidity_Zinc1jet_TUnfold
# ./runUnfoldingCombinedRun2 variable=dPhiLepJet1AK8_Zinc1jet_TUnfold
# ./runUnfoldingCombinedRun2 variable=SecondJetAK8Pt_Zinc2jet_TUnfold
# ./runUnfoldingCombinedRun2 variable=SecondJetAK8AbsRapidity_Zinc2jet_TUnfold
# ./runUnfoldingCombinedRun2 variable=dPhiLepJet2AK8_Zinc2jet_TUnfold
# # alpha-s ratios
# ./runUnfoldingCombinedRun2 variable=LepPtPlusLeadingJetAK8Pt_Zinc1jet_TUnfold
# ./runUnfoldingCombinedRun2 variable=LepPtPlusLeadingJetAK8Pt_Zinc2jet_TUnfold
# ./runUnfoldingCombinedRun2 variable=LepPtPlusLeadingJetAK8Pt_Zinc3jet_TUnfold
# ./runUnfoldingCombinedRun2 variable=LepPtPlusHT2over2AK8_Zinc2jet_TUnfold
# ./runUnfoldingCombinedRun2 variable=LepPtPlusHT2over2AK8_Zinc3jet_TUnfold

# 13 November 20 --
# unfolding distributions with just 2016 data today
#./runUnfoldingZJets year=2016 variable=LepPtPlusLeadingJetAK8Pt_Zinc1jet_TUnfold
#./runUnfoldingZJets year=2016 variable=LepPtPlusLeadingJetAK8Pt_Zinc2jet_TUnfold
## ./runUnfoldingZJets year=2016 variable=LepPtPlusLeadingJetAK8Pt_Zinc3jet_TUnfold
#./runUnfoldingZJets year=2016 variable=LepPtPlusLeadingJetAK8Pt_Zexc1jet_TUnfold
#./runUnfoldingZJets year=2016 variable=LepPtPlusHT2over2AK8_Zinc2jet_TUnfold
## ./runUnfoldingZJets year=2016 variable=LepPtPlusHT2over2AK8_Zinc3jet_TUnfold

# # 30 July 20 --
# # NOTE: THESE ARE THE COMMANDS USED TO UNFOLD FULL RUN2 DATA AND THEN TAKE RATIOS OF UNFOLDED XSECS!!!
# # ----------------------------------------------------------------------------
# # AK4 jet kinematics
# ./runUnfoldingCombinedRun2 variable=FirstJetPt_Zinc1jet_TUnfold
# ./runUnfoldingCombinedRun2 variable=FirstJetAbsRapidity_Zinc1jet_TUnfold
# ./runUnfoldingCombinedRun2 variable=dPhiLepJet1_Zinc1jet_TUnfold
# ./runUnfoldingCombinedRun2 variable=SecondJetPt_Zinc2jet_TUnfold
# ./runUnfoldingCombinedRun2 variable=SecondJetAbsRapidity_Zinc2jet_TUnfold
# ./runUnfoldingCombinedRun2 variable=dPhiLepJet2_Zinc2jet_TUnfold
# # AK8 jet kinematics
# ./runUnfoldingCombinedRun2 variable=FirstJetAK8Pt_Zinc1jet_TUnfold
# ./runUnfoldingCombinedRun2 variable=FirstJetAK8AbsRapidity_Zinc1jet_TUnfold
# ./runUnfoldingCombinedRun2 variable=dPhiLepJet1AK8_Zinc1jet_TUnfold
# ./runUnfoldingCombinedRun2 variable=SecondJetAK8Pt_Zinc2jet_TUnfold
# ./runUnfoldingCombinedRun2 variable=SecondJetAK8AbsRapidity_Zinc2jet_TUnfold
# ./runUnfoldingCombinedRun2 variable=dPhiLepJet2AK8_Zinc2jet_TUnfold
# # alpha-s ratios
# ./runUnfoldingCombinedRun2 variable=LepPtPlusLeadingJetPt_Zinc1jet_TUnfold
# ./runUnfoldingCombinedRun2 variable=LepPtPlusLeadingJetPt_Zinc2jet_TUnfold
# # ./runUnfoldingCombinedRun2 variable=LepPtPlusLeadingJetPt_Zinc3jet_TUnfold
# ./runUnfoldingCombinedRun2 variable=LepPtPlusLeadingJetPt_Zexc1jet_TUnfold
# ./runUnfoldingCombinedRun2 variable=LepPtPlusLeadingJetAK8Pt_Zinc1jet_TUnfold
# ./runUnfoldingCombinedRun2 variable=LepPtPlusLeadingJetAK8Pt_Zinc2jet_TUnfold
# # ./runUnfoldingCombinedRun2 variable=LepPtPlusLeadingJetAK8Pt_Zinc3jet_TUnfold
# ./runUnfoldingCombinedRun2 variable=LepPtPlusLeadingJetAK8Pt_Zexc1jet_TUnfold
# # ----------------------------------------------------------------------------

# # 28 July 20 --
# # AK4 jet kinematics
# ./runUnfoldingZJets year=2016 variable=FirstJetPt_Zinc1jet_TUnfold
# ./runUnfoldingZJets year=2016 variable=FirstJetAbsRapidity_Zinc1jet_TUnfold
# ./runUnfoldingZJets year=2016 variable=dPhiLepJet1_Zinc1jet_TUnfold
# ./runUnfoldingZJets year=2016 variable=SecondJetPt_Zinc2jet_TUnfold
# ./runUnfoldingZJets year=2016 variable=SecondJetAbsRapidity_Zinc2jet_TUnfold
# ./runUnfoldingZJets year=2016 variable=dPhiLepJet2_Zinc2jet_TUnfold
# # AK8 jet kinematics
# ./runUnfoldingZJets year=2016 variable=FirstJetAK8Pt_Zinc1jet_TUnfold
# ./runUnfoldingZJets year=2016 variable=FirstJetAK8AbsRapidity_Zinc1jet_TUnfold
# ./runUnfoldingZJets year=2016 variable=dPhiLepJet1AK8_Zinc1jet_TUnfold
# ./runUnfoldingZJets year=2016 variable=SecondJetAK8Pt_Zinc2jet_TUnfold
# ./runUnfoldingZJets year=2016 variable=SecondJetAK8AbsRapidity_Zinc2jet_TUnfold
# ./runUnfoldingZJets year=2016 variable=dPhiLepJet2AK8_Zinc2jet_TUnfold
# # alpha-s ratios
# ./runUnfoldingZJets year=2016 variable=LepPtPlusLeadingJetPt_Zinc1jet_TUnfold
# ./runUnfoldingZJets year=2016 variable=LepPtPlusLeadingJetPt_Zinc2jet_TUnfold
# ./runUnfoldingZJets year=2016 variable=LepPtPlusLeadingJetPt_Zinc3jet_TUnfold
# ./runUnfoldingZJets year=2016 variable=LepPtPlusLeadingJetPt_Zexc1jet_TUnfold
# ./runUnfoldingZJets year=2016 variable=LepPtPlusLeadingJetAK8Pt_Zinc1jet_TUnfold
# ./runUnfoldingZJets year=2016 variable=LepPtPlusLeadingJetAK8Pt_Zinc2jet_TUnfold
# ./runUnfoldingZJets year=2016 variable=LepPtPlusLeadingJetAK8Pt_Zexc1jet_TUnfold

# 3 July 20 --
# ./runUnfoldingZJets year=2017 variable=FirstJetPt_Zinc1jet_TUnfold
# ./runUnfoldingZJets year=2017 variable=FirstJetAbsRapidity_Zinc1jet_TUnfold
# ./runUnfoldingZJets year=2017 variable=dPhiLepJet1_Zinc1jet_TUnfold
# ./runUnfoldingZJets year=2017 variable=LepPtPlusLeadingJetPt_Zinc1jet_TUnfold
# ./runUnfoldingZJets year=2017 variable=LepPtPlusLeadingJetPt_Zinc2jet_TUnfold
# ./runUnfoldingZJets year=2017 variable=LepPtPlusLeadingJetPt_Zinc3jet_TUnfold
# ./runUnfoldingZJets year=2017 variable=LepPtPlusLeadingJetPt_Zexc1jet_TUnfold
# ./runUnfoldingZJets year=2017 variable=LepPtPlusLeadingJetAK8Pt_Zinc1jet_TUnfold
# ./runUnfoldingZJets year=2017 variable=LepPtPlusLeadingJetAK8Pt_Zinc2jet_TUnfold

# 12 June 20 --
# unfolding data
# ./runUnfoldingZJets year=2016 variable=FirstJetPt_Zinc1jet_TUnfold
# ./runUnfoldingZJets year=2016 variable=FirstJetAbsRapidity_Zinc1jet_TUnfold
# ./runUnfoldingZJets year=2016 variable=dPhiLepJet1_Zinc1jet_TUnfold
# ./runUnfoldingZJets year=2016 variable=LepPtPlusLeadingJetPt_Zinc1jet_TUnfold
# ./runUnfoldingZJets year=2016 variable=LepPtPlusLeadingJetPt_Zinc2jet_TUnfold
# # ./runUnfoldingZJets year=2016 variable=LepPtPlusLeadingJetPt_Zinc3jet_TUnfold
# ./runUnfoldingZJets year=2016 variable=LepPtPlusLeadingJetPt_Zexc1jet_TUnfold
# ./runUnfoldingZJets year=2016 variable=LepPtPlusLeadingJetAK8Pt_Zinc1jet_TUnfold
# ./runUnfoldingZJets year=2016 variable=LepPtPlusLeadingJetAK8Pt_Zinc2jet_TUnfold
# closure test (using W+jets reco MC)
# ./runUnfoldingZJets year=2016 isClosureTest=true variable=FirstJetPt_Zinc1jet_TUnfold
# ./runUnfoldingZJets year=2016 isClosureTest=true variable=FirstJetAbsRapidity_Zinc1jet_TUnfold
# ./runUnfoldingZJets year=2016 isClosureTest=true variable=dPhiLepJet1_Zinc1jet_TUnfold
# ./runUnfoldingZJets year=2016 isClosureTest=true variable=LepPtPlusLeadingJetPt_Zinc1jet_TUnfold
# ./runUnfoldingZJets year=2016 isClosureTest=true variable=LepPtPlusLeadingJetPt_Zinc2jet_TUnfold
# ./runUnfoldingZJets year=2016 isClosureTest=true variable=LepPtPlusLeadingJetPt_Zinc3jet_TUnfold
# ./runUnfoldingZJets year=2016 isClosureTest=true variable=LepPtPlusLeadingJetPt_Zexc1jet_TUnfold
# ./runUnfoldingZJets year=2016 isClosureTest=true variable=LepPtPlusLeadingJetAK8Pt_Zinc1jet_TUnfold
# ./runUnfoldingZJets year=2016 isClosureTest=true variable=LepPtPlusLeadingJetAK8Pt_Zinc2jet_TUnfold


# 18 May 20 --
# ./runUnfoldingZJets year=2016 variable=FirstJetPt_Zinc1jet_TUnfold
# ./runUnfoldingZJets year=2016 variable=FirstJetAbsRapidity_Zinc1jet_TUnfold
# ./runUnfoldingZJets year=2016 variable=dPhiLepJet1_Zinc1jet_TUnfold
# ./runUnfoldingZJets year=2016 variable=LepPtPlusLeadingJetPt_Zinc1jet_TUnfold
# ./runUnfoldingZJets year=2016 variable=LepPtPlusLeadingJetPt_Zinc2jet_TUnfold
# ./runUnfoldingZJets year=2016 variable=LepPtPlusLeadingJetPt_Zinc3jet_TUnfold
# ./runUnfoldingZJets year=2016 variable=LepPtPlusLeadingJetPt_Zexc1jet_TUnfold
# ./runUnfoldingZJets year=2016 variable=LepPtPlusLeadingJetPt_Zexc2jet_TUnfold
# ./runUnfoldingZJets year=2016 variable=LepPtPlusLeadingJetPt_Zexc3jet_TUnfold
# ./runUnfoldingZJets year=2016 variable=LepPtPlusLeadingJetAK8Pt_Zinc1jet_TUnfold
# ./runUnfoldingZJets year=2016 variable=LepPtPlusLeadingJetAK8Pt_Zinc2jet_TUnfold
# ./runUnfoldingZJets year=2016 variable=LepPtPlusLeadingJetAK8Pt_Zinc3jet_TUnfold
# ./runUnfoldingZJets year=2016 variable=LepPtPlusLeadingJetAK8Pt_Zexc1jet_TUnfold
# ./runUnfoldingZJets year=2016 variable=LepPtPlusLeadingJetAK8Pt_Zexc2jet_TUnfold
# ./runUnfoldingZJets year=2016 variable=LepPtPlusLeadingJetAK8Pt_Zexc3jet_TUnfold


# 8 May 20 --
#./runUnfoldingZJets year=2016 variable=LepPtPlusLeadingJetPt_TUnfold_Zinc1jet 
#./runUnfoldingZJets year=2016 variable=LepPtPlusLeadingJetPt_TUnfold_Zinc2jet
#./runUnfoldingZJets year=2016 variable=LepPtPlusLeadingJetPt_TUnfold_Zinc3jet
#./runUnfoldingZJets year=2016 variable=FirstJetPt_Zinc1jet_TUnfold
#./runUnfoldingZJets year=2016 variable=FirstJetAbsRapidity_Zinc1jet_TUnfold
#./runUnfoldingZJets year=2016 variable=dPhiLepJet1_Zinc1jet_TUnfold
#./runUnfoldingZJets year=2017 variable=LepPtPlusLeadingJetPt_TUnfold_Zinc1jet 
#./runUnfoldingZJets year=2017 variable=LepPtPlusLeadingJetPt_TUnfold_Zinc2jet
#./runUnfoldingZJets year=2017 variable=LepPtPlusLeadingJetPt_TUnfold_Zinc3jet
#./runUnfoldingZJets year=2017 variable=FirstJetPt_Zinc1jet_TUnfold
#./runUnfoldingZJets year=2017 variable=FirstJetAbsRapidity_Zinc1jet_TUnfold
#./runUnfoldingZJets year=2017 variable=dPhiLepJet1_Zinc1jet_TUnfold
#./runUnfoldingZJets year=2018 variable=LepPtPlusLeadingJetPt_TUnfold_Zinc1jet 
#./runUnfoldingZJets year=2018 variable=LepPtPlusLeadingJetPt_TUnfold_Zinc2jet
#./runUnfoldingZJets year=2018 variable=LepPtPlusLeadingJetPt_TUnfold_Zinc3jet
#./runUnfoldingZJets year=2018 variable=FirstJetPt_Zinc1jet_TUnfold
#./runUnfoldingZJets year=2018 variable=FirstJetAbsRapidity_Zinc1jet_TUnfold
#./runUnfoldingZJets year=2018 variable=dPhiLepJet1_Zinc1jet_TUnfold
