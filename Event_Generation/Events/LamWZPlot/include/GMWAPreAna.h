//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Jun 25 13:48:44 2018 by ROOT version 6.08/06
// from TTree GMWAPreAna/New Variables for GM WA Ana
// found on file: PreAna_delphes_events_1.root
//////////////////////////////////////////////////////////

#ifndef GMWAPreAna_h
#define GMWAPreAna_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <string>
#include <vector>
#include <algorithm>
#include "plotheader.h"

// Header file for the classes stored in the TTree if any.

class GMWAPreAna {
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
   Double_t        ElePT[2];   //[NEle]
   Double_t        EleEta[2];   //[NEle]
   Double_t        ElePhi[2];   //[NEle]
   Double_t        EleC[2];   //[NEle]
   Int_t           NMuon;
   Double_t        MuonPT[2];   //[NMuon]
   Double_t        MuonEta[2];   //[NMuon]
   Double_t        MuonPhi[2];   //[NMuon]
   Double_t        MuonC[2];   //[NMuon]
   Int_t           NJet;
   Int_t           NBJet;
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
   TBranch        *b_NBJet; //!
   TBranch        *b_HT;   //!
   TBranch        *b_MET;   //!
   TBranch        *b_MET_Phi;   //!
   TBranch        *b_MET_Eta;   //!
   TBranch        *b_NLep_Af;   //!
   TBranch        *b_NPhoton_Af;   //!
   TBranch        *b_PLepDOTq;   //!
   TBranch        *b_PTLepAndA;   //!

   GMWAPreAna(TTree *tree=0);
   virtual ~GMWAPreAna();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    Cut();
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);

   void PreAna();
   void initialize(binning *bn);

   Int_t CateIndicator;
   char UsedCuts[800];
   // double NLep_Af;
   // double NPhoton_Af;
   // std::vector<TLorentzVector> LVPhoton;
   // std::vector<TLorentzVector> LVLep;
   // std::vector<double> CLep;
   // TLorentzVector LVMET;
   // double PLepDOTq;
   // double PTLepAndA;

   void GetCate();
   void SetCuts(int METmin = 0, int METmax = 9999999, int HTmin = 0, int HTmax = 9999999, int PTLepAmin = 0, int PTLepAmax = 9999999, int DotProdmin = 0, int DotProdmax = 999999);
   int _METmin,_METmax,_HTmin,_HTmax,_PTLepAmin,_PTLepAmax,_DotProdmin,_DotProdmax;
};

#endif

#ifdef GMWAPreAna_cxx
bool TLVCompare(TLorentzVector a, TLorentzVector b)
{
   return a.Pt()>b.Pt();
}

GMWAPreAna::GMWAPreAna(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("PreAna_delphes_events_1.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("PreAna_delphes_events_1.root");
      }
      f->GetObject("GMWAPreAna",tree);

   }
   Init(tree);
}

GMWAPreAna::~GMWAPreAna()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

void GMWAPreAna::PreAna()
{
// No need to use this function
   // double PhotonPTMin = 10;// GeV;
   // double LepPTMin = 15;// GeV;
   // TLorentzVector tmp;
   // NPhoton_Af=0;
   // LVPhoton.clear();
   // for (int i = 0; i < NPhoton; ++i)
   // {
   //    if (photonPT[i]>PhotonPTMin)
   //    {
   //       NPhoton_Af++;
   //       tmp.SetPtEtaPhiE(photonPT[i],photonEta[i],photonPhi[i],photonE[i]);
   //       LVPhoton.push_back(tmp);
   //    }
   // }
   // NLep_Af=0;
   // LVLep.clear();
   // for (int i = 0; i < NEle; ++i)
   // {
   //    if (ElePT[i]>LepPTMin)
   //    {
   //       NLep_Af++;
   //       tmp.SetPtEtaPhiM(ElePT[i],EleEta[i],ElePhi[i],0.000511);
   //       LVLep.push_back(tmp);
   //    }
   // }
   // for (int i = 0; i < NMuon; ++i)
   // {
   //    if (MuonPT[i]>LepPTMin)
   //    {
   //       NLep_Af++;
   //       tmp.SetPtEtaPhiM(MuonPT[i],MuonEta[i],MuonPhi[i],0.105);
   //       LVLep.push_back(tmp);
   //    }
   // }
   // std::sort(LVLep.begin(),LVLep.end(),TLVCompare);
   // LVMET.SetPtEtaPhiM(MET,MET_Eta,MET_Phi,0);
}

