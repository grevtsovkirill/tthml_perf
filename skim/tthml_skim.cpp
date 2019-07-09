

void tthml_skim() {


  TFile *oldfile = new TFile("Files/preskimmed/ttH_skim.root");
  TTree *oldtree = (TTree*)oldfile->Get("nominal");
  Long64_t nentries = oldtree->GetEntries();
  //*
  oldtree->SetBranchStatus("*",0);
  oldtree->SetBranchStatus("lep_Pt_0",1);
  oldtree->SetBranchStatus( "scale_nom"                   , 1);
  oldtree->SetBranchStatus( "pileupEventWeight_090",1);
  oldtree->SetBranchStatus( "JVT_EventWeight"             , 1);
  oldtree->SetBranchStatus( "MV2c10_70_EventWeight"       , 1);
  oldtree->SetBranchStatus( "lepSFObjTight"               , 1);
  oldtree->SetBranchStatus( "lepSFTrigTight"               , 1);
  oldtree->SetBranchStatus( "SherpaNJetWeight"               , 1);
  oldtree->SetBranchStatus( "RunYear"               , 1);



  Long_t entries;  Double_t JVT_EventWeight;   Float_t scale_nom; 
  Double_t MV2c10_70_EventWeight;   Double_t SherpaNJetWeight; Double_t pileupEventWeight_090;
  Float_t lepSFObjTight;   Float_t lepSFTrigTight;   Int_t RunYear;
    oldtree->SetBranchAddress( "pileupEventWeight_090", &pileupEventWeight_090);
    oldtree->SetBranchAddress( "scale_nom"                             , &scale_nom                   );
    oldtree->SetBranchAddress( "JVT_EventWeight"                       , &JVT_EventWeight             );
    oldtree->SetBranchAddress( "MV2c10_70_EventWeight"                 , &MV2c10_70_EventWeight       );
    oldtree->SetBranchAddress( "lepSFObjTight"                         , &lepSFObjTight               );
    oldtree->SetBranchAddress( "lepSFTrigTight"                         , &lepSFTrigTight               );
    oldtree->SetBranchAddress( "SherpaNJetWeight"                         , &SherpaNJetWeight               );
    oldtree->SetBranchAddress( "RunYear"                         , &RunYear               );

  //*/

	/*
	       vars[0] = (float)max_eta;
	       vars[1] = (float)lep_Pt_1;
	       vars[2] = (float)Mll01;
	       vars[3] = (float)minDeltaR_LJ_0;
	       vars[4] = (float)minDeltaR_LJ_1;
	       vars[5] = (float)MET_RefFinal_et;
	       vars[6] = (float)((nJets_OR_T < 8) ? nJets_OR_T : 7);
	       vars[7] = (float)(nJets_OR_T_MV2c10_70);
	       vars[8] = (float)lep_flavour;
	       vars[9] = (float)lep_Pt_0;
	       vars[10] = (float)Meff;
	       vars[11] = (float)Ptll01;         
	       vars[12] = (float)HT_lep;         
	       vars[13] = (float)lead_jetPt;
	       vars[14] = (float)sublead_jetPt;
	       vars[15] = (float)lead_BjetPt;
	       vars[16] = (float)lead_jet_sumBEff;
	       vars[17] = (float)sublead_jet_sumBEff;
	       vars[18] = (float)DRll01;
	       vars[19] = (float)lep_Eta_0;      
	       vars[20] = (float)DRlj00;
	       vars[21] = (float)min_DRl0b;
	       vars[22] = (float)lep_E_0;        
	       vars[23] = (float)min_DRlj_new;
	       vars[24] = (float)DPhij0MET;
	       vars[25] = (float)DEtall01;       
	       vars[26] = (float)DRjj01;         
	       vars[27] = (float)lead_jet_is_lead_antitag;
	       vars[28] = (float)HT_jets;        
	       vars[29] = (float)lead_jetEta;    
	       vars[30] = (float)lead_jetE;      
	       vars[31] = (float)MET_RefFinal_sumet;
	       vars[32] = (float)SumPtBJet;
	       vars[33] = (float)lep_Eta_1;      
	       vars[34] = (float)lep_E_1;   
	       //*/

  
  //Create a new file + a clone of old tree in new file
  TFile *newfile = new TFile("tth.root","recreate");
  TTree *newtree = oldtree->CloneTree(0);
  Double_t weightS=1;

  auto newBranch = newtree->Branch("weightS", &weightS, "weightS/D");

  for (Long64_t i=0;i<nentries; i++) {
    oldtree->GetEntry(i);
    weightS = scale_nom*pileupEventWeight_090 *JVT_EventWeight *MV2c10_70_EventWeight * lepSFObjTight *lepSFTrigTight*( 36074.6*( RunYear==2015 || RunYear==2016 ) + 43813.7*( RunYear==2017 ) );
    newtree->Fill();
  }
  newtree->Print();
  newtree->AutoSave();
  delete oldfile;
  delete newfile;
}
