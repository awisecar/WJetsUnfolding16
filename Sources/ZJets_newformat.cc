//-*- c-basic-offset: 4; -*-
#define PI 3.14159265359
#define DEBUG              0

#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TLorentzVector.h>
#include <TDatime.h>
#include <TMath.h>
#include <TRandom3.h>
#include <TRandom.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include "LHAPDF/LHAPDF.h"
#include "functions.h"
#include "standalone_LumiReWeighting.h"
#include "ZJets_newformat.h"
#include <sys/time.h>
#include <sys/types.h>
#include <regex.h>
#include "ConfigVJets.h"
#include "rochcor2015.h"

extern ConfigVJets cfg;//defined in runZJets_newformat.cc

using namespace std;

void ZJets::Loop(bool hasRecoInfo, bool hasGenInfo, int jobNum, int nJobs,
		 TString pdfSet, int pdfMember, double muR, double muF, double yieldScale)
{

    //--- Random generator necessary for BTagging ---
    TRandom3* RandGen = new TRandom3();
    //--------------------------------------------
    doRochester = true;
    rmcor = new rochcor2015();

    //--- Initialize PDF from LHAPDF if needed ---
    if (pdfSet != "") initLHAPDF(pdfSet, pdfMember);
    //--------------------------------------------

    //--- Counters to check the yields ---
    Long64_t nEvents(0);
    unsigned int nEventsVInc0Jets(0),  nEventsVInc0JetsNoTrig(0), nEventsVInc1Jets(0), nEventsVInc2Jets(0), nEventsVInc3Jets(0);
    unsigned int nGenEventsVInc0Jets(0), nGenEventsVInc1Jets(0), nGenEventsVInc2Jets(0), nGenEventsVInc3Jets(0);
    unsigned int nEventsWithTwoGoodLeptons(0), nEventsWithTwoGoodLeptonsWithOppCharge(0), nEventsWithTwoGoodLeptonsWithOppChargeAndGoodMass(0);
    unsigned int nGenEventsWithTwoGoodLeptons(0), nGenEventsWithTwoGoodLeptonsWithOppCharge(0), nGenEventsWithTwoGoodLeptonsWithOppChargeAndGoodMass(0);


    double nEffEventsVInc0Jets(0), nEffEventsVInc0JetsNoTrig(0), nEffEventsVInc1Jets(0), nEffEventsVInc2Jets(0), nEffEventsVInc3Jets(0);
    double nEffGenEventsVInc0Jets(0), nEffGenEventsVInc1Jets(0), nEffGenEventsVInc2Jets(0), nEffGenEventsVInc3Jets(0);
    double nEffEventsWithTwoGoodLeptons(0), nEffEventsWithTwoGoodLeptonsWithOppCharge(0), nEffEventsWithTwoGoodLeptonsWithOppChargeAndGoodMass(0);
    double nEffGenEventsWithTwoGoodLeptons(0), nEffGenEventsWithTwoGoodLeptonsWithOppCharge(0), nEffGenEventsWithTwoGoodLeptonsWithOppChargeAndGoodMass(0);

    unsigned int nEventsPassingTrigger(0);
    double nEffEventsPassingTrigger(0);
         
    bool UnfoldUnc = cfg.getB("unfoldUnc", false);
    if(UnfoldUnc){
	std::cout << "Reweighting mode. MC will be reweighted to compute an alternative response matrix to be used for the estimation of the model dependency.\n";
    }

    //------------------------------------

    //==========================================================================================================//
    //    int ZMCutLow(71), ZMCutHigh(111);
    double ZMCutLow = cfg.getD("ZMassMin", 71.);
    double ZMCutHigh = cfg.getD("ZMassMax", 111.);
    int MTCutLow(50), METCutLow(0);
    // additional variables
    double ZptRange[6] = {0, 40, 80, 120, 160, 1000};
    int LeptonID(11);
    if (lepSel == "DMu" || lepSel == "SMu") LeptonID = 13;

    muIso_ = cfg.getD("muRelIso");
    eIso_ = cfg.getD("elRelIso");
    bool pogSF = cfg.getD("pogSF", true);

    //==========================================================================================================//
    //         Output file name           //
    //===================================//
    CreateOutputFileName(pdfSet, pdfMember, muR, muF, nJobs == 1 ? 0 : jobNum);
    std::cerr << "Histogram file name " << outputFileName << " for sample " << sampleLabel_ << "\n";
    TFile *outputFile = new TFile(outputFileName, "RECREATE");
    //==========================================================================================================//


    if (DEBUG) cout << "Stop after line " << __LINE__ << endl;
    //==========================================================================================================//
    //       Load efficiency tables        //
    //====================================//
    //table TableJESunc("EfficiencyTables/JESUnce_FT_53_V21_AN4_Uncertainty_AK5PFchs.txt");
    table TableJESunc("EfficiencyTables/JECUncertainty_Summer15_25nsV5_Data_AK4PF.txt");
    //table TableJESunc("EfficiencyTables/JECUncertainty_Summer15_25nsV6_AK4PF.txt");

    // muon SF
    table TrigMu17Mu8SF("EfficiencyTables/Efficiency_SF_Mu17Mu8.txt");

    //8TeV
    //table LeptIso, LeptID;
    //table SC_RunABCD_TightID("EfficiencyTables/Muon_IDTight_Efficiencies_Run_2012ABCD_53X_Eta_Pt.txt");
    //table SC_RunABCD_LooseIso("EfficiencyTables/Muon_ISOLoose_forTight_Efficiencies_Run_2012ABCD_53X_Eta_Pt.txt");
    //LeptID = SC_RunABCD_TightID;
    //LeptIso = SC_RunABCD_LooseIso;

    // electron SF
    //table Ele_Rec;
    //table Ele_Rec_8TeV("EfficiencyTables/Ele_SF_Reconstruction_2012.txt");
    //table SC_Ele_2012EA("EfficiencyTables/Ele_SF_EA2012.txt");

    // new for 13 TeV SF for Id+Iso
    table Iso_TightID13TeV("EfficiencyTables/ratios.txt"); 
    table TrigIsoMu24SF("EfficiencyTables/Efficiency_SF_IsoMu24_eta2p1.txt");

    //Aram's numbers
    table LeptIdIso = Iso_TightID13TeV;

    //Muon POG numbers
    table LeptIso("EfficiencyTables/Muon_ISOLoose_forTight_Efficiencies_RunD_2015_Eta_Pt.txt");
    table LeptId("EfficiencyTables/Muon_IDTight_Efficiencies_RunD_2015_Eta_Pt.txt");
	
    table LeptTrig = TrigMu17Mu8SF;
    
    //Ele_Rec = Ele_Rec_8TeV;
    //if (lepSel == "DE" || lepSel == "SE") LeptID = SC_Ele_2012EA;
    // else if (lepSel == "SMu") LeptTrig = TrigIsoMu24SF;
    //==========================================================================================================//
  

    if (DEBUG) cout << "Stop after line " << __LINE__ << endl;
    //==========================================================================================================//
    //     Systematics: jec, pu, xsec     //
    //====================================//
    cout << "Lepton Flavor: " << lepSel << "  systematics: " << systematics << "  direction: " << direction << endl;

    int mode = (systematics == 1) ? direction : 0;
    standalone_LumiReWeighting puWeight(2015250, mode);

    int scale(0); //0,+1,-1; (keep 0 for noJEC shift study)
    if (systematics == 2) scale =  direction;

    xsecFactor_ = 1.;
    if (systematics == 3) xsecFactor_ = 1. + direction * xsecUnc;

    int smearJet(0);
    if (systematics == 4) smearJet = direction;

    int lepscale(0);
    if (systematics == 5) lepscale = direction;

    int smearlepton(0);
    if (systematics == 6) smearlepton = direction;
    //==========================================================================================================//



    // setting weight when running on MIX of exclusive DY/WJets files to match number of parton events
    double mixingWeightsDY[4] = {0.192686, 0.0718097, 0.04943495, 0.0360337}; // here we match all partons, and combine electron and muon side
    double mixingWeightsWJ_SMu[4] ={0.366713,  0.1119323,  0.07641136,  0.03803325};

    //======================================================================
    //additionnal PU weights
    TH1* addPuWeights = 0;
    string addPuFile = cfg.getS("additionalPuWeightFile");
    if(addPuFile.size() > 0){
        TFile f(addPuFile.c_str());
        if(f.IsZombie()){
            std::cerr << "PU reweighting file, " << addPuFile << " was not found!" << endl;
        } else{
            cout << "Event will be reweighting according to their number of vertices using weights from file "
                << addPuFile << "." << endl;
            f.GetObject("hWeights", addPuWeights);
            addPuWeights->SetDirectory(0);
        }
    }
    //======================================================================
    // Data/MC ration  needed for unfolding uncertainties

    // jet pt
    TFile *fratio1jpt = new TFile("FirstJetPt_2_Zinc1jet_ratio.root");
    TH1D *FirstJetPt_2_Zinc1jet_ratio =  (TH1D*) fratio1jpt->Get("FirstJetPt_2_Zinc1jet_ratio");

    TFile *fratio2jpt = new TFile("SecondJetPt_2_Zinc2jet_ratio.root");
    TH1D *SecondJetPt_2_Zinc2jet_ratio =  (TH1D*) fratio2jpt->Get("SecondJetPt_2_Zinc2jet_ratio");

    TFile *fratio3jpt = new TFile("ThirdJetPt_2_Zinc3jet_ratio.root");
    TH1D *ThirdJetPt_2_Zinc3jet_ratio =  (TH1D*) fratio3jpt->Get("ThirdJetPt_2_Zinc3jet_ratio");

    // jet Ht
    TFile *fratioht = new TFile("JetsHT_2_Zinc1jet_ratio.root");
    TH1D *JetsHT_2_Zinc1jet_ratio =  (TH1D*) fratioht->Get("JetsHT_2_Zinc1jet_ratio");

    TFile *fratioht2 = new TFile("JetsHT_2_Zinc2jet_ratio.root");
    TH1D *JetsHT_2_Zinc2jet_ratio =  (TH1D*) fratioht2->Get("JetsHT_2_Zinc2jet_ratio");

    TFile *fratioht3 = new TFile("JetsHT_2_Zinc3jet_ratio.root");
    TH1D *JetsHT_2_Zinc3jet_ratio =  (TH1D*) fratioht3->Get("JetsHT_2_Zinc3jet_ratio");

    // jet eta
    TFile *fratio1jeta = new TFile("FirstJetEta_2_Zinc1jet_ratio.root");
    TH1D *FirstJetEta_2_Zinc1jet_ratio =  (TH1D*) fratio1jeta->Get("FirstJetEta_2_Zinc1jet_ratio");

    TFile *fratio2jeta = new TFile("SecondJetEta_2_Zinc2jet_ratio.root");
    TH1D *SecondJetEta_2_Zinc2jet_ratio =  (TH1D*) fratio2jeta->Get("SecondJetEta_2_Zinc2jet_ratio");

    TFile *fratio3jeta = new TFile("ThirdJetEta_2_Zinc3jet_ratio.root");
    TH1D *ThirdJetEta_2_Zinc3jet_ratio =  (TH1D*) fratio3jeta->Get("ThirdJetEta_2_Zinc3jet_ratio");

    //  multiplicity
    TFile *fratioNJexc = new TFile("ZNGoodJets_Zexc_ratio.root");
    TH1D *ZNGoodJets_Zexc_ratio =  (TH1D*) fratioNJexc->Get("ZNGoodJets_Zexc_ratio");


    //==========================================================================================================//
    // Start looping over all the events //
    //===================================//
    cout << endl;
    stringstream s;
    cout << "\nProcessing : " << fileName << "\n";
    s <<"\t--> " << outputFileName << "\n";
    cout << s.str();
    for(unsigned i = 0 ; i < s.str().size(); ++i) cout << "-";
    cout << "\n\n" << flush;
    
    //--- Initialize the tree branches ---
    Init(hasRecoInfo, hasGenInfo);
    if (fChain == 0) return;
    Long64_t nentries = fChain->GetEntries();

    //------------------------------------

    struct timeval t0;
    int mess_every_n =  std::min(1000LL, nentries/10);

    // ------ Random number for lepton energy resolution smearing -----
    //TRandom* RamMu = new TRandom(10);
    //TRandom* RamEle = new TRandom(20);
    // --------------------------------

    //event yield normalisation for MC
    norm_ = yieldScale;

    double prev_rate = 0;

    Long64_t entry_start = 0;
    Long64_t entry_stop = nentries;
    if(nJobs > 1){
	Long64_t eventsPerJob = nentries / nJobs;
	entry_start = eventsPerJob * (jobNum - 1);
	if(jobNum < nJobs)  entry_stop = entry_start + eventsPerJob;
	else entry_stop = nentries;
    }

    int nEventsToProcess = entry_stop - entry_start;
    if(nMaxEvents >= 0 && nEventsToProcess > nMaxEvents) nEventsToProcess = nMaxEvents;
    cout << "We will run on " << nEventsToProcess << " events" << endl;
    
    //store integrated luminosity in Lumi histogram:
    Lumi->SetBit(TH1::kIsAverage); //to prevent hadd to sum-up the numbers
    Lumi->Fill(0., lumi_);

    processedEventMcWeightSum_ = 0.;
    nEvents = 0;
    double weightSum = 0;
    double genWeightSum = 0;

    if(!GLepBarePrompt){
	std::cout << "Warning: tau gen veto was not found in the ntuple. It is fine if the samples is not DY or if it does not contains Z->\\tau\\tau.\n" ;
    }
    
    //======================================================================
    // Event loop starts here
    //======================================================================
    for (Long64_t jentry = entry_start; jentry < entry_stop; jentry += 1) {
	if (0 <= nMaxEvents && nMaxEvents  <= nEvents) break;
	
	Long64_t ientry = LoadTree(jentry);
        if (ientry < 0) break;
        //cout << "---------------------------------------------------------------------" << endl;
	
	if(nEvents == mess_every_n) gettimeofday(&t0, 0);
        if (nEvents % mess_every_n == 0 && nEvents > mess_every_n){
            timeval t1;
            gettimeofday(&t1, 0);
            double rate = ((t1.tv_sec - t0.tv_sec) + 1.e-6*(t1.tv_usec - t0.tv_usec)) 
		/ (nEvents - mess_every_n);
	    if(fabs(rate / prev_rate - 1.) > 0.1){
		prev_rate = 0.5*(prev_rate + rate);
	    }
            double rem = prev_rate * (nEventsToProcess - nEvents);
            int rem_s = int(rem + 0.5);
            int rem_h = int(rem_s) / 3600; 
            rem_s -= rem_h * 3600;
            int rem_m = rem_s / 60;
            rem_s -= rem_m *60;
            cout << "\r" << TString::Format("%4.1f%%", (100. * nEvents) / nEventsToProcess)
		 << " " << std::setw(11) << nEvents << " / " << nEventsToProcess
		 << " " << std::setw(7) << int(prev_rate * 1.e6 + 0.5) << " us/event"
		 << " Remaining time for this dataset: " 
		 << std::setw(2) << rem_h << " h "
		 << std::setw(2) << rem_m << " min "
		 << std::setw(2) << rem_s << " s"
		 << std::flush;
        }

        if(fChain->GetEntry(jentry) == 0){
	    std::cerr << "Failed to read Tree entry " << jentry << "!\n";
	    continue;
	}

	if(nEvents == 0 && !EvtIsRealData){
	    norm_ = yieldScale * lumi_ * xsec_ * xsecFactor_ * skimAccep_[0];
	    if(norm_ == 0){
		std::cerr << "Error: normaliation factor for sample " << fileName
			  << " is null! Aborts at " __FILE__ ":" 
			  << __LINE__ << "." << std::endl;
		abort();
	    }
	}
	
        //=======================================================================================================//
        //         Continue Statements        //
        //====================================//
        //if (nEvents % 2 == 0) continue;
        //if (EvtVtxCnt <= 14) continue;

        //=======================================================================================================//

	nEvents++;


        if (DEBUG) cout << "Stop after line " << __LINE__ << endl;
        //=======================================================================================================//
        //         Computing weight            //
        //====================================//
        double weight = norm_;

	if (hasRecoInfo && !EvtIsRealData) {
	    //cout << "PU weight: " << EvtPuCntTruth << " , " << puWeight.weight(EvtPuCntTruth) << "\n";
	    weight *= puWeight.weight(EvtPuCntTruth);
	}

        if(addPuWeights){
            double add_w_ = addPuWeights->GetBinContent(addPuWeights->GetXaxis()->FindBin(EvtVtxCnt));
            if(add_w_ > 0) weight *= add_w_;
        }

        if (fileName.Index("DYJets") >= 0 && fileName.Index("MIX") >= 0 && GNup > 5) weight *= mixingWeightsDY[GNup - 6]; 
        if (fileName.Index("SMu_8TeV_WJets") >= 0 && fileName.Index("MIX") >= 0 && GNup > 5) weight *= mixingWeightsWJ_SMu[GNup - 6]; 
       // if (fileName.Index("Sherpa") >= 0 && fileName.Index("UNFOLDING") >= 0) {
       //     weight *= mcSherpaWeights_->at(0) / 43597515.;
       // }

	if(!EvtIsRealData){
	    if(EvtWeights->size() > 0){
		weight *= (*EvtWeights)[0];
		processedEventMcWeightSum_ += (*EvtWeights)[0];
	    } else{
		processedEventMcWeightSum_ += 1.;
	    }
	}

//        if (fileName.Index("Sherpa2") >= 0) {
//            weight *= EvtWeights->at(0);
//            weight_amcNLO_sum += EvtWeights->at(1);
//        }
//	
//        if (fileName.Index("mcatnlo") >= 0) {
//            weight *= EvtWeights->at(0);
//         //  cout << EvtWeights->at(0) << "  , " << EvtWeights->at(0) << "\n";
//
//            //if (muR == 0.0 && muF == 0.0 && pdfMember == -1) weight *= EvtWeights->at(0);
//            //if (muR == 1.0 && muF == 1.0 && pdfMember == -1) weight *= EvtWeights->at(0);
//            // CommentAG: only at(0) available for EvtWeights
//         /*
//            if (muR == 1.0 && muF == 2.0 && pdfMember == -1) weight *= EvtWeights->at(2);
//            if (muR == 1.0 && muF == 0.5 && pdfMember == -1) weight *= EvtWeights->at(3);
//            if (muR == 2.0 && muF == 1.0 && pdfMember == -1) weight *= EvtWeights->at(4);
//            if (muR == 2.0 && muF == 2.0 && pdfMember == -1) weight *= EvtWeights->at(5);
//            if (muR == 2.0 && muF == 0.5 && pdfMember == -1) weight *= EvtWeights->at(6);
//            if (muR == 0.5 && muF == 1.0 && pdfMember == -1) weight *= EvtWeights->at(7);
//            if (muR == 0.5 && muF == 2.0 && pdfMember == -1) weight *= EvtWeights->at(8);
//            if (muR == 0.5 && muF == 0.5 && pdfMember == -1) weight *= EvtWeights->at(9);
//            if (muR == 0.0 && muF == 0.0 && pdfMember != -1) weight *= EvtWeights->at(pdfMember+10);
//            weight_amcNLO_sum += EvtWeights->at(1);
//	 */
//            weight_amcNLO_sum += EvtWeights->at(0); 
//        }

       // cout << weight << "\n";
        //==========================================================================================================//
        // Compute the weight for PDF syst    //
        //===================================//
        double wPdf(1);
        if (pdfSet != "") wPdf = computePDFWeight();
        //==========================================================================================================//

        //--- There is no pile-up so no need to reweight for that ---
        double genWeight = weight * wPdf;
	genWeightSum += genWeight;
        //=======================================================================================================//


        if (DEBUG) cout << "Stop after line " << __LINE__ << endl;

	//Trigger
	bool passesTrigger = ((*ourTrig_) & triggerMask_);

	if(passesTrigger){
	    nEventsPassingTrigger++;
	    nEffEventsPassingTrigger += weight;
	}
	
        //=======================================================================================================//
        //         Retrieving leptons          //
        //====================================//
        bool passesLeptonCut(0), passesLeptonChargeCut(0), passesTauCut(1);
	//bool passesLeptonMassCut(0);
        unsigned short nLeptons(0), nVetoMuons(0), nVetoElectrons(0);
        vector<leptonStruct> leptons;
        vector<leptonStruct> vetoMuons;
        vector<leptonStruct> vetoElectrons;
        TLorentzVector MET;
        double MT = -99;
        TLorentzVector EWKBoson;

        if (hasRecoInfo) {
            //--- get Muons ---
            if (lepSel == "DMu" || lepSel == "SMu") {
                getMuons(leptons, vetoMuons);

            }


            //--- get Electrons ---
            if (lepSel == "DE" || lepSel == "SE") {
                getElectrons(leptons, vetoElectrons);
            }

            //--- get MET --- 
            if (lepSel == "SMu" || lepSel == "SE") {
                int whichMET(0); //0 - slimmedMETs  1- slimmedMETsNoHF 2- slimmedMETsPuppi
                MET.SetXYZM(METPx->at(whichMET), METPy->at(whichMET), 0, 0);
            }

            //--- get the size of the collections ---
            nLeptons = leptons.size();
            nVetoMuons = vetoMuons.size();
            nVetoElectrons = vetoElectrons.size();


            //--- sort leptons by descending pt ---
            sort(leptons.begin(), leptons.end(), LepDescendingOrder);
            sort(vetoMuons.begin(), vetoMuons.end(), LepDescendingOrder);

            sort(vetoElectrons.begin(), vetoElectrons.end(), LepDescendingOrder);

            //-- determine if the event passes the leptons requirements for EWKBoson = Z Boson
           // cout << " nLeptons " << nLeptons << "\n";

            if ((lepSel == "DMu" || lepSel == "DE") && nLeptons >= 2) {
                // --- lepton energy scale and resolution variation ---
                if(lepSel == "DMu"){
                    // --- muon energy scale variation ---
                    leptons[0].v.SetPtEtaPhiE(leptons[0].v.Pt() * (1 + lepscale*0.002), leptons[0].v.Eta(), leptons[0].v.Phi(), leptons[0].v.E() * (1 + lepscale*0.002));
                    leptons[1].v.SetPtEtaPhiE(leptons[1].v.Pt() * (1 + lepscale*0.002), leptons[1].v.Eta(), leptons[1].v.Phi(), leptons[1].v.E() * (1 + lepscale*0.002));

                }
                else if(lepSel == "DE")
                {
                    // --- electron energy scale variation ---
                    if(fabs(leptons[0].v.Eta())<1.479){
                        leptons[0].v.SetPtEtaPhiE(leptons[0].v.Pt() * (1 + lepscale * 0.006), leptons[0].v.Eta(), leptons[0].v.Phi(), leptons[0].v.E() * (1 + lepscale * 0.006));
                    }
                    else{
                        leptons[0].v.SetPtEtaPhiE(leptons[0].v.Pt() * (1 + lepscale * 0.015), leptons[0].v.Eta(),leptons[0].v.Phi(), leptons[0].v.E() * (1 + lepscale * 0.015));
                    }

                    if(fabs(leptons[1].v.Eta())<1.479){
                        leptons[1].v.SetPtEtaPhiE(leptons[1].v.Pt() * (1 + lepscale * 0.006), leptons[1].v.Eta(), leptons[1].v.Phi(), leptons[1].v.E() * (1 + lepscale * 0.006));
                    }
                    else{
                        leptons[1].v.SetPtEtaPhiE(leptons[1].v.Pt() * (1 + lepscale * 0.015), leptons[1].v.Eta(),leptons[1].v.Phi(), leptons[1].v.E() * (1 + lepscale * 0.015));
                    }
		    
                }

                nEventsWithTwoGoodLeptons++;
                nEffEventsWithTwoGoodLeptons += weight;
    
		//  if(!hasGenInfo){   // CommentAG: comment this out since don't enter 'lepton energy smearing' block
		// build Electroweak boson candidate: here it is expected to be a Z

                if (!EvtIsRealData) {
                    double effWeight = 1.;
                    if (lepSel == "DMu") {
			if(pogSF){
			    effWeight *= LeptId.getEfficiency(leptons[0].v.Pt(), fabs(leptons[0].v.Eta()));
			    effWeight *= LeptId.getEfficiency(leptons[1].v.Pt(), fabs(leptons[1].v.Eta()));
			    effWeight *= LeptIso.getEfficiency(leptons[0].v.Pt(), fabs(leptons[0].v.Eta()));
			    effWeight *= LeptIso.getEfficiency(leptons[1].v.Pt(), fabs(leptons[1].v.Eta()));
			} else{
			    effWeight *= LeptIdIso.getEfficiency(leptons[0].v.Pt(), fabs(leptons[0].v.Eta()));
			}
                        if (useTriggerCorrection) effWeight *= LeptTrig.getEfficiency(fabs(leptons[0].v.Eta()), fabs(leptons[1].v.Eta()));
                    }
                    //else if (lepSel == "DE") {
                    //    effWeight *= Ele_Rec.getEfficiency(leptons[0].v.Pt(), fabs(leptons[0].scEta));
                    //    effWeight *= Ele_Rec.getEfficiency(leptons[1].v.Pt(), fabs(leptons[1].scEta));
                    //    effWeight *= LeptID.getEfficiency(leptons[0].v.Pt(), fabs(leptons[0].scEta));
                    //    effWeight *= LeptID.getEfficiency(leptons[1].v.Pt(), fabs(leptons[1].scEta)); 
                    //}
                    weight *= effWeight;
                }

		weightSum += weight;

		EWKBoson = leptons[0].v + leptons[1].v;
		// apply trigger, charge, mass cut
		if (passesTrigger && (leptons[0].charge * leptons[1].charge < 0)) {
		    nEventsWithTwoGoodLeptonsWithOppCharge++;
		    nEffEventsWithTwoGoodLeptonsWithOppCharge += weight;
		    passesLeptonChargeCut = 1;
		    if (EWKBoson.M() > ZMCutLow && EWKBoson.M() < ZMCutHigh 
			&& leptons[0].v.Pt() > lepPtCutMin && leptons[1].v.Pt() > lepPtCutMin) {
			nEventsWithTwoGoodLeptonsWithOppChargeAndGoodMass++;
			nEffEventsWithTwoGoodLeptonsWithOppChargeAndGoodMass += weight;
			passesLeptonCut = 1;
			//passesLeptonMassCut = 1;
		    }
		}
		if((leptons[0].charge * leptons[1].charge < 0)
		   &&  EWKBoson.M() > ZMCutLow && EWKBoson.M() < ZMCutHigh 
		   && leptons[0].v.Pt() > lepPtCutMin && leptons[1].v.Pt() > lepPtCutMin){
		    ++nEventsVInc0JetsNoTrig;
		    nEffEventsVInc0JetsNoTrig += weight;
		}
		//}
            } // end if Z study and nLeptons >= 2

            // determine if the event passes the leptons requirements for EWKBoson = W Boson
            // exactly one muon (or exactly one electron) must be present and no additional
            // charged leptons can be present.
            else if ((lepSel == "SMu" || lepSel == "SE") && (nLeptons == 1 && nVetoMuons == 0 && nVetoElectrons == 0)) {
                // add the MET to the leptons collection: leptons[1] = MET
                leptons.push_back(leptonStruct(MET.Pt(), 0, MET.Phi(), MET.Pt(), 0, 0, 0, 0, 0)); 

                // build Electroweak boson candidate: here it is expected to be a W
                EWKBoson = leptons[0].v + MET;
                MT = sqrt(2 * leptons[0].v.Pt() * MET.Pt() * (1 - cos(leptons[0].v.Phi() - MET.Phi())));

                // apply transver mass and MET cut
                if (MT > MTCutLow && MET.Pt() > METCutLow && leptons[0].v.Pt() > lepPtCutMin) passesLeptonCut = 1;

                // apply scale factors only on MC.
                if (!EvtIsRealData) {
                    double effWeight = 1.;
                    if (lepSel == "SMu") {
			if(pogSF){
			    effWeight *= LeptId.getEfficiency(leptons[0].v.Pt(), fabs(leptons[0].v.Eta()));
			    effWeight *= LeptId.getEfficiency(leptons[1].v.Pt(), fabs(leptons[1].v.Eta()));
			    effWeight *= LeptIso.getEfficiency(leptons[0].v.Pt(), fabs(leptons[0].v.Eta()));
			    effWeight *= LeptIso.getEfficiency(leptons[1].v.Pt(), fabs(leptons[1].v.Eta()));
			} else{
			    effWeight *= LeptIdIso.getEfficiency(leptons[0].v.Pt(), fabs(leptons[0].v.Eta()));
			}
                        if (useTriggerCorrection) effWeight *= LeptTrig.getEfficiency(fabs(leptons[0].v.Eta()), fabs(leptons[1].v.Eta()));
                    }
                    else if (lepSel == "SE") {
                        //effWeight *= Ele_Rec.getEfficiency(leptons[0].v.Pt(), fabs(leptons[0].scEta));
                        //effWeight *= LeptID.getEfficiency(leptons[0].v.Pt(), fabs(leptons[0].scEta));
                    }
                    weight *= effWeight;
                }
            } // end if W study and nLeptons >= 1
        }// end has reco info

       //cout << effWeight << "\n";
        if (DEBUG) cout << "Stop after line " << __LINE__ << endl;
        //=======================================================================================================//
        //       Retrieving gen leptons        //
        //====================================//
        bool passesgenLeptonCut(0);
        unsigned short nTotgenLeptons(0), ngenLeptons(0), nTotGenPhotons(0);
        vector<leptonStruct> genLeptons;
        vector<int> usedGenPho;
        TLorentzVector genMET;
        double genMT = -99;
        TLorentzVector genEWKBoson;
        int countTauS3 = 0;

        if (hasGenInfo) {
            // CommentAG: this line is commented because can't do countTauS3-- since status 3 is not stored
            // if (hasRecoInfo) countTauS3 = (lepSel == "DMu" || lepSel == "DE") ? 2 : 1; // AG
            nTotGenPhotons = GLepClosePhotEta->size();
            nTotgenLeptons = GLepBareEta->size();
            //-- retriveing generated leptons with status 1
            for (unsigned short i(0); i < nTotgenLeptons; i++) {
                bool lepToBeConsidered(false);
                if ((lepSel == "DMu" || lepSel == "DE") && abs(GLepBareId->at(i)) == LeptonID) lepToBeConsidered = true; 
                else if ((lepSel == "SMu" || lepSel == "SE") && (abs(GLepBareId->at(i)) == LeptonID || abs(GLepBareId->at(i)) == 12 || abs(GLepBareId->at(i)) == 14)) lepToBeConsidered = true;
                // following two lines should give the same result
                if (GLepBareSt->at(i) == 3 && abs(GLepBareId->at(i)) != LeptonID && (abs(GLepBareId->at(i)) == 15 || abs(GLepBareId->at(i)) == 13 || abs(GLepBareId->at(i)) == 11)) countTauS3++;
                if (GLepBareSt->at(i) == 3 && abs(GLepBareId->at(i)) == LeptonID) countTauS3--;

		bool passesTauVeto = true;
		if(GLepBarePrompt) passesTauVeto = (*GLepBarePrompt)[i];
		if(!passesTauVeto) continue;
		
                if (!lepToBeConsidered) continue;
		
                int charge;
                if (abs(GLepBareId->at(i)) == 12 || abs(GLepBareId->at(i)) == 14 || abs(GLepBareId->at(i)) == 16) charge = 0;
                else if (GLepBareId->at(i) < 0) charge = -1;
                else charge = 1;

		leptonStruct genLep(GLepBarePt->at(i), GLepBareEta->at(i), GLepBarePhi->at(i), GLepBareE->at(i), charge, 0, 0, 0, 0);
                leptonStruct genLepNoFSR(GLepBarePt->at(i), GLepBareEta->at(i), GLepBarePhi->at(i), GLepBareE->at(i), charge, 0, 0, 0, 0);

                //-- dress the leptons with photon (cone size = 0.1). Only for status 1 leptons (after FSR)
                if ((GLepBareSt->at(i) == 1 && lepToBeConsidered) || ((lepSel == "SMu" || lepSel == "SE") && charge == 0)) {

                    for (unsigned short j(0); j < nTotGenPhotons; j++){
                        TLorentzVector tmpGenPho;
                        tmpGenPho.SetPtEtaPhiM(GLepClosePhotPt->at(j), GLepClosePhotEta->at(j), GLepClosePhotPhi->at(j), 0.);
                        int used(0);
                        for (unsigned short k(0); k < usedGenPho.size(); k++){
                            if (j == usedGenPho[k]) used = 1;
                        }
                        if (deltaR(tmpGenPho.Phi(), tmpGenPho.Eta(), genLepNoFSR.v.Phi(), genLepNoFSR.v.Eta()) < 0.1 && !used){
                            genLep.v += tmpGenPho;
                            usedGenPho.push_back(j);
                        }
                    }   

                    if ((genLep.v.Pt() >= lepPtCutMin && fabs(genLep.v.Eta()) <= 0.1*lepEtaCutMax && genLep.charge != 0)  
                           || ((lepSel == "SMu" || lepSel == "SE") && genLep.charge == 0)) {   
                          genLeptons.push_back(genLep);
                    }
                }
            }

            ngenLeptons = genLeptons.size();


            // sort leptons by descending pt
            sort(genLeptons.begin(), genLeptons.end(), LepDescendingOrder);
	    
	    //assert(genLeptons.size() < 2 || genLeptons[0].v.Pt() >= genLeptons[1].v.Pt());
	    if(genLeptons.size() > 1 &&  genLeptons[0].v.Pt() < genLeptons[1].v.Pt()){
		std::cerr << "Problem in Gen jet ordering!\n";
	    }

            if (countTauS3 == 0 && fileName.Index("UNFOLDING") >= 0 && fileName.Index("Sherpa") < 0) {
                partonsN->Fill(GNup-5);
                partonsNWeighted->Fill(GNup-5, genWeight);
            }

            //--- if there are taus, but we do not run on the Tau file, thus we run on the DYJets file, 
            //    then we don't count the event at reco.
            if (countTauS3 > 0 && fileName.Index("Tau") < 0 && fileName.Index("Sherpa") < 0 && fileName.Index("MG5") < 0) {        
                passesTauCut = 0;
		std::cout << "Tau veto changed passesLeptonCut value!\n";
                passesLeptonCut = 0; 
            }
 
            //-- determine if the event passes the leptons requirements
             if ((lepSel == "DMu" || lepSel == "DE") && ngenLeptons >= 2) {

		 ++nGenEventsWithTwoGoodLeptons;
		 nEffGenEventsWithTwoGoodLeptons += genWeight;
		 
		 // build the EWKBoson candidate and the kinematic
		 genEWKBoson = genLeptons[0].v + genLeptons[1].v;
		 
		 if (genLeptons[0].charge * genLeptons[1].charge < 0){
		     ++nGenEventsWithTwoGoodLeptonsWithOppCharge;
		     nEffGenEventsWithTwoGoodLeptonsWithOppCharge += genWeight;
		 }
		 
		 // apply charge, mass and eta cut
		 if (genLeptons[0].charge * genLeptons[1].charge < 0 && genEWKBoson.M() > ZMCutLow && genEWKBoson.M() < ZMCutHigh) {
		     ++nGenEventsWithTwoGoodLeptonsWithOppChargeAndGoodMass;
		     nEffGenEventsWithTwoGoodLeptonsWithOppChargeAndGoodMass += genWeight;
		     passesgenLeptonCut = 1;
		 }
                //--- if there are taus we don't want the gen level
		 if (countTauS3 > 0 && fileName.Index("Bugra") < 0 && fileName.Index("MG5") < 0) passesgenLeptonCut = 0;
	     }
	     else if ((lepSel == "SMu" || lepSel == "SE") && (ngenLeptons >= 2)) {


                if (abs(genLeptons[0].charge) > 0 && genLeptons[1].charge == 0) {
                    genMET = genLeptons[1].v;
                }
                else if (abs(genLeptons[1].charge) > 0 && genLeptons[0].charge == 0) {
                    genMET = genLeptons[0].v;
                    genLeptons[0] = genLeptons[1];
                }
                else passesgenLeptonCut = 0;

                genEWKBoson = genLeptons[0].v + genMET;
                genMT = sqrt(2 * genLeptons[0].v.Pt() * genMET.Pt() * (1 - cos(genLeptons[0].v.Phi() - genMET.Phi())));


                // apply transverse mass and MET cut
                if (genMT > MTCutLow && genMET.Pt() > METCutLow) passesgenLeptonCut = 1;
                //--- if there are taus we don't want the gen level
                if (countTauS3 > 0) passesgenLeptonCut = 0;

 
            }
        } // end of hasGenInfo


	//=======================================================================================================//
        //   ------- lepton energy smearing ------
        //==========================================//
        if (hasRecoInfo && hasGenInfo){
            if((lepSel == "DMu" || lepSel == "DE") && nLeptons >= 2 && ngenLeptons >= 2){  
		if(smearlepton){
		    double oldLeptonPt;
		    double genLeptonPt;
		    double newLeptonPt;
		    double smearFactor;
		    
		    smearFactor = (lepSel == "DMu") ? 0.006 : 0.06;
		    
		    oldLeptonPt = leptons[0].v.Pt();
		    genLeptonPt = genLeptons[0].v.Pt();
		    newLeptonPt = SmearLepPt(oldLeptonPt, genLeptonPt, smearlepton, smearFactor);
		    leptons[0].v.SetPtEtaPhiE(newLeptonPt, leptons[0].v.Eta(), leptons[0].v.Phi(), leptons[0].v.E()*newLeptonPt/oldLeptonPt);
		    
		    oldLeptonPt = leptons[1].v.Pt();
		    genLeptonPt = genLeptons[1].v.Pt();
		    newLeptonPt = SmearLepPt(oldLeptonPt,genLeptonPt,smearlepton,smearFactor);
		    leptons[1].v.SetPtEtaPhiE(newLeptonPt, leptons[1].v.Eta(), leptons[1].v.Phi(), leptons[1].v.E()*newLeptonPt/oldLeptonPt);
		    
		    // build Electroweak boson candidate: here it is expected to be a Z
		    EWKBoson = leptons[0].v + leptons[1].v;

		    // apply charge, mass cut
		    if (passesTrigger && leptons[0].charge * leptons[1].charge < 0) {
			passesLeptonChargeCut = 1;
			if (EWKBoson.M() > ZMCutLow && EWKBoson.M() < ZMCutHigh && leptons[0].v.Pt() > lepPtCutMin && leptons[1].v.Pt() > lepPtCutMin) {
			    // cout <<  "AndGoodMass" << "\n";
			    passesLeptonCut = 1;
			    //passesLeptonMassCut = 1;
			} else{
			    passesLeptonCut = 0;
			}
		    } // end if re-selection of leptons
		}
            } // end if DMu/DE channel

        } // end of hasRecoInfo and hasGenInfo

        if (DEBUG) cout << "Stop after line " << __LINE__ << endl;
        //=======================================================================================================//
        //          Retrieving jets           //
        //====================================//
        unsigned short nGoodJets(0), nGoodJets_20(0), nTotJets(0);
        bool bTagJetFound(false);
        double jetsHT(0);
        vector<jetStruct> jets;
        vector<jetStruct> jets_20; // additional jet collection with pt threshold of 20 GeV
        TLorentzVector jet1Plus2, jet1Minus2;

        if (hasRecoInfo) {
            nTotJets = JetAk04Eta->size();
            for (unsigned short i(0); i < nTotJets; i++) {
                bool passesBJets = false;
                if (fileName.Index("Sherpa") < 0) passesBJets = (JetAk04BDiscCisvV2->at(i) >= 0.679); 

                if (!EvtIsRealData && lepSel == "SMu") {
                    BTagModification(RandGen->Rndm(), JetAk04Pt->at(i), JetAk04Eta->at(i),JetAk04PartFlav->at(i), passesBJets);
                }

                jetStruct jet(JetAk04Pt->at(i), JetAk04Eta->at(i), JetAk04Phi->at(i), JetAk04E->at(i), i, passesBJets);

                //-- apply jet energy scale uncertainty (need to change the scale when initiating the object)
                double jetEnergyCorr = 0.; 
                bool jetPassesPtCut(jet.v.Pt() >= 10); 
                jetEnergyCorr = TableJESunc.getEfficiency(jet.v.Pt(), jet.v.Eta());

                jet.v.SetPtEtaPhiE(jet.v.Pt() * (1 + scale * jetEnergyCorr), jet.v.Eta(), jet.v.Phi(), jet.v.E() * (1 + scale * jetEnergyCorr));

                bool jetPassesEtaCut(fabs(jet.v.Rapidity()) <= 0.1*jetEtaCutMax); 
                bool jetPassesIdCut(JetAk04Id->at(i) > 0);
                bool jetPassesMVACut(JetAk04PuMva->at(i) > -0.2);

                bool jetPassesdRCut(1);
                unsigned short nRemovedLep = min(int(nLeptons), 2);
                for (unsigned short j(0); j < nRemovedLep; j++) {
                    if (deltaR(jet.v, leptons[j].v) < 0.5) {
                        jetPassesdRCut = 0;
                    }
                }

                //CAG
                if (passesLeptonCut && jet.v.Pt() >= 30 && jetPassesEtaCut && jetPassesIdCut && jetPassesdRCut)  // no MVA cut
                puMVA->Fill(JetAk04PuMva->at(i), weight);

                if (jetPassesPtCut && jetPassesEtaCut && jetPassesIdCut && jetPassesMVACut && jetPassesdRCut) {
                    jets.push_back(jet);
                    // as soon as one selected jet is a b-jet, turn bTagJetFound to true
                    bTagJetFound = (bTagJetFound || passesBJets); 
                }
            } //End of loop over all the jets 

            nGoodJets = jets.size();

            // line below to test reco events that originate from TAU
            if (fileName.Index("Tau") >= 0 && countTauS3 == 0 && hasGenInfo) {
                passesTauCut = 0;
		std::cout << "Tau veto changes passesLeptonCut value!\n";
                passesLeptonCut = 0;
            }
        }  // END IF HAS RECO
        //=======================================================================================================//

//cout << "passesLeptonCut 2 : " << passesLeptonCut << "\n";
        if (DEBUG) cout << "Stop after line " << __LINE__ << endl;
        //=======================================================================================================//
        //        Retrieving gen jets          //
        //====================================//
        unsigned short nGoodGenJets(0), nGoodGenJets_20(0), nTotGenJets(0);
        double genJetsHT(0);
        vector<jetStruct> genJets;
        vector<TLorentzVector> genLVJets;
        vector<jetStruct> genJets_20;
        TLorentzVector genJet1Plus2, genJet1Minus2;

        if (hasGenInfo){
            nTotGenJets = GJetAk04Eta->size();
            //-- retrieving generated jets
            for (unsigned short i(0); i < nTotGenJets; i++){
                jetStruct genJet(GJetAk04Pt->at(i), GJetAk04Eta->at(i), GJetAk04Phi->at(i), GJetAk04E->at(i), i, 0);
                bool genJetPassesdRCut(1);
                for (unsigned short j(0); j < ngenLeptons; j++){ 
                    if (deltaR(genJet.v, genLeptons[j].v) < 0.5) {
                        genJetPassesdRCut = 0;
                    }
                }
                if (genJet.v.Pt() >= 10 && fabs(genJet.v.Eta()) <= 5.0 && genJetPassesdRCut){
                    genJets.push_back(genJet);                  
                }
            }
            nGoodGenJets = genJets.size();
        }
        //=======================================================================================================//


        if (DEBUG) cout << "Stop after line " << __LINE__ << endl;
        //=======================================================================================================//
        //     Matching gen and reco jets     //
        //====================================//
        vector<int> genJetsIndex(nGoodGenJets, 0);
        vector<vector<int> > matchingTable(nGoodJets, genJetsIndex);
        if (hasRecoInfo && hasGenInfo){
            for (unsigned short i(0); i < nGoodJets; i++){
                double mindR(0.15);
                int index(-1);
                double dR(9999);
                for (unsigned short j(0); j < nGoodGenJets; j++){
                    dR = deltaR(genJets[j].v, jets[i].v);
                    if (dR < mindR){
                        mindR = dR;
                        index = j;
                    }
                }
                if (index > -1 ){
                    matchingTable[i][index] = 1; 
                    double oldJetPt = jets[i].v.Pt();
                    double newJetPt = SmearJetPt(oldJetPt, genJets[index].v.Pt(), jets[i].v.Eta(), smearJet);
                    jets[i].v.SetPtEtaPhiE(newJetPt, jets[i].v.Eta(), jets[i].v.Phi(), jets[i].v.E() * newJetPt / oldJetPt);
                }
            }

/*
            //-- print the mathcing table
            //cout << "\n mathcing Table: \n" << endl; 
            for (int i = 0; i < int(matchingTable.size()); i++){
              for (int j = 0; j < int(matchingTable[i].size()); j++){
                cout << matchingTable[i][j] << "  ";
              }
              cout << endl;
            }
*/
        }

        //=======================================================================================================//
        // Re-analyze the jets collections and cut on the Pt    //
        // we can do it only now since we needed to smear      //
        // the jet pt distribution for the MC                 //
        //===================================================//

        if (hasRecoInfo){     
            // subsamble the prior jet collection by applying pt cut
            vector<jetStruct> tmpJets;
            for (unsigned short i(0); i < nGoodJets; i++){
                if (jets[i].v.Pt() >= jetPtCutMin) tmpJets.push_back(jets[i]);
                if (jets[i].v.Pt() >= 20) jets_20.push_back(jets[i]);
            }
            jets.clear(); 
            jets = tmpJets; 
            nGoodJets = jets.size();
            nGoodJets_20 = jets_20.size();
            sort(jets.begin(), jets.end(), JetDescendingOrder);
            sort(jets_20.begin(), jets_20.end(), JetDescendingOrder);
            if (nGoodJets >= 2){
                jet1Plus2 = jets[0].v + jets[1].v;
                jet1Minus2 = jets[0].v - jets[1].v;
            }
            jetsHT = 0;
            for (unsigned short i(0); i < nGoodJets; i++){
                jetsHT += jets[i].v.Pt();  
            }
        }

        if (hasGenInfo){
            vector<jetStruct> tmpGenJets;
            for (unsigned short i(0); i < nGoodGenJets; i++){
                if (genJets[i].v.Pt() >= jetPtCutMin && fabs(genJets[i].v.Rapidity()) <= 0.1*jetEtaCutMax) tmpGenJets.push_back(genJets[i]);
                if (genJets[i].v.Pt() >= 20 && fabs(genJets[i].v.Rapidity()) <= 0.1*jetEtaCutMax) genJets_20.push_back(genJets[i]);
            }
            genJets.clear();
            genJets = tmpGenJets; 
            nGoodGenJets = genJets.size();
            nGoodGenJets_20 = genJets_20.size();
            sort(genJets.begin(), genJets.end(), JetDescendingOrder);
            if (nGoodGenJets >= 2){
                genJet1Plus2 = genJets[0].v + genJets[1].v;
                genJet1Minus2 = genJets[0].v - genJets[1].v;
            }
            genJetsHT = 0.;
            for (unsigned short i(0); i < nGoodGenJets; i++){
                genJetsHT += genJets[i].v.Pt();  
            }
            sort(genJets_20.begin(), genJets_20.end(), JetDescendingOrder);

        }
        //=======================================================================================================//


        if (DEBUG) cout << "Stop after line " << __LINE__ << endl;
        //=======================================================================================================//
        // Select the best pair of jets for DPS  //
        //=======================================//
        pair<TLorentzVector, TLorentzVector> bestTwoJets;
        TLorentzVector bestJet1Plus2, bestJet1Minus2;
        if (hasRecoInfo){
            bestTwoJetsCandidatesPt(jets, bestTwoJets);
            //bestTwoJetsCandidatesPhi(jets, bestTwoJets);
            if (nGoodJets >= 2){
                bestJet1Plus2 = bestTwoJets.first + bestTwoJets.second;
                bestJet1Minus2 = bestTwoJets.first - bestTwoJets.second;
            }
        }

        pair<TLorentzVector, TLorentzVector> genBestTwoJets;
        TLorentzVector genBestJet1Plus2, genBestJet1Minus2;
        if (hasGenInfo){
            bestTwoJetsCandidatesPt(genJets, genBestTwoJets);
            //bestTwoJetsCandidatesPhi(genJets, genBestTwoJets);
            if (nGoodGenJets >= 2){
                genBestJet1Plus2 = genBestTwoJets.first + genBestTwoJets.second; 
                genBestJet1Minus2 = genBestTwoJets.first - genBestTwoJets.second; 
            }
        }
        //=======================================================================================================//



        if (DEBUG) cout << "Stop after line " << __LINE__ << endl;
        //=======================================================================================================//
        //   Filling gen end parton histos    //
        //====================================//

        double gentau_sum(0), gentau_max(0);
        double gentau_c_sum(0), gentau_c_max(0);
        double gentau_cm_sum(0), gentau_cm_max(0);
        double gentau_c_cm_sum(0), gentau_c_cm_max(0); 

        // cout << "passesgenLeptonCut : " << passesgenLeptonCut << endl;
     
        if (hasGenInfo){
            if (passesgenLeptonCut){
                //cout << "Selected at gen level" << endl;
                nGenEventsVInc0Jets++;
                nEffGenEventsVInc0Jets += genWeight;

		double RatioValue = 1.;
		if(UnfoldUnc){
		    int binNumber = ZNGoodJets_Zexc_ratio->GetXaxis()->FindBin(nGoodJets);
		    RatioValue =  ZNGoodJets_Zexc_ratio->GetBinContent(binNumber);
		} 

                genZNGoodJets_Zexc->Fill(nGoodGenJets, genWeight*RatioValue);
                genZNGoodJets_Zinc->Fill(0., genWeight);
                genZMass_Zinc0jet->Fill(genEWKBoson.M(), genWeight);
                genZPt_Zinc0jet->Fill(genEWKBoson.Pt(), genWeight);
                genZRapidity_Zinc0jet->Fill(genEWKBoson.Rapidity(), genWeight);
                genZEta_Zinc0jet->Fill(genEWKBoson.Eta(), genWeight);
                genlepPt_Zinc0jet->Fill(genLeptons[0].v.Pt(), genWeight);
                genlepPt_Zinc0jet->Fill(genLeptons[1].v.Pt(), genWeight);
                genlepEta_Zinc0jet->Fill(genLeptons[0].v.Eta(), genWeight);
                genlepEta_Zinc0jet->Fill(genLeptons[1].v.Eta(), genWeight);

                if (nGoodGenJets_20 >= 1) {

		    double RatioValue = 1.;
		    if(nGoodJets_20 >= 1 && UnfoldUnc){
			double binNumber = FirstJetPt_2_Zinc1jet_ratio->GetXaxis()->FindBin(jets_20[0].v.Pt());
		       RatioValue =  FirstJetPt_2_Zinc1jet_ratio->GetBinContent(binNumber);
		    }
		    
		    genFirstJetPt_Zinc1jet->Fill(genJets_20[0].v.Pt(), genWeight*RatioValue);
		    genFirstJetPtEta_Zinc1jet->Fill(genJets_20[0].v.Pt(), fabs(genJets[0].v.Eta()), genWeight);
                }
                if (nGoodGenJets >= 1){
                    nGenEventsVInc1Jets++;
                    nEffGenEventsVInc1Jets += genWeight;
                    genAbsFirstJetRapidity_Zinc1jet->Fill(fabs(genJets[0].v.Rapidity()),genWeight);
                    genSumZFirstJetRapidity_Zinc1jet->Fill(fabs(genEWKBoson.Rapidity()+genJets[0].v.Rapidity())/2.0,genWeight);
                    genDifZFirstJetRapidity_Zinc1jet->Fill(fabs(genEWKBoson.Rapidity()-genJets[0].v.Rapidity())/2.0,genWeight);
                    //cross check//////
                    genSumZFirstJetEta_Zinc1jet->Fill(fabs(genEWKBoson.Eta()+genJets[0].v.Eta())/2.0,genWeight);
                    genDifZFirstJetEta_Zinc1jet->Fill(fabs(genEWKBoson.Eta()-genJets[0].v.Eta())/2.0,genWeight);

                    ///Azimuth cross check//////////////////////////
                    genDPhiZFirstJet_Zinc1jet->Fill(fabs(genEWKBoson.DeltaPhi(genJets[0].v)),genWeight);


                    if(genEWKBoson.Pt()>100.)
                    {
                        genAbsZRapidity_ZPt100_Zinc1jet->Fill(fabs(genEWKBoson.Rapidity()),genWeight);
                        genAbsFirstJetRapidity_ZPt100_Zinc1jet->Fill(fabs(genJets[0].v.Rapidity()),genWeight);
                        genSumZFirstJetRapidity_ZPt100_Zinc1jet->Fill(fabs(genEWKBoson.Rapidity()+genJets[0].v.Rapidity())/2.0,genWeight);
                        genDifZFirstJetRapidity_ZPt100_Zinc1jet->Fill(fabs(genEWKBoson.Rapidity()-genJets[0].v.Rapidity())/2.0,genWeight);

                    }

                    if(genEWKBoson.Pt()>150.)
                    {
                        genAbsZRapidity_ZPt150_Zinc1jet->Fill(fabs(genEWKBoson.Rapidity()),genWeight);
                        genAbsFirstJetRapidity_ZPt150_Zinc1jet->Fill(fabs(genJets[0].v.Rapidity()),genWeight);
                        genSumZFirstJetRapidity_ZPt150_Zinc1jet->Fill(fabs(genEWKBoson.Rapidity()+genJets[0].v.Rapidity())/2.0,genWeight);
                        genDifZFirstJetRapidity_ZPt150_Zinc1jet->Fill(fabs(genEWKBoson.Rapidity()-genJets[0].v.Rapidity())/2.0,genWeight);


                        genDPhiZFirstJet_ZPt150_Zinc1jet->Fill(fabs(genEWKBoson.DeltaPhi(genJets[0].v)),genWeight);
                    }

                    if(genEWKBoson.Pt()>300.)
                    {
                        genAbsZRapidity_ZPt300_Zinc1jet->Fill(fabs(genEWKBoson.Rapidity()),genWeight);
                        genAbsFirstJetRapidity_ZPt300_Zinc1jet->Fill(fabs(genJets[0].v.Rapidity()),genWeight);
                        genSumZFirstJetRapidity_ZPt300_Zinc1jet->Fill(fabs(genEWKBoson.Rapidity()+genJets[0].v.Rapidity())/2.0,genWeight);
                        genDifZFirstJetRapidity_ZPt300_Zinc1jet->Fill(fabs(genEWKBoson.Rapidity()-genJets[0].v.Rapidity())/2.0,genWeight);

                        genDPhiZFirstJet_ZPt300_Zinc1jet->Fill(fabs(genEWKBoson.DeltaPhi(genJets[0].v)),genWeight);
                    }

                    ///different JetPt cuts///////
                    if(genJets[0].v.Pt()>50.)
                    {
                        genAbsZRapidity_FirstJetPt50_Zinc1jet->Fill(fabs(genEWKBoson.Rapidity()),genWeight);
                        genAbsFirstJetRapidity_FirstJetPt50_Zinc1jet->Fill(fabs(genJets[0].v.Rapidity()),genWeight);
                        genSumZFirstJetRapidity_FirstJetPt50_Zinc1jet->Fill(fabs(genEWKBoson.Rapidity()+genJets[0].v.Rapidity())/2.0,genWeight);
                        genDifZFirstJetRapidity_FirstJetPt50_Zinc1jet->Fill(fabs(genEWKBoson.Rapidity()-genJets[0].v.Rapidity())/2.0,genWeight);

                    }

                    if(genJets[0].v.Pt()>80.)
                    {
                        genAbsZRapidity_FirstJetPt80_Zinc1jet->Fill(fabs(genEWKBoson.Rapidity()),genWeight);
                        genAbsFirstJetRapidity_FirstJetPt80_Zinc1jet->Fill(fabs(genJets[0].v.Rapidity()),genWeight);
                        genSumZFirstJetRapidity_FirstJetPt80_Zinc1jet->Fill(fabs(genEWKBoson.Rapidity()+genJets[0].v.Rapidity())/2.0,genWeight);
                        genDifZFirstJetRapidity_FirstJetPt80_Zinc1jet->Fill(fabs(genEWKBoson.Rapidity()-genJets[0].v.Rapidity())/2.0,genWeight);

                    }


                    for (unsigned short i(0); i < nGoodGenJets; i++) {
                        double trans_mass = genJets[i].v.Mt();

                        gentau_sum += trans_mass * exp(-fabs(genJets[i].v.Rapidity() - genEWKBoson.Rapidity())); 
                        gentau_max = max(gentau_max, trans_mass * exp(-fabs(genJets[i].v.Rapidity() - genEWKBoson.Rapidity()))); 

                        gentau_c_sum += trans_mass / (2 * cosh(genJets[i].v.Rapidity() - genEWKBoson.Rapidity()));
                        gentau_c_max = max(gentau_c_max, trans_mass / (2 * cosh(genJets[i].v.Rapidity() - genEWKBoson.Rapidity())));

                        gentau_cm_sum += trans_mass * exp(-fabs(genJets[i].v.Rapidity())); 
                        gentau_cm_max = max(gentau_cm_max, trans_mass * exp(-fabs(genJets[i].v.Rapidity()))); 

                        gentau_c_cm_sum += trans_mass / (2 * cosh(genJets[i].v.Rapidity()));
                        gentau_c_cm_max = max(gentau_c_cm_max, trans_mass / (2 * cosh(genJets[i].v.Rapidity())));

                    }

                    for (unsigned short i(0); i < 5; i++) {
                        if (genEWKBoson.Pt() > ZptRange[i] && genEWKBoson.Pt() <= ZptRange[i+1]) {
                            gentau_sum_Zinc1jet[i]->Fill(gentau_sum, genWeight);
                            gentau_max_Zinc1jet[i]->Fill(gentau_max, genWeight);
                            gentau_c_sum_Zinc1jet[i]->Fill(gentau_c_sum, genWeight);
                            gentau_c_max_Zinc1jet[i]->Fill(gentau_c_max, genWeight);
                            gentau_cm_sum_Zinc1jet[i]->Fill(gentau_cm_sum, genWeight);
                            gentau_cm_max_Zinc1jet[i]->Fill(gentau_cm_max, genWeight);
                            gentau_c_cm_sum_Zinc1jet[i]->Fill(gentau_c_cm_sum, genWeight);
                            gentau_c_cm_max_Zinc1jet[i]->Fill(gentau_c_cm_max, genWeight);
                        }
                    }

                    genZNGoodJets_Zinc->Fill(1., genWeight);
                    genZPt_Zinc1jet->Fill(genEWKBoson.Pt(), genWeight);
                    genZRapidity_Zinc1jet->Fill(genEWKBoson.Rapidity(), genWeight);
                    genZAbsRapidity_Zinc1jet->Fill(fabs(genEWKBoson.Rapidity()), genWeight);
                    genZEta_Zinc1jet->Fill(genEWKBoson.Eta(), genWeight);

                    double RatioValue = 1.;
		    double RatioValue1 = 1.;
                    if(nGoodJets >= 1 && UnfoldUnc){
			double binNumber = FirstJetEta_2_Zinc1jet_ratio ->GetXaxis()->FindBin(fabs(jets[0].v.Eta()));
                        RatioValue =  FirstJetEta_2_Zinc1jet_ratio ->GetBinContent(binNumber);
                        double binNumber1 = JetsHT_2_Zinc1jet_ratio->GetXaxis()->FindBin(jetsHT);
                        RatioValue1 = JetsHT_2_Zinc1jet_ratio->GetBinContent(binNumber1);         
                    }
                    // cout << RatioValue1 << " , " << RatioValue1 << "\n";

                    genFirstJetEta_Zinc1jet->Fill(fabs(genJets[0].v.Eta()), genWeight*RatioValue);
                    genAbsZRapidity_Zinc1jet->Fill(fabs(genEWKBoson.Rapidity()),genWeight);		    
                    genFirstJetAbsRapidity_Zinc1jet->Fill(fabs(genJets[0].v.Rapidity()), genWeight*RatioValue);
                    genFirstJetEtaHigh_Zinc1jet->Fill(fabs(genJets[0].v.Eta()), genWeight);
                    genFirstJetRapidityHigh_Zinc1jet->Fill(fabs(genJets[0].v.Rapidity()), genWeight);
                    genJetsHT_Zinc1jet->Fill(genJetsHT, genWeight*RatioValue1);
                    //genJetsHT_2_Zinc1jet->Fill(genJetsHT, genWeight);
                    genSumZJetRapidity_Zinc1jet->Fill(0.5*fabs(genEWKBoson.Rapidity()+genJets[0].v.Rapidity()), genWeight);
                    genDifZJetRapidity_Zinc1jet->Fill(0.5*fabs(genEWKBoson.Rapidity()-genJets[0].v.Rapidity()), genWeight);
                    if (nGoodGenJets == 1){
                        genFirstJetPt_Zexc1jet->Fill(genJets[0].v.Pt(), genWeight);
                        //Additional Branch
                        genAbsZRapidity_Zexc1jet->Fill(fabs(genEWKBoson.Rapidity()),genWeight);
                        genAbsJetRapidity_Zexc1jet->Fill(fabs(genJets[0].v.Rapidity()),genWeight);
                        genSumZJetRapidity_Zexc1jet->Fill(fabs(genEWKBoson.Rapidity()+genJets[0].v.Rapidity())/2.0,genWeight);
                        genDifZJetRapidity_Zexc1jet->Fill(fabs(genEWKBoson.Rapidity()-genJets[0].v.Rapidity())/2.0,genWeight);

                        if(genEWKBoson.Pt()>100.)
                        {
                            genAbsZRapidity_ZPt100_Zexc1jet->Fill(fabs(genEWKBoson.Rapidity()),genWeight);
                            genAbsJetRapidity_ZPt100_Zexc1jet->Fill(fabs(genJets[0].v.Rapidity()),genWeight);
                            genSumZJetRapidity_ZPt100_Zexc1jet->Fill(fabs(genEWKBoson.Rapidity()+genJets[0].v.Rapidity())/2.0,genWeight);
                            genDifZJetRapidity_ZPt100_Zexc1jet->Fill(fabs(genEWKBoson.Rapidity()-genJets[0].v.Rapidity())/2.0,genWeight);
                        }

                        if(genEWKBoson.Pt()>150.)
                        {
                            genAbsZRapidity_ZPt150_Zexc1jet->Fill(fabs(genEWKBoson.Rapidity()),genWeight);
                            genAbsJetRapidity_ZPt150_Zexc1jet->Fill(fabs(genJets[0].v.Rapidity()),genWeight);
                            genSumZJetRapidity_ZPt150_Zexc1jet->Fill(fabs(genEWKBoson.Rapidity()+genJets[0].v.Rapidity())/2.0,genWeight);
                            genDifZJetRapidity_ZPt150_Zexc1jet->Fill(fabs(genEWKBoson.Rapidity()-genJets[0].v.Rapidity())/2.0,genWeight);
                        }

                    }
                }
                if (nGoodGenJets_20 >= 2) {

		    double RatioValue = 1.;
		    if(nGoodJets_20 >= 2 && UnfoldUnc){
			double binNumber = SecondJetPt_2_Zinc2jet_ratio->GetXaxis()->FindBin(jets_20[1].v.Pt());
			RatioValue =  SecondJetPt_2_Zinc2jet_ratio->GetBinContent(binNumber);
		    }

                   genSecondJetPt_Zinc2jet->Fill(genJets_20[1].v.Pt(), genWeight*RatioValue);

                }
                if (nGoodGenJets >= 2) {
                    TLorentzVector genJet1Plus2PlusZ = genJet1Plus2 + genEWKBoson;
                    nGenEventsVInc2Jets++;
                    nEffGenEventsVInc2Jets += genWeight;
                    ///////////Special Branch//////////////////
                    genAbsFirstJetRapidity_Zinc2jet->Fill(fabs(genJets[0].v.Rapidity()),genWeight);
                    genSumZFirstJetRapidity_Zinc2jet->Fill(fabs(genEWKBoson.Rapidity()+genJets[0].v.Rapidity())/2.0,genWeight);
                    genDifZFirstJetRapidity_Zinc2jet->Fill(fabs(genEWKBoson.Rapidity()-genJets[0].v.Rapidity())/2.0,genWeight);

                    genAbsZRapidity_Zinc2jet->Fill(fabs(genEWKBoson.Rapidity()),genWeight);
                    genAbsSecondJetRapidity_Zinc2jet->Fill(fabs(genJets[1].v.Rapidity()),genWeight);
                    genSumZSecondJetRapidity_Zinc2jet->Fill(fabs(genEWKBoson.Rapidity()+genJets[1].v.Rapidity())/2.0,genWeight);
                    genDifZSecondJetRapidity_Zinc2jet->Fill(fabs(genEWKBoson.Rapidity()-genJets[1].v.Rapidity())/2.0,genWeight);

                    genSumFirstSecondJetRapidity_Zinc2jet->Fill(fabs(genJets[0].v.Rapidity()+genJets[1].v.Rapidity())/2.0,genWeight);
                    genDifFirstSecondJetRapidity_Zinc2jet->Fill(fabs(genJets[0].v.Rapidity()-genJets[1].v.Rapidity())/2.0,genWeight);

                    TLorentzVector genDiJets = genJets[0].v + genJets[1].v;
                    genSumZTwoJetsRapidity_Zinc2jet->Fill(fabs(genEWKBoson.Rapidity()+genDiJets.Rapidity())/2.0,genWeight);
                    genDifZTwoJetsRapidity_Zinc2jet->Fill(fabs(genEWKBoson.Rapidity()-genDiJets.Rapidity())/2.0,genWeight);

                    /////Azimuth cross check//////////////////////////////////
                    genDPhiZFirstJet_Zinc2jet->Fill(fabs(genEWKBoson.DeltaPhi(genJets[0].v)),genWeight);
                    genDPhiZSecondJet_Zinc2jet->Fill(fabs(genEWKBoson.DeltaPhi(genJets[1].v)),genWeight);
                    genDPhiFirstSecondJet_Zinc2jet->Fill(fabs(genJets[0].v.DeltaPhi(genJets[1].v)),genWeight);


                    if(genEWKBoson.Pt()>100.)
                    {
                        genAbsZRapidity_ZPt100_Zinc2jet->Fill(fabs(genEWKBoson.Rapidity()),genWeight);
                        genAbsSecondJetRapidity_ZPt100_Zinc2jet->Fill(fabs(genJets[1].v.Rapidity()),genWeight);
                        genSumZSecondJetRapidity_ZPt100_Zinc2jet->Fill(fabs(genEWKBoson.Rapidity()+genJets[1].v.Rapidity())/2.0,genWeight);
                        genDifZSecondJetRapidity_ZPt100_Zinc2jet->Fill(fabs(genEWKBoson.Rapidity()-genJets[1].v.Rapidity())/2.0,genWeight);
                    }

                    if(genEWKBoson.Pt()>150.)
                    {
                        genAbsZRapidity_ZPt150_Zinc2jet->Fill(fabs(genEWKBoson.Rapidity()),genWeight);
                        genAbsSecondJetRapidity_ZPt150_Zinc2jet->Fill(fabs(genJets[1].v.Rapidity()),genWeight);
                        genSumZSecondJetRapidity_ZPt150_Zinc2jet->Fill(fabs(genEWKBoson.Rapidity()+genJets[1].v.Rapidity())/2.0,genWeight);
                        genDifZSecondJetRapidity_ZPt150_Zinc2jet->Fill(fabs(genEWKBoson.Rapidity()-genJets[1].v.Rapidity())/2.0,genWeight);

                        genDPhiZFirstJet_ZPt150_Zinc2jet->Fill(fabs(genEWKBoson.DeltaPhi(genJets[0].v)),genWeight);
                    }

                    if(genEWKBoson.Pt()>300.)
                    {
                        genDPhiZFirstJet_ZPt300_Zinc2jet->Fill(fabs(genEWKBoson.DeltaPhi(genJets[0].v)),genWeight);
                    }

                    //Set Jet rapidity discriminator/////

                    if(fabs(genJets[0].v.Rapidity()-genJets[1].v.Rapidity())>2)
                    {
                        genAbsZRapidity_DifJetRapidityl2_Zinc2jet->Fill(fabs(genEWKBoson.Rapidity()),genWeight);
                        genAbsFirstJetRapidity_DifJetRapidityl2_Zinc2jet->Fill(fabs(genJets[0].v.Rapidity()),genWeight);
                        genSumZFirstJetRapidity_DifJetRapidityl2_Zinc2jet->Fill(fabs(genEWKBoson.Rapidity()+genJets[0].v.Rapidity())/2.0,genWeight);
                        genDifZFirstJetRapidity_DifJetRapidityl2_Zinc2jet->Fill(fabs(genEWKBoson.Rapidity()-genJets[0].v.Rapidity())/2.0,genWeight);
                    }

                    if(fabs(genJets[0].v.Rapidity()-genJets[1].v.Rapidity())<2)
                    {
                        genAbsZRapidity_DifJetRapiditys2_Zinc2jet->Fill(fabs(genEWKBoson.Rapidity()),genWeight);
                        genAbsFirstJetRapidity_DifJetRapiditys2_Zinc2jet->Fill(fabs(genJets[0].v.Rapidity()),genWeight);
                        genSumZFirstJetRapidity_DifJetRapiditys2_Zinc2jet->Fill(fabs(genEWKBoson.Rapidity()+genJets[0].v.Rapidity())/2.0,genWeight);
                        genDifZFirstJetRapidity_DifJetRapiditys2_Zinc2jet->Fill(fabs(genEWKBoson.Rapidity()-genJets[0].v.Rapidity())/2.0,genWeight);
                    }


                    genZNGoodJets_Zinc->Fill(2., genWeight);
                    genTwoJetsPtDiff_Zinc2jet->Fill(genJet1Minus2.Pt(), genWeight);
                    genBestTwoJetsPtDiff_Zinc2jet->Fill(genBestJet1Minus2.Pt(), genWeight);
                    genJetsMass_Zinc2jet->Fill(genJet1Plus2.M(), genWeight);
                    genllJetsMass_Zinc2jet->Fill(genJet1Plus2PlusZ.M(), genWeight);

                    if (genJet1Plus2PlusZ.M() > 450 && genJet1Plus2PlusZ.M() < 600) {
                        if (fabs(genJets[0].v.Eta()) < fabs(genJets[1].v.Eta())) {
                            genCentralJetEta_Zinc2jet->Fill(fabs(genJets[0].v.Eta()), genWeight);
                            genForwardJetEta_Zinc2jet->Fill(fabs(genJets[1].v.Eta()), genWeight);
                            genCentralJetPt_Zinc2jet->Fill(genJets[0].v.Pt(), genWeight);
                            genForwardJetPt_Zinc2jet->Fill(genJets[1].v.Pt(), genWeight);
                        }
                        else {
                            genCentralJetEta_Zinc2jet->Fill(fabs(genJets[1].v.Eta()), genWeight);
                            genForwardJetEta_Zinc2jet->Fill(fabs(genJets[0].v.Eta()), genWeight);
                            genCentralJetPt_Zinc2jet->Fill(genJets[1].v.Pt(), genWeight);
                            genForwardJetPt_Zinc2jet->Fill(genJets[0].v.Pt(), genWeight);
                        }
                    }

                    if (EvtVtxCnt < 14) genJetsMassLowPU_Zinc2jet->Fill(genJet1Plus2.M(), genWeight);
                    else if (EvtVtxCnt < 18) genJetsMassMidPU_Zinc2jet->Fill(genJet1Plus2.M(), genWeight);
                    else genJetsMassHigPU_Zinc2jet->Fill(genJet1Plus2.M(), genWeight);
                    genZPt_Zinc2jet->Fill(genEWKBoson.Pt(), genWeight);
                    genZRapidity_Zinc2jet->Fill(genEWKBoson.Rapidity(), genWeight);
                    genZEta_Zinc2jet->Fill(genEWKBoson.Eta(), genWeight);

                    double RatioValue =1.;
		    double RatioValue1 = 1.;
                    if(nGoodJets >= 2 && UnfoldUnc){
			double binNumber = SecondJetEta_2_Zinc2jet_ratio->GetXaxis()->FindBin(fabs(jets[1].v.Eta()));
			RatioValue =  SecondJetEta_2_Zinc2jet_ratio->GetBinContent(binNumber);
			double binNumber1 = JetsHT_2_Zinc2jet_ratio->GetXaxis()->FindBin(jetsHT);
			RatioValue1 = JetsHT_2_Zinc2jet_ratio->GetBinContent(binNumber1);
                    }

                    genSecondJetEta_Zinc2jet->Fill(fabs(genJets[1].v.Eta()), genWeight*RatioValue);
                    genSecondJetAbsRapidity_Zinc2jet->Fill(fabs(genJets[1].v.Rapidity()), genWeight*RatioValue);
                    genSecondJetEtaHigh_Zinc2jet->Fill(fabs(genJets[1].v.Eta()), genWeight);
                    genSecondJetRapidityHigh_Zinc2jet->Fill(fabs(genJets[1].v.Rapidity()), genWeight);
                    genJetsHT_Zinc2jet->Fill(genJetsHT, genWeight*RatioValue1);
                    genptBal_Zinc2jet->Fill(genJet1Plus2PlusZ.Pt(), genWeight);
                    gendPhiJets_Zinc2jet->Fill(deltaPhi(genJets[0].v, genJets[1].v), genWeight);
                    genBestdPhiJets_Zinc2jet->Fill(deltaPhi(genBestTwoJets.first, genBestTwoJets.second), genWeight);
                    gendEtaJets_Zinc2jet->Fill(genJets[0].v.Eta() - genJets[1].v.Eta(), genWeight);
                    gendEtaFirstJetZ_Zinc2jet->Fill(genJets[0].v.Eta() - genEWKBoson.Eta(), genWeight);
                    gendEtaSecondJetZ_Zinc2jet->Fill(genJets[1].v.Eta() - genEWKBoson.Eta(), genWeight);
                    gendEtaJet1Plus2Z_Zinc2jet->Fill(genJet1Plus2.Eta() - genEWKBoson.Eta(), genWeight);
                    genPHI_Zinc2jet->Fill(PHI(genLeptons[0].v, genLeptons[1].v, genJets[0].v, genJets[1].v), genWeight);
                    genBestPHI_Zinc2jet->Fill(PHI(genLeptons[0].v, genLeptons[1].v, genBestTwoJets.first, genBestTwoJets.second), genWeight);
                    genPHI_T_Zinc2jet->Fill(PHI_T(genLeptons[0].v, genLeptons[1].v, genJets[0].v, genJets[1].v), genWeight);
                    genBestPHI_T_Zinc2jet->Fill(PHI_T(genLeptons[0].v, genLeptons[1].v, genBestTwoJets.first, genBestTwoJets.second), genWeight);
                    genSpT_Zinc2jet->Fill(SpT(genLeptons[0].v, genLeptons[1].v, genJets[0].v, genJets[1].v), genWeight);
                    genBestSpT_Zinc2jet->Fill(SpT(genLeptons[0].v, genLeptons[1].v, genBestTwoJets.first, genBestTwoJets.second), genWeight);
                    genSpTJets_Zinc2jet->Fill(SpTsub(genJets[0].v, genJets[1].v), genWeight);
                    genBestSpTJets_Zinc2jet->Fill(SpTsub(genBestTwoJets.first, genBestTwoJets.second), genWeight);
                    genSpTLeptons_Zinc2jet->Fill(SpTsub(genLeptons[0].v, genLeptons[1].v), genWeight);
                    genSPhi_Zinc2jet->Fill(SPhi(genLeptons[0].v, genLeptons[1].v, genJets[0].v, genJets[1].v), genWeight);
                    genBestSPhi_Zinc2jet->Fill(SPhi(genLeptons[0].v, genLeptons[1].v, genBestTwoJets.first, genBestTwoJets.second), genWeight);

                    if (genEWKBoson.Pt() < 25){
                        genptBal_LowPt_Zinc2jet->Fill(genJet1Plus2PlusZ.Pt(), genWeight);
                        gendPhiJets_LowPt_Zinc2jet->Fill(deltaPhi(genJets[0].v, genJets[1].v), genWeight);
                        genBestdPhiJets_LowPt_Zinc2jet->Fill(deltaPhi(genBestTwoJets.first, genBestTwoJets.second), genWeight);
                        genPHI_T_LowPt_Zinc2jet->Fill(PHI_T(genLeptons[0].v, genLeptons[1].v, genJets[0].v, genJets[1].v), genWeight);
                        genBestPHI_T_LowPt_Zinc2jet->Fill(PHI_T(genLeptons[0].v, genLeptons[1].v, genBestTwoJets.first, genBestTwoJets.second), genWeight);
                        genPHI_LowPt_Zinc2jet->Fill(PHI(genLeptons[0].v, genLeptons[1].v, genJets[0].v, genJets[1].v), genWeight);
                        genBestPHI_LowPt_Zinc2jet->Fill(PHI(genLeptons[0].v, genLeptons[1].v, genBestTwoJets.first, genBestTwoJets.second), genWeight);
                        genSpTJets_LowPt_Zinc2jet->Fill(SpTsub(genJets[0].v, genJets[1].v), genWeight);
                        genBestSpTJets_LowPt_Zinc2jet->Fill(SpTsub(genBestTwoJets.first, genBestTwoJets.second), genWeight);
                        genSpTLeptons_LowPt_Zinc2jet->Fill(SpTsub(genLeptons[0].v, genLeptons[1].v), genWeight);
                        genSpT_LowPt_Zinc2jet->Fill(SpT(genLeptons[0].v, genLeptons[1].v, genJets[0].v, genJets[1].v), genWeight);
                        genBestSpT_LowPt_Zinc2jet->Fill(SpT(genLeptons[0].v, genLeptons[1].v, genBestTwoJets.first, genBestTwoJets.second), genWeight);
                        genSPhi_LowPt_Zinc2jet->Fill(SPhi(genLeptons[0].v, genLeptons[1].v, genJets[0].v, genJets[1].v), genWeight);
                        genBestSPhi_LowPt_Zinc2jet->Fill(SPhi(genLeptons[0].v, genLeptons[1].v, genBestTwoJets.first, genBestTwoJets.second), genWeight);
                        if (SpT(genLeptons[0].v, genLeptons[1].v, genJets[0].v, genJets[1].v) < 0.5){ 
                            genPHI_LowSpT_LowPt_Zinc2jet->Fill(PHI(genLeptons[0].v, genLeptons[1].v, genJets[0].v, genJets[1].v), genWeight);
                            genSPhi_LowSpT_LowPt_Zinc2jet->Fill(SPhi(genLeptons[0].v, genLeptons[1].v, genJets[0].v, genJets[1].v), genWeight);
                        }
                        else {
                            genPHI_HighSpT_LowPt_Zinc2jet->Fill(PHI(genLeptons[0].v, genLeptons[1].v, genJets[0].v, genJets[1].v), genWeight);
                            genSPhi_HighSpT_LowPt_Zinc2jet->Fill(SPhi(genLeptons[0].v, genLeptons[1].v, genJets[0].v, genJets[1].v), genWeight);
                        }
                        if (SPhi(genLeptons[0].v, genLeptons[1].v, genJets[0].v, genJets[1].v) < 0.5){
                            genSpT_LowSPhi_LowPt_Zinc2jet->Fill(SpT(genLeptons[0].v, genLeptons[1].v, genJets[0].v, genJets[1].v), genWeight);
                        }
                        else {
                            genSpT_HighSPhi_LowPt_Zinc2jet ->Fill(SpT(genLeptons[0].v, genLeptons[1].v, genJets[0].v, genJets[1].v), genWeight);
                        }
                    }
                    else {
                        genptBal_HighPt_Zinc2jet->Fill(genJet1Plus2PlusZ.Pt(), genWeight);
                        gendPhiJets_HighPt_Zinc2jet->Fill(deltaPhi(genJets[0].v, genJets[1].v), genWeight);
                        genPHI_HighPt_Zinc2jet->Fill(PHI(genLeptons[0].v, genLeptons[1].v, genJets[0].v, genJets[1].v), genWeight);
                        genPHI_T_HighPt_Zinc2jet->Fill(PHI_T(genLeptons[0].v, genLeptons[1].v, genJets[0].v, genJets[1].v), genWeight);
                        genSpTJets_HighPt_Zinc2jet->Fill(SpTsub(genJets[0].v, genJets[1].v), genWeight);
                        genSpTLeptons_HighPt_Zinc2jet->Fill(SpTsub(genLeptons[0].v, genLeptons[1].v), genWeight);
                        genSpT_HighPt_Zinc2jet->Fill(SpT(genLeptons[0].v, genLeptons[1].v, genJets[0].v, genJets[1].v), genWeight);
                        genSPhi_HighPt_Zinc2jet->Fill(SPhi(genLeptons[0].v, genLeptons[1].v, genJets[0].v, genJets[1].v), genWeight);
                    }
                    if (nGoodGenJets == 2){
                        //////Special Branch/////////////////////////
                        genAbsZRapidity_Zexc2jet->Fill(fabs(genEWKBoson.Rapidity()),genWeight);
                        genAbsSecondJetRapidity_Zexc2jet->Fill(fabs(genJets[1].v.Rapidity()),genWeight);
                        genSumZSecondJetRapidity_Zexc2jet->Fill(fabs(genEWKBoson.Rapidity()+genJets[1].v.Rapidity())/2.0,genWeight);
                        genDifZSecondJetRapidity_Zexc2jet->Fill(fabs(genEWKBoson.Rapidity()-genJets[1].v.Rapidity())/2.0,genWeight);

                        if(genEWKBoson.Pt()>100.)
                        {
                            genAbsZRapidity_ZPt100_Zexc2jet->Fill(fabs(genEWKBoson.Rapidity()),genWeight);
                            genAbsSecondJetRapidity_ZPt100_Zexc2jet->Fill(fabs(genJets[1].v.Rapidity()),genWeight);
                            genSumZSecondJetRapidity_ZPt100_Zexc2jet->Fill(fabs(genEWKBoson.Rapidity()+genJets[1].v.Rapidity())/2.0,genWeight);
                            genDifZSecondJetRapidity_ZPt100_Zexc2jet->Fill(fabs(genEWKBoson.Rapidity()-genJets[1].v.Rapidity())/2.0,genWeight);
                        }

                        if(genEWKBoson.Pt()>150.)
                        {
                            genAbsZRapidity_ZPt150_Zexc2jet->Fill(fabs(genEWKBoson.Rapidity()),genWeight);
                            genAbsSecondJetRapidity_ZPt150_Zexc2jet->Fill(fabs(genJets[1].v.Rapidity()),genWeight);
                            genSumZSecondJetRapidity_ZPt150_Zexc2jet->Fill(fabs(genEWKBoson.Rapidity()+genJets[1].v.Rapidity())/2.0,genWeight);
                            genDifZSecondJetRapidity_ZPt150_Zexc2jet->Fill(fabs(genEWKBoson.Rapidity()-genJets[1].v.Rapidity())/2.0,genWeight);
                        }


                        genTwoJetsPtDiff_Zexc2jet->Fill(genJet1Minus2.Pt(), genWeight);
                        genJetsMass_Zexc2jet->Fill(genJet1Plus2.M(), genWeight);
                        genSecondJetPt_Zexc2jet->Fill(genJets[1].v.Pt(), genWeight);
                        gendPhiJets_Zexc2jet->Fill(deltaPhi(genJets[0].v, genJets[1].v), genWeight);
                        genPHI_Zexc2jet->Fill(PHI(genLeptons[0].v, genLeptons[1].v, genJets[0].v, genJets[1].v), genWeight);
                        genPHI_T_Zexc2jet->Fill(PHI_T(genLeptons[0].v, genLeptons[1].v, genJets[0].v, genJets[1].v), genWeight);
                        gendEtaJets_Zexc2jet->Fill(genJets[0].v.Eta() - genJets[1].v.Eta(), genWeight);
                        gendEtaFirstJetZ_Zexc2jet->Fill(genJets[0].v.Eta() - genEWKBoson.Eta(), genWeight);
                        gendEtaSecondJetZ_Zexc2jet->Fill(genJets[1].v.Eta() - genEWKBoson.Eta(), genWeight);
                        gendEtaJet1Plus2Z_Zexc2jet->Fill(genJet1Plus2.Eta() - genEWKBoson.Eta(), genWeight);
                        genSpT_Zexc2jet->Fill(SpT(genLeptons[0].v, genLeptons[1].v, genJets[0].v, genJets[1].v), genWeight);
                        genSpTJets_Zexc2jet->Fill(SpTsub(genJets[0].v, genJets[1].v), genWeight);
                        genSpTLeptons_Zexc2jet->Fill(SpTsub(genLeptons[0].v, genLeptons[1].v), genWeight);
                        genSPhi_Zexc2jet->Fill(SPhi(genLeptons[0].v, genLeptons[1].v, genJets[0].v, genJets[1].v), genWeight);
                        genptBal_Zexc2jet->Fill(genJet1Plus2PlusZ.Pt(), genWeight);

                        if (genEWKBoson.Pt() < 25){
                            genptBal_LowPt_Zexc2jet->Fill(genJet1Plus2PlusZ.Pt(), genWeight);
                            gendPhiJets_LowPt_Zexc2jet->Fill(deltaPhi(genJets[0].v, genJets[1].v), genWeight);
                            genPHI_T_LowPt_Zexc2jet->Fill(PHI_T(genLeptons[0].v, genLeptons[1].v, genJets[0].v, genJets[1].v), genWeight);
                            genPHI_LowPt_Zexc2jet->Fill(PHI(genLeptons[0].v, genLeptons[1].v, genJets[0].v, genJets[1].v), genWeight);
                            genSpTJets_LowPt_Zexc2jet->Fill(SpTsub(genJets[0].v, genJets[1].v), genWeight);
                            genSpTLeptons_LowPt_Zexc2jet->Fill(SpTsub(genLeptons[0].v, genLeptons[1].v), genWeight);
                            genSpT_LowPt_Zexc2jet->Fill(SpT(genLeptons[0].v, genLeptons[1].v, genJets[0].v, genJets[1].v), genWeight);
                            genSPhi_LowPt_Zexc2jet->Fill(SPhi(genLeptons[0].v, genLeptons[1].v, genJets[0].v, genJets[1].v), genWeight);
                            if (SpT(genLeptons[0].v, genLeptons[1].v, genJets[0].v, genJets[1].v) < 0.5) { 
                                genPHI_LowSpT_LowPt_Zexc2jet->Fill(PHI(genLeptons[0].v, genLeptons[1].v, genJets[0].v, genJets[1].v), genWeight);
                                genSPhi_LowSpT_LowPt_Zexc2jet->Fill(SPhi(genLeptons[0].v, genLeptons[1].v, genJets[0].v, genJets[1].v), genWeight);
                            }
                            else {
                                genPHI_HighSpT_LowPt_Zexc2jet->Fill(PHI(genLeptons[0].v, genLeptons[1].v, genJets[0].v, genJets[1].v), genWeight);
                                genSPhi_HighSpT_LowPt_Zexc2jet->Fill(SPhi(genLeptons[0].v, genLeptons[1].v, genJets[0].v, genJets[1].v), genWeight);
                            }
                            if (SPhi(genLeptons[0].v, genLeptons[1].v, genJets[0].v, genJets[1].v) < 0.5) {
                                genSpT_LowSPhi_LowPt_Zexc2jet->Fill(SpT(genLeptons[0].v, genLeptons[1].v, genJets[0].v, genJets[1].v), genWeight);
                            }
                            else {
                                genSpT_HighSPhi_LowPt_Zexc2jet ->Fill(SpT(genLeptons[0].v, genLeptons[1].v, genJets[0].v, genJets[1].v), genWeight);
                            }
                        }
                        else {
                            genptBal_HighPt_Zexc2jet->Fill(genJet1Plus2PlusZ.Pt(), genWeight);
                            gendPhiJets_HighPt_Zexc2jet->Fill(deltaPhi(genJets[0].v, genJets[1].v), genWeight);
                            genPHI_HighPt_Zexc2jet->Fill(PHI(genLeptons[0].v, genLeptons[1].v, genJets[0].v, genJets[1].v), genWeight);
                            genPHI_T_HighPt_Zexc2jet->Fill(PHI_T(genLeptons[0].v, genLeptons[1].v, genJets[0].v, genJets[1].v), genWeight);
                            genSpTJets_HighPt_Zexc2jet->Fill(SpTsub(genJets[0].v, genJets[1].v), genWeight);
                            genSpTLeptons_HighPt_Zexc2jet->Fill(SpTsub(genLeptons[0].v, genLeptons[1].v), genWeight);
                            genSpT_HighPt_Zexc2jet->Fill(SpT(genLeptons[0].v, genLeptons[1].v, genJets[0].v, genJets[1].v), genWeight);
                            genSPhi_HighPt_Zexc2jet->Fill(SPhi(genLeptons[0].v, genLeptons[1].v, genJets[0].v, genJets[1].v), genWeight);
                        }
                    }

                }
                if (nGoodGenJets_20 >= 3) {

                    double RatioValue = 1.;
                    if(nGoodJets_20 >= 3 && UnfoldUnc){
                       double binNumber = ThirdJetPt_2_Zinc3jet_ratio->GetXaxis()->FindBin(jets_20[2].v.Pt());
                       RatioValue =  ThirdJetPt_2_Zinc3jet_ratio->GetBinContent(binNumber);
                    }

                    genThirdJetPt_Zinc3jet->Fill(genJets_20[2].v.Pt(), genWeight*RatioValue);


                }
                if (nGoodGenJets >= 3){
                    nGenEventsVInc3Jets++;
                    nEffGenEventsVInc3Jets += genWeight;
                    genZNGoodJets_Zinc->Fill(3., genWeight);

                    double RatioValue = 1.;
		    double RatioValue1 = .1;
                    if(nGoodJets >= 3 && UnfoldUnc){
                       double binNumber = ThirdJetEta_2_Zinc3jet_ratio->GetXaxis()->FindBin(fabs(jets[2].v.Eta()));
                       RatioValue =  ThirdJetEta_2_Zinc3jet_ratio->GetBinContent(binNumber);
                       double binNumber1 = JetsHT_2_Zinc3jet_ratio->GetXaxis()->FindBin(jetsHT);
                       RatioValue1 = JetsHT_2_Zinc3jet_ratio->GetBinContent(binNumber1);
                    }

                    genThirdJetEta_Zinc3jet->Fill(fabs(genJets[2].v.Eta()), genWeight*RatioValue);
                    genThirdJetAbsRapidity_Zinc3jet->Fill(fabs(genJets[2].v.Rapidity()), genWeight*RatioValue);
                    genThirdJetEtaHigh_Zinc3jet->Fill(fabs(genJets[2].v.Eta()), genWeight);
                    genThirdJetRapidityHigh_Zinc3jet->Fill(fabs(genJets[2].v.Rapidity()), genWeight);
                    genJetsHT_Zinc3jet->Fill(genJetsHT, genWeight*RatioValue1);
 
                    /////Azimuth cross check//////////////////////////////////
                    genDPhiZFirstJet_Zinc3jet->Fill(fabs(genEWKBoson.DeltaPhi(genJets[0].v)),genWeight);
                    genDPhiZSecondJet_Zinc3jet->Fill(fabs(genEWKBoson.DeltaPhi(genJets[1].v)),genWeight);
                    genDPhiZThirdJet_Zinc3jet->Fill(fabs(genEWKBoson.DeltaPhi(genJets[2].v)),genWeight);
                    genDPhiFirstSecondJet_Zinc3jet->Fill(fabs(genJets[0].v.DeltaPhi(genJets[1].v)),genWeight);
                    genDPhiFirstThirdJet_Zinc3jet->Fill(fabs(genJets[0].v.DeltaPhi(genJets[2].v)),genWeight);
                    genDPhiSecondThirdJet_Zinc3jet->Fill(fabs(genJets[1].v.DeltaPhi(genJets[2].v)),genWeight);

                    if(genEWKBoson.Pt()>150.)
                    {
                        genDPhiZFirstJet_ZPt150_Zinc3jet->Fill(fabs(genEWKBoson.DeltaPhi(genJets[0].v)),genWeight);
                        genDPhiZSecondJet_ZPt150_Zinc3jet->Fill(fabs(genEWKBoson.DeltaPhi(genJets[1].v)),genWeight);
                        genDPhiZThirdJet_ZPt150_Zinc3jet->Fill(fabs(genEWKBoson.DeltaPhi(genJets[2].v)),genWeight);
                        genDPhiFirstSecondJet_ZPt150_Zinc3jet->Fill(fabs(genJets[0].v.DeltaPhi(genJets[1].v)),genWeight);
                        genDPhiFirstThirdJet_ZPt150_Zinc3jet->Fill(fabs(genJets[0].v.DeltaPhi(genJets[2].v)),genWeight);
                        genDPhiSecondThirdJet_ZPt150_Zinc3jet->Fill(fabs(genJets[1].v.DeltaPhi(genJets[2].v)),genWeight);
                    }

                    if(genEWKBoson.Pt()>300.)
                    {
                        genDPhiZFirstJet_ZPt300_Zinc3jet->Fill(fabs(genEWKBoson.DeltaPhi(genJets[0].v)),genWeight);
                        genDPhiZSecondJet_ZPt300_Zinc3jet->Fill(fabs(genEWKBoson.DeltaPhi(genJets[1].v)),genWeight);
                        genDPhiZThirdJet_ZPt300_Zinc3jet->Fill(fabs(genEWKBoson.DeltaPhi(genJets[2].v)),genWeight);
                        genDPhiFirstSecondJet_ZPt300_Zinc3jet->Fill(fabs(genJets[0].v.DeltaPhi(genJets[1].v)),genWeight);
                        genDPhiFirstThirdJet_ZPt300_Zinc3jet->Fill(fabs(genJets[0].v.DeltaPhi(genJets[2].v)),genWeight);
                        genDPhiSecondThirdJet_ZPt300_Zinc3jet->Fill(fabs(genJets[1].v.DeltaPhi(genJets[2].v)),genWeight);
                    }

                    if(genEWKBoson.Pt()>150. && (genJets[0].v.Pt()+genJets[1].v.Pt()+genJets[2].v.Pt()>300.))
                    {
                        genDPhiZFirstJet_ZPt150_HT300_Zinc3jet->Fill(fabs(genEWKBoson.DeltaPhi(genJets[0].v)),genWeight);
                        genDPhiZSecondJet_ZPt150_HT300_Zinc3jet->Fill(fabs(genEWKBoson.DeltaPhi(genJets[1].v)),genWeight);
                        genDPhiZThirdJet_ZPt150_HT300_Zinc3jet->Fill(fabs(genEWKBoson.DeltaPhi(genJets[2].v)),genWeight);
                    }

                }
                if (nGoodGenJets_20 >= 4) genFourthJetPt_Zinc4jet->Fill(genJets_20[3].v.Pt(), genWeight);
                if (nGoodGenJets >= 4){
                    genZNGoodJets_Zinc->Fill(4., genWeight);
                    genFourthJetEta_Zinc4jet->Fill(fabs(genJets[3].v.Eta()), genWeight);
                    genFourthJetAbsRapidity_Zinc4jet->Fill(fabs(genJets[3].v.Rapidity()), genWeight);
                    genFourthJetEtaHigh_Zinc4jet->Fill(fabs(genJets[3].v.Eta()), genWeight);
                    genFourthJetRapidityHigh_Zinc4jet->Fill(fabs(genJets[3].v.Rapidity()), genWeight);
                    genJetsHT_Zinc4jet->Fill(genJetsHT, genWeight);
                }
                if (nGoodGenJets_20 >= 5) genFifthJetPt_Zinc5jet->Fill(genJets_20[4].v.Pt(), genWeight);
                if (nGoodGenJets >= 5){
                    genZNGoodJets_Zinc->Fill(5., genWeight);
                    genFifthJetEta_Zinc5jet->Fill(fabs(genJets[4].v.Eta()), genWeight);
                    genFifthJetAbsRapidity_Zinc5jet->Fill(fabs(genJets[4].v.Rapidity()), genWeight);
                    genFifthJetEtaHigh_Zinc5jet->Fill(fabs(genJets[4].v.Eta()), genWeight);
                    genFifthJetRapidityHigh_Zinc5jet->Fill(fabs(genJets[4].v.Rapidity()), genWeight);
                    genJetsHT_Zinc5jet->Fill(genJetsHT, genWeight);
                }
                if (nGoodGenJets_20 >= 6) genSixthJetPt_Zinc6jet->Fill(genJets_20[5].v.Pt(), genWeight);
                if (nGoodGenJets >= 6){
                    genZNGoodJets_Zinc->Fill(6., genWeight);
                    genSixthJetEta_Zinc6jet->Fill(fabs(genJets[5].v.Eta()), genWeight);
                    genSixthJetEtaHigh_Zinc6jet->Fill(fabs(genJets[5].v.Eta()), genWeight);
                    genSixthJetRapidityHigh_Zinc6jet->Fill(fabs(genJets[5].v.Rapidity()), genWeight);
                    genJetsHT_Zinc6jet->Fill(genJetsHT, genWeight);
                }
                if (nGoodGenJets >= 7) {
                    genZNGoodJets_Zinc->Fill(7., genWeight);               
                }
                if (nGoodGenJets >= 8) {
                    genZNGoodJets_Zinc->Fill(8., genWeight);
                }
            }
        }
        //=======================================================================================================//
        if (DEBUG) cout << "Stop after line " << __LINE__ << endl;




        //=======================================================================================================//
        //      Selection for Reco Histos      //
        //====================================//
        double tau_sum(0), tau_max(0);
        double tau_c_sum(0), tau_c_max(0);
        double tau_cm_sum(0), tau_cm_max(0);
        double tau_c_cm_sum(0), tau_c_cm_max(0); 

        if (hasRecoInfo && passesLeptonChargeCut && passesTauCut) {
            ZMassFrom60_Zinc0jet->Fill((leptons[0].v + leptons[1].v).M(), weight);
        }
       //  cout << passesLeptonCut << " , " << (!bTagJetFound || !rejectBTagEvents)  << "\n";
        if (hasRecoInfo && passesLeptonCut && (!bTagJetFound || !rejectBTagEvents)) { 
            //=======================================================================================================//

            if (DEBUG) cout << "Stop after line " << __LINE__ << endl;
            //=======================================================================================================//
            //      Start filling histograms      //
            //====================================//

            //cout << "Selected at reco level" << endl;
            NVtx->Fill(EvtVtxCnt, weight);

	    
            double weightNoPUweight(weight);
            if (hasRecoInfo && !EvtIsRealData) weightNoPUweight /= puWeight.weight(int(EvtPuCntTruth));
            NVtx_NoPUweight->Fill(EvtVtxCnt, weightNoPUweight);

            nEventsVInc0Jets++;
            nEffEventsVInc0Jets += weight;
            ZNGoodJetsNVtx_Zexc->Fill(nGoodJets, EvtVtxCnt  , weight);
            ZNGoodJets_Zinc->Fill(0., weight);

	    double RatioValue = 1;
	    if(UnfoldUnc){
		double binNumber = ZNGoodJets_Zexc_ratio->GetXaxis()->FindBin(nGoodJets);
		RatioValue =  ZNGoodJets_Zexc_ratio->GetBinContent(binNumber);
	    }

            ZNGoodJets_Zexc->Fill(nGoodJets, weight*RatioValue);
            ZNGoodJets_Zinc_NoWeight->Fill(0.);
            ZMass_Zinc0jet->Fill(EWKBoson.M(), weight);
            ZPt_Zinc0jet->Fill(EWKBoson.Pt(), weight);
            ZRapidity_Zinc0jet->Fill(EWKBoson.Rapidity(), weight);
            ZEta_Zinc0jet->Fill(EWKBoson.Eta(), weight);
            ZEtaUpTo5_Zinc0jet->Fill(EWKBoson.Eta(), weight);
            lepPt_Zinc0jet->Fill(leptons[0].v.Pt(), weight);
            lepEta_Zinc0jet->Fill(leptons[0].v.Eta(), weight);
            lepPhi_Zinc0jet->Fill(leptons[0].v.Phi(), weight);
            lepPt_Zinc0jet->Fill(leptons[1].v.Pt(), weight);
            lepEta_Zinc0jet->Fill(leptons[1].v.Eta(), weight);
            lepPhi_Zinc0jet->Fill(leptons[1].v.Phi(), weight);
            dPhiLeptons_Zinc0jet->Fill(deltaPhi(leptons[0].v, leptons[1].v), weight);
            dEtaLeptons_Zinc0jet->Fill(leptons[0].v.Eta() - leptons[1].v.Eta(), weight);
            dRLeptons_Zinc0jet->Fill(deltaR(leptons[0].v, leptons[1].v), weight);
            SpTLeptons_Zinc0jet->Fill(SpTsub(leptons[0].v, leptons[1].v), weight);

            if (nGoodJets == 0){
                //TruePU_0->Fill(EvtPuCntTruth, weight);
                //PU_0->Fill(EvtPuCnt, weight);
                PU_0->Fill(EvtVtxCnt, weight);
                ZNGoodJets_Zexc_NoWeight->Fill(0.);
                ZPt_Zexc0jet->Fill(EWKBoson.Pt(), weight);
                ZRapidity_Zexc0jet->Fill(EWKBoson.Rapidity(), weight);
                ZEta_Zexc0jet->Fill(EWKBoson.Eta(), weight);
                lepPt_Zexc0jet->Fill(leptons[0].v.Pt(), weight);
                lepEta_Zexc0jet->Fill(leptons[0].v.Eta(), weight);
                lepPt_Zexc0jet->Fill(leptons[1].v.Pt(), weight);
                lepEta_Zexc0jet->Fill(leptons[1].v.Eta(), weight);
                dPhiLeptons_Zexc0jet->Fill(deltaPhi(leptons[0].v, leptons[1].v), weight);
                dEtaLeptons_Zexc0jet->Fill(leptons[0].v.Eta() - leptons[1].v.Eta(), weight);
                SpTLeptons_Zexc0jet->Fill(SpTsub(leptons[0].v, leptons[0].v), weight);
            }

            if (nGoodJets_20 >= 1) {
		
		double RatioValue=1;
		if(UnfoldUnc){
		    double binNumber = FirstJetPt_2_Zinc1jet_ratio->GetXaxis()->FindBin(jets_20[0].v.Pt());
		    RatioValue =  FirstJetPt_2_Zinc1jet_ratio->GetBinContent(binNumber);
		}

                FirstJetPt_Zinc1jet->Fill(jets_20[0].v.Pt(), weight*RatioValue);
                if(nEvents % 2) FirstJetPt_Zinc1jet_Odd->Fill(jets_20[0].v.Pt(), weight*RatioValue);
                else FirstJetPt_Zinc1jet_Even->Fill(jets_20[0].v.Pt(), weight*RatioValue);
                FirstJetPt_2_Zinc1jet->Fill(jets_20[0].v.Pt(), weight);
                FirstJetPt_Zinc1jet_NVtx->Fill(jets_20[0].v.Pt(), EvtVtxCnt, weight);
                FirstJetPtEta_Zinc1jet->Fill(jets_20[0].v.Pt(), fabs(jets[0].v.Eta()), weight);
            }

            if (nGoodJets >= 1){
                nEventsVInc1Jets++;
                nEffEventsVInc1Jets += weight;
                AbsZRapidity_Zinc1jet->Fill(fabs(EWKBoson.Rapidity()),weight);
                AbsFirstJetRapidity_Zinc1jet->Fill(fabs(jets[0].v.Rapidity()),weight);
                SumZFirstJetRapidity_Zinc1jet->Fill(fabs(EWKBoson.Rapidity()+jets[0].v.Rapidity())/2.0,weight);
                DifZFirstJetRapidity_Zinc1jet->Fill(fabs(EWKBoson.Rapidity()-jets[0].v.Rapidity())/2.0,weight);

                //cross check//////
                SumZFirstJetEta_Zinc1jet->Fill(fabs(EWKBoson.Eta()+jets[0].v.Eta())/2.0,weight);
                DifZFirstJetEta_Zinc1jet->Fill(fabs(EWKBoson.Eta()-jets[0].v.Eta())/2.0,weight);

                ///Azimuth cross check/////////////////////////////
                DPhiZFirstJet_Zinc1jet->Fill(fabs(EWKBoson.DeltaPhi(jets[0].v)),weight);


                if(EWKBoson.Pt()>100.)
                {
                    AbsZRapidity_ZPt100_Zinc1jet->Fill(fabs(EWKBoson.Rapidity()),weight);
                    AbsFirstJetRapidity_ZPt100_Zinc1jet->Fill(fabs(jets[0].v.Rapidity()),weight);
                    SumZFirstJetRapidity_ZPt100_Zinc1jet->Fill(fabs(EWKBoson.Rapidity()+jets[0].v.Rapidity())/2.0,weight);
                    DifZFirstJetRapidity_ZPt100_Zinc1jet->Fill(fabs(EWKBoson.Rapidity()-jets[0].v.Rapidity())/2.0,weight);
                }

                if(EWKBoson.Pt()>150.)
                {
                    AbsZRapidity_ZPt150_Zinc1jet->Fill(fabs(EWKBoson.Rapidity()),weight);
                    AbsFirstJetRapidity_ZPt150_Zinc1jet->Fill(fabs(jets[0].v.Rapidity()),weight);
                    SumZFirstJetRapidity_ZPt150_Zinc1jet->Fill(fabs(EWKBoson.Rapidity()+jets[0].v.Rapidity())/2.0,weight);
                    DifZFirstJetRapidity_ZPt150_Zinc1jet->Fill(fabs(EWKBoson.Rapidity()-jets[0].v.Rapidity())/2.0,weight);

                    DPhiZFirstJet_ZPt150_Zinc1jet->Fill(fabs(EWKBoson.DeltaPhi(jets[0].v)),weight);

                }

                if(EWKBoson.Pt()>300.)
                {
                    AbsZRapidity_ZPt300_Zinc1jet->Fill(fabs(EWKBoson.Rapidity()),weight);
                    AbsFirstJetRapidity_ZPt300_Zinc1jet->Fill(fabs(jets[0].v.Rapidity()),weight);
                    SumZFirstJetRapidity_ZPt300_Zinc1jet->Fill(fabs(EWKBoson.Rapidity()+jets[0].v.Rapidity())/2.0,weight);
                    DifZFirstJetRapidity_ZPt300_Zinc1jet->Fill(fabs(EWKBoson.Rapidity()-jets[0].v.Rapidity())/2.0,weight);

                    DPhiZFirstJet_ZPt300_Zinc1jet->Fill(fabs(EWKBoson.DeltaPhi(jets[0].v)),weight);
                }

                ///different JetPt Cuts//////
                if(jets[0].v.Pt()>50.)
                {
                    AbsZRapidity_FirstJetPt50_Zinc1jet->Fill(fabs(EWKBoson.Rapidity()),weight);
                    AbsFirstJetRapidity_FirstJetPt50_Zinc1jet->Fill(fabs(jets[0].v.Rapidity()),weight);
                    SumZFirstJetRapidity_FirstJetPt50_Zinc1jet->Fill(fabs(EWKBoson.Rapidity()+jets[0].v.Rapidity())/2.0,weight);
                    DifZFirstJetRapidity_FirstJetPt50_Zinc1jet->Fill(fabs(EWKBoson.Rapidity()-jets[0].v.Rapidity())/2.0,weight);
                }

                if(jets[0].v.Pt()>80.)
                {
                    AbsZRapidity_FirstJetPt80_Zinc1jet->Fill(fabs(EWKBoson.Rapidity()),weight);
                    AbsFirstJetRapidity_FirstJetPt80_Zinc1jet->Fill(fabs(jets[0].v.Rapidity()),weight);
                    SumZFirstJetRapidity_FirstJetPt80_Zinc1jet->Fill(fabs(EWKBoson.Rapidity()+jets[0].v.Rapidity())/2.0,weight);
                    DifZFirstJetRapidity_FirstJetPt80_Zinc1jet->Fill(fabs(EWKBoson.Rapidity()-jets[0].v.Rapidity())/2.0,weight);
                }

                ZNGoodJets_Zinc->Fill(1., weight);
                ZNGoodJets_Zinc_NoWeight->Fill(1.);
                ZPt_Zinc1jet->Fill(EWKBoson.Pt(), weight);
                lepPt_Zinc1jet->Fill(leptons[0].v.Pt(), weight);
                lepEta_Zinc1jet->Fill(leptons[0].v.Eta(), weight);  
                lepPt_Zinc1jet->Fill(leptons[1].v.Pt(), weight);
                lepEta_Zinc1jet->Fill(leptons[1].v.Eta(), weight);
                dPhiLeptons_Zinc1jet->Fill(deltaPhi(leptons[0].v, leptons[1].v), weight);
                dRLeptons_Zinc1jet->Fill(deltaPhi(leptons[0].v, leptons[1].v), weight);
                dEtaLeptons_Zinc1jet->Fill(leptons[0].v.Eta() - leptons[1].v.Eta(), weight);
                ZMass_Zinc1jet->Fill(EWKBoson.M(), weight);
                ZRapidity_Zinc1jet->Fill(EWKBoson.Rapidity(), weight);
                ZAbsRapidity_Zinc1jet->Fill(fabs(EWKBoson.Rapidity()), weight);
                ZEta_Zinc1jet->Fill(EWKBoson.Eta(), weight);
                ZEtaUpTo5_Zinc1jet->Fill(EWKBoson.Eta(), weight);
                SpTLeptons_Zinc1jet->Fill(SpTsub(leptons[0].v, leptons[1].v), weight);


		double RatioValue = 1.;
		double RatioValue1 = 1.;
		
		if(UnfoldUnc){
		    double binNumber = FirstJetEta_2_Zinc1jet_ratio->GetXaxis()->FindBin(fabs(jets[0].v.Eta()));
		    RatioValue = FirstJetEta_2_Zinc1jet_ratio->GetBinContent(binNumber);
		    double binNumber1 = JetsHT_2_Zinc1jet_ratio->GetXaxis()->FindBin(jetsHT);
		    RatioValue1 = JetsHT_2_Zinc1jet_ratio->GetBinContent(binNumber1);
		}

                FirstJetEta_Zinc1jet->Fill(fabs(jets[0].v.Eta()), weight*RatioValue);
                FirstJetEta_2_Zinc1jet->Fill(fabs(jets[0].v.Eta()), weight);
                FirstJetAbsRapidity_Zinc1jet->Fill(fabs(jets[0].v.Rapidity()), weight*RatioValue);
                if(nEvents % 2) FirstJetAbsRapidity_Zinc1jet_Odd->Fill(fabs(jets[0].v.Rapidity()), weight*RatioValue);
		else FirstJetAbsRapidity_Zinc1jet_Even->Fill(fabs(jets[0].v.Rapidity()), weight*RatioValue);
                FirstJetEtaHigh_Zinc1jet->Fill(fabs(jets[0].v.Eta()), weight);
                FirstJetRapidityHigh_Zinc1jet->Fill(fabs(jets[0].v.Rapidity()), weight);
                FirstJetEtaFull_Zinc1jet->Fill(jets[0].v.Eta(), weight);
                FirstJetPhi_Zinc1jet->Fill(jets[0].v.Phi(), weight);
                JetsHT_Zinc1jet->Fill(jetsHT, weight*RatioValue1);
                JetsHT_2_Zinc1jet->Fill(jetsHT, weight);
                dEtaBosonJet_Zinc1jet->Fill(fabs(jets[0].v.Eta() - EWKBoson.Eta()), weight);
                SumZJetRapidity_Zinc1jet->Fill(0.5*fabs(EWKBoson.Rapidity()+jets[0].v.Rapidity()), weight);
                DifZJetRapidity_Zinc1jet->Fill(0.5*fabs(EWKBoson.Rapidity()-jets[0].v.Rapidity()), weight);

                for (unsigned short i(0); i < nGoodJets; i++) {
                    double trans_mass = jets[i].v.Mt();

                    tau_sum += trans_mass * exp(-fabs(jets[i].v.Rapidity() - EWKBoson.Rapidity())); 
                    tau_max = max(tau_max, trans_mass * exp(-fabs(jets[i].v.Rapidity() - EWKBoson.Rapidity()))); 

                    tau_c_sum += trans_mass / (2 * cosh(jets[i].v.Rapidity() - EWKBoson.Rapidity()));
                    tau_c_max = max(tau_c_max, trans_mass / (2 * cosh(jets[i].v.Rapidity() - EWKBoson.Rapidity())));

                    tau_cm_sum += trans_mass * exp(-fabs(jets[i].v.Rapidity())); 
                    tau_cm_max = max(tau_cm_max, trans_mass * exp(-fabs(jets[i].v.Rapidity()))); 

                    tau_c_cm_sum += trans_mass / (2 * cosh(jets[i].v.Rapidity()));
                    tau_c_cm_max = max(tau_c_cm_max, trans_mass / (2 * cosh(jets[i].v.Rapidity())));
                }

                for (unsigned short i(0); i < 5; i++) {
                    if (EWKBoson.Pt() > ZptRange[i] && EWKBoson.Pt() <= ZptRange[i+1]) {
                        tau_sum_Zinc1jet[i]->Fill(tau_sum, weight);
                        tau_max_Zinc1jet[i]->Fill(tau_max, weight);
                        tau_c_sum_Zinc1jet[i]->Fill(tau_c_sum, weight);
                        tau_c_max_Zinc1jet[i]->Fill(tau_c_max, weight);
                        tau_cm_sum_Zinc1jet[i]->Fill(tau_cm_sum, weight);
                        tau_cm_max_Zinc1jet[i]->Fill(tau_cm_max, weight);
                        tau_c_cm_sum_Zinc1jet[i]->Fill(tau_c_cm_sum, weight);
                        tau_c_cm_max_Zinc1jet[i]->Fill(tau_c_cm_max, weight);
                    }
                }

                if (nGoodJets == 1){
                    //TruePU_1->Fill(EvtPuCntTruth, weight);
                    //PU_1->Fill(EvtPuCnt, weight);
                    PU_1->Fill(EvtVtxCnt, weight);
                    ZNGoodJets_Zexc_NoWeight->Fill(1.);
                    ZPt_Zexc1jet->Fill(EWKBoson.Pt(), weight);
                    ZRapidity_Zexc1jet->Fill(EWKBoson.Rapidity(), weight);
                    ZEta_Zexc1jet->Fill(EWKBoson.Eta(), weight);
                    SpTLeptons_Zexc1jet->Fill(SpTsub(leptons[0].v, leptons[1].v), weight);
                    FirstJetPt_Zexc1jet->Fill(jets[0].v.Pt(), weight);
                    FirstJetEta_Zexc1jet->Fill(jets[0].v.Eta(), weight);
                    FirstJetPhi_Zexc1jet->Fill(jets[0].v.Phi(), weight);
                    dEtaBosonJet_Zexc1jet->Fill(fabs(jets[0].v.Eta()-EWKBoson.Eta()), weight);
                    //Additional Branch
                    AbsZRapidity_Zexc1jet->Fill(fabs(EWKBoson.Rapidity()), weight);
                    AbsJetRapidity_Zexc1jet->Fill(fabs(jets[0].v.Rapidity()), weight);
                    SumZJetRapidity_Zexc1jet->Fill(fabs(EWKBoson.Rapidity()+jets[0].v.Rapidity())/2.0,weight);
                    DifZJetRapidity_Zexc1jet->Fill(fabs(EWKBoson.Rapidity()-jets[0].v.Rapidity())/2.0,weight);

                    if(EWKBoson.Pt()>100.)
                    {
                        AbsZRapidity_ZPt100_Zexc1jet->Fill(fabs(EWKBoson.Rapidity()), weight);
                        AbsJetRapidity_ZPt100_Zexc1jet->Fill(fabs(jets[0].v.Rapidity()), weight);
                        SumZJetRapidity_ZPt100_Zexc1jet->Fill(fabs(EWKBoson.Rapidity()+jets[0].v.Rapidity())/2.0,weight);
                        DifZJetRapidity_ZPt100_Zexc1jet->Fill(fabs(EWKBoson.Rapidity()-jets[0].v.Rapidity())/2.0,weight);
                    }

                    if(EWKBoson.Pt()>150.)
                    {
                        AbsZRapidity_ZPt150_Zexc1jet->Fill(fabs(EWKBoson.Rapidity()), weight);
                        AbsJetRapidity_ZPt150_Zexc1jet->Fill(fabs(jets[0].v.Rapidity()), weight);
                        SumZJetRapidity_ZPt150_Zexc1jet->Fill(fabs(EWKBoson.Rapidity()+jets[0].v.Rapidity())/2.0,weight);
                        DifZJetRapidity_ZPt150_Zexc1jet->Fill(fabs(EWKBoson.Rapidity()-jets[0].v.Rapidity())/2.0,weight);
                    }


                }
            }
            if (nGoodJets_20 >= 2) {
		double RatioValue = 1.;
		if(UnfoldUnc){
		    double binNumber = SecondJetPt_2_Zinc2jet_ratio->GetXaxis()->FindBin(jets_20[1].v.Pt());
		    RatioValue =  SecondJetPt_2_Zinc2jet_ratio->GetBinContent(binNumber);
                }
		
                SecondJetPt_Zinc2jet->Fill(jets_20[1].v.Pt(), weight*RatioValue);
                if(nEvents%2) SecondJetPt_Zinc2jet_Odd->Fill(jets_20[1].v.Pt(), weight*RatioValue);
                else SecondJetPt_Zinc2jet_Even->Fill(jets_20[1].v.Pt(), weight*RatioValue);
                SecondJetPt_2_Zinc2jet->Fill(jets_20[1].v.Pt(), weight);
            }
            if (nGoodJets >= 2){
                nEventsVInc2Jets++;
                nEffEventsVInc2Jets += weight;
                //////////////////Special Branch////////////////////////
                AbsFirstJetRapidity_Zinc2jet->Fill(fabs(jets[0].v.Rapidity()),weight);
                SumZFirstJetRapidity_Zinc2jet->Fill(fabs(EWKBoson.Rapidity()+jets[0].v.Rapidity())/2.0,weight);
                DifZFirstJetRapidity_Zinc2jet->Fill(fabs(EWKBoson.Rapidity()-jets[0].v.Rapidity())/2.0,weight);

                AbsZRapidity_Zinc2jet->Fill(fabs(EWKBoson.Rapidity()),weight);
                AbsSecondJetRapidity_Zinc2jet->Fill(fabs(jets[1].v.Rapidity()),weight);
                SumZSecondJetRapidity_Zinc2jet->Fill(fabs(EWKBoson.Rapidity()+jets[1].v.Rapidity())/2.0,weight);
                DifZSecondJetRapidity_Zinc2jet->Fill(fabs(EWKBoson.Rapidity()-jets[1].v.Rapidity())/2.0,weight);

                SumFirstSecondJetRapidity_Zinc2jet->Fill(fabs(jets[0].v.Rapidity()+jets[1].v.Rapidity())/2.0,weight);
                DifFirstSecondJetRapidity_Zinc2jet->Fill(fabs(jets[0].v.Rapidity()-jets[1].v.Rapidity())/2.0,weight);

                TLorentzVector DiJets = jets[0].v + jets[1].v;
                SumZTwoJetsRapidity_Zinc2jet->Fill(fabs(EWKBoson.Rapidity()+DiJets.Rapidity())/2.0,weight);
                DifZTwoJetsRapidity_Zinc2jet->Fill(fabs(EWKBoson.Rapidity()-DiJets.Rapidity())/2.0,weight);

                ///Azimuth cross check
                DPhiZFirstJet_Zinc2jet->Fill(fabs(EWKBoson.DeltaPhi(jets[0].v)),weight);
                DPhiZSecondJet_Zinc2jet->Fill(fabs(EWKBoson.DeltaPhi(jets[1].v)),weight);
                DPhiFirstSecondJet_Zinc2jet->Fill(fabs(jets[0].v.DeltaPhi(jets[1].v)),weight);


                if(EWKBoson.Pt()>100.)
                {
                    AbsZRapidity_ZPt100_Zinc2jet->Fill(fabs(EWKBoson.Rapidity()),weight);
                    AbsSecondJetRapidity_ZPt100_Zinc2jet->Fill(fabs(jets[1].v.Rapidity()),weight);
                    SumZSecondJetRapidity_ZPt100_Zinc2jet->Fill(fabs(EWKBoson.Rapidity()+jets[1].v.Rapidity())/2.0,weight);
                    DifZSecondJetRapidity_ZPt100_Zinc2jet->Fill(fabs(EWKBoson.Rapidity()-jets[1].v.Rapidity())/2.0,weight);
                }

                if(EWKBoson.Pt()>150.)
                {
                    AbsZRapidity_ZPt150_Zinc2jet->Fill(fabs(EWKBoson.Rapidity()),weight);
                    AbsSecondJetRapidity_ZPt150_Zinc2jet->Fill(fabs(jets[1].v.Rapidity()),weight);
                    SumZSecondJetRapidity_ZPt150_Zinc2jet->Fill(fabs(EWKBoson.Rapidity()+jets[1].v.Rapidity())/2.0,weight);
                    DifZSecondJetRapidity_ZPt150_Zinc2jet->Fill(fabs(EWKBoson.Rapidity()-jets[1].v.Rapidity())/2.0,weight);

                    DPhiZFirstJet_ZPt150_Zinc2jet->Fill(fabs(EWKBoson.DeltaPhi(jets[0].v)),weight);

                }

                if(EWKBoson.Pt()>300.)
                {
                    DPhiZFirstJet_ZPt300_Zinc2jet->Fill(fabs(EWKBoson.DeltaPhi(jets[0].v)),weight);
                }

                //set jet rapidity discriminator/////

                if(fabs(jets[0].v.Rapidity()-jets[1].v.Rapidity())>2)
                {
                    AbsZRapidity_DifJetRapidityl2_Zinc2jet->Fill(fabs(EWKBoson.Rapidity()),weight);
                    AbsFirstJetRapidity_DifJetRapidityl2_Zinc2jet->Fill(fabs(jets[0].v.Rapidity()),weight);
                    SumZFirstJetRapidity_DifJetRapidityl2_Zinc2jet->Fill(fabs(EWKBoson.Rapidity()+jets[0].v.Rapidity())/2.0,weight);
                    DifZFirstJetRapidity_DifJetRapidityl2_Zinc2jet->Fill(fabs(EWKBoson.Rapidity()-jets[0].v.Rapidity())/2.0,weight);
                }

                if(fabs(jets[0].v.Rapidity()-jets[1].v.Rapidity())<2)
                {
                    AbsZRapidity_DifJetRapiditys2_Zinc2jet->Fill(fabs(EWKBoson.Rapidity()),weight);
                    AbsFirstJetRapidity_DifJetRapiditys2_Zinc2jet->Fill(fabs(jets[0].v.Rapidity()),weight);
                    SumZFirstJetRapidity_DifJetRapiditys2_Zinc2jet->Fill(fabs(EWKBoson.Rapidity()+jets[0].v.Rapidity())/2.0,weight);
                    DifZFirstJetRapidity_DifJetRapiditys2_Zinc2jet->Fill(fabs(EWKBoson.Rapidity()-jets[0].v.Rapidity())/2.0,weight);
                }


                TLorentzVector jet1Plus2PlusZ = jet1Plus2 + EWKBoson;
                ZNGoodJets_Zinc->Fill(2., weight);
                ZNGoodJets_Zinc_NoWeight->Fill(2.);
                TwoJetsPtDiff_Zinc2jet->Fill(jet1Minus2.Pt(), weight);
                BestTwoJetsPtDiff_Zinc2jet->Fill(bestJet1Minus2.Pt(), weight);
                JetsMass_Zinc2jet->Fill(jet1Plus2.M(), weight);
                llJetsMass_Zinc2jet->Fill(jet1Plus2PlusZ.M(), genWeight);

                if (jet1Plus2PlusZ.M() > 450 && jet1Plus2PlusZ.M() < 600) {
                    if (fabs(jets[0].v.Eta()) < fabs(jets[1].v.Eta())) {
                        CentralJetEta_Zinc2jet->Fill(fabs(jets[0].v.Eta()), weight);
                        ForwardJetEta_Zinc2jet->Fill(fabs(jets[1].v.Eta()), weight);
                        CentralJetPt_Zinc2jet->Fill(jets[0].v.Pt(), weight);
                        ForwardJetPt_Zinc2jet->Fill(jets[1].v.Pt(), weight);
                    }
                    else {
                        CentralJetEta_Zinc2jet->Fill(fabs(jets[1].v.Eta()), weight);
                        ForwardJetEta_Zinc2jet->Fill(fabs(jets[0].v.Eta()), weight);
                        CentralJetPt_Zinc2jet->Fill(jets[1].v.Pt(), weight);
                        ForwardJetPt_Zinc2jet->Fill(jets[0].v.Pt(), weight);
                    }
                }

                if (EvtVtxCnt < 14) JetsMassLowPU_Zinc2jet->Fill(jet1Plus2.M(), weight);
                else if (EvtVtxCnt < 18) JetsMassMidPU_Zinc2jet->Fill(jet1Plus2.M(), weight);
                else JetsMassHigPU_Zinc2jet->Fill(jet1Plus2.M(), weight);
                ZPt_Zinc2jet->Fill(EWKBoson.Pt(), weight);
                ZRapidity_Zinc2jet->Fill(EWKBoson.Rapidity(), weight);
                ZEta_Zinc2jet->Fill(EWKBoson.Eta(), weight);
                SpTLeptons_Zinc2jet->Fill(SpTsub(leptons[0].v, leptons[1].v), weight);

		double RatioValue = 1.;
		double RatioValue1 = 1.;
		if(UnfoldUnc){
		    double binNumber = SecondJetEta_2_Zinc2jet_ratio->GetXaxis()->FindBin(fabs(jets[1].v.Eta()));
		    RatioValue = SecondJetEta_2_Zinc2jet_ratio->GetBinContent(binNumber);
		    double binNumber1 = JetsHT_2_Zinc2jet_ratio->GetXaxis()->FindBin(jetsHT);
		    RatioValue1 = JetsHT_2_Zinc2jet_ratio->GetBinContent(binNumber1);
		}

                SecondJetEta_Zinc2jet->Fill(fabs(jets[1].v.Eta()), weight*RatioValue);
                SecondJetEta_2_Zinc2jet->Fill(fabs(jets[1].v.Eta()), weight);
                SecondJetAbsRapidity_Zinc2jet->Fill(fabs(jets[1].v.Rapidity()), weight*RatioValue);
		if(nEvents % 2) SecondJetAbsRapidity_Zinc2jet_Odd->Fill(fabs(jets[1].v.Rapidity()), weight*RatioValue);
		else SecondJetAbsRapidity_Zinc2jet_Even->Fill(fabs(jets[1].v.Rapidity()), weight*RatioValue);
                SecondJetEtaHigh_Zinc2jet->Fill(fabs(jets[1].v.Eta()), weight);
                SecondJetRapidityHigh_Zinc2jet->Fill(fabs(jets[1].v.Rapidity()), weight);
                SecondJetEtaFull_Zinc2jet->Fill(jets[1].v.Eta(), weight);
                SecondJetPhi_Zinc2jet->Fill(jets[1].v.Phi(), weight);        
                JetsHT_Zinc2jet->Fill(jetsHT, weight*RatioValue1);
                JetsHT_2_Zinc2jet->Fill(jetsHT, weight);
                ptBal_Zinc2jet->Fill(jet1Plus2PlusZ.Pt(), weight);
                dPhiJets_Zinc2jet->Fill(deltaPhi(jets[0].v, jets[1].v), weight);
                BestdPhiJets_Zinc2jet->Fill(deltaPhi(bestTwoJets.first, bestTwoJets.second), weight);
                dEtaJets_Zinc2jet->Fill(jets[0].v.Eta() - jets[1].v.Eta(), weight);
                dEtaFirstJetZ_Zinc2jet->Fill(jets[0].v.Eta() - EWKBoson.Eta(), weight);
                dEtaSecondJetZ_Zinc2jet->Fill(jets[1].v.Eta() - EWKBoson.Eta(), weight);
                dEtaJet1Plus2Z_Zinc2jet->Fill(jet1Plus2.Eta() - EWKBoson.Eta(), weight);
                PHI_Zinc2jet->Fill(PHI(leptons[0].v, leptons[1].v, jets[0].v, jets[1].v), weight);
                BestPHI_Zinc2jet->Fill(PHI(leptons[0].v, leptons[1].v, bestTwoJets.first, bestTwoJets.second), weight);
                PHI_T_Zinc2jet->Fill(PHI_T(leptons[0].v, leptons[1].v, jets[0].v, jets[1].v), weight);
                BestPHI_T_Zinc2jet->Fill(PHI_T(leptons[0].v, leptons[1].v, bestTwoJets.first, bestTwoJets.second), weight);
                SpT_Zinc2jet->Fill(SpT(leptons[0].v, leptons[1].v, jets[0].v, jets[1].v), weight);
                BestSpT_Zinc2jet->Fill(SpT(leptons[0].v, leptons[1].v, bestTwoJets.first, bestTwoJets.second), weight);
                SpTJets_Zinc2jet->Fill(SpTsub(jets[0].v, jets[1].v), weight);
                BestSpTJets_Zinc2jet->Fill(SpTsub(bestTwoJets.first, bestTwoJets.second), weight);
                SPhi_Zinc2jet->Fill(SPhi(leptons[0].v, leptons[1].v, jets[0].v, jets[1].v), weight);
                BestSPhi_Zinc2jet->Fill(SPhi(leptons[0].v, leptons[1].v, bestTwoJets.first, bestTwoJets.second), weight);

                if (EWKBoson.Pt() < 25){
                    ptBal_LowPt_Zinc2jet->Fill(jet1Plus2PlusZ.Pt(), weight);
                    dPhiJets_LowPt_Zinc2jet->Fill(deltaPhi(jets[0].v, jets[1].v), weight);
                    BestdPhiJets_LowPt_Zinc2jet->Fill(deltaPhi(bestTwoJets.first, bestTwoJets.second), weight);
                    dPhiLeptons_LowPt_Zinc2jet->Fill(deltaPhi(leptons[0].v, leptons[1].v), weight);
                    PHI_LowPt_Zinc2jet->Fill(PHI(leptons[0].v, leptons[1].v, jets[0].v, jets[1].v), weight);
                    BestPHI_LowPt_Zinc2jet->Fill(PHI(leptons[0].v, leptons[1].v, bestTwoJets.first, bestTwoJets.second), weight);
                    PHI_T_LowPt_Zinc2jet->Fill(PHI_T(leptons[0].v, leptons[1].v, jets[0].v, jets[1].v), weight);
                    BestPHI_T_LowPt_Zinc2jet->Fill(PHI_T(leptons[0].v, leptons[1].v, bestTwoJets.first, bestTwoJets.second), weight);
                    SpT_LowPt_Zinc2jet->Fill(SpT(leptons[0].v, leptons[1].v, jets[0].v, jets[1].v), weight);
                    BestSpT_LowPt_Zinc2jet->Fill(SpT(leptons[0].v, leptons[1].v, bestTwoJets.first, bestTwoJets.second), weight);
                    SpTJets_LowPt_Zinc2jet->Fill(SpTsub(jets[0].v, jets[1].v), weight);
                    BestSpTJets_LowPt_Zinc2jet->Fill(SpTsub(bestTwoJets.first, bestTwoJets.second), weight);
                    SpTLeptons_LowPt_Zinc2jet->Fill(SpTsub(leptons[0].v, leptons[1].v), weight);
                    SPhi_LowPt_Zinc2jet->Fill(SPhi(leptons[0].v, leptons[1].v, jets[0].v, jets[1].v), weight);
                    BestSPhi_LowPt_Zinc2jet->Fill(SPhi(leptons[0].v, leptons[1].v, bestTwoJets.first, bestTwoJets.second), weight);
                    if (SpT(leptons[0].v, leptons[1].v, jets[0].v, jets[1].v) < 0.5){ 
                        PHI_LowSpT_LowPt_Zinc2jet->Fill(PHI(leptons[0].v, leptons[1].v, jets[0].v, jets[1].v), weight);
                        SPhi_LowSpT_LowPt_Zinc2jet->Fill(SPhi(leptons[0].v, leptons[1].v, jets[0].v, jets[1].v), weight);
                    }
                    else {
                        PHI_HighSpT_LowPt_Zinc2jet->Fill(PHI(leptons[0].v, leptons[1].v, jets[0].v, jets[1].v), weight);
                        SPhi_HighSpT_LowPt_Zinc2jet->Fill(SPhi(leptons[0].v, leptons[1].v, jets[0].v, jets[1].v), weight);
                    }
                    if (SPhi(leptons[0].v, leptons[1].v, jets[0].v, jets[1].v) < 0.5){
                        SpT_LowSPhi_LowPt_Zinc2jet->Fill(SpT(leptons[0].v, leptons[1].v, jets[0].v, jets[1].v), weight);
                    }
                    else {
                        SpT_HighSPhi_LowPt_Zinc2jet->Fill(SpT(leptons[0].v, leptons[1].v, jets[0].v, jets[1].v), weight);
                    }
                }
                else {
                    ptBal_HighPt_Zinc2jet->Fill(jet1Plus2PlusZ.Pt(),weight);
                    dPhiJets_HighPt_Zinc2jet->Fill(deltaPhi(jets[0].v, jets[1].v), weight);
                    dPhiLeptons_HighPt_Zinc2jet->Fill(deltaPhi(leptons[0].v, leptons[1].v), weight);
                    PHI_HighPt_Zinc2jet->Fill(PHI(leptons[0].v, leptons[1].v, jets[0].v, jets[1].v), weight);
                    PHI_T_HighPt_Zinc2jet->Fill(PHI_T(leptons[0].v, leptons[1].v, jets[0].v, jets[1].v), weight);
                    SpT_HighPt_Zinc2jet->Fill(SpT(leptons[0].v, leptons[1].v, jets[0].v, jets[1].v), weight);
                    SpTJets_HighPt_Zinc2jet->Fill(SpTsub(jets[0].v, jets[1].v), weight);
                    SpTLeptons_HighPt_Zinc2jet->Fill(SpTsub(leptons[0].v, leptons[1].v), weight);
                    SPhi_HighPt_Zinc2jet->Fill(SPhi(leptons[0].v, leptons[1].v, jets[0].v, jets[1].v), weight);
                    if (SpT(leptons[0].v, leptons[1].v, jets[0].v, jets[1].v) < 0.5){
                        PHI_LowSpT_HighPt_Zinc2jet->Fill(PHI(leptons[0].v, leptons[1].v, jets[0].v, jets[1].v), weight);
                        SPhi_LowSpT_HighPt_Zinc2jet->Fill(SPhi(leptons[0].v, leptons[1].v, jets[0].v, jets[1].v), weight); 
                    }
                    else {
                        PHI_HighSpT_HighPt_Zinc2jet->Fill(PHI(leptons[0].v, leptons[1].v, jets[0].v, jets[1].v), weight); 
                        SPhi_HighSpT_HighPt_Zinc2jet->Fill(SPhi(leptons[0].v, leptons[1].v, jets[0].v, jets[1].v), weight);
                    }
                    if (SPhi(leptons[0].v, leptons[1].v, jets[0].v, jets[1].v) < 0.5){
                        SpT_LowSPhi_HighPt_Zinc2jet->Fill(SpT(leptons[0].v, leptons[1].v, jets[0].v, jets[1].v), weight);
                    }
                    else {
                        SpT_HighSPhi_HighPt_Zinc2jet->Fill(SpT(leptons[0].v, leptons[1].v, jets[0].v, jets[1].v), weight);
                    }
                }
                if (SPhi(leptons[0].v, leptons[1].v, jets[0].v, jets[1].v) < 0.5){
                    SpT_LowSPhi_Zinc2jet->Fill(SpT(leptons[0].v, leptons[1].v, jets[0].v, jets[1].v), weight);
                }
                else {
                    SpT_HighSPhi_Zinc2jet->Fill(SpT(leptons[0].v, leptons[1].v, jets[0].v, jets[1].v), weight);
                }
                if (SpT(leptons[0].v, leptons[1].v, jets[0].v, jets[1].v) < 0.5){
                    PHI_LowSpT_Zinc2jet->Fill(PHI(leptons[0].v, leptons[1].v, jets[0].v, jets[1].v), weight);
                    SPhi_LowSpT_Zinc2jet->Fill(SPhi(leptons[0].v, leptons[1].v, jets[0].v, jets[1].v), weight);
                }
                else {
                    PHI_HighSpT_Zinc2jet->Fill(PHI(leptons[0].v, leptons[1].v, jets[0].v, jets[1].v), weight);
                    SPhi_HighSpT_Zinc2jet->Fill(SPhi(leptons[0].v, leptons[1].v, jets[0].v, jets[1].v), weight);
                }
                if (nGoodJets == 2){
                    ////////////Special Branch/////////////////////////////////
                    AbsZRapidity_Zexc2jet->Fill(fabs(EWKBoson.Rapidity()),weight);
                    AbsSecondJetRapidity_Zexc2jet->Fill(fabs(jets[1].v.Rapidity()),weight);
                    SumZSecondJetRapidity_Zexc2jet->Fill(fabs(EWKBoson.Rapidity()+jets[1].v.Rapidity())/2.0,weight);
                    DifZSecondJetRapidity_Zexc2jet->Fill(fabs(EWKBoson.Rapidity()-jets[1].v.Rapidity())/2.0,weight);
                    ZNGoodJetsNVtx_Zexc->Fill(2., EvtVtxCnt, weight);

                    if(EWKBoson.Pt()>100.)
                    {
                        AbsZRapidity_ZPt100_Zexc2jet->Fill(fabs(EWKBoson.Rapidity()),weight);
                        AbsSecondJetRapidity_ZPt100_Zexc2jet->Fill(fabs(jets[1].v.Rapidity()),weight);
                        SumZSecondJetRapidity_ZPt100_Zexc2jet->Fill(fabs(EWKBoson.Rapidity()+jets[1].v.Rapidity())/2.0,weight);
                        DifZSecondJetRapidity_ZPt100_Zexc2jet->Fill(fabs(EWKBoson.Rapidity()-jets[1].v.Rapidity())/2.0,weight);
                    }

                    if(EWKBoson.Pt()>150.)
                    {
                        AbsZRapidity_ZPt150_Zexc2jet->Fill(fabs(EWKBoson.Rapidity()),weight);
                        AbsSecondJetRapidity_ZPt150_Zexc2jet->Fill(fabs(jets[1].v.Rapidity()),weight);
                        SumZSecondJetRapidity_ZPt150_Zexc2jet->Fill(fabs(EWKBoson.Rapidity()+jets[1].v.Rapidity())/2.0,weight);
                        DifZSecondJetRapidity_ZPt150_Zexc2jet->Fill(fabs(EWKBoson.Rapidity()-jets[1].v.Rapidity())/2.0,weight);
                    }

                    //TruePU_2->Fill(EvtPuCntTruth, weight);
                    //PU_2->Fill(EvtPuCnt, weight);              
                    PU_2->Fill(EvtVtxCnt, weight);
                    ZNGoodJets_Zexc_NoWeight->Fill(2.);
                    ZPt_Zexc2jet->Fill(EWKBoson.Pt(), weight);
                    ZRapidity_Zexc2jet->Fill(EWKBoson.Rapidity(), weight);
                    ZEta_Zexc2jet->Fill(EWKBoson.Eta(), weight);
                    SpTLeptons_Zexc2jet->Fill(SpTsub(leptons[0].v, leptons[1].v), weight);
                    SecondJetPt_Zexc2jet->Fill(jets[1].v.Pt(), weight);
                    SecondJetEta_Zexc2jet->Fill(jets[1].v.Eta(), weight);
                    SecondJetPhi_Zexc2jet->Fill(jets[1].v.Phi(), weight); 

                    //-- DPS Histograms
                    TwoJetsPtDiff_Zexc2jet->Fill(jet1Minus2.Pt(), weight);
                    JetsMass_Zexc2jet->Fill(jet1Plus2.M(), weight);
                    ptBal_Zexc2jet->Fill(jet1Plus2PlusZ.Pt(), weight);
                    dPhiJets_Zexc2jet->Fill(deltaPhi(jets[0].v, jets[1].v), weight);
                    dEtaJets_Zexc2jet->Fill(jets[0].v.Eta() - jets[1].v.Eta(), weight);
                    dEtaFirstJetZ_Zexc2jet->Fill(jets[0].v.Eta() - EWKBoson.Eta(), weight);
                    dEtaSecondJetZ_Zexc2jet->Fill(jets[1].v.Eta() - EWKBoson.Eta(), weight);
                    dEtaJet1Plus2Z_Zexc2jet->Fill(jet1Plus2.Eta() - EWKBoson.Eta(), weight);
                    PHI_Zexc2jet->Fill(PHI(leptons[0].v, leptons[1].v, jets[0].v, jets[1].v), weight);
                    PHI_T_Zexc2jet->Fill(PHI_T(leptons[0].v, leptons[1].v, jets[0].v, jets[1].v), weight);
                    SpT_Zexc2jet->Fill(SpT(leptons[0].v, leptons[1].v, jets[0].v, jets[1].v), weight);
                    SpTJets_Zexc2jet->Fill(SpTsub(jets[0].v, jets[1].v), weight);
                    SPhi_Zexc2jet->Fill(SPhi(leptons[0].v, leptons[1].v, jets[0].v, jets[1].v), weight);
                    if (EWKBoson.Pt() < 25){
                        ptBal_LowPt_Zexc2jet->Fill(jet1Plus2PlusZ.Pt(), weight);
                        dPhiJets_LowPt_Zexc2jet->Fill(deltaPhi(jets[0].v, jets[1].v), weight);
                        PHI_LowPt_Zexc2jet->Fill(PHI(leptons[0].v, leptons[1].v, jets[0].v, jets[1].v), weight);
                        PHI_T_LowPt_Zexc2jet->Fill(PHI_T(leptons[0].v, leptons[1].v, jets[0].v, jets[1].v), weight);
                        SpT_LowPt_Zexc2jet->Fill(SpT(leptons[0].v, leptons[1].v, jets[0].v, jets[1].v), weight);
                        SpTJets_LowPt_Zexc2jet->Fill(SpTsub(jets[0].v, jets[1].v), weight);
                        SpTLeptons_LowPt_Zexc2jet->Fill(SpTsub(leptons[0].v, leptons[1].v), weight);
                        SPhi_LowPt_Zexc2jet->Fill(SPhi(leptons[0].v, leptons[1].v, jets[0].v, jets[1].v), weight);
                        if (SpT(leptons[0].v, leptons[1].v, jets[0].v, jets[1].v) < 0.5){ 
                            PHI_LowSpT_LowPt_Zexc2jet->Fill(PHI(leptons[0].v, leptons[1].v, jets[0].v, jets[1].v), weight);
                            SPhi_LowSpT_LowPt_Zexc2jet->Fill(SPhi(leptons[0].v, leptons[1].v, jets[0].v, jets[1].v), weight);
                        }
                        else {
                            PHI_HighSpT_LowPt_Zexc2jet->Fill(PHI(leptons[0].v, leptons[1].v, jets[0].v, jets[1].v), weight);
                            SPhi_HighSpT_LowPt_Zexc2jet->Fill(SPhi(leptons[0].v, leptons[1].v, jets[0].v, jets[1].v), weight);
                        }
                        if (SPhi(leptons[0].v, leptons[1].v, jets[0].v, jets[1].v) < 0.5){
                            SpT_LowSPhi_LowPt_Zexc2jet->Fill(SpT(leptons[0].v, leptons[1].v, jets[0].v, jets[1].v), weight);
                        }
                        else {
                            SpT_HighSPhi_LowPt_Zexc2jet ->Fill(SpT(leptons[0].v, leptons[1].v, jets[0].v, jets[1].v), weight);
                        }
                    }
                    else {
                        ptBal_HighPt_Zexc2jet->Fill(jet1Plus2PlusZ.Pt(),weight);
                        dPhiJets_HighPt_Zexc2jet->Fill(deltaPhi(jets[0].v, jets[1].v), weight);
                        PHI_HighPt_Zexc2jet->Fill(PHI(leptons[0].v, leptons[1].v, jets[0].v, jets[1].v), weight);
                        PHI_T_HighPt_Zexc2jet->Fill(PHI_T(leptons[0].v, leptons[1].v, jets[0].v, jets[1].v), weight);
                        SpT_HighPt_Zexc2jet->Fill(SpT(leptons[0].v, leptons[1].v, jets[0].v, jets[1].v), weight);
                        SpTJets_HighPt_Zexc2jet->Fill(SpTsub(jets[0].v, jets[1].v), weight);
                        SpTLeptons_HighPt_Zexc2jet->Fill(SpTsub(leptons[0].v, leptons[1].v), weight);
                        SPhi_HighPt_Zexc2jet->Fill(SPhi(leptons[0].v, leptons[1].v, jets[0].v, jets[1].v), weight);
                        if (SpT(leptons[0].v, leptons[1].v, jets[0].v, jets[1].v) < 0.5){
                            PHI_LowSpT_HighPt_Zexc2jet->Fill(PHI(leptons[0].v, leptons[1].v, jets[0].v, jets[1].v), weight);
                            SPhi_LowSpT_HighPt_Zexc2jet->Fill(SPhi(leptons[0].v, leptons[1].v, jets[0].v, jets[1].v), weight); 
                        }
                        else {
                            PHI_HighSpT_HighPt_Zexc2jet->Fill(PHI(leptons[0].v, leptons[1].v, jets[0].v, jets[1].v), weight); 
                            SPhi_HighSpT_HighPt_Zexc2jet->Fill(SPhi(leptons[0].v, leptons[1].v, jets[0].v, jets[1].v), weight);
                        }
                        if (SPhi(leptons[0].v, leptons[1].v, jets[0].v, jets[1].v) < 0.5){
                            SpT_LowSPhi_HighPt_Zexc2jet->Fill(SpT(leptons[0].v, leptons[1].v, jets[0].v, jets[1].v), weight);
                        }
                        else {
                            SpT_HighSPhi_HighPt_Zexc2jet->Fill(SpT(leptons[0].v, leptons[1].v, jets[0].v, jets[1].v), weight);
                        }
                    }
                    if (SPhi(leptons[0].v, leptons[1].v, jets[0].v, jets[1].v) < 0.5){
                        SpT_LowSPhi_Zexc2jet->Fill(SpT(leptons[0].v, leptons[1].v, jets[0].v, jets[1].v), weight);
                    }
                    else {
                        SpT_HighSPhi_Zexc2jet->Fill(SpT(leptons[0].v, leptons[1].v, jets[0].v, jets[1].v), weight);
                    }
                    if (SpT(leptons[0].v, leptons[1].v, jets[0].v, jets[1].v) < 0.5){
                        PHI_LowSpT_Zexc2jet->Fill(PHI(leptons[0].v, leptons[1].v, jets[0].v, jets[1].v), weight);
                        SPhi_LowSpT_Zexc2jet->Fill(SPhi(leptons[0].v, leptons[1].v, jets[0].v, jets[1].v), weight);
                    }
                    else {
                        PHI_HighSpT_Zexc2jet->Fill(PHI(leptons[0].v, leptons[1].v, jets[0].v, jets[1].v), weight);
                        SPhi_HighSpT_Zexc2jet->Fill(SPhi(leptons[0].v, leptons[1].v, jets[0].v, jets[1].v), weight);
                    }
                }
            }
            if (nGoodJets_20 >= 3) {
		double RatioValue = 1.;
		if(UnfoldUnc){
		    double binNumber = ThirdJetPt_2_Zinc3jet_ratio->GetXaxis()->FindBin(jets_20[2].v.Pt());
		    RatioValue =  ThirdJetPt_2_Zinc3jet_ratio->GetBinContent(binNumber);
		}

                ThirdJetPt_Zinc3jet->Fill(jets_20[2].v.Pt(), weight*RatioValue);
                if(nEvents%2) ThirdJetPt_Zinc3jet_Odd->Fill(jets_20[2].v.Pt(), weight*RatioValue);
                else ThirdJetPt_Zinc3jet_Even->Fill(jets_20[2].v.Pt(), weight*RatioValue);
                ThirdJetPt_2_Zinc3jet->Fill(jets_20[2].v.Pt(), weight);
            }
            if (nGoodJets >= 3) {
                nEventsVInc3Jets++;
                nEffEventsVInc3Jets += weight;
                ZNGoodJets_Zinc->Fill(3., weight);
                ZNGoodJets_Zinc_NoWeight->Fill(3.);

		double RatioValue = 1.;
		double RatioValue1 = 1.;
		if(UnfoldUnc){
		    double binNumber = ThirdJetEta_2_Zinc3jet_ratio->GetXaxis()->FindBin(fabs(jets[2].v.Eta()));
		    RatioValue =  ThirdJetEta_2_Zinc3jet_ratio->GetBinContent(binNumber);
		    double binNumber1 = JetsHT_2_Zinc3jet_ratio->GetXaxis()->FindBin(jetsHT);
		    RatioValue1 = JetsHT_2_Zinc3jet_ratio->GetBinContent(binNumber1);
		}

                ThirdJetEta_Zinc3jet->Fill(fabs(jets[2].v.Eta()), weight*RatioValue);
                ThirdJetEta_2_Zinc3jet->Fill(fabs(jets[2].v.Eta()), weight);
                ThirdJetAbsRapidity_Zinc3jet->Fill(fabs(jets[2].v.Rapidity()), weight*RatioValue);
                if(nEvents% 2) ThirdJetAbsRapidity_Zinc3jet_Odd->Fill(fabs(jets[2].v.Rapidity()), weight*RatioValue);
                else ThirdJetAbsRapidity_Zinc3jet_Even->Fill(fabs(jets[2].v.Rapidity()), weight*RatioValue);
                ThirdJetEtaHigh_Zinc3jet->Fill(fabs(jets[2].v.Eta()), weight);
                ThirdJetRapidityHigh_Zinc3jet->Fill(fabs(jets[2].v.Rapidity()), weight);
                ThirdJetEtaFull_Zinc3jet->Fill(jets[2].v.Eta(), weight);
                ThirdJetPhi_Zinc3jet->Fill(jets[2].v.Phi(), weight);        
                JetsHT_Zinc3jet->Fill(jetsHT, weight*RatioValue1);
                JetsHT_2_Zinc3jet->Fill(jetsHT, weight);

                ///Azimuth cross check
                DPhiZFirstJet_Zinc3jet->Fill(fabs(EWKBoson.DeltaPhi(jets[0].v)),weight);
                DPhiZSecondJet_Zinc3jet->Fill(fabs(EWKBoson.DeltaPhi(jets[1].v)),weight);
                DPhiZThirdJet_Zinc3jet->Fill(fabs(EWKBoson.DeltaPhi(jets[2].v)),weight);
                DPhiFirstSecondJet_Zinc3jet->Fill(fabs(jets[0].v.DeltaPhi(jets[1].v)),weight);
                DPhiFirstThirdJet_Zinc3jet->Fill(fabs(jets[0].v.DeltaPhi(jets[2].v)),weight);
                DPhiSecondThirdJet_Zinc3jet->Fill(fabs(jets[1].v.DeltaPhi(jets[2].v)),weight);

                if(EWKBoson.Pt()>150.)
                {
                    DPhiZFirstJet_ZPt150_Zinc3jet->Fill(fabs(EWKBoson.DeltaPhi(jets[0].v)),weight);
                    DPhiZSecondJet_ZPt150_Zinc3jet->Fill(fabs(EWKBoson.DeltaPhi(jets[1].v)),weight);
                    DPhiZThirdJet_ZPt150_Zinc3jet->Fill(fabs(EWKBoson.DeltaPhi(jets[2].v)),weight);
                    DPhiFirstSecondJet_ZPt150_Zinc3jet->Fill(fabs(jets[0].v.DeltaPhi(jets[1].v)),weight);
                    DPhiFirstThirdJet_ZPt150_Zinc3jet->Fill(fabs(jets[0].v.DeltaPhi(jets[2].v)),weight);
                    DPhiSecondThirdJet_ZPt150_Zinc3jet->Fill(fabs(jets[1].v.DeltaPhi(jets[2].v)),weight);
                }

                if(EWKBoson.Pt()>300.)
                {
                    DPhiZFirstJet_ZPt300_Zinc3jet->Fill(fabs(EWKBoson.DeltaPhi(jets[0].v)),weight);
                    DPhiZSecondJet_ZPt300_Zinc3jet->Fill(fabs(EWKBoson.DeltaPhi(jets[1].v)),weight);
                    DPhiZThirdJet_ZPt300_Zinc3jet->Fill(fabs(EWKBoson.DeltaPhi(jets[2].v)),weight);
                    DPhiFirstSecondJet_ZPt300_Zinc3jet->Fill(fabs(jets[0].v.DeltaPhi(jets[1].v)),weight);
                    DPhiFirstThirdJet_ZPt300_Zinc3jet->Fill(fabs(jets[0].v.DeltaPhi(jets[2].v)),weight);
                    DPhiSecondThirdJet_ZPt300_Zinc3jet->Fill(fabs(jets[1].v.DeltaPhi(jets[2].v)),weight);
                }

                if(EWKBoson.Pt()>150. && (jets[0].v.Pt()+jets[1].v.Pt()+jets[2].v.Pt()>300.))
                {
                    DPhiZFirstJet_ZPt150_HT300_Zinc3jet->Fill(fabs(EWKBoson.DeltaPhi(jets[0].v)),weight);
                    DPhiZSecondJet_ZPt150_HT300_Zinc3jet->Fill(fabs(EWKBoson.DeltaPhi(jets[1].v)),weight);
                    DPhiZThirdJet_ZPt150_HT300_Zinc3jet->Fill(fabs(EWKBoson.DeltaPhi(jets[2].v)),weight);
                }


                if (nGoodJets == 3){
                    //TruePU_3->Fill(EvtPuCntTruth, weight);
                    //PU_3->Fill(EvtPuCnt, weight);
                    PU_3->Fill(EvtVtxCnt, weight);
                    ZNGoodJets_Zexc_NoWeight->Fill(3.);
                }
            }
            if (nGoodJets_20 >= 4) FourthJetPt_Zinc4jet->Fill(jets_20[3].v.Pt(), weight);
            if (nGoodJets >= 4){
                ZNGoodJets_Zinc->Fill(4., weight);
                ZNGoodJets_Zinc_NoWeight->Fill(4.);
                FourthJetEta_Zinc4jet->Fill(fabs(jets[3].v.Eta()), weight);
                FourthJetAbsRapidity_Zinc4jet->Fill(fabs(jets[3].v.Rapidity()), weight);
                FourthJetEtaHigh_Zinc4jet->Fill(fabs(jets[3].v.Eta()), weight);
                FourthJetRapidityHigh_Zinc4jet->Fill(fabs(jets[3].v.Rapidity()), weight);
                FourthJetEtaFull_Zinc4jet->Fill(jets[3].v.Eta(), weight);
                FourthJetPhi_Zinc4jet->Fill(jets[3].v.Phi(), weight);        
                JetsHT_Zinc4jet->Fill(jetsHT, weight);
                if (nGoodJets == 4){
                    //TruePU_4->Fill(EvtPuCntTruth, weight);
                    //PU_4->Fill(EvtPuCnt, weight);
                    PU_4->Fill(EvtVtxCnt, weight);
                    ZNGoodJets_Zexc_NoWeight->Fill(4.);
                }
            }    
            if (nGoodJets_20 >= 5) FifthJetPt_Zinc5jet->Fill(jets_20[4].v.Pt(), weight);
            if (nGoodJets >= 5){
                ZNGoodJets_Zinc->Fill(5., weight);
                ZNGoodJets_Zinc_NoWeight->Fill(5.);
                FifthJetEta_Zinc5jet->Fill(fabs(jets[4].v.Eta()), weight);
                FifthJetAbsRapidity_Zinc5jet->Fill(fabs(jets[4].v.Rapidity()), weight);
                FifthJetEtaHigh_Zinc5jet->Fill(fabs(jets[4].v.Eta()), weight);
                FifthJetRapidityHigh_Zinc5jet->Fill(fabs(jets[4].v.Rapidity()), weight);
                FifthJetEtaFull_Zinc5jet->Fill(jets[4].v.Eta(), weight);
                FifthJetPhi_Zinc5jet->Fill(jets[4].v.Phi(), weight);        
                JetsHT_Zinc5jet->Fill(jetsHT, weight);
                if (nGoodJets == 5){
                    //TruePU_5->Fill(EvtPuCntTruth, weight);
                    //PU_5->Fill(EvtPuCnt, weight);
                    PU_5->Fill(EvtVtxCnt, weight);
                    ZNGoodJets_Zexc_NoWeight->Fill(5.);
                }
            }    
            if (nGoodJets_20 >= 6) SixthJetPt_Zinc6jet->Fill(jets_20[5].v.Pt(), weight);
            if (nGoodJets >= 6){
                ZNGoodJets_Zinc->Fill(6., weight);
                ZNGoodJets_Zinc_NoWeight->Fill(6.);
                SixthJetEta_Zinc6jet->Fill(fabs(jets[5].v.Eta()), weight);
                SixthJetEtaHigh_Zinc6jet->Fill(fabs(jets[5].v.Eta()), weight);
                SixthJetRapidityHigh_Zinc6jet->Fill(fabs(jets[5].v.Rapidity()), weight);
                SixthJetEtaFull_Zinc6jet->Fill(jets[5].v.Eta(), weight);
                SixthJetPhi_Zinc6jet->Fill(jets[5].v.Phi(), weight);        
                JetsHT_Zinc6jet->Fill(jetsHT, weight);
                if (nGoodJets == 6){
                    //TruePU_6->Fill(EvtPuCntTruth, weight);
                    //PU_6->Fill(EvtPuCnt, weight);
                    PU_6->Fill(EvtVtxCnt, weight);
                    ZNGoodJets_Zexc_NoWeight->Fill(6.);
                }
            }
            if (nGoodJets >= 7){
                ZNGoodJets_Zinc->Fill(7., weight);
                if (nGoodJets == 7 ){
                    //TruePU_7->Fill(EvtPuCntTruth, weight);
                    //PU_7->Fill(EvtPuCnt, weight);
                    PU_7->Fill(EvtVtxCnt, weight);
                }
            }
            if (nGoodJets >= 8){
                ZNGoodJets_Zinc->Fill(8., weight);
            }
	    
            //=======================================================================================================//
        }

        //=======================================================================================================//
        //             Unfolding               //
        //====================================//

	if (hasRecoInfo && hasGenInfo && (!bTagJetFound || !rejectBTagEvents)) {
            if (nGoodJets_20 >= 1 && nGoodGenJets_20 >= 1) {
                //looks for matching gen jet:
                int igen = 0;
                double dr2 = 999.;
                for(int i = 0; i < nGoodGenJets_20; ++i){
                    double dr2_ = std::pow(jets_20[0].v.Eta() - genJets_20[i].v.Eta(), 2)
                        + std::pow(jets_20[0].v.Pt() - genJets_20[i].v.Pt(), 2);
                    if( dr2_ < dr2) { dr2 = dr2_; igen = i;}
                }
                FirstJetPtRecoOvGen_Zinc1jet_NVtx->Fill(jets_20[0].v.Pt()/genJets_20[igen].v.Pt(), EvtVtxCnt, weight);
            }

            //-- EWKBoson Mass and jet multiplicity
            if (passesgenLeptonCut && passesLeptonCut) {

		double RatioValue = 1.;
		if(UnfoldUnc){
		    double binNumber = ZNGoodJets_Zexc_ratio->GetXaxis()->FindBin(nGoodJets);
		    RatioValue =  ZNGoodJets_Zexc_ratio->GetBinContent(binNumber);
		}

                hresponseZNGoodJets_Zexc->Fill(nGoodJets, nGoodGenJets, weight*RatioValue);
                hresponseZPt_Zinc0jet->Fill(EWKBoson.Pt(), genEWKBoson.Pt(), weight);
            }

            //-- First Jet Pt 
            if (nGoodGenJets >= 1 && passesgenLeptonCut && nGoodJets >= 1 && passesLeptonCut) {

                hresponseZPt_Zinc1jet->Fill(EWKBoson.Pt(),genEWKBoson.Pt(),weight);
                hresponseZAbsRapidity_Zinc1jet->Fill(fabs(EWKBoson.Rapidity()), fabs(genEWKBoson.Rapidity()), weight);      

		double RatioValue = 1.;
		double RatioValue1 = 1.;
		if(UnfoldUnc){
		    double binNumber = FirstJetEta_2_Zinc1jet_ratio->GetXaxis()->FindBin(fabs(jets[0].v.Eta()));
		    RatioValue =  FirstJetEta_2_Zinc1jet_ratio->GetBinContent(binNumber);
		    double binNumber1 = JetsHT_2_Zinc1jet_ratio->GetXaxis()->FindBin(jetsHT);
		    RatioValue1 = JetsHT_2_Zinc1jet_ratio->GetBinContent(binNumber1);
		}

                hresponseFirstJetEta_Zinc1jet->Fill(fabs(jets[0].v.Eta()), fabs(genJets[0].v.Eta()), weight*RatioValue);        
                hresponseFirstJetAbsRapidity_Zinc1jet->Fill(fabs(jets[0].v.Rapidity()), fabs(genJets[0].v.Rapidity()), weight*RatioValue);      
                hresponseFirstJetEtaHigh_Zinc1jet->Fill(fabs(jets[0].v.Eta()), fabs(genJets[0].v.Eta()), weight);      
                hresponseFirstJetRapidityHigh_Zinc1jet->Fill(fabs(jets[0].v.Rapidity()), fabs(genJets[0].v.Rapidity()), weight);      
                hresponseJetsHT_Zinc1jet->Fill(jetsHT, genJetsHT, weight*RatioValue1);

                // Additional Abs responses of variables
                hresponseAbsZRapidity_Zinc1jet->Fill(fabs(EWKBoson.Rapidity()),fabs(genEWKBoson.Rapidity()),weight);
                hresponseAbsFirstJetRapidity_Zinc1jet->Fill(fabs(jets[0].v.Rapidity()),fabs(genJets[0].v.Rapidity()),weight);
                hresponseSumZFirstJetRapidity_Zinc1jet->Fill(fabs(EWKBoson.Rapidity()+jets[0].v.Rapidity())/2.0,fabs(genEWKBoson.Rapidity()+genJets[0].v.Rapidity())/2.0,weight);
                hresponseDifZFirstJetRapidity_Zinc1jet->Fill(fabs(EWKBoson.Rapidity()-jets[0].v.Rapidity())/2.0,fabs(genEWKBoson.Rapidity()-genJets[0].v.Rapidity())/2.0,weight);

                //cross check//////
                hresponseSumZFirstJetEta_Zinc1jet->Fill(fabs(EWKBoson.Eta()+jets[0].v.Eta())/2.0,fabs(genEWKBoson.Eta()+genJets[0].v.Eta())/2.0,weight);
                hresponseDifZFirstJetEta_Zinc1jet->Fill(fabs(EWKBoson.Eta()-jets[0].v.Eta())/2.0,fabs(genEWKBoson.Eta()-genJets[0].v.Eta())/2.0,weight);

                /////Azimuthal cross check////////////////////////////
                hresponseDPhiZFirstJet_Zinc1jet->Fill(fabs(EWKBoson.DeltaPhi(jets[0].v)),fabs(genEWKBoson.DeltaPhi(genJets[0].v)),weight);

                if(genEWKBoson.Pt()>100.&&EWKBoson.Pt()>100.)
                {
                    hresponseAbsZRapidity_ZPt100_Zinc1jet->Fill(fabs(EWKBoson.Rapidity()),fabs(genEWKBoson.Rapidity()),weight);
                    hresponseAbsFirstJetRapidity_ZPt100_Zinc1jet->Fill(fabs(jets[0].v.Rapidity()),fabs(genJets[0].v.Rapidity()),weight);
                    hresponseSumZFirstJetRapidity_ZPt100_Zinc1jet->Fill(fabs(EWKBoson.Rapidity()+jets[0].v.Rapidity())/2.0,fabs(genEWKBoson.Rapidity()+genJets[0].v.Rapidity())/2.0,weight);
                    hresponseDifZFirstJetRapidity_ZPt100_Zinc1jet->Fill(fabs(EWKBoson.Rapidity()-jets[0].v.Rapidity())/2.0,fabs(genEWKBoson.Rapidity()-genJets[0].v.Rapidity())/2.0,weight);
                }

                if(genEWKBoson.Pt()>150.&&EWKBoson.Pt()>150.)
                {
                    hresponseAbsZRapidity_ZPt150_Zinc1jet->Fill(fabs(EWKBoson.Rapidity()),fabs(genEWKBoson.Rapidity()),weight);
                    hresponseAbsFirstJetRapidity_ZPt150_Zinc1jet->Fill(fabs(jets[0].v.Rapidity()),fabs(genJets[0].v.Rapidity()),weight);
                    hresponseSumZFirstJetRapidity_ZPt150_Zinc1jet->Fill(fabs(EWKBoson.Rapidity()+jets[0].v.Rapidity())/2.0,fabs(genEWKBoson.Rapidity()+genJets[0].v.Rapidity())/2.0,weight);
                    hresponseDifZFirstJetRapidity_ZPt150_Zinc1jet->Fill(fabs(EWKBoson.Rapidity()-jets[0].v.Rapidity())/2.0,fabs(genEWKBoson.Rapidity()-genJets[0].v.Rapidity())/2.0,weight);

                    hresponseDPhiZFirstJet_ZPt150_Zinc1jet->Fill(fabs(EWKBoson.DeltaPhi(jets[0].v)),fabs(genEWKBoson.DeltaPhi(genJets[0].v)),weight);

                }

                if(genEWKBoson.Pt()>300.&&EWKBoson.Pt()>300.)
                {
                    hresponseAbsZRapidity_ZPt300_Zinc1jet->Fill(fabs(EWKBoson.Rapidity()),fabs(genEWKBoson.Rapidity()),weight);
                    hresponseAbsFirstJetRapidity_ZPt300_Zinc1jet->Fill(fabs(jets[0].v.Rapidity()),fabs(genJets[0].v.Rapidity()),weight);
                    hresponseSumZFirstJetRapidity_ZPt300_Zinc1jet->Fill(fabs(EWKBoson.Rapidity()+jets[0].v.Rapidity())/2.0,fabs(genEWKBoson.Rapidity()+genJets[0].v.Rapidity())/2.0,weight);
                    hresponseDifZFirstJetRapidity_ZPt300_Zinc1jet->Fill(fabs(EWKBoson.Rapidity()-jets[0].v.Rapidity())/2.0,fabs(genEWKBoson.Rapidity()-genJets[0].v.Rapidity())/2.0,weight);

                    hresponseDPhiZFirstJet_ZPt300_Zinc1jet->Fill(fabs(EWKBoson.DeltaPhi(jets[0].v)),fabs(genEWKBoson.DeltaPhi(genJets[0].v)),weight);
                }

                ///different JetPt Cuts///////

                if(genJets[0].v.Pt()>50.&&jets[0].v.Pt()>50.)
                {
                    hresponseAbsZRapidity_FirstJetPt50_Zinc1jet->Fill(fabs(EWKBoson.Rapidity()),fabs(genEWKBoson.Rapidity()),weight);
                    hresponseAbsFirstJetRapidity_FirstJetPt50_Zinc1jet->Fill(fabs(jets[0].v.Rapidity()),fabs(genJets[0].v.Rapidity()),weight);
                    hresponseSumZFirstJetRapidity_FirstJetPt50_Zinc1jet->Fill(fabs(EWKBoson.Rapidity()+jets[0].v.Rapidity())/2.0,fabs(genEWKBoson.Rapidity()+genJets[0].v.Rapidity())/2.0,weight);
                    hresponseDifZFirstJetRapidity_FirstJetPt50_Zinc1jet->Fill(fabs(EWKBoson.Rapidity()-jets[0].v.Rapidity())/2.0,fabs(genEWKBoson.Rapidity()-genJets[0].v.Rapidity())/2.0,weight);
                }

                if(genJets[0].v.Pt()>80.&&jets[0].v.Pt()>80.)
                {
                    hresponseAbsZRapidity_FirstJetPt80_Zinc1jet->Fill(fabs(EWKBoson.Rapidity()),fabs(genEWKBoson.Rapidity()),weight);
                    hresponseAbsFirstJetRapidity_FirstJetPt80_Zinc1jet->Fill(fabs(jets[0].v.Rapidity()),fabs(genJets[0].v.Rapidity()),weight);
                    hresponseSumZFirstJetRapidity_FirstJetPt80_Zinc1jet->Fill(fabs(EWKBoson.Rapidity()+jets[0].v.Rapidity())/2.0,fabs(genEWKBoson.Rapidity()+genJets[0].v.Rapidity())/2.0,weight);
                    hresponseDifZFirstJetRapidity_FirstJetPt80_Zinc1jet->Fill(fabs(EWKBoson.Rapidity()-jets[0].v.Rapidity())/2.0,fabs(genEWKBoson.Rapidity()-genJets[0].v.Rapidity())/2.0,weight);
                }

                hresponseSumZJetRapidity_Zinc1jet->Fill(0.5*fabs(EWKBoson.Rapidity()+jets[0].v.Rapidity()), 0.5*fabs(genEWKBoson.Rapidity()+genJets[0].v.Rapidity()), weight);
                hresponseDifZJetRapidity_Zinc1jet->Fill(0.5*fabs(EWKBoson.Rapidity()-jets[0].v.Rapidity()), 0.5*fabs(genEWKBoson.Rapidity()-genJets[0].v.Rapidity()), weight);


                for (unsigned short i(0); i < 5; i++) {
                    if (EWKBoson.Pt() > ZptRange[i] && EWKBoson.Pt() <= ZptRange[i+1]) {
                        hresponsetau_sum_Zinc1jet[i]->Fill(tau_sum, gentau_sum, weight);
                        hresponsetau_max_Zinc1jet[i]->Fill(tau_max, gentau_max, weight);
                        hresponsetau_c_sum_Zinc1jet[i]->Fill(tau_c_sum, gentau_c_sum, weight);
                        hresponsetau_c_max_Zinc1jet[i]->Fill(tau_c_max, gentau_c_max, weight);
                        hresponsetau_cm_sum_Zinc1jet[i]->Fill(tau_cm_sum, gentau_cm_sum, weight);
                        hresponsetau_cm_max_Zinc1jet[i]->Fill(tau_cm_max, gentau_cm_max, weight);
                        hresponsetau_c_cm_sum_Zinc1jet[i]->Fill(tau_c_cm_sum, gentau_c_cm_sum, weight);
                        hresponsetau_c_cm_max_Zinc1jet[i]->Fill(tau_c_cm_max, gentau_c_cm_max, weight);
                    }
                }
            }

            if (nGoodGenJets_20 >= 1 && passesgenLeptonCut && nGoodJets_20 >= 1 && passesLeptonCut) {

		double RatioValue = 1.;
		if(UnfoldUnc){
		    double binNumber = FirstJetPt_2_Zinc1jet_ratio->GetXaxis()->FindBin(jets_20[0].v.Pt());
		    RatioValue =  FirstJetPt_2_Zinc1jet_ratio->GetBinContent(binNumber);               
		}

                hresponseFirstJetPt_Zinc1jet->Fill(jets_20[0].v.Pt(), genJets_20[0].v.Pt(), weight*RatioValue);      
                hresponseFirstJetPtEta_Zinc1jet->Fill(0.5 + FirstJetPtEta_Zinc1jet->FindBin(jets_20[0].v.Pt(), fabs(jets_20[0].v.Eta())), 
                        0.5 + FirstJetPtEta_Zinc1jet->FindBin(genJets_20[0].v.Pt(), fabs(genJets_20[0].v.Eta())),
                        weight);      
            }

            //exclusive one jet case
            if (nGoodGenJets == 1 && passesgenLeptonCut && nGoodJets == 1 && passesLeptonCut){
                hresponseAbsZRapidity_Zexc1jet->Fill(fabs(EWKBoson.Rapidity()),fabs(genEWKBoson.Rapidity()),weight);
                hresponseAbsJetRapidity_Zexc1jet->Fill(fabs(jets[0].v.Rapidity()),fabs(genJets[0].v.Rapidity()),weight);
                hresponseSumZJetRapidity_Zexc1jet->Fill(fabs(EWKBoson.Rapidity()+jets[0].v.Rapidity())/2.0,fabs(genEWKBoson.Rapidity()+genJets[0].v.Rapidity())/2.0,weight);
                hresponseDifZJetRapidity_Zexc1jet->Fill(fabs(EWKBoson.Rapidity()-jets[0].v.Rapidity())/2.0,fabs(genEWKBoson.Rapidity()-genJets[0].v.Rapidity())/2.0,weight);

                if(EWKBoson.Pt()>100.&&genEWKBoson.Pt()>100.)
                {
                    hresponseAbsZRapidity_ZPt100_Zexc1jet->Fill(fabs(EWKBoson.Rapidity()),fabs(genEWKBoson.Rapidity()),weight);
                    hresponseAbsJetRapidity_ZPt100_Zexc1jet->Fill(fabs(jets[0].v.Rapidity()),fabs(genJets[0].v.Rapidity()),weight);
                    hresponseSumZJetRapidity_ZPt100_Zexc1jet->Fill(fabs(EWKBoson.Rapidity()+jets[0].v.Rapidity())/2.0,fabs(genEWKBoson.Rapidity()+genJets[0].v.Rapidity())/2.0,weight);
                    hresponseDifZJetRapidity_ZPt100_Zexc1jet->Fill(fabs(EWKBoson.Rapidity()-jets[0].v.Rapidity())/2.0,fabs(genEWKBoson.Rapidity()-genJets[0].v.Rapidity())/2.0,weight);
                }

                if(EWKBoson.Pt()>150.&&genEWKBoson.Pt()>150.)
                {
                    hresponseAbsZRapidity_ZPt150_Zexc1jet->Fill(fabs(EWKBoson.Rapidity()),fabs(genEWKBoson.Rapidity()),weight);
                    hresponseAbsJetRapidity_ZPt150_Zexc1jet->Fill(fabs(jets[0].v.Rapidity()),fabs(genJets[0].v.Rapidity()),weight);
                    hresponseSumZJetRapidity_ZPt150_Zexc1jet->Fill(fabs(EWKBoson.Rapidity()+jets[0].v.Rapidity())/2.0,fabs(genEWKBoson.Rapidity()+genJets[0].v.Rapidity())/2.0,weight);
                    hresponseDifZJetRapidity_ZPt150_Zexc1jet->Fill(fabs(EWKBoson.Rapidity()-jets[0].v.Rapidity())/2.0,fabs(genEWKBoson.Rapidity()-genJets[0].v.Rapidity())/2.0,weight);
                }


            }

            //-- Second Jet Pt inclusive 
            if (nGoodGenJets >= 2 && passesgenLeptonCut && nGoodJets >= 2 && passesLeptonCut) {
                ////////////////////////Special Branch//////////////////////
                hresponseZPt_Zinc2jet->Fill(EWKBoson.Pt(),genEWKBoson.Pt(),weight);

                hresponseAbsFirstJetRapidity_Zinc2jet->Fill(fabs(jets[0].v.Rapidity()),fabs(genJets[0].v.Rapidity()),weight);
                hresponseSumZFirstJetRapidity_Zinc2jet->Fill(fabs(EWKBoson.Rapidity()+jets[0].v.Rapidity())/2.0,fabs(genEWKBoson.Rapidity()+genJets[0].v.Rapidity())/2.0,weight);
                hresponseDifZFirstJetRapidity_Zinc2jet->Fill(fabs(EWKBoson.Rapidity()-jets[0].v.Rapidity())/2.0,fabs(genEWKBoson.Rapidity()-genJets[0].v.Rapidity())/2.0,weight);

                hresponseAbsZRapidity_Zinc2jet->Fill(fabs(EWKBoson.Rapidity()),fabs(genEWKBoson.Rapidity()),weight);
                hresponseAbsSecondJetRapidity_Zinc2jet->Fill(fabs(jets[1].v.Rapidity()),fabs(genJets[1].v.Rapidity()),weight);
                hresponseSumZSecondJetRapidity_Zinc2jet->Fill(fabs(EWKBoson.Rapidity()+jets[1].v.Rapidity())/2.0,fabs(genEWKBoson.Rapidity()+genJets[1].v.Rapidity())/2.0,weight);
                hresponseDifZSecondJetRapidity_Zinc2jet->Fill(fabs(EWKBoson.Rapidity()-jets[1].v.Rapidity())/2.0,fabs(genEWKBoson.Rapidity()-genJets[1].v.Rapidity())/2.0,weight);

                hresponseSumFirstSecondJetRapidity_Zinc2jet->Fill(fabs(jets[0].v.Rapidity()+jets[1].v.Rapidity())/2.0,fabs(genJets[0].v.Rapidity()+genJets[1].v.Rapidity())/2.0,weight);
                hresponseDifFirstSecondJetRapidity_Zinc2jet->Fill(fabs(jets[0].v.Rapidity()-jets[1].v.Rapidity())/2.0,fabs(genJets[0].v.Rapidity()-genJets[1].v.Rapidity())/2.0,weight);

                TLorentzVector genDiJets = genJets[0].v + genJets[1].v;
                TLorentzVector DiJets = jets[0].v + jets[1].v;
                hresponseSumZTwoJetsRapidity_Zinc2jet->Fill(fabs(EWKBoson.Rapidity()+DiJets.Rapidity())/2.0,fabs(genEWKBoson.Rapidity()+genDiJets.Rapidity())/2.0,weight);
                hresponseDifZTwoJetsRapidity_Zinc2jet->Fill(fabs(EWKBoson.Rapidity()-DiJets.Rapidity())/2.0,fabs(genEWKBoson.Rapidity()-genDiJets.Rapidity())/2.0,weight);

                /////Azimuthal cross check//////////////////////////////
                hresponseDPhiZFirstJet_Zinc2jet->Fill(fabs(EWKBoson.DeltaPhi(jets[0].v)),fabs(genEWKBoson.DeltaPhi(genJets[0].v)),weight);
                hresponseDPhiZSecondJet_Zinc2jet->Fill(fabs(EWKBoson.DeltaPhi(jets[1].v)),fabs(genEWKBoson.DeltaPhi(genJets[1].v)),weight);
                hresponseDPhiFirstSecondJet_Zinc2jet->Fill(fabs(jets[0].v.DeltaPhi(jets[1].v)),fabs(genJets[0].v.DeltaPhi(genJets[1].v)),weight);

                if(EWKBoson.Pt()>100.&&genEWKBoson.Pt()>100.)
                {
                    hresponseAbsZRapidity_ZPt100_Zinc2jet->Fill(fabs(EWKBoson.Rapidity()),fabs(genEWKBoson.Rapidity()),weight);
                    hresponseAbsSecondJetRapidity_ZPt100_Zinc2jet->Fill(fabs(jets[1].v.Rapidity()),fabs(genJets[1].v.Rapidity()),weight);
                    hresponseSumZSecondJetRapidity_ZPt100_Zinc2jet->Fill(fabs(EWKBoson.Rapidity()+jets[1].v.Rapidity())/2.0,fabs(genEWKBoson.Rapidity()+genJets[1].v.Rapidity())/2.0,weight);
                    hresponseDifZSecondJetRapidity_ZPt100_Zinc2jet->Fill(fabs(EWKBoson.Rapidity()-jets[1].v.Rapidity())/2.0,fabs(genEWKBoson.Rapidity()-genJets[1].v.Rapidity())/2.0,weight);
                }

                if(EWKBoson.Pt()>150.&&genEWKBoson.Pt()>150.)
                {
                    hresponseAbsZRapidity_ZPt150_Zinc2jet->Fill(fabs(EWKBoson.Rapidity()),fabs(genEWKBoson.Rapidity()),weight);
                    hresponseAbsSecondJetRapidity_ZPt150_Zinc2jet->Fill(fabs(jets[1].v.Rapidity()),fabs(genJets[1].v.Rapidity()),weight);
                    hresponseSumZSecondJetRapidity_ZPt150_Zinc2jet->Fill(fabs(EWKBoson.Rapidity()+jets[1].v.Rapidity())/2.0,fabs(genEWKBoson.Rapidity()+genJets[1].v.Rapidity())/2.0,weight);
                    hresponseDifZSecondJetRapidity_ZPt150_Zinc2jet->Fill(fabs(EWKBoson.Rapidity()-jets[1].v.Rapidity())/2.0,fabs(genEWKBoson.Rapidity()-genJets[1].v.Rapidity())/2.0,weight);

                    hresponseDPhiZFirstJet_ZPt150_Zinc2jet->Fill(fabs(EWKBoson.DeltaPhi(jets[0].v)),fabs(genEWKBoson.DeltaPhi(genJets[0].v)),weight);

                }

                if(EWKBoson.Pt()>300.&&genEWKBoson.Pt()>300.)
                {
                    hresponseDPhiZFirstJet_ZPt300_Zinc2jet->Fill(fabs(EWKBoson.DeltaPhi(jets[0].v)),fabs(genEWKBoson.DeltaPhi(genJets[0].v)),weight);
                }

                //set jet rapidity discriminator////

                if(fabs(genJets[0].v.Rapidity()-genJets[1].v.Rapidity())>2 && fabs(jets[0].v.Rapidity()-jets[1].v.Rapidity())>2)
                {
                    hresponseAbsZRapidity_DifJetRapidityl2_Zinc2jet->Fill(fabs(EWKBoson.Rapidity()),fabs(genEWKBoson.Rapidity()),weight);
                    hresponseAbsFirstJetRapidity_DifJetRapidityl2_Zinc2jet->Fill(fabs(jets[0].v.Rapidity()),fabs(genJets[0].v.Rapidity()),weight);
                    hresponseSumZFirstJetRapidity_DifJetRapidityl2_Zinc2jet->Fill(fabs(EWKBoson.Rapidity()+jets[0].v.Rapidity())/2.0,fabs(genEWKBoson.Rapidity()+genJets[0].v.Rapidity())/2.0,weight);
                    hresponseDifZFirstJetRapidity_DifJetRapidityl2_Zinc2jet->Fill(fabs(EWKBoson.Rapidity()-jets[0].v.Rapidity())/2.0,fabs(genEWKBoson.Rapidity()-genJets[0].v.Rapidity())/2.0,weight);
                }

                if(fabs(genJets[0].v.Rapidity()-genJets[1].v.Rapidity())<2 && fabs(jets[0].v.Rapidity()-jets[1].v.Rapidity())<2)
                {
                    hresponseAbsZRapidity_DifJetRapiditys2_Zinc2jet->Fill(fabs(EWKBoson.Rapidity()),fabs(genEWKBoson.Rapidity()),weight);
                    hresponseAbsFirstJetRapidity_DifJetRapiditys2_Zinc2jet->Fill(fabs(jets[0].v.Rapidity()),fabs(genJets[0].v.Rapidity()),weight);
                    hresponseSumZFirstJetRapidity_DifJetRapiditys2_Zinc2jet->Fill(fabs(EWKBoson.Rapidity()+jets[0].v.Rapidity())/2.0,fabs(genEWKBoson.Rapidity()+genJets[0].v.Rapidity())/2.0,weight);
                    hresponseDifZFirstJetRapidity_DifJetRapiditys2_Zinc2jet->Fill(fabs(EWKBoson.Rapidity()-jets[0].v.Rapidity())/2.0,fabs(genEWKBoson.Rapidity()-genJets[0].v.Rapidity())/2.0,weight);
                }

		double RatioValue = 1.;
		double RatioValue1 = 1.;
		if(UnfoldUnc){
		    double binNumber = SecondJetEta_2_Zinc2jet_ratio->GetXaxis()->FindBin(fabs(jets[1].v.Eta()));
		    RatioValue =  SecondJetEta_2_Zinc2jet_ratio->GetBinContent(binNumber);
		    double binNumber1 = JetsHT_2_Zinc2jet_ratio->GetXaxis()->FindBin(jetsHT);
		    RatioValue1 = JetsHT_2_Zinc2jet_ratio->GetBinContent(binNumber1);
		}

                hresponseSecondJetEta_Zinc2jet->Fill(fabs(jets[1].v.Eta()), fabs(genJets[1].v.Eta()), weight*RatioValue);      
                hresponseSecondJetAbsRapidity_Zinc2jet->Fill(fabs(jets[1].v.Rapidity()), fabs(genJets[1].v.Rapidity()), weight*RatioValue);      
                hresponseSecondJetEtaHigh_Zinc2jet->Fill(fabs(jets[1].v.Eta()), fabs(genJets[1].v.Eta()), weight);      
                hresponseSecondJetRapidityHigh_Zinc2jet->Fill(fabs(jets[1].v.Rapidity()), fabs(genJets[1].v.Rapidity()), weight);      
                hresponseJetsHT_Zinc2jet->Fill(jetsHT, genJetsHT, weight*RatioValue1);
                // hresponseJetsHT_2_Zinc2jet->Fill(jetsHT, genJetsHT, weight);
                //responseTwoJetsPtDiffInc->Fill(jet1Minus2.Pt(), genJet1Minus2.Pt(), weight);
                //responseBestTwoJetsPtDiffInc->Fill(bestJet1Minus2.Pt(), genBestJet1Minus2.Pt(), weight);
                //responseJetsMassInc->Fill(jet1Plus2.M(), genJet1Plus2.M(), weight);
                hresponseJetsMass_Zinc2jet->Fill(jet1Plus2.M(), genJet1Plus2.M(), weight);

                if (EvtVtxCnt < 14) hresponseJetsMassLowPU_Zinc2jet->Fill(jet1Plus2.M(), genJet1Plus2.M(), weight);
                else if (EvtVtxCnt < 18) hresponseJetsMassMidPU_Zinc2jet->Fill(jet1Plus2.M(), genJet1Plus2.M(), weight);
                else hresponseJetsMassHigPU_Zinc2jet->Fill(jet1Plus2.M(), genJet1Plus2.M(), weight);

                //responseBestJetsMassInc->Fill(bestJet1Plus2.M(), genBestJet1Plus2.M(), weight);
                //responseSpTJets_Zinc2jet->Fill(SpTsub(jets[0].v, jets[1].v), SpTsub(genJets[0].v, genJets[1].v), weight);
                //responseBestSpTJets_Zinc2jet->Fill(SpTsub(bestTwoJets.first, bestTwoJets.second), SpTsub(genBestTwoJets.first, genBestTwoJets.second), weight);
                //responseSpT_Zinc2jet->Fill(SpT(leptons[0].v, leptons[1].v, jets[0].v, jets[1].v), SpT(genLeptons[0].v, genLeptons[1].v, genJets[0].v, genJets[1].v), weight);
                //responseBestSpT_Zinc2jet->Fill(SpT(leptons[0].v, leptons[1].v, bestTwoJets.first, bestTwoJets.second), SpT(genLeptons[0].v, genLeptons[1].v, genBestTwoJets.first, genBestTwoJets.second), weight);
                //responsedPhiJets_Zinc2jet->Fill(deltaPhi(jets[0].v, jets[1].v), deltaPhi(genJets[0].v, genJets[1].v), weight);
                //responseBestdPhiJets_Zinc2jet->Fill(deltaPhi(bestTwoJets.first, bestTwoJets.second), deltaPhi(genBestTwoJets.first, genBestTwoJets.second), weight);
                //responsePHI_Zinc2jet->Fill(PHI(leptons[0].v, leptons[1].v, jets[0].v, jets[1].v), PHI(genLeptons[0].v, genLeptons[1].v, genJets[0].v, genJets[1].v), weight);
                //responseBestPHI_Zinc2jet->Fill(PHI(leptons[0].v, leptons[1].v, bestTwoJets.first, bestTwoJets.second), PHI(genLeptons[0].v, genLeptons[1].v, genBestTwoJets.first, genBestTwoJets.second), weight);
                //responsePHI_T_Zinc2jet->Fill(PHI_T(leptons[0].v, leptons[1].v, jets[0].v, jets[1].v), PHI_T(genLeptons[0].v, genLeptons[1].v, genJets[0].v, genJets[1].v), weight);
                //responseBestPHI_T_Zinc2jet->Fill(PHI_T(leptons[0].v, leptons[1].v, bestTwoJets.first, bestTwoJets.second), PHI_T(genLeptons[0].v, genLeptons[1].v, genBestTwoJets.first, genBestTwoJets.second), weight);
                //responseSPhi_Zinc2jet->Fill(SPhi(leptons[0].v, leptons[1].v, jets[0].v, jets[1].v), SPhi(genLeptons[0].v, genLeptons[1].v, genJets[0].v, genJets[1].v), weight);
                //responsedEtaJets_Zinc2jet->Fill(fabs(genJets[0].eta-genJets[1].eta),fabs(jets[0].eta-jets[1].eta), weight);
                //responseBestSPhi_Zinc2jet->Fill(SPhi(leptons[0].v, leptons[1].v, bestTwoJets.first, bestTwoJets.second), SPhi(genLeptons[0].v, genLeptons[1].v, genBestTwoJets.first, genBestTwoJets.second), weight);

            }


            if (nGoodGenJets_20 >= 2 && passesgenLeptonCut && nGoodJets_20 >= 2 && passesLeptonCut) {
		double RatioValue = 1.;
		if(UnfoldUnc){
		    double binNumber = SecondJetPt_2_Zinc2jet_ratio->GetXaxis()->FindBin(jets_20[1].v.Pt());
		    RatioValue =  SecondJetPt_2_Zinc2jet_ratio->GetBinContent(binNumber);
		}

                hresponseSecondJetPt_Zinc2jet->Fill(jets_20[1].v.Pt(), genJets_20[1].v.Pt(), weight*RatioValue);              
            }

            //-- Second Jet Pt exclusive
            if (nGoodGenJets == 2 && passesgenLeptonCut && nGoodJets == 2 && passesLeptonCut) {
                //////////////////////////////Special Branch/////////////////
                hresponseAbsZRapidity_Zexc2jet->Fill(fabs(EWKBoson.Rapidity()),fabs(genEWKBoson.Rapidity()),weight);
                hresponseAbsSecondJetRapidity_Zexc2jet->Fill(fabs(jets[1].v.Rapidity()),fabs(genJets[1].v.Rapidity()),weight);
                hresponseSumZSecondJetRapidity_Zexc2jet->Fill(fabs(EWKBoson.Rapidity()+jets[1].v.Rapidity())/2.0,fabs(genEWKBoson.Rapidity()+genJets[1].v.Rapidity())/2.0,weight);
                hresponseDifZSecondJetRapidity_Zexc2jet->Fill(fabs(EWKBoson.Rapidity()-jets[1].v.Rapidity())/2.0,fabs(genEWKBoson.Rapidity()-genJets[1].v.Rapidity())/2.0,weight);

                if(EWKBoson.Pt()>100.&&genEWKBoson.Pt()>100.)
                {
                    hresponseAbsZRapidity_ZPt100_Zexc2jet->Fill(fabs(EWKBoson.Rapidity()),fabs(genEWKBoson.Rapidity()),weight);
                    hresponseAbsSecondJetRapidity_ZPt100_Zexc2jet->Fill(fabs(jets[1].v.Rapidity()),fabs(genJets[1].v.Rapidity()),weight);
                    hresponseSumZSecondJetRapidity_ZPt100_Zexc2jet->Fill(fabs(EWKBoson.Rapidity()+jets[1].v.Rapidity())/2.0,fabs(genEWKBoson.Rapidity()+genJets[1].v.Rapidity())/2.0,weight);
                    hresponseDifZSecondJetRapidity_ZPt100_Zexc2jet->Fill(fabs(EWKBoson.Rapidity()-jets[1].v.Rapidity())/2.0,fabs(genEWKBoson.Rapidity()-genJets[1].v.Rapidity())/2.0,weight);
                }

                if(EWKBoson.Pt()>150.&&genEWKBoson.Pt()>150.)
                {
                    hresponseAbsZRapidity_ZPt150_Zexc2jet->Fill(fabs(EWKBoson.Rapidity()),fabs(genEWKBoson.Rapidity()),weight);
                    hresponseAbsSecondJetRapidity_ZPt150_Zexc2jet->Fill(fabs(jets[1].v.Rapidity()),fabs(genJets[1].v.Rapidity()),weight);
                    hresponseSumZSecondJetRapidity_ZPt150_Zexc2jet->Fill(fabs(EWKBoson.Rapidity()+jets[1].v.Rapidity())/2.0,fabs(genEWKBoson.Rapidity()+genJets[1].v.Rapidity())/2.0,weight);
                    hresponseDifZSecondJetRapidity_ZPt150_Zexc2jet->Fill(fabs(EWKBoson.Rapidity()-jets[1].v.Rapidity())/2.0,fabs(genEWKBoson.Rapidity()-genJets[1].v.Rapidity())/2.0,weight);
                }

                //

                //responseTwoJetsPtDiffExc->Fill(jet1Minus2.Pt(), genJet1Minus2.Pt(), weight);
                //responseJetsMassExc->Fill(jet1Plus2.M(), genJet1Plus2.M(), weight);
                //responseSpTJets_Zexc2jet->Fill(SpTsub(jets[0].v, jets[1].v), SpTsub(genJets[0].v, genJets[1].v), weight);
                //responseSpT_Zexc2jet->Fill(SpT(leptons[0].v, leptons[1].v, jets[0].v, jets[1].v), SpT(genLeptons[0].v, genLeptons[1].v, genJets[0].v, genJets[1].v), weight);
                //responsedPhiJets_Zexc2jet->Fill(deltaPhi(jets[0].v, jets[1].v), deltaPhi(genJets[0].v, genJets[1].v), weight);
                //responsePHI_Zexc2jet->Fill(PHI(leptons[0].v, leptons[1].v, jets[0].v, jets[1].v), PHI(genLeptons[0].v, genLeptons[1].v, genJets[0].v, genJets[1].v), weight);
                //responsePHI_T_Zexc2jet->Fill(PHI_T(leptons[0].v, leptons[1].v, jets[0].v, jets[1].v), PHI_T(genLeptons[0].v, genLeptons[1].v, genJets[0].v, genJets[1].v), weight);
                //responsedEtaJets_Zexc2jet->Fill(fabs(genJets[0].eta-genJets[1].eta),fabs(jets[0].eta-jets[1].eta), weight);
                //responseSPhi_Zexc2jet->Fill(SPhi(leptons[0].v, leptons[1].v, jets[0].v, jets[1].v), SPhi(genLeptons[0].v, genLeptons[1].v, genJets[0].v, genJets[1].v), weight);
            }

            //-- Third Jet Pt  
            if (nGoodGenJets >= 3 && passesgenLeptonCut && nGoodJets >= 3 && passesLeptonCut) {

		double RatioValue = 1.;
		double RatioValue1 = 1.;
		if(UnfoldUnc){
		    double binNumber = ThirdJetEta_2_Zinc3jet_ratio->GetXaxis()->FindBin(fabs(jets[2].v.Eta()));
		    RatioValue =  ThirdJetEta_2_Zinc3jet_ratio->GetBinContent(binNumber);
		    double binNumber1 = JetsHT_2_Zinc3jet_ratio->GetXaxis()->FindBin(jetsHT);
		    RatioValue1 = JetsHT_2_Zinc3jet_ratio->GetBinContent(binNumber1);
		}

                hresponseThirdJetEta_Zinc3jet->Fill(fabs(jets[2].v.Eta()), fabs(genJets[2].v.Eta()), weight*RatioValue);         
                hresponseThirdJetAbsRapidity_Zinc3jet->Fill(fabs(jets[2].v.Rapidity()), fabs(genJets[2].v.Rapidity()), weight*RatioValue);      
                hresponseThirdJetEtaHigh_Zinc3jet->Fill(fabs(jets[2].v.Eta()), fabs(genJets[2].v.Eta()), weight);      
                hresponseThirdJetRapidityHigh_Zinc3jet->Fill(fabs(jets[2].v.Rapidity()), fabs(genJets[2].v.Rapidity()), weight);      
                hresponseJetsHT_Zinc3jet->Fill(jetsHT, genJetsHT, weight*RatioValue1);

                /////Azimuthal cross check//////////////////////////////
                hresponseDPhiZFirstJet_Zinc3jet->Fill(fabs(EWKBoson.DeltaPhi(jets[0].v)),fabs(genEWKBoson.DeltaPhi(genJets[0].v)),weight);
                hresponseDPhiZSecondJet_Zinc3jet->Fill(fabs(EWKBoson.DeltaPhi(jets[1].v)),fabs(genEWKBoson.DeltaPhi(genJets[1].v)),weight);
                hresponseDPhiZThirdJet_Zinc3jet->Fill(fabs(EWKBoson.DeltaPhi(jets[2].v)),fabs(genEWKBoson.DeltaPhi(genJets[2].v)),weight);
                hresponseDPhiFirstSecondJet_Zinc3jet->Fill(fabs(jets[0].v.DeltaPhi(jets[1].v)),fabs(genJets[0].v.DeltaPhi(genJets[1].v)),weight);
                hresponseDPhiFirstThirdJet_Zinc3jet->Fill(fabs(jets[0].v.DeltaPhi(jets[2].v)),fabs(genJets[0].v.DeltaPhi(genJets[2].v)),weight);
                hresponseDPhiSecondThirdJet_Zinc3jet->Fill(fabs(jets[1].v.DeltaPhi(jets[2].v)),fabs(genJets[1].v.DeltaPhi(genJets[2].v)),weight);

                if(EWKBoson.Pt()>150.&&genEWKBoson.Pt()>150.)
                {
                    hresponseDPhiZFirstJet_ZPt150_Zinc3jet->Fill(fabs(EWKBoson.DeltaPhi(jets[0].v)),fabs(genEWKBoson.DeltaPhi(genJets[0].v)),weight);
                    hresponseDPhiZSecondJet_ZPt150_Zinc3jet->Fill(fabs(EWKBoson.DeltaPhi(jets[1].v)),fabs(genEWKBoson.DeltaPhi(genJets[1].v)),weight);
                    hresponseDPhiZThirdJet_ZPt150_Zinc3jet->Fill(fabs(EWKBoson.DeltaPhi(jets[2].v)),fabs(genEWKBoson.DeltaPhi(genJets[2].v)),weight);
                    hresponseDPhiFirstSecondJet_ZPt150_Zinc3jet->Fill(fabs(jets[0].v.DeltaPhi(jets[1].v)),fabs(genJets[0].v.DeltaPhi(genJets[1].v)),weight);
                    hresponseDPhiFirstThirdJet_ZPt150_Zinc3jet->Fill(fabs(jets[0].v.DeltaPhi(jets[2].v)),fabs(genJets[0].v.DeltaPhi(genJets[2].v)),weight);
                    hresponseDPhiSecondThirdJet_ZPt150_Zinc3jet->Fill(fabs(jets[1].v.DeltaPhi(jets[2].v)),fabs(genJets[1].v.DeltaPhi(genJets[2].v)),weight);
                }

                if(EWKBoson.Pt()>300.&&genEWKBoson.Pt()>300.)
                {
                    hresponseDPhiZFirstJet_ZPt300_Zinc3jet->Fill(fabs(EWKBoson.DeltaPhi(jets[0].v)),fabs(genEWKBoson.DeltaPhi(genJets[0].v)),weight);
                    hresponseDPhiZSecondJet_ZPt300_Zinc3jet->Fill(fabs(EWKBoson.DeltaPhi(jets[1].v)),fabs(genEWKBoson.DeltaPhi(genJets[1].v)),weight);
                    hresponseDPhiZThirdJet_ZPt300_Zinc3jet->Fill(fabs(EWKBoson.DeltaPhi(jets[2].v)),fabs(genEWKBoson.DeltaPhi(genJets[2].v)),weight);
                    hresponseDPhiFirstSecondJet_ZPt300_Zinc3jet->Fill(fabs(jets[0].v.DeltaPhi(jets[1].v)),fabs(genJets[0].v.DeltaPhi(genJets[1].v)),weight);
                    hresponseDPhiFirstThirdJet_ZPt300_Zinc3jet->Fill(fabs(jets[0].v.DeltaPhi(jets[2].v)),fabs(genJets[0].v.DeltaPhi(genJets[2].v)),weight);
                    hresponseDPhiSecondThirdJet_ZPt300_Zinc3jet->Fill(fabs(jets[1].v.DeltaPhi(jets[2].v)),fabs(genJets[1].v.DeltaPhi(genJets[2].v)),weight);
                }

                if(EWKBoson.Pt()>150. && genEWKBoson.Pt()>150. && (jets[0].v.Pt()+jets[1].v.Pt()+jets[2].v.Pt()>300.) && (genJets[0].v.Pt()+genJets[1].v.Pt()+genJets[2].v.Pt()>300.))
                {
                    hresponseDPhiZFirstJet_ZPt150_HT300_Zinc3jet->Fill(fabs(EWKBoson.DeltaPhi(jets[0].v)),fabs(genEWKBoson.DeltaPhi(genJets[0].v)),weight);
                    hresponseDPhiZSecondJet_ZPt150_HT300_Zinc3jet->Fill(fabs(EWKBoson.DeltaPhi(jets[1].v)),fabs(genEWKBoson.DeltaPhi(genJets[1].v)),weight);
                    hresponseDPhiZThirdJet_ZPt150_HT300_Zinc3jet->Fill(fabs(EWKBoson.DeltaPhi(jets[2].v)),fabs(genEWKBoson.DeltaPhi(genJets[2].v)),weight);
                }

            }


            if (nGoodGenJets_20 >= 3 && passesgenLeptonCut && nGoodJets_20 >= 3 && passesLeptonCut) {

		double RatioValue = 1.;
		if(UnfoldUnc){
		    double binNumber = ThirdJetPt_2_Zinc3jet_ratio->GetXaxis()->FindBin(jets_20[2].v.Pt());
		    RatioValue = ThirdJetPt_2_Zinc3jet_ratio->GetBinContent(binNumber);
		}

                hresponseThirdJetPt_Zinc3jet->Fill(jets_20[2].v.Pt(), genJets_20[2].v.Pt(), weight*RatioValue); 
        
            }


            //-- Fourth Jet Pt  
            if (nGoodGenJets >= 4 && passesgenLeptonCut && nGoodJets >= 4 && passesLeptonCut){

                hresponseFourthJetEta_Zinc4jet->Fill(fabs(jets[3].v.Eta()), fabs(genJets[3].v.Eta()), weight);      
                hresponseFourthJetAbsRapidity_Zinc4jet->Fill(fabs(jets[3].v.Rapidity()), fabs(genJets[3].v.Rapidity()), weight);      
                hresponseFourthJetEtaHigh_Zinc4jet->Fill(fabs(jets[3].v.Eta()), fabs(genJets[3].v.Eta()), weight);      
                hresponseFourthJetRapidityHigh_Zinc4jet->Fill(fabs(jets[3].v.Rapidity()), fabs(genJets[3].v.Rapidity()), weight);      
                hresponseJetsHT_Zinc4jet->Fill(jetsHT, genJetsHT, weight);
            }

            if (nGoodGenJets_20 >= 4 && passesgenLeptonCut && nGoodJets_20 >= 4 && passesLeptonCut){
                hresponseFourthJetPt_Zinc4jet->Fill(jets_20[3].v.Pt(), genJets_20[3].v.Pt(), weight);      
            }

            //-- Fifth Jet Pt  
            if (nGoodGenJets >= 5 && passesgenLeptonCut && nGoodJets >= 5 && passesLeptonCut){

                hresponseFifthJetEta_Zinc5jet->Fill(fabs(jets[4].v.Eta()), fabs(genJets[4].v.Eta()), weight);      
                hresponseFifthJetAbsRapidity_Zinc5jet->Fill(fabs(jets[4].v.Rapidity()), fabs(genJets[4].v.Rapidity()), weight);      
                hresponseFifthJetEtaHigh_Zinc5jet->Fill(fabs(jets[4].v.Eta()), fabs(genJets[4].v.Eta()), weight);      
                hresponseFifthJetRapidityHigh_Zinc5jet->Fill(fabs(jets[4].v.Rapidity()), fabs(genJets[4].v.Rapidity()), weight);      
                hresponseJetsHT_Zinc5jet->Fill(jetsHT, genJetsHT, weight);
            } 

            if (nGoodGenJets_20 >= 5 && passesgenLeptonCut && nGoodJets_20 >= 5 && passesLeptonCut){
                hresponseFifthJetPt_Zinc5jet->Fill(jets_20[4].v.Pt(), genJets_20[4].v.Pt(), weight);      
            }

        }
        //=======================================================================================================//

//	if(passesLeptonCut && !passesgenLeptonCut){
//	    std::cout << "Event " << nEvents << " passes Reco cut but not gen cuts: reco - gen\n"
//		      << "Nleptons: " << nLeptons << " - " << ngenLeptons << "\n"
//		      << "Pt l1:    " 
//		      << (nLeptons > 0 ? leptons[0].v.Pt() : -1) << " - "
//		      << (ngenLeptons > 0 ? genLeptons[0].v.Pt() : -1) << "\n"
//		      << "Pt l2:    " 
//		      << (nLeptons > 1 ? leptons[1].v.Pt() : -1) << " - "
//		      << (ngenLeptons > 1 ? genLeptons[1].v.Pt() : -1) << "\n"
//		      << "Eta l1:    " 
//		      << (nLeptons > 0 ? leptons[0].v.Eta() : -1) << " - "
//		      << (ngenLeptons > 0 ? genLeptons[0].v.Eta() : -1) << "\n"
//		      << "Eta l2:    " 
//		      << (nLeptons > 1 ? leptons[1].v.Eta() : -1) << " - "
//		      << (ngenLeptons > 1 ? genLeptons[1].v.Eta() : -1) << "\n";
//			  
//	    if(nLeptons >= 2 && ngenLeptons >= 2){
//		std::cout << "Charges: " << (leptons[0].charge ? '+' : '-' )
//                          << (leptons[1].charge ? '+' : '-' )
//                          << "-" << (genLeptons[0].charge ? '+' : '-')
//			  << (genLeptons[1].charge ? '+' : '-');
//	    }
//	}
    } //End of loop over all the events
    cout << endl;
    //==========================================================================================================//

    if (DEBUG) cout << "Stop after line " << __LINE__ << endl;
    //==========================================================================================================//
    //         Writing file              //
    //==================================//

    outputFile->cd();

    //--- Save all the histograms ---
    unsigned short numbOfHistograms = listOfHistograms.size();
    for (unsigned short i(0); i < numbOfHistograms; i++){
        string hName = listOfHistograms[i]->GetName();
        if ((!hasGenInfo && hName.find("gen") != string::npos) || (!hasRecoInfo && hName.find("gen") == string::npos)) continue; 
	//finalize normalisation of MC histograms:
	double a = 1./processedEventMcWeightSum_;
	if(!EvtIsRealData){
	    listOfHistograms[i]->Scale(a);
	}
        listOfHistograms[i]->Write();        
    }

    //--- let's delete all histograms --- 
    for (unsigned short i(0); i < numbOfHistograms; i++){
        delete listOfHistograms[i];
    }

    outputFile->Write();
    outputFile->Close();

    //==========================================================================================================//

    nEffEventsPassingTrigger *= nEvents / genWeightSum;
    nEffEventsVInc0JetsNoTrig *= nEvents / genWeightSum;
    nEffEventsVInc0Jets *= nEvents / genWeightSum;
    nEffEventsVInc1Jets *= nEvents / genWeightSum;
    nEffEventsVInc2Jets *= nEvents / genWeightSum;
    nEffEventsVInc3Jets *= nEvents / genWeightSum;
    nEffEventsWithTwoGoodLeptons *= nEvents / genWeightSum;
    nEffEventsWithTwoGoodLeptonsWithOppCharge *= nEvents / genWeightSum;
    nEffEventsWithTwoGoodLeptonsWithOppChargeAndGoodMass  *= nEvents / genWeightSum;

    nEffGenEventsVInc0Jets *= nEvents / genWeightSum;
    nEffGenEventsVInc1Jets *= nEvents / genWeightSum;
    nEffGenEventsVInc2Jets *= nEvents / genWeightSum;
    nEffGenEventsVInc3Jets *= nEvents / genWeightSum;    
    nEffGenEventsWithTwoGoodLeptons  *= nEvents / genWeightSum;
    nEffGenEventsWithTwoGoodLeptonsWithOppCharge  *= nEvents / genWeightSum;
    nEffGenEventsWithTwoGoodLeptonsWithOppChargeAndGoodMass *= nEvents / genWeightSum;
	    
    cout << "Number of processed events                                : " << nEvents << endl;
    if(maxFiles_ < 0){
	cout << "Fraction of processed events from dataset                 : " << nEvents << "/" << EvtCount_
	     << " = " << (nEvents/double(EvtCount_)) << endl;
    }
    cout << "Number of events passing the trigger                      : " << nEventsPassingTrigger << "\n";
    cout << "Number with two good leptons (gen)                        : " << nEventsWithTwoGoodLeptons 
	 << " (" << nGenEventsWithTwoGoodLeptons << ")" << endl;
    cout << "Number with two good leptons of opp. charge (gem)         : " << nEventsWithTwoGoodLeptonsWithOppCharge 
	 << " (" << nGenEventsWithTwoGoodLeptonsWithOppCharge << ")" << endl;
    cout << "Number with two good leptons of opp. charge and good mass : " << nEventsWithTwoGoodLeptonsWithOppChargeAndGoodMass 
	 << " (" << nGenEventsWithTwoGoodLeptonsWithOppChargeAndGoodMass << ")" << endl;
    cout << "Number Reco Inclusif V + 0 jets                           : " << nEventsVInc0Jets << endl;
    cout << "Number Reco Inclusif V + 0 jets no trigger                : " << nEventsVInc0JetsNoTrig << endl;
    cout << "Number Reco Inclusif V + 1 jets                           : " << nEventsVInc1Jets << endl;
    cout << "Number Reco Inclusif V + 2 jets                           : " << nEventsVInc2Jets << endl;
    cout << "Number Reco Inclusif V + 3 jets                           : " << nEventsVInc3Jets << endl;
    cout << "Number GEN Inclusif V + 0 jets                            : " << nGenEventsVInc0Jets << endl;
    cout << "Number GEN Inclusif V + 1 jets                            : " << nGenEventsVInc1Jets << endl;
    cout << "Number GEN Inclusif V + 2 jets                            : " << nGenEventsVInc2Jets << endl;
    cout << "Number GEN Inclusif V + 3 jets                            : " << nGenEventsVInc3Jets << endl;

    cout << "Eff. number with two good leptons (gen)                        : " << nEffEventsWithTwoGoodLeptons 
	 << " (" << nEffGenEventsWithTwoGoodLeptons << ")" << endl;
    cout << "Eff. number with two good leptons of opp. charge (gem)         : " << nEffEventsWithTwoGoodLeptonsWithOppCharge 
	 << " (" << nEffGenEventsWithTwoGoodLeptonsWithOppCharge << ")" << endl;
    cout << "Eff. number with two good leptons of opp. charge and good mass : " << nEffEventsWithTwoGoodLeptonsWithOppChargeAndGoodMass 
	 << " (" << nEffGenEventsWithTwoGoodLeptonsWithOppChargeAndGoodMass << ")" << endl;
    cout << "Eff. number Reco Inclusif V + 0 jets                           : " << nEffEventsVInc0Jets << endl;
    cout << "Eff. number Reco Inclusif V + 0 jets no trigger                : " << nEffEventsVInc0JetsNoTrig << endl;
    cout << "Eff. number Reco Inclusif V + 1 jets                           : " << nEffEventsVInc1Jets << endl;
    cout << "Eff. number Reco Inclusif V + 2 jets                           : " << nEffEventsVInc2Jets << endl;
    cout << "Eff. number Reco Inclusif V + 3 jets                           : " << nEffEventsVInc3Jets << endl;
    cout << "Eff. number GEN Inclusif V + 0 jets                            : " << nEffGenEventsVInc0Jets << endl;
    cout << "Eff. number GEN Inclusif V + 1 jets                            : " << nEffGenEventsVInc1Jets << endl;
    cout << "Eff. number GEN Inclusif V + 2 jets                            : " << nEffGenEventsVInc2Jets << endl;
    cout << "Eff. number GEN Inclusif V + 3 jets                            : " << nEffGenEventsVInc3Jets << endl;
    cout << "Sum of MC event weights                                        : " << processedEventMcWeightSum_ << endl;
    if(!EvtIsRealData){
    cout << "MC norm., yield_scale*lumi*xsec*skim_accep/sum_weights*unc_var. : " 
	     << yieldScale << "*" << lumi_ << "*" << xsec_ << "*"
	     << skimAccep_[0] << "/" << processedEventMcWeightSum_
	     << "*" << xsecFactor_ << " = " << norm_ / processedEventMcWeightSum_ << endl;
    }
}

void ZJets::initLHAPDF(TString pdfSet, int pdfMember)
{
    LHAPDF::initPDFSet(1, pdfSet.Data(), pdfMember);
    //LHAPDF::initPDFSet(1, "CT10.LHgrid");
    LHAPDF::initPDFSet(2, "cteq6ll.LHpdf");
    if (pdfMember > LHAPDF::numberPDF(1) + 1) {
        cout << "Warning pdfMember to high" << endl;
        return;
    }
}

double ZJets::computePDFWeight()
{

    //-- get the pdgId of the two colliding partons 
    double wPdf(1.);
/*

    int id1 = pdfInfo_->at(0);
    int id2 = pdfInfo_->at(1);
    if (id1 == 21) id1 = 0; // 21 is Pythia convention for gluon, but needs to be 0 for LHAPDF
    if (id2 == 21) id2 = 0;

    double pdf1  = LHAPDF::xfx(1, pdfInfo_->at(2), pdfInfo_->at(4), id1);
    double pdf2  = LHAPDF::xfx(1, pdfInfo_->at(3), pdfInfo_->at(4), id2);
    double pdf01 = LHAPDF::xfx(2, pdfInfo_->at(2), pdfInfo_->at(4), id1);
    double pdf02 = LHAPDF::xfx(2, pdfInfo_->at(3), pdfInfo_->at(4), id2);

    if (pdfInfo_->at(2) * pdfInfo_->at(3) > 0) {
        wPdf = pdf1 * pdf2;
        if (pdf01*pdf02 <= 0 || pdf1*pdf2 <= 0) {
            cout << "Small problem" << endl;
            wPdf = 1;
        }
        else {
            wPdf /= (pdf01 * pdf02);
        }
    }
*/

    return wPdf;

}

