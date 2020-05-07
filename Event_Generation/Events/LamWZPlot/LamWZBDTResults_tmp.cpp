#include "plotheader.h"
#include "LamWZPreAna.h"
#include "TStyle.h"
#include "TH2F.h"
#include "TLatex.h"
#include "TGaxis.h"
#include "Utilities.h"
#include "CutList.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "TMVA/Factory.h"
#include "TMVA/DataLoader.h"
#include "TMVA/Tools.h"
#include "TMVA/TMVAGui.h"
#include "TMVA/Config.h"
#include "TMVA/Reader.h"

using namespace std;
string DIR = "/data/data068/ycwu/LamWZ/Event_Generation/Events/Plots";
const int LUMINOSITY = __LUMI__; //fb^-1
const int N_SIGCAT = __NSIG__;
const int N_BKGCAT = __NBKG__;
const int N_VARIABLE = 30;
// string DecayChannel[2] = {"bbll","tatall"};
string SIG_NAME[N_SIGCAT] = {__SIGNAME__};
string BKG_NAME[N_BKGCAT] = {__BKGNAME__};
string SIG_LABEL[N_SIGCAT] = {__SIGLABEL__};
string BKG_LABEL[N_BKGCAT] = {__BKGLABEL__};
int Sig_NTOTAL[N_SIGCAT] = {__SIGNEVE__};
int Bkg_NTOTAL[N_BKGCAT] = {__BKGNEVE__};
int bkgcolor[6]={kOrange+8,kSpring-3,kCyan-4,kAzure-3,kMagenta-4,kMagenta+3};

int main(int argc, char const *argv[])
{
    SetPlotStyle();

    string InputDir(argv[1]);
    string tag(argv[2]);
    int channelID = atoi(argv[3]);

    //Create the Folder Saving the results
    string dir,dirtop;
    stringstream sst,ssttop;
    ssttop<<DIR;
    ssttop>>dirtop;
    createdir(dirtop.c_str());
    sst<<dirtop<<"/BDT_Train_LamWZ_"<<tag;
    sst>>dir;
    createdir(dir.c_str());


    char temp[500];
    sprintf(temp,"%s/BDTResult.root",dir.c_str());
    TFile *f2 = new TFile(temp);
    // TTree *t2 = new TTree("BDTResult","The results from BDT");
    TTree *t2 = (TTree*)f2->Get("BDTResult");
    double BDTScore;
    int processID;
    int SorB;
    double Weight;

    t2->SetBranchAddress("processID",&processID);
    t2->SetBranchAddress("SorB",&SorB);
    t2->SetBranchAddress("Weight",&Weight);
    t2->SetBranchAddress("BDTScore",&BDTScore);

    TH1F *hsig = new TH1F("sig","",100,-0.7,0.5);
    TH1F *hbkg[N_BKGCAT];
    for (int i = 0; i < N_BKGCAT; i++)
    {
        sprintf(temp,"h%s",BKG_NAME[i].c_str());
        hbkg[i] = new TH1F(temp,"",100,-0.7,0.5);
    }
    int nentries = t2->GetEntriesFast();
    for (int entry = 0; entry < nentries; ++entry)
    {
        if((entry+1)%100000==0) {cout<<entry+1<<" entries processed...\r"; cout.flush();}
        t2->GetEntry(entry);
        if (SorB == 1)
        {
            hsig->Fill(BDTScore,Weight);
        }
        else
        {
            hbkg[processID]->Fill(BDTScore,Weight);
        }
    }
    double max=0;
    double maxtemp;
    hsig->SetLineColor(kBlack);
    hsig->SetLineWidth(3);
    maxtemp = hsig->GetMaximum();
    max = maxtemp>max?maxtemp:max;
    for (int i = 0; i < N_BKGCAT; i++)
    {
        hbkg[i]->SetLineColor(bkgcolor[i]);
        hbkg[i]->SetLineWidth(3);
        maxtemp = hbkg[i]->GetMaximum();
        max = maxtemp>max?maxtemp:max;
    }
    TCanvas *c1 = new TCanvas("c1","",1000,800);
    hsig->GetYaxis()->SetTitle("Events/bin");
    hsig->GetYaxis()->SetRangeUser(0,int(1.1*max));
    hsig->GetXaxis()->SetTitle("BDT Score");
    hsig->Draw("HIST");
    for (int i = 0; i < N_BKGCAT; i++)
    {
        hbkg[i]->Draw("HIST SAME");
    }
    TLegend *lg = new TLegend(0.62,0.72,0.83,0.94,"");
    lg->SetBorderSize(0);
    lg->SetMargin(0.25);
    lg->SetFillColor(0);
    lg->SetFillStyle(0);
    for (int i = 0; i < N_BKGCAT; i++)
    {
        lg->AddEntry(hbkg[i],BKG_NAME[i].c_str(),"l");
    }
    lg->AddEntry(hsig,"Signal","l");
    c1->Update();

    TH1F *hsigC = (TH1F*)hsig->GetCumulative(false);
    TH1F *hbkgC[N_BKGCAT];
    for (int i = 0; i < N_BKGCAT; i++)
    {
        hbkgC[i] = (TH1F*) hbkg[i]->GetCumulative(false);
    }

    TH1F *htotal = (TH1F*)hsigC->Clone("total");
    for (size_t i = 0; i < N_BKGCAT; i++)
    {
        htotal->Add(hbkgC[i]);
    }
    TH1F *hsqrt = (TH1F*)htotal->Clone();
    double count;
    double sqrtc;
    for (int i = 0; i < 100; i++)
    {
        count = htotal->GetBinContent(i);
        sqrtc = count>0?sqrt(count):-1;
        hsqrt->SetBinContent(i,sqrtc);
    }
    TH1F *hsigma = (TH1F*)hsqrt->Clone();
    hsigma->Divide(hsigC,hsqrt);
    double sigmamax = hsigma->GetMaximum();
    int maxbin=hsigma->GetMaximumBin();
    double xmax = hsigma->GetBinCenter(maxbin);
    max = 1.5*sigmamax;
    double scale = gPad->GetUymax()/max;
    hsigma->SetLineColor(kBlue);
    hsigma->SetLineWidth(2);
    hsigma->Scale(scale);
    hsigma->Draw("HIST SAME");
    TLine *lmax = new TLine();
    lmax->SetLineColor(kGray);
    lmax->SetLineStyle(9);
    lmax->DrawLine(xmax,sigmamax*scale,0.5,sigmamax*scale);
    TLatex *Tmax = new TLatex();
    sprintf(temp,"#color[920]{%.2f}",sigmamax);
    Tmax->SetTextSize(0.03);
    Tmax->SetTextAlign(13);
    Tmax->DrawLatex(0.7*0.5,0.98*sigmamax*scale,temp);
    lg->AddEntry(hsigma,"Significance","l");
    lg->Draw("SAME");

    TGaxis *axis = new TGaxis(gPad->GetUxmax(),gPad->GetUymin(),gPad->GetUxmax(),gPad->GetUymax(),0,max,510,"+L");
    axis->SetLineColor(kBlue);
    axis->SetLabelColor(kBlue);
    axis->SetTitle("Significance");
    axis->SetTitleFont(62);
    axis->SetTitleSize(0.055);
    axis->SetTitleOffset(1.2);
    axis->SetLabelFont(42);
    axis->SetLabelSize(0.045);
    axis->SetLabelOffset(0.012);
    axis->Draw();


    sprintf(temp,"%s/BDTResults.pdf",dir.c_str());
    c1->SaveAs(temp);
    cout<<"Program Exit!"<<endl;
    cout<<"=================================>"<<endl;
    return 0;
}