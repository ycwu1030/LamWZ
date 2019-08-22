//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Apr 22 15:07:22 2019 by ROOT version 6.08/06
// from TTree LamWZPreAna/New Variables for Lambda_WZ Ana
// found on file: tt_bkg_bbll_0401_15_0.root
//////////////////////////////////////////////////////////

#ifndef LamWZPreAna_h
#define LamWZPreAna_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <string>
#include <vector>
#include <algorithm>
#include "plotheader.h"

// Header file for the classes stored in the TTree if any.

class LamWZPreAna {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Int_t           Cate;
   Double_t        CS;
   Int_t           NEVENTS;
   Int_t           NEle;
   Double_t        ElePT[3];   //[NEle]
   Double_t        EleEta[3];   //[NEle]
   Double_t        ElePhi[3];   //[NEle]
   Double_t        EleC[3];   //[NEle]
   Int_t           NMuon;
   Double_t        MuonPT[2];   //[NMuon]
   Double_t        MuonEta[2];   //[NMuon]
   Double_t        MuonPhi[2];   //[NMuon]
   Double_t        MuonC[2];   //[NMuon]
   Int_t           NJet;
   Double_t        JetPT[9];   //[NJet]
   Double_t        JetEta[9];   //[NJet]
   Double_t        JetPhi[9];   //[NJet]
   Double_t        JetMass[9];   //[NJet]
   Int_t           JetBTag[9];   //[NJet]
   Double_t        ResonancePT;
   Double_t        ResonanceEta;
   Double_t        ResonancePhi;
   Double_t        ResonanceMass;
   Double_t        VPT;
   Double_t        VEta;
   Double_t        VPhi;
   Double_t        VMass;
   Double_t        FLepPT;
   Double_t        FLepEta;
   Double_t        FLepPhi;
   Double_t        FLepMass;
   Double_t        MET;
   Double_t        MET_Phi;
   Double_t        MET_Eta;
   Int_t           NBJet;
   Double_t        HT;
   Int_t           NLep_Af;
   Int_t           NEle_Af;
   Int_t           NMuon_Af;
   Double_t        Mbb;
   Double_t        Mll;
   Double_t        AnglebV;
   Double_t        dRbV;
   Double_t        shat;

   // List of branches
   TBranch        *b_Cate;   //!
   TBranch        *b_CS;   //!
   TBranch        *b_NEVENTS;   //!
   TBranch        *b_NEle;   //!
   TBranch        *b_ElePT;   //!
   TBranch        *b_EleEta;   //!
   TBranch        *b_ElePhi;   //!
   TBranch        *b_EleC;   //!
   TBranch        *b_NMuon;   //!
   TBranch        *b_MuonPT;   //!
   TBranch        *b_MuonEta;   //!
   TBranch        *b_MuonPhi;   //!
   TBranch        *b_MuonC;   //!
   TBranch        *b_NJet;   //!
   TBranch        *b_JetPT;   //!
   TBranch        *b_JetEta;   //!
   TBranch        *b_JetPhi;   //!
   TBranch        *b_JetMass;   //!
   TBranch        *b_JetBTag;   //!
   TBranch        *b_ResonancePT;   //!
   TBranch        *b_ResonanceEta;   //!
   TBranch        *b_ResonancePhi;   //!
   TBranch        *b_ResonanceMass;   //!
   TBranch        *b_VPT;   //!
   TBranch        *b_VEta;   //!
   TBranch        *b_VPhi;   //!
   TBranch        *b_VMass;   //!
   TBranch        *b_FLepPT;   //!
   TBranch        *b_FLepEta;   //!
   TBranch        *b_FLepPhi;   //!
   TBranch        *b_FLepMass;   //!
   TBranch        *b_MET;   //!
   TBranch        *b_MET_Phi;   //!
   TBranch        *b_MET_Eta;   //!
   TBranch        *b_NBJet;   //!
   TBranch        *b_HT;   //!
   TBranch        *b_NLep_Af;   //!
   TBranch        *b_NEle_Af;   //!
   TBranch        *b_NMuon_Af;   //!
   TBranch        *b_Mbb;   //!
   TBranch        *b_Mll;   //!
   TBranch        *b_AnglebV;   //!
   TBranch        *b_dRbV;   //!
   TBranch        *b_shat;   //!

   LamWZPreAna(TTree *tree=0);
   virtual ~LamWZPreAna();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);

   void initialize(binning *bn);
   void GetCate();

   int CateIndicator;

};

#endif

#ifdef LamWZPreAna_cxx
LamWZPreAna::LamWZPreAna(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("tt_bkg_bbll_0401_15_0.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("tt_bkg_bbll_0401_15_0.root");
      }
      f->GetObject("LamWZPreAna",tree);

   }
   Init(tree);
}

LamWZPreAna::~LamWZPreAna()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

void LamWZPreAna::GetCate()
{
   if (Cate<300)
   {
      CateIndicator = Cate%10-1;
   }
   else if (Cate > 300)
   {
      CateIndicator = Cate/100;
   }
}