void ZJets::getMuons(vector<leptonStruct>& leptons,  vector<leptonStruct>& vetoMuons)
{

    //--- get the number of Muon candidates from the vector size ---
    unsigned short nTotLeptons(MuEta->size());

    //    bool eventTrigger = false;
    // we also have event trigger variables --> we should at least match one of the leptons to trigger
/* // CommentAG: check patMuonTrig
    for (unsigned short i(0); i < nTotLeptons; i++) {
        int whichTrigger(patMuonTrig_->at(i));
        if (lepSel == "SMu" && (whichTrigger & 0x1)) eventTrigger = true;
    }
  */        

    for (unsigned short i(0); i < nTotLeptons; i++) {
	//        double muonId = 0;

       //CommentAG: don't have patMuonCombId
/*
        if(fileName.Index("mcatnlo") >= 0 || fileName.Index("MG-MLM") >= 0) {
            muonId = (double) patMuonCombId_Int->at(i);
        }
        else { 
            muonId = (double) patMuonCombId_Double->at(i);
        }
*/
        leptonStruct mu(MuPt->at(i), 
                MuEta->at(i), 
                MuPhi->at(i), 
                MuE->at(i), 
                MuCh->at(i), 
                MuIdTight->at(i),    //  CommentAG: muonId; Tight muons are selected in Bonzai
                MuPfIso->at(i), 
                MuEta->at(i),
                0);  // CommentAG


	float qter = 1.0;
	if (doRochester) {
	    if (!EvtIsRealData) {
		rmcor->momcor_mc(mu.v, (float)mu.charge, 0, qter);
	    }
	    else {
		rmcor->momcor_data(mu.v, (float)mu.charge, 0, qter);
	    }
	}

        bool muPassesPtCut(mu.v.Pt() >= (lepPtCutMin*0.8));
        bool muPassesEtaCut(fabs(mu.v.Eta()) <= 0.1*lepEtaCutMax);
       // bool muPassesIdCut(mu.id & 0x1);  //CommentAG: Tight muons Id are selected in the Bonzai Maker
        bool muPassesIdCut(mu.id  & 1); 

        bool muPassesIsoCut(0);
	///        if (lepSel == "DMu" && mu.iso < 0.25) muPassesIsoCut = 1;  
	//        else if (lepSel == "SMu" && mu.iso < 0.15) muPassesIsoCut = 1;
	muPassesIsoCut = (mu.iso < muIso_);
        bool muPassesTrig(1); //no matching with leptons offtrigger
       // if (lepSel == "DMu" && (mu.trigger & 0x4)) muPassesTrig = 1;       // HLT_Mu17_Mu8 !!!! changed from 0x8 to 0x4
//	const Long64_t dimutrigmask = 19 | 21;
//	const Long64_t mutrigmask = 18 | 22;
//        if (lepSel == "DMu" && (TrigHltDiMu  & dimutrigmask)) muPassesTrig = 1; 
//	else if (lepSel == "SMu" && (mu.trigger & mutrigmask)) muPassesTrig = 1;  // HLT_IsoMu24_eta2p1_v

        //--- veto muons ---
        bool muPassesVetoPtCut(mu.v.Pt() >= 15);
        bool muPassesVetoEtaCut(fabs(mu.v.Eta()) <= 2.4);
        // bool muPassesVetoIdCut(mu.id > 0); // muon Id  // CommentAG: not sure what does it mean

        /// for files obtained form bugra
        if (fileName.Index("Sherpa_Bugra_1_13_UNFOLDING") >= 0 && mu.trigger > 0) muPassesTrig = 1; // Bugra only keeps the double electron trigger !!!!! 

        // select the good muons only
        //cout << i << " pt=" << mu.v.Pt() << " eta=" << mu.v.Eta() << " phi=" << mu.v.Phi() << endl; 
        //cout << "id=" << muPassesIdCut << " iso=" << muPassesIsoCut << " trig=" << muPassesTrig << endl;

        
       // if (muPassesPtCut && muPassesEtaCut && muPassesIdCut && muPassesIsoCut && (!useTriggerCorrection || muPassesTrig || eventTrigger)) {   // CommentAG: this is original line which is replaced by:


        if (muPassesPtCut && muPassesEtaCut && muPassesIdCut && muPassesIsoCut && muPassesTrig) {
            leptons.push_back(mu); 
        }
        // select the veto muons
        else if (lepSel == "SMu" && muPassesVetoPtCut && muPassesVetoEtaCut) {  //CommentAG:  need to check muPassesVetoIdCut! 
            vetoMuons.push_back(mu); 
        }


    }//End of loop over all the muons
}

