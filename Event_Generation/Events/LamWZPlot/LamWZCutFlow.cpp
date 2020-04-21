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

using namespace std;
string DIR = "/data/data068/ycwu/LamWZ/Event_Generation/Events/Plots";
const int LUMINOSITY = 4000; //fb^-1
const int N_SIGCAT = 4;
const int N_BKGCAT = 3;
const int N_VARIABLE = 30;
string DecayChannel[2] = {"bbll","tatall"};
string SIG_NAME[N_SIGCAT] = {"wh_hww","wh_hzz","wh_Inter","wh_Full"};
// string SIG_NAME[N_SIGCAT] = {"wh_Full"};
string BKG_NAME[N_BKGCAT] = {"tt","wz","zz"};
string SIG_LABEL[N_SIGCAT] = {"Wh hWW","Wh hZZ","Wh Interference","Wh Full"};
// string SIG_LABEL[N_SIGCAT] = {"Wh Full"};
string BKG_LABEL[N_BKGCAT] = {"t#bar{t}","WZ","ZZ"};
int Sig_NTOTAL[N_SIGCAT] = {247244,247138,247328,750000};
// int Sig_NTOTAL[N_SIGCAT] = {7500000};
int Bkg_NTOTAL[N_BKGCAT] = {2500000,2500000,2500000};

int main(int argc, char const *argv[])
{
    // Set Plot Style;
    SetPlotStyle();
    TChain *ch = new TChain("LamWZPreAna");

    if (argc != 4) return -1;
    string InputDir(argv[1]);
    int decayID=atoi(argv[2]);
    string tag(argv[3]);

    LoadingRootFile(ch,InputDir);
    LamWZPreAna *LamWZch = new LamWZPreAna(ch);
//Setup the cuts to be scanned:
    CutList *CutsFlow = new CutList();
    CutsFlow->addCut(&LamWZch->NBJet,"NB",2,2,0,2,2,0);
    CutsFlow->addCut(&LamWZch->NLep_Af,"NL",2,2,0,2,2,0);
    CutsFlow->addCut(&LamWZch->HT,"HT",400,600,4,650,1500,4);
    CutsFlow->addCut(&LamWZch->Mbb,"Mbb",90,110,4,130,150,2);
    CutsFlow->addCut(&LamWZch->Mll,"Mll",100,110,2,-1,-1,-1);
    CutsFlow->addCut(&LamWZch->AnglebV,"AnglebV",0.8,1.5,4,2.6,2.9,4);
//Event Loop
    Int_t nentries = Int_t(ch->GetEntries());
    double valueX,valueY;
    int SorB, id;
    int bid,cat;
    double *Sig_CS = new double[CutsFlow->NCuts];
    double *Bkg_CS = new double[CutsFlow->NCuts];
    int *Sig_MC_N = new int[CutsFlow->NCuts];
    int *Bkg_MC_N = new int[CutsFlow->NCuts];
    printf("%d entries to be processed!\n",nentries);
    bool good;
    double CS;
    for (Int_t entry = 0; entry < nentries; ++entry)
    {
        //Load the entry and set required variables;
        LamWZch->GetEntry(entry);
        cat = LamWZch->Cate;
        CS = LamWZch->CS;
        if((entry+1)%100000==0) {cout<<entry+1<<" entries processed...\r"; cout.flush();}
        // cout<<entry<<endl;
        GetHistID(cat,SorB,id);
        for (int icut = 0; icut < CutsFlow->NCuts; ++icut)
        {
            if (entry == 0)
            {
                Sig_CS[icut] = 0.0;
                Bkg_CS[icut] = 0.0;
                Sig_MC_N[icut] = 0;
                Bkg_MC_N[icut] = 0;
            }
            good = CutsFlow->Apply(icut);
            if (!good) continue;
            if (SorB == 1)
            {
                Sig_CS[icut] += CS/((double)Sig_NTOTAL[id]);
                Sig_MC_N[icut] += 1;
            }
            else
            {
                Bkg_CS[icut] += CS/((double)Bkg_NTOTAL[id]);
                Bkg_MC_N[icut] += 1;
            }
        }

    }
    cout<<"All Events have been processed..."<<endl;
    cout<<"Event Loop Over"<<endl;
    cout<<endl;

    //Plotting the Histogram;
    cout<<"Collect Cuts Results..."<<endl;
    int bkgcolor[N_BKGCAT]={kOrange+8,kSpring-3,kCyan-4};//,kAzure-3,kMagenta-4,kMagenta+3};
    int sigcolor[N_SIGCAT]={kBlack,kGray+2,kGray};

    string dir,dirtop;
    stringstream sst,ssttop;
    ssttop<<DIR;
    ssttop>>dirtop;
    createdir(dirtop.c_str());
    sst<<dirtop<<"/Cuts_LamWZ_"<<tag;
    sst>>dir;
    createdir(dir.c_str());
    char temp[200];
    sprintf(temp,"%s/cuts_logs.dat",dir.c_str());
    ofstream cutlog(temp);
    double sigma=0.0;
    double sigmatemp=0.0;
    int bestid=-1;
    for (int i = 0; i < CutsFlow->NCuts; ++i)
    {
        sigmatemp = Sig_CS[i]*sqrt(LUMINOSITY)/sqrt(Sig_CS[i]+Bkg_CS[i]+pow(10,-10));
        cutlog<<i<<"  "<<CutsFlow->CutInfo(i)<<"  "<<Sig_CS[i]<<"/"<<Sig_MC_N[i]<<"  "<<Bkg_CS[i]<<"/"<<Bkg_MC_N[i]<<"  "<<sigmatemp<<endl;
        if (Bkg_MC_N[i]>100&&Sig_MC_N[i]>100)
        {
            if (sigmatemp>sigma)
            {
                sigma = sigmatemp;
                bestid = i;
            }
        }
    }
    cout<<"Best Level: "<<sigma<<", achieved by cut-"<<bestid<<endl;
    cutlong<<"Best Level: "<<sigma<<", achieved by cut-"<<bestid<<endl;
    delete CutsFlow;
    delete[] Sig_CS;
    delete[] Sig_MC_N;
    delete[] Bkg_CS;
    delete[] Bkg_MC_N;
    cout<<"Program Exit!"<<endl;
    cout<<"========================>>"<<endl;
    return 0;
}
