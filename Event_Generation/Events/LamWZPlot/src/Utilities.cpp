#include "Utilities.h"
using namespace std;

void GetHistID(int cat,int &SorB,int &ID)
{
    if (abs(cat)<300) // Sig
    {
        SorB = 1; 
        ID = cat%10-1;
    }
    else
    {
        SorB = 0;
        ID = cat/100-3;
    }
}

void createdir(const char *dir)
{
    int exist;
    exist=access(dir,NULL);
    if (exist==0) return;
    mkdir(dir,S_IRWXU|S_IRWXG|S_IROTH|S_IXOTH);//Mode: 775
}
void SetPlotStyle()
{
    gErrorIgnoreLevel = kWarning;
    
    //ZAxis Color
    // const Int_t NRGBs = 5;
    // const Int_t NCont = 255;
    // Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
    // Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
    // Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
    // Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
    // TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
    // gStyle->SetNumberContours(NCont);
    
    //General
    gStyle->SetOptStat(kFALSE);
    gStyle->SetCanvasBorderMode(0);
    
    //Frame
    // gStyle->SetFrameBorderMode(0);
    // gStyle->SetFrameFillColor(0);
    // gStyle->SetFrameLineColor(1);
    // gStyle->SetFrameLineWidth(2);
    
    //Lines
    // gStyle->SetLineWidth(1);
    // gStyle->SetFillColor(0);
    // gStyle->SetCanvasColor(0);
    
    //Margins
    gStyle->SetPadLeftMargin(0.15);
    gStyle->SetPadBottomMargin(0.15);
    gStyle->SetPadRightMargin(0.05);
    gStyle->SetPadTopMargin(0.05);
    
    //text
    gStyle->SetTextFont(132);
    gStyle->SetTextSize(0.06);
    
    //Axis Title
    gStyle->SetTitleFont(62,"XYZ");
    gStyle->SetTitleSize(0.055,"X");
    gStyle->SetTitleSize(0.055,"Y");
    gStyle->SetTitleSize(0.055,"Z");
    gStyle->SetTitleOffset(1.0,"X");
    gStyle->SetTitleOffset(1.3,"Y");
    gStyle->SetTitleOffset(1.0,"Z");
    //Axis Lable
    gStyle->SetLabelFont(42,"XYZ");
    gStyle->SetLabelSize(0.045,"X");
    gStyle->SetLabelSize(0.045,"Y");
    gStyle->SetLabelSize(0.045,"Z");
    gStyle->SetLabelOffset(0.012,"X");
    gStyle->SetLabelOffset(0.012,"Y");
    
    //Linestyle
    // gStyle->SetLineStyleString(10,"");
    // gStyle->SetLineStyleString(11,"30 30");
    // gStyle->SetLineStyleString(12,"30 20 10 20");
    // gStyle->SetLineStyleString(13,"10 10");
    // gStyle->SetLineStyleString(14,"30 20 10 20 10 20 10 20");
}
void LoadingRootFile(TChain *ch,string InputDir)
{
    char temp[500];
    sprintf(temp,"%s/*.root",InputDir.c_str());
    ch->Add(temp);
    // sprintf(temp,"%s/wh_*_%s_*.root",InputDir.c_str(),DecayChannel[decayID].c_str());
    // ch->Add(temp);
}