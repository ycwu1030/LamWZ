// This is for the Pre-analysis for wh channel
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
        if (NEle < 1) continue; // We at least have an electron in the forward region
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
        if ( NLep_Af < 2 ) continue; // In total we have two lepton in the final state
        
        // Store the lepton in the central region
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

        // This is for h -> b b case.
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
        // Store the `mother particle` of the two b-jets
        LVResonance = LVBJet[0]+LVBJet[1];
        ResonancePT=LVResonance.Pt();
        ResonanceEta=LVResonance.Eta();
        ResonancePhi=LVResonance.Phi();
        ResonanceMass=LVResonance.M();

        VPT = LVCentralLep.Pt();
        VEta = LVCentralLep.Eta();
        VPhi = LVCentralLep.Phi();
        VMass = LVCentralLep.M();

        HT = del->ScalarHT_HT[0];
        MET = del->MissingET_MET[0];
        MET_Phi = del->MissingET_Phi[0];
        MET_Eta = del->MissingET_Eta[0];
        LVMET.SetPtEtaPhiM(MET,MET_Eta,MET_Phi,0);

        Mbb = LVResonance.M();
        Mll = (LVForwardEle + LVCentralLep).M();
        AnglebV = LVResonance.Angle(LVCentralLep.Vect());
        dRbV = LVResonance.DeltaR(LVCentralLep);
        t2->Fill();
    }