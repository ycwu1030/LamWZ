#include "plotheader.h"
#include "LamWZPreAna.h"
#include "TStyle.h"
#include "TH2F.h"
#include "Utilities.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

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
// int sigcolor[N_SIGCAT]={kBlack,kGray+2,kGray,kBlack,kRed+4,kRed-1,kRed-5,kRed-8,kRed-10};
int sigcolor[9]={kBlue+4,kBlue+3,kBlue+2,kBlue+1,kBlue,kBlue-4,kBlue-7,kBlue-9,kBlue-10};

int main(int argc, char const *argv[])
{
    // Set Plot Style;
    SetPlotStyle();
    TChain *ch = new TChain("LamWZPreAna");

    string InputDir(argv[1]);
    string tag(argv[2]);
    int Renormal = atoi(argv[3]); //0 keep the weight, 1: renormalize to 1.

    LoadingRootFile(ch,InputDir);
    LamWZPreAna *LamWZch = new LamWZPreAna(ch);

    // Initial the Ploting System;
    binning *bn = new binning();
    LamWZch->initialize(bn);

    #include "HistogramDeclare_tmp.inc"


//Event Loop
    Int_t nentries = Int_t(ch->GetEntries());
    double valueX,valueY;
    int SorB, id;
    int bid,cat;
    double weight;
    Float_t F_NB, F_NL, F_FLepEta, F_MovedEta, F_HT, F_MET, F_Mbb, F_Mll, F_AnglebV, F_shat;
    auto hardcut = [&](){
        return __CUTS__;
    };
    bool good;
    printf("%d entries to be processed!\n",nentries);
    for (Int_t entry = 0; entry < nentries; ++entry)
    {
        //Load the entry and set required variables;
        LamWZch->GetEntry(entry);
        if((entry+1)%100000==0) {cout<<entry+1<<" entries processed...\r"; cout.flush();}
        // cout<<entry<<endl;
        SorB = LamWZch->SorB;
        id = LamWZch->processID;
        F_NB = LamWZch->NBJet;
        F_NL = LamWZch->NLep_Af;
        F_FLepEta = LamWZch->FLepEta;
        F_MovedEta = F_FLepEta<0?F_FLepEta+6:F_FLepEta;
        F_HT = LamWZch->HT;
        F_MET = LamWZch->MET;
        F_Mll = LamWZch->Mll;
        F_Mbb = LamWZch->Mbb;
        F_AnglebV = LamWZch->AnglebV;
        F_shat = LamWZch->shat;
        good = hardcut();
        if (!good) continue;
        if (SorB == 1)
        {
            weight = LamWZch->CS*LUMINOSITY/((double)Sig_NTOTAL[id]);
        }
        else
        {
            weight = LamWZch->CS*LUMINOSITY/((double)Bkg_NTOTAL[id]);
        }
        #include "HistogramFilling.inc"
    }
    cout<<"All Events have been processed..."<<endl;
    cout<<"Event Loop Over"<<endl;
    cout<<endl;

    //Plotting the Histogram;
    cout<<"Collect Histograms..."<<endl;

    string dir,dirtop;
    stringstream sst,ssttop;
    ssttop<<DIR;
    ssttop>>dirtop;
    createdir(dirtop.c_str());
    sst<<dirtop<<"/Plots_LamWZ_"<<tag;
    sst>>dir;
    createdir(dir.c_str());

    #include "HistogramOutput_tmp.inc"

    cout<<"Program Exit!"<<endl;
    cout<<"========================>>"<<endl;
    return 0;
}
