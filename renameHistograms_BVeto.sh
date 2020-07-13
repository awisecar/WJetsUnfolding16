#!/bin/bash
cd HistoFiles_2017
echo $PWD

########## Central
### Data and Signal MC (NLO & LO) --
mv SMu_13TeV_Data_dR_5311_List_EffiCorr_0_TrigCorr_1_Syst_0_JetPtMin_30_VarWidth_BVeto.root            SMu_13TeV_Data_TrigCorr_1_Syst_0_JetPtMin_30_JetEtaMax_24.root
mv SMu_13TeV_WJets_FxFx_Wpt_dR_5311_List_EffiCorr_1_TrigCorr_1_Syst_0_JetPtMin_30_VarWidth_BVeto.root  SMu_13TeV_WJetsToLN_UNFOLDING_FxFx_TrigCorr_1_Syst_0_JetPtMin_30_JetEtaMax_24.root
# mv SMu_13TeV_WJets_FxFx_012J_dR_5311_List_EffiCorr_1_TrigCorr_1_Syst_0_JetPtMin_30_VarWidth_BVeto.root SMu_13TeV_WJetsToLN_UNFOLDING_FxFx_TrigCorr_1_Syst_0_JetPtMin_30_JetEtaMax_24.root
mv SMu_13TeV_WJets_MLM_dR_5311_List_EffiCorr_1_TrigCorr_1_Syst_0_JetPtMin_30_VarWidth_BVeto.root       SMu_13TeV_WJetsToLN_UNFOLDING_MLM_dR_TrigCorr_1_Syst_0_JetPtMin_30_JetEtaMax_24.root

### BG --
mv SMu_13TeV_DYJets50toInf_dR_5311_List_EffiCorr_1_TrigCorr_1_Syst_0_JetPtMin_30_VarWidth_BVeto.root   SMu_13TeV_DYJets10toInf3_TrigCorr_1_Syst_0_JetPtMin_30_JetEtaMax_24.root
mv SMu_13TeV_DataQCD_dR_5311_List_EffiCorr_0_TrigCorr_1_Syst_0_JetPtMin_30_VarWidth_BVeto.root         SMu_13TeV_QCD_TrigCorr_1_Syst_0_JetPtMin_30_JetEtaMax_24.root
mv SMu_13TeV_Top_dR_5311_List_EffiCorr_1_TrigCorr_1_Syst_0_JetPtMin_30_VarWidth_BVeto.root             SMu_13TeV_Top_TrigCorr_1_Syst_0_JetPtMin_30_JetEtaMax_24.root
mv SMu_13TeV_TTJets_dR_5311_List_EffiCorr_1_TrigCorr_1_Syst_0_JetPtMin_30_VarWidth_BVeto.root          SMu_13TeV_TT_TrigCorr_1_Syst_0_JetPtMin_30_JetEtaMax_24.root
mv SMu_13TeV_ttV_dR_5311_List_EffiCorr_1_TrigCorr_1_Syst_0_JetPtMin_30_VarWidth_BVeto.root             SMu_13TeV_ttV_TrigCorr_1_Syst_0_JetPtMin_30_JetEtaMax_24.root
mv SMu_13TeV_WW_dR_5311_List_EffiCorr_1_TrigCorr_1_Syst_0_JetPtMin_30_VarWidth_BVeto.root              SMu_13TeV_WWTo2L2Nu_TrigCorr_1_Syst_0_JetPtMin_30_JetEtaMax_24.root
mv SMu_13TeV_WZ_dR_5311_List_EffiCorr_1_TrigCorr_1_Syst_0_JetPtMin_30_VarWidth_BVeto.root              SMu_13TeV_WZ_TrigCorr_1_Syst_0_JetPtMin_30_JetEtaMax_24.root
mv SMu_13TeV_ZZ_dR_5311_List_EffiCorr_1_TrigCorr_1_Syst_0_JetPtMin_30_VarWidth_BVeto.root              SMu_13TeV_ZZ_TrigCorr_1_Syst_0_JetPtMin_30_JetEtaMax_24.root


