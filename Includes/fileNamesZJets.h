//-*- c-basic-offset: 4; -*-
#ifndef _FILENAMESZJETS_h_
#define _FILENAMESZJETS_h_

#include <TString.h>

//-- directory of input root files --------------------
//Commented out, as it should be obtained from the configuration parameter histoDir
//const TString FILESDIRECTORY("HistoFiles/");
//---------- lets add basic information on samples inot common struct -------------------------------------------
struct processInfoStruct{
    TString name;
    char    merge; //'+' indicates the sample be merged to the next one
                   //until '=' is found. Line with '=' refers to the sample
                   //resulting from the merge.
    double NEvents, xsec, xsecFrac, xsecError;
    int colorAN, colorPAS;
    TString legendAN, legendPAS;
};

//--- first element must point to the data
//--- last element must point to the MC Signal(s)
const processInfoStruct Samples[] = {
    //--  Name  --- merge - #events -- xsec - BR - xsec rel. unc. - colorAN - colorPAS - name on legend
    /* 0*/{"Data",                      ' ',     1.,       1.,      1,  1,        kBlack,    kBlack,    " Data", " Data"},
    /* 1*/{"TT",                        ' ',     1.,       1.,      1,  0.10,     kBlue,     kBlue,     " TT",   " t#bar{t}"},  
    /* 2*/{"Top",                       ' ',     1.,       1.,      1,  0.06,     kMagenta,  kMagenta,  " Single Top", "Single top"},
    /* 3*/{"QCD",                       ' ',     1.,       1.,      1,  0.06,     kYellow,   kYellow,   " QCD", "QCD"},
    /* 4*/{"VV",                        ' ',     1.,       1.,      1,  0.06,     kOrange,   kOrange,   " VV", " VV"},
    /* 5*/{"DYJets10toInf3",            ' ',     1.,       1.,      1,  0.06,     kGreen-8,  kGreen-8,  " DYJets", " Z/#gamma^{*} #rightarrow ll"},
    /* 6*/{"ttV",                       ' ',     1.,       1.,      1,  0.10,     kGreen,    kGreen,    " ttV", " ttV"},
    /* 7*/{"WJetsToLN_UNFOLDING_FxFx",  ' ',     1.,       1.,      1,  0.06,     kPink,     kPink,     " WJets", " W+Jets"},
};

const int NSamples = sizeof(Samples) / sizeof(Samples[0]);
const int DATA(0);
const int DYJETS(NSamples - 1); // Signal MC is the last sample in the list.

// Total number of samples after sample grouping, including real data, background MC, and signal MC
const unsigned int NFILESDYJETS = 8;

// Number of background MC samples: all minus data and the signal MC
const unsigned int NBGDYJETS = NFILESDYJETS - 2;

/** List of indices of samples from Samples to be used for ZJets analysis
 * When samples are grouped, only the merged sample is included in this list.
 */
const unsigned int FilesDYJets[NFILESDYJETS] = {0, 1, 2, 3, 4, 5, 6, 7};

//--- Use this Flag for Unfolding Resp Syst ---------------------
const TString DYSHERPA14FILENAME("");
//const TString DYSHERPA14FILENAME("sherpa");
const TString DYSHERPA14LEGEND("Sherpa1.4 LO");
//---------------------------------------------------------------

//--- Use this Flag for the 2nd Theory --------------------------
//const TString DYMGPYTHIA8FILENAME("");
const TString DYMGPYTHIA8FILENAME("WJetsToLN_UNFOLDING_MLM");
const TString DYMGPYTHIA8LEGEND("MG_aMC + PY8 (#leq 4j LO + PS)");
//---------------------------------------------------------------

//--- Use this Flag for the 3rd Theory --------------------------
const TString NNLO1JFILENAME("");
//const TString NNLO1JFILENAME("NNLO1J");
const TString NNLO1JLEGEND("N_{jetti} NNLO");
//---------------------------------------------------------------

//--- Use this Flag for the NLO FxFx Inclusive sample------------
const TString NLOFXFXINCLUSIVE_FILENAME("WJetsToLN_UNFOLDING_FxFx_Inclusive");
const TString NLOFXFXINCLUSIVE_LEGEND("NLO FxFx Inclusive Sample");
//---------------------------------------------------------------

#endif