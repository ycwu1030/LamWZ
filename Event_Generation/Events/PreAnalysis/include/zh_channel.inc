// This is for the Pre-analysis for zh channel
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
        NMuon = del->Muon_size;
        if (NEle < 2 && NMuon < 2)
        {
            continue; // We need two lepton coming from Z decay.
        }
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
        NEle_Af = LVEle.size();
        std::sort(LVEle.begin(),LVEle.end(),TLVCompare);

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
        NMuon_Af = LVMuon.size();
        std::sort(LVMuon.begin(),LVMuon.end(),TLVCompare);
        NLep_Af = LVLep.size();
        if ( NLep_Af < 2 ) continue; // In total we have two lepton in the final state
        // Get the lepton pair which comes from Z-decay
        double mzpeak = 9999999;
        double mztemp;
        TLorentzVector Zpeak;
        if (NEle_Af > 2)
        {
            for (int i = 0; i < NEle_Af; ++i)
            {
                for (int j = i+1; j < NEle_Af; ++j)
                {
                    mztemp = (LVEle[i]+LVEle[j]).M();
                    if (abs(mztemp-91.1776)<abs(mzpeak-91.1776))
                    {
                        mzpeak = mztemp;
                        Zpeak = LVEle[i]+LVEle[j];
                    }
                }
            }
        }
        if (NMuon_Af > 2)
        {
            for (int i = 0; i < NMuon_Af; ++i)
            {
                for (int j = i+1; j < NMuon_Af; ++j)
                {
                    mztemp = (LVMuon[i]+LVMuon[j]).M();
                    if (abs(mztemp-91.1776)<abs(mzpeak-91.1776))
                    {
                        mzpeak = mztemp;
                        Zpeak = LVMuon[i]+LVMuon[j];
                    }
                }
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

        FLepPT = 0.0;
        FLepEta = 0.0;
        FLepPhi = 0.0;
        FLepMass = 0.0;

        HT = del->ScalarHT_HT[0];
        MET = del->MissingET_MET[0];
        MET_Phi = del->MissingET_Phi[0];
        MET_Eta = del->MissingET_Eta[0];
        LVMET.SetPtEtaPhiM(MET,MET_Eta,MET_Phi,0);

        VPT = Zpeak.Pt();
        VEta = Zpeak.Eta();
        VPhi = Zpeak.Phi();
        VMass = Zpeak.M();

        Mbb = LVResonance.M();
        Mll = (Zpeak).M();
        AnglebV = LVResonance.Angle(Zpeak.Vect());
        dRbV = LVResonance.DeltaR(Zpeak);
        shat = (LVResonance+Zpeak).M();
        t2->Fill();
    }