//-*- c-basic-offset: 8; -*-
#ifndef ZJets_h
#define ZJets_h

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TLorentzVector.h>
#include <RooUnfoldResponse.h>
#include <TDatime.h>
#include <TMath.h>
#include <TRandom3.h>

// Header file for the classes stored in the TTree if any.
#include "functions.h"
#include "getFilesAndHistogramsZJets.h"
#include "HistoSetZJets.h"
#include "rochcor2015.h"


using namespace std;


class ZJets: public HistoSetZJets {
    public :
        bool doRochester;
        rochcor2015 *rmcor;
        //TTree          *fChain;   //!pointer to the analyzed TTree or TChain
        TChain          *fChain;   //!pointer to the analyzed TTree or TChain
	TChain          fBonzaiHeaderChain;
	TChain          fBonzaiBitFieldsChain;
        //TTree          *tree;
        Int_t           fCurrent; //!current Tree number in a TChain

        // Declaration of leaf types
   Int_t           EvtIsRealData;
   UInt_t          EvtNum;
   UInt_t          EvtRunNum;
   Int_t           EvtLumiNum;
   Int_t           EvtBxNum;
   Float_t         EvtVtxCnt;
   Int_t           EvtPuCnt;
   Int_t           EvtPuCntTruth;
   vector<double>  *EvtWeights;
   Float_t         EvtFastJetRho;
   UInt_t          TrigHlt;
   ULong64_t       TrigHltPhot;
   ULong64_t       TrigHltMu;
   ULong64_t       TrigHltDiMu;
   ULong64_t       TrigHltEl;
   ULong64_t       TrigHltDiEl;
   ULong64_t*      ourTrig_;
   vector<float>   *METPt;
   vector<float>   *METPx;
   vector<float>   *METPy;
   vector<float>   *METPz;
   vector<float>   *METE;
   vector<float>   *METsigx2;
   vector<float>   *METsigxy;
   vector<float>   *METsigy2;
   vector<float>   *METsig;
   vector<float>   *GLepDr01Pt;
   vector<float>   *GLepDr01Eta;
   vector<float>   *GLepDr01Phi;
   vector<float>   *GLepDr01E;
   vector<int>     *GLepDr01Id;
   vector<int>     *GLepDr01St;
   vector<int>     *GLepDr01MomId;
   vector<float>   *GLepBarePt;
   vector<float>   *GLepBareEta;
   vector<float>   *GLepBarePhi;
   vector<float>   *GLepBareE;
   vector<int>     *GLepBareId;
   vector<bool>    *GLepBarePrompt;
   vector<int>     *GLepBareSt;
   vector<int>     *GLepBareMomId;
   vector<float>   *GLepSt3Pt;
   vector<float>   *GLepSt3Eta;
   vector<float>   *GLepSt3Phi;
   vector<float>   *GLepSt3E;
   vector<int>     *GLepSt3Id;
   vector<int>     *GLepSt3St;
   vector<int>     *GLepSt3Mother0Id;
   vector<int>     *GLepSt3MotherCnt;
   vector<float>   *GLepClosePhotPt;
   vector<float>   *GLepClosePhotEta;
   vector<float>   *GLepClosePhotPhi;
   vector<float>   *GLepClosePhotE;
   vector<int>     *GLepClosePhotId;
   vector<int>     *GLepClosePhotMother0Id;
   vector<int>     *GLepClosePhotMotherCnt;
   vector<int>     *GLepClosePhotSt;
   vector<float>   *GJetAk04Pt;
   vector<float>   *GJetAk04Eta;
   vector<float>   *GJetAk04Phi;
   vector<float>   *GJetAk04E;
   vector<float>   *GJetAk04ChFrac;
   vector<int>     *GJetAk04ConstCnt;
   vector<int>     *GJetAk04ConstId;
   vector<float>   *GJetAk04ConstPt;
   vector<float>   *GJetAk04ConstEta;
   vector<float>   *GJetAk04ConstPhi;
   vector<float>   *GJetAk04ConstE;
   vector<float>   *GPdfId1;
   vector<float>   *GPdfId2;
   vector<float>   *GPdfx1;
   vector<float>   *GPdfx2;
   vector<float>   *GPdfScale;
   Float_t         GBinningValue;
   Int_t           GNup;
   vector<float>   *MuPt;
   vector<float>   *MuEta;
   vector<float>   *MuPhi;
   vector<float>   *MuE;
   vector<int>     *MuId;
   vector<unsigned int> *MuIdTight;
   vector<float>   *MuCh;
   vector<float>   *MuVtxZ;
   vector<float>   *MuDxy;
   vector<float>   *MuIsoRho;
   vector<float>   *MuPfIso;
   vector<float>   *MuType;
   vector<float>   *MuIsoTkIsoAbs;
   vector<float>   *MuIsoTkIsoRel;
   vector<float>   *MuIsoCalAbs;
   vector<float>   *MuIsoCombRel;
   vector<float>   *MuTkNormChi2;
   vector<float>   *MuTkHitCnt;
   vector<float>   *MuMatchedStationCnt;
   vector<float>   *MuDz;
   vector<float>   *MuPixelHitCnt;
   vector<float>   *MuTkLayerCnt;
   vector<float>   *MuPfIsoChHad;
   vector<float>   *MuPfIsoNeutralHad;
   vector<float>   *MuPfIsoRawRel;
   vector<float>   *MuHltMatch;
   vector<float>   *ElPt;
   vector<float>   *ElEta;
   vector<float>   *ElEtaSc;
   vector<float>   *ElPhi;
   vector<float>   *ElE;
   vector<unsigned int> *ElId;
   vector<float>   *ElCh;
   vector<float>   *ElMvaTrig;
   vector<float>   *ElMvaNonTrig;
   vector<float>   *ElMvaPresel;
   vector<float>   *ElDEtaTkScAtVtx;
   vector<float>   *ElDPhiTkScAtVtx;
   vector<float>   *ElHoE;
   vector<float>   *ElSigmaIetaIeta;
   vector<float>   *ElSigmaIetaIetaFull5x5;
   vector<float>   *ElEinvMinusPinv;
   vector<float>   *ElD0;
   vector<float>   *ElDz;
   vector<int>     *ElExpectedMissingInnerHitCnt;
   vector<int>     *ElPassConvVeto;
   vector<float>   *ElHltMatch;
   vector<float>   *ElPfIsoChHad;
   vector<float>   *ElPfIsoNeutralHad;
   vector<float>   *ElPfIsoIso;
   vector<float>   *ElPfIsoPuChHad;
   vector<float>   *ElPfIsoRaw;
   vector<float>   *ElPfIsoDbeta;
   vector<float>   *ElPfIsoRho;
   vector<float>   *ElAEff;
   vector<float>   *charged;
   vector<float>   *photon;
   vector<float>   *neutral;
   vector<float>   *charged_Tom;
   vector<float>   *photon_Tom;
   vector<float>   *neutral_Tom;
   vector<float>   *PhotPt;
   vector<float>   *PhotEta;
   vector<float>   *PhotPhi;
   vector<float>   *PhotIsoEcal;
   vector<float>   *PhotIsoHcal;
   vector<float>   *PhotIsoTk;
   vector<float>   *PhotPfIsoChHad;
   vector<float>   *PhotPfIsoNeutralHad;
   vector<float>   *PhotPfIsoPhot;
   vector<float>   *PhotPfIsoPuChHad;
   vector<float>   *PhotPfIsoEcalClus;
   vector<float>   *PhotPfIsoHcalClus;
   vector<float>   *PhotE3x3;
   vector<float>   *PhotSigmaIetaIeta;
   vector<float>   *PhotHoE;
   vector<unsigned int> *PhotId;
   vector<bool>    *PhotHasPixelSeed;
   vector<float>   *JetAk04Pt;
   vector<float>   *JetAk04Eta;
   vector<float>   *JetAk04Phi;
   vector<float>   *JetAk04E;
   vector<float>   *JetAk04Id;
   vector<bool>    *JetAk04PuId;
   vector<float>   *JetAk04PuMva;
   vector<float>   *JetAk04RawPt;
   vector<float>   *JetAk04RawE;
   vector<float>   *JetAk04HfHadE;
   vector<float>   *JetAk04HfEmE;
   vector<float>   *JetAk04ChHadFrac;
   vector<float>   *JetAk04NeutralHadAndHfFrac;
   vector<float>   *JetAk04ChEmFrac;
   vector<float>   *JetAk04NeutralEmFrac;
   vector<float>   *JetAk04ChMult;
   vector<float>   *JetAk04ConstCnt;
   vector<float>   *JetAk04JetBeta;
   vector<float>   *JetAk04JetBetaClassic;
   vector<float>   *JetAk04JetBetaStar;
   vector<float>   *JetAk04JetBetaStarClassic;
   vector<float>   *JetAk04BTagCsv;
   vector<float>   *JetAk04BTagCsvV1;
   vector<float>   *JetAk04BTagCsvSLV1;
   vector<float>   *JetAk04BDiscCisvV2;
   vector<float>   *JetAk04BDiscJp;
   vector<float>   *JetAk04BDiscBjp;
   vector<float>   *JetAk04BDiscTche;
   vector<float>   *JetAk04BDiscTchp;
   vector<float>   *JetAk04BDiscSsvhe;
   vector<float>   *JetAk04BDiscSsvhp;
   vector<float>   *JetAk04PartFlav;
   vector<float>   *JetAk04JecUncUp;
   vector<float>   *JetAk04JecUncDwn;
   vector<int>     *JetAk04ConstId;
   vector<float>   *JetAk04ConstPt;
   vector<float>   *JetAk04ConstEta;
   vector<float>   *JetAk04ConstPhi;
   vector<float>   *JetAk04ConstE;
   vector<int>     *JetAk04GenJet;
        //Double_t mcEveWeight_;

