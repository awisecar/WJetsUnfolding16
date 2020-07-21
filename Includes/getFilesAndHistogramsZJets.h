#ifndef _getFilesAndhistogramsZJets_h_
#define _getFilesAndhistogramsZJets_h_

#include <iostream>
#include <TH1.h>
#include <TH2.h>
#include <TFile.h>
#include <TString.h>
#include <vector>
#include "fileNamesZJets.h"

using namespace std;

/** Open the histogram file of a given contribution and configuration.
 * @param histoDir    location of histogram files.
 * @param lepSel      Analysis and channel: DMu, DE, SMu, SE. D: Z+jets, S: W+jets,
 *                    Mu: muon channel, E: electron channel
 * @param energy      Center-of-mass beam energy
 * @param Name        Contribution/sample name
 * @param jetPtMin    Lower bound of jet pt. Typ. 20 or 30
 * @param jetEtaMax   Upper bound of jet |eta| is 1/10th unit
 * @param closureTest Closure test label if applicable, empty string otherwise
 * @param syst        Systematic label. "0" indicates nominal histogram.
 */
TFile* getFile(TString histoDir, TString lepSel, TString energy, TString Name, int jetPtMin = 30, int jetEtaMax = 24, TString closureTest = "", TString syst = "0");

/** Opens the histogram files of a given contribution for the nominal value and 
 *  all the systematic uncertainty sources variations.
 * Variations are (index, variation):
 *    data:      0- nominal, 1- JES up, 2- JES down
 *    signal mc: 0- nominal, 1- PU up, 2- PU down, 3- JER up, 4- JER down,
 *               5- LES up, 6- LES down, 7- LER up, 8- LER down
 *    backgound: 0- nominal, 1- PU up, 2- PU down, 2- xsec up, 4- xsec dwn,
 *               5- LES up, 6 - LES down
 * @param histoDir    location of histogram files.
 * @param Files       [out] List of opened files.
 * @param lepSel      Analysis and channel: DMu, DE, SMu, SE. D: Z+jets, S: W+jets,
 *                    Mu: muon channel, E: electron channel
 * @param energy      Center-of-mass beam energy
 * @param Name        Contribution/sample name
 * @param jetPtMin    Lower bound of jet pt. Typ. 20 or 30
 * @param jetEtaMax   Upper bound of jet |eta| is 1/10th unit
 */
void getFiles(TString histoDir, TFile *Files[], TString lepSel, TString energy, TString Name, int jetPtMin = 30, int jetEtaMax = 24);

/** Opens all the histogram files for all contributions, nominal value and 
 *  systematic uncertainty sources variations. Different TFile array is 
 * returned for data, signal MC and  each background MC. See the documentaion of
 * the getFile() method version without sys parameter for the description of
 * uncertainty source variations and fData, fDJetss, fBg corresponding index.
 * @param histoDir    location of histogram files.
 * @param Files       Result: list of opened files.
 * @param lepSel      Analysis and channel: DMu, DE, SMu, SE. D: Z+jets, S: W+jets,
 *                    Mu: muon channel, E: electron channel
 * @param energy      Center-of-mass beam energy
 * @param jetPtMin    Lower bound of jet pt. Typ. 20 or 30
 * @param jetEtaMax   Upper bound of jet |eta| is 1/10th unit
 * @param fData       [out] List of opened real data TFiles.
 * @param fDYJets     [out] List of opened signal MC TFiles. First index
 *                    corresponds to the sample, second one to the systematic
 *                    uncertainty variation.
 * @param fBg         [out] List of opened signal MC TFiles First index
 *                    corresponds to the sample, second one to the systematic
 *                    uncertainty variation.
 * @param nBg         Number of background samples. It must be equal to number of
 *                    Samples (see filesNameZJets.h) element minus 2, or a smallest
 *                    number in which case only the first nBg samples listed in Samples
 *                    will be considered.
 */
void getAllFiles(TString histoDir, TString lepSel, TString energy, int jetPtMin, int jetEtaMax, TFile *fData[3], TFile *fDYJets[11], TFile *fBg[][11], int nBg);

void closeFile(TFile*);
void closeFiles(TFile *Files[]);
void closeFiles(TFile *Files[], int nFiles);
void closeAllFiles(TFile *fData[3], TFile *fDYJets[11], TFile *fBg[][11], int nBg);

void setNegBinZero(TH2D *histograms2D);

TH1D* getHisto(TFile*, TString);
void getHistos(TH1D *histograms[], TFile *Files[], TString variable, int year);
void getHistos(TH2D *histograms[], TFile *Files[], TString variable, int year);

void getAllFakes(TH1D *hFakDYJets[18], TH1D *hRecData[3], TH1D *hRecSumBg[13], TH1D *hRecDYJets[13], TH2D *hResDYJets[13], bool isClosureTest);
TH1D* getFakes(TH1D *hRecDYJets, TH1D *hRecData, TH1D *hRecSumBg, TH2D *hResDYJets, bool isVerbose, bool isClosureTest);

void getAllHistos(TString variable, int year, TH1D *hRecData[3], TFile *fData[3], TH1D *hRecDYJets[13], TH1D *hGenDYJets[13], TH2D *hResDYJets[13], TFile *fDYJets[11], TH1D *hRecBg[][13], TH1D *hRecSumBg[13], TFile *fBg[][11], int nBg, TH1D* hFakDYJets[18], TH2D *hAccepDYJets[13], bool isClosureTest);


#endif

