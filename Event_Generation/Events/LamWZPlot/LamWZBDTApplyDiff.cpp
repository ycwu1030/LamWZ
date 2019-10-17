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
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooDataHist.h"
#include "RooHistPdf.h"
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
    double NEVENTS[N_SIGCAT];
    TChain *Chaintotal[N_SIGCAT];
    TChain *ChainBKG = new TChain("LamWZPreAna");
    TH1F *HistSHAT[N_SIGCAT];
    LamWZPreAna *ch[N_SIGCAT];
    LamWZPreAna *chbkg;
    TTree *tshat[N_SIGCAT];
    double shat;
    double Weight;
    // TChain *Chaintotal = new TChain("LamWZPreAna");
    for (int i = 0; i < N_BKGCAT; ++i)
    {
        sprintf(temp,"%s/%s*.root",InputDir.c_str(),BKG_NAME[i].c_str());
        ChainBKG -> Add(temp);
    }
    chbkg = new LamWZPreAna(ChainBKG);
    for (int i = 0; i < N_SIGCAT; ++i)
    {
        Chaintotal[i] = new TChain("LamWZPreAna");
        sprintf(temp,"%s/%s_*.root",InputDir.c_str(),SIG_NAME[channelID-1][i].c_str());
        Chaintotal[i] -> Add(temp);
        ch[i] = new LamWZPreAna(Chaintotal[i]);
        sprintf(temp,"BDTResult_Diff_%s",SIG_NAME[channelID-1][i].c_str());
        tshat[i] = new TTree(temp,"shat storage for different lamWZ");
        tshat[i]->Branch("shat",&shat,"shat/D");
        tshat[i]->Branch("Weight",&Weight,"Weight/D");
        NEVENTS[i] = 0;
        sprintf(temp,"Hist_%s",SIG_NAME[channelID-1][i].c_str());
        HistSHAT[i] = new TH1F(temp,"",20,SHATMIN3000[channelID-1],SHATMAX3000[channelID-1]);
    }

    TMVA::Tools::Instance();
    sprintf(temp,"%s/BDTResult_Diff.root",dir.c_str());
    TFile *f2 = new TFile(temp,"RECREATE");
    double BDTScore;
    int cat;
    int SorB;
    

    // t2->Branch("Cate",&cat,"Cate/I");
    // t2->Branch("SorB",&SorB,"SorB/I");
    // t2->Branch("Weight",&Weight,"Weight/D");
    // t2->Branch("BDTScore",&BDTScore,"BDTScore/D");
    // t2->Branch("shat",&shat,"shat/D");

    TMVA::Reader *reader = new TMVA::Reader("!Color:Silent");

    Float_t F_FLepEta, F_HT, F_MET, F_Mbb, F_Mll, F_dRbV, F_shat;

    if (channelID == 1)
    {
        reader->AddVariable( "FLepEta", &F_FLepEta );
        reader->AddVariable( "HT", &F_HT );
        reader->AddVariable( "MET", &F_MET );
        reader->AddVariable( "Mbb", &F_Mbb );
        reader->AddVariable( "Mll", &F_Mll );
        reader->AddVariable( "dRbV", &F_dRbV );
        reader->AddVariable( "shat", &F_shat );
    }
    else if (channelID == 2)
    {
        reader->AddVariable( "HT", &F_HT );
        reader->AddVariable( "MET", &F_MET );
        reader->AddVariable( "Mbb", &F_Mbb );
        reader->AddVariable( "Mll", &F_Mll );
        reader->AddVariable( "dRbV", &F_dRbV );
        reader->AddVariable( "shat", &F_shat );
    }
    
    TCut precuts = "NBJet==2&&NLep_Af==2";
    string methodName = "BDT method";
    string weightfile = dir + "/TMVAClassification_BDT.weights.xml";
    reader->BookMVA(methodName,weightfile);
    // --------------------------------------------------------------
    Int_t Good=true;
    int id; // Histogram ID;
    Int_t nentries = Int_t(ChainBKG->GetEntries());
    printf("%d bkg entries to be processed!\n",nentries);
    for (int entry = 0; entry < nentries; ++entry)
    {
        if((entry+1)%100000==0) {cout<<entry+1<<" entries processed...\r"; cout.flush();}
        chbkg->GetEntry(entry);
        cat = chbkg->Cate;
        Good = (chbkg->NBJet == 2 && chbkg->NLep_Af==2);
        if(!Good) continue;
        GetHistID(cat,SorB,id);
        if (SorB == 1)
        {
            if (sqrts == 3000)
            {
                Weight = chbkg->CS*LUMINOSITY3000/((double)Sig_NTOTAL3000[channelID-1][id]);
            }
            else if (sqrts == 1500)
            {
                Weight = chbkg->CS*LUMINOSITY1500/((double)Sig_NTOTAL1500[channelID-1][id]);
            }
            
        }
        else
        {
            if (sqrts == 3000)
            {
                Weight = chbkg->CS*LUMINOSITY3000/((double)Bkg_NTOTAL[id]);
            }
            else if (sqrts == 1500)
            {
                Weight = chbkg->CS*LUMINOSITY1500/((double)Bkg_NTOTAL[id]);
            }
            
        }
        if (channelID == 1)
        {
            F_FLepEta = chbkg->FLepEta;
            F_HT = chbkg->HT;
            F_MET = chbkg->MET;
            F_Mll = chbkg->Mll;
            F_Mbb = chbkg->Mbb;
            F_dRbV = chbkg->dRbV;
            F_shat = chbkg->shat;
            shat = F_shat;
        }
        else if (channelID == 2)
        {
            F_HT = chbkg->HT;
            F_MET = chbkg->MET;
            F_Mll = chbkg->Mll;
            F_Mbb = chbkg->Mbb;
            F_dRbV = chbkg->dRbV;
            F_shat = chbkg->shat;
            shat = F_shat;
        }
        BDTScore = reader->EvaluateMVA("BDT method");
        if (sqrts == 3000)
        {
            if (BDTScore<BDTScoreCuts3000[channelID-1])
            {
                continue;
            }
        }
        else if (sqrts == 1500)
        {
            if (BDTScore < BDTScoreCuts1500[channelID-1])
            {
                continue;
            }
        }
        for (int isig = 0; isig < N_SIGCAT; isig++)
        {
            NEVENTS[isig] += Weight;
            tshat[isig]->Fill();
            HistSHAT[isig]->Fill(shat,Weight);
        }      
        // t2->Fill();
    }
    for (int isig = 0; isig < N_SIGCAT; isig++)
    {
        nentries = Int_t(Chaintotal[isig]->GetEntries());
        printf("%d signal entries to be processed!\n",nentries);
        for (int entry = 0; entry < nentries; ++entry)
        {
            if((entry+1)%100000==0) {cout<<entry+1<<" entries processed...\r"; cout.flush();}
            ch[isig]->GetEntry(entry);
            cat = ch[isig]->Cate;
            Good = (ch[isig]->NBJet == 2 && ch[isig]->NLep_Af==2);
            if(!Good) continue;
            GetHistID(cat,SorB,id);
            if (SorB == 1)
            {
                if (sqrts == 3000)
                {
                    Weight = ch[isig]->CS*LUMINOSITY3000/((double)Sig_NTOTAL3000[channelID-1][id]);
                }
                else if (sqrts == 1500)
                {
                    Weight = ch[isig]->CS*LUMINOSITY1500/((double)Sig_NTOTAL1500[channelID-1][id]);
                }
                
            }
            else
            {
                if (sqrts == 3000)
                {
                    Weight = ch[isig]->CS*LUMINOSITY3000/((double)Bkg_NTOTAL[id]);
                }
                else if (sqrts == 1500)
                {
                    Weight = ch[isig]->CS*LUMINOSITY1500/((double)Bkg_NTOTAL[id]);
                }
                
            }
            if (channelID == 1)
            {
                F_FLepEta = ch[isig]->FLepEta;
                F_HT = ch[isig]->HT;
                F_MET = ch[isig]->MET;
                F_Mll = ch[isig]->Mll;
                F_Mbb = ch[isig]->Mbb;
                F_dRbV = ch[isig]->dRbV;
                F_shat = ch[isig]->shat;
                shat = F_shat;
            }
            else if (channelID == 2)
            {
                F_HT = ch[isig]->HT;
                F_MET = ch[isig]->MET;
                F_Mll = ch[isig]->Mll;
                F_Mbb = ch[isig]->Mbb;
                F_dRbV = ch[isig]->dRbV;
                F_shat = ch[isig]->shat;
                shat = F_shat;
            }
            BDTScore = reader->EvaluateMVA("BDT method");
            if (sqrts == 3000)
            {
                if (BDTScore<BDTScoreCuts3000[channelID-1])
                {
                    continue;
                }
            }
            else if (sqrts == 1500)
            {
                if (BDTScore < BDTScoreCuts1500[channelID-1])
                {
                    continue;
                }
            }
            NEVENTS[isig] += Weight;
            tshat[isig]->Fill(); 
            HistSHAT[isig]->Fill(shat,Weight);     
            // t2->Fill();
        }
        f2->cd();
        tshat[isig]->Write();
        HistSHAT[isig]->Write();
    }
    f2->Close();

    sprintf(temp,"%s/BDTResult_Diff.root",dir.c_str());
    f2 = new TFile(temp);
    sprintf(temp,"%s/BDTResult_Diff_PDF.root",dir.c_str());
    TFile *f3 = new TFile(temp,"RECREATE");
    int NBINS = 20;
    RooRealVar roo_shat("shat","shat",SHATMIN3000[channelID-1],SHATMAX3000[channelID-1]);
    roo_shat.setBins(NBINS);
    // RooRealVar roo_weight("Weight","Weight",0,20);
    // RooArgSet testset = RooArgSet(roo_shat,roo_weight);

    // RooDataSet *roo_data[N_SIGCAT];
    RooDataHist *roo_datahist[N_SIGCAT];
    RooHistPdf *roo_pdfhist[N_SIGCAT]; 
    RooPlot *roo_pdfframe = roo_shat.frame();
    TH1F *roo_Hist[N_SIGCAT];
    for (int i = 0; i < N_SIGCAT; i++)
    {
        sprintf(temp,"Hist_%s",SIG_NAME[channelID-1][i].c_str());
        roo_Hist[i] = (TH1F*) f2->Get(temp);
        sprintf(temp,"%s",SIG_NAME[channelID-1][i].c_str());
        // roo_data[i] = new RooDataSet(temp,"",testset,Import(*tshat[i]),WeightVar(roo_weight));
        // roo_datahist[i] = roo_data[i]->binnedClone();
        roo_datahist[i] = new RooDataHist(temp,"",roo_shat,roo_Hist[i]);
        sprintf(temp,"%s_pdf",SIG_NAME[channelID-1][i].c_str());
        roo_pdfhist[i] = new RooHistPdf(temp,temp,roo_shat,*roo_datahist[i]);
        roo_pdfhist[i]->plotOn(roo_pdfframe);
    }
    TCanvas *c1 = new TCanvas("c1","",1000,800);
    roo_pdfframe->Draw();
    f3->cd();
    c1->Write();
    f3->Close();
    // int NTRIALS = 5000;

    // double NLL[N_SIGCAT];
    // double Delta_NLL[N_SIGCAT];
    // for (int isig = 0; isig < N_SIGCAT; isig++)
    // {
    //     NLL[isig] = 0;
    //     for (int i = 0; i < NTRIALS; i++)
    //     {
    //         RooDataSet* roo_testdata = roo_pdfhist[CENTERID]->generate(roo_shat,NEVENTS[CENTERID]);
    //         NLL[isig] += (roo_pdfhist[isig]->createNLL(*roo_testdata,Extended(true)))->getVal();
    //     }
    //     NLL[isig]/=NTRIALS;
    // }
    // for (int isig = 0; isig < N_SIGCAT; isig++)
    // {
    //     Delta_NLL[isig] = NLL[isig] - NLL[CENTERID];
    // }
    // TGraph *g1 = new TGraph(N_SIGCAT,lwzValue,Delta_NLL);
    // f2->cd();
    // g1->Write();

    // delete g1;
    // f2->Close();
    cout<<"Program Exit!"<<endl;
    cout<<"=================================>"<<endl;
    return 0;
}