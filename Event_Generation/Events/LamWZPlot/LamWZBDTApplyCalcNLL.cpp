#include "plotheader.h"
#include "LamWZPreAna.h"
#include "TStyle.h"
#include "TH2F.h"
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

#include "RooArgSet.h"
#include "RooArgList.h"
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooDataHist.h"
#include "RooHistPdf.h"
#include "RooAddPdf.h"
#include "RooPlot.h"

using namespace RooFit;
using namespace std;
string DIR = "/data/data068/ycwu/LamWZ/Event_Generation/Events/Plots";
const int LUMINOSITY3000 = 4000; //fb^-1
const int LUMINOSITY1500 = 2000; //fb^-1
const int N_SIGCAT = 9;
const int N_BKGCAT = 3;
const int N_VARIABLE = 30;
string DecayChannel[2] = {"bbll","tatall"};
string SIG_NAME[2][N_SIGCAT] = {{"wh_Full_lwz0x6","wh_Full_lwz0x65","wh_Full_lwz0x7","wh_Full_lwz0x75","wh_Full_lwz0x8","wh_Full_lwz1x0","wh_Full_lwz1x2","wh_Full_lwz1x4","wh_Full_lwz1x6"},{"zh_Full_lwz0x6","zh_Full_lwz0x65","zh_Full_lwz0x7","zh_Full_lwz0x75","zh_Full_lwz0x8","zh_Full_lwz1x0","zh_Full_lwz1x2","zh_Full_lwz1x4","zh_Full_lwz1x6"}};
string BKG_NAME[N_BKGCAT] = {"tt","wz","zz"};
string SIG_LABEL[2][N_SIGCAT] = {{"Wh(#lambda_{WZ}=0.6)","Wh(#lambda_{WZ}=0.65)","Wh(#lambda_{WZ}=0.7)","Wh(#lambda_{WZ}=0.75)","Wh(#lambda_{WZ}=0.8)","Wh(#lambda_{WZ}=1.0)","Wh(#lambda_{WZ}=1.2)","Wh(#lambda_{WZ}=1.4)","Wh(#lambda_{WZ}=1.6)"},{"Zh(#lambda_{WZ}=0.6)","Zh(#lambda_{WZ}=0.65)","Zh(#lambda_{WZ}=0.7)","Zh(#lambda_{WZ}=0.75)","Zh(#lambda_{WZ}=0.8)","Zh(#lambda_{WZ}=1.0)","Zh(#lambda_{WZ}=1.2)","Zh(#lambda_{WZ}=1.4)","Zh(#lambda_{WZ}=1.6)"}};
string BKG_LABEL[N_BKGCAT] = {"t#bar{t}","WZ","ZZ"};
int Sig_NTOTAL3000[2][N_SIGCAT] = {{500000,500000,461798,478026,316640,750000,500000,500000,500000},{424246,453548,453398,467820,500000,525505,452102,409133,442904}};
int Sig_NTOTAL1500[2][N_SIGCAT] = {{550000,500000,500000,500000,550000,750000,550000,538928,500000},{516971,500000,500000,500000,500000,750000,500000,500000,500000}};
int Bkg_NTOTAL[N_BKGCAT] = {2500000,2500000,2500000};
double BDTScoreCuts3000[2] = {-0.005,0.055};
double BDTScoreCuts1500[2] = {0.035,0.115};
double SHATMIN3000[2] = {100,100};
double SHATMAX3000[2] = {1200,1800};
double lwzValue[N_SIGCAT] = {0.6,0.65,0.7,0.75,0.8,1.0,1.2,1.4,1.6};
int CENTERID = 5;

int main(int argc, char const *argv[])
{
    SetPlotStyle();

    if (argc != 6) return -1;
    string InputDir(argv[1]);
    int decayID=atoi(argv[2]);
    string tag(argv[3]);
    int channelID = atoi(argv[4]);
    int sqrts = atoi(argv[5]);

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
    double NEVENTS_SIG[N_SIGCAT];
    double NEVENTS_BKG[N_SIGCAT];

    sprintf(temp,"%s/BDTResult_Diff.root",dir.c_str());
    TFile *f2 = new TFile(temp);
    sprintf(temp,"%s/BDTResult_Diff_PDF.root",dir.c_str());
    TFile *f3 = new TFile(temp,"RECREATE");
    int NBINS = 20;
    RooRealVar roo_shat("shat","shat",SHATMIN3000[channelID-1],SHATMAX3000[channelID-1]);
    roo_shat.setBins(NBINS);
    // RooRealVar roo_weight("Weight","Weight",0,1);
    // RooArgSet testset = RooArgSet(roo_shat,roo_weight);

    // RooDataSet *roo_data[N_SIGCAT];
    RooDataHist *roo_datahist_SIG[N_SIGCAT];
    RooHistPdf *roo_pdfhist_SIG[N_SIGCAT]; 
    RooDataHist *roo_datahist_BKG[N_SIGCAT];
    RooHistPdf *roo_pdfhist_BKG[N_SIGCAT]; 
    RooDataHist *roo_datahist_TOT[N_SIGCAT];
    RooHistPdf *roo_pdfhist_TOT[N_SIGCAT]; 
    // RooPlot *roo_pdfframe = roo_shat.frame();
    TH1F *roo_Hist_SIG[N_SIGCAT];
    TH1F *roo_Hist_BKG[N_SIGCAT];
    TH1F *roo_Hist_TOT[N_SIGCAT];
    for (int i = 0; i < N_SIGCAT; i++)
    {
        sprintf(temp,"Hist_SIG_%s",SIG_NAME[channelID-1][i].c_str());
        roo_Hist_SIG[i] = (TH1F*) f2->Get(temp);
        NEVENTS_SIG[i] = roo_Hist_SIG[i]->Integral();
        // sprintf(temp,"BDTResult_Diff_%s",SIG_NAME[channelID-1][i].c_str());
        // tshat[i] = (TTree*) f2->Get(temp);
        sprintf(temp,"%s_SIG",SIG_NAME[channelID-1][i].c_str());
        // roo_data[i] = new RooDataSet(temp,"",testset,Import(*tshat[i]),WeightVar(roo_weight));
        // roo_datahist[i] = roo_data[i]->binnedClone();
        roo_datahist_SIG[i] = new RooDataHist(temp,"",roo_shat,roo_Hist_SIG[i]);
        sprintf(temp,"%s_SIG_pdf",SIG_NAME[channelID-1][i].c_str());
        roo_pdfhist_SIG[i] = new RooHistPdf(temp,temp,roo_shat,*roo_datahist_SIG[i]);
        // roo_pdfhist_SIG[i].plotOn(roo_pdfframe);

        sprintf(temp,"Hist_BKG_%s",SIG_NAME[channelID-1][i].c_str());
        roo_Hist_BKG[i] = (TH1F*) f2->Get(temp);
        NEVENTS_BKG[i] = roo_Hist_BKG[i]->Integral();
        sprintf(temp,"%s_BKG",SIG_NAME[channelID-1][i].c_str());
        roo_datahist_BKG[i] = new RooDataHist(temp,"",roo_shat,roo_Hist_BKG[i]);
        sprintf(temp,"%s_BKG_pdf",SIG_NAME[channelID-1][i].c_str());
        roo_pdfhist_BKG[i] = new RooHistPdf(temp,temp,roo_shat,*roo_datahist_BKG[i]);

        sprintf(temp,"Hist_TOT_%s",SIG_NAME[channelID-1][i].c_str());
        roo_Hist_TOT[i] = (TH1F*) f2->Get(temp);
        sprintf(temp,"%s_TOT",SIG_NAME[channelID-1][i].c_str());
        roo_datahist_TOT[i] = new RooDataHist(temp,"",roo_shat,roo_Hist_TOT[i]);
        sprintf(temp,"%s_TOT_pdf",SIG_NAME[channelID-1][i].c_str());
        roo_pdfhist_TOT[i] = new RooHistPdf(temp,temp,roo_shat,*roo_datahist_TOT[i]);
    }
    // TCanvas *c1 = new TCanvas("c1","",1000,800);
    // roo_pdfframe->Draw();
    // f3->cd();
    // c1->Write();
    // f3->Close();

    RooArgList *shapes[N_SIGCAT];
    RooArgList *yields[N_SIGCAT];
    RooAddPdf *totalPdf[N_SIGCAT];
    RooRealVar *sig_yeild[N_SIGCAT];
    RooRealVar *bkg_yeild[N_SIGCAT];
    for (int isig = 0; isig < N_SIGCAT; isig++)
    {
        sprintf(temp,"Shape_%s",SIG_NAME[channelID-1][isig].c_str());
        shapes[isig] = new RooArgList(temp);
        shapes[isig]->add(*roo_pdfhist_SIG[isig]);
        shapes[isig]->add(*roo_pdfhist_BKG[isig]);
        sprintf(temp,"Yeild_SIG_%s",SIG_NAME[channelID-1][isig].c_str());
        sig_yeild[isig] = new RooRealVar(temp,"",NEVENTS_SIG[isig]);
        sprintf(temp,"Yeild_BKG_%s",SIG_NAME[channelID-1][isig].c_str());
        bkg_yeild[isig] = new RooRealVar(temp,"",NEVENTS_BKG[isig]);
        cout<<"Sig-"<<isig<<": "<<NEVENTS_SIG[isig]<<", "<<NEVENTS_BKG[isig]<<endl;
        sprintf(temp,"Yeild_%s",SIG_NAME[channelID-1][isig].c_str());
        yields[isig] = new RooArgList(temp);
        yields[isig]->add(*sig_yeild[isig]);
        yields[isig]->add(*bkg_yeild[isig]);
        sprintf(temp,"Total_PDF_%s",SIG_NAME[channelID-1][isig].c_str());
        totalPdf[isig] = new RooAddPdf(temp,"",*shapes[isig],*yields[isig]); 
    }

    int NTRIALS = 500;
    double NLL[N_SIGCAT];
    double Delta_NLL[N_SIGCAT];
    for (int isig = 0; isig < N_SIGCAT; isig++)
    {
        NLL[isig] = 0;
        cout << "For Signal-"<<isig<<": "<<endl;
        for (int i = 0; i < NTRIALS; i++)
        {
            if((i+1)%100==0) { cout << "\tTRIALS: "<<(i+1)<<"\r"; cout.flush(); }
            RooDataSet* roo_testdata = roo_pdfhist_TOT[CENTERID]->generate(roo_shat,NEVENTS_SIG[CENTERID]+NEVENTS_BKG[CENTERID]);
            NLL[isig] += (totalPdf[isig]->createNLL(*roo_testdata,Extended(true)))->getVal();
            delete roo_testdata;
        }
        cout<<endl;
        NLL[isig]/=NTRIALS;
    }
    for (int isig = 0; isig < N_SIGCAT; isig++)
    {
        Delta_NLL[isig] = NLL[isig] - NLL[CENTERID];
    }
    TGraph *g1 = new TGraph(N_SIGCAT,lwzValue,Delta_NLL);
    f3->cd();
    g1->Write();

    for (int isig = 0; isig < N_SIGCAT; isig++)
    {
        delete totalPdf[isig];
        shapes[isig]->removeAll();
        yields[isig]->removeAll();
        delete sig_yeild[isig];
        delete bkg_yeild[isig];
        delete shapes[isig];
        delete yields[isig];
        delete roo_pdfhist_TOT[isig];
        delete roo_datahist_TOT[isig];
        delete roo_Hist_TOT[isig];
        delete roo_pdfhist_BKG[isig];
        delete roo_datahist_BKG[isig];
        delete roo_Hist_BKG[isig];
        delete roo_pdfhist_SIG[isig];
        delete roo_datahist_SIG[isig];
    }
//    f2->Close();
//    f3->Close();
    cout<<"Program Exit!"<<endl;
    cout<<"=================================>"<<endl;
    return 0;
}
