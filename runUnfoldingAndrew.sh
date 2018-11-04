#!/bin/sh

#Running all of the relevant variables for alpha-s analysis
./runUnfoldingZJets lepSel=SMu variable=ZNGoodJets_Zinc
./runUnfoldingZJets lepSel=SMu variable=ZNGoodJets_Zexc
./runUnfoldingZJets lepSel=SMu variable=LeadingJetPt_Zinc1jet_R21
./runUnfoldingZJets lepSel=SMu variable=LeadingJetPt_Zinc2jet_R21
./runUnfoldingZJets lepSel=SMu variable=LeadingJetPt_Zinc2jet_R32
./runUnfoldingZJets lepSel=SMu variable=LeadingJetPt_Zinc3jet_R32
./runUnfoldingZJets lepSel=SMu variable=LeadingJetPt_Zinc3jet_R43
./runUnfoldingZJets lepSel=SMu variable=LeadingJetPt_Zinc4jet_R43
./runUnfoldingZJets lepSel=SMu variable=HTover2_Zinc2jet_R32
./runUnfoldingZJets lepSel=SMu variable=HTover2_Zinc3jet_R32
./runUnfoldingZJets lepSel=SMu variable=HTover2_Zinc3jet_R43
./runUnfoldingZJets lepSel=SMu variable=HTover2_Zinc4jet_R43
#./runUnfoldingZJets lepSel=SMu variable=HTover3_Zinc3jet_R43
#./runUnfoldingZJets lepSel=SMu variable=HTover3_Zinc4jet_R43
./runUnfoldingZJets lepSel=SMu variable=LepPtPlusLeadingJetPt_Zinc1jet_R21
./runUnfoldingZJets lepSel=SMu variable=LepPtPlusLeadingJetPt_Zinc2jet_R21
./runUnfoldingZJets lepSel=SMu variable=LepPtPlusLeadingJetPt_Zinc2jet_R32
./runUnfoldingZJets lepSel=SMu variable=LepPtPlusLeadingJetPt_Zinc3jet_R32
./runUnfoldingZJets lepSel=SMu variable=LepPtPlusLeadingJetPt_Zinc3jet_R43
./runUnfoldingZJets lepSel=SMu variable=LepPtPlusLeadingJetPt_Zinc4jet_R43
# ./runUnfoldingZJets lepSel=SMu variable=LepPtPlusHT_Zinc1jet_R21
# ./runUnfoldingZJets lepSel=SMu variable=LepPtPlusHT_Zinc2jet_R21
# ./runUnfoldingZJets lepSel=SMu variable=LepPtPlusHT_Zinc2jet_R32
# ./runUnfoldingZJets lepSel=SMu variable=LepPtPlusHT_Zinc3jet_R32
# ./runUnfoldingZJets lepSel=SMu variable=LepPtPlusHT_Zinc3jet_R43
# ./runUnfoldingZJets lepSel=SMu variable=LepPtPlusHT_Zinc4jet_R43
./runUnfoldingZJets lepSel=SMu variable=LepPtPlusHTover2_Zinc2jet_R32
./runUnfoldingZJets lepSel=SMu variable=LepPtPlusHTover2_Zinc3jet_R32
./runUnfoldingZJets lepSel=SMu variable=LepPtPlusHTover2_Zinc3jet_R43
./runUnfoldingZJets lepSel=SMu variable=LepPtPlusHTover2_Zinc4jet_R43
#./runUnfoldingZJets lepSel=SMu variable=LepPtPlusHTover3_Zinc3jet_R43
#./runUnfoldingZJets lepSel=SMu variable=LepPtPlusHTover3_Zinc4jet_R43

./runUnfoldingZJets lepSel=SMu variable=ZPt_Zinc1jet
./runUnfoldingZJets lepSel=SMu variable=ZPt_Zinc2jet
./runUnfoldingZJets lepSel=SMu variable=ZPt_Zinc3jet
./runUnfoldingZJets lepSel=SMu variable=ZPt_Zinc4jet
./runUnfoldingZJets lepSel=SMu variable=ZPtPlusLeadingJetPt_Zinc1jet
./runUnfoldingZJets lepSel=SMu variable=ZPtPlusLeadingJetPt_Zinc2jet
./runUnfoldingZJets lepSel=SMu variable=ZPtPlusLeadingJetPt_Zinc3jet
./runUnfoldingZJets lepSel=SMu variable=ZPtPlusLeadingJetPt_Zinc4jet
./runUnfoldingZJets lepSel=SMu variable=ZPtPlusHTover2_Zinc2jet
./runUnfoldingZJets lepSel=SMu variable=ZPtPlusHTover2_Zinc3jet
./runUnfoldingZJets lepSel=SMu variable=ZPtPlusHTover2_Zinc4jet

#./runUnfoldingZJets lepSel=SMu variable=JetsHT_Zinc1jet
#./runUnfoldingZJets lepSel=SMu variable=JetsHT_Zinc2jet
#./runUnfoldingZJets lepSel=SMu variable=FirstJetAbsRapidity_Zinc1jet

