//-*- c-basic-offset: 4; -*-
#include <iostream>
#include <thread>
#include <TString.h>
#include "ArgParser.h"
#include "ConfigVJets.h"
#include "ZJets_newformat.h"

//--- Load configuration ---
ConfigVJets cfg;

int main(int argc, char **argv)
{
    //-----------------------------------------------------------------------

    //--- Settings ---
    // doWhat = "DATA", "BACKGROUND", "TAU", "DYJETS", 
    //          "WJETS", "ALL", "PDF", "SHERPA"

    TString dataBonzaiDir  = cfg.getS("dataBonzaiDir");
    TString mcBonzaiDir  = cfg.getS("mcBonzaiDir");
    TString histoDir   = cfg.getS("histoDir", "HistoFiles");
    TString lepSel     = cfg.getS("lepSel", "DMu");
    TString doWhat     = cfg.getS("doWhat", "DYJETS");
    int lepPtMin       = cfg.getI("lepPtMin", 20);
    int lepEtaMax      = cfg.getI("lepEtaMax", 24);
    int jetPtMin       = cfg.getI("jetPtMin", 30);
    int jetEtaMax      = cfg.getI("jetEtaMax", 24);
    int whichSyst      = cfg.getI("whichSyst", -1);
    double muR         = cfg.getD("muR", 0);
    double muF         = cfg.getD("muF", 0);
    bool doSysRunning  = cfg.getB("doSysRunning", 0);
    bool doCentral     = cfg.getB("doCentral", 1);
    Int_t maxEvents   = cfg.getL("maxEvents", -1);
    Int_t maxFiles    = cfg.getL("maxFiles", -1);
    int jobNum         = cfg.getI("jobNum", 1);
    int nJobs          = cfg.getI("nJobs", 1);
    double mcYieldScale = cfg.getD("mcYieldScale", 1.);
    
    //    TString dataSample        = cfg.getS("dataSample"       , "%s_Data_13TeV.txt");
    //    TString mcSample_ST_tW    = cfg.getS("mcSample_ST_tW"   , "%s_ST_tW_top_13TeV.txt");
    //    TString mcSample_STbar_tW = cfg.getS("mcSample_STbar_tW", "%s_ST_tW_ant_13TeV.txt");
    //    TString mcSample_ST_tch   = cfg.getS("mcSample_ST_tch"  , "%s_ST_t-chan_13TeV.txt");
    //    TString mcSample_ST_sch   = cfg.getS("mcSample_ST_sch"  , "%s_ST_s-chan_13TeV.txt");
    //    TString mcSample_TT       = cfg.getS("mcSample_TT"      , "%s_TT_TuneCU_13TeV.txt");
    //    TString mcSample_ZZ       = cfg.getS("mcSample_ZZ"      , "%s_ZZ_TuneCU_13TeV.txt");
    //    TString mcSample_WW       = cfg.getS("mcSample_WW"      , "%s_WWTo2L2Nu_13TeV.txt");
    //    TString mcSample_WZ       = cfg.getS("mcSample_WZ"      , "%s_WZJets_powheg_13TeV.txt");
    //    TString mcSample_W        = cfg.getS("mcSample_W"       , "%s_WJetsToLN_13TeV.txt");
    //    TString mcSample_DY       = cfg.getS("mcSample_DY"      , "%s_DYJetsToLL_v3_amcatnlo_13TeV.txt");
    //
    //    TString dataSampleLabel        = cfg.getS("dataSampleLabel"       , "Data_dR");
    //    TString mcSampleLabel_ST_tW    = cfg.getS("mcSampleLabel_ST_tW"   , "T_tW_channel_dR");
    //    TString mcSampleLabel_STbar_tW = cfg.getS("mcSampleLabel_STbar_tW", "Tbar_tW_channel_dR");
    //    TString mcSampleLabel_ST_tch   = cfg.getS("mcSampleLabel_ST_tch"  , "T_t_channel_dR");
    //    TString mcSampleLabel_ST_sch   = cfg.getS("mcSampleLabel_ST_sch"  , "Tbar_s_channel_dR");
    //    TString mcSampleLabel_TT       = cfg.getS("mcSampleLabel_TT"      , "TTJets_DR");
    //    TString mcSampleLabel_ZZ       = cfg.getS("mcSampleLabel_ZZ"      , "ZZ_TuneCU");
    //    TString mcSampleLabel_WW       = cfg.getS("mcSampleLabel_WW"      , "WWTo2L2Nu");
    //    TString mcSampleLabel_WZ       = cfg.getS("mcSampleLabel_WZ"      , "WZJets");
    //    TString mcSampleLabel_W        = cfg.getS("mcSampleLabel_W"       , "WJetsToLN");
    //    TString mcSampleLabel_DY       = cfg.getS("mcSampleLabel_DY"      , "DYJets");

    //--- save config to .vjets.cfg ---
    cfg.writeConfigVJets(".vjets.cfg");
    //----------------------------------------------------------------------

    //--- Parse the arguments -----------------------------------------------------
    if (argc > 1) {
        for (int i = 1; i < argc; ++i) {
            TString currentArg = argv[i];
            //--- possible options ---
            if (currentArg.BeginsWith("dataBonzaiDir=")) {
                getArg(currentArg, dataBonzaiDir);
		cfg.set("dataBonzaiDir", dataBonzaiDir);
            }
            else if (currentArg.BeginsWith("mcBonzaiDir=")) {
                getArg(currentArg, mcBonzaiDir);
		cfg.set("mcBonzaiDir", mcBonzaiDir);
            }
            else if (currentArg.BeginsWith("histoDir")) {
                getArg(currentArg, histoDir);
		cfg.set("histoDir", histoDir);
            }
            else if (currentArg.BeginsWith("lepSel=")) {
                getArg(currentArg, lepSel);
		cfg.set("lepSel", lepSel);
            }
            else if (currentArg.BeginsWith("doWhat=")) {
                getArg(currentArg, doWhat);
		cfg.set("doWhat", doWhat);
            }
            else if (currentArg.BeginsWith("whichSyst=")) {
                getArg(currentArg, whichSyst);
		cfg.set("whichSyst", whichSyst);
            }
            else if (currentArg.BeginsWith("lepPtMin=")) {
                getArg(currentArg, lepPtMin);
		cfg.set("lepPtMin", lepPtMin);
            }
            else if (currentArg.BeginsWith("lepEtaMax=")) {
                getArg(currentArg, lepEtaMax);
		cfg.set("lepEtaMax", lepEtaMax);
            }
            else if (currentArg.BeginsWith("jetPtMin=")) {
                getArg(currentArg, jetPtMin);
		cfg.set("jetPtMin", jetPtMin);
            }
            else if (currentArg.BeginsWith("jetEtaMax=")) {
                getArg(currentArg, jetEtaMax);
		cfg.set("jetEtaMax", jetEtaMax);
            }
            else if (currentArg.BeginsWith("muR=")) {
                getArg(currentArg, muR);
		cfg.set("muR", muR);
            }
            else if (currentArg.BeginsWith("muF=")) {
                getArg(currentArg, muF);
		cfg.set("muF", muF);
            }
            else if (currentArg.BeginsWith("doSysRunning=")) {
                getArg(currentArg, doSysRunning);
		cfg.set("doSysRunning", doSysRunning);
            }
            else if (currentArg.BeginsWith("doCentral=")) {
                getArg(currentArg, doCentral);
		cfg.set("doCentral", doCentral);
            }
            else if (currentArg.BeginsWith("maxEvents=")) {
                getArg(currentArg, maxEvents);
		cfg.set("maxEvents", maxEvents);
            }
	    else if (currentArg.BeginsWith("maxFiles=")) {
                getArg(currentArg, maxFiles);
		cfg.set("maxFiles", maxFiles);
            }
	    else if (currentArg.BeginsWith("jobNum=")) {
                getArg(currentArg, jobNum);
		cfg.set("jobNum", jobNum);
            }
	    else if (currentArg.BeginsWith("nJobs=")) {
                getArg(currentArg, nJobs);
		cfg.set("nJobs", nJobs);
            }
	    else if (currentArg.BeginsWith("mcYieldScale=")) {
                getArg(currentArg, mcYieldScale);
		cfg.set("mcYieldScale", mcYieldScale);
            }
            //--- asking for help ---

            //--- asking for help ---
            else if (currentArg.Contains("help") || currentArg.BeginsWith("-h")) {
                std::cout << "\nUsage: ./runZJets [dataBonzaiDir=(path)] [mcBonzaiDir=(path)] [histoDir=(path)] [lepSel=(DMu, DE)] [algo=(Bayes, SVD)] [lepPtMin=(int)] [lepEtaMax=(int*10)] [jetPtMin=(int)] [jetEtaMax=(int*10)] ";
                std::cout << "[doWhat=(sample)] [doSysRunning=(1,0)] [doCentral=(1,0)] [maxEvents=-1|N] [--help]" << std::endl;
                std::cout << "eg: ./runZJets lepSel=DMu jetEtaMax=24" << std::endl;
                std::cout << "unspecified options will be read from vjets.cfg\n" << std::endl;
                return 0;
            }
            //--- bad option ---
            else {
                std::cerr << "Warning: unknown option \"" << currentArg << "\"" << std::endl;
                std::cerr << "Please issue ./runZJets --help for more information on possible options" << std::endl;
                return 0;
            }
        }
    }

    if (maxEvents > 0) {
        histoDir.Remove(TString::kTrailing, '/');
        histoDir += TString::Format("_%devts/", maxEvents);
        cout << "Output directory (histoDir) has been changed to " << histoDir << endl;
    }
    
    if (!histoDir.EndsWith("/")) histoDir += "/";
    doWhat.ToUpper();

    //-----------------------------------------------------------------------------

    double lumi = cfg.getD("lumi", -1);

    const unsigned NSystData     = 3;
    const unsigned NSystMC       = 7;
    const unsigned NSystMCSignal = 9;
    const unsigned NSystMax      = NSystMCSignal;

    short dataSyst[NSystData] = {0, 2, 2};
    short dataDir[NSystData]  = {0,-1, 1};

    short bgSyst[NSystMC]   = {0, 1, 1,    3,    3, 5, 5};
    short bgDir[NSystMC]    = {0,-1, 1,   -1,    1,-1, 1};

    short mcSignalSyst[NSystMCSignal]   = {0, 1, 1, 4, 4, 5, 5, 6, 6};
    short mcSignalDir[NSystMCSignal]    = {0,-1, 1,-1, 1,-1, 1,-1, 1};

    TString pdfSet("");
    int pdfMember = -1;
    
    //----------------------------------------------------------------------
    
    //Reads integrated luminosity from data file catalog:
    if(lumi < 0){
	TString input = cfg.getS("sample_Data");
	if(input.Length()==0){
	    std::cerr << "Data catalog is not defined (sample_Data). "
		"Integrated luminosity set to 1/fb fallback value.\n";
	    lumi = 1.;
	} else{
	    TString fullPath;
	    lumi = 0;
	    ZJets::canonizeInputFilePath(dataBonzaiDir, TString::Format(input, lepSel.Data()),
					 &fullPath);
	    ZJets::readCatalog(fullPath, dataBonzaiDir, 0, &lumi);
	    if(lumi > 0){
		std::cout << "Lumi read from catalog: " << lumi << std::endl;
	    } else{
		std::cerr << "Integrated luminosity was not found. The value 1 /fb will be used." << std::endl;
		lumi = 1.;
	    }
	}
    }
    const int kNominal = 0;
    int trigCorr = 1;
    bool hasRecoInfo = true;
    bool hasGenInfo  = true;
    short* syst   = 0;
    short* systDir = 0;
    TString bonzaiDir;
    double yieldScale = 1.;
    std::vector<std::string> tomerge;
    for (unsigned int iSyst = 0; iSyst < NSystMax; iSyst++) {
	if (whichSyst < 0){
	    if (iSyst == kNominal && !doCentral) continue;
	    if (iSyst != kNominal && !doSysRunning) continue;
	} else{
	    if (iSyst != (unsigned)whichSyst) continue;
	}

	for(unsigned iSample = 0; iSample < NSamples; ++iSample){
	    if(iSample==DATA){
		if (doWhat != "DATA" && doWhat != "ALL") continue;
		if(iSyst >= NSystData) continue;
		hasRecoInfo = true;
		hasGenInfo   = false;
		syst = dataSyst;
		systDir = dataDir;
		bonzaiDir = dataBonzaiDir;
		yieldScale = 1.;
	    } else if(iSample==DYJETS){//signal MC
		hasRecoInfo = true;
		hasGenInfo  = true;
		syst = mcSignalSyst;
		systDir = mcSignalDir;
		bonzaiDir = mcBonzaiDir;
		yieldScale = mcYieldScale;
		if (doWhat != "DYJETS" && doWhat != "ALL") continue;
		if(iSyst >= NSystMCSignal) continue;				
	    } else if(iSample < DYJETS){ //background MC
		hasRecoInfo = true;
		hasGenInfo   = false;
		syst    = bgSyst;
		systDir = bgDir;
		bonzaiDir = mcBonzaiDir;
		yieldScale = mcYieldScale;
		if (doWhat != "BACKGROUND" && doWhat != "ALL") continue;
		if(iSyst >= NSystMC) continue;
	    } else {//alternative DY
		hasRecoInfo = true;
		if (doWhat != "MG_MLM" && doWhat != "ALL") continue;
		hasGenInfo  = true;
		syst = mcSignalSyst;
		systDir = mcSignalDir;
		bonzaiDir = mcBonzaiDir;
		yieldScale = mcYieldScale;
		if (doWhat != "DYJETS" && doWhat != "ALL") continue;
		if(iSyst != 0) continue;
	    }
	
	    if(Samples[iSample].merge == '='){//sample is a merge of previous one
		const TString mergedFile = ZJets::CreateOutputFileName(pdfSet, pdfMember, muR, muF,
								       nJobs > 1 ? jobNum : -1,
								       lepSel, Samples[iSample].name,
								       trigCorr, syst[iSyst], systDir[iSyst],
								       jetPtMin, jetEtaMax = 24, histoDir);
		std::cout << "Merging files ";
		for(unsigned ifile = 0; ifile < tomerge.size(); ++ifile){
		    std::cout << (ifile ? ", " : "" ) << tomerge[ifile];
		}
		std::cout << " into file " << mergedFile << std::endl;
	
		mergeHistFiles(tomerge, mergedFile.Data());
		tomerge.clear();
		continue;
	    } //.merge == '='
	    TString input = cfg.getS(TString::Format("sample_%s", Samples[iSample].name.Data()));
	    if(input.Length()==0){
		std::cerr << "Error: input file or catalog of sample " << Samples[iSample].name
			  << " needs to be defined in configuration file with sample_Data parameter.";
		return 1;
	    }

	    ZJets ana(lepSel, Samples[iSample].name, TString::Format(input, lepSel.Data()), lumi,  trigCorr,
		      syst[iSyst], systDir[iSyst], Samples[iSample].xsecError,
		      lepPtMin, lepEtaMax, jetPtMin, jetEtaMax,
		      maxEvents, histoDir, bonzaiDir, maxFiles);
	    ana.Loop(hasRecoInfo, hasGenInfo, jobNum, nJobs, pdfSet, pdfMember, muR, muF, yieldScale);
	    if(Samples[iSample].merge == '+') tomerge.push_back(ana.outputFileName.Data());
	}//next sample, iSample      
    }//next systematic, iSyst
    return 0;
}
