#include "plotheader.h"
#include "LamWZPreAna.h"
#include "TStyle.h"
#include "TH2F.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

using namespace std;
string DIR = "/data/data068/ycwu/LamWZ/Event_Generation/Events/Plots";
const int LUMINOSITY = 300; //fb^-1
const int N_SIGCAT = 3;
const int N_BKGCAT = 3;
const int N_VARIABLE = 30;
string DecayChannel[2] = {"bbll","tatall"};
string SIG_NAME[N_SIGCAT] = {"wh_hww","wh_hzz","wh_Inter"};
string BKG_NAME[N_BKGCAT] = {"tt","wz","zz"};
string SIG_LABEL[N_SIGCAT] = {"Wh hWW","Wh hZZ","Wh Interference"};
string BKG_LABEL[N_BKGCAT] = {"t#bar{t}","WZ","ZZ"};
void GetHistID(int cat,int &SorB,int &ID);
void createdir(const char *dir);
void SetPlotStyle();
void Analysis(int i);
void LoadingRootFile(TChain *ch,string InputDir,int id);
int main(int argc, char const *argv[])
{
// Usage:
// ./TauPlot config_file
// In the configure file, it indicates the files that will be used to draw the plot.
// Set Plot Style;
	SetPlotStyle();
    TChain *ch = new TChain("LamWZPreAna");

    string InputDir(argv[1]);
    int decayID=atoi(argv[2]);
    LoadingRootFile(ch,InputDir,decayID);
    LamWZPreAna *LamWZch = new LamWZPreAna(ch);
    
    
    // const char* tag = argv[3];
    // Initial the Ploting System;
    binning *bn = new binning();
    LamWZch->initialize(bn);

// Initial the Histogram;
	TFile *fPlots; // The root file used for storing 
	
    TH1F *Hsig[N_SIGCAT][N_VARIABLE]; // the first dim for channel of signal, the second dim for different variables.
    TH1F *Hbkg[N_BKGCAT][N_VARIABLE]; // for bkgs
    TH1F *HsigNORM[N_SIGCAT][N_VARIABLE]; // the first dim for channel of signal, the second dim for different variables.
    TH1F *HbkgNORM[N_BKGCAT][N_VARIABLE]; // for bkgs
	// Using Function void GetHistID(int cat,int &SorB, int &ID);
	TH2F *Hsig2[N_SIGCAT][N_VARIABLE];
	TH2F *Hbkg2[N_BKGCAT][N_VARIABLE];
	// For each variable and each signal cate, we need one THStack, and one Canvas;
	THStack* HSK[N_VARIABLE];
    THStack* HSKNORM[N_VARIABLE];
	TCanvas* CV[N_VARIABLE]; // For Stack
    TCanvas* CVS[N_VARIABLE];// For overlap thus renormalized to 1
	// For every variable, we can use the same legend;
	TLegend* lg;

	// The Style of the Histogram should be set only depending on the Sig or Bkg CAT not Variables.
	char temp[200];
	for (int i = 0; i < bn->nvar; ++i)
	{	
		if(bn->TwoDQ[i]==0)
		{
			// For signal
            for (int j = 0; j < N_SIGCAT; ++j)
            {
				sprintf(temp,"Hsig_%s_var%d_%s",SIG_NAME[j].c_str(),i+1,(bn->VarNameX[i]).c_str());
				Hsig[j][i] = new TH1F(temp,"",bn->nbinx[i],bn->xlo[i],bn->xhi[i]);
				Hsig[j][i]->Sumw2();
            }
			// For background
			for (int j = 0; j < N_BKGCAT; ++j)
			{
				sprintf(temp,"Hbkg_%s_var%d_%s",BKG_NAME[j].c_str(),i+1,(bn->VarNameX[i]).c_str());
				Hbkg[j][i] = new TH1F(temp,"",bn->nbinx[i],bn->xlo[i],bn->xhi[i]);
				Hbkg[j][i]->Sumw2();
			}
		}
		else
		{
            for (int j = 0; j < N_SIGCAT; ++j)
            {
				sprintf(temp,"Hsig_%s_var%d_%svs%s",SIG_NAME[j].c_str(),i+1,(bn->VarNameX[i]).c_str(),(bn->VarNameY[i]).c_str());
				Hsig2[j][i] = new TH2F(temp,"",bn->nbinx[i],bn->xlo[i],bn->xhi[i],bn->nbiny[i],bn->ylo[i],bn->yhi[i]);
				Hsig2[j][i]->Sumw2();
            }
			// For background
			for (int j = 0; j < N_BKGCAT; ++j)
			{
				sprintf(temp,"Hbkg_%s_var%d_%svs%s",BKG_NAME[j].c_str(),i+1,(bn->VarNameX[i]).c_str(),(bn->VarNameY[i]).c_str());
				Hbkg2[j][i] = new TH2F(temp,"",bn->nbinx[i],bn->xlo[i],bn->xhi[i],bn->nbiny[i],bn->ylo[i],bn->yhi[i]);
				Hbkg2[j][i]->Sumw2();
			}
		}
	}


//Event Loop
	Int_t nentries = Int_t(ch->GetEntries());
    double valueX,valueY;
    int SorB, id;
    int bid,cat;
	printf("%d entries to be processed!\n",nentries);
	for (Int_t entry = 0; entry < nentries; ++entry)
	{
		//Load the entry and set required variables;
		LamWZch->GetEntry(entry);
        cat = LamWZch->Cate;
		if((entry+1)%100000==0) {cout<<entry+1<<" entries processed...\r"; cout.flush();}
		// cout<<entry<<endl;
        GetHistID(cat,SorB,id);
		for (int i = 0; i < bn->nvar; ++i)
		{
			if(bn->TwoDQ[i]==0)
			{
				valueX=bn->getValX(i);
				// printf("Value %f\n", value);
				if (SorB==1)
				{
					bid=Hsig[id][i]->Fill(valueX);
				}
				else
				{
					bid=Hbkg[id][i]->Fill(valueX);
				}
			}
			else
			{
				valueX=bn->getValX(i);
				valueY=bn->getValY(i);
				// printf("Value %f vs %f\n", valueX,valueY);
				if (SorB==1)
				{
					bid=Hsig2[id][i]->Fill(valueX,valueY);
				}
				else
				{
					bid=Hbkg2[id][i]->Fill(valueX,valueY);
				}
			}
		}
	}
	cout<<"All Events have been processed..."<<endl;
	cout<<"Event Loop Over"<<endl;
	cout<<endl;

	//Plotting the Histogram;
	cout<<"Collect Info for all Cuts..."<<endl;
	// int bkgcolor[N_BKGCAT]={64,8};
    int bkgcolor[N_BKGCAT]={kOrange+8,kSpring-3,kCyan-4};//,kAzure-3,kMagenta-4,kMagenta+3};
    int sigcolor[N_SIGCAT]={kBlack,kGray+2,kGray};
	//Loop over All variables;
	string dir,dirtop;
	stringstream sst,ssttop;
    ssttop<<DIR;//<<"/"<<tag;
    ssttop>>dirtop;
    cout<<dirtop<<endl;
    createdir(dirtop.c_str());
	sst<<dirtop<<"/Plots_LamWZ";
	sst>>dir;
	createdir(dir.c_str());

    double SigMax=0;
    double sigtemp;
    int BestCutID=0;
    double sigAll;
    double bkgAll;

	int TwoDQ_;
    double normfactor;
    sprintf(temp,"%s/Plots.root",dir.c_str());
    fPlots = new TFile(temp,"RECREATE");
	for (int i = 0; i < bn->nvar; ++i)
	{
        TwoDQ_=bn->TwoDQ[i];
        // cout<<"Var: "<<i<<endl;
        //Book the corresponding THStack and the TCanvas;
        if (TwoDQ_==0)
        {
            sprintf(temp,"Hsk_var%d_%s",i+1,(bn->VarNameX[i]).c_str());
            HSK[i] = new THStack(temp,"");
            // sprintf(temp,"HskOver_var%d_%s",i+1,(bn->VarNameX[i]).c_str());
            // HSKNORM[i] = new THStack(temp,"");

            // sprintf(temp,"HskBEST_var%d_%s",i+1,(bn->VarNameX[i]).c_str());
            // HSKBEST[i] = new THStack(temp,"");
            // sprintf(temp,"HskOverBEST_var%d_%s",i+1,(bn->VarNameX[i]).c_str());
            // HSKNORMBEST[i] = new THStack(temp,"");
        }
        else
        {
            sprintf(temp,"Hsk_var%d_%svs%s",i+1,(bn->VarNameX[i]).c_str(),(bn->VarNameY[i]).c_str());
            HSK[i] = new THStack(temp,"");
            // sprintf(temp,"HskOver_var%d_%svs%s",i+1,(bn->VarNameX[i]).c_str(),(bn->VarNameY[i]).c_str());
            // HSKNORM[i] = new THStack(temp,"");

            // sprintf(temp,"HskBEST_var%d_%svs%s",i+1,(bn->VarNameX[i]).c_str(),(bn->VarNameY[i]).c_str());
            // HSKBEST[i] = new THStack(temp,"");
            // sprintf(temp,"HskOverBEST_var%d_%svs%s",i+1,(bn->VarNameX[i]).c_str(),(bn->VarNameY[i]).c_str());
            // HSKNORMBEST[i] = new THStack(temp,"");
        }
        
		//First Setting the LineSyle/Color etc. of corresponding Histograms
		// cout<<"HSK Booked"<<endl;
		for (int j = 0; j < N_BKGCAT; ++j)
		{
			if(TwoDQ_==0)
			{
                // sprintf(temp,"Events/( %3.1f )",Hbkg[j][i]->GetBinWidth(1));
                normfactor = Hbkg[j][i]->Integral();
                Hbkg[j][i]->Scale(1.0/normfactor);
                Hbkg[j][i]->GetYaxis()->SetTitle("Arbitrary Unit");
                Hbkg[j][i]->GetXaxis()->SetTitle(bn->titleX[i].c_str());
				Hbkg[j][i]->SetLineColor(bkgcolor[j]);
                Hbkg[j][i]->SetLineWidth(2);
				// Hbkg[j][i]->SetFillColor(bkgcolor[j]);
                fPlots->cd();
                Hbkg[j][i]->Write();
                HSK[i]->Add((TH1*)Hbkg[j][i]);
			}
			else
			{
				Hbkg2[j][i]->SetLineColor(bkgcolor[j]);
				// Hbkg2[j][i]->SetFillColor(bkgcolor[j]);
                fPlots->cd();
                Hbkg2[j][i]->Write();
                HSK[i]->Add((TH1*)Hbkg2[j][i]);
			}
		}
        // cout<<"BKG Done"<<endl;
		
		//And Drawing the Histograms;
		for (int j = 0; j < N_SIGCAT; ++j)
		{
			if(TwoDQ_==0)
			{
                // sprintf(temp,"Events/( %3.1f )",Hsig[j][i]->GetBinWidth(1));
                normfactor = Hsig[j][i]->Integral();
                Hsig[j][i]->Scale(1.0/normfactor);
                Hsig[j][i]->GetYaxis()->SetTitle("Arbitrary Unit");
                Hsig[j][i]->GetXaxis()->SetTitle(bn->titleX[i].c_str());
				Hsig[j][i]->SetLineColor(sigcolor[j]);
				Hsig[j][i]->SetLineWidth(2);
                // Hsig[j][i]->SetFillColor(sigcolor[j]);
                fPlots->cd();
                Hsig[j][i]->Write();
                HSK[i]->Add((TH1*)Hsig[j][i]);
			}
			else
			{
				Hsig2[j][i]->SetLineColor(sigcolor[j]);
                // Hsig2[j][i]->SetFillColor(sigcolor[j]);
                fPlots->cd();
                Hsig2[j][i]->Write();
                HSK[i]->Add((TH1*)Hsig2[j][i]);
			}
		}
        // cout<<"SIG Done"<<endl;
        if(i==0)
        {
            lg= new TLegend(0.70,0.70,0.95,0.95,"");
            lg->SetBorderSize(0);
            lg->SetMargin(0.25);
            // lg[j]->SetFillColor(kWhite);
            lg->SetFillColor(0);
            lg->SetFillStyle(0);
            for (int ii = 0; ii < N_BKGCAT; ++ii)
            {
                lg->AddEntry(Hbkg[ii][i],BKG_LABEL[ii].c_str(),"F");
            }
            for (int ii = 0; ii < N_SIGCAT; ++ii)
            {
                sprintf(temp,"Signal(%s)",SIG_LABEL[ii].c_str());
                lg->AddEntry(Hsig[ii][i],temp,"F");
            }
            // cout<<"Legend Done!"<<endl;
        }

        if(TwoDQ_==0)
        {
            sprintf(temp,"Canv_var%d_%s",i+1,(bn->VarNameX[i]).c_str());
            CV[i] = new TCanvas(temp,"",800,800);
            HSK[i]->Draw("nostack hist");
            sprintf(temp,"Arbitrary Unit");
            HSK[i]->GetYaxis()->SetTitle(temp);

            // cout<<"HSK Done"<<endl;

            // sprintf(temp,"Cover_var%d_%s",i+1,(bn->VarNameX[i]).c_str());
            // CVS[i] = new TCanvas(temp,"",800,800);
            // HSKNORM[i]->Draw("nostack hist");
            // sprintf(temp,"Arbitrary Unit");
            // HSKNORM[i]->GetYaxis()->SetTitle(temp);

            // sprintf(temp,"CoverBEST_var%d_%s",i+1,(bn->VarNameX[i]).c_str());
            // CVSBEST[i] = new TCanvas(temp,"",800,800);
            // HSKNORMBEST[i]->Draw("nostack hist");
            // sprintf(temp,"Arbitrary Unit");
            // HSKNORMBEST[i]->GetYaxis()->SetTitle(temp);
            
            // cout<<"HSKNORM Done"<<endl;
        }
        else
        {
            sprintf(temp,"Canv_var%d_%svs%s",i+1,(bn->VarNameX[i]).c_str(),(bn->VarNameY[i]).c_str());
            CV[i] = new TCanvas(temp,"",800,800);
            HSK[i]->Draw();
            HSK[i]->GetYaxis()->SetTitle(bn->titleY[i].c_str());

            // sprintf(temp,"CanvBEST_var%d_%svs%s",i+1,(bn->VarNameX[i]).c_str(),(bn->VarNameY[i]).c_str());
            // CVBEST[i] = new TCanvas(temp,"",800,800);
            // HSKBEST[i]->Draw();
            // HSKBEST[i]->GetYaxis()->SetTitle(bn->titleY[i].c_str());

            // sprintf(temp,"Events/( %3.1f )",Hbkg[0][i]->GetBinWidth(1));
            // HSK[j][i]->GetZaxis()->SetTitle(temp);
        }
        HSK[i]->GetXaxis()->SetTitle(bn->titleX[i].c_str());
        // HSKNORM[i]->GetXaxis()->SetTitle(bn->titleX[i].c_str());

        // HSKBEST[i]->GetXaxis()->SetTitle(bn->titleX[i].c_str());
        // HSKNORMBEST[i]->GetXaxis()->SetTitle(bn->titleX[i].c_str());
        // cout<<"Drawing on Canvas"<<endl;
         
        if (TwoDQ_==0)
        {
            CV[i]->cd();
            lg->Draw("same");
            sprintf(temp,"%s/C_var%d_%s.png",dir.c_str(),i+1,(bn->VarNameX[i]).c_str());
            CV[i]->SaveAs(temp);

            // CVBEST[i]->cd();
            // lgBEST->Draw("same");
            // sprintf(temp,"%s/CBEST_var%d_%s.png",dir.c_str(),i+1,(bn->VarNameX[i]).c_str());
            // CVBEST[i]->SaveAs(temp);

            // CVS[i]->cd();
            // lg->Draw("same");
            // sprintf(temp,"%s/Cover_var%d_%s.png",dir.c_str(),i+1,(bn->VarNameX[i]).c_str());
            // CVS[i]->SaveAs(temp);

            // CVSBEST[i]->cd();
            // lgBEST->Draw("same");
            // sprintf(temp,"%s/CoverBEST_var%d_%s.png",dir.c_str(),i+1,(bn->VarNameX[i]).c_str());
            // CVSBEST[i]->SaveAs(temp);
        }
        else
        {
            // CV[j][i]->Write();
            sprintf(temp,"%s/C_var%d_%svs%s.png",dir.c_str(),i+1,(bn->VarNameX[i]).c_str(),(bn->VarNameY[i]).c_str());
            CV[i]->SaveAs(temp);

            // sprintf(temp,"%s/CBEST_var%d_%svs%s.png",dir.c_str(),i+1,(bn->VarNameX[i]).c_str(),(bn->VarNameY[i]).c_str());
            // CVBEST[i]->SaveAs(temp);
        }
        // cout<<"Saving Done"<<endl;
        fPlots->cd();
        CV[i]->Write();
        // CVS[i]->Write();

        // CVBEST[i]->Write();
        // CVSBEST[i]->Write();
	
	}
	fPlots->Close();
	cout<<"All Plots have been saved to desired directory."<<endl;
	cout<<"Program Exit!"<<endl;
    cout<<">>>>>>>>>>>>>>>>>>>>>>>>>>>>>"<<endl;
	return 0;
}

