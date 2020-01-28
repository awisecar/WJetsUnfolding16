#!/bin/sh

#Correct distributions for TUnfold
##./runUnfoldingZJetsRatios lepSel=SMu variableNUM=LeadingJetPt_2_Zinc2jet variableDENOM=LeadingJetPt_2_Zinc1jet
##./runUnfoldingZJetsRatios lepSel=SMu variableNUM=LeadingJetPt_2_Zinc3jet variableDENOM=LeadingJetPt_2_Zinc2jet
##./runUnfoldingZJetsRatios lepSel=SMu variableNUM=LeadingJetPt_2_Zinc4jet variableDENOM=LeadingJetPt_2_Zinc3jet
##./runUnfoldingZJetsRatios lepSel=SMu variableNUM=HTover2_2_Zinc3jet variableDENOM=HTover2_2_Zinc2jet
##./runUnfoldingZJetsRatios lepSel=SMu variableNUM=HTover2_2_Zinc4jet variableDENOM=HTover2_2_Zinc3jet
##./runUnfoldingZJetsRatios lepSel=SMu variableNUM=LepPtPlusLeadingJetPt_2_Zinc2jet variableDENOM=LepPtPlusLeadingJetPt_2_Zinc1jet
##./runUnfoldingZJetsRatios lepSel=SMu variableNUM=LepPtPlusLeadingJetPt_2_Zinc3jet variableDENOM=LepPtPlusLeadingJetPt_2_Zinc2jet
##./runUnfoldingZJetsRatios lepSel=SMu variableNUM=LepPtPlusLeadingJetPt_2_Zinc4jet variableDENOM=LepPtPlusLeadingJetPt_2_Zinc3jet
##./runUnfoldingZJetsRatios lepSel=SMu variableNUM=LepPtPlusHTover2_2_Zinc3jet variableDENOM=LepPtPlusHTover2_2_Zinc2jet
##./runUnfoldingZJetsRatios lepSel=SMu variableNUM=LepPtPlusHTover2_2_Zinc4jet variableDENOM=LepPtPlusHTover2_2_Zinc3jet
##./runUnfoldingZJetsRatios lepSel=SMu variableNUM=ZPt_2_Zinc2jet variableDENOM=ZPt_2_Zinc1jet
##./runUnfoldingZJetsRatios lepSel=SMu variableNUM=ZPt_2_Zinc3jet variableDENOM=ZPt_2_Zinc2jet
##./runUnfoldingZJetsRatios lepSel=SMu variableNUM=ZPt_2_Zinc4jet variableDENOM=ZPt_2_Zinc3jet
##./runUnfoldingZJetsRatios lepSel=SMu variableNUM=ZPtPlusLeadingJetPt_2_Zinc2jet variableDENOM=ZPtPlusLeadingJetPt_2_Zinc1jet
##./runUnfoldingZJetsRatios lepSel=SMu variableNUM=ZPtPlusLeadingJetPt_2_Zinc3jet variableDENOM=ZPtPlusLeadingJetPt_2_Zinc2jet
##./runUnfoldingZJetsRatios lepSel=SMu variableNUM=ZPtPlusLeadingJetPt_2_Zinc4jet variableDENOM=ZPtPlusLeadingJetPt_2_Zinc3jet
##./runUnfoldingZJetsRatios lepSel=SMu variableNUM=ZPtPlusHTover2_2_Zinc3jet variableDENOM=ZPtPlusHTover2_2_Zinc2jet
##./runUnfoldingZJetsRatios lepSel=SMu variableNUM=ZPtPlusHTover2_2_Zinc4jet variableDENOM=ZPtPlusHTover2_2_Zinc3jet

#Getting the reco&gen distributions with the nominal binning to try 
#bin-by-bin particle-level corrections derived using W+jets signal MC
#9 May 2019
./runUnfoldingZJetsRatios lepSel=SMu variableNUM=LeadingJetPt_Zinc2jet variableDENOM=LeadingJetPt_Zinc1jet
./runUnfoldingZJetsRatios lepSel=SMu variableNUM=LepPtPlusLeadingJetPt_Zinc2jet variableDENOM=LepPtPlusLeadingJetPt_Zinc1jet