void ZJets::getElectrons(vector<leptonStruct>& leptons,  vector<leptonStruct>& vetoElectrons)
{
    //--- get the number of Electron candidates from the vector size ---
    unsigned short nTotLeptons(ElEta->size());

    // if we don't really care to match both leptons to trigger
    bool eventTrigger = false;
    for (unsigned short i(0); i < nTotLeptons; i++){
/* // CommentAG
        int whichTrigger(patElecTrig_->at(i));
        if (lepSel == "DE" && (whichTrigger & 0x2)) eventTrigger = true;      // HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v
        else if (lepSel == "SE" && (whichTrigger & 0x1)) eventTrigger = true; // HLT_Ele27_WP80_v
*/
    }
    for (unsigned short i(0); i < nTotLeptons; i++){

        leptonStruct ele(ElPt->at(i), 
			 ElEta->at(i), 
			 ElPhi->at(i), 
			 ElE->at(i), 
			 ElCh->at(i), 
			 ElId->at(i), 
			 ElPfIsoRho->at(i), 
			 ElEtaSc->at(i), 
			 0.); // CommentAG patElecTrig_->at(i)
	
        //--- good electrons ---
        bool elePassesPtCut(ele.v.Pt() >= (lepPtCutMin*0.8));
        bool elePassesEtaCut(fabs(ele.scEta) <= min(1.4442, 0.1*lepEtaCutMax) || (fabs(ele.scEta) >= 1.566 && fabs(ele.scEta) <= 0.1*lepEtaCutMax));
        bool elePassesIdCut(ele.id >= 4); /// 4 is medium ID, 2 is Loose ID
        bool elePassesIsoCut(ele.iso < eIso_);
        //bool elePassesAnyTrig(ele.trigger & 0x2);
	bool elePassesAnyTrig(true); //no matching with lepton from trigger.
        if (fileName.Index("Sherpa_Bugra_1_13_UNFOLDING") > 0) elePassesAnyTrig = true;

        //--- veto electrons ---
        bool elePassesVetoPtCut(ele.v.Pt() >= 15);
        bool elePassesVetoEtaCut(fabs(ele.v.Eta()) <= 2.4);
        bool elePassesVetoIdCut(ele.id >= 2); // ele Id
        bool elePassesVetoIsoCut(ele.iso < 0.25);

        // select the good electrons only
        if (elePassesPtCut && elePassesEtaCut && elePassesIdCut && elePassesIsoCut && (!useTriggerCorrection || elePassesAnyTrig || eventTrigger)){
            leptons.push_back(ele);
        }
        // select the veto electrons
        else if (elePassesVetoPtCut && elePassesVetoEtaCut && elePassesVetoIdCut && elePassesVetoIsoCut) {
            vetoElectrons.push_back(ele);
        }

    }//End of loop over all the electrons
}


