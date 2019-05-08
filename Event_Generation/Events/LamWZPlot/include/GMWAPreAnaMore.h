//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Aug 30 10:52:57 2018 by ROOT version 6.08/06
// from TTree GMWAPreAnaMore/New Variables for GM WA Ana
// found on file: PreAna_more_delphes_events_1.root
//////////////////////////////////////////////////////////

#ifndef GMWAPreAnaMore_h
#define GMWAPreAnaMore_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

class GMWAPreAnaMore {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Int_t           Cate;
   Int_t           NPhoton;
   Double_t        photonPT[4];   //[NPhoton]
   Double_t        photonEta[4];   //[NPhoton]
   Double_t        photonPhi[4];   //[NPhoton]
   Double_t        photonE[4];   //[NPhoton]
   Int_t           NEle;
   Double_t        ElePT[1];   //[NEle]
   Double_t        EleEta[1];   //[NEle]
   Double_t        ElePhi[1];   //[NEle]
   Double_t        EleC[1];   //[NEle]
   Int_t           NMuon;
   Double_t        MuonPT[1];   //[NMuon]
   Double_t        MuonEta[1];   //[NMuon]
   Double_t        MuonPhi[1];   //[NMuon]
   Double_t        MuonC[1];   //[NMuon]
   Int_t           NJet;
   Double_t        HT;
   Double_t        MET;
   Double_t        MET_Phi;
   Double_t        MET_Eta;
   Int_t           NLep_Af;
   Int_t           NPhoton_Af;
   Double_t        PLepDOTq;
   Double_t        PTLepAndA;

   // List of branches
   TBranch        *b_Cate;   //!
   TBranch        *b_NPhoton;   //!
   TBranch        *b_photonPT;   //!
   TBranch        *b_photonEta;   //!
   TBranch        *b_photonPhi;   //!
   TBranch        *b_photonE;   //!
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
   TBranch        *b_HT;   //!
   TBranch        *b_MET;   //!
   TBranch        *b_MET_Phi;   //!
   TBranch        *b_MET_Eta;   //!
   TBranch        *b_NLep_Af;   //!
   TBranch        *b_NPhoton_Af;   //!
   TBranch        *b_PLepDOTq;   //!
   TBranch        *b_PTLepAndA;   //!

   GMWAPreAnaMore(TTree *tree=0);
   virtual ~GMWAPreAnaMore();
   virtual Int_t    Cut(Long64_t entry);
   virtual bool     Cut();
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef GMWAPreAnaMore_cxx
GMWAPreAnaMore::GMWAPreAnaMore(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("PreAna_more_delphes_events_1.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("PreAna_more_delphes_events_1.root");
      }
      f->GetObject("GMWAPreAnaMore",tree);

   }
   Init(tree);
}

GMWAPreAnaMore::~GMWAPreAnaMore()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t GMWAPreAnaMore::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t GMWAPreAnaMore::LoadTree(Long64_t entry)
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

void GMWAPreAnaMore::Init(TTree *tree)
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
   fChain->SetBranchAddress("NPhoton", &NPhoton, &b_NPhoton);
   fChain->SetBranchAddress("photonPT", photonPT, &b_photonPT);
   fChain->SetBranchAddress("photonEta", photonEta, &b_photonEta);
   fChain->SetBranchAddress("photonPhi", photonPhi, &b_photonPhi);
   fChain->SetBranchAddress("photonE", photonE, &b_photonE);
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
   fChain->SetBranchAddress("HT", &HT, &b_HT);
   fChain->SetBranchAddress("MET", &MET, &b_MET);
   fChain->SetBranchAddress("MET_Phi", &MET_Phi, &b_MET_Phi);
   fChain->SetBranchAddress("MET_Eta", &MET_Eta, &b_MET_Eta);
   fChain->SetBranchAddress("NLep_Af", &NLep_Af, &b_NLep_Af);
   fChain->SetBranchAddress("NPhoton_Af", &NPhoton_Af, &b_NPhoton_Af);
   fChain->SetBranchAddress("PLepDOTq", &PLepDOTq, &b_PLepDOTq);
   fChain->SetBranchAddress("PTLepAndA", &PTLepAndA, &b_PTLepAndA);
   Notify();
}

Bool_t GMWAPreAnaMore::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void GMWAPreAnaMore::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t GMWAPreAnaMore::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
bool GMWAPreAnaMore::Cut()
{
   bool good = true;
   if (NJet>2 || NPhoton_Af < 1 || NLep_Af < 1 || PTLepAndA < 0 || PLepDOTq < 0)
   {
      good = false;
   }
   return good;
}
#endif // #ifdef GMWAPreAnaMore_cxx