        // List of branches
   TBranch        *b_EvtIsRealData;   //!
   TBranch        *b_EvtNum;   //!
   TBranch        *b_EvtRunNum;   //!
   TBranch        *b_EvtLumiNum;   //!
   TBranch        *b_EvtBxNum;   //!
   TBranch        *b_EvtVtxCnt;   //!
   TBranch        *b_EvtPuCnt;   //!
   TBranch        *b_EvtPuCntTruth;   //!
   TBranch        *b_EvtWeights;   //!
   TBranch        *b_EvtFastJetRho;   //!
   TBranch        *b_TrigHlt;   //!
   TBranch        *b_TrigHltPhot;   //!
   TBranch        *b_TrigHltMu;   //!
   TBranch        *b_TrigHltDiMu;   //!
   TBranch        *b_TrigHltEl;   //!
   TBranch        *b_TrigHltDiEl;   //!
   TBranch        *b_METPt;   //!
   TBranch        *b_METPx;   //!
   TBranch        *b_METPy;   //!
   TBranch        *b_METPz;   //!
   TBranch        *b_METE;   //!
   TBranch        *b_METsigx2;   //!
   TBranch        *b_METsigxy;   //!
   TBranch        *b_METsigy2;   //!
   TBranch        *b_METsig;   //!
   TBranch        *b_GLepDr01Pt;   //!
   TBranch        *b_GLepDr01Eta;   //!
   TBranch        *b_GLepDr01Phi;   //!
   TBranch        *b_GLepDr01E;   //!
   TBranch        *b_GLepDr01Id;   //!
   TBranch        *b_GLepDr01St;   //!
   TBranch        *b_GLepDr01MomId;   //!
   TBranch        *b_GLepBarePt;   //!
   TBranch        *b_GLepBareEta;   //!
   TBranch        *b_GLepBarePhi;   //!
   TBranch        *b_GLepBareE;   //!
   TBranch        *b_GLepBareId;   //!
   TBranch        *b_GLepBarePrompt;   //!
   TBranch        *b_GLepBareSt;   //!
   TBranch        *b_GLepBareMomId;   //!
   TBranch        *b_GLepSt3Pt;   //!
   TBranch        *b_GLepSt3Eta;   //!
   TBranch        *b_GLepSt3Phi;   //!
   TBranch        *b_GLepSt3E;   //!
   TBranch        *b_GLepSt3Id;   //!
   TBranch        *b_GLepSt3St;   //!
   TBranch        *b_GLepSt3Mother0Id;   //!
   TBranch        *b_GLepSt3MotherCnt;   //!
   TBranch        *b_GLepClosePhotPt;   //!
   TBranch        *b_GLepClosePhotEta;   //!
   TBranch        *b_GLepClosePhotPhi;   //!
   TBranch        *b_GLepClosePhotE;   //!
   TBranch        *b_GLepClosePhotId;   //!
   TBranch        *b_GLepClosePhotMother0Id;   //!
   TBranch        *b_GLepClosePhotMotherCnt;   //!
   TBranch        *b_GLepClosePhotSt;   //!
   TBranch        *b_GJetAk04Pt;   //!
   TBranch        *b_GJetAk04Eta;   //!
   TBranch        *b_GJetAk04Phi;   //!
   TBranch        *b_GJetAk04E;   //!
   TBranch        *b_GJetAk04ChFrac;   //!
   TBranch        *b_GJetAk04ConstCnt;   //!
   TBranch        *b_GJetAk04ConstId;   //!
   TBranch        *b_GJetAk04ConstPt;   //!
   TBranch        *b_GJetAk04ConstEta;   //!
   TBranch        *b_GJetAk04ConstPhi;   //!
   TBranch        *b_GJetAk04ConstE;   //!
   TBranch        *b_GPdfId1;   //!
   TBranch        *b_GPdfId2;   //!
   TBranch        *b_GPdfx1;   //!
   TBranch        *b_GPdfx2;   //!
   TBranch        *b_GPdfScale;   //!
   TBranch        *b_GBinningValue;   //!
   TBranch        *b_GNup;   //!
   TBranch        *b_MuPt;   //!
   TBranch        *b_MuEta;   //!
   TBranch        *b_MuPhi;   //!
   TBranch        *b_MuE;   //!
   TBranch        *b_MuId;   //!
   TBranch        *b_MuIdTight;   //!
   TBranch        *b_MuCh;   //!
   TBranch        *b_MuVtxZ;   //!
   TBranch        *b_MuDxy;   //!
   TBranch        *b_MuIsoRho;   //!
   TBranch        *b_MuPfIso;   //!
   TBranch        *b_MuType;   //!
   TBranch        *b_MuIsoTkIsoAbs;   //!
   TBranch        *b_MuIsoTkIsoRel;   //!
   TBranch        *b_MuIsoCalAbs;   //!
   TBranch        *b_MuIsoCombRel;   //!
   TBranch        *b_MuTkNormChi2;   //!
   TBranch        *b_MuTkHitCnt;   //!
   TBranch        *b_MuMatchedStationCnt;   //!
   TBranch        *b_MuDz;   //!
   TBranch        *b_MuPixelHitCnt;   //!
   TBranch        *b_MuTkLayerCnt;   //!
   TBranch        *b_MuPfIsoChHad;   //!
   TBranch        *b_MuPfIsoNeutralHad;   //!
   TBranch        *b_MuPfIsoRawRel;   //!
   TBranch        *b_MuHltMatch;   //!
   TBranch        *b_ElPt;   //!
   TBranch        *b_ElEta;   //!
   TBranch        *b_ElEtaSc;   //!
   TBranch        *b_ElPhi;   //!
   TBranch        *b_ElE;   //!
   TBranch        *b_ElId;   //!
   TBranch        *b_ElCh;   //!
   TBranch        *b_ElMvaTrig;   //!
   TBranch        *b_ElMvaNonTrig;   //!
   TBranch        *b_ElMvaPresel;   //!
   TBranch        *b_ElDEtaTkScAtVtx;   //!
   TBranch        *b_ElDPhiTkScAtVtx;   //!
   TBranch        *b_ElHoE;   //!
   TBranch        *b_ElSigmaIetaIeta;   //!
   TBranch        *b_ElSigmaIetaIetaFull5x5;   //!
   TBranch        *b_ElEinvMinusPinv;   //!
   TBranch        *b_ElD0;   //!
   TBranch        *b_ElDz;   //!
   TBranch        *b_ElExpectedMissingInnerHitCnt;   //!
   TBranch        *b_ElPassConvVeto;   //!
   TBranch        *b_ElHltMatch;   //!
   TBranch        *b_ElPfIsoChHad;   //!
   TBranch        *b_ElPfIsoNeutralHad;   //!
   TBranch        *b_ElPfIsoIso;   //!
   TBranch        *b_ElPfIsoPuChHad;   //!
   TBranch        *b_ElPfIsoRaw;   //!
   TBranch        *b_ElPfIsoDbeta;   //!
   TBranch        *b_ElPfIsoRho;   //!
   TBranch        *b_ElAEff;   //!
   TBranch        *b_charged;   //!
   TBranch        *b_photon;   //!
   TBranch        *b_neutral;   //!
   TBranch        *b_charged_Tom;   //!
   TBranch        *b_photon_Tom;   //!
   TBranch        *b_neutral_Tom;   //!
   TBranch        *b_PhotPt;   //!
   TBranch        *b_PhotEta;   //!
   TBranch        *b_PhotPhi;   //!
   TBranch        *b_PhotIsoEcal;   //!
   TBranch        *b_PhotIsoHcal;   //!
   TBranch        *b_PhotIsoTk;   //!
   TBranch        *b_PhotPfIsoChHad;   //!
   TBranch        *b_PhotPfIsoNeutralHad;   //!
   TBranch        *b_PhotPfIsoPhot;   //!
   TBranch        *b_PhotPfIsoPuChHad;   //!
   TBranch        *b_PhotPfIsoEcalClus;   //!
   TBranch        *b_PhotPfIsoHcalClus;   //!
   TBranch        *b_PhotE3x3;   //!
   TBranch        *b_PhotSigmaIetaIeta;   //!
   TBranch        *b_PhotHoE;   //!
   TBranch        *b_PhotId;   //!
   TBranch        *b_PhotHasPixelSeed;   //!
   TBranch        *b_JetAk04Pt;   //!
   TBranch        *b_JetAk04Eta;   //!
   TBranch        *b_JetAk04Phi;   //!
   TBranch        *b_JetAk04E;   //!
   TBranch        *b_JetAk04Id;   //!
   TBranch        *b_JetAk04PuId;   //!
   TBranch        *b_JetAk04PuMva;   //!
   TBranch        *b_JetAk04RawPt;   //!
   TBranch        *b_JetAk04RawE;   //!
   TBranch        *b_JetAk04HfHadE;   //!
   TBranch        *b_JetAk04HfEmE;   //!
   TBranch        *b_JetAk04ChHadFrac;   //!
   TBranch        *b_JetAk04NeutralHadAndHfFrac;   //!
   TBranch        *b_JetAk04ChEmFrac;   //!
   TBranch        *b_JetAk04NeutralEmFrac;   //!
   TBranch        *b_JetAk04ChMult;   //!
   TBranch        *b_JetAk04ConstCnt;   //!
   TBranch        *b_JetAk04JetBeta;   //!
   TBranch        *b_JetAk04JetBetaClassic;   //!
   TBranch        *b_JetAk04JetBetaStar;   //!
   TBranch        *b_JetAk04JetBetaStarClassic;   //!
   TBranch        *b_JetAk04BTagCsv;   //!
   TBranch        *b_JetAk04BTagCsvV1;   //!
   TBranch        *b_JetAk04BTagCsvSLV1;   //!
   TBranch        *b_JetAk04BDiscCisvV2;   //!
   TBranch        *b_JetAk04BDiscJp;   //!
   TBranch        *b_JetAk04BDiscBjp;   //!
   TBranch        *b_JetAk04BDiscTche;   //!
   TBranch        *b_JetAk04BDiscTchp;   //!
   TBranch        *b_JetAk04BDiscSsvhe;   //!
   TBranch        *b_JetAk04BDiscSsvhp;   //!
   TBranch        *b_JetAk04PartFlav;   //!
   TBranch        *b_JetAk04JecUncUp;   //!
   TBranch        *b_JetAk04JecUncDwn;   //!
   TBranch        *b_JetAk04ConstId;   //!
   TBranch        *b_JetAk04ConstPt;   //!
   TBranch        *b_JetAk04ConstEta;   //!
   TBranch        *b_JetAk04ConstPhi;   //!
   TBranch        *b_JetAk04ConstE;   //!
   TBranch        *b_JetAk04GenJet;   //!





