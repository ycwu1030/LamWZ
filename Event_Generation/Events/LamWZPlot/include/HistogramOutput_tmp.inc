    int TwoDQ_;
    double normfactor;
    sprintf(temp,"%s/Plots.root",dir.c_str());
    TFile *fPlots = new TFile(temp,"RECREATE");
    for (int i = 0; i < bn->nvar; ++i)
    {
        TwoDQ_=bn->TwoDQ[i];
        //Book the corresponding THStack and the TCanvas;
        if (TwoDQ_==0)
        {
            sprintf(temp,"Hsk_var%d_%s",i+1,(bn->VarNameX[i]).c_str());
            HSK[i] = new THStack(temp,"");
            // sprintf(temp,"HskOver_var%d_%s",i+1,(bn->VarNameX[i]).c_str());
            // HSKNORM[i] = new THStack(temp,"");

            // sprintf(temp,"HskBEST_var%d_%s",i+1,(bn->VarNameX[i]).c_str());
            // HSKBEST[i] = new THStack(temp,"");
            // sprintf(temp,"HskOverBEST_var%d_%s",i+1,(bn->VarNameX[i]).c_str());
            // HSKNORMBEST[i] = new THStack(temp,"");
        }
        else
        {
            sprintf(temp,"Hsk_var%d_%svs%s",i+1,(bn->VarNameX[i]).c_str(),(bn->VarNameY[i]).c_str());
            HSK[i] = new THStack(temp,"");
            // sprintf(temp,"HskOver_var%d_%svs%s",i+1,(bn->VarNameX[i]).c_str(),(bn->VarNameY[i]).c_str());
            // HSKNORM[i] = new THStack(temp,"");

            // sprintf(temp,"HskBEST_var%d_%svs%s",i+1,(bn->VarNameX[i]).c_str(),(bn->VarNameY[i]).c_str());
            // HSKBEST[i] = new THStack(temp,"");
            // sprintf(temp,"HskOverBEST_var%d_%svs%s",i+1,(bn->VarNameX[i]).c_str(),(bn->VarNameY[i]).c_str());
            // HSKNORMBEST[i] = new THStack(temp,"");
        }
        
        //First Setting the LineSyle/Color etc. of corresponding Histograms
        // cout<<"HSK Booked"<<endl;
        for (int j = 0; j < N_BKGCAT; ++j)
        {
            if(TwoDQ_==0)
            {
                // sprintf(temp,"Events/( %3.1f )",Hbkg[j][i]->GetBinWidth(1));
                normfactor = Hbkg[j][i]->Integral();
                Hbkg[j][i]->Scale(1.0/normfactor);
                Hbkg[j][i]->GetYaxis()->SetTitle("Arbitrary Unit");
                Hbkg[j][i]->GetXaxis()->SetTitle(bn->titleX[i].c_str());
                Hbkg[j][i]->SetLineColor(bkgcolor[j]);
                Hbkg[j][i]->SetLineWidth(2);
                // Hbkg[j][i]->SetFillColor(bkgcolor[j]);
                fPlots->cd();
                Hbkg[j][i]->Write();
                HSK[i]->Add((TH1*)Hbkg[j][i]);
            }
            else
            {
                Hbkg2[j][i]->SetLineColor(bkgcolor[j]);
                // Hbkg2[j][i]->SetFillColor(bkgcolor[j]);
                fPlots->cd();
                Hbkg2[j][i]->Write();
                HSK[i]->Add((TH1*)Hbkg2[j][i]);
            }
        }
        // cout<<"BKG Done"<<endl;
        
        //And Drawing the Histograms;
        for (int j = 0; j < N_SIGCAT; ++j)
        {
            if(TwoDQ_==0)
            {
                // sprintf(temp,"Events/( %3.1f )",Hsig[j][i]->GetBinWidth(1));
                normfactor = Hsig[j][i]->Integral();
                Hsig[j][i]->Scale(1.0/normfactor);
                Hsig[j][i]->GetYaxis()->SetTitle("Arbitrary Unit");
                Hsig[j][i]->GetXaxis()->SetTitle(bn->titleX[i].c_str());
                Hsig[j][i]->SetLineColor(sigcolor[j]);
                Hsig[j][i]->SetLineWidth(2);
                // Hsig[j][i]->SetFillColor(sigcolor[j]);
                fPlots->cd();
                Hsig[j][i]->Write();
                HSK[i]->Add((TH1*)Hsig[j][i]);
            }
            else
            {
                Hsig2[j][i]->SetLineColor(sigcolor[j]);
                // Hsig2[j][i]->SetFillColor(sigcolor[j]);
                fPlots->cd();
                Hsig2[j][i]->Write();
                HSK[i]->Add((TH1*)Hsig2[j][i]);
            }
        }
        // cout<<"SIG Done"<<endl;
        if(i==0)
        {
            lg= new TLegend(0.70,0.70,0.95,0.95,"");
            lg->SetBorderSize(0);
            lg->SetMargin(0.25);
            // lg[j]->SetFillColor(kWhite);
            lg->SetFillColor(0);
            lg->SetFillStyle(0);
            for (int ii = 0; ii < N_BKGCAT; ++ii)
            {
                lg->AddEntry(Hbkg[ii][i],BKG_LABEL[ii].c_str(),"F");
            }
            for (int ii = 0; ii < N_SIGCAT; ++ii)
            {
                if (Hsig[ii][i]->GetEntries()<=0) continue;
                sprintf(temp,"Signal(%s)",SIG_LABEL[ii].c_str());
                lg->AddEntry(Hsig[ii][i],temp,"F");
            }
            // cout<<"Legend Done!"<<endl;
        }

        if(TwoDQ_==0)
        {
            sprintf(temp,"Canv_var%d_%s",i+1,(bn->VarNameX[i]).c_str());
            CV[i] = new TCanvas(temp,"",800,800);
            HSK[i]->Draw("nostack hist");
            sprintf(temp,"Arbitrary Unit");
            HSK[i]->GetYaxis()->SetTitle(temp);

            // cout<<"HSK Done"<<endl;

            // sprintf(temp,"Cover_var%d_%s",i+1,(bn->VarNameX[i]).c_str());
            // CVS[i] = new TCanvas(temp,"",800,800);
            // HSKNORM[i]->Draw("nostack hist");
            // sprintf(temp,"Arbitrary Unit");
            // HSKNORM[i]->GetYaxis()->SetTitle(temp);

            // sprintf(temp,"CoverBEST_var%d_%s",i+1,(bn->VarNameX[i]).c_str());
            // CVSBEST[i] = new TCanvas(temp,"",800,800);
            // HSKNORMBEST[i]->Draw("nostack hist");
            // sprintf(temp,"Arbitrary Unit");
            // HSKNORMBEST[i]->GetYaxis()->SetTitle(temp);
            
            // cout<<"HSKNORM Done"<<endl;
        }
        else
        {
            sprintf(temp,"Canv_var%d_%svs%s",i+1,(bn->VarNameX[i]).c_str(),(bn->VarNameY[i]).c_str());
            CV[i] = new TCanvas(temp,"",800,800);
            HSK[i]->Draw();
            HSK[i]->GetYaxis()->SetTitle(bn->titleY[i].c_str());

            // sprintf(temp,"CanvBEST_var%d_%svs%s",i+1,(bn->VarNameX[i]).c_str(),(bn->VarNameY[i]).c_str());
            // CVBEST[i] = new TCanvas(temp,"",800,800);
            // HSKBEST[i]->Draw();
            // HSKBEST[i]->GetYaxis()->SetTitle(bn->titleY[i].c_str());

            // sprintf(temp,"Events/( %3.1f )",Hbkg[0][i]->GetBinWidth(1));
            // HSK[j][i]->GetZaxis()->SetTitle(temp);
        }
        HSK[i]->GetXaxis()->SetTitle(bn->titleX[i].c_str());
        // HSKNORM[i]->GetXaxis()->SetTitle(bn->titleX[i].c_str());

        // HSKBEST[i]->GetXaxis()->SetTitle(bn->titleX[i].c_str());
        // HSKNORMBEST[i]->GetXaxis()->SetTitle(bn->titleX[i].c_str());
        // cout<<"Drawing on Canvas"<<endl;
         
        if (TwoDQ_==0)
        {
            CV[i]->cd();
            lg->Draw("same");
            sprintf(temp,"%s/C_var%d_%s.png",dir.c_str(),i+1,(bn->VarNameX[i]).c_str());
            CV[i]->SaveAs(temp);

            // CVBEST[i]->cd();
            // lgBEST->Draw("same");
            // sprintf(temp,"%s/CBEST_var%d_%s.png",dir.c_str(),i+1,(bn->VarNameX[i]).c_str());
            // CVBEST[i]->SaveAs(temp);

            // CVS[i]->cd();
            // lg->Draw("same");
            // sprintf(temp,"%s/Cover_var%d_%s.png",dir.c_str(),i+1,(bn->VarNameX[i]).c_str());
            // CVS[i]->SaveAs(temp);

            // CVSBEST[i]->cd();
            // lgBEST->Draw("same");
            // sprintf(temp,"%s/CoverBEST_var%d_%s.png",dir.c_str(),i+1,(bn->VarNameX[i]).c_str());
            // CVSBEST[i]->SaveAs(temp);
        }
        else
        {
            // CV[j][i]->Write();
            sprintf(temp,"%s/C_var%d_%svs%s.png",dir.c_str(),i+1,(bn->VarNameX[i]).c_str(),(bn->VarNameY[i]).c_str());
            CV[i]->SaveAs(temp);

            // sprintf(temp,"%s/CBEST_var%d_%svs%s.png",dir.c_str(),i+1,(bn->VarNameX[i]).c_str(),(bn->VarNameY[i]).c_str());
            // CVBEST[i]->SaveAs(temp);
        }
        // cout<<"Saving Done"<<endl;
        fPlots->cd();
        CV[i]->Write();
        // CVS[i]->Write();

        // CVBEST[i]->Write();
        // CVSBEST[i]->Write();
    
    }
    fPlots->Close();
    cout<<"All Plots have been saved to desired directory."<<endl;