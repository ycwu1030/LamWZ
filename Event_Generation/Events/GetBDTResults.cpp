
void GetBDTSignificance(string filename)
{
    TFile *f1 = new TFile(filename.c_str());
    TTree *t1 = (TTree*)f1->Get("BDTResult");
    TCanvas *c1 = new TCanvas("c1","",1000,800);
    t1->Draw("BDTScore>>hs(200,-1,1)","Weight*(SorB==1)");
    c1->Update();
    TH1F *hs = (TH1F*)gPad->GetPrimitive("hs");
    t1->Draw("BDTScore>>hb(200,-1,1)","Weight*(SorB==0)");
    c1->Update();
    TH1F *hb = (TH1F*)gPad->GetPrimitive("hb");

    TH1F *fhs = (TH1F*)hs->GetCumulative(false);
    TH1F *fhb = (TH1F*)hb->GetCumulative(false);

    TH1F *fall = (TH1F*)fhb->Clone();
    fall->Add(fhs,fhb);
    for (int i = 0; i < 200; ++i)
    {
        double c = fall->GetBinContent(i);
        double sqrtc = c<=0?-1:sqrt(c);
        fall -> SetBinContent(i,sqrtc);
    }
    TH1F *fsigma = (TH1F*) fall->Clone();
    fsigma->Divide(fhs,fall);
    fsigma->Draw();
    c1->SaveAs("BDTResult.png");
    cout<<"The maximum significance is: "<<fsigma->GetMaximum()<<endl;
}