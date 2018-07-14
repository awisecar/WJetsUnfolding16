Quick start
============

Set up a CMSSW environment:

```
   $ cmsrel CMSSW_7_4_11
   $ cd CMSSW_7_4_11
   $ cmsenv
   $ cd ..
```

Clone the project to your working directory:

    git clone https://github.com/awisecar/WJetsUnfolding16.git
    or (depending on how much coffee github has had today)
    git clone git@github.com:awisecar/WJetsUnfolding16.git

Build the code:

```
    $ cd WJets13TeVUnfolding
    $ make clean
    $ make
```

Configuration is stored in vjet.cfg. A commented example can be found in example.cfg
vjets.cfg is used for customizing the parameters, the one stored on git should be fine.
(Unless you have already your own configuration file, copy it from the example.cfg)

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
 * SMu_13TeV_WJets_FxFx_012J_dR_5311_List_EffiCorr_1_TrigCorr_1_Syst_0_JetPtMin_30_VarWidth_BVeto.root
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

