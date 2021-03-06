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
    TChain *Chaintotal = new TChain("LamWZPreAna");
    for (int i = 0; i < N_SIGCAT; ++i)
    {
        sprintf(temp,"%s/%s_Full*.root",InputDir.c_str(),SIG_NAME[i].c_str());
        Chaintotal -> Add(temp);
        // LamWZPreAna *ch = new LamWZPreAna(ChainSIG[i]);
        // ch->GetEntry(0);
        // WeightSIG[i] = ch->CS*LUMINOSITY/((double)Sig_NTOTAL[i]);
    }
    for (int i = 0; i < N_BKGCAT; ++i)
    {
        sprintf(temp,"%s/%s_bkg*.root",InputDir.c_str(),BKG_NAME[i].c_str());
        Chaintotal -> Add(temp);
        // LamWZPreAna *ch = new LamWZPreAna(ChainBKG[i]);
        // ch->GetEntry(0);
        // WeightBKG[i] = ch->CS*LUMINOSITY/((double)Bkg_NTOTAL[i]);
    }
    LamWZPreAna *ch = new LamWZPreAna(Chaintotal);

    TMVA::Tools::Instance();
    sprintf(temp,"%s/BDTResult.root",dir.c_str());
    TFile *f2 = new TFile(temp,"RECREATE");
    TTree *t2 = new TTree("BDTResult","The results from BDT");
    double BDTScore;
    int processID;
    int SorB;
    double Weight;

    t2->Branch("processID",&processID,"processID/I");
    t2->Branch("SorB",&SorB,"SorB/I");
    t2->Branch("Weight",&Weight,"Weight/D");
    t2->Branch("BDTScore",&BDTScore,"BDTScore/D");

    TMVA::Reader *reader = new TMVA::Reader("!Color:Silent");

    Float_t F_FLepEta, F_HT, F_MET, F_Mbb, F_Mll, F_AnglebV, F_shat;

    if (channelID == 1)
    {
        reader->AddVariable( "FLepEta", &F_FLepEta );
        reader->AddVariable( "HT", &F_HT );
        reader->AddVariable( "MET", &F_MET );
        reader->AddVariable( "Mbb", &F_Mbb );
        reader->AddVariable( "Mll", &F_Mll );
        reader->AddVariable( "AnglebV", &F_AnglebV );
        reader->AddVariable( "shat", &F_shat );
    }
    else if (channelID == 2)
    {
        reader->AddVariable( "HT", &F_HT );
        reader->AddVariable( "MET", &F_MET );
        reader->AddVariable( "Mbb", &F_Mbb );
        reader->AddVariable( "Mll", &F_Mll );
        reader->AddVariable( "AnglebV", &F_AnglebV );
        reader->AddVariable( "shat", &F_shat );
    }
    
    TCut precuts = "NBJet==2&&NLep_Af==2";
    string methodName = "BDT method";
    string weightfile = dir + "/TMVAClassification_BDT.weights.xml";
    reader->BookMVA(methodName,weightfile);

    // --------------------------------------------------------------
    Int_t nentries = Int_t(Chaintotal->GetEntries());
    Int_t Good=true;
    // int SorB; // 1 for Sig, 0 for Bkg;
    int id; // Histogram ID;
    printf("%d entries to be processed!\n",nentries);
    for (int entry = 0; entry < nentries; ++entry)
    {
        if((entry+1)%100000==0) {cout<<entry+1<<" entries processed...\r"; cout.flush();}
        ch->GetEntry(entry);
        Good = (ch->NBJet == 2 && ch->NLep_Af==2);
        if(!Good) continue;
        SorB = ch->SorB;
        processID = ch->processID;
        if (SorB == 1)
        {
            Weight = ch->CS*LUMINOSITY/((double)Sig_NTOTAL[processID]);
        }
        else
        {
            Weight = ch->CS*LUMINOSITY/((double)Bkg_NTOTAL[id]);
        }
        if (channelID == 1)
        {
            F_FLepEta = ch->FLepEta;
            F_HT = ch->HT;
            F_MET = ch->MET;
            F_Mll = ch->Mll;
            F_Mbb = ch->Mbb;
            F_AnglebV = ch->AnglebV;
            F_shat = ch->shat;
        }
        else if (channelID == 2)
        {
            F_HT = ch->HT;
            F_MET = ch->MET;
            F_Mll = ch->Mll;
            F_Mbb = ch->Mbb;
            F_AnglebV = ch->AnglebV;
            F_shat = ch->shat;
        }
        BDTScore = reader->EvaluateMVA("BDT method");
        t2->Fill();
    }


    f2->cd();
    t2->Write();

    delete t2;
    f2->Close();
    cout<<"Program Exit!"<<endl;
    cout<<"=================================>"<<endl;
    return 0;
}