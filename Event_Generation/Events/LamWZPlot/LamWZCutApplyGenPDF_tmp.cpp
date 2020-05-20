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
    TChain *Chaintotal[N_SIGCAT];
    TChain *ChainBKG = new TChain("LamWZPreAna");
    TH1F *HistSHAT_SIG[N_SIGCAT];
    TH1F *HistSHAT_BKG[N_SIGCAT];
    TH1F *HistSHAT_TOT[N_SIGCAT];
    LamWZPreAna *ch[N_SIGCAT];
    LamWZPreAna *chbkg;
    TTree *tshat_SIG[N_SIGCAT];
    TTree *tshat_BKG;

    int ProcessID;
    int SorB;
    double Weight;
    Int_t NBJet, NLep_Af;
    Float_t FLepEta, MovedEta, HT, MET, Mbb, Mll, AnglebV, shat;

    auto hardcut = [&](){
        return __CUTS__;
    };
    string CUTS_STR = "__CUTS__";
    // TChain *Chaintotal = new TChain("LamWZPreAna");
    for (int i = 0; i < N_BKGCAT; ++i)
    {
        sprintf(temp,"%s/%s_bkg*.root",InputDir.c_str(),BKG_NAME[i].c_str());
        ChainBKG -> Add(temp);
    }
    chbkg = new LamWZPreAna(ChainBKG);
    // sprintf(temp,"BDTResult_BKG_Diff_%s",SIG_NAME[channelID-1][i].c_str());
    tshat_BKG = new TTree("CutResult_BKG_Diff","shat storage for different lamWZ");
    tshat_BKG->Branch("shat",&shat,"shat/F");
    tshat_BKG->Branch("Weight",&Weight,"Weight/F");
    for (int i = 0; i < N_SIGCAT; i++)
    {
        NEVENTS_BKG[i] = 0;
        sprintf(temp,"Hist_BKG_%s",SIG_NAME[i].c_str());
        HistSHAT_BKG[i] = new TH1F(temp,"",50,SHATRANGE[0],SHATRANGE[1]);
        sprintf(temp,"Hist_TOT_%s",SIG_NAME[i].c_str());
        HistSHAT_TOT[i] = new TH1F(temp,"",50,SHATRANGE[0],SHATRANGE[1]);
    }
    for (int i = 0; i < N_SIGCAT; ++i)
    {
        Chaintotal[i] = new TChain("LamWZPreAna");
        sprintf(temp,"%s/%s_Full*.root",InputDir.c_str(),SIG_NAME[i].c_str());
        Chaintotal[i] -> Add(temp);
        ch[i] = new LamWZPreAna(Chaintotal[i]);
        sprintf(temp,"CutResult_SIG_Diff_%s",SIG_NAME[i].c_str());
        tshat_SIG[i] = new TTree(temp,"shat storage for different lamWZ");
        tshat_SIG[i]->Branch("shat",&shat,"shat/F");
        tshat_SIG[i]->Branch("Weight",&Weight,"Weight/F");
        NEVENTS_SIG[i] = 0;
        sprintf(temp,"Hist_SIG_%s",SIG_NAME[i].c_str());
        HistSHAT_SIG[i] = new TH1F(temp,"",50,SHATRANGE[0],SHATRANGE[1]);
    }

    sprintf(temp,"%s/Diff_LamWZ_Results.root",dir.c_str());
    TFile *f2 = new TFile(temp,"RECREATE");

    
    // --------------------------------------------------------------
    Int_t Good=true;
    int id; // Histogram ID;
    Int_t nentries = Int_t(ChainBKG->GetEntries());
    printf("%d bkg entries to be processed!\n",nentries);
    for (int entry = 0; entry < nentries; ++entry)
    {
        if((entry+1)%100000==0) {cout<<entry+1<<" entries processed...\r"; cout.flush();}
        chbkg->GetEntry(entry);
        ProcessID = chbkg->processID;
        Weight = chbkg->CS*LUMINOSITY/((double)Bkg_NTOTAL[ProcessID]);
        if (channelID == 1)
        {
            NBJet = chbkg->NBJet;
            NLep_Af = chbkg->NLep_Af;
            FLepEta = chbkg->FLepEta;
            MovedEta = FLepEta<0?FLepEta+6:FLepEta;
            HT = chbkg->HT;
            MET = chbkg->MET;
            Mll = chbkg->Mll;
            Mbb = chbkg->Mbb;
            AnglebV = chbkg->AnglebV;
            shat = chbkg->shat;
        }
        else if (channelID == 2)
        {
            NBJet = chbkg->NBJet;
            NLep_Af = chbkg->NLep_Af;
            HT = chbkg->HT;
            MET = chbkg->MET;
            Mll = chbkg->Mll;
            Mbb = chbkg->Mbb;
            AnglebV = chbkg->AnglebV;
            shat = chbkg->shat;
        }
        Good = (chbkg->NBJet == 2 && chbkg->NLep_Af==2)&&hardcut();
        if(!Good) continue;
        tshat_BKG->Fill();
        for (int isig = 0; isig < N_SIGCAT; isig++)
        {
            NEVENTS_BKG[isig] += Weight;
            HistSHAT_BKG[isig]->Fill(shat,Weight);   
            HistSHAT_TOT[isig]->Fill(shat,Weight);
        }
        // t2->Fill();
    }
    f2->cd();
    for (int isig = 0; isig < N_SIGCAT; isig++)
    {
        HistSHAT_BKG[isig]->Write();
    }
    tshat_BKG->Write();

    for (int isig = 0; isig < N_SIGCAT; isig++)
    {
        nentries = Int_t(Chaintotal[isig]->GetEntries());
        printf("%d signal entries to be processed!\n",nentries);
        for (int entry = 0; entry < nentries; ++entry)
        {
            if((entry+1)%100000==0) {cout<<entry+1<<" entries processed...\r"; cout.flush();}
            ch[isig]->GetEntry(entry);
            SorB = ch[isig]->SorB;
            ProcessID = ch[isig]->processID;
            Weight = ch[isig]->CS*LUMINOSITY/((double)Sig_NTOTAL[ProcessID]);
            if (channelID == 1)
            {
                NBJet = ch[isig]->NBJet;
                NLep_Af = ch[isig]->NLep_Af;
                FLepEta = ch[isig]->FLepEta;
                MovedEta = FLepEta<0?FLepEta+6:FLepEta;
                HT = ch[isig]->HT;
                MET = ch[isig]->MET;
                Mll = ch[isig]->Mll;
                Mbb = ch[isig]->Mbb;
                AnglebV = ch[isig]->AnglebV;
                shat = ch[isig]->shat;
            }
            else if (channelID == 2)
            {
                NBJet = ch[isig]->NBJet;
                NLep_Af = ch[isig]->NLep_Af;
                HT = ch[isig]->HT;
                MET = ch[isig]->MET;
                Mll = ch[isig]->Mll;
                Mbb = ch[isig]->Mbb;
                AnglebV = ch[isig]->AnglebV;
                shat = ch[isig]->shat;
            }
            Good = (ch[isig]->NBJet == 2 && ch[isig]->NLep_Af==2)&&hardcut();
            if(!Good) continue;
            NEVENTS_SIG[isig] += Weight;
            tshat_SIG[isig]->Fill(); 
            HistSHAT_SIG[isig]->Fill(shat,Weight);  
            HistSHAT_TOT[isig]->Fill(shat,Weight);   
            // t2->Fill();
        }
        f2->cd();
        tshat_SIG[isig]->Write();
        HistSHAT_SIG[isig]->Write();
        HistSHAT_TOT[isig]->Write();
    }
    f2->Close();

    
    cout<<"Program Exit!"<<endl;
    cout<<"=================================>"<<endl;
    return 0;
}