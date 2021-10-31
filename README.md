 === README for the W+jets unfolding code ===


mkdir WJetsUnfolding16_lxplus7_2020
cd WJetsUnfolding16_lxplus7_2020


CMSSW version: CMSSW_7_6_7
<set up folder structure using cmsrel, then cd into src, and cmsenv>

<then pull github repository>

Histogram files imported here after running event-selection should be in folders titled:
HistoFiles_<year>

NOTE: histogram files necessary to make unfolded xsec plots are located here:
/eos/cms/store/group/phys_smp/AnalysisFramework/Baobab/awisecar/wjetsRun2_histoFiles_forUnfoldedPlots

If moving histogram files from reco-level event selection code for first time, need to rename them to the proper format using (make sure to change the variable for the year of data-taking) --
./renameHistograms_NoBVeto.sh

Before compiling, can change certain source code files of interest --
1) Sources/getFilesAndHistogramsZJets.cc ==> allows you to turn off and on the running of different systematic uncertainties when unfolding the data
2) Includes/variablesOfInterestZJets.h ==> if you need to add additional observables/distributions to unfold, can add them here
3) Sources/PlotSettings.cc ==> allows you to change any TLatex labels, x-axis ranges, any other cosmetics concerning the final plot showing the unfolded data and the theoretical predictions

To compile executables, do --
make clean
make

Run shell script to unfold the data to measure the single-differential cross sections (currently uses the runUnfoldingCombinedRun2 macro to combine all three years of data at reco-level and unfold once) --
./unfoldXSecs.sh

And then for the cross section ratios (currently uses the runTakeRatiosOfUnfoldedXSecs macro to take ratios of the already-unfolded cross sections) --
./unfoldRatios.sh





=====

NOTE on histogram naming conventions:
- Variables unfolded using TUnfold software in this repository are named as having "_TUnfold" at the end of their name (this is done in the event selection code when setting up histograms)
- To reiterate, one needs four histogram objects for each distribution
--> <variable-name> (reco)
--> gen+<variable-name> (gen)
--> hresponse+<variable-name> (response matrix 1)
--> hresponse+<variable-name>+_NOEFFWEIGHTS (response matrix 2)

NOTE on code organization:
- the "run" executables are in the base directory of the repository
- source code is in "Includes" and "Sources"
- "Makefile" is used to compile the code
- files in "RooUnfold" are deprecated, are still in the repository from when RooUnfold software was used to do unfolding, however, the way the Makefile is set up, it grabs some libraries/dependencies in this folder (one should be able to fix this)
- "vjets.cfg" is essentially a steering file that is needed to run the code (was used more so in past versions of the Shears framework), shouldn’t have to alter this much




==========================================================================================================================================
>>>> Everything below is potentially deprecated, see above



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
 * also ttV
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