# ############ Systematics
# #### Data - 2: JES
# mv SMu_13TeV_Data_dR_5311_List_EffiCorr_0_TrigCorr_1_Syst_2_Up_JetPtMin_30_VarWidth_BVeto.root SMu_13TeV_Data_TrigCorr_1_Syst_2_Up_JetPtMin_30_JetEtaMax_24.root
# mv SMu_13TeV_Data_dR_5311_List_EffiCorr_0_TrigCorr_1_Syst_2_Down_JetPtMin_30_VarWidth_BVeto.root SMu_13TeV_Data_TrigCorr_1_Syst_2_Down_JetPtMin_30_JetEtaMax_24.root
# ##
# #### WJets Signal - 1: PU, 4: JER, 5: LepSF, 6: BtagSF
# mv SMu_13TeV_WJets_FxFx_Wpt_dR_5311_List_EffiCorr_1_TrigCorr_1_Syst_1_Up_JetPtMin_30_VarWidth_BVeto.root SMu_13TeV_WJetsToLN_UNFOLDING_FxFx_TrigCorr_1_Syst_1_Up_JetPtMin_30_JetEtaMax_24.root
# mv SMu_13TeV_WJets_FxFx_Wpt_dR_5311_List_EffiCorr_1_TrigCorr_1_Syst_1_Down_JetPtMin_30_VarWidth_BVeto.root SMu_13TeV_WJetsToLN_UNFOLDING_FxFx_TrigCorr_1_Syst_1_Down_JetPtMin_30_JetEtaMax_24.root
# mv SMu_13TeV_WJets_FxFx_Wpt_dR_5311_List_EffiCorr_1_TrigCorr_1_Syst_4_Up_JetPtMin_30_VarWidth_BVeto.root SMu_13TeV_WJetsToLN_UNFOLDING_FxFx_TrigCorr_1_Syst_4_Up_JetPtMin_30_JetEtaMax_24.root
# mv SMu_13TeV_WJets_FxFx_Wpt_dR_5311_List_EffiCorr_1_TrigCorr_1_Syst_4_Down_JetPtMin_30_VarWidth_BVeto.root SMu_13TeV_WJetsToLN_UNFOLDING_FxFx_TrigCorr_1_Syst_4_Down_JetPtMin_30_JetEtaMax_24.root
# ##mv SMu_13TeV_WJets_FxFx_Wpt_dR_5311_List_EffiCorr_1_TrigCorr_1_Syst_5_Up_JetPtMin_30_VarWidth_BVeto.root SMu_13TeV_WJetsToLN_UNFOLDING_FxFx_TrigCorr_1_Syst_5_Up_JetPtMin_30_JetEtaMax_24.root
# ##mv SMu_13TeV_WJets_FxFx_Wpt_dR_5311_List_EffiCorr_1_TrigCorr_1_Syst_5_Down_JetPtMin_30_VarWidth_BVeto.root SMu_13TeV_WJetsToLN_UNFOLDING_FxFx_TrigCorr_1_Syst_5_Down_JetPtMin_30_JetEtaMax_24.root
# mv SMu_13TeV_WJets_FxFx_Wpt_dR_5311_List_EffiCorr_1_TrigCorr_1_Syst_6_Up_JetPtMin_30_VarWidth_BVeto.root SMu_13TeV_WJetsToLN_UNFOLDING_FxFx_TrigCorr_1_Syst_6_Up_JetPtMin_30_JetEtaMax_24.root
# mv SMu_13TeV_WJets_FxFx_Wpt_dR_5311_List_EffiCorr_1_TrigCorr_1_Syst_6_Down_JetPtMin_30_VarWidth_BVeto.root SMu_13TeV_WJetsToLN_UNFOLDING_FxFx_TrigCorr_1_Syst_6_Down_JetPtMin_30_JetEtaMax_24.root
# ##
# #### BG (7 backgrounds: TTbar, DYJets, Single Top, QCD, WW, WZ, ZZ)  -  1: PU, 3: XSEC, 5: LepSF, 6: BtagSF
# #### Note: for QCD we assume negligible change in systematic variation so we just copy the central one
# ##
# #### Syst 1 - PU
# mv SMu_13TeV_TTJets_dR_5311_List_EffiCorr_1_TrigCorr_1_Syst_1_Up_JetPtMin_30_VarWidth_BVeto.root SMu_13TeV_TT_TrigCorr_1_Syst_1_Up_JetPtMin_30_JetEtaMax_24.root
# mv SMu_13TeV_TTJets_dR_5311_List_EffiCorr_1_TrigCorr_1_Syst_1_Down_JetPtMin_30_VarWidth_BVeto.root SMu_13TeV_TT_TrigCorr_1_Syst_1_Down_JetPtMin_30_JetEtaMax_24.root
# mv SMu_13TeV_DYJets50toInf_dR_5311_List_EffiCorr_1_TrigCorr_1_Syst_1_Up_JetPtMin_30_VarWidth_BVeto.root SMu_13TeV_DYJets10toInf3_TrigCorr_1_Syst_1_Up_JetPtMin_30_JetEtaMax_24.root
# mv SMu_13TeV_DYJets50toInf_dR_5311_List_EffiCorr_1_TrigCorr_1_Syst_1_Down_JetPtMin_30_VarWidth_BVeto.root SMu_13TeV_DYJets10toInf3_TrigCorr_1_Syst_1_Down_JetPtMin_30_JetEtaMax_24.root
# mv SMu_13TeV_Top_dR_5311_List_EffiCorr_1_TrigCorr_1_Syst_1_Up_JetPtMin_30_VarWidth_BVeto.root SMu_13TeV_Top_TrigCorr_1_Syst_1_Up_JetPtMin_30_JetEtaMax_24.root
# mv SMu_13TeV_Top_dR_5311_List_EffiCorr_1_TrigCorr_1_Syst_1_Down_JetPtMin_30_VarWidth_BVeto.root SMu_13TeV_Top_TrigCorr_1_Syst_1_Down_JetPtMin_30_JetEtaMax_24.root
# mv SMu_13TeV_WW_dR_5311_List_EffiCorr_1_TrigCorr_1_Syst_1_Up_JetPtMin_30_VarWidth_BVeto.root SMu_13TeV_WWTo2L2Nu_TrigCorr_1_Syst_1_Up_JetPtMin_30_JetEtaMax_24.root
# mv SMu_13TeV_WW_dR_5311_List_EffiCorr_1_TrigCorr_1_Syst_1_Down_JetPtMin_30_VarWidth_BVeto.root SMu_13TeV_WWTo2L2Nu_TrigCorr_1_Syst_1_Down_JetPtMin_30_JetEtaMax_24.root
# mv SMu_13TeV_WZ_dR_5311_List_EffiCorr_1_TrigCorr_1_Syst_1_Up_JetPtMin_30_VarWidth_BVeto.root SMu_13TeV_WZ_TrigCorr_1_Syst_1_Up_JetPtMin_30_JetEtaMax_24.root
# mv SMu_13TeV_WZ_dR_5311_List_EffiCorr_1_TrigCorr_1_Syst_1_Down_JetPtMin_30_VarWidth_BVeto.root SMu_13TeV_WZ_TrigCorr_1_Syst_1_Down_JetPtMin_30_JetEtaMax_24.root
# mv SMu_13TeV_ZZ_dR_5311_List_EffiCorr_1_TrigCorr_1_Syst_1_Up_JetPtMin_30_VarWidth_BVeto.root SMu_13TeV_ZZ_TrigCorr_1_Syst_1_Up_JetPtMin_30_JetEtaMax_24.root
# mv SMu_13TeV_ZZ_dR_5311_List_EffiCorr_1_TrigCorr_1_Syst_1_Down_JetPtMin_30_VarWidth_BVeto.root SMu_13TeV_ZZ_TrigCorr_1_Syst_1_Down_JetPtMin_30_JetEtaMax_24.root
# cp SMu_13TeV_QCD_TrigCorr_1_Syst_0_JetPtMin_30_JetEtaMax_24.root SMu_13TeV_QCD_TrigCorr_1_Syst_1_Up_JetPtMin_30_JetEtaMax_24.root
# cp SMu_13TeV_QCD_TrigCorr_1_Syst_0_JetPtMin_30_JetEtaMax_24.root SMu_13TeV_QCD_TrigCorr_1_Syst_1_Down_JetPtMin_30_JetEtaMax_24.root
# ##
# #### Syst 3 - XSEC
# mv SMu_13TeV_TTJets_dR_5311_List_EffiCorr_1_TrigCorr_1_Syst_3_Up_JetPtMin_30_VarWidth_BVeto.root SMu_13TeV_TT_TrigCorr_1_Syst_3_Up_JetPtMin_30_JetEtaMax_24.root
# mv SMu_13TeV_TTJets_dR_5311_List_EffiCorr_1_TrigCorr_1_Syst_3_Down_JetPtMin_30_VarWidth_BVeto.root SMu_13TeV_TT_TrigCorr_1_Syst_3_Down_JetPtMin_30_JetEtaMax_24.root
# mv SMu_13TeV_DYJets50toInf_dR_5311_List_EffiCorr_1_TrigCorr_1_Syst_3_Up_JetPtMin_30_VarWidth_BVeto.root SMu_13TeV_DYJets10toInf3_TrigCorr_1_Syst_3_Up_JetPtMin_30_JetEtaMax_24.root
# mv SMu_13TeV_DYJets50toInf_dR_5311_List_EffiCorr_1_TrigCorr_1_Syst_3_Down_JetPtMin_30_VarWidth_BVeto.root SMu_13TeV_DYJets10toInf3_TrigCorr_1_Syst_3_Down_JetPtMin_30_JetEtaMax_24.root
# mv SMu_13TeV_Top_dR_5311_List_EffiCorr_1_TrigCorr_1_Syst_3_Up_JetPtMin_30_VarWidth_BVeto.root SMu_13TeV_Top_TrigCorr_1_Syst_3_Up_JetPtMin_30_JetEtaMax_24.root
# mv SMu_13TeV_Top_dR_5311_List_EffiCorr_1_TrigCorr_1_Syst_3_Down_JetPtMin_30_VarWidth_BVeto.root SMu_13TeV_Top_TrigCorr_1_Syst_3_Down_JetPtMin_30_JetEtaMax_24.root
# mv SMu_13TeV_WW_dR_5311_List_EffiCorr_1_TrigCorr_1_Syst_3_Up_JetPtMin_30_VarWidth_BVeto.root SMu_13TeV_WWTo2L2Nu_TrigCorr_1_Syst_3_Up_JetPtMin_30_JetEtaMax_24.root
# mv SMu_13TeV_WW_dR_5311_List_EffiCorr_1_TrigCorr_1_Syst_3_Down_JetPtMin_30_VarWidth_BVeto.root SMu_13TeV_WWTo2L2Nu_TrigCorr_1_Syst_3_Down_JetPtMin_30_JetEtaMax_24.root
# mv SMu_13TeV_WZ_dR_5311_List_EffiCorr_1_TrigCorr_1_Syst_3_Up_JetPtMin_30_VarWidth_BVeto.root SMu_13TeV_WZ_TrigCorr_1_Syst_3_Up_JetPtMin_30_JetEtaMax_24.root
# mv SMu_13TeV_WZ_dR_5311_List_EffiCorr_1_TrigCorr_1_Syst_3_Down_JetPtMin_30_VarWidth_BVeto.root SMu_13TeV_WZ_TrigCorr_1_Syst_3_Down_JetPtMin_30_JetEtaMax_24.root
# mv SMu_13TeV_ZZ_dR_5311_List_EffiCorr_1_TrigCorr_1_Syst_3_Up_JetPtMin_30_VarWidth_BVeto.root SMu_13TeV_ZZ_TrigCorr_1_Syst_3_Up_JetPtMin_30_JetEtaMax_24.root
# mv SMu_13TeV_ZZ_dR_5311_List_EffiCorr_1_TrigCorr_1_Syst_3_Down_JetPtMin_30_VarWidth_BVeto.root SMu_13TeV_ZZ_TrigCorr_1_Syst_3_Down_JetPtMin_30_JetEtaMax_24.root
# cp SMu_13TeV_QCD_TrigCorr_1_Syst_0_JetPtMin_30_JetEtaMax_24.root SMu_13TeV_QCD_TrigCorr_1_Syst_3_Up_JetPtMin_30_JetEtaMax_24.root
# cp SMu_13TeV_QCD_TrigCorr_1_Syst_0_JetPtMin_30_JetEtaMax_24.root SMu_13TeV_QCD_TrigCorr_1_Syst_3_Down_JetPtMin_30_JetEtaMax_24.root
# ##
# ###### Syst 5 - LepSF
# ####mv SMu_13TeV_TTJets_dR_5311_List_EffiCorr_1_TrigCorr_1_Syst_5_Up_JetPtMin_30_VarWidth_BVeto.root SMu_13TeV_TT_TrigCorr_1_Syst_5_Up_JetPtMin_30_JetEtaMax_24.root
# ####mv SMu_13TeV_TTJets_dR_5311_List_EffiCorr_1_TrigCorr_1_Syst_5_Down_JetPtMin_30_VarWidth_BVeto.root SMu_13TeV_TT_TrigCorr_1_Syst_5_Down_JetPtMin_30_JetEtaMax_24.root
# ####mv SMu_13TeV_DYJets50toInf_dR_5311_List_EffiCorr_1_TrigCorr_1_Syst_5_Up_JetPtMin_30_VarWidth_BVeto.root SMu_13TeV_DYJets10toInf3_TrigCorr_1_Syst_5_Up_JetPtMin_30_JetEtaMax_24.root
# ####mv SMu_13TeV_DYJets50toInf_dR_5311_List_EffiCorr_1_TrigCorr_1_Syst_5_Down_JetPtMin_30_VarWidth_BVeto.root SMu_13TeV_DYJets10toInf3_TrigCorr_1_Syst_5_Down_JetPtMin_30_JetEtaMax_24.root
# ####mv SMu_13TeV_Top_dR_5311_List_EffiCorr_1_TrigCorr_1_Syst_5_Up_JetPtMin_30_VarWidth_BVeto.root SMu_13TeV_Top_TrigCorr_1_Syst_5_Up_JetPtMin_30_JetEtaMax_24.root
# ####mv SMu_13TeV_Top_dR_5311_List_EffiCorr_1_TrigCorr_1_Syst_5_Down_JetPtMin_30_VarWidth_BVeto.root SMu_13TeV_Top_TrigCorr_1_Syst_5_Down_JetPtMin_30_JetEtaMax_24.root
# ####mv SMu_13TeV_WW_dR_5311_List_EffiCorr_1_TrigCorr_1_Syst_5_Up_JetPtMin_30_VarWidth_BVeto.root SMu_13TeV_WWTo2L2Nu_TrigCorr_1_Syst_5_Up_JetPtMin_30_JetEtaMax_24.root
# ####mv SMu_13TeV_WW_dR_5311_List_EffiCorr_1_TrigCorr_1_Syst_5_Down_JetPtMin_30_VarWidth_BVeto.root SMu_13TeV_WWTo2L2Nu_TrigCorr_1_Syst_5_Down_JetPtMin_30_JetEtaMax_24.root
# ####mv SMu_13TeV_WZ_dR_5311_List_EffiCorr_1_TrigCorr_1_Syst_5_Up_JetPtMin_30_VarWidth_BVeto.root SMu_13TeV_WZ_TrigCorr_1_Syst_5_Up_JetPtMin_30_JetEtaMax_24.root
# ####mv SMu_13TeV_WZ_dR_5311_List_EffiCorr_1_TrigCorr_1_Syst_5_Down_JetPtMin_30_VarWidth_BVeto.root SMu_13TeV_WZ_TrigCorr_1_Syst_5_Down_JetPtMin_30_JetEtaMax_24.root
# ####mv SMu_13TeV_ZZ_dR_5311_List_EffiCorr_1_TrigCorr_1_Syst_5_Up_JetPtMin_30_VarWidth_BVeto.root SMu_13TeV_ZZ_TrigCorr_1_Syst_5_Up_JetPtMin_30_JetEtaMax_24.root
# ####mv SMu_13TeV_ZZ_dR_5311_List_EffiCorr_1_TrigCorr_1_Syst_5_Down_JetPtMin_30_VarWidth_BVeto.root SMu_13TeV_ZZ_TrigCorr_1_Syst_5_Down_JetPtMin_30_JetEtaMax_24.root
# ####cp SMu_13TeV_QCD_TrigCorr_1_Syst_0_JetPtMin_30_JetEtaMax_24.root SMu_13TeV_QCD_TrigCorr_1_Syst_5_Down_JetPtMin_30_JetEtaMax_24.root
# ####cp SMu_13TeV_QCD_TrigCorr_1_Syst_0_JetPtMin_30_JetEtaMax_24.root SMu_13TeV_QCD_TrigCorr_1_Syst_5_Up_JetPtMin_30_JetEtaMax_24.root
# ##
# #### Syst 6 - BtagSF
# mv SMu_13TeV_TTJets_dR_5311_List_EffiCorr_1_TrigCorr_1_Syst_6_Up_JetPtMin_30_VarWidth_BVeto.root SMu_13TeV_TT_TrigCorr_1_Syst_6_Up_JetPtMin_30_JetEtaMax_24.root
# mv SMu_13TeV_TTJets_dR_5311_List_EffiCorr_1_TrigCorr_1_Syst_6_Down_JetPtMin_30_VarWidth_BVeto.root SMu_13TeV_TT_TrigCorr_1_Syst_6_Down_JetPtMin_30_JetEtaMax_24.root
# mv SMu_13TeV_DYJets50toInf_dR_5311_List_EffiCorr_1_TrigCorr_1_Syst_6_Up_JetPtMin_30_VarWidth_BVeto.root SMu_13TeV_DYJets10toInf3_TrigCorr_1_Syst_6_Up_JetPtMin_30_JetEtaMax_24.root
# mv SMu_13TeV_DYJets50toInf_dR_5311_List_EffiCorr_1_TrigCorr_1_Syst_6_Down_JetPtMin_30_VarWidth_BVeto.root SMu_13TeV_DYJets10toInf3_TrigCorr_1_Syst_6_Down_JetPtMin_30_JetEtaMax_24.root
# mv SMu_13TeV_Top_dR_5311_List_EffiCorr_1_TrigCorr_1_Syst_6_Up_JetPtMin_30_VarWidth_BVeto.root SMu_13TeV_Top_TrigCorr_1_Syst_6_Up_JetPtMin_30_JetEtaMax_24.root
# mv SMu_13TeV_Top_dR_5311_List_EffiCorr_1_TrigCorr_1_Syst_6_Down_JetPtMin_30_VarWidth_BVeto.root SMu_13TeV_Top_TrigCorr_1_Syst_6_Down_JetPtMin_30_JetEtaMax_24.root
# mv SMu_13TeV_WW_dR_5311_List_EffiCorr_1_TrigCorr_1_Syst_6_Up_JetPtMin_30_VarWidth_BVeto.root SMu_13TeV_WWTo2L2Nu_TrigCorr_1_Syst_6_Up_JetPtMin_30_JetEtaMax_24.root
# mv SMu_13TeV_WW_dR_5311_List_EffiCorr_1_TrigCorr_1_Syst_6_Down_JetPtMin_30_VarWidth_BVeto.root SMu_13TeV_WWTo2L2Nu_TrigCorr_1_Syst_6_Down_JetPtMin_30_JetEtaMax_24.root
# mv SMu_13TeV_WZ_dR_5311_List_EffiCorr_1_TrigCorr_1_Syst_6_Up_JetPtMin_30_VarWidth_BVeto.root SMu_13TeV_WZ_TrigCorr_1_Syst_6_Up_JetPtMin_30_JetEtaMax_24.root
# mv SMu_13TeV_WZ_dR_5311_List_EffiCorr_1_TrigCorr_1_Syst_6_Down_JetPtMin_30_VarWidth_BVeto.root SMu_13TeV_WZ_TrigCorr_1_Syst_6_Down_JetPtMin_30_JetEtaMax_24.root
# mv SMu_13TeV_ZZ_dR_5311_List_EffiCorr_1_TrigCorr_1_Syst_6_Up_JetPtMin_30_VarWidth_BVeto.root SMu_13TeV_ZZ_TrigCorr_1_Syst_6_Up_JetPtMin_30_JetEtaMax_24.root
# mv SMu_13TeV_ZZ_dR_5311_List_EffiCorr_1_TrigCorr_1_Syst_6_Down_JetPtMin_30_VarWidth_BVeto.root SMu_13TeV_ZZ_TrigCorr_1_Syst_6_Down_JetPtMin_30_JetEtaMax_24.root
# cp SMu_13TeV_QCD_TrigCorr_1_Syst_0_JetPtMin_30_JetEtaMax_24.root SMu_13TeV_QCD_TrigCorr_1_Syst_6_Down_JetPtMin_30_JetEtaMax_24.root
# cp SMu_13TeV_QCD_TrigCorr_1_Syst_0_JetPtMin_30_JetEtaMax_24.root SMu_13TeV_QCD_TrigCorr_1_Syst_6_Up_JetPtMin_30_JetEtaMax_24.root
