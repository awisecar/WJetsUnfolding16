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
#include "RooUnfoldResponse.h"


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

double SmearLepPt(double recoPt, double genPt, int smearlepton, double smearFactor){

    double smearedPt(0);

    if (smearlepton == 0) {
        smearedPt = std::max(0., recoPt);
    }

    else if (smearlepton == 1) {
        smearedPt = std::max(0., genPt + (1.0 + smearFactor)*(recoPt - genPt));
    }

    else if (smearlepton == -1) {
        smearedPt = std::max(0., genPt + (1.0 - smearFactor)*(recoPt - genPt));
    }

    return smearedPt;
}

double SmearJetPt(double recoPt, double genPt, double eta, int smearJet){
    // Fall 2015 resolution scale factor
    // twiki.cern.ch/twiki/bin/view/CMS/JetResolution
    double centralSF(1.00);
    if      (fabs(eta) < 0.8) centralSF = 1.061;
    else if (fabs(eta) < 1.3) centralSF = 1.088;
    else if (fabs(eta) < 1.9) centralSF = 1.106;
    else if (fabs(eta) < 2.5) centralSF = 1.126;
    else if (fabs(eta) < 3.0) centralSF = 1.343;
    else if (fabs(eta) < 3.2) centralSF = 1.303;
    else if (fabs(eta) < 5.0) centralSF = 1.320;
    else centralSF = 1.320;

    double upSF(1.00);
    if      (fabs(eta) < 0.8) upSF = 1.084;
    else if (fabs(eta) < 1.3) upSF = 1.117;
    else if (fabs(eta) < 1.9) upSF = 1.136;
    else if (fabs(eta) < 2.5) upSF = 1.220;
    else if (fabs(eta) < 3.0) upSF = 1.466;
    else if (fabs(eta) < 3.2) upSF = 1.414;
    else if (fabs(eta) < 5.0) upSF = 1.606;
    else upSF = 1.606;

    double downSF(1.00);
    if      (fabs(eta) < 0.8) downSF = 1.038;
    else if (fabs(eta) < 1.3) downSF = 1.059;
    else if (fabs(eta) < 1.9) downSF = 1.076;
    else if (fabs(eta) < 2.5) downSF = 1.032;
    else if (fabs(eta) < 3.0) downSF = 1.220;
    else if (fabs(eta) < 3.2) downSF = 1.192;
    else if (fabs(eta) < 5.0) downSF = 1.034;
    else downSF = 1.034;

    double smearedPt(0);

    if (smearJet == 0) {
        smearedPt = std::max(0., genPt + centralSF*(recoPt - genPt));
    }
    else if (smearJet == 1) {
        smearedPt = std::max(0., genPt + upSF*(recoPt - genPt));
    }
    else if (smearJet == -1) {
        smearedPt = std::max(0., genPt + downSF*(recoPt - genPt));
    }

    return smearedPt;

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

void BTagModification(double randNumber, double pt, double eta, int jetFlavour, bool &passBJets)
{
    double x = 0.679;     ///discrim_cut;
    if (abs(jetFlavour) == 5) {

        float effb = -1.73338329789*x*x*x*x +  1.26161794785*x*x*x +  0.784721653518*x*x +  -1.03328577451*x +  1.04305075822;
        float SFb = (0.938887+(0.00017124*pt))+(-2.76366e-07*(pt*pt));

        float SFb_error = 0;

        if      (pt >= 20  && pt < 30)  SFb_error = 0.0415707;
        else if (pt >= 30  && pt < 40)  SFb_error = 0.0204209;
        else if (pt >= 40  && pt < 50)  SFb_error = 0.0223227;
        else if (pt >= 50  && pt < 60)  SFb_error = 0.0206655;
        else if (pt >= 60  && pt < 70)  SFb_error = 0.0199325;
        else if (pt >= 70  && pt < 80)  SFb_error = 0.0174121;
        else if (pt >= 80  && pt < 100) SFb_error = 0.0202332;
        else if (pt >= 100 && pt < 120) SFb_error = 0.0182446;
        else if (pt >= 120 && pt < 160) SFb_error = 0.0159777;
        else if (pt >= 160 && pt < 210) SFb_error = 0.0218531;
        else if (pt >= 210 && pt < 260) SFb_error = 0.0204688;
        else if (pt >= 260 && pt < 320) SFb_error = 0.0265191;
        else if (pt >= 320 && pt < 400) SFb_error = 0.0313175;
        else if (pt >= 400 && pt < 500) SFb_error = 0.0415417;
        else if (pt >= 500 && pt < 600) SFb_error = 0.0740446;
        else if (pt >= 600) SFb_error = 0.0596716;

        float SFb_up = SFb + SFb_error;
        float SFb_down = SFb - SFb_error;

        // F values for rand comparison
        float f = 0.0;
        float f_up = 0.0;
        float f_down = 0.0;

        if (SFb <1.0) f = (1.0 - SFb);
        if (SFb_up <1.0) f_up = (1.0 - SFb_up);
        if (SFb_down <1.0) f_down = (1.0 - SFb_down);

        if (SFb > 1.0) f = (1.0 - SFb)/(1.0 - 1.0/effb);
        if (SFb_up > 1.0) f_up = (1.0 - SFb_up)/(1.0 - 1.0/effb);
        if (SFb_down > 1.0) f_down = (1.0 - SFb_down)/(1.0 - 1.0/effb);

        bool passBJets_SFB_sys_up = passBJets;   // Initialize the systematic_up as the central value
        bool passBJets_SFB_sys_down = passBJets; // Initialize the systematic_down as the central value

        // Untag a tagged jet

        if (passBJets && SFb<1.0 && randNumber<f) passBJets = false; // for central value
        if (passBJets_SFB_sys_up && SFb<1.0 && randNumber<f_up) passBJets_SFB_sys_up = false; // for systematic_up
        if (passBJets_SFB_sys_down && SFb<1.0 && randNumber<f_down) passBJets_SFB_sys_down = false; // for sytematic_down


        // Tag an untagged jet
        if (!passBJets && SFb>1.0 && randNumber<f) passBJets = true; // for central value
        if (!passBJets_SFB_sys_up && SFb>1.0 && randNumber<f_up) passBJets_SFB_sys_up = true; // for systematic_up
        if (!passBJets_SFB_sys_down && SFb>1.0 && randNumber<f_down) passBJets_SFB_sys_down = true; // for sytematic_down

    }
    // ---------------- For Real C-jets--------------- //
    else if (abs(jetFlavour) == 4) {

        float effc = -1.5734604211*x*x*x*x +  1.52798999269*x*x*x +  0.866697059943*x*x +  -1.66657942274*x +  0.780639301724;
        float SFc = (0.938887+(0.00017124*pt))+(-2.76366e-07*(pt*pt));

        float SFc_error = 0.;

        if      (pt >= 20  && pt < 30)  SFc_error = 0.0415707;
        else if (pt >= 30  && pt < 40)  SFc_error = 0.0204209;
        else if (pt >= 40  && pt < 50)  SFc_error = 0.0223227;
        else if (pt >= 50  && pt < 60)  SFc_error = 0.0206655;
        else if (pt >= 60  && pt < 70)  SFc_error = 0.0199325;
        else if (pt >= 70  && pt < 80)  SFc_error = 0.0174121;
        else if (pt >= 80  && pt < 100) SFc_error = 0.0202332;
        else if (pt >= 100 && pt < 120) SFc_error = 0.0182446;
        else if (pt >= 120 && pt < 160) SFc_error = 0.0159777;
        else if (pt >= 160 && pt < 210) SFc_error = 0.0218531;
        else if (pt >= 210 && pt < 260) SFc_error = 0.0204688;
        else if (pt >= 260 && pt < 320) SFc_error = 0.0265191;
        else if (pt >= 320 && pt < 400) SFc_error = 0.0313175;
        else if (pt >= 400 && pt < 500) SFc_error = 0.0415417;
        else if (pt >= 500 && pt < 600) SFc_error = 0.0740446;
        else if (pt >= 600) SFc_error = 0.0596716;

        float SFc_up = SFc + 2*SFc_error;
        float SFc_down = SFc - 2*SFc_error;

        // F values for rand comparison
        float f = 0.0;
        float f_up = 0.0;
        float f_down = 0.0;

        if (SFc <1.0) f = (1.0 - SFc);
        if (SFc_up <1.0) f_up = (1.0 - SFc_up);
        if (SFc_down <1.0) f_down = (1.0 - SFc_down);

        if (SFc > 1.0) f = (1.0 - SFc)/(1.0 - 1.0/effc);
        if (SFc_up > 1.0) f_up = (1.0 - SFc_up)/(1.0 - 1.0/effc);
        if (SFc_down > 1.0) f_down = (1.0 - SFc_down)/(1.0 - 1.0/effc);

        bool passBJets_SFB_sys_up = passBJets;     // Initialize the systematic_up as the central value
        bool passBJets_SFB_sys_down = passBJets; // Initialize the systematic_down as the central value

        // Untag a tagged jet

        if (passBJets && SFc<1.0 && randNumber<f) passBJets = false; // for central value
        if (passBJets_SFB_sys_up && SFc<1.0 && randNumber<f_up) passBJets_SFB_sys_up = false; // for systematic_up
        if (passBJets_SFB_sys_down && SFc<1.0 && randNumber<f_down) passBJets_SFB_sys_down = false; // for sytematic_down


        // Tag an untagged jet
        if (!passBJets && SFc>1.0 && randNumber<f) passBJets = true; // for central value
        if (!passBJets_SFB_sys_up && SFc>1.0 && randNumber<f_up) passBJets_SFB_sys_up = true; // for systematic_up
        if (!passBJets_SFB_sys_down && SFc>1.0 && randNumber < f_down) passBJets_SFB_sys_down = true; // for sytematic_down

    }
    // ---------------- For REAL Light-jets --------------- //
    else if (abs(jetFlavour) < 4) {

        float SFlight=1.0;
        float SFlight_up=1.0;
        float SFlight_down=1.0;
        float eff_l = 0.0;

        if (fabs(eta) <= 0.8) {
            SFlight = (((1.07541+(0.00231827*pt))+(-4.74249e-06*(pt*pt)))+(2.70862e-09*(pt*(pt*pt))));
            SFlight_up = (((1.18638+(0.00314148*pt))+(-6.68993e-06*(pt*pt)))+(3.89288e-09*(pt*(pt*pt))));
            SFlight_down = (((0.964527+(0.00149055*pt))+(-2.78338e-06*(pt*pt)))+(1.51771e-09*(pt*(pt*pt))));
            eff_l = ((0.00967751+(2.54564e-05*pt))+(-6.92256e-10*(pt*pt)));
        }
        else if (fabs(eta) <= 1.6) {
            SFlight = (((1.05613+(0.00114031*pt))+(-2.56066e-06*(pt*pt)))+(1.67792e-09*(pt*(pt*pt))));
            SFlight_up = (((1.16624+(0.00151884*pt))+(-3.59041e-06*(pt*pt)))+(2.38681e-09*(pt*(pt*pt))));
            SFlight_down = (((0.946051+(0.000759584*pt))+(-1.52491e-06*(pt*pt)))+(9.65822e-10*(pt*(pt*pt))));
            eff_l = ((0.00974141+(5.09503e-05*pt))+(2.0641e-08*(pt*pt)));
        }
        else if (fabs(eta) <= 2.4) {
            SFlight = (((1.05625+(0.000487231*pt))+(-2.22792e-06*(pt*pt)))+(1.70262e-09*(pt*(pt*pt))));
            SFlight_up = (((1.15575+(0.000693344*pt))+(-3.02661e-06*(pt*pt)))+(2.39752e-09*(pt*(pt*pt))));
            SFlight_down = (((0.956736+(0.000280197*pt))+(-1.42739e-06*(pt*pt)))+(1.0085e-09*(pt*(pt*pt))));
            eff_l = ((0.013595+(0.000104538*pt))+(-1.36087e-08*(pt*pt)));
        }

        // F values for rand comparison
        float f = 0.0;
        float f_up = 0.0;
        float f_down = 0.0;

        if (SFlight <1.0) f = (1.0 - SFlight);
        if (SFlight_up <1.0) f_up = (1.0 - SFlight_up);
        if (SFlight_down <1.0) f_down = (1.0 - SFlight_down);

        if (SFlight > 1.0) f = (1.0 - SFlight)/(1.0 - 1.0/eff_l);
        if (SFlight_up > 1.0) f_up = (1.0 - SFlight_up)/(1.0 - 1.0/eff_l);
        if (SFlight_down > 1.0) f_down = (1.0 - SFlight_down)/(1.0 - 1.0/eff_l);

        bool passBJets_SFB_sys_up = passBJets;     // Initialize the systematic_up as the central value
        bool passBJets_SFB_sys_down = passBJets; // Initialize the systematic_down as the central value

        // Untag a tagged jet

        if (passBJets && SFlight<1.0 && randNumber<f) passBJets = false; // for central value
        if (passBJets_SFB_sys_up && SFlight<1.0 && randNumber<f_up) passBJets_SFB_sys_up = false; // for systematic_up
        if (passBJets_SFB_sys_down && SFlight<1.0 && randNumber<f_down) passBJets_SFB_sys_down = false; // for sytematic_down


        // Tag an untagged jet
        if (!passBJets && SFlight>1.0 && randNumber<f) passBJets = true; // for central value
        if (!passBJets_SFB_sys_up && SFlight>1.0 && randNumber<f_up) passBJets_SFB_sys_up = true; // for systematic_up
        if (!passBJets_SFB_sys_down && SFlight>1.0 && randNumber<f_down) passBJets_SFB_sys_down = true; // for sytematic_down

    }   ////////flavour lop                     
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

bool mergeHistFiles(const std::vector<std::string>& src, const std::string& dest)
{
    TH1::SetDefaultSumw2();

    std::cerr << "Creating file " << dest << std::endl;
    TFile fout(dest.c_str(), "RECREATE");
    if(fout.IsZombie()){
	std::cerr << "Error: failed to create file " << dest 
		  << " (" << __FILE__ << ":" << __LINE__ << ").\n\n";
	return false;
    }

    union u { TObject* o; TH1* h; RooUnfoldResponse* r; } obj;
    std::map<std::string,u> objs;
    for(unsigned iFile = 0; iFile < src.size(); ++iFile){
	TFile in(src[iFile].c_str());
	if(in.IsZombie()) continue;
	TIter next(in.GetListOfKeys());
	TKey* key;
	while((key = (TKey*) next())){
	    obj.o = key->ReadObj();
	    enum {kHist, kResp, kOther} type = kOther;
	    if(obj.o->InheritsFrom("TH1")) type = kHist;
	    if(obj.o->InheritsFrom("RooUnfoldResponse")) type = kResp;
	    if(type == kOther) continue;
	    if(iFile == 0){
		if(type==kHist) obj.h->SetDirectory(0);
		objs[obj.o->GetName()].o = obj.o;
	    } else{
		std::map<std::string,u>::iterator it = objs.find(obj.o->GetName());
		if(it!=objs.end()){
		    if(type==kHist) (*it).second.h->Add(obj.h);
		    if(type==kResp) (*it).second.r->Add(*(obj.r));
		}
	    }
	} //next Object
	in.Close();
    }//next File

    fout.cd();
    for(std::map<std::string,u>::iterator it = objs.begin();
	it != objs.end(); ++it){
	it->second.o->Write();
	delete it->second.o;
    }
    fout.Close();
    return true;
}
