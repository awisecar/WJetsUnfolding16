#ifndef _functions_h_
#define _functions_h_

#include <iostream>
#include <cstdarg>
#include <vector>
#include <TLorentzVector.h>
#include <RooUnfoldResponse.h>

using namespace std;

void barre_de_progression(int);

struct leptonStruct{

    leptonStruct();
    leptonStruct(double pt_, double eta_, double phi_, double en_, double charge_, double id_, double iso_, double scEta_, double trigger_) {
        v.SetPtEtaPhiE(pt_, eta_, phi_, en_);
        charge = charge_;
        id = id_;
        iso = iso_;
        scEta = scEta_;
        trigger = trigger_;
    }

    TLorentzVector v;
    double charge, iso, scEta;
    int id, trigger;
};

struct jetStruct{
    jetStruct();
    jetStruct(double pt_, double eta_, double phi_, double en_, int patIndex_, bool isBJet_) {
        v.SetPtEtaPhiE(pt_, eta_, phi_, en_);
        patIndex = patIndex_;
        isBJet = isBJet_;
    }

    TLorentzVector v;
    int patIndex;
    bool isBJet;
};

bool LepDescendingOrder(leptonStruct, leptonStruct);
bool JetDescendingOrder(jetStruct, jetStruct);
//--- for WJets ---
bool JetYDescendingOrder(TLorentzVector, TLorentzVector);
double deltaRYPhi(TLorentzVector, TLorentzVector);
//-----------------

vector<double> makeVector(int num, ...);
void insertVector(vector<double>& veca, int num, ...);

double phi0to2pi(double);

double deltaPhi(TLorentzVector, TLorentzVector);

double deltaPhi(double, double);
double deltaR(TLorentzVector, TLorentzVector);
double deltaR(double, double, double, double);
double PHI(TLorentzVector, TLorentzVector, TLorentzVector, TLorentzVector);
double PHI_T(TLorentzVector, TLorentzVector, TLorentzVector, TLorentzVector);
double SpTsub(TLorentzVector, TLorentzVector);
double SpT(TLorentzVector, TLorentzVector, TLorentzVector, TLorentzVector);
double SPhi(TLorentzVector, TLorentzVector, TLorentzVector, TLorentzVector);

class record{
    public:
        double ptLow, ptHi, etaLow, etaHi, effi, effiErrorLow, effiErrorHigh;
        record();
        record(double, double, double, double, double, double, double);
        bool belongTo(double, double);
};

class table{
    public:
        table();
        table(string);
        double getEfficiency(double, double);  
        double getEfficiencyLow(double, double);  
        double getEfficiencyHigh(double, double);  

    private:
        vector<record> recd;
};

double SmearLepPt(double recoPt, double genPt, int smearlepton, double smearFactor);
double SmearJetPt(double, double, double, int);
void bestTwoJetsCandidatesPt(vector<jetStruct>, pair<TLorentzVector, TLorentzVector>&);
void bestTwoJetsCandidatesPhi(vector<jetStruct>, pair<TLorentzVector, TLorentzVector>&);
void BTagModification(double randNumber, double pt, double eta, int jetFlavour, bool &passBJets);

/** Open in read mode a file on eos or a local file. For EOS file,
 * performance might be limited and it should be kept to small file:
 * it executes the "xrdfs cat" command (see man xrdfs).
 * @param path file path. It is assumes an EOS file if it starts with
 * root:// string.
 * @param closeFunc pointer where to store the function to call for close
 * the file instead of fclose.
 * @return pointer to the file stream.
 */
FILE* eosOpen(const char* path, int (**closeFunc)(FILE*));

/** Test if a file is a root file. The test is based on the "magic number" 
 * contained in the file which identifies its type. A ROOT file starts with
 * the sequence r,o,o,t,\0
 * @param path path of the file to test.
 * @return true iff the file is a ROOT file.
 */
bool isRootFile(const char* path);

/** Adds histograms and RooUnfoldResponse objects  with same name
 * and definition read from different files and writes the result
 * in a new file.
 * @param src list of input files
 * @param dest output files
 * @return true on success, false on failure
 */
bool mergeHistFiles(const std::vector<std::string>& src, const std::string& dest);

#endif