Int_t GMWAPreAna::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   Int_t tmp = fChain->GetEntry(entry);
   GetCate();
   // PreAna();
   return tmp;
}
Long64_t GMWAPreAna::LoadTree(Long64_t entry)
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

void GMWAPreAna::GetCate()
{
   if (Cate<1000)
   {
      CateIndicator = int(Cate/100)-1;
   }
   else if (Cate > 1000)
   {
      CateIndicator = Cate%1000+2;
   }
}

void GMWAPreAna::initialize(binning *bn)
{
   bn->add(10,0,10,"NEvents","Events Category",&CateIndicator);
   bn->add(10,0,10,"NJet","NJet",&NJet);
   bn->add(10,0,10,"NBJet","NBJet",&NBJet);
   bn->add(10,0,10,"NPhoton","NPhoton",&NPhoton_Af);
   bn->add(10,0,10,"NLep","NLep",&NLep_Af);
   bn->add(100,0,1000,"HT","H_{T} [GeV]",&HT);
   bn->add(200,0,20000,"PLepDOTq","p_{l}.q [GeV^{2}]",&PLepDOTq);
   bn->add(100,0,1000,"PTLep_A_MET","P_{T}^{l+#gamma+MET} [GeV]",&PTLepAndA);
   bn->add(100,0,1000,"MET","MET [GeV]",&MET);
}

void GMWAPreAna::Init(TTree *tree)
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
   fChain->SetBranchAddress("NBJet", &NBJet, &b_NBJet);
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

Bool_t GMWAPreAna::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void GMWAPreAna::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
void GMWAPreAna::SetCuts(int METmin, int METmax, int HTmin, int HTmax, int PTLepAmin, int PTLepAmax, int DotProdmin, int DotProdmax)
{
   _METmin = METmin;
   _METmax = METmax;
   _HTmin = HTmin;
   _HTmax = HTmax;
   _PTLepAmin = PTLepAmin;
   _PTLepAmax = PTLepAmax;
   _DotProdmin = DotProdmin;
   _DotProdmax = DotProdmax;
   sprintf(UsedCuts,"(MET>=%d)&&(MET<=%d)&&(NPhoton_Af>=1)&&(NLep_Af>=1)&&(NJet<=2)&&(NBJet==0)&&(HT>%d)&&(HT<%d)&&(PTLepAndA>=%d)&&(PTLepAndaA<=%d)&&(DotProd>=%d)&&(DotProd<=%d)",_METmin,_METmax,_HTmin,_HTmax,_PTLepAmin,_PTLepAmax,_DotProdmin,_DotProdmax);
}

Int_t GMWAPreAna::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
Int_t GMWAPreAna::Cut()
{
   // PLepDOTq = 0;
   Bool_t cut = (MET>=_METmin)&&(MET<=_METmax)&&(NPhoton_Af>=1)&&(NLep_Af>=1)&&(NJet<=2)&&(NBJet==0)&&(HT>_HTmin)&&(HT<_HTmax);
   if (!cut) return cut;
   // double dR = 999;
   // double tmpdR;
   // TLorentzVector PT;
   // TLorentzVector TVLep;
   // for (int i = 0; i < NPhoton_Af; ++i)
   // {
   //    tmpdR = LVLep[0].DeltaR(LVPhoton[i]);
   //    if (tmpdR < dR)
   //    {
   //       dR = tmpdR;
   //       PLepDOTq = LVLep[0]*LVPhoton[i];
   //       PT = LVLep[0] + LVPhoton[i];
   //    }
   // }
   // PTLepAndA = (PT+LVMET).Pt();
   cut = cut&&(PLepDOTq > _DotProdmin)&&(PLepDOTq<_DotProdmax)&&(PTLepAndA>_PTLepAmin)&&(PTLepAndA<_PTLepAmax);
   return cut;
}

#endif // #ifdef GMWAPreAna_cxx