ZJets::ZJets(const TString& lepSel_, TString sampleLabel, TString fileName_,
	     float lumi, bool useTriggerCorrection_, int systematics_,
	     int direction_, float xsecUnc_, int lepPtCutMin_, int lepEtaCutMax_, 
	     int jetPtCutMin_, int jetEtaCutMax_,  Long_t maxEvents_,
	     TString outDir_, TString bonzaiDir, int maxFiles): 
    HistoSetZJets(lepSel_), outputDirectory(outDir_),
    fileName(fileName_), lumi_(lumi), useTriggerCorrection(useTriggerCorrection_), 
    systematics(systematics_), direction(direction_), xsecUnc(xsecUnc_), 
    lepPtCutMin(lepPtCutMin_), lepEtaCutMax(lepEtaCutMax_), jetPtCutMin(jetPtCutMin_), jetEtaCutMax(jetEtaCutMax_),
    nMaxEvents(maxEvents_), lepSel(lepSel_), xsec_(0.), sampleLabel_(sampleLabel), maxFiles_(maxFiles),
    triggerMask_(0), triggerMaskSet_(false), muIso_(0), eIso_(0)
{
    //--- Create output directory if necessary ---
    TString command = "mkdir -p " + outputDirectory;
    system(command);

    //--------------------------------------------

    rejectBTagEvents = lepSel.BeginsWith("S"); 
    
    fChain = new TChain("", "");

    TString fullFileName;
    TString baseName;

    canonizeInputFilePath(bonzaiDir, fileName,
			  &fullFileName, &baseName);

    fileName = baseName;

    readCatalog(fullFileName, bonzaiDir, maxFiles, &lumi_, &xsec_,
		fChain, &fBonzaiHeaderChain, &fBonzaiBitFieldsChain);
    
    getMcNorm();

    if(!setTriggerMask()){
	std::cerr << "Failed to set trigger mask. Aborts at " __FILE__ ":" 
		  << __LINE__ << "." << std::endl;
	abort();
    }

    std::cout << "Trigger mask: " << std::hex << "0x" << triggerMask_ << std::dec << "\n";
}