void GetHistID(int cat,int &SorB,int &ID)
{
	if (abs(cat)<300) // Sig
	{
		SorB = 1; 
		ID = cat%10-1;
	}
	else
	{
		SorB = 0;
		ID = cat/100-3;
	}
}

void createdir(const char *dir)
{
    int exist;
    exist=access(dir,NULL);
    if (exist==0) return;
    mkdir(dir,S_IRWXU|S_IRWXG|S_IROTH|S_IXOTH);//Mode: 775
}
void SetPlotStyle()
{
    gErrorIgnoreLevel = kWarning;
    
    //ZAxis Color
    // const Int_t NRGBs = 5;
    // const Int_t NCont = 255;
    // Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
    // Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
    // Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
    // Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
    // TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
    // gStyle->SetNumberContours(NCont);
    
    //General
    gStyle->SetOptStat(kFALSE);
    gStyle->SetCanvasBorderMode(0);
    
    //Frame
    // gStyle->SetFrameBorderMode(0);
    // gStyle->SetFrameFillColor(0);
    // gStyle->SetFrameLineColor(1);
    // gStyle->SetFrameLineWidth(2);
    
    //Lines
    // gStyle->SetLineWidth(1);
    // gStyle->SetFillColor(0);
    // gStyle->SetCanvasColor(0);
    
    //Margins
    gStyle->SetPadLeftMargin(0.15);
    gStyle->SetPadBottomMargin(0.15);
    gStyle->SetPadRightMargin(0.05);
    gStyle->SetPadTopMargin(0.05);
    
    //text
    gStyle->SetTextFont(132);
    gStyle->SetTextSize(0.06);
    
    //Axis Title
    gStyle->SetTitleFont(62,"XYZ");
    gStyle->SetTitleSize(0.055,"X");
    gStyle->SetTitleSize(0.055,"Y");
    gStyle->SetTitleSize(0.055,"Z");
    gStyle->SetTitleOffset(1.0,"X");
    gStyle->SetTitleOffset(1.3,"Y");
    gStyle->SetTitleOffset(1.0,"Z");
    //Axis Lable
    gStyle->SetLabelFont(42,"XYZ");
    gStyle->SetLabelSize(0.045,"X");
    gStyle->SetLabelSize(0.045,"Y");
    gStyle->SetLabelSize(0.045,"Z");
    gStyle->SetLabelOffset(0.012,"X");
    gStyle->SetLabelOffset(0.012,"Y");
    
    //Linestyle
    // gStyle->SetLineStyleString(10,"");
    // gStyle->SetLineStyleString(11,"30 30");
    // gStyle->SetLineStyleString(12,"30 20 10 20");
    // gStyle->SetLineStyleString(13,"10 10");
    // gStyle->SetLineStyleString(14,"30 20 10 20 10 20 10 20");
}
void LoadingRootFile(TChain *ch,string InputDir,int decayID)
{
    char temp[500];
    sprintf(temp,"%s/*_bkg_%s_*.root",InputDir.c_str(),DecayChannel[decayID].c_str());
    ch->Add(temp);
    sprintf(temp,"%s/wh_*_%s_*.root",InputDir.c_str(),DecayChannel[decayID].c_str());
    ch->Add(temp);
}