# Alternative binnings
# ./runUnfoldingZJets lepSel=SMu variable=LepPtPlusHT_1_Zinc1jet_R21
# ./runUnfoldingZJets lepSel=SMu variable=LepPtPlusHT_1_Zinc2jet_R21
# ./runUnfoldingZJets lepSel=SMu variable=LepPtPlusHT_1_Zinc2jet_R32
# ./runUnfoldingZJets lepSel=SMu variable=LepPtPlusHT_1_Zinc3jet_R32
# ./runUnfoldingZJets lepSel=SMu variable=LepPtPlusHT_1_Zinc3jet_R43
# ./runUnfoldingZJets lepSel=SMu variable=LepPtPlusHT_1_Zinc4jet_R43
# ./runUnfoldingZJets lepSel=SMu variable=LepPtPlusHTover2_1_Zinc2jet_R32
# ./runUnfoldingZJets lepSel=SMu variable=LepPtPlusHTover2_1_Zinc3jet_R32
# ./runUnfoldingZJets lepSel=SMu variable=LepPtPlusHTover2_1_Zinc3jet_R43
# ./runUnfoldingZJets lepSel=SMu variable=LepPtPlusHTover2_1_Zinc4jet_R43
# ./runUnfoldingZJets lepSel=SMu variable=LepPtPlusHTover3_1_Zinc3jet_R43
# ./runUnfoldingZJets lepSel=SMu variable=LepPtPlusHTover3_1_Zinc4jet_R43
# ./runUnfoldingZJets lepSel=SMu variable=LepPtPlusHT_2_Zinc1jet_R21
# ./runUnfoldingZJets lepSel=SMu variable=LepPtPlusHT_2_Zinc2jet_R21
# ./runUnfoldingZJets lepSel=SMu variable=LepPtPlusHT_2_Zinc2jet_R32
# ./runUnfoldingZJets lepSel=SMu variable=LepPtPlusHT_2_Zinc3jet_R32
# ./runUnfoldingZJets lepSel=SMu variable=LepPtPlusHT_2_Zinc3jet_R43
# ./runUnfoldingZJets lepSel=SMu variable=LepPtPlusHT_2_Zinc4jet_R43
# ./runUnfoldingZJets lepSel=SMu variable=LepPtPlusHTover2_2_Zinc2jet_R32
# ./runUnfoldingZJets lepSel=SMu variable=LepPtPlusHTover2_2_Zinc3jet_R32
# ./runUnfoldingZJets lepSel=SMu variable=LepPtPlusHTover2_2_Zinc3jet_R43
# ./runUnfoldingZJets lepSel=SMu variable=LepPtPlusHTover2_2_Zinc4jet_R43
# ./runUnfoldingZJets lepSel=SMu variable=LepPtPlusHTover3_2_Zinc3jet_R43
# ./runUnfoldingZJets lepSel=SMu variable=LepPtPlusHTover3_2_Zinc4jet_R43
# ./runUnfoldingZJets lepSel=SMu variable=LepPtPlusHT_3_Zinc1jet_R21
# ./runUnfoldingZJets lepSel=SMu variable=LepPtPlusHT_3_Zinc2jet_R21
# ./runUnfoldingZJets lepSel=SMu variable=LepPtPlusHT_3_Zinc2jet_R32
# ./runUnfoldingZJets lepSel=SMu variable=LepPtPlusHT_3_Zinc3jet_R32
# ./runUnfoldingZJets lepSel=SMu variable=LepPtPlusHT_3_Zinc3jet_R43
# ./runUnfoldingZJets lepSel=SMu variable=LepPtPlusHT_3_Zinc4jet_R43
# ./runUnfoldingZJets lepSel=SMu variable=LepPtPlusHTover2_3_Zinc2jet_R32
# ./runUnfoldingZJets lepSel=SMu variable=LepPtPlusHTover2_3_Zinc3jet_R32
# ./runUnfoldingZJets lepSel=SMu variable=LepPtPlusHTover2_3_Zinc3jet_R43
# ./runUnfoldingZJets lepSel=SMu variable=LepPtPlusHTover2_3_Zinc4jet_R43
# ./runUnfoldingZJets lepSel=SMu variable=LepPtPlusHTover3_3_Zinc3jet_R43
# ./runUnfoldingZJets lepSel=SMu variable=LepPtPlusHTover3_3_Zinc4jet_R43
# ./runUnfoldingZJets lepSel=SMu variable=LepPtPlusHT_4_Zinc1jet_R21
# ./runUnfoldingZJets lepSel=SMu variable=LepPtPlusHT_4_Zinc2jet_R21
# ./runUnfoldingZJets lepSel=SMu variable=LepPtPlusHT_4_Zinc2jet_R32
# ./runUnfoldingZJets lepSel=SMu variable=LepPtPlusHT_4_Zinc3jet_R32
# ./runUnfoldingZJets lepSel=SMu variable=LepPtPlusHT_4_Zinc3jet_R43
# ./runUnfoldingZJets lepSel=SMu variable=LepPtPlusHT_4_Zinc4jet_R43
# ./runUnfoldingZJets lepSel=SMu variable=LepPtPlusHTover2_4_Zinc2jet_R32
# ./runUnfoldingZJets lepSel=SMu variable=LepPtPlusHTover2_4_Zinc3jet_R32
# ./runUnfoldingZJets lepSel=SMu variable=LepPtPlusHTover2_4_Zinc3jet_R43
# ./runUnfoldingZJets lepSel=SMu variable=LepPtPlusHTover2_4_Zinc4jet_R43
# ./runUnfoldingZJets lepSel=SMu variable=LepPtPlusHTover3_4_Zinc3jet_R43
# ./runUnfoldingZJets lepSel=SMu variable=LepPtPlusHTover3_4_Zinc4jet_R43
