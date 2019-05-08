#include "plotheader.h"

using namespace std;

binning::binning() {
  nvar = 0;
  for(Int_t i=0; i<MAX_VARS; i++) {
    nbinx[i] = 1; xlo[i] = 0; xhi[i] = 1; varX[i] = 0; varY[i] = 0; MeVtoGeV[i] = 0;
    nbiny[i] = 1; ylo[i] = 0; yhi[i] = 1; varInt[i] = 0; TwoDQ[i] = 0;
  }
}

binning::~binning() {}

// void binning::add(Int_t nbin_, Float_t xlo_, Float_t xhi_,const char* varname, const char* titleX_, Int_t* var_) {
//   if(nvar>=0 && nvar<MAX_VARS) {
//     nbinx[nvar] = nbin_; xlo[nvar] = xlo_; xhi[nvar] = xhi_;
//     titleX[nvar] = titleX_; varInt[nvar] = var_;
//     VarNameX[nvar] = varname;
//     nvar++;
//   }
//   else
//   {
//     printf("No More Room For Variables in this Binning");
//   }
// }

// void binning::add(Int_t nbin_, Float_t xlo_, Float_t xhi_,const char* varname, const char* titleX_, Float_t* var_, Bool_t MeVtoGeV_) {
//   if(nvar>=0 && nvar<MAX_VARS) {
//     nbinx[nvar] = nbin_; xlo[nvar] = xlo_; xhi[nvar] = xhi_;
//     titleX[nvar] = titleX_; varX[nvar] = var_; MeVtoGeV[nvar] = MeVtoGeV_;
//     VarNameX[nvar] = varname;
//     nvar++;
//   }
// }

// void binning::add(Int_t nbinx_, Float_t xlo_, Float_t xhi_, const char* varnamex, const char* titleX_, Float_t* varX_, Int_t nbiny_, Float_t ylo_, Float_t yhi_, const char* varnamey, const char* titleY_, Float_t* varY_)
// {
//   if (nvar>=0 && nvar<MAX_VARS)
//   {
//     nbinx[nvar]=nbinx_; xlo[nvar]=xlo_; xhi[nvar]=xhi_; VarNameX[nvar]=varnamex; titleX[nvar] = titleX_; varX[nvar]=varX_;
//     nbiny[nvar]=nbiny_; ylo[nvar]=ylo_; yhi[nvar]=yhi_; VarNameY[nvar]=varnamey; titleY[nvar] = titleY_; varY[nvar]=varY_;
//     TwoDQ[nvar]=1;
//     nvar++;
//   }

// }

// Float_t binning::getValX(Int_t i) {
//   Float_t tmp = -999999;
//   if(i>=0  && i<nvar) {
//     if(varX[i]) tmp = MeVtoGeV[i] ? *varX[i]/GeV : *varX[i];
//     else if(varInt[i]) tmp = *varInt[i];
//   }
//   // if(tmp >= xhi[i]) tmp = xhi[i]*0.999999;
//   // if(tmp < xlo[i]) tmp = xlo[i];
//   return tmp;
// }

// Float_t binning::getValY(Int_t i) {
//   Float_t tmp = -999999;
//   if(i>=0  && i<nvar) {
//     if(varY[i]) tmp =  *varY[i];
//   }
//   // if(tmp >= yhi[i]) tmp = yhi[i]*0.999999;
//   // if(tmp < ylo[i]) tmp = ylo[i];
//   return tmp;
// }

void binning::add(Int_t nbin_, Double_t xlo_, Double_t xhi_,const char* varname, const char* titleX_, Int_t* var_) {
  if(nvar>=0 && nvar<MAX_VARS) {
    nbinx[nvar] = nbin_; xlo[nvar] = xlo_; xhi[nvar] = xhi_;
    titleX[nvar] = titleX_; varInt[nvar] = var_;
    VarNameX[nvar] = varname;
    nvar++;
  }
  else
  {
    printf("No More Room For Variables in this Binning");
  }
}

void binning::add(Int_t nbin_, Double_t xlo_, Double_t xhi_,const char* varname, const char* titleX_, Double_t* var_, Bool_t MeVtoGeV_) {
  if(nvar>=0 && nvar<MAX_VARS) {
    nbinx[nvar] = nbin_; xlo[nvar] = xlo_; xhi[nvar] = xhi_;
    titleX[nvar] = titleX_; varX[nvar] = var_; MeVtoGeV[nvar] = MeVtoGeV_;
    VarNameX[nvar] = varname;
    nvar++;
  }
}


void binning::add(Int_t nbinx_, Double_t xlo_, Double_t xhi_, const char* varnamex, const char* titleX_, Double_t* varX_, Int_t nbiny_, Double_t ylo_, Double_t yhi_, const char* varnamey, const char* titleY_, Double_t* varY_)
{
  if (nvar>=0 && nvar<MAX_VARS)
  {
    nbinx[nvar]=nbinx_; xlo[nvar]=xlo_; xhi[nvar]=xhi_; VarNameX[nvar]=varnamex; titleX[nvar] = titleX_; varX[nvar]=varX_;
    nbiny[nvar]=nbiny_; ylo[nvar]=ylo_; yhi[nvar]=yhi_; VarNameY[nvar]=varnamey; titleY[nvar] = titleY_; varY[nvar]=varY_;
    TwoDQ[nvar]=1;
    nvar++;
  }

}

Double_t binning::getValX(Int_t i) {
  Double_t tmp = -999999;
  if(i>=0  && i<nvar) {
    if(varX[i]) tmp = MeVtoGeV[i] ? *varX[i]/GeV : *varX[i];
    else if(varInt[i]) tmp = *varInt[i];
  }
  // if(tmp >= xhi[i]) tmp = xhi[i]*0.999999;
  // if(tmp < xlo[i]) tmp = xlo[i];
  return tmp;
}

Double_t binning::getValY(Int_t i) {
  Double_t tmp = -999999;
  if(i>=0  && i<nvar) {
    if(varY[i]) tmp =  *varY[i];
  }
  // if(tmp >= yhi[i]) tmp = yhi[i]*0.999999;
  // if(tmp < ylo[i]) tmp = ylo[i];
  return tmp;
}