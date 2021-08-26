#!/bin/sh

# =========================================================================================================

# unfolding distributions with full Run 2 data
# AK4 jet kinematics
./runUnfoldingCombinedRun2 variable=FirstJetPt_Zinc1jet_TUnfold
./runUnfoldingCombinedRun2 variable=FirstJetAbsRapidity_Zinc1jet_TUnfold
./runUnfoldingCombinedRun2 variable=dPhiLepJet1_Zinc1jet_TUnfold
./runUnfoldingCombinedRun2 variable=SecondJetPt_Zinc2jet_TUnfold
./runUnfoldingCombinedRun2 variable=SecondJetAbsRapidity_Zinc2jet_TUnfold
./runUnfoldingCombinedRun2 variable=dPhiLepJet2_Zinc2jet_TUnfold
./runUnfoldingCombinedRun2 variable=dPhiJets_Zinc2jet_TUnfold
./runUnfoldingCombinedRun2 variable=dRapidityJets_Zinc2jet_TUnfold
./runUnfoldingCombinedRun2 variable=dRJets_Zinc2jet_TUnfold
./runUnfoldingCombinedRun2 variable=diJetMass_Zinc2jet_TUnfold
./runUnfoldingCombinedRun2 variable=dRLepCloseJetCo300dR04_Zinc1jet_TUnfold
./runUnfoldingCombinedRun2 variable=dRLepCloseJetCo500dR04_Zinc1jet_TUnfold
# AK8 jet kinematics
./runUnfoldingCombinedRun2 variable=FirstJetAK8Pt_Zinc1jet_TUnfold
./runUnfoldingCombinedRun2 variable=FirstJetAK8AbsRapidity_Zinc1jet_TUnfold
./runUnfoldingCombinedRun2 variable=dPhiLepJet1AK8_Zinc1jet_TUnfold
./runUnfoldingCombinedRun2 variable=SecondJetAK8Pt_Zinc2jet_TUnfold
./runUnfoldingCombinedRun2 variable=SecondJetAK8AbsRapidity_Zinc2jet_TUnfold
./runUnfoldingCombinedRun2 variable=dPhiLepJet2AK8_Zinc2jet_TUnfold
./runUnfoldingCombinedRun2 variable=dPhiJetsAK8_Zinc2jet_TUnfold
./runUnfoldingCombinedRun2 variable=dRapidityJetsAK8_Zinc2jet_TUnfold
./runUnfoldingCombinedRun2 variable=dRJetsAK8_Zinc2jet_TUnfold
./runUnfoldingCombinedRun2 variable=diJetAK8Mass_Zinc2jet_TUnfold
# alpha-s ratios
./runUnfoldingCombinedRun2 variable=LepPtPlusLeadingJetAK8Pt_Zinc1jet_TUnfold
./runUnfoldingCombinedRun2 variable=LepPtPlusLeadingJetAK8Pt_Zinc2jet_TUnfold
./runUnfoldingCombinedRun2 variable=LepPtPlusLeadingJetAK8Pt_Zinc3jet_TUnfold
./runUnfoldingCombinedRun2 variable=LepPtPlusLeadingJetAK8Pt_Zexc1jet_TUnfold
./runUnfoldingCombinedRun2 variable=LepPtPlusLeadingJetAK8Pt_Zexc2jet_TUnfold
./runUnfoldingCombinedRun2 variable=LepPtPlusHT2over2AK8_Zinc2jet_TUnfold
./runUnfoldingCombinedRun2 variable=LepPtPlusHT2over2AK8_Zinc3jet_TUnfold


