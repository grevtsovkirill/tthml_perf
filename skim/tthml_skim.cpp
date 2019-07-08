

void tthml_skim() {


  TFile *oldfile = new TFile("Files/signal_a.root");
  TTree *oldtree = (TTree*)oldfile->Get("nominal");
  Long64_t nentries = oldtree->GetEntries();
  //*
  oldtree->SetBranchStatus("*",0);
  oldtree->SetBranchStatus("lep_Pt_0",1);
  oldtree->SetBranchStatus( "pileupEventWeight_090",1);
  oldtree->SetBranchStatus( "scale_nom"                   , 1);
  oldtree->SetBranchStatus( "JVT_EventWeight"             , 1);
  oldtree->SetBranchStatus( "MV2c10_70_EventWeight"       , 1);
  oldtree->SetBranchStatus( "lepSFObjTight"               , 1);
  //*/
  //oldtree->SetBranchAddress("lep_Pt_0",&event);

  Float_t lep_Pt_0; oldtree->SetBranchAddress( "lep_Pt_0", &lep_Pt_0   );
  Float_t scale_nom                  ; oldtree->SetBranchAddress( "scale_nom"                             , &scale_nom                   );
  Double_t pileupEventWeight_090; oldtree->SetBranchAddress( "pileupEventWeight_090", &pileupEventWeight_090   );
  Double_t JVT_EventWeight           ; oldtree->SetBranchAddress( "JVT_EventWeight"                       , &JVT_EventWeight             );
  Double_t MV2c10_70_EventWeight     ; oldtree->SetBranchAddress( "MV2c10_70_EventWeight"                 , &MV2c10_70_EventWeight       );
  Float_t lepSFObjTight              ; oldtree->SetBranchAddress( "lepSFObjTight"                         , &lepSFObjTight               );

  
  //Create a new file + a clone of old tree in new file
  TFile *newfile = new TFile("tth.root","recreate");
  TTree *newtree = oldtree->CloneTree(0);
  Double_t weightS=1;

  auto newBranch = newtree->Branch("weightS", &weightS, "weightS/D");

  for (Long64_t i=0;i<nentries; i++) {
    oldtree->GetEntry(i);

    if (lep_Pt_0<50000){
    weightS = scale_nom*pileupEventWeight_090 *JVT_EventWeight *MV2c10_70_EventWeight * lepSFObjTight ;
    cout << scale_nom<< "  " << pileupEventWeight_090 << "  " << JVT_EventWeight << "  " << MV2c10_70_EventWeight << "  " <<  lepSFObjTight  <<endl;

      newtree->Fill();
    }
  }
  newtree->Print();
  newtree->AutoSave();
  delete oldfile;
  delete newfile;
}
