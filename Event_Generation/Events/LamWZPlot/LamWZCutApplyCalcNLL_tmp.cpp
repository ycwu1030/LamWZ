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
// double BDTScoreCuts3000[2] = {-0.005,0.055};
// double BDTScoreCuts1500[2] = {0.035,0.115};
// double SHATMIN3000[2] = {100,100};
// double SHATMAX3000[2] = {1200,1800};
double SHATRANGE[2] = {100,__SQRTS__};
double lwzValue[N_SIGCAT] = {__SIGLWZ__};
int CENTERID = __CENTERID__;

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
    sst<<dirtop<<"/Diff_LamWZ_Ana_"<<tag;
    sst>>dir;
    createdir(dir.c_str());


    char temp[500];
    double NEVENTS_SIG[N_SIGCAT];
    double NEVENTS_BKG[N_SIGCAT];

    sprintf(temp,"%s/Diff_LamWZ_Results.root",dir.c_str());
    TFile *f2 = new TFile(temp);
    sprintf(temp,"%s/Diff_LamWZ_Results_PDF.root",dir.c_str());
    TFile *f3 = new TFile(temp,"RECREATE");
    int NBINS = 50;
    RooRealVar roo_shat("shat","shat",SHATRANGE[0],SHATRANGE[1]);
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
        sprintf(temp,"Hist_SIG_%s",SIG_NAME[i].c_str());
        roo_Hist_SIG[i] = (TH1F*) f2->Get(temp);
        NEVENTS_SIG[i] = roo_Hist_SIG[i]->Integral();
        // sprintf(temp,"BDTResult_Diff_%s",SIG_NAME[channelID-1][i].c_str());
        // tshat[i] = (TTree*) f2->Get(temp);
        sprintf(temp,"%s_SIG",SIG_NAME[i].c_str());
        // roo_data[i] = new RooDataSet(temp,"",testset,Import(*tshat[i]),WeightVar(roo_weight));
        // roo_datahist[i] = roo_data[i]->binnedClone();
        roo_datahist_SIG[i] = new RooDataHist(temp,"",roo_shat,roo_Hist_SIG[i]);
        sprintf(temp,"%s_SIG_pdf",SIG_NAME[i].c_str());
        roo_pdfhist_SIG[i] = new RooHistPdf(temp,temp,roo_shat,*roo_datahist_SIG[i]);
        // roo_pdfhist_SIG[i].plotOn(roo_pdfframe);

        sprintf(temp,"Hist_BKG_%s",SIG_NAME[i].c_str());
        roo_Hist_BKG[i] = (TH1F*) f2->Get(temp);
        NEVENTS_BKG[i] = roo_Hist_BKG[i]->Integral();
        sprintf(temp,"%s_BKG",SIG_NAME[i].c_str());
        roo_datahist_BKG[i] = new RooDataHist(temp,"",roo_shat,roo_Hist_BKG[i]);
        sprintf(temp,"%s_BKG_pdf",SIG_NAME[i].c_str());
        roo_pdfhist_BKG[i] = new RooHistPdf(temp,temp,roo_shat,*roo_datahist_BKG[i]);

        sprintf(temp,"Hist_TOT_%s",SIG_NAME[i].c_str());
        roo_Hist_TOT[i] = (TH1F*) f2->Get(temp);
        sprintf(temp,"%s_TOT",SIG_NAME[i].c_str());
        roo_datahist_TOT[i] = new RooDataHist(temp,"",roo_shat,roo_Hist_TOT[i]);
        sprintf(temp,"%s_TOT_pdf",SIG_NAME[i].c_str());
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
        sprintf(temp,"Shape_%s",SIG_NAME[isig].c_str());
        shapes[isig] = new RooArgList(temp);
        shapes[isig]->add(*roo_pdfhist_SIG[isig]);
        shapes[isig]->add(*roo_pdfhist_BKG[isig]);
        sprintf(temp,"Yeild_SIG_%s",SIG_NAME[isig].c_str());
        sig_yeild[isig] = new RooRealVar(temp,"",NEVENTS_SIG[isig]);
        sprintf(temp,"Yeild_BKG_%s",SIG_NAME[isig].c_str());
        bkg_yeild[isig] = new RooRealVar(temp,"",NEVENTS_BKG[isig]);
        cout<<"Sig-"<<isig<<": "<<NEVENTS_SIG[isig]<<", "<<NEVENTS_BKG[isig]<<endl;
        sprintf(temp,"Yeild_%s",SIG_NAME[isig].c_str());
        yields[isig] = new RooArgList(temp);
        yields[isig]->add(*sig_yeild[isig]);
        yields[isig]->add(*bkg_yeild[isig]);
        sprintf(temp,"Total_PDF_%s",SIG_NAME[isig].c_str());
        totalPdf[isig] = new RooAddPdf(temp,"",*shapes[isig],*yields[isig]); 
    }

    int NTRIALS = 5000;
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
