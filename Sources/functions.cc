#include "functions.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
#include <cstdarg>
#include <cstring>
#include "TH1.h"
#include "TH2.h"
#include "TRandom.h"
#include "TLorentzVector.h"
#include "TFile.h"
#include "TKey.h"

using namespace std;

bool LepDescendingOrder(leptonStruct l1, leptonStruct l2){
    return (l1.v.Pt() > l2.v.Pt());
}

bool JetDescendingOrder(jetStruct j1, jetStruct j2){
    return (j1.v.Pt() > j2.v.Pt());
}

//--- for WJets ---
bool JetYDescendingOrder(TLorentzVector tj1, TLorentzVector tj2){
    return (tj1.Rapidity() > tj2.Rapidity());
}

double deltaRYPhi(TLorentzVector j1, TLorentzVector j2){
    double dY = j1.Rapidity() - j2.Rapidity();
    double dPhi = deltaPhi(j1, j2);
    return sqrt(dY * dY + dPhi * dPhi);
}
//-----------------

vector<double> makeVector(int num, ...)
{
    va_list list;
    va_start(list, num);
    vector<double> vec;
    for (int i(0); i < num; i++) {
        double next = va_arg(list, double);
        vec.push_back(next);
    }
    va_end(list);
    return vec;
}

void insertVector(vector<double>& veca, int num, ...)
{
    va_list list;
    va_start(list, num);
    vector<double> vecb;
    for (int i(0); i < num; i++) {
        double next = va_arg(list, double);
        vecb.push_back(next);
    }
    va_end(list);
    veca.insert(veca.end(), vecb.begin(), vecb.end());
}

double phi0to2pi(double phi){
    double pi = 3.141592653589793238;
    while (phi >= 2.*pi) phi -= 2.*pi;
    while (phi < 0.) phi += 2.*pi;
    return phi;
}

double deltaPhi(TLorentzVector v1, TLorentzVector v2){
    // build the delta Phi angle between the two vectors
    double pi = 3.141592653589793238;
    double phi1 = phi0to2pi(v1.Phi());
    double phi2 = phi0to2pi(v2.Phi());
    double dPhi = phi0to2pi(phi1 - phi2);
    dPhi = (dPhi > (2*pi - dPhi)) ? 2*pi - dPhi : dPhi;
    return dPhi;
} 

double deltaPhi(double Phi1, double Phi2){
    // build the delta Phi angle between the two vectors
    double pi = 3.141592653589793238;
    double phi1 = phi0to2pi(Phi1);
    double phi2 = phi0to2pi(Phi2);
    double dPhi = phi0to2pi(phi1 - phi2);
    dPhi = (dPhi > (2*pi - dPhi)) ? 2*pi - dPhi : dPhi;
    //cout << "      DeltaPhi: " << endl;
    //cout << "      phi1 = " << phi1 << "  phi2 = " << phi2 << endl;
    //cout << "      DeltaPhi = " << dPhi << endl;
    return dPhi;
} 

double deltaR(TLorentzVector v1, TLorentzVector v2){
    double dEta = v1.Eta() - v2.Eta();
    double dPhi = deltaPhi(v1, v2);
    return sqrt(dEta * dEta + dPhi * dPhi);
}

double deltaR(double Phi1, double Eta1, double Phi2, double Eta2){
    double dEta = Eta1 - Eta2;
    double dPhi = deltaPhi(Phi1, Phi2);
    return sqrt(dEta * dEta + dPhi * dPhi);
}

double PHI(TLorentzVector l1, TLorentzVector l2, TLorentzVector j1, TLorentzVector j2){
    // build the angle PHI between the two subsytems (l1+l2, j1+j2) vectors 
    double lPx = (l1.Px() + l2.Px());
    double lPy = (l1.Py() + l2.Py());
    double lPz = (l1.Pz() + l2.Pz());
    double lNorm = sqrt(lPx * lPx + lPy * lPy + lPz * lPz);
    double jPx = (j1.Px() + j2.Px());
    double jPy = (j1.Py() + j2.Py());
    double jPz = (j1.Pz() + j2.Pz());
    double jNorm = sqrt(jPx * jPx + jPy * jPy + jPz * jPz);
    return acos((jPx * lPx + jPy * lPy + jPz * lPz) / (jNorm * lNorm));
}

double PHI_T(TLorentzVector l1, TLorentzVector l2, TLorentzVector j1, TLorentzVector j2){
    // build the angle PHI between the two subsytems (l1+l2, j1+j2) vectors in the transverse plane
    double lPx = (l1.Px() + l2.Px());
    double lPy = (l1.Py() + l2.Py());
    double lNorm = sqrt(lPx * lPx + lPy * lPy);
    double jPx = (j1.Px() + j2.Px());
    double jPy = (j1.Py() + j2.Py());
    double jNorm = sqrt(jPx * jPx + jPy * jPy);
    return acos((jPx * lPx + jPy * lPy) / (jNorm * lNorm));
}

double SpTsub(TLorentzVector v1, TLorentzVector v2){
    return sqrt(pow(v1.Px() + v2.Px(), 2) + pow(v1.Py() + v2.Py(), 2)) / (v1.Pt() + v2.Pt());
}

double SpT(TLorentzVector l1, TLorentzVector l2, TLorentzVector j1, TLorentzVector j2){
    return sqrt( pow(SpTsub(l1, l2), 2) + pow(SpTsub(j1, j2), 2)  ) / sqrt(2.);
} 