        ZJets(const TString& lepSel, TString sampleName, TString fileName_, float lumi_ = 1.,
	      bool useTriggerCorrection_ = 0, int systematics_ = 0, int direction_ = 0, float xsecfactor_ = 1., 
	      int lepPtCutMin_ = 20, int lepEtaCutMax_ = 24, int jetPtCutMin_ = 30, 
	      int jetEtaCutMax_ = 24, Long_t nEvents_ = 0, TString outDir_ = "TEST", 
	      TString bonzaiDir = "/afs/cern.ch/work/a/aleonard/public/ZJetsFiles/",
	      int maxFiles = -1);
        ~ZJets();
	
	/** Build the name of output file
	 * @param pdfSet
	 * @param pdfMember
	 * @param muR scale factor applied on the renormalisation scale. 0 is equivalent to 1.
	 * @param muF scale factor applied on the factorisation scale. 0 is equivalent to 1.
	 * @param iJob job number in case the running is split in several job, 0 otherwise.
	 */
        void CreateOutputFileName(const TString& pdfSet = "", int pdfMember = -1,
				      double muR = 0, double muF = 0, int iJob = 0);
	static std::string
	CreateOutputFileName(const TString& pdfSet, int pdfMember, double muR, double muF, int iJob,
			     const TString& lepSel, const TString& sampleLabel_,
			     bool useTriggerCorrection_, int systematics_, int direction,
			     int jetPtCutMin_, int jetEtaCutMax_ , TString outDir_);
        Int_t    Cut(Long64_t entry);
        Int_t    GetEntry(Long64_t entry);
        Long64_t LoadTree(Long64_t entry);
        void     Init(bool hasRecoInfo, bool hasGenInfo);
        void     initLHAPDF(TString pdfSet, int pdfMember);
        double   computePDFWeight();
        void     Loop(bool hasRecoInfo = 1, bool hasGenInfo = 0, int jobNum = 1, int nJobs = 1,
		      TString pdfSet = "", int pdfMember = -1, double muR = 0, double muF = 0,
		      double yieldScale = 1.);
        void     getMuons(vector<leptonStruct>& leptons,  vector<leptonStruct>& vetoMuons);
        void     getElectrons(vector<leptonStruct>& leptons,  vector<leptonStruct>& vetoElectrons);
        Bool_t   Notify();
        void     Show(Long64_t entry = -1);
	static void readCatalog(const TString& fileName, const TString& bonzaiDir, int maxFiles = -1,
				double* pLumi = 0, double* pXsec = 0, TChain* pChain = 0,
				TChain* pBonzaiHeaderChain = 0, TChain* pBonzaiBitFieldsChain = 0);
	static void canonizeInputFilePath(const TString& bonzaiDir, const TString& fileName,
					  TString* fullFileName, TString* baseName = 0,
					  TString* ext = 0);



