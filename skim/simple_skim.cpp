void simple_skim() {


  TFile *oldfile = new TFile("Files/signal_a.root");
  TTree *oldtree = (TTree*)oldfile->Get("nominal");
  Long64_t nentries = oldtree->GetEntries();
  oldtree->SetBranchStatus("*",0);
  oldtree->SetBranchStatus("lep_Pt_0",1);
  oldtree->SetBranchStatus( "pileupEventWeight_090",1);

  //oldtree->SetBranchAddress("lep_Pt_0",&event);

  Float_t lep_Pt_0; oldtree->SetBranchAddress( "lep_Pt_0", &lep_Pt_0   );
  Double_t pileupEventWeight_090; oldtree->SetBranchAddress( "pileupEventWeight_090", &pileupEventWeight_090   );

  //Create a new file + a clone of old tree in new file
  TFile *newfile = new TFile("small.root","recreate");
  TTree *newtree = oldtree->CloneTree(0);

  for (Long64_t i=0;i<nentries; i++) {
    oldtree->GetEntry(i);
    if (lep_Pt_0<50000){
      lep_Pt_0*=pileupEventWeight_090;
      newtree->Fill();
    }
  }
  newtree->Print();
  newtree->AutoSave();
  delete oldfile;
  delete newfile;
}