void ZJets::canonizeInputFilePath(const TString& bonzaiDir, const TString& fileName,
				  TString* fullFileName, TString* baseName,
				  TString* ext){
    
    if(fileName.BeginsWith("/")){//absolute path
	*fullFileName = fileName;
    } else{
	*fullFileName = bonzaiDir + "/" + fileName;
    }
    
    if(fullFileName->BeginsWith("/store/")){
	fullFileName->Insert(0, "root://eoscms.cern.ch//eos/cms");
    }
    
    //fileName is expected to contain only the basename without extension
    //remove the .root, .txt extensions:
    if(baseName){
	*baseName = gSystem->BaseName(fileName);	
	if(baseName->EndsWith(".root")){
	    baseName->Remove(baseName->Length()-5, 5);
	    if(ext) *ext = ".root";
	}
	if(baseName->EndsWith(".txt")){
	    baseName->Remove(baseName->Length() - 4, 4);
	    if(ext) *ext = ".txt";
	}
    }
}

void ZJets::readCatalog(const TString& fullFileName, const TString& bonzaiDir, int maxFiles,
			double* pLumi, double* pXsec, TChain* pEventTreeChain,
			TChain* pBonzaiHeaderChain, TChain* pBonzaiBitFieldsChain){
    regex_t xsecLine;
    int rc =  regcomp(&xsecLine,"[#*][[:space:]]*sample xsec[[:space:]:=]\\+\\([[:digit:].eE+-]\\+\\)", 0);
    if(rc){
	char buffer[256];
	regerror(rc, &xsecLine, buffer, sizeof(buffer));
	buffer[sizeof(buffer)-1] = 0;
	std::cerr << "Bug found in " << __FILE__  << ":" << __LINE__ << ": " << buffer << "\n";
    }
    
    regex_t lumiLine;
    rc =  regcomp(&lumiLine,"[#*][[:space:]]*lumi[[:space:]:=]\\+\\([[:digit:].eE+-]\\+\\)", 0);
    if(rc){
    	char buffer[256];
    	regerror(rc, &lumiLine, buffer, sizeof(buffer));
    	buffer[sizeof(buffer)-1] = 0;
    	std::cerr << "Bug found in " << __FILE__  << ":" << __LINE__ << ": " << buffer << "\n";
    }
    
    
    if (isRootFile(fullFileName)){
        TString treePath = fullFileName + "/tupel/EventTree";
	TString bonzaiHeaderPath = fullFileName + "/tupel/BonzaiHeader";
	TString bonzaiBitFieldsPath = fullFileName + "/tupel/BitFields";
        cout << "Loading file: " << fullFileName << endl;
        if(pEventTreeChain) pEventTreeChain->Add(treePath);
	if(pBonzaiHeaderChain) pBonzaiHeaderChain->Add(bonzaiHeaderPath);
	if(pBonzaiBitFieldsChain) pBonzaiBitFieldsChain->Add(bonzaiBitFieldsPath);
    } else {
	int (*closeFunc)(FILE*);
	FILE* f = eosOpen(fullFileName, &closeFunc);
	if(!f){
	    std::cerr << "Failed to  open file " << fullFileName << ".\n";
	} else{
	    std::cout << "Reading input files from catalog file " << fullFileName << "\n";
	    string line; 
	    char* buffer = 0;
	    size_t buffer_size = 0;
	    int ifile = 0;
	    //if maxFiles = 0 only catalog header is read.
	    while (!feof(f)){
		ssize_t len = getline(&buffer, &buffer_size, f);
		if(len  < 0) break;
		char* line = buffer;
		size_t n = len - 1;
		//trim white spaces:
		while(line[0] == ' ' || line[0] == '\t') {++line; --n;}
		while(n >=0 && (line[n] == ' ' || line[n] == '\t' || line[n] == '\r' || line[n] == '\n' )){
		    line[n] = 0; --n;
		}

		regmatch_t pmatch[2];
		if(pXsec && !regexec(&xsecLine, line, sizeof(pmatch)/sizeof(pmatch[0]), pmatch, 0)){
		    line[pmatch[1].rm_eo] = 0;
		    *pXsec = strtod(line + pmatch[1].rm_so, 0);
		    if(*pXsec == 0){
			std::cerr << "Value of parameter 'sample xsec', " << line + pmatch[1].rm_so
				  << " found in file " << fullFileName << " is not valid.\n";
		    }
		}
		else if(pLumi && !regexec(&lumiLine, line, sizeof(pmatch)/sizeof(pmatch[0]), pmatch, 0)){
		    line[pmatch[1].rm_eo] = 0;
		    *pLumi = strtod(line + pmatch[1].rm_so, 0);
		    if(*pLumi == 0){
			std::cerr << "Integrated luminosity parameter value, " << line + pmatch[1].rm_so
				  << " found in file " << fullFileName << " is not valid.\n";
		    }
		}
		
		//skip empty lines,  comment lines and metadata lines:
		if (line[0] == 0 || line[0] == '#' || line[0] == '*') continue;
		
		if(maxFiles == 0 || (pEventTreeChain == 0 && pEventTreeChain ==0)) break;
		
		//keep content of first column only:
		char* p = line;
		while(*p != 0 && *p != ' ' && *p != '\t' && *p != '\r' && *p != '\n') ++p;
		*p = 0;

		//following check is done having read the header
		//such that maxFiles = 0 can be used to read only
		//the header.
		if(maxFiles >= 0 && ifile >= maxFiles) break;
		
		TString treePath = TString(line) + "/tupel";
		if(treePath[0]!='/'){
		    treePath.Insert(0, TString(bonzaiDir) + "/");
		}
		if(treePath.BeginsWith("/store/")){
		    treePath.Insert(0, "root://eoscms.cern.ch//eos/cms");
		}
		TString bonzaiHeaderPath = treePath + "/BonzaiHeader";
		TString bonzaiBitFieldsPath = treePath + "/BitFields";
		treePath += "/EventTree";
		std::cout << "Adding path " << treePath << " to the tree chain.\n";
		if(pEventTreeChain) pEventTreeChain->Add(treePath);
		if(pBonzaiHeaderChain) pBonzaiHeaderChain->Add(bonzaiHeaderPath);
		if(pBonzaiBitFieldsChain) pBonzaiBitFieldsChain->Add(bonzaiBitFieldsPath);
		++ifile;
	    }//next line
	    std::cout << "Closing catalog file " << fullFileName << "\n";
	    if(buffer) free(buffer);
	    closeFunc(f);
	} //file opening succeeded
    }//is root file
    regfree(&xsecLine);
    regfree(&lumiLine);
}

