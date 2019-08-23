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
    int channelID=atoi(argv[4]);
    int sqrts=atoi(argv[5]);

    //Create the Folder Saving the results
    string dir,dirtop;
    stringstream sst,ssttop;
    ssttop<<DIR;
    ssttop>>dirtop;
    createdir(dirtop.c_str());
    sst<<dirtop<<"/BDT_Train_LamWZ_"<<tag;
    sst>>dir;
    createdir(dir.c_str());

    TMVA::Tools::Instance();
    char temp[500];
    sprintf(temp,"%s/TMVA_results.root",dir.c_str());
    TString outfileName( temp );
    TFile* outputFile = TFile::Open( outfileName, "RECREATE" );
    (TMVA::gConfig().GetIONames()).fWeightFileDir = "../../../../../../.." + dir;
    TMVA::Factory *factory = new TMVA::Factory( "TMVAClassification", outputFile,
                                               "!V:Silent:Color:DrawProgressBar:Transformations=I;D;P;G,D:AnalysisType=Classification" );
    TMVA::DataLoader *dataloader=new TMVA::DataLoader("dataset");

    if (channelID == 1)
    {
        dataloader->AddVariable( "FLepEta", 'F' );
        dataloader->AddVariable( "HT", 'F' );
        dataloader->AddVariable( "MET", 'F' );
        dataloader->AddVariable( "Mbb", 'F' );
        dataloader->AddVariable( "Mll", 'F' );
        dataloader->AddVariable( "AnglebV", 'F' );
        dataloader->AddVariable( "shat", 'F' );
    }
    else if (channelID == 2)
    {
        dataloader->AddVariable( "HT", 'F' );
        dataloader->AddVariable( "MET", 'F' );
        dataloader->AddVariable( "Mbb", 'F' );
        dataloader->AddVariable( "Mll", 'F' );
        dataloader->AddVariable( "AnglebV", 'F' );
        dataloader->AddVariable( "shat", 'F' );
    }

    TChain *ChainSIG[N_SIGCAT];
    TChain *ChainBKG[N_BKGCAT];
    double WeightSIG[N_SIGCAT];
    double WeightBKG[N_BKGCAT];
    int nfiles;
    for (int i = 0; i < N_SIGCAT; ++i)
    {
        ChainSIG[i] = new TChain("LamWZPreAna");
        sprintf(temp,"%s/%s*.root",InputDir.c_str(),SIG_NAME[channelID-1][i].c_str());
        nfiles = ChainSIG[i] -> Add(temp);
        if (nfiles > 0)
        {
            LamWZPreAna *ch = new LamWZPreAna(ChainSIG[i]);
            ch->GetEntry(0);
            if (sqrts == 3000)
            {
                WeightSIG[i] = ch->CS*LUMINOSITY3000/((double)Sig_NTOTAL3000[channelID-1][i]);
            }
            else if (sqrts == 1500)
            {
                WeightSIG[i] = ch->CS*LUMINOSITY1500/((double)Sig_NTOTAL1500[channelID-1][i]);
            }
            // delete ch;
            dataloader->AddSignalTree(ChainSIG[i],WeightSIG[i]);
        }
    }
    for (int i = 0; i < N_BKGCAT; ++i)
    {
        ChainBKG[i] = new TChain("LamWZPreAna");
        sprintf(temp,"%s/%s*.root",InputDir.c_str(),BKG_NAME[i].c_str());
        ChainBKG[i] -> Add(temp);
        LamWZPreAna *ch = new LamWZPreAna(ChainBKG[i]);
        ch->GetEntry(0);
        if (sqrts == 3000)
        {
            WeightBKG[i] = ch->CS*LUMINOSITY3000/((double)Bkg_NTOTAL[i]);
        }
        else if (sqrts == 1500)
        {
            WeightBKG[i] = ch->CS*LUMINOSITY1500/((double)Bkg_NTOTAL[i]);
        }

        // delete ch;
        dataloader->AddBackgroundTree(ChainBKG[i],WeightBKG[i]);
    }
    TCut precuts = "NBJet==2&&NLep_Af==2";
    dataloader->PrepareTrainingAndTestTree( precuts, precuts,
                                        "SplitMode=Random:NormMode=None:!V" );
    factory->BookMethod( dataloader, TMVA::Types::kBDT, "BDT",
                           "!H:!V:NTrees=850:MinNodeSize=2.5%:MaxDepth=5:BoostType=AdaBoost:AdaBoostBeta=0.5:UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=GiniIndex:nCuts=20" );
    // Train MVAs using the set of training events
    factory->TrainAllMethods();

    // Evaluate all MVAs using the set of test events
    factory->TestAllMethods();

    // Evaluate and compare performance of all configured MVAs
    factory->EvaluateAllMethods();

    // --------------------------------------------------------------

    // Save the output
    outputFile->Close();

    std::cout << "==> Wrote root file: " << outputFile->GetName() << std::endl;
    std::cout << "==> TMVAClassification is done!" << std::endl;

    delete factory;
    delete dataloader;
    
    // cout<<"All Plots have been saved to desired directory."<<endl;
    cout<<"Program Exit!"<<endl;
    cout<<"=================================>"<<endl;
    return 0;


    return 0;
}