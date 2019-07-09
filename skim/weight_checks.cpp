#include "/Users/grevtsov/Documents/working_files/AtlasStyle/AtlasStyle.C"
#include "/Users/grevtsov/Documents/working_files/AtlasStyle/AtlasUtils.C"

void weight_checks(){
  gROOT->Reset();
  //  SetAtlasStyle();
  //gStyle->SetPalette(1);

  TFile * file_data[2];
  TTree *tree[2];
  TH1F *h_lep_Pt_0[2];
  Float_t lep_Pt_0[2]; 
  Double_t pileupEventWeight_090[2];
  //string fname[2]={"unw_small.root","hand_w_small.root"};//hand_w_small.root hand_w_after_small.root
  string fname[2]={"/Users/grevtsov/Documents/working_files/ttH/ttH-ML/TemplateFit/ttbar/acceptance/skim_ntup/source/skim.root","hand_w_small.root"};
  Long_t entries[2];
  Double_t JVT_EventWeight[2]           ; 
  Float_t scale_nom[2]                  ; 
  Double_t MV2c10_70_EventWeight[2]     ; 
  Double_t SherpaNJetWeight[2]     ; 
  Float_t lepSFObjTight[2]              ; 
  Float_t lepSFTrigTight[2]              ; 
  Int_t RunYear[2];

  Double_t w_tot=1;
  for (int f=0;f<1;f++){
    file_data[f]= TFile::Open(fname[f].c_str());
    tree[f]= (TTree*)file_data[f]->Get("nominal");
    tree[f]->SetBranchAddress( "lep_Pt_0", &lep_Pt_0[f]   );
    tree[f]->SetBranchAddress( "pileupEventWeight_090", &pileupEventWeight_090[f]);
    //*

    tree[f]->SetBranchAddress( "scale_nom"                             , &scale_nom[f]                   );
    tree[f]->SetBranchAddress( "JVT_EventWeight"                       , &JVT_EventWeight[f]             );
    tree[f]->SetBranchAddress( "MV2c10_70_EventWeight"                 , &MV2c10_70_EventWeight[f]       );
    tree[f]->SetBranchAddress( "lepSFObjTight"                         , &lepSFObjTight[f]               );
    tree[f]->SetBranchAddress( "lepSFTrigTight"                         , &lepSFTrigTight[f]               );
    tree[f]->SetBranchAddress( "SherpaNJetWeight"                         , &SherpaNJetWeight[f]               );
    tree[f]->SetBranchAddress( "RunYear"                         , &RunYear[f]               );
  //*/
    //h_lep_Pt_0[f] = new TH1F("lep_Pt_0_0","lep_Pt_0_0",160,0,80);
    h_lep_Pt_0[f] = new TH1F("lep_Pt_0_0","lep_Pt_0_0",100, 0, 1000);
    
    entries[f]= tree[f]->GetEntries();
    //

    for (Int_t i=0; i<entries[f]; i++) {
      tree[f]->GetEntry(i);
      w_tot=pileupEventWeight_090[f]*scale_nom[f]*JVT_EventWeight[f]  *MV2c10_70_EventWeight[f] *lepSFObjTight[f] * lepSFTrigTight[f]* SherpaNJetWeight[f] * ( 36074.6*( RunYear[f]==2015 || RunYear[f]==2016 ) + 43813.7*( RunYear[f]==2017 ) );
      if(f==0)
	h_lep_Pt_0[f]->Fill(lep_Pt_0[f]/1000,w_tot);
      else h_lep_Pt_0[f]->Fill(lep_Pt_0[f]/1000);
    }
  }

  TCanvas* canv = new TCanvas("c1", "c1",10,10,800,600);
  canv->cd();
  h_lep_Pt_0[0]->Draw();
  //h_lep_Pt_0[1]->SetLineColor(2);  h_lep_Pt_0[1]->Draw("same");


}
