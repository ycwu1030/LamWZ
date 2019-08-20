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
const int N_SIGCAT = 4;
const int N_BKGCAT = 3;
const int N_VARIABLE = 30;
string DecayChannel[2] = {"bbll","tatall"};
string SIG_NAME[2][N_SIGCAT] = {{"wh_hww","wh_hzz","wh_Inter","wh_Full"},{"zh_hww","zh_hzz","zh_Inter","zh_Full"}};
string BKG_NAME[N_BKGCAT] = {"tt","wz","zz"};
string SIG_LABEL[2][N_SIGCAT] = {{"Wh hWW","Wh hZZ","Wh Interference","Wh All"},{"Zh hWW","Zh hZZ","Zh Interference","Zh All"}};
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
    int sigcolor[N_SIGCAT]={kBlack,kGray+2,kGray,kBlack};

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