# # closure test with full Run 2 data
# # AK4 jet kinematics
# ./runUnfoldingCombinedRun2 variable=FirstJetPt_Zinc1jet_TUnfold               isClosureTest=true
# ./runUnfoldingCombinedRun2 variable=FirstJetAbsRapidity_Zinc1jet_TUnfold      isClosureTest=true
# ./runUnfoldingCombinedRun2 variable=dPhiLepJet1_Zinc1jet_TUnfold              isClosureTest=true
# ./runUnfoldingCombinedRun2 variable=SecondJetPt_Zinc2jet_TUnfold              isClosureTest=true
# ./runUnfoldingCombinedRun2 variable=SecondJetAbsRapidity_Zinc2jet_TUnfold     isClosureTest=true
# ./runUnfoldingCombinedRun2 variable=dPhiLepJet2_Zinc2jet_TUnfold              isClosureTest=true
# ./runUnfoldingCombinedRun2 variable=dPhiJets_Zinc2jet_TUnfold                 isClosureTest=true
# ./runUnfoldingCombinedRun2 variable=dRapidityJets_Zinc2jet_TUnfold            isClosureTest=true
# ./runUnfoldingCombinedRun2 variable=dRJets_Zinc2jet_TUnfold                   isClosureTest=true
# ./runUnfoldingCombinedRun2 variable=diJetMass_Zinc2jet_TUnfold                isClosureTest=true
# ./runUnfoldingCombinedRun2 variable=dRLepCloseJetCo300dR04_Zinc1jet_TUnfold   isClosureTest=true
# ./runUnfoldingCombinedRun2 variable=dRLepCloseJetCo500dR04_Zinc1jet_TUnfold   isClosureTest=true
# # AK8 jet kinematics
# ./runUnfoldingCombinedRun2 variable=FirstJetAK8Pt_Zinc1jet_TUnfold            isClosureTest=true
# ./runUnfoldingCombinedRun2 variable=FirstJetAK8AbsRapidity_Zinc1jet_TUnfold   isClosureTest=true
# ./runUnfoldingCombinedRun2 variable=dPhiLepJet1AK8_Zinc1jet_TUnfold           isClosureTest=true
# ./runUnfoldingCombinedRun2 variable=SecondJetAK8Pt_Zinc2jet_TUnfold           isClosureTest=true
# ./runUnfoldingCombinedRun2 variable=SecondJetAK8AbsRapidity_Zinc2jet_TUnfold  isClosureTest=true
# ./runUnfoldingCombinedRun2 variable=dPhiLepJet2AK8_Zinc2jet_TUnfold           isClosureTest=true
# ./runUnfoldingCombinedRun2 variable=dPhiJetsAK8_Zinc2jet_TUnfold              isClosureTest=true
# ./runUnfoldingCombinedRun2 variable=dRapidityJetsAK8_Zinc2jet_TUnfold         isClosureTest=true
# ./runUnfoldingCombinedRun2 variable=dRJetsAK8_Zinc2jet_TUnfold                isClosureTest=true
# ./runUnfoldingCombinedRun2 variable=diJetAK8Mass_Zinc2jet_TUnfold             isClosureTest=true
# # alpha-s ratios
# ./runUnfoldingCombinedRun2 variable=LepPtPlusLeadingJetAK8Pt_Zinc1jet_TUnfold isClosureTest=true
# ./runUnfoldingCombinedRun2 variable=LepPtPlusLeadingJetAK8Pt_Zinc2jet_TUnfold isClosureTest=true
# ./runUnfoldingCombinedRun2 variable=LepPtPlusLeadingJetAK8Pt_Zinc3jet_TUnfold isClosureTest=true
# ./runUnfoldingCombinedRun2 variable=LepPtPlusLeadingJetAK8Pt_Zexc1jet_TUnfold isClosureTest=true
# ./runUnfoldingCombinedRun2 variable=LepPtPlusLeadingJetAK8Pt_Zexc2jet_TUnfold isClosureTest=true
# ./runUnfoldingCombinedRun2 variable=LepPtPlusHT2over2AK8_Zinc2jet_TUnfold     isClosureTest=true
# ./runUnfoldingCombinedRun2 variable=LepPtPlusHT2over2AK8_Zinc3jet_TUnfold     isClosureTest=true


# =========================================================================================================


