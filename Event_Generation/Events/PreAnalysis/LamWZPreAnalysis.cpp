#include "Delphes.h"
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TStyle.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

// void SetBranch(TTree *t1,string name,void *para,TBranch *branch,int doubleQ=1);
// std::vector<TBranch*> BranchList;
bool TLVCompare(TLorentzVector a, TLorentzVector b)
{
       return a.Pt()>b.Pt();
}
int main(int argc, char const *argv[])
{
    const double PTLepTrigger = 20;
    const double PTJetTrigger = 20;
    TFile *f1 = new TFile(argv[1]);
    TTree *t1 = (TTree *)f1->Get("Delphes");
    Delphes *del = new Delphes(t1);
//    string fileori(argv[1]);
//    int pos = fileori.find_last_of('/');
//    string filename(fileori.substr(pos+1));
//    string filepath(fileori.substr(0,pos));
    string outdir(argv[2]);
    string fileout(argv[3]);
    char temp[200];
    sprintf(temp,"%s/%s",outdir.c_str(),fileout.c_str());
    TFile *f2 = new TFile(temp,"RECREATE");
    TTree *t2 = new TTree("LamWZPreAna","New Variables for Lambda_WZ Ana");

    int mode_ = atoi(argv[4]);// bbll or tautaull, signal or background, 
    // a + 10*b + 100*c;
    // a: 0 for background, 1 for from hWW, 2 for from hZZ, 3 for from inter
    // b: 1 for bbll, 2 for tautaull
    // c: 1 for Wh, 2 for Zh, 3 for ttbar, 4 for wz, 5 for zz
    sprintf(temp,"%s/events_proc_%d.dat",outdir.c_str(),mode_);
    ofstream outlog(temp);
    double CS = atof(argv[5]);// The cross section for the process, not the weight
    int channelID = atoi(argv[6]);
    int ntot;
    
    const int MAXEle=5;
    int NEle;
    double ElePT[MAXEle];
    double EleEta[MAXEle];
    double ElePhi[MAXEle];
    double EleC[MAXEle];

    const int MAXMuon=5;
    int NMuon;
    double MuonPT[MAXMuon];
    double MuonEta[MAXMuon];
    double MuonPhi[MAXMuon];
    double MuonC[MAXMuon];

    const int MAXJet=20;
    int NJet;
    double JetPT[MAXJet];
    double JetEta[MAXJet];
    double JetPhi[MAXJet];
    double JetMass[MAXJet];
    int JetBTag[MAXJet];

    double ResonancePT;
    double ResonanceEta;
    double ResonancePhi;
    double ResonanceMass;

    double VPT;
    double VEta;
    double VPhi;
    double VMass;

    int NLep_Af;
    int NEle_Af;
    int NMuon_Af;
    int NJet_Af;
    int NBJet;

    double HT;
    double MET;
    double MET_Phi;
    double MET_Eta;
    double Mbb;
    double Mll;
    double AnglebV;
    double dRbV;


    t2->Branch("Cate",&mode_,"Cate/I");
    t2->Branch("CS",&CS,"CS/D");
    t2->Branch("NEVENTS",&ntot,"NEVENTS/I");

    t2->Branch("NEle",&NEle,"NEle/I");
    t2->Branch("ElePT",ElePT,"ElePT[NEle]/D");
    t2->Branch("EleEta",EleEta,"EleEta[NEle]/D");
    t2->Branch("ElePhi",ElePhi,"ElePhi[NEle]/D");
    t2->Branch("EleC",EleC,"EleC[NEle]/D");

    t2->Branch("NMuon",&NMuon,"NMuon/I");
    t2->Branch("MuonPT",MuonPT,"MuonPT[NMuon]/D");
    t2->Branch("MuonEta",MuonEta,"MuonEta[NMuon]/D");
    t2->Branch("MuonPhi",MuonPhi,"MuonPhi[NMuon]/D");
    t2->Branch("MuonC",MuonC,"MuonC[NMuon]/D");

    t2->Branch("NJet",&NJet,"NJet/I");
    t2->Branch("JetPT",JetPT,"JetPT[NJet]/D");
    t2->Branch("JetEta",JetEta,"JetEta[NJet]/D");
    t2->Branch("JetPhi",JetPhi,"JetPhi[NJet]/D");
    t2->Branch("JetMass",JetMass,"JetMass[NJet]/D");
    t2->Branch("JetBTag",JetBTag,"JetBTag[NJet]/I");

    t2->Branch("ResonancePT",&ResonancePT,"ResonancePT/D");
    t2->Branch("ResonanceEta",&ResonanceEta,"ResonanceEta/D");
    t2->Branch("ResonancePhi",&ResonancePhi,"ResonancePhi/D");
    t2->Branch("ResonanceMass",&ResonanceMass,"ResonanceMass/D");

    t2->Branch("VPT",&VPT,"VPT/D");
    t2->Branch("VEta",&VEta,"VEta/D");
    t2->Branch("VPhi",&VPhi,"VPhi/D");
    t2->Branch("VMass",&VMass,"VMass/D");

    t2->Branch("MET",&MET,"MET/D");
    t2->Branch("MET_Phi",&MET_Phi,"MET_Phi/D");
    t2->Branch("MET_Eta",&MET_Eta,"MET_Eta/D");

    t2->Branch("NBJet",&NBJet,"NBJet/I");
    t2->Branch("HT",&HT,"HT/D");
    t2->Branch("NLep_Af",&NLep_Af,"NLep_Af/I");
    t2->Branch("NEle_Af",&NEle_Af,"NEle_Af/I");
    t2->Branch("NMuon_Af",&NMuon_Af,"NMuon_Af/I");
    t2->Branch("Mbb",&Mbb,"Mbb/D");
    t2->Branch("Mll",&Mll,"Mll/D");
    t2->Branch("AnglebV",&AnglebV,"AnglebV/D");
    t2->Branch("dRbV",&dRbV,"dRbV/D");
    ntot = t1->GetEntries();
    outlog<<ntot<<endl;
    outlog.close();
    bool good;
    vector<TLorentzVector> LVEle;
    vector<TLorentzVector> LVMuon;
    vector<TLorentzVector> LVLep;
    vector<TLorentzVector> LVBJet;
    TLorentzVector LVResonance;
    TLorentzVector LVForwardEle;
    TLorentzVector LVCentralLep;
    TLorentzVector LVMET;
    TLorentzVector tmp;
    TLorentzVector PT;
    double tmpdR;
    double dR;
    if (channelID == 1)
    {
    #include "wh_channel.inc"
    }
    else if (channelID == 2)
    {
    #include "zh_channel.inc"
    }
    cout<<endl;
    f2->cd();
    t2->Write();

    delete t2;
    delete t1;
    f2->Close();
    f1->Close();
    return 0;
}
