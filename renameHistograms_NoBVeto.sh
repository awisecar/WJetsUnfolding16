cd HistoFiles_2016
echo $PWD

########## Central

## Data and Signal MC --
mv SMu_13TeV_Data_dR_5311_List_EffiCorr_0_TrigCorr_1_Syst_0_JetPtMin_30_VarWidth.root            SMu_13TeV_Data_TrigCorr_1_Syst_0_JetPtMin_30_JetEtaMax_24.root
# mv SMu_13TeV_WJets_FxFx_012J_dR_5311_List_EffiCorr_1_TrigCorr_1_Syst_0_JetPtMin_30_VarWidth.root SMu_13TeV_WJetsToLN_UNFOLDING_FxFx_TrigCorr_1_Syst_0_JetPtMin_30_JetEtaMax_24.root

# note: just making a duplicate of the NLO FxFx prediction to make the code happy right now
cp SMu_13TeV_WJets_FxFx_012J_dR_5311_List_EffiCorr_1_TrigCorr_1_Syst_0_JetPtMin_30_VarWidth.root SMu_13TeV_WJetsToLN_UNFOLDING_FxFx_TrigCorr_1_Syst_0_JetPtMin_30_JetEtaMax_24.root
mv SMu_13TeV_WJets_FxFx_012J_dR_5311_List_EffiCorr_1_TrigCorr_1_Syst_0_JetPtMin_30_VarWidth.root SMu_13TeV_WJetsToLN_UNFOLDING_MLM_dR_TrigCorr_1_Syst_0_JetPtMin_30_JetEtaMax_24.root

## BG --
mv SMu_13TeV_DYJets50toInf_dR_5311_List_EffiCorr_1_TrigCorr_1_Syst_0_JetPtMin_30_VarWidth.root   SMu_13TeV_DYJets10toInf3_TrigCorr_1_Syst_0_JetPtMin_30_JetEtaMax_24.root
mv SMu_13TeV_DataQCD_dR_5311_List_EffiCorr_0_TrigCorr_1_Syst_0_JetPtMin_30_VarWidth.root         SMu_13TeV_QCD_TrigCorr_1_Syst_0_JetPtMin_30_JetEtaMax_24.root
mv SMu_13TeV_Top_dR_5311_List_EffiCorr_1_TrigCorr_1_Syst_0_JetPtMin_30_VarWidth.root             SMu_13TeV_Top_TrigCorr_1_Syst_0_JetPtMin_30_JetEtaMax_24.root
mv SMu_13TeV_TTJets_dR_5311_List_EffiCorr_1_TrigCorr_1_Syst_0_JetPtMin_30_VarWidth.root          SMu_13TeV_TT_TrigCorr_1_Syst_0_JetPtMin_30_JetEtaMax_24.root
mv SMu_13TeV_ttV_dR_5311_List_EffiCorr_1_TrigCorr_1_Syst_0_JetPtMin_30_VarWidth.root             SMu_13TeV_ttV_TrigCorr_1_Syst_0_JetPtMin_30_JetEtaMax_24.root
mv SMu_13TeV_WW_dR_5311_List_EffiCorr_1_TrigCorr_1_Syst_0_JetPtMin_30_VarWidth.root              SMu_13TeV_WWTo2L2Nu_TrigCorr_1_Syst_0_JetPtMin_30_JetEtaMax_24.root
mv SMu_13TeV_WZ_dR_5311_List_EffiCorr_1_TrigCorr_1_Syst_0_JetPtMin_30_VarWidth.root              SMu_13TeV_WZ_TrigCorr_1_Syst_0_JetPtMin_30_JetEtaMax_24.root
mv SMu_13TeV_ZZ_dR_5311_List_EffiCorr_1_TrigCorr_1_Syst_0_JetPtMin_30_VarWidth.root              SMu_13TeV_ZZ_TrigCorr_1_Syst_0_JetPtMin_30_JetEtaMax_24.root