//#define weight_bug //to read bonzai version 2.

void ZJets::getMcNorm(){
    Int_t InEvtCount = 0;
//#ifdef weight_bug
//    std::vector<Double_t> InEvtWeightSums(1,0);
//    std::vector<Double_t> EvtWeightSums(1,0);
//    fBonzaiHeaderChain.SetBranchAddress("InEvtWeightSums", &InEvtWeightSums[0]);
//    fBonzaiHeaderChain.SetBranchAddress("EvtWeightSums", &EvtWeightSums[0]);
//#else
    std::vector<Double_t>* InEvtWeightSums  = 0;
    std::vector<Double_t>* EvtWeightSums = 0;
    fBonzaiHeaderChain.SetBranchAddress("InEvtWeightSums", &InEvtWeightSums);
    fBonzaiHeaderChain.SetBranchAddress("EvtWeightSums", &EvtWeightSums);
    //#endif
    //for(Long64_t i = 0; i < nfiles; ++ i){
    int nheaders = fBonzaiHeaderChain.GetEntries(); //can be several in case files were merged with haddd
    for(int ientry = 0; ientry < nheaders; ++ientry){
	fBonzaiHeaderChain.GetEntry(ientry);
	if(ientry == 0){
	    InEvtWeightSums_ = std::vector<Double_t>(InEvtWeightSums->size(), 0);
	    EvtWeightSums_ = std::vector<Double_t>(EvtWeightSums->size(), 0);
	    if(InEvtWeightSums->size() != EvtWeightSums->size()){
		std::cerr << "InEvtWeightSums and EvtWeightSums branches "
		    "of input BonzaiHeader tree have different size ("
		    "resp. " << InEvtWeightSums->size() << " and "
			  << EvtWeightSums->size() << "). Aborts at " __FILE__ ":"
			  << __LINE__ << ".\n";
		abort();
	    }
	} 
	if(InEvtWeightSums->size() != InEvtWeightSums_.size()){
	    std::cerr << "Inconsistency in number of elements of "
		      << " InEvtWeightSums branch of input files! Aborts at " __FILE__ ":" 
		      << __LINE__ << ".\n";
	    abort();
	}
	if(EvtWeightSums->size() != EvtWeightSums_.size()){
	    std::cerr << "Inconsistency in number of elements of EvtWeightSums "
		"branch of input files! Aborts at " __FILE__ ":" << __LINE__ << ".\n";
	    abort();
	}
	for(size_t i = 0; i < InEvtWeightSums_.size(); ++i){
	    InEvtWeightSums_[i] += (*InEvtWeightSums)[i];
	}
	for(size_t i = 0; i < EvtWeightSums_.size(); ++i){
	    EvtWeightSums_[i] += (*EvtWeightSums)[i];
	}
	InEvtCount_ += InEvtCount;
    }
    //}

    EvtCount_ = fChain->GetEntries();

    if(InEvtWeightSums_.size() > 0){
	std::cerr << "InEvtWeightSums_[0] = " <<	InEvtWeightSums_[0] << "\n";
    }

    if(EvtWeightSums_.size() == 0 || InEvtWeightSums_.size() == 0 || InEvtWeightSums_[0] == 0 ){
	if(InEvtCount_){
	    skimAccep_ = std::vector<double>(1, EvtCount_/InEvtCount_);
	} else{
	    std::cout << "Warning: InEvtCount is equal to 0. Event yield normalization might be wrong!" << std::endl;
	}
    } else{
	skimAccep_ = std::vector<double>(InEvtWeightSums_.size());
	for(unsigned i = 0; i < InEvtWeightSums_.size() && i < EvtWeightSums_.size(); ++i){
	    skimAccep_[i] = EvtWeightSums_[i]/InEvtWeightSums_[i];
	}
    }
    //    delete InEvtWeightSums;
    //    delete EvtWeightSums;
} 

