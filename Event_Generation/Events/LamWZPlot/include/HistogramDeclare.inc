// Initial the Histogram;
    
    TH1F *Hsig[N_SIGCAT][N_VARIABLE]; // the first dim for channel of signal, the second dim for different variables.
    TH1F *Hbkg[N_BKGCAT][N_VARIABLE]; // for bkgs
    TH1F *HsigNORM[N_SIGCAT][N_VARIABLE]; // the first dim for channel of signal, the second dim for different variables.
    TH1F *HbkgNORM[N_BKGCAT][N_VARIABLE]; // for bkgs
    // Using Function void GetHistID(int cat,int &SorB, int &ID);
    TH2F *Hsig2[N_SIGCAT][N_VARIABLE];
    TH2F *Hbkg2[N_BKGCAT][N_VARIABLE];
    // For each variable and each signal cate, we need one THStack, and one Canvas;
    THStack* HSK[N_VARIABLE];
    THStack* HSKNORM[N_VARIABLE];
    TCanvas* CV[N_VARIABLE]; // For Stack
    TCanvas* CVS[N_VARIABLE];// For overlap thus renormalized to 1
    // For every variable, we can use the same legend;
    TLegend* lg;
    // The Style of the Histogram should be set only depending on the Sig or Bkg CAT not Variables.
    char temp[200];
    for (int i = 0; i < bn->nvar; ++i)
    {   
        if(bn->TwoDQ[i]==0)
        {
            // For signal
            for (int j = 0; j < N_SIGCAT; ++j)
            {
                sprintf(temp,"Hsig_%s_var%d_%s",SIG_NAME[j].c_str(),i+1,(bn->VarNameX[i]).c_str());
                Hsig[j][i] = new TH1F(temp,"",bn->nbinx[i],bn->xlo[i],bn->xhi[i]);
                Hsig[j][i]->Sumw2();
            }
            // For background
            for (int j = 0; j < N_BKGCAT; ++j)
            {
                sprintf(temp,"Hbkg_%s_var%d_%s",BKG_NAME[j].c_str(),i+1,(bn->VarNameX[i]).c_str());
                Hbkg[j][i] = new TH1F(temp,"",bn->nbinx[i],bn->xlo[i],bn->xhi[i]);
                Hbkg[j][i]->Sumw2();
            }
        }
        else
        {
            for (int j = 0; j < N_SIGCAT; ++j)
            {
                sprintf(temp,"Hsig_%s_var%d_%svs%s",SIG_NAME[j].c_str(),i+1,(bn->VarNameX[i]).c_str(),(bn->VarNameY[i]).c_str());
                Hsig2[j][i] = new TH2F(temp,"",bn->nbinx[i],bn->xlo[i],bn->xhi[i],bn->nbiny[i],bn->ylo[i],bn->yhi[i]);
                Hsig2[j][i]->Sumw2();
            }
            // For background
            for (int j = 0; j < N_BKGCAT; ++j)
            {
                sprintf(temp,"Hbkg_%s_var%d_%svs%s",BKG_NAME[j].c_str(),i+1,(bn->VarNameX[i]).c_str(),(bn->VarNameY[i]).c_str());
                Hbkg2[j][i] = new TH2F(temp,"",bn->nbinx[i],bn->xlo[i],bn->xhi[i],bn->nbiny[i],bn->ylo[i],bn->yhi[i]);
                Hbkg2[j][i]->Sumw2();
            }
        }
    }