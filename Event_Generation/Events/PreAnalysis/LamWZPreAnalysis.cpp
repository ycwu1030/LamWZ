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
    string fileout(argv[2]);
    char temp[200];
    sprintf(temp,"%s",fileout.c_str());
    TFile *f2 = new TFile(temp,"RECREATE");
    TTree *t2 = new TTree("LamWZPreAna","New Variables for Lambda_WZ Ana");

    int mode_ = atoi(argv[3]);// bbll or tautaull, signal or background, 
    // a + 10*b + 100*c;
    // a: 0 for background, 1 for from hWW, 2 for from hZZ, 3 for from inter
    // b: 1 for bbll, 2 for tautaull
    // c: 1 for Wh, 2 for Zh, 3 for ttbar, 4 for wz, 5 for zz

    double CS = atof(argv[4]);// The cross section for the process, not the weight
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

    int NLep_Af;
    int NEle_Af;
    int NJet_Af;
    int NBJet;

    double HT;
    double MET;
    double MET_Phi;
    double MET_Eta;
    double Mbb;
    double Mll;
    double Anglebl;
    double dRbl;


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

    t2->Branch("MET_Phi",&MET_Phi,"MET_Phi/D");
    t2->Branch("MET_Eta",&MET_Eta,"MET_Eta/D");

    t2->Branch("NBJet",&NBJet,"NBJet/I");
    t2->Branch("HT",&HT,"HT/D");
    t2->Branch("MET",&MET,"MET/D");
    t2->Branch("NLep_Af",&NLep_Af,"NLep_Af/I");
    t2->Branch("NEle_Af",&NEle_Af,"NEle_Af/I");
    t2->Branch("Mbb",&Mbb,"Mbb/D");
    t2->Branch("Mll",&Mll,"Mll/D");
    t2->Branch("Anglebl",&Anglebl,"Anglebl/D");
    t2->Branch("dRbl",&dRbl,"dRbl/D");
    ntot = t1->GetEntries();
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
    for (int ne = 0; ne < ntot; ++ne)
    {
        if((ne+1)%1000==0) cout<<"Event: "<<ne+1<<"\r"; cout.flush();
        // cout<<ne<<endl;
        del->GetEntry(ne);
        LVEle.clear();
        LVMuon.clear();
        LVLep.clear();
        LVBJet.clear();
        // cout<<tautree->mc_event_weight<<endl;
        NEle = del->Electron_size;
        if (NEle < 1) continue;
        for (int i = 0; i < NEle; ++i)
        {
            ElePT[i] = del->Electron_PT[i];
            EleEta[i] = del->Electron_Eta[i];
            ElePhi[i] = del->Electron_Phi[i];
            EleC[i] = del->Electron_Charge[i];
            if (ElePT[i]>PTLepTrigger)
            {
                tmp.SetPtEtaPhiM(ElePT[i],EleEta[i],ElePhi[i],0.000511);
                LVLep.push_back(tmp);
                LVEle.push_back(tmp);
            }
        }
        if (LVEle.size()<1) continue;
        NEle_Af = LVEle.size();
        std::sort(LVEle.begin(),LVEle.end(),TLVCompare);
        LVForwardEle=LVEle[0];

        NMuon = del->Muon_size;
        for (int i = 0; i < NMuon; ++i)
        {
            MuonPT[i] = del->Muon_PT[i];
            MuonEta[i] = del->Muon_Eta[i];
            MuonPhi[i] = del->Muon_Phi[i];
            MuonC[i] = del->Muon_Charge[i];
            if (MuonPT[i]>PTLepTrigger)
            {
                tmp.SetPtEtaPhiM(MuonPT[i],MuonEta[i],MuonPhi[i],0.105);
                LVLep.push_back(tmp);
                LVMuon.push_back(tmp);
            }
        }
        NLep_Af = LVLep.size();
        if ( NLep_Af < 2 ) continue;
        if ( LVMuon.size() < 1)
        {
            LVCentralLep = LVEle[1];
        }
        else if ( LVEle.size() < 2 )
        {
            LVCentralLep = LVMuon[0];
        }
        else
        {
            if (LVMuon[0].Pt() > LVEle[1].Pt())
            {
                LVCentralLep = LVMuon[0];
            }
            else
            {
                LVCentralLep = LVEle[1];
            }
        }

        NJet = del->Jet_size;
        NBJet = 0;
        for (int i = 0; i < NJet; ++i)
        {
            JetPT[i] = del->Jet_PT[i];
            JetEta[i] = del->Jet_Eta[i];
            JetPhi[i] = del->Jet_Phi[i];
            JetMass[i] = del->Jet_Mass[i];
            JetBTag[i] = del->Jet_BTag[i];
            if ( (JetBTag[i] & (1<<0)) ) 
            {
                if (JetPT[i] > PTJetTrigger)
                {
                    tmp.SetPtEtaPhiM(JetPT[i],JetEta[i],JetPhi[i],JetMass[i]);
                    LVBJet.push_back(tmp);
                }
            }
        }
        if (LVBJet.size() < 2) continue;
        NBJet = LVBJet.size();
        std::sort(LVBJet.begin(),LVBJet.end(),TLVCompare);
        LVResonance = LVBJet[0]+LVBJet[1];
        ResonancePT=LVResonance.Pt();
        ResonanceEta=LVResonance.Eta();
        ResonancePhi=LVResonance.Phi();
        ResonanceMass=LVResonance.M();


        HT = del->ScalarHT_HT[0];
        MET = del->MissingET_MET[0];
        MET_Phi = del->MissingET_Phi[0];
        MET_Eta = del->MissingET_Eta[0];
        LVMET.SetPtEtaPhiM(MET,MET_Eta,MET_Phi,0);

        Mbb = LVResonance.M();
        Mll = (LVForwardEle + LVCentralLep).M();
        Anglebl = LVResonance.Angle(LVCentralLep.Vect());
        dRbl = LVResonance.DeltaR(LVCentralLep);
        t2->Fill();
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
