#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <TString.h>
#include <iostream>
#include    <TObjString.h>
//#include    "TString.h"
#include    <sstream>
#include <vector>
#include <TDirectory.h>
#include <TFile.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TKey.h>
#include <TH1.h>
#include <TH2.h>
#include <TH2D.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TPaveStats.h>
#include <TText.h>
#include <TLegend.h>
#include <TGraphAsymmErrors.h>
#include <TCanvas.h>
void getMuonID(){

//TFile* file=new TFile("Muon_ID_iso_Efficiencies_Run_2012ABCD_53X.root");
TFile file("Muon_ID_iso_Efficiencies_Run_2012ABCD_53X.root");
TGraphAsymmErrors* a[10];
 int nHist = file.GetListOfKeys()->GetEntries();
int j=0;
//FILE *outFile = fopen("Muon_ID_iso_Efficiencies_Run_2012ABCD_53X.txt","w");
FILE *outFile1 = fopen("sranje1.txt","w");
// ID TIGHT
 for (int i = 0; i < nHist; i++) {
      string  histoNameTemp = file.GetListOfKeys()->At(i)->GetName();
        fprintf( outFile1, "%d  ",i);
         if (histoNameTemp.find("DATA_over_MC_Tight_pt_abseta")  != string::npos  ) {
        cout<<histoNameTemp<<endl;
        a[j]=(TGraphAsymmErrors* ) file.Get(histoNameTemp.c_str());
        TCanvas *BB=new TCanvas(histoNameTemp.c_str(),histoNameTemp.c_str(),700,500);
        BB->cd();
        a[j]->Draw("AP");
        a[j]->GetN();
        cout<<  a[j]->GetN()<<endl;
        double x=0.,y=0.;
        for (int ll=0; ll<a[j]->GetN(); ll++){
                 a[j]->GetPoint(ll,x,y);
                //cout<< a[j]->GetPoint(ll,x,y)<<"   "<<x<<"   "<<y<<endl;
                //cout<<" tu sam \n"<<endl;     
                 if ( j == 0 )  {fprintf( outFile1," 0   0.9     ");
                                cout<<" 0    0.9    ";}
                 if ( j == 1 )  cout<<" 0.9    1.2    "  ;
                 if ( j == 2 )  cout<<" 1.2    2.1    "  ;
        //       if ( j == 2 )  out<<" 1.2    2.1    "  


                if (x < 20 ){    cout<<"10      20    ";
                                fprintf( outFile1,"10      20    ");}
                else if ( x < 25 ) cout<<"20      25    ";
                else if ( x < 30 ) cout<<"25      30    ";
                else if ( x < 35 ) cout<<"30      35    ";
                else if ( x < 40 ) cout<<"35      40    ";
                else if ( x < 50 ) cout<<"40      50    ";
                else if ( x < 60 ) cout<<"50      60    ";
                else if ( x < 90 ) cout<<"60      90    ";
                else if ( x < 140 ) cout<<"90      140    ";
  else if ( x < 140 ) cout<<"90      140    ";
                else if ( x < 500 ) cout<<"140      500    ";
                fprintf( outFile1, " %e \n",y);
                cout<<y<<endl;
                }
        j++;
        }


}










j=0;
////////////////// isolation
FILE *outFile = fopen("sranje.txt","w");
  for (int i = 0; i < nHist; i++) {
      string  histoNameTemp = file.GetListOfKeys()->At(i)->GetName();
	fprintf( outFile, "%d  ",i);
	 if (histoNameTemp.find("DATA_over_MC_combRelIsoPF04dBeta")  != string::npos  && histoNameTemp.find("012_Tight_pt")   != string::npos ) {
	cout<<histoNameTemp<<endl;
	a[j]=(TGraphAsymmErrors* ) file.Get(histoNameTemp.c_str());
	TCanvas *BB=new TCanvas(histoNameTemp.c_str(),histoNameTemp.c_str(),700,500);
	BB->cd();
	a[j]->Draw("AP");
	a[j]->GetN();
	cout<<  a[j]->GetN()<<endl;
	double x=0.,y=0.;
	for (int ll=0; ll<a[j]->GetN(); ll++){
		 a[j]->GetPoint(ll,x,y);
		//cout<< a[j]->GetPoint(ll,x,y)<<"   "<<x<<"   "<<y<<endl;
		//cout<<" tu sam \n"<<endl;	
		 if ( j == 0 )  {fprintf( outFile," 0	0.9	"); 
				cout<<" 0    0.9    ";} 
		 if ( j == 1 )  cout<<" 0.9    1.2    "  ;
		 if ( j == 2 )  cout<<" 1.2    2.1    "  ;
	//	 if ( j == 2 )  out<<" 1.2    2.1    "  
	

		if (x < 20 ){    cout<<"10      20    ";
				fprintf( outFile,"10      20	");}
		else if ( x < 25 ) cout<<"20      25    ";
		else if ( x < 30 ) cout<<"25      30    ";
		else if ( x < 35 ) cout<<"30      35    ";
		else if ( x < 40 ) cout<<"35      40    ";
		else if ( x < 50 ) cout<<"40      50    ";
		else if ( x < 60 ) cout<<"50      60    ";
		else if ( x < 90 ) cout<<"60      90    ";
		else if ( x < 140 ) cout<<"90      140    ";
		else if ( x < 500 ) cout<<"140      500    ";
		fprintf( outFile, " %e \n",y);
		cout<<y<<endl;
		}
	j++;
	} 
}


}