# # unfolding with 2016 only
# # AK4 jet kinematics
# ./runUnfoldingZJets year=2016 variable=FirstJetPt_Zinc1jet_TUnfold
# ./runUnfoldingZJets year=2016 variable=FirstJetAbsRapidity_Zinc1jet_TUnfold
# ./runUnfoldingZJets year=2016 variable=dPhiLepJet1_Zinc1jet_TUnfold
# ./runUnfoldingZJets year=2016 variable=SecondJetPt_Zinc2jet_TUnfold
# ./runUnfoldingZJets year=2016 variable=SecondJetAbsRapidity_Zinc2jet_TUnfold
# ./runUnfoldingZJets year=2016 variable=dPhiLepJet2_Zinc2jet_TUnfold
# ./runUnfoldingZJets year=2016 variable=dPhiJets_Zinc2jet_TUnfold
# ./runUnfoldingZJets year=2016 variable=dRapidityJets_Zinc2jet_TUnfold
# ./runUnfoldingZJets year=2016 variable=dRJets_Zinc2jet_TUnfold
# ./runUnfoldingZJets year=2016 variable=diJetMass_Zinc2jet_TUnfold
# ./runUnfoldingZJets year=2016 variable=dRLepCloseJetCo300dR04_Zinc1jet_TUnfold
# ./runUnfoldingZJets year=2016 variable=dRLepCloseJetCo500dR04_Zinc1jet_TUnfold
# # AK8 jet kinematics
# ./runUnfoldingZJets year=2016 variable=FirstJetAK8Pt_Zinc1jet_TUnfold
# ./runUnfoldingZJets year=2016 variable=FirstJetAK8AbsRapidity_Zinc1jet_TUnfold
# ./runUnfoldingZJets year=2016 variable=dPhiLepJet1AK8_Zinc1jet_TUnfold
# ./runUnfoldingZJets year=2016 variable=SecondJetAK8Pt_Zinc2jet_TUnfold
# ./runUnfoldingZJets year=2016 variable=SecondJetAK8AbsRapidity_Zinc2jet_TUnfold
# ./runUnfoldingZJets year=2016 variable=dPhiLepJet2AK8_Zinc2jet_TUnfold
# ./runUnfoldingZJets year=2016 variable=dPhiJetsAK8_Zinc2jet_TUnfold
# ./runUnfoldingZJets year=2016 variable=dRapidityJetsAK8_Zinc2jet_TUnfold
# ./runUnfoldingZJets year=2016 variable=dRJetsAK8_Zinc2jet_TUnfold
# ./runUnfoldingZJets year=2016 variable=diJetAK8Mass_Zinc2jet_TUnfold
# # alpha-s ratios
# ./runUnfoldingZJets year=2016 variable=LepPtPlusLeadingJetAK8Pt_Zinc1jet_TUnfold
# ./runUnfoldingZJets year=2016 variable=LepPtPlusLeadingJetAK8Pt_Zinc2jet_TUnfold
# ./runUnfoldingZJets year=2016 variable=LepPtPlusLeadingJetAK8Pt_Zinc3jet_TUnfold
# ./runUnfoldingZJets year=2016 variable=LepPtPlusLeadingJetAK8Pt_Zexc1jet_TUnfold
# ./runUnfoldingZJets year=2016 variable=LepPtPlusLeadingJetAK8Pt_Zexc2jet_TUnfold
# ./runUnfoldingZJets year=2016 variable=LepPtPlusHT2over2AK8_Zinc2jet_TUnfold
# ./runUnfoldingZJets year=2016 variable=LepPtPlusHT2over2AK8_Zinc3jet_TUnfold


# # closure test with 2016 only
# # AK4 jet kinematics
# ./runUnfoldingZJets year=2016 variable=FirstJetPt_Zinc1jet_TUnfold               isClosureTest=true
# ./runUnfoldingZJets year=2016 variable=FirstJetAbsRapidity_Zinc1jet_TUnfold      isClosureTest=true
# ./runUnfoldingZJets year=2016 variable=dPhiLepJet1_Zinc1jet_TUnfold              isClosureTest=true
# ./runUnfoldingZJets year=2016 variable=SecondJetPt_Zinc2jet_TUnfold              isClosureTest=true
# ./runUnfoldingZJets year=2016 variable=SecondJetAbsRapidity_Zinc2jet_TUnfold     isClosureTest=true
# ./runUnfoldingZJets year=2016 variable=dPhiLepJet2_Zinc2jet_TUnfold              isClosureTest=true
# ./runUnfoldingZJets year=2016 variable=dPhiJets_Zinc2jet_TUnfold                 isClosureTest=true
# ./runUnfoldingZJets year=2016 variable=dRapidityJets_Zinc2jet_TUnfold            isClosureTest=true
# ./runUnfoldingZJets year=2016 variable=dRJets_Zinc2jet_TUnfold                   isClosureTest=true
# ./runUnfoldingZJets year=2016 variable=diJetMass_Zinc2jet_TUnfold                isClosureTest=true
# ./runUnfoldingZJets year=2016 variable=dRLepCloseJetCo300dR04_Zinc1jet_TUnfold   isClosureTest=true
# ./runUnfoldingZJets year=2016 variable=dRLepCloseJetCo500dR04_Zinc1jet_TUnfold   isClosureTest=true
# # AK8 jet kinematics
# ./runUnfoldingZJets year=2016 variable=FirstJetAK8Pt_Zinc1jet_TUnfold            isClosureTest=true
# ./runUnfoldingZJets year=2016 variable=FirstJetAK8AbsRapidity_Zinc1jet_TUnfold   isClosureTest=true
# ./runUnfoldingZJets year=2016 variable=dPhiLepJet1AK8_Zinc1jet_TUnfold           isClosureTest=true
# ./runUnfoldingZJets year=2016 variable=SecondJetAK8Pt_Zinc2jet_TUnfold           isClosureTest=true
# ./runUnfoldingZJets year=2016 variable=SecondJetAK8AbsRapidity_Zinc2jet_TUnfold  isClosureTest=true
# ./runUnfoldingZJets year=2016 variable=dPhiLepJet2AK8_Zinc2jet_TUnfold           isClosureTest=true
# ./runUnfoldingZJets year=2016 variable=dPhiJetsAK8_Zinc2jet_TUnfold              isClosureTest=true
# ./runUnfoldingZJets year=2016 variable=dRapidityJetsAK8_Zinc2jet_TUnfold         isClosureTest=true
# ./runUnfoldingZJets year=2016 variable=dRJetsAK8_Zinc2jet_TUnfold                isClosureTest=true
# ./runUnfoldingZJets year=2016 variable=diJetAK8Mass_Zinc2jet_TUnfold             isClosureTest=true
# # alpha-s ratios
# ./runUnfoldingZJets year=2016 variable=LepPtPlusLeadingJetAK8Pt_Zinc1jet_TUnfold isClosureTest=true
# ./runUnfoldingZJets year=2016 variable=LepPtPlusLeadingJetAK8Pt_Zinc2jet_TUnfold isClosureTest=true
# ./runUnfoldingZJets year=2016 variable=LepPtPlusLeadingJetAK8Pt_Zinc3jet_TUnfold isClosureTest=true
# ./runUnfoldingZJets year=2016 variable=LepPtPlusLeadingJetAK8Pt_Zexc1jet_TUnfold isClosureTest=true
# ./runUnfoldingZJets year=2016 variable=LepPtPlusLeadingJetAK8Pt_Zexc2jet_TUnfold isClosureTest=true
# ./runUnfoldingZJets year=2016 variable=LepPtPlusHT2over2AK8_Zinc2jet_TUnfold     isClosureTest=true
# ./runUnfoldingZJets year=2016 variable=LepPtPlusHT2over2AK8_Zinc3jet_TUnfold     isClosureTest=true


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
