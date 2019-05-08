#ifndef PLOTHEADER_H
#define PLOTHEADER_H
#include "TROOT.h"
#include "TObject.h"
#include "TChain.h"
#include "TFile.h"
#include "TH1F.h"
#include "TMinuit.h"
#include "THStack.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "TSystem.h"
#include "TLorentzVector.h"
#include "TLine.h"
#include "TLegend.h"

const Double_t GeV=1000;
const Double_t PI=3.1415926536;
const Int_t MAX_VARS = 30;
class binning {
	// The Class used for store the value of Observables
public:
  Int_t nbinx[MAX_VARS];
  Double_t xlo[MAX_VARS];
  Double_t xhi[MAX_VARS];
  Int_t nbiny[MAX_VARS];
  Double_t ylo[MAX_VARS];
  Double_t yhi[MAX_VARS];
  std::string titleX[MAX_VARS];
  std::string titleY[MAX_VARS];
  std::string VarNameX[MAX_VARS];
  std::string VarNameY[MAX_VARS];
  Double_t* varX[MAX_VARS];
  Int_t* varInt[MAX_VARS];
  Bool_t MeVtoGeV[MAX_VARS];
  Int_t nvar;

  Double_t* varY[MAX_VARS];
  Int_t TwoDQ[MAX_VARS];
  
  binning();
  virtual ~binning();

  // void add(Int_t nbinx_, Float_t xlo_, Float_t xhi_,const char* varname, const char* titleX_, Int_t* var_);
  // void add(Int_t nbinx_, Float_t xlo_, Float_t xhi_,const char* varname, const char* titleX_, Float_t* var_, Bool_t MeVtoGeV_=false);
  // void add(Int_t nbinx_, Float_t xlo_, Float_t xhi_, const char* varnamex, const char* titleX_, Float_t* varX_, Int_t nbiny_, Float_t ylo_, Float_t yhi_, const char* varnamey, const char* titleY_, Float_t* varY_);

  // Float_t getValX(Int_t i);
  // Float_t getValY(Int_t i);

  void add(Int_t nbinx_, Double_t xlo_, Double_t xhi_,const char* varname, const char* titleX_, Int_t* var_);
  void add(Int_t nbinx_, Double_t xlo_, Double_t xhi_,const char* varname, const char* titleX_, Double_t* var_, Bool_t MeVtoGeV_=false);
  void add(Int_t nbinx_, Double_t xlo_, Double_t xhi_, const char* varnamex, const char* titleX_, Double_t* varX_, Int_t nbiny_, Double_t ylo_, Double_t yhi_, const char* varnamey, const char* titleY_, Double_t* varY_);

  Double_t getValX(Int_t i);
  Double_t getValY(Int_t i);
};

inline Double_t significance(Double_t b0, Double_t s0, Double_t db=0) {
  if(db==0) return sqrt(2*(s0+b0)*log(1+s0/b0)-2*s0);
  else {
    Double_t tmp = b0-db*db;
    Double_t b = 0.5*(tmp+sqrt(pow(tmp,2)+4*db*db*(b0+s0)));
    return sqrt(2*(b0+s0)*log((b0+s0)/b)-(b0+s0)+b-(b-b0)*b0/db/db);
  }
}
#endif