	/** Gets integrated luminosity read from lumi parameter of catalog file.
	 */
	double getLumi(){ return lumi_;}
	

	void getMcNorm();

	/** Sets the trigger mask based on triggers parameter read
	 * from the configuration file.
	 * @return true if succesful false otherwise
	 */
	bool setTriggerMask();
	
	TString outputDirectory;
	TString outputFileName;
        TString fileName; 
        double lumi_;
        bool useTriggerCorrection;
        bool isData;
        int systematics;
        int direction;
        float xsecUnc;
        int lepPtCutMin;
        int lepEtaCutMax;
        int jetPtCutMin;
        int jetEtaCutMax;
        Long_t nMaxEvents;
        TString lepSel;
        bool rejectBTagEvents;

	std::vector<Double_t> InEvtWeightSums_;
	std::vector<Double_t> EvtWeightSums_;
	/** Baobab->Bonzai acceptance
	 */
	std::vector<Double_t> skimAccep_;
	Int_t InEvtCount_;
	Int_t EvtCount_;
	double norm_;
	double xsec_;
	double xsecFactor_;
	TString sampleLabel_;
	int maxFiles_;
	
	double processedEventMcWeightSum_;
	ULong64_t triggerMask_;
	bool triggerMaskSet_;
	double muIso_;
	double eIso_;
};
#endif

