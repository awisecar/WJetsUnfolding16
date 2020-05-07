Quick start
============

Set up a CMSSW environment:

```
   $ cmsrel CMSSW_7_6_7
   $ cd CMSSW_7_6_7/src
   $ cmsenv
```

Clone the project to your working directory:

    git clone https://github.com/awisecar/WJetsUnfolding16.git
    or ---
    git clone git@github.com:awisecar/WJetsUnfolding16.git

Build the code:

```
    $ cd WJetsUnfolding16
    $ make clean
    $ make
```

Configuration is stored in vjet.cfg. A commented example can be found in example.cfg
vjets.cfg is used for customizing the parameters, the one stored on git should be fine.

Note: vjet.cfg file not used currently?? (6.3.2018)

Make sure to run 
./renameHisto.sh
to get files in format required by fileNamesZJets.h(?)

If you want to change systematics that are run (currently just doing JES), make sure to edit the appropriate lines in getFilesAndHistogramsZJets.cc

Rename all of the histograms from WJetsTreeAnalysis to the names needed for this code --
./renameHisto.sh 

Files needed to run this code:
 * SMu_13TeV_Data_dR_5311_List_EffiCorr_0_TrigCorr_1_Syst_0_JetPtMin_30_VarWidth_BVeto.root
 * SMu_13TeV_DataQCD_dR_5311_List_EffiCorr_0_TrigCorr_1_Syst_0_JetPtMin_30_VarWidth_BVeto.root
 * SMu_13TeV_DYJets50toInf_dR_5311_List_EffiCorr_1_TrigCorr_1_Syst_0_JetPtMin_30_VarWidth_BVeto.root
 * SMu_13TeV_Top_dR_5311_List_EffiCorr_1_TrigCorr_1_Syst_0_JetPtMin_30_VarWidth_BVeto.root
 * SMu_13TeV_TTJets_dR_5311_List_EffiCorr_1_TrigCorr_1_Syst_0_JetPtMin_30_VarWidth_BVeto.root
 * SMu_13TeV_WJets_FxFx_Wpt_dR_5311_List_EffiCorr_1_TrigCorr_1_Syst_0_JetPtMin_30_VarWidth_BVeto.root
 * SMu_13TeV_WJets_MLM_dR_5311_List_EffiCorr_1_TrigCorr_1_Syst_0_JetPtMin_30_VarWidth_BVeto.root
 * SMu_13TeV_WW_dR_5311_List_EffiCorr_1_TrigCorr_1_Syst_0_JetPtMin_30_VarWidth_BVeto.root
 * SMu_13TeV_WZ_dR_5311_List_EffiCorr_1_TrigCorr_1_Syst_0_JetPtMin_30_VarWidth_BVeto.root
 * SMu_13TeV_ZZ_dR_5311_List_EffiCorr_1_TrigCorr_1_Syst_0_JetPtMin_30_VarWidth_BVeto.root

If you need to add/change the way systematics are computed, edit the getFiles function in
Sources/getFilesAndHistogramsZJets.cc

Run the unfolding
=================

Before running the unfolding you need to produce the histogran for all the variations of the systematic sources. 
The directory HistoFiles containing these histogram root files should be present.


```
./runUnfoldingZJets lepSel=SMu variable=ZNGoodJetsFull_Zexc
```

The unfolded result can then be found in the UnfoldedFiles directory and the control plots in the UnfoldingCheck.

Can also runUnfoldingAndrew.sh to run script for all variables relevant to alpha-s analysis.

If you need to add new variables to unfold that are not already present, edit
Includes/variablesOfInterestZJets.h


Code organization
=================

The directory contains several runXXX.cc files, which have been compiles in executable called runXXX. Each of these executable runs the code implemented in the class XXX with the corresponding name and defined in Includes/XXX.h and Sources/XXX.cc.  The list of runXXX applications is provided below.

=====================================================================================

compile software with --
make clean && make

merge files in each folder first (do for all three years) --
./renameHistograms_NoBVeto.sh

./runUnfoldingZJets year=2016 variable=LepPtPlusLeadingJetPt_TUnfold_Zinc1jet 
./runUnfoldingZJets year=2016 variable=LepPtPlusLeadingJetPt_TUnfold_Zinc2jet
./runUnfoldingZJets year=2016 variable=LepPtPlusLeadingJetPt_TUnfold_Zinc3jet
./runUnfoldingZJets year=2016 variable=FirstJetPt_Zinc1jet_TUnfold
./runUnfoldingZJets year=2016 variable=FirstJetAbsRapidity_Zinc1jet_TUnfold
./runUnfoldingZJets year=2016 variable=dPhiLepJet1_Zinc1jet_TUnfold
./runUnfoldingZJets year=2017 variable=LepPtPlusLeadingJetPt_TUnfold_Zinc1jet 
./runUnfoldingZJets year=2017 variable=LepPtPlusLeadingJetPt_TUnfold_Zinc2jet
./runUnfoldingZJets year=2017 variable=LepPtPlusLeadingJetPt_TUnfold_Zinc3jet
./runUnfoldingZJets year=2017 variable=FirstJetPt_Zinc1jet_TUnfold
./runUnfoldingZJets year=2017 variable=FirstJetAbsRapidity_Zinc1jet_TUnfold
./runUnfoldingZJets year=2017 variable=dPhiLepJet1_Zinc1jet_TUnfold
./runUnfoldingZJets year=2018 variable=LepPtPlusLeadingJetPt_TUnfold_Zinc1jet 
./runUnfoldingZJets year=2018 variable=LepPtPlusLeadingJetPt_TUnfold_Zinc2jet
./runUnfoldingZJets year=2018 variable=LepPtPlusLeadingJetPt_TUnfold_Zinc3jet
./runUnfoldingZJets year=2018 variable=FirstJetPt_Zinc1jet_TUnfold
./runUnfoldingZJets year=2018 variable=FirstJetAbsRapidity_Zinc1jet_TUnfold
./runUnfoldingZJets year=2018 variable=dPhiLepJet1_Zinc1jet_TUnfold


./runCombineUnfoldedRun2 variable=FirstJetPt_Zinc1jet_TUnfold
./runCombineUnfoldedRun2 variable=FirstJetAbsRapidity_Zinc1jet_TUnfold
./runCombineUnfoldedRun2 variable=dPhiLepJet1_Zinc1jet_TUnfold
./runCombineUnfoldedRun2 variable=LepPtPlusLeadingJetPt_TUnfold_Zinc1jet
./runCombineUnfoldedRun2 variable=LepPtPlusLeadingJetPt_TUnfold_Zinc2jet
./runCombineUnfoldedRun2 variable=LepPtPlusLeadingJetPt_TUnfold_Zinc3jet