void LamWZPreAna::initialize(binning *bn)
{
   bn->add(10,0,10,"NEvents","Events Category",&CateIndicator);
   bn->add(10,0,10,"NBJet","N_{B-jet}",&NBJet);
   bn->add(10,0,10,"NLep","N_{l}",&NLep_Af);
   bn->add(10,0,10,"NEle","N_{e}",&NEle_Af);
   bn->add(30,-3,3,"EtaLep","#eta_{l}",&FLepEta);
   bn->add(50,100,1500,"HT","H_{T} [GeV]",&HT);
   bn->add(50,0,600,"MET","MET [GeV]",&MET);
   bn->add(50,0,400,"Mbb","M_{bb} [GeV]",&Mbb);
   bn->add(50,0,400,"Mll","M_{ll} [GeV]",&Mll);
   bn->add(50,0,3.1415926,"Anglebl","#theta_{bl}",&AnglebV);
   bn->add(50,0,10,"dRbl","#Delta R_{bl}",&dRbV);
   bn->add(50,0,3000,"shat","#sqrt{#hat{s}} [GeV]",&shat);

}

Int_t LamWZPreAna::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   Int_t tmp = fChain->GetEntry(entry);
   GetCate();
   // PreAna();
   return tmp;
}
Long64_t LamWZPreAna::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void LamWZPreAna::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("Cate", &Cate, &b_Cate);
   fChain->SetBranchAddress("CS", &CS, &b_CS);
   fChain->SetBranchAddress("NEVENTS", &NEVENTS, &b_NEVENTS);
   fChain->SetBranchAddress("NEle", &NEle, &b_NEle);
   fChain->SetBranchAddress("ElePT", ElePT, &b_ElePT);
   fChain->SetBranchAddress("EleEta", EleEta, &b_EleEta);
   fChain->SetBranchAddress("ElePhi", ElePhi, &b_ElePhi);
   fChain->SetBranchAddress("EleC", EleC, &b_EleC);
   fChain->SetBranchAddress("NMuon", &NMuon, &b_NMuon);
   fChain->SetBranchAddress("MuonPT", MuonPT, &b_MuonPT);
   fChain->SetBranchAddress("MuonEta", MuonEta, &b_MuonEta);
   fChain->SetBranchAddress("MuonPhi", MuonPhi, &b_MuonPhi);
   fChain->SetBranchAddress("MuonC", MuonC, &b_MuonC);
   fChain->SetBranchAddress("NJet", &NJet, &b_NJet);
   fChain->SetBranchAddress("JetPT", JetPT, &b_JetPT);
   fChain->SetBranchAddress("JetEta", JetEta, &b_JetEta);
   fChain->SetBranchAddress("JetPhi", JetPhi, &b_JetPhi);
   fChain->SetBranchAddress("JetMass", JetMass, &b_JetMass);
   fChain->SetBranchAddress("JetBTag", JetBTag, &b_JetBTag);
   fChain->SetBranchAddress("ResonancePT", &ResonancePT, &b_ResonancePT);
   fChain->SetBranchAddress("ResonanceEta", &ResonanceEta, &b_ResonanceEta);
   fChain->SetBranchAddress("ResonancePhi", &ResonancePhi, &b_ResonancePhi);
   fChain->SetBranchAddress("ResonanceMass", &ResonanceMass, &b_ResonanceMass);
   fChain->SetBranchAddress("VPT", &VPT, &b_VPT);
   fChain->SetBranchAddress("VEta", &VEta, &b_VEta);
   fChain->SetBranchAddress("VPhi", &VPhi, &b_VPhi);
   fChain->SetBranchAddress("VMass", &VMass, &b_VMass);
   fChain->SetBranchAddress("FLepPT", &FLepPT, &b_FLepPT);
   fChain->SetBranchAddress("FLepEta", &FLepEta, &b_FLepEta);
   fChain->SetBranchAddress("FLepPhi", &FLepPhi, &b_FLepPhi);
   fChain->SetBranchAddress("FLepMass", &FLepMass, &b_FLepMass);
   fChain->SetBranchAddress("MET", &MET, &b_MET);
   fChain->SetBranchAddress("MET_Phi", &MET_Phi, &b_MET_Phi);
   fChain->SetBranchAddress("MET_Eta", &MET_Eta, &b_MET_Eta);
   fChain->SetBranchAddress("NBJet", &NBJet, &b_NBJet);
   fChain->SetBranchAddress("HT", &HT, &b_HT);
   fChain->SetBranchAddress("NLep_Af", &NLep_Af, &b_NLep_Af);
   fChain->SetBranchAddress("NEle_Af", &NEle_Af, &b_NEle_Af);
   fChain->SetBranchAddress("NMuon_Af", &NMuon_Af, &b_NMuon_Af);
   fChain->SetBranchAddress("Mbb", &Mbb, &b_Mbb);
   fChain->SetBranchAddress("Mll", &Mll, &b_Mll);
   fChain->SetBranchAddress("AnglebV", &AnglebV, &b_AnglebV);
   fChain->SetBranchAddress("dRbV", &dRbV, &b_dRbV);
   fChain->SetBranchAddress("shat", &shat, &b_shat);
   Notify();
}

Bool_t LamWZPreAna::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void LamWZPreAna::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t LamWZPreAna::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef LamWZPreAna_cxx
