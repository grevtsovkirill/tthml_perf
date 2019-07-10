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

}
