from WMCore.Configuration import Configuration
config = Configuration()
config.section_('General')
config.General.transferOutputs = True
config.General.transferLogs = True
config.General.requestName = 'ZJetsAna'
config.section_('JobType')
config.JobType.outputFiles = [ 'HistoFiles.tgz' ]
config.JobType.scriptArgs = ['cfg=vjets_silver_dmu_crab.cfg']
config.JobType.scriptExe = 'job_crab.sh'
config.JobType.pluginName = 'PrivateMC'
config.JobType.psetName = 'donothing_cfg.py'
config.JobType.inputFiles = [ 'runZJets_newformat', 'EfficiencyTables.tgz', 'vjets.cfg', 'RooUnfold/libRooUnfold.so', 'RooUnfold/RooUnfoldDict_rdict.pcm', 'ZNGoodJets_Zexc_ratio.root', 'SecondJetEta_2_Zinc2jet_ratio.root', 'FirstJetPt_2_Zinc1jet_ratio.root', 'ThirdJetPt_2_Zinc3jet_ratio.root',  'JetsHT_2_Zinc3jet_ratio.root', 'FirstJetEta_2_Zinc1jet_ratio.root', 'ThirdJetEta_2_Zinc3jet_ratio.root', 'JetsHT_2_Zinc2jet_ratio.root', 'SecondJetPt_2_Zinc2jet_ratio.root', 'JetsHT_2_Zinc1jet_ratio.root' ]
config.section_('Data')
config.Data.unitsPerJob = 1
config.Data.totalUnits = 19
config.Data.publication = False
config.Data.splitting = 'EventBased'
config.Data.outLFNDirBase = 'TO BE FILLED'
config.section_('User')
config.section_('Site')
config.Site.whitelist = ['T2_CH_CERN']
config.Site.storageSite = 'T2_CH_CERN'