double SPhi(TLorentzVector l1, TLorentzVector l2, TLorentzVector j1, TLorentzVector j2){
    return sqrt(deltaPhi(l1, l2) * deltaPhi(l1, l2) + deltaPhi(j1, j2) * deltaPhi(j1, j2)) / sqrt(2.);
}

record::record(): 
    ptLow(0), ptHi(0), etaLow(0), etaHi(0), effi(0),effiErrorLow(0), effiErrorHigh(0)
{
}

record::record(double pt1, double pt2, double eta1, double eta2, double eff, double effLow, double effHigh):
    ptLow(pt1), ptHi(pt2), etaLow(eta1), etaHi(eta2), effi(eff),effiErrorLow(effLow), effiErrorHigh(effHigh)
{
}

bool record::belongTo(double pt, double eta)
{
    return (pt < ptHi && pt >= ptLow) && (eta < etaHi && eta >= etaLow);
}

table::table()
{
}

table::table(string filename)
{
    ifstream file(filename.c_str());
    if (!file) {
        cerr << "Could not open " << filename << endl;
    }
    double data[7];
    while (file) {
        for (int i(0); i < 7; i++) {
            file >> data[i];
        }
        recd.push_back(record(data[2], data[3], data[0], data[1], data[4], data[5], data[6]));
    }

}

double table::getEfficiency(double pt, double eta){
    double hiPtBin= 0;
    for (unsigned int i=0; i != recd.size(); i++) {
        // if finds the proper bin, then return the efficiency
        if ((recd[i]).belongTo(pt, eta)) return recd[i].effi;
        // else store the average pt of the current bin efficency but do not return and try the next bin
        if ((recd[i]).belongTo(0.5*(recd[i].ptHi + recd[i].ptLow), eta)) hiPtBin = recd[i].effi;
    }
    return hiPtBin;
}

double table::getEfficiencyLow(double pt, double eta){
    double hiPtBin= 0;
    for (unsigned int i=0; i != recd.size(); i++) {
        if ((recd[i]).belongTo(pt, eta)) return recd[i].effi-recd[i].effiErrorLow;
        if ((recd[i]).belongTo(350, eta)) hiPtBin = recd[i].effi;
    }
    return hiPtBin;
}

double table::getEfficiencyHigh(double pt, double eta){
    double hiPtBin= 0;
    for (unsigned int i=0; i != recd.size(); i++) {
        if ((recd[i]).belongTo(pt, eta)) return recd[i].effi+recd[i].effiErrorHigh;
        if ((recd[i]).belongTo(350, eta)) hiPtBin = recd[i].effi;
    }
    return hiPtBin;
}

void bestTwoJetsCandidatesPt(vector<jetStruct> jets, pair<TLorentzVector, TLorentzVector>& bestTwoJets)
{
    int nGoodJets(jets.size());
    if (nGoodJets >= 2){
        //cout << "\nMore than 2 jets, selecting best pair" << endl;
        double minPt(999999.);
        for (int i(0); i < nGoodJets - 1; i++) {
            TLorentzVector jeti = jets[i].v;
            for (int j(i + 1); j < nGoodJets; j++) {
                TLorentzVector jetj = jets[j].v;
                TLorentzVector jetij = jeti + jetj;
                //cout << i << " " << j << ": Pair pt = " << jetij.Pt() << endl;
                if (jetij.Pt() < minPt){
                    bestTwoJets.first = jeti; 
                    bestTwoJets.second = jetj;
                    minPt = jetij.Pt();
                    //cout << "Smallest pt = " << jetij.Pt() << endl;
                }
            }
        } 
    }
}

void bestTwoJetsCandidatesPhi(vector<jetStruct> jets, pair<TLorentzVector, TLorentzVector>& bestTwoJets)
{
    int nGoodJets(jets.size());
    if (nGoodJets >= 2){
        //cout << "\nMore than 2 jets, selecting best pair" << endl;
        double maxdPhi(-0.0001);
        for (int i(0); i < nGoodJets - 1; i++) {
            TLorentzVector jeti = jets[i].v;
            for (int j(i + 1); j < nGoodJets; j++) {
                TLorentzVector jetj = jets[j].v;
                double dPhi = deltaPhi(jeti, jetj);
                //cout << i << " " << j << ": dPhi = " << dPhi << endl;
                if (dPhi > maxdPhi){
                    bestTwoJets.first = jeti; 
                    bestTwoJets.second = jetj;
                    maxdPhi = dPhi;
                    //cout << "Biggest dPhi = " << dPhi << endl;
                }
            }
        } 
    }
}

FILE* eosOpen(const char* path, int (**closeFunc)(FILE*)){
    TString tspath(path);
    if(tspath.BeginsWith("root://")){
	*closeFunc = pclose;
	tspath.Remove(0, strlen("root://"));
	Ssiz_t p = tspath.First("/");
	if(p==TString::kNPOS) return 0;
	TString server(tspath(0, p));
	TString filepath(tspath(p + 1, tspath.Length() - p));
	//   std::cout << ">>> server: " << server << ", path: " << filepath << "\n";
	return popen(TString::Format("xrdfs %s cat %s", server.Data(), filepath.Data()), "r");
    } else{
	*closeFunc = fclose;
	return fopen(path, "r");
    }
}

bool isRootFile(const char* path){
    int (*funcClose)(FILE*);
    FILE* f = eosOpen(path, &funcClose);

    char buffer[5];
    if(f){
	bool rc = (fread(buffer, 5, 1, f)==1) && (memcmp(buffer, "root\0", 5)==0);
	funcClose(f);
	return rc; 
    } else{
	std::cerr << "Warning: failed to read file " << path
		  << ". File type determined from its extension.\n";
	return TString(path).EndsWith(".root");
    }
}
