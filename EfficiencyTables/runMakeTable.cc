#include <iostream>
#include <iomanip>
#include <TFile.h>
#include <TGraphAsymmErrors.h>

using namespace std;

void getInfoFromTGraph(TGraphAsymmErrors *gr, int nBin, double coord[]);
void getBinLimitsFromTGraph(TGraphAsymmErrors *gr, double binLimit[]);
void makeEfficiencyTable(string workingPoint = "Tight");
void makeEfficiencyTableHelp()
{
   cout << endl;
   cout << "Possible working point:       " << endl;
   cout << "      - Tight (default)       " << endl;
   cout << "      - Loose                 " << endl;
   cout << "      - Soft                  " << endl;
   cout << "      - HightPt               " << endl;
   cout << endl;
}

void runMakeTable()
{
   makeEfficiencyTable("Tight");
   makeEfficiencyTable("combRelIsoPF04dBeta<02_Tight");
   makeEfficiencyTable("combRelIsoPF04dBeta<012_Tight");
}

void makeEfficiencyTable(string workingPoint)
{
   TFile *effFile = new TFile("Muon_ID_iso_Efficiencies_Run_2012ABCD_53X.root", "READ");
   string endName = "_2012ABCD";
   string absetaBins[3] = {"<0.9", "0.9-1.2", "1.2-2.1"};
  


  cout << "\\begin{tabular}{ccccc}" << endl;

  if (workingPoint.find("Iso") == string::npos) cout << "\\multicolumn{5}{c}{Scale Factors for Muons Tight ID} \\\\ \\hline" << endl;
  else if (workingPoint.find("<02") != string::npos) cout << "\\multicolumn{5}{c}{Scale Factors for Tight Muons CombRelIsodBeta $< 0.2$ (R=0.4)} \\\\ \\hline" << endl;
  else if (workingPoint.find("<012") != string::npos) cout << "\\multicolumn{5}{c}{Scale Factors for Tight Muons CombRelIsodBeta $< 0.12$ (R=0.4)} \\\\ \\hline" << endl;
  cout << "$p_T$  $\\backslash$ $|\\eta|$" << " & $0 < |\\eta| \\leq 0.9$ " << "& $0.9 < |\\eta| \\leq 1.2$ " << "& $1.2 < | \\eta | \\leq 2.1$ " << "& $2.1 < |\\eta| \\leq 2.4$ \\\\ \\hline" << endl; 
  

  
  string grName = "DATA_" + workingPoint + "_pt_abseta" + absetaBins[0] + endName;
  TGraphAsymmErrors *grPt = (TGraphAsymmErrors*) effFile->Get(grName.c_str());
  int nPtBins(grPt->GetN());
  double binLimit[nPtBins+1];
  getBinLimitsFromTGraph(grPt, binLimit);


  grName = "DATA_over_MC_" + workingPoint + "_eta_pt20-500" + endName;
  grPt   = (TGraphAsymmErrors*) effFile->Get(grName.c_str());
  int newNBin(grPt->GetN());
  double coord1[8], coord2[8];
  getInfoFromTGraph(grPt, 0, coord1); 
  getInfoFromTGraph(grPt, newNBin-1, coord2); 
  double HiEtaEff(0.5*(coord1[5]+coord2[5]));
  double HiEtaErr(0.5*( 0.5* (coord1[6]+coord1[7]) + 0.5* (coord2[6]+coord2[7])));


  for (int i(1); i < nPtBins; i++){
     cout << binLimit[i] << " $< p_T \\leq $ " <<  binLimit[i+1];
     for (int j(0); j < 3; j++){
	grName = "DATA_over_MC_" + workingPoint + "_pt_abseta" + absetaBins[j] + endName;
	grPt   = (TGraphAsymmErrors*) effFile->Get(grName.c_str());
	double coordSF[8];
	getInfoFromTGraph(grPt, i, coordSF);
	cout << fixed << setprecision(3);
	cout << "  &  " << coordSF[5] << " $\\pm$ " << 0.5*(coordSF[6] + coordSF[7]);
     }
     cout << "  &  " << HiEtaEff << " $\\pm$ " << HiEtaErr;
     cout << fixed << setprecision(0);

     cout << "\\\\ " << endl;
  }
  cout << "\\hline \\end{tabular}" << endl;
}


void getInfoFromTGraph(TGraphAsymmErrors *gr, int nBin, double coord[])
{
   gr->GetPoint(nBin, coord[0], coord[5]);  // x and y
   coord[3] = gr->GetErrorXlow(nBin);       // xErrLow
   coord[4] = gr->GetErrorXhigh(nBin);      // xErrHigh
   coord[1] = coord[0] - coord[3];          // xLow
   coord[2] = coord[0] + coord[4];          // xHigh
   coord[6] = gr->GetErrorYlow(nBin);       // yErrLow
   coord[7] = gr->GetErrorYhigh(nBin);      // yErrHigh
}

void getBinLimitsFromTGraph(TGraphAsymmErrors *gr, double binLimit[])
{
   int nBins(gr->GetN());
   for (int i(0); i < nBins; i++){
      double x, y, xEl, xEh;
      gr->GetPoint(i, x, y);
      xEl = gr->GetErrorXlow(i);
      xEh = gr->GetErrorXhigh(i);
      binLimit[i] = x - xEl;
      binLimit[i+1] = x + xEh;
   }

}

