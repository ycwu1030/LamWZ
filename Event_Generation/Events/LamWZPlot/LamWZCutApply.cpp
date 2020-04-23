#include "plotheader.h"
#include "LamWZPreAna.h"
#include "TStyle.h"
#include "TH2F.h"
#include "TGraph.h"
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
const int LUMINOSITY3000 = 4000; //fb^-1
const int LUMINOSITY1500 = 2000; //fb^-1
const int N_SIGCAT = 4;
const int N_BKGCAT = 3;
const int N_VARIABLE = 30;
string DecayChannel[2] = {"bbll","tatall"};
string SIG_NAME[2][N_SIGCAT] = {{"wh_hWW","wh_hZZ","wh_Inter","wh_Full"},{"zh_hWW","zh_hZZ","zh_Inter","zh_Full"}};
string BKG_NAME[N_BKGCAT] = {"tt","wz","zz"};
string SIG_LABEL[2][N_SIGCAT] = {{"Wh hWW","Wh hZZ","Wh Interference","Wh Full"},{"Zh hWW","Zh hZZ","Zh Interference","Zh Full"}};
string BKG_LABEL[N_BKGCAT] = {"t#bar{t}","WZ","ZZ"};
int Sig_NTOTAL3000[2][N_SIGCAT] = {{247244,247138,247328,750000},{249239,245187,245138,525505}};
int Sig_NTOTAL1500[2][N_SIGCAT] = {{247244,247138,247328,750000},{249239,245187,245138,750000}};
int Bkg_NTOTAL[N_BKGCAT] = {2500000,2500000,2500000};

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
    sst<<dirtop<<"/CUT_Train_LamWZ_"<<tag;
    sst>>dir;
    createdir(dir.c_str());


    char temp[500];
    TChain *Chaintotal = new TChain("LamWZPreAna");
    for (int i = 0; i < N_SIGCAT; ++i)
    {
        sprintf(temp,"%s/%s*.root",InputDir.c_str(),SIG_NAME[channelID-1][i].c_str());
        Chaintotal -> Add(temp);
        // LamWZPreAna *ch = new LamWZPreAna(ChainSIG[i]);
        // ch->GetEntry(0);
        // WeightSIG[i] = ch->CS*LUMINOSITY/((double)Sig_NTOTAL[i]);
    }
    for (int i = 0; i < N_BKGCAT; ++i)
    {
        sprintf(temp,"%s/%s*.root",InputDir.c_str(),BKG_NAME[i].c_str());
        Chaintotal -> Add(temp);
        // LamWZPreAna *ch = new LamWZPreAna(ChainBKG[i]);
        // ch->GetEntry(0);
        // WeightBKG[i] = ch->CS*LUMINOSITY/((double)Bkg_NTOTAL[i]);
    }
    LamWZPreAna *ch = new LamWZPreAna(Chaintotal);

    TMVA::Tools::Instance();
    sprintf(temp,"%s/CUTResult.root",dir.c_str());
    TFile *f2 = new TFile(temp,"RECREATE");
    TTree *t2 = new TTree("CUTResult","The results from CUT");
    const int N_EffS_samples = 21;
    int N_EffS = N_EffS_samples;
    double EffSignals[N_EffS_samples];
    int CUTcate[N_EffS_samples];
    double EventsSig[N_EffS_samples];
    double EventsBkg[N_EffS_samples];
    double Significance[N_EffS_samples];
    bool TMVARes;
    double EffSigStart = 0.1, EffSigEnd = 0.9;
    for (int i = 0; i < N_EffS_samples; i++)
    {
        EffSignals[i] = EffSigStart + i*(EffSigEnd-EffSigStart)/(double(N_EffS_samples-1));
        EventsSig[i] = 0;
        EventsBkg[i] = 0;
    }

    int cat;
    int SorB;
    double Weight;

    t2->Branch("Cate",&cat,"Cate/I");
    t2->Branch("SorB",&SorB,"SorB/I");
    t2->Branch("Weight",&Weight,"Weight/D");
    t2->Branch("N_EffS",&N_EffS,"N_EffS/I");
    t2->Branch("EffSignals",EffSignals,"EffSignals[N_EffS]/D");
    t2->Branch("CUTcate",CUTcate,"CUTcate[N_EffS]/I");

    TMVA::Reader *reader = new TMVA::Reader("!Color:Silent");

    Float_t F_FLepEta, F_MovedEta, F_HT, F_MET, F_Mbb, F_Mll, F_AnglebV, F_shat;

    if (channelID == 1)
    {
        reader->AddVariable( "MovedEta := FLepEta<0?FLepEta+6:FLepEta", &F_MovedEta );
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
    string methodName = "CutsGA method";
    string weightfile = dir + "/TMVAClassification_CutsGA.weights.xml";
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
        cat = ch->Cate;
        Good = (ch->NBJet == 2 && ch->NLep_Af==2);
        if(!Good) continue;
        GetHistID(cat,SorB,id);
        if (SorB == 1)
        {
            if (sqrts == 3000)
            {
                Weight = ch->CS*LUMINOSITY3000/((double)Sig_NTOTAL3000[channelID-1][id]);
            }
            else if (sqrts == 1500)
            {
                Weight = ch->CS*LUMINOSITY1500/((double)Sig_NTOTAL1500[channelID-1][id]);
            }
            
        }
        else
        {
            if (sqrts == 3000)
            {
                Weight = ch->CS*LUMINOSITY3000/((double)Bkg_NTOTAL[id]);
            }
            else if (sqrts == 1500)
            {
                Weight = ch->CS*LUMINOSITY1500/((double)Bkg_NTOTAL[id]);
            }
            
        }
        if (channelID == 1)
        {
            F_FLepEta = ch->FLepEta;
            F_MovedEta = F_FLepEta<0?F_FLepEta+6:F_FLepEta;
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
        for (int ieffs = 0; ieffs < N_EffS_samples; ieffs++)
        {
            TMVARes = reader->EvaluateMVA("CutsGA method", EffSignals[ieffs]);
            if (TMVARes)
            {
                CUTcate[ieffs] = 1;
                if (SorB == 1)
                {
                    EventsSig[ieffs] += Weight;
                }
                else
                {
                    EventsBkg[ieffs] += Weight;
                }
                
            }
            else
            {
                CUTcate[ieffs] = 0;
            }   
        }
        // BDTScore = reader->EvaluateMVA("CutsGA method",);
        t2->Fill();
    }
    double sigmax = -1;
    double EffSmax;
    for (int i = 0; i < N_EffS_samples; i++)
    {
        Significance[i] = EventsSig[i]/sqrt(EventsSig[i]+EventsBkg[i]);
        if (Significance[i] > sigmax) { 
            sigmax = Significance[i]; 
            EffSmax = EffSignals[i];
        }
    }
    cout<<"Maximum Significance is: "<<sigmax<<endl;
    cout<<"Achieved at EffS = "<<EffSmax<<endl;
    TGraph *gsig = new TGraph(N_EffS_samples,EffSignals,Significance);
    gsig->SetName("g_significance");

    f2->cd();
    t2->Write();
    gsig->Write();

    delete t2;
    f2->Close();
    cout<<"Program Exit!"<<endl;
    cout<<"=================================>"<<endl;
    return 0;
}