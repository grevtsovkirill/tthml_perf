#include "/Users/grevtsov/Documents/working_files/AtlasStyle/AtlasStyle.C"
#include "/Users/grevtsov/Documents/working_files/AtlasStyle/AtlasUtils.C"

void weight_checks(){
  gROOT->Reset();
  SetAtlasStyle();
  gStyle->SetPalette(1);

  TFile * file_data[2];
  TTree *tree[2];
  TH1F *h_lep_Pt_0[2];
  Float_t lep_Pt_0[2]; 
  Double_t pileupEventWeight_090[2];
  string fname[2]={"unw_small.root","hand_w_small.root"};//hand_w_small.root hand_w_after_small.root
  Long_t entries;

  for (int f=0;f<2;f++){
    file_data[f]= TFile::Open(fname[f].c_str());
    tree[f]= (TTree*)file_data[f]->Get("nominal");
    tree[f]->SetBranchAddress( "lep_Pt_0", &lep_Pt_0[f]   );
    tree[f]->SetBranchAddress( "pileupEventWeight_090", &pileupEventWeight_090[f]);
    h_lep_Pt_0[f] = new TH1F("lep_Pt_0_0","lep_Pt_0_0",160,0,80);
    
    entries= tree[f]->GetEntries();

    for (Int_t i=0; i<entries; i++) {
      tree[f]->GetEntry(i);
      if(f==0)
	h_lep_Pt_0[f]->Fill(lep_Pt_0[f]/1000,pileupEventWeight_090[f]);
      else h_lep_Pt_0[f]->Fill(lep_Pt_0[f]/1000);
    }
  }

  TCanvas* canv = new TCanvas("c1", "c1",10,10,800,600);
  canv->cd();
  h_lep_Pt_0[0]->Draw();
  h_lep_Pt_0[1]->SetLineColor(2);  h_lep_Pt_0[1]->Draw("same");


}