ZJets::~ZJets(){
    if (!fChain) return;
    delete fChain->GetCurrentFile();
}

void ZJets::CreateOutputFileName(const TString& pdfSet, int pdfMember, double muR, double muF, int iJob){
    outputFileName =  CreateOutputFileName(pdfSet, pdfMember, muR, muF, iJob,
					   lepSel, sampleLabel_, useTriggerCorrection, systematics, direction,
					   jetPtCutMin, jetEtaCutMax, outputDirectory);
}

string ZJets::CreateOutputFileName(const TString& pdfSet, int pdfMember, double muR, double muF, int iJob,
				   const TString& lepSel, const TString& sampleLabel_,
				   bool useTriggerCorrection, int systematics, int direction,
				   int jetPtCutMin, int jetEtaCutMax, TString outputDirectory)
{
    ostringstream result;
    //    result << outputDirectory << fileName;
    result << outputDirectory << lepSel << "_" << "13TeV" << "_" << sampleLabel_;
    result << "_TrigCorr_" << useTriggerCorrection;
    result << "_Syst_" << systematics;
    if (direction == 1) result << "_Up";
    else if (direction == -1) result << "_Down";
    result << "_JetPtMin_" << jetPtCutMin;
    result << "_JetEtaMax_" << jetEtaCutMax;
    if(iJob > 0) result << "_" << iJob;

    if (muR != 0 && muF != 0 && muR != 1 && muF != 1) result << "_muR_" << muR << "_muF_" << muF;
    if (pdfSet != "") result << "_PDF_" << pdfSet << "_" << pdfMember;
    if (pdfSet == "" && pdfMember != -1) result << "_NNPDF_" << pdfMember;
    //--- Add your test names here ---
    //result << "_NoPUCut";
    //result << "_LooseID";

    result << ".root";
    return result.str();
}

Int_t ZJets::GetEntry(Long64_t entry){
    // Read contents of entry.
    if (!fChain) return 0;
    return fChain->GetEntry(entry);
}

Long64_t ZJets::LoadTree(Long64_t entry){
    // Set the environment to read one entry
    if (!fChain) return -5;
    Long64_t centry = fChain->LoadTree(entry);
    if (centry < 0) return centry;
    if (fChain->GetTreeNumber() != fCurrent) {
        fCurrent = fChain->GetTreeNumber();
        Notify();
    }
    return centry;
}

void ZJets::Init(bool hasRecoInfo, bool hasGenInfo){
    // The Init() function is called when the selector needs to initialize
    // a new tree or chain. Typically here the branch addresses and branch
    // pointers of the tree will be set.
    // It is normally not necessary to make changes to the generated
    // code, but the routine can be extended by the user if needed.
    // Init() will be called many times when running on PROOF
    // (once per file to be processed).

    // Set object pointer
 //  pdfInfo_ = 0;
    GLepBarePt = 0;
    GLepBareEta = 0;
    GLepBarePhi = 0;
    GLepBareE = 0;
   // genLepQ_ = 0;
    GLepBareId = 0;
    GLepBareSt = 0;
    GLepBarePrompt = 0;
    GLepClosePhotPt = 0;
    GLepClosePhotEta = 0;
    GLepClosePhotPhi = 0;
    GJetAk04Pt = 0;
    GJetAk04Eta = 0;
    GJetAk04Phi = 0;
    GJetAk04E = 0;

    ElPt = 0;
    ElEta = 0;
    ElPhi = 0;
    ElE = 0;
    ElCh = 0;
    ElId = 0;
   // patElecTrig_ = 0;
    ElPfIsoRho = 0;
    ElEtaSc = 0;

    MuPt = 0;
    MuEta = 0;
    MuPhi = 0;
    MuE = 0;
    MuIdTight = 0;
    MuCh = 0;
    MuId = 0;
   // patMuonCombId_Double = 0;
   // patMuonTrig_ = 0;
    MuPfIso = 0;

    JetAk04E = 0;
    JetAk04Pt = 0;
    JetAk04Eta = 0;
    JetAk04Phi = 0;
    JetAk04Id = 0;
    JetAk04PuMva = 0;
    JetAk04BDiscCisvV2 = 0;
    JetAk04PartFlav = 0;

    METPt = 0;
    METPx = 0;
    METPy = 0;
    METsig = 0;
    //mcSherpaWeights_ = 0; 
    //weight_amcNLO_ = 0; 
    //weight_amcNLO_sum_ = 0; 
    EvtWeights = 0;
    
    TrigHlt = 0;
    TrigHltPhot = 0;
    TrigHltMu = 0;
    TrigHltDiMu = 0;
    TrigHltEl = 0;
    TrigHltDiEl = 0;

    // Set branch addresses and branch pointers
    fCurrent = -1;
    fChain->SetMakeClass(1);
    fChain->SetBranchAddress("EvtIsRealData", &EvtIsRealData, &b_EvtIsRealData);
    //    if (fileName.Index("Data") < 0) {
    fChain->SetBranchAddress("EvtPuCntTruth", &EvtPuCntTruth, &b_EvtPuCntTruth);
    fChain->SetBranchAddress("EvtPuCnt", &EvtPuCnt, &b_EvtPuCnt);
    // }
    if (hasRecoInfo){
        fChain->SetBranchAddress("EvtVtxCnt", &EvtVtxCnt, &b_EvtVtxCnt);
        fChain->SetBranchAddress("EvtRunNum", &EvtRunNum, &b_EvtRunNum); 
        fChain->SetBranchAddress("EvtNum", &EvtNum, &b_EvtNum); 
        fChain->SetBranchAddress("JetAk04E", &JetAk04E, &b_JetAk04E);
        fChain->SetBranchAddress("JetAk04Pt", &JetAk04Pt, &b_JetAk04Pt);
        fChain->SetBranchAddress("JetAk04Eta", &JetAk04Eta, &b_JetAk04Eta);
        fChain->SetBranchAddress("JetAk04Phi", &JetAk04Phi, &b_JetAk04Phi);
        fChain->SetBranchAddress("JetAk04Id", &JetAk04Id, &b_JetAk04Id);
        fChain->SetBranchAddress("JetAk04PuMva", &JetAk04PuMva, &b_JetAk04PuMva);
        fChain->SetBranchAddress("JetAk04BDiscCisvV2", &JetAk04BDiscCisvV2, &b_JetAk04BDiscCisvV2);
        //fChain->SetBranchAddress("JetAk04PartFlav", &JetAk04PartFlav, &b_JetAk04PartFlav);
        fChain->SetBranchAddress("METPt", &METPt, &b_METPt);
	fChain->SetBranchAddress("METPx", &METPx, &b_METPx);
	fChain->SetBranchAddress("METPy", &METPy, &b_METPy);
        //fChain->SetBranchAddress("METsig", &METsig, &b_METsig); // not used
        fChain->SetBranchAddress("TrigHlt", &TrigHlt, &b_TrigHlt);
        fChain->SetBranchAddress("TrigHltMu", &TrigHltMu, &b_TrigHltMu);
        fChain->SetBranchAddress("TrigHltDiMu", &TrigHltDiMu, &b_TrigHltDiMu);
        fChain->SetBranchAddress("TrigHltEl",   &TrigHltEl,   &b_TrigHltEl);
        fChain->SetBranchAddress("TrigHltDiEl", &TrigHltDiEl, &b_TrigHltDiEl);

        if (lepSel == "DE" || lepSel == "SE"){
            fChain->SetBranchAddress("ElPt", &ElPt, &b_ElPt);
            fChain->SetBranchAddress("ElEta", &ElEta, &b_ElEta);
            fChain->SetBranchAddress("ElPhi", &ElPhi, &b_ElPhi);
            fChain->SetBranchAddress("ElE", &ElE, &b_ElE);
            fChain->SetBranchAddress("ElCh", &ElCh, &b_ElCh);
            fChain->SetBranchAddress("ElId", &ElId, &b_ElId);
           // fChain->SetBranchAddress("patElecTrig_", &patElecTrig_, &b_patElecTrig_);
            fChain->SetBranchAddress("ElPfIsoRho", &ElPfIsoRho, &b_ElPfIsoRho); 
            fChain->SetBranchAddress("ElEtaSc", &ElEtaSc, &b_ElEtaSc);
        }
        if (lepSel == "DMu" || lepSel == "SMu"){
            fChain->SetBranchAddress("MuPt", &MuPt, &b_MuPt);
            fChain->SetBranchAddress("MuEta", &MuEta, &b_MuEta);
            fChain->SetBranchAddress("MuPhi", &MuPhi, &b_MuPhi);
            fChain->SetBranchAddress("MuE", &MuE, &b_MuE);
            fChain->SetBranchAddress("MuCh", &MuCh, &b_MuCh);
            fChain->SetBranchAddress("MuIdTight", &MuIdTight, &b_MuIdTight);
            if(fileName.Index("mcatnlo") >= 0 || fileName.Index("MG-MLM") >= 0) {
                // CommentAG: before: patMuonCombId_Int
                fChain->SetBranchAddress("MuId", &MuId, &b_MuId);
            }
          //  else {  // CommentAG: check this
          //      fChain->SetBranchAddress("patMuonCombId_", &patMuonCombId_Double, &b_patMuonCombId_Double);
          //  }
         //   fChain->SetBranchAddress("patMuonTrig_", &patMuonTrig_, &b_patMuonTrig_);
            fChain->SetBranchAddress("MuPfIso", &MuPfIso, &b_MuPfIso);
        }
    }
    if (hasGenInfo){
        fChain->SetBranchAddress("GLepBarePt", &GLepBarePt, &b_GLepBarePt);
        fChain->SetBranchAddress("GLepBareEta", &GLepBareEta, &b_GLepBareEta);
        fChain->SetBranchAddress("GLepBarePhi", &GLepBarePhi, &b_GLepBarePhi);
        fChain->SetBranchAddress("GLepBareE", &GLepBareE, &b_GLepBareE);
       // fChain->SetBranchAddress("genLepQ_", &genLepQ_, &b_genLepQ_);
        fChain->SetBranchAddress("GLepBareId", &GLepBareId, &b_GLepBareId);
        fChain->SetBranchAddress("GLepBareSt", &GLepBareSt, &b_GLepBareSt);
        fChain->SetBranchAddress("GJetAk04Pt", &GJetAk04Pt, &b_GJetAk04Pt);
        fChain->SetBranchAddress("GJetAk04Eta", &GJetAk04Eta, &b_GJetAk04Eta);
        fChain->SetBranchAddress("GJetAk04Phi", &GJetAk04Phi, &b_GJetAk04Phi);
        fChain->SetBranchAddress("GJetAk04E", &GJetAk04E, &b_GJetAk04E);
        fChain->SetBranchAddress("GLepClosePhotPt", &GLepClosePhotPt, &b_GLepClosePhotPt);
        fChain->SetBranchAddress("GLepClosePhotEta", &GLepClosePhotEta, &b_GLepClosePhotEta);
        fChain->SetBranchAddress("GLepClosePhotPhi", &GLepClosePhotPhi, &b_GLepClosePhotPhi);
	if(fChain->GetBranch("GLepBarePrompt")){
	    fChain->SetBranchAddress("GLepBarePrompt", &GLepBarePrompt, &b_GLepClosePhotPhi);
	}
        if (fileName.Index("MIX") >= 0 && fileName.Index("UNFOLDING") >= 0) {
           // fChain->SetBranchAddress("pdfInfo_", &pdfInfo_, &b_pdfInfo_);
            fChain->SetBranchAddress("GNup", &GNup, &b_GNup);
        }
	//  if (fileName.Index("Sherpa") >= 0 && fileName.Index("UNFOLDING") >= 0) {
	//      fChain->SetBranchAddress("mcSherpaWeights_", &mcSherpaWeights_, &b_mcSherpaWeights_);
	//  }
        //if (fileName.Index("mcatnlo") >= 0) {
        //    fChain->SetBranchAddress("EvtWeights", &EvtWeights, &b_EvtWeights);
        //}
        //if(fileName.Index("Sherpa2") >= 0){
        //    fChain->SetBranchAddress("EvtWeights", &EvtWeights, &b_EvtWeights);
        //}
        //if((fileName.Index("Sherpa") >= 0 && fileName.Index("UNFOLDING") >= 0) || fileName.Index("mcatnlo") >= 0) {
        //    fChain->SetBranchAddress("weight_amcNLO_", &weight_amcNLO_, &b_weight_amcNLO_);
        //    fChain->SetBranchAddress("weight_amcNLO_sum_", &weight_amcNLO_sum_, &b_weight_amcNLO_sum_);
        //}
    }

    fChain->SetBranchAddress("EvtWeights", &EvtWeights, &b_EvtWeights);

    Notify();
    cout << "Branches are properly initialized." << endl;
}

Bool_t ZJets::Notify(){
    // The Notify() function is called when a new file is opened. This
    // can be either for a new TTree in a TChain or when when a new TTree
    // is started when using PROOF. It is normally not necessary to make changes
    // to the generated code, but the routine can be extended by the
    // user if needed. The return value is currently not used.

    return kTRUE;
}

void ZJets::Show(Long64_t entry){
    // Print contents of entry.
    // If entry is not specified, print current entry
    if (!fChain) return;
    fChain->Show(entry);
}

Int_t ZJets::Cut(Long64_t entry){
    // This function may be called from Loop.
    // returns  1 if entry is accepted.
    // returns -1 otherwise.
    printf("entry %lld", entry);
    return 1;
}

bool ZJets::setTriggerMask(){
    if (triggerMaskSet_) return triggerMask_;
    std::vector<std::string> triggers = cfg.getVS("triggers");

    if(triggers.size()){
	std::cout << "An or of the following trigger paths will be used:";
	for(unsigned i = 0; i < triggers.size(); ++i){
	std::cout << " " << triggers[i];
	}
	std::cout << "\n";
    } else{
	std::cout << "No trigger requirement will be applied.\n\n";
    }
    
    TString branchName;
    std::vector<std::string>* TrigHlt = 0;

    if(lepSel == "DMu") { branchName = "TrigHltDiMu"; ourTrig_ = &TrigHltDiMu;  }
    else if(lepSel == "DE"){ branchName = "TrigHltDiEl"; ourTrig_ = &TrigHltDiEl; }
    else if(lepSel == "SMu"){ branchName = "TrigHltMu"; ourTrig_ = &TrigHltMu; }
    else if(lepSel == "SE"){ branchName = "TrigHltEl"; ourTrig_ = &TrigHltEl; }
    else{
	std::cerr << __FILE__ << ":" << __LINE__ << ". "
		  << "lepSel value, '" << lepSel
		  << "', was  not recognzed. We cannot set the trigger bits.\n\n";
	return false;
    }

    if(triggers.size()==0){
	triggerMask_ = (ULong64_t) -1; //all bits set.
	return true;
    }
    
    if(fBonzaiBitFieldsChain.GetBranch(branchName)){
	fBonzaiBitFieldsChain.SetBranchAddress(branchName, &TrigHlt);
    } else{
	std::cerr << "Cannot set the trigger bits, because the Branch "
		  << branchName << " was not found in the tree BitFields.\n\n";
	return false;
    }

    //FIXME: check all files?
    if(fBonzaiBitFieldsChain.GetEntry(0)<=0){
	std::cerr << "Failed to read BitFields tree. Is the tree empty? "
		  << "Cannot set the trigger bits.\n\n";
	return false;	
    }

    std::cout << "Available triggers for " << branchName << ":";
    for(unsigned i = 0; i < TrigHlt->size(); ++i){
	if((*TrigHlt)[i].size()) std::cout << " " << (*TrigHlt)[i];
    }
    std::cout << "\n";
    
    bool rc = true;
    triggerMask_ = 0;
    for(unsigned i = 0; i < triggers.size(); ++i){
	int found = 0;
	for(unsigned bit = 0; bit < TrigHlt->size(); ++bit){
	    if(triggers[i] == (*TrigHlt)[bit]){
		triggerMask_ |= (1 <<bit);
		++found;
	    }
	}
	if(found==0){
	    std::cerr << "Trigger " << triggers[i] << " was not found in the input sample!\n\n";
	    rc =false;
	}
	if(found > 1){
	    std::cerr << "Trigger " << triggers[i] << " is assigned to several bits!\n\n";
	    rc =true;
	}
    }
    return rc;
}

//bool ZJets::compTrigger(const char* a, const char* bv) const{
//  int i = 0;
//  for(;a[i]!=0 && bv[i]!=0; ++i){
//    if(a[i]!= bv[i]) return false;
//  }
//  if(a[i]) return false;
//  if(bv[i]==0) return true;
//  if(bv[i] != '_') return false;
//  if(bv[++i]!='v') return false;
//  for(;;){
//    if(bv[++i]==0) return true;
//    if(!isdigit(bv[i])) return false;
//  }
//  return true;
//};
