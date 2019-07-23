#include <iostream>
#include <map>
#include <string>


#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TObjString.h"
#include "TSystem.h"
#include "TROOT.h"

#include "TMVA/DataLoader.h"
#include "TMVA/Factory.h"
#include "TMVA/Tools.h"
#include "TMVA/MethodCategory.h"
#include "TMVA/TMVAGui.h"

using namespace std;

int main( int argc, char **argv )
{


  vector<TString> InputVar;
  InputVar = {
    "max_eta"              ,
    "lep_Pt_1"             ,
    "Mll01"                , 
    "minDeltaR_LJ_0"       ,
    "minDeltaR_LJ_1"       ,
    "MET_RefFinal_et"      ,
    "nJets_OR_T"           ,
    "nJets_OR_T_MV2c10_70" ,
    "lep_flavour"          
  };
  
  cout << endl
	    << "==> Start TMVAClassificationCategory" << endl;
  
  // This loads the library
  TMVA::Tools::Instance();
  //outfileName
  TFile* outputFile = TFile::Open("out.root" , "RECREATE" );
  string factoryOptions( "!V:!Silent:Transformations=I" );
  TMVA::Factory *factory = new TMVA::Factory( "TMVAClassificationCategory", outputFile, factoryOptions );
  TMVA::DataLoader *dataloader = new TMVA::DataLoader();

  // Define the input variables used for the MVA training
  Int_t InputVarSize = 0;
  InputVarSize = InputVar.size();
  cout << "There are < " << InputVarSize << " > input variables !!! " << endl;
  for (UInt_t ivar=0; ivar<InputVarSize; ivar++){
    dataloader->AddVariable(InputVar[ivar]);
    cout << ivar + 1 << "\t" << InputVar[ivar] << endl;
  }


  // You can add so-called "Spectator variables", which are not used in the MVA training, 
  // but will appear in the final "TestTree" produced by TMVA. This TestTree will contain the input
  // variables, the response values of all trained MVAs, and the spectator variables
  dataloader->AddSpectator( "total_charge" );
  dataloader->AddSpectator( "lep_flavour" );

  TChain *signal = new TChain("nominal");
  signal->Add("../Files/skimmed/ttH.root");
  TChain *background = new TChain("nominal");
  background->Add("../Files/skimmed/ttW.root");


  int number_training_entries_sig = 0;  int number_training_entries_bkg = 0;
  int number_test_entries_sig = 0;  int number_test_entries_bkg = 0;
  double number_training_events_sig = 0.;  double number_training_events_bkg = 0.;
  double number_test_events_sig     = 0.;  double number_test_events_bkg     = 0.;
  
  vector<Double_t> vars(InputVarSize); // vector has size of number of input variables
  
  Float_t  max_eta                   ; signal->SetBranchAddress( "max_eta"                               , &max_eta                     );
  Float_t lep_Eta_0                  ; signal->SetBranchAddress( "lep_Eta_0"                             , &lep_Eta_0                   );
  Float_t lep_Eta_1                  ; signal->SetBranchAddress( "lep_Eta_1"                             , &lep_Eta_1                   );
  Float_t lep_Pt_0                   ; signal->SetBranchAddress( "lep_Pt_0"                              , &lep_Pt_0                    );
  Float_t lep_Pt_1                   ; signal->SetBranchAddress( "lep_Pt_1"                              , &lep_Pt_1                    );
  Float_t Mll01                      ; signal->SetBranchAddress( "Mll01"                                 , &Mll01                       );
  Float_t Meff                       ; signal->SetBranchAddress( "Meff"                                  , &Meff                        );
  Float_t minDeltaR_LJ_0             ; signal->SetBranchAddress( "minDeltaR_LJ_0"                        , &minDeltaR_LJ_0              );
  Float_t minDeltaR_LJ_1             ; signal->SetBranchAddress( "minDeltaR_LJ_1"                        , &minDeltaR_LJ_1              );
  Float_t MET_RefFinal_et            ; signal->SetBranchAddress( "MET_RefFinal_et"                       , &MET_RefFinal_et             );
  Int_t nJets_OR_T                   ; signal->SetBranchAddress( "nJets_OR_T"                            , &nJets_OR_T                  );
  Int_t nJets_OR_T_MV2c10_70         ; signal->SetBranchAddress( "nJets_OR_T_MV2c10_70"                  , &nJets_OR_T_MV2c10_70        );
  Int_t lep_flavour                  ; signal->SetBranchAddress( "lep_flavour"                           , &lep_flavour                 );
  Int_t total_charge                 ; signal->SetBranchAddress( "total_charge"                          , &total_charge                );
  Double_t weightS                   ; signal->SetBranchAddress( "weightS"                          , &weightS                );
  ULong64_t EventNumber              ; signal->SetBranchAddress( "EventNumber"                           , &EventNumber                 );

  Int_t evenodd = 0;


  for (UInt_t i=0; i<signal->GetEntries(); i++){
    signal->GetEntry(i);
    if (i == 0) cout << "max_eta sig "<< max_eta<<endl; 
    vars[0] = (float)max_eta;
    vars[1] = (float)lep_Pt_1;
    vars[2] = (float)Mll01;
    vars[3] = (float)minDeltaR_LJ_0;
    vars[4] = (float)minDeltaR_LJ_1;
    vars[5] = (float)MET_RefFinal_et;
    vars[6] = (float)((nJets_OR_T < 8) ? nJets_OR_T : 7);
    vars[7] = (float)((nJets_OR_T_MV2c10_70 < 3) ? nJets_OR_T_MV2c10_70 : 2);
    vars[8] = (float)lep_flavour;

    if (EventNumber%2 == evenodd){
      dataloader->AddSignalTrainingEvent( vars, (weightS) );
      number_training_entries_sig++;
      number_training_events_sig += weightS;
    }
    else{
      dataloader->AddSignalTestEvent    ( vars, (weightS) );
      number_test_entries_sig++;
      number_test_events_sig += weightS;
    }
  }//loop over signal



 background->SetBranchAddress( "max_eta"                               , &max_eta                     );
 background->SetBranchAddress( "lep_Eta_0"                             , &lep_Eta_0                   );
 background->SetBranchAddress( "lep_Eta_1"                             , &lep_Eta_1                   );
 background->SetBranchAddress( "lep_Pt_0"                              , &lep_Pt_0                    );
 background->SetBranchAddress( "lep_Pt_1"                              , &lep_Pt_1                    );
 background->SetBranchAddress( "Mll01"                                 , &Mll01                       );
 background->SetBranchAddress( "Meff"                                  , &Meff                        );
 background->SetBranchAddress( "minDeltaR_LJ_0"                        , &minDeltaR_LJ_0              );
 background->SetBranchAddress( "minDeltaR_LJ_1"                        , &minDeltaR_LJ_1              );
 background->SetBranchAddress( "MET_RefFinal_et"                       , &MET_RefFinal_et             );
 background->SetBranchAddress( "nJets_OR_T"                            , &nJets_OR_T                  );
 background->SetBranchAddress( "nJets_OR_T_MV2c10_70"                  , &nJets_OR_T_MV2c10_70        );
 background->SetBranchAddress( "lep_flavour"                           , &lep_flavour                 );
 background->SetBranchAddress( "total_charge"                          , &total_charge                );
 background->SetBranchAddress( "weightS"                          , &weightS                );
 background->SetBranchAddress( "EventNumber"                           , &EventNumber                 );

  for (UInt_t i=0; i<background->GetEntries(); i++) {
    //if (i%100000 == 0) std::cout << whichsig + " vs " + whichbkg << "\t" << trainingevents << "\t Background --- ... Processing event: " << i << std::endl;
    background->GetEntry(i);
    if (i == 0) cout << "max_eta bkg "<< max_eta<<endl; 
    vars[0] = (float)max_eta;
    vars[1] = (float)lep_Pt_1;
    vars[2] = (float)Mll01;
    vars[3] = (float)minDeltaR_LJ_0;
    vars[4] = (float)minDeltaR_LJ_1;
    vars[5] = (float)MET_RefFinal_et;
    vars[6] = (float)((nJets_OR_T < 8) ? nJets_OR_T : 7);
    vars[7] = (float)((nJets_OR_T_MV2c10_70 < 3) ? nJets_OR_T_MV2c10_70 : 2);
    vars[8] = (float)lep_flavour;

    if (EventNumber%2 == evenodd){
      dataloader->AddBackgroundTrainingEvent( vars, (weightS) );
      number_training_entries_bkg++;
      number_training_events_bkg += weightS;
    }
    else{
      dataloader->AddBackgroundTestEvent    ( vars, (weightS) );
      number_test_entries_bkg++;
      number_test_events_bkg += weightS;
    }
    
  }//loop over background      


  // --- end ------------------------------------------------------------
  
  // Apply additional cuts on the signal and background samples (can be different)
  TCut mycuts;   TCut mycutb;
  
  // Tell the factory how to use the training and testing events
  dataloader->PrepareTrainingAndTestTree( mycuts, mycutb,
					  "nTest_Signal=0:nTrain_Background=0:NormMode=NumEvents" );

  //Issues after first run
  ////CreateMVAPdfs 
  //// Error in macro "BDT.C": cannot find directory "Method_BDT" in file: out.root
  ////+++ Could not locate directory 'Method_BDT; Could not locate directory 'Method_BDT' in file out.root

  TString Method_Opt;
  // ttbar Method_Opt = "!H:!V:NTrees=1000:MinNodeSize=1.5%:BoostType=Grad:Shrinkage=0.10:UseBaggedBoost:BaggedSampleFraction=0.5:nCuts=10:MaxDepth=1";
  Method_Opt = "!H:!V:NTrees=1000:MinNodeSize=1.5%:BoostType=Grad:Shrinkage=0.10:UseBaggedBoost:BaggedSampleFraction=0.5:nCuts=20:MaxDepth=2";
    
  // ---- Book MVA methods
  factory->BookMethod(  dataloader, TMVA::Types::kBDT, "out_BDTG", Method_Opt);
  
  // Train MVAs using the set of training events
  factory->TrainAllMethods();
  
  // ---- Evaluate all MVAs using the set of test events
  factory->TestAllMethods();
  
  // ----- Evaluate and compare performance of all configured MVAs
  factory->EvaluateAllMethods();
  
  // --------------------------------------------------------------
  
  // Save the output
  outputFile->Close();
  
  cout << "==> Wrote root file: " << outputFile->GetName() << endl
       << "==> TMVAClassificationCategory is done!" << endl
       << endl
       << "==> To view the results, launch the GUI: \"root -l ./TMVAGui.C\"" << endl
       << endl;
  
  cout << "\t number_training_entries_sig = " << number_training_entries_sig << endl;
  cout << "\t number_test_entries_sig     = " << number_test_entries_sig     << endl;
  cout << "\t number_training_entries_bkg = " << number_training_entries_bkg << endl;
  cout << "\t number_test_entries_bkg     = " << number_test_entries_bkg     << endl;
  cout << endl;
  cout << "\t number_training_events_sig  = " << number_training_events_sig   << endl;
  cout << "\t number_test_events_sig      = " << number_test_events_sig       << endl;
  cout << "\t number_training_events_bkg  = " << number_training_events_bkg   << endl;
  cout << "\t number_test_events_bkg      = " << number_test_events_bkg       << endl;
  cout << endl;
  cout << "\t nb entries sig              = " << number_training_entries_sig + number_test_entries_sig   << endl;
  cout << "\t nb entries bkg              = " << number_training_entries_bkg + number_test_entries_bkg   << endl;
  cout << "\t nb events sig               = " << number_training_events_sig  + number_test_events_sig    << endl;
  cout << "\t nb events bkg               = " << number_training_events_bkg  + number_test_events_bkg    << endl;

  // Clean up
  delete factory;
  delete dataloader;
  


}
