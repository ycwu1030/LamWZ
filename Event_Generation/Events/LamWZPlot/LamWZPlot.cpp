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
const int LUMINOSITY = 300; //fb^-1
const int N_SIGCAT = 9;
const int N_BKGCAT = 3;
const int N_VARIABLE = 30;
string DecayChannel[2] = {"bbll","tatall"};
string SIG_NAME[2][N_SIGCAT] = {{"wh_Full_lwz0x6","wh_Full_lwz0x65","wh_Full_lwz0x7","wh_Full_lwz0x75","wh_Full_lwz0x8","wh_Full_lwz1x0","wh_Full_lwz1x2","wh_Full_lwz1x4","wh_Full_lwz1x6"},{"zh_Full_lwz0x6","zh_Full_lwz0x65","zh_Full_lwz0x7","zh_Full_lwz0x75","zh_Full_lwz0x8","zh_Full_lwz1x0","zh_Full_lwz1x2","zh_Full_lwz1x4","zh_Full_lwz1x6"}};
string BKG_NAME[N_BKGCAT] = {"tt","wz","zz"};
string SIG_LABEL[2][N_SIGCAT] = {{"Wh(#lambda_{WZ}=0.6)","Wh(#lambda_{WZ}=0.65)","Wh(#lambda_{WZ}=0.7)","Wh(#lambda_{WZ}=0.75)","Wh(#lambda_{WZ}=0.8)","Wh(#lambda_{WZ}=1.0)","Wh(#lambda_{WZ}=1.2)","Wh(#lambda_{WZ}=1.4)","Wh(#lambda_{WZ}=1.6)"},{"Zh(#lambda_{WZ}=0.6)","Zh(#lambda_{WZ}=0.65)","Zh(#lambda_{WZ}=0.7)","Zh(#lambda_{WZ}=0.75)","Zh(#lambda_{WZ}=0.8)","Zh(#lambda_{WZ}=1.0)","Zh(#lambda_{WZ}=1.2)","Zh(#lambda_{WZ}=1.4)","Zh(#lambda_{WZ}=1.6)"}};
string BKG_LABEL[N_BKGCAT] = {"t#bar{t}","WZ","ZZ"};


int main(int argc, char const *argv[])
{
    // Set Plot Style;
    SetPlotStyle();
    TChain *ch = new TChain("LamWZPreAna");

    if (argc != 5) return -1;
    string InputDir(argv[1]);
    int decayID=atoi(argv[2]);
    string tag(argv[3]);
    int channelID=atoi(argv[4]);

    LoadingRootFile(ch,InputDir);
    LamWZPreAna *LamWZch = new LamWZPreAna(ch);

    // Initial the Ploting System;
    binning *bn = new binning();
    LamWZch->initialize(bn);

    #include "HistogramDeclare.inc"


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
        #include "HistogramFilling.inc"
    }
    cout<<"All Events have been processed..."<<endl;
    cout<<"Event Loop Over"<<endl;
    cout<<endl;

    //Plotting the Histogram;
    cout<<"Collect Histograms..."<<endl;
    int bkgcolor[N_BKGCAT]={kOrange+8,kSpring-3,kCyan-4};//,kAzure-3,kMagenta-4,kMagenta+3};
    // int sigcolor[N_SIGCAT]={kBlack,kGray+2,kGray,kBlack,kRed+4,kRed-1,kRed-5,kRed-8,kRed-10};
    int sigcolor[N_SIGCAT]={kBlue+4,kBlue+3,kBlue+2,kBlue+1,kBlue,kBlue-4,kBlue-7,kBlue-9,kBlue-10};

    string dir,dirtop;
    stringstream sst,ssttop;
    ssttop<<DIR;
    ssttop>>dirtop;
    createdir(dirtop.c_str());
    sst<<dirtop<<"/Plots_LamWZ_"<<tag;
    sst>>dir;
    createdir(dir.c_str());

    #include "HistogramOutput.inc"

    cout<<"Program Exit!"<<endl;
    cout<<"========================>>"<<endl;
    return 0;
}
