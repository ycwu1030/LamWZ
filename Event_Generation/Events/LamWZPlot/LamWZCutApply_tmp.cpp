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
#include "TMVA/MethodCuts.h"
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
    sst<<dirtop<<"/CUT_Train_LamWZ_"<<tag;
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
    sprintf(temp,"%s/CUTResult.root",dir.c_str());
    TFile *f2 = new TFile(temp,"RECREATE");
    TTree *t2 = new TTree("CUTResult","The results from CUT");
    int CUTHardcate;
    double EventsSigHard=0;
    double EventsBkgHard=0;
    double SignificanceHard;
    const int N_EffS_samples = 21;
    int N_EffS = N_EffS_samples;
    double EffSignals[N_EffS_samples];
    int CUTGAcate[N_EffS_samples];
    double EventsSigGA[N_EffS_samples];
    double EventsBkgGA[N_EffS_samples];
    double SignificanceGA[N_EffS_samples];

    int CUTMCcate[N_EffS_samples];
    double EventsSigMC[N_EffS_samples];
    double EventsBkgMC[N_EffS_samples];
    double SignificanceMC[N_EffS_samples];

    int CUTSAcate[N_EffS_samples];
    double EventsSigSA[N_EffS_samples];
    double EventsBkgSA[N_EffS_samples];
    double SignificanceSA[N_EffS_samples];
    bool TMVARes;
    double EffSigStart = 0.1, EffSigEnd = 0.9;
    for (int i = 0; i < N_EffS_samples; i++)
    {
        EffSignals[i] = EffSigStart + i*(EffSigEnd-EffSigStart)/(double(N_EffS_samples-1));
        EventsSigGA[i] = 0;
        EventsBkgGA[i] = 0;

        EventsSigMC[i] = 0;
        EventsBkgMC[i] = 0;

        EventsSigSA[i] = 0;
        EventsBkgSA[i] = 0;
    }

    int ProcessID;
    int SorB;
    double Weight;
    Float_t F_FLepEta, F_MovedEta, F_HT, F_MET, F_Mbb, F_Mll, F_AnglebV, F_shat;

    t2->Branch("ProcessID",&ProcessID,"ProcessID/I");
    t2->Branch("SorB",&SorB,"SorB/I");
    t2->Branch("Weight",&Weight,"Weight/D");
    t2->Branch("FLepEta",&F_FLepEta,"FLepEta/F");
    t2->Branch("FMovedEta",&F_MovedEta,"FmovedEta/F");
    t2->Branch("HT",&F_HT,"HT/F");
    t2->Branch("MET",&F_MET,"MET/F");
    t2->Branch("Mbb",&F_Mbb,"Mbb/F");
    t2->Branch("Mll",&F_Mll, "Mll/F");
    t2->Branch("AnglebV",&F_AnglebV,"AnglebV/F");
    t2->Branch("shat",&F_shat,"shat/F");
    t2->Branch("N_EffS",&N_EffS,"N_EffS/I");
    t2->Branch("EffSignals",EffSignals,"EffSignals[N_EffS]/D");
    t2->Branch("CUTHardcate",&CUTHardcate,"CUTHardcate/I");
    t2->Branch("CUTGAcate",CUTGAcate,"CUTGAcate[N_EffS]/I");
    t2->Branch("CUTMCcate",CUTMCcate,"CUTMCcate[N_EffS]/I");
    t2->Branch("CUTSAcate",CUTSAcate,"CUTSAcate[N_EffS]/I");

    auto hardcut = [&](){
        return (F_Mll > 98.0)&&(F_Mbb > 95 && F_Mbb < 140)&&(F_MET > 20 && F_MET < 300)&&(F_FLepEta < -1 || F_FLepEta > 1.2);
    };

    TMVA::Reader *reader = new TMVA::Reader("!Color:Silent");


    if (channelID == 1)
    {
        reader->AddVariable( "MovedEta := FLepEta<0?FLepEta+5:FLepEta", &F_MovedEta );
        reader->AddVariable( "HT", &F_HT );
        reader->AddVariable( "MET", &F_MET );
        reader->AddVariable( "Mbb", &F_Mbb );
        reader->AddVariable( "Mll", &F_Mll );
        // reader->AddVariable( "AnglebV", &F_AnglebV );
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
    methodName = "CutsMC method";
    weightfile = dir + "/TMVAClassification_CutsMC.weights.xml";
    reader->BookMVA(methodName,weightfile);
    methodName = "CutsSA method";
    weightfile = dir + "/TMVAClassification_CutsSA.weights.xml";
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
        ProcessID = ch->processID;
        if (SorB == 1)
        {
            Weight = ch->CS*LUMINOSITY/((double)Sig_NTOTAL[id]);
        }
        else
        {
            Weight = ch->CS*LUMINOSITY/((double)Bkg_NTOTAL[id]);
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
        TMVARes = hardcut();
        if (TMVARes)
        {
            CUTHardcate = 1;
            if (SorB == 1)
            {
                EventsSigHard += Weight;
            }
            else
            {
                EventsBkgHard += Weight;
            }
        }
        else
        {
            CUTHardcate = 0;
        }
        
        
        for (int ieffs = 0; ieffs < N_EffS_samples; ieffs++)
        {
            TMVARes = reader->EvaluateMVA("CutsGA method", EffSignals[ieffs]);
            if (TMVARes)
            {
                CUTGAcate[ieffs] = 1;
                if (SorB == 1)
                {
                    EventsSigGA[ieffs] += Weight;
                }
                else
                {
                    EventsBkgGA[ieffs] += Weight;
                }
                
            }
            else
            {
                CUTGAcate[ieffs] = 0;
            } 
            TMVARes = reader->EvaluateMVA("CutsMC method", EffSignals[ieffs]);
            if (TMVARes)
            {
                CUTMCcate[ieffs] = 1;
                if (SorB == 1)
                {
                    EventsSigMC[ieffs] += Weight;
                }
                else
                {
                    EventsBkgMC[ieffs] += Weight;
                }
                
            }
            else
            {
                CUTMCcate[ieffs] = 0;
            } 
            TMVARes = reader->EvaluateMVA("CutsSA method", EffSignals[ieffs]);
            if (TMVARes)
            {
                CUTSAcate[ieffs] = 1;
                if (SorB == 1)
                {
                    EventsSigSA[ieffs] += Weight;
                }
                else
                {
                    EventsBkgSA[ieffs] += Weight;
                }
                
            }
            else
            {
                CUTSAcate[ieffs] = 0;
            }   
        }
        // BDTScore = reader->EvaluateMVA("CutsGA method",);
        t2->Fill();
    }
    double sigmaxGA = -1;
    double EffSmaxGA;

    double sigmaxMC = -1;
    double EffSmaxMC;

    double sigmaxSA = -1;
    double EffSmaxSA;
    for (int i = 0; i < N_EffS_samples; i++)
    {
        SignificanceGA[i] = EventsSigGA[i]/sqrt(EventsSigGA[i]+EventsBkgGA[i] + 0*EventsBkgGA[i]*EventsBkgGA[i]);//5% systematic
        if (SignificanceGA[i] > sigmaxGA) { 
            sigmaxGA = SignificanceGA[i]; 
            EffSmaxGA = EffSignals[i];
        }

        SignificanceMC[i] = EventsSigMC[i]/sqrt(EventsSigMC[i]+EventsBkgMC[i] + 0*EventsBkgMC[i]*EventsBkgMC[i]);//5% systematic
        if (SignificanceMC[i] > sigmaxMC) { 
            sigmaxMC = SignificanceMC[i]; 
            EffSmaxMC = EffSignals[i];
        }

        SignificanceSA[i] = EventsSigSA[i]/sqrt(EventsSigSA[i]+EventsBkgSA[i] + 0*EventsBkgSA[i]*EventsBkgSA[i]);//5% systematic
        if (SignificanceSA[i] > sigmaxSA) { 
            sigmaxSA = SignificanceSA[i]; 
            EffSmaxSA = EffSignals[i];
        }
    }
    SignificanceHard = EventsSigHard/sqrt(EventsSigHard+EventsBkgHard + 0*EventsBkgHard*EventsBkgHard);//5% systematic
    cout<<"Hard Maximum Significance is: "<<SignificanceHard<<endl;
    cout<<"GA Maximum Significance is: "<<sigmaxGA<<endl;
    cout<<"Achieved at EffS = "<<EffSmaxGA<<endl;
    TMVA::MethodCuts* mcutsGA = reader->FindCutsMVA("CutsGA method");
    if (mcutsGA)
    {
        std::vector<Double_t> cutsMin;
        std::vector<Double_t> cutsMax;
        mcutsGA->GetCuts( EffSmaxGA, cutsMin, cutsMax );
        std::cout << "--- -------------------------------------------------------------" << std::endl;
        std::cout << "--- Retrieve cut values for signal efficiency of "<<EffSmaxGA<<" from Reader" << std::endl;
        for (UInt_t ivar=0; ivar<cutsMin.size(); ivar++) {
        std::cout << "... Cut: "
                    << cutsMin[ivar]
                    << " < \""
                    << mcutsGA->GetInputVar(ivar)
                    << "\" <= "
                    << cutsMax[ivar] << std::endl;
        }
        std::cout << "--- -------------------------------------------------------------" << std::endl;
    }

    cout<<"MC Maximum Significance is: "<<sigmaxMC<<endl;
    cout<<"Achieved at EffS = "<<EffSmaxMC<<endl;
    TMVA::MethodCuts* mcutsMC = reader->FindCutsMVA("CutsMC method");
    if (mcutsMC)
    {
        std::vector<Double_t> cutsMin;
        std::vector<Double_t> cutsMax;
        mcutsMC->GetCuts( EffSmaxMC, cutsMin, cutsMax );
        std::cout << "--- -------------------------------------------------------------" << std::endl;
        std::cout << "--- Retrieve cut values for signal efficiency of "<<EffSmaxMC<<" from Reader" << std::endl;
        for (UInt_t ivar=0; ivar<cutsMin.size(); ivar++) {
        std::cout << "... Cut: "
                    << cutsMin[ivar]
                    << " < \""
                    << mcutsMC->GetInputVar(ivar)
                    << "\" <= "
                    << cutsMax[ivar] << std::endl;
        }
        std::cout << "--- -------------------------------------------------------------" << std::endl;
    }

    cout<<"SA Maximum Significance is: "<<sigmaxSA<<endl;
    cout<<"Achieved at EffS = "<<EffSmaxSA<<endl;
    TMVA::MethodCuts* mcutsSA = reader->FindCutsMVA("CutsSA method");
    if (mcutsSA)
    {
        std::vector<Double_t> cutsMin;
        std::vector<Double_t> cutsMax;
        mcutsSA->GetCuts( EffSmaxSA, cutsMin, cutsMax );
        std::cout << "--- -------------------------------------------------------------" << std::endl;
        std::cout << "--- Retrieve cut values for signal efficiency of "<<EffSmaxSA<<" from Reader" << std::endl;
        for (UInt_t ivar=0; ivar<cutsMin.size(); ivar++) {
        std::cout << "... Cut: "
                    << cutsMin[ivar]
                    << " < \""
                    << mcutsSA->GetInputVar(ivar)
                    << "\" <= "
                    << cutsMax[ivar] << std::endl;
        }
        std::cout << "--- -------------------------------------------------------------" << std::endl;
    }
    
    TGraph *gsigGA = new TGraph(N_EffS_samples,EffSignals,SignificanceGA);
    gsigGA->SetName("g_significance_GA");

    TGraph *gsigMC = new TGraph(N_EffS_samples,EffSignals,SignificanceMC);
    gsigMC->SetName("g_significance_MC");

    TGraph *gsigSA = new TGraph(N_EffS_samples,EffSignals,SignificanceSA);
    gsigSA->SetName("g_significance_SA");

    f2->cd();
    t2->Write();
    gsigGA->Write();
    gsigMC->Write();
    gsigSA->Write();

    delete t2;
    f2->Close();
    cout<<"Program Exit!"<<endl;
    cout<<"=================================>"<<endl;
    return 0;
}