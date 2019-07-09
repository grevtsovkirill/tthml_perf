

void tthml_skim() {


  TFile *oldfile = new TFile("Files/preskimmed/ttW_skim.root");
  TTree *oldtree = (TTree*)oldfile->Get("nominal");
  Long64_t nentries = oldtree->GetEntries();
  oldtree->SetBranchStatus("*",0);
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
  //finish weights

  //variables to test training:
  //leptons:
  oldtree->SetBranchStatus( "max_eta"                     , 1);
  oldtree->SetBranchStatus( "Mll01"                       , 1);
  oldtree->SetBranchStatus( "lep_flavour"                 , 1);
  oldtree->SetBranchStatus( "lep_Pt_0"                    , 1);
  oldtree->SetBranchStatus( "lep_Phi_0"                   , 1);
  oldtree->SetBranchStatus( "lep_Eta_0"                   , 1);
  oldtree->SetBranchStatus( "lep_Pt_1"                    , 1);
  oldtree->SetBranchStatus( "lep_Phi_1"                   , 1);
  oldtree->SetBranchStatus( "lep_Eta_1"                   , 1);
  oldtree->SetBranchStatus( "Ptll01"                      , 1);
  oldtree->SetBranchStatus( "HT_lep"                      , 1);
  oldtree->SetBranchStatus( "DRll01"                      , 1);
  oldtree->SetBranchStatus( "DEtall01"                    , 1);
  //jets
  oldtree->SetBranchStatus( "DRlj00"                      , 1);
  oldtree->SetBranchStatus( "min_DRlj_new"                , 1);
  oldtree->SetBranchStatus( "minDeltaR_LJ_0"              , 1);
  oldtree->SetBranchStatus( "minDeltaR_LJ_1"              , 1);
  oldtree->SetBranchStatus( "nJets_OR_T"                  , 1);
  oldtree->SetBranchStatus( "nJets_OR_T_MV2c10_70"        , 1);
  oldtree->SetBranchStatus( "Meff"                        , 1);
  oldtree->SetBranchStatus( "HT_jets"                     , 1);
  oldtree->SetBranchStatus( "lead_jetPt"                  , 1);
  oldtree->SetBranchStatus( "sublead_jetPt"               , 1);
  oldtree->SetBranchStatus( "lead_jet_sumBEff"            , 1);
  oldtree->SetBranchStatus( "sublead_jet_sumBEff"         , 1);
  oldtree->SetBranchStatus( "lead_BjetPt"                 , 1);
  oldtree->SetBranchStatus( "min_DRl0b"                   , 1);
  oldtree->SetBranchStatus( "MET_RefFinal_et"             , 1);
  oldtree->SetBranchStatus( "DPhij0MET"                   , 1);
	

  
  //Create a new file + a clone of old tree in new file
  //TFile *newfile = new TFile("tth.root","recreate");
  TFile *newfile = new TFile("ttW.root","recreate");
  TTree *newtree = oldtree->CloneTree(0);
  Double_t weightS=1;

  auto newBranch = newtree->Branch("weightS", &weightS, "weightS/D");

  for (Long64_t i=0;i<nentries; i++) {
    oldtree->GetEntry(i);
    weightS = scale_nom*pileupEventWeight_090 *JVT_EventWeight *MV2c10_70_EventWeight * lepSFObjTight *lepSFTrigTight*( 36074.6*( RunYear==2015 || RunYear==2016 ) + 43813.7*( RunYear==2017 ) );
    newtree->Fill();
  }
  //newtree->Print();
  newtree->AutoSave();
  delete oldfile;
  delete newfile;
}
