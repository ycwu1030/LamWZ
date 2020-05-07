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
    int channelID=atoi(argv[3]);

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
        sprintf(temp,"%s/%s_Full*.root",InputDir.c_str(),SIG_NAME[i].c_str());
        nfiles = ChainSIG[i] -> Add(temp);
        if (nfiles > 0)
        {
            LamWZPreAna *ch = new LamWZPreAna(ChainSIG[i]);
            ch->GetEntry(0);
            WeightSIG[i] = ch->CS*LUMINOSITY/((double)Sig_NTOTAL[i]);
            // delete ch;
            dataloader->AddSignalTree(ChainSIG[i],WeightSIG[i]);
        }
    }
    for (int i = 0; i < N_BKGCAT; ++i)
    {
        ChainBKG[i] = new TChain("LamWZPreAna");
        sprintf(temp,"%s/%s_bkg*.root",InputDir.c_str(),BKG_NAME[i].c_str());
        ChainBKG[i] -> Add(temp);
        LamWZPreAna *ch = new LamWZPreAna(ChainBKG[i]);
        ch->GetEntry(0);
        WeightBKG[i] = ch->CS*LUMINOSITY/((double)Bkg_NTOTAL[i]);
        
        // delete ch;
        dataloader->AddBackgroundTree(ChainBKG[i],WeightBKG[i]);
    }
    TCut precuts = "NBJet==2&&NLep_Af==2";
    dataloader->PrepareTrainingAndTestTree( precuts, precuts,
                                        "SplitMode=Random:NormMode=None:!V" );
    factory->BookMethod( dataloader, TMVA::Types::kBDT, "BDT",
                           "!H:!V:NTrees=1000:MinNodeSize=2.5%:MaxDepth=5:BoostType=AdaBoost:AdaBoostBeta=0.5:UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=GiniIndex:nCuts=30" );
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