#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TMath.h"
#include "vector"
#include <iostream>
#include <fstream>
#include <sstream>
#include <TROOT.h>
#include <string>
#include <sys/stat.h>

void ugly_2lss_sel(string inpFile, string outputPath, int nEvents=0, string prefix="", string syst="nominal", string fakeTypes="") {
  
  if ( !prefix.empty() ) printf("Running '%s%s'... \n", prefix.c_str(), fakeTypes.c_str() );
  else{
    printf("Wrong input arguments!...Check that arguments are: (inpRootFile, outPath, prefix, syst, fakeTypes). The 'prefix' argument (optional) takes only ['fakes','qmisidDD','qmisidMC','data','mc'] values... \n");
    exit(0);
  }

  // TFile types...
  TFile *f;
  TFile *outFile;

  // TTree types...
  TTree *fChain;

  // TH1D types...
  TH1D  *h_2SS0tau_CutFlow;
  TH1D  *h_2SS0tau_CutFlow_Raw;
  TH1D  *h_lep_Pt_0;

  // Bool types...
  Bool_t debug;
  Bool_t isData;
  Bool_t common_selection;
  Bool_t master_selection;
  Bool_t TemplateFit_CR;
  Bool_t passAtLeastOneLep;
  Bool_t passEventClean;
  Bool_t passTrig;
  Bool_t passDilepton;
  Bool_t pass2LSS;
  Bool_t passTrigMatch;
  Bool_t passTightLep;
  Bool_t passNJets;
  Bool_t passNbtagJets;
  Bool_t passTauCut;
  Bool_t passLessThan3Bjets;
  Bool_t passLepPtCut;
  Bool_t is2lSS_2Tau_NonPrompt;
  Bool_t passTightLep_NonPrompt;
  Bool_t passPromptLep;
  Bool_t passZCR;
  Bool_t passDRllCut;
  Bool_t passEtaCut;
  Bool_t passMllCut;
  Bool_t passQMisIDLep;
  Bool_t passTtbarConv;
  Bool_t passConvInt;
  Bool_t passHasMEphoton_tty;
  Bool_t passElecDefinition;
  Bool_t lep_isFake_0;
  Bool_t lep_isFake_1;
  Bool_t isTight_0;
  Bool_t isTight_1;
  Bool_t isNotIntConvElect_0;
  Bool_t isNotIntConvElect_1;
  Bool_t isNotExtConvElect_0;
  Bool_t isNotExtConvElect_1;
  Bool_t doQMisID;
  Bool_t doTFfakes_convInt;
  Bool_t doTFfakes_convExt;
  Bool_t doTFfakes_hf_e;
  Bool_t doTFfakes_hf_mu;
  Bool_t doTFfakes_lf_e;
  Bool_t doTFfakes_lf_mu;
  Bool_t lep_isHFfake_0;
  Bool_t lep_isHFfake_1;

 
  // Event categorization....
  Bool_t is2mu1b5j;
  Bool_t isOF1b5j;
  Bool_t is2e1b5j;
  Bool_t is2mu2b5j;
  Bool_t isOF2b5j;
  Bool_t is2e2b5j;
  Bool_t is2mu1b4j;
  Bool_t isOF1b4j;
  Bool_t is2e1b4j;
  Bool_t is2mu2b4j;
  Bool_t isOF2b4j;
  Bool_t is2e2b4j;


  Float_t Norm_ttW = 1.755;
  Float_t Norm_ttbar_ConvExt = 1.9728;
  Float_t Norm_ttbar_ConvInt = 0.805799;
  Float_t Norm_ttbar_HF_e = 0.826749;
  Float_t Norm_ttbar_HF_mu = 1.15523;
  Float_t Norm_ttbar_LF_e = 0.826749;
  Float_t Norm_ttbar_LF_mu = 1.15523;
  //vector<string> samples;
  vector<TH1D*> all1DHists;
  vector<TH2D*> all2DHists;

  //pointers...

  //Structures
  struct stat sb;

  // String types...
  string name = "XXXXX";
  string inputFileName = "XXXXX";
    
  // Int types...
  Int_t lepPairFlav;
    
  // Log int types...
  ULong64_t nentries;
    
  // float types...
  Float_t percent;
  Float_t event_weight;
  Float_t e_mass = 0.000511; // GeV
  Float_t mu_mass = 0.1056583715; // GeV
  Float_t Z_mass = 91.1876; // GeV
  Float_t m0, m1, px0, px1, py0, py1, pz0, pz1, e0, e1, mll;
  Float_t maxEta;
    
  Float_t RadUp_weight;
  Float_t ttW_weight;
  Float_t lumi_weight;
  Float_t fakes_HF_weight;
  Float_t fakes_Conv_weight;
    
    
  // Switch to zero if you dont want to apply fake NFs...
  Bool_t scaleFakes = false;  // set this as true if you want to apply the Fakes scale factor...
  Bool_t scaleTtW = false;	// set this as true if you want to apply the ttW scale factor...
    
  // Declaration of leaf types
  Bool_t          is1L2Tau;
  Bool_t          is2LSS0Tau;
  Bool_t          pass_tight_2L;
  Bool_t          pass_SS_2L;
  Bool_t          is2LSS1Tau;
  Bool_t          is2LOS1Tau;
  Bool_t          is3L;
  Bool_t          is3L1Tau;
  Bool_t          is4L;
  Bool_t          is2lLowNJet0Tau;
  Bool_t          is2LSS0Tau_SR_Relaxed;
  Bool_t          is2LSS1Tau_SR_Relaxed;
  Bool_t          is3L0Tau_SR_Relaxed;
  Bool_t          is3L0Tau_SR_Relaxed3j;
  Bool_t          is4L_SR_VV_Relaxed;
  Bool_t          is4L_SR_ttV_Relaxed;
  Bool_t          is4L_SR_ttV_Zenriched;
  Bool_t          is3L0Tau_CR_ttbar_Relaxed;
  Bool_t          is3L0Tau_CR_VV_Relaxed;
  Bool_t          is3L0Tau_CR_ttW_Relaxed;
  Bool_t          is2L_VR_ttW_ICHEP;
  Bool_t          is2L_VR_ttbarFakes_ICHEP;
  Bool_t          is4lCRttZRelaxed;
  Bool_t          is2LOS1Tau_CR_ttbar;
  Bool_t          is2LOS1Tau_CR_ttZ;
  Char_t          is2lOS0TauBaseline;
  Char_t          is2lOS0TauBaseline_ee;
  Char_t          is2lOS0TauBaseline_mm;
  Char_t          is2lOS0TauBaseline_OF;
  Char_t          is2lOS0TauBaseline_me;
  Char_t          is2lOS0TauBaseline_em;
  Char_t          is2lSS0TauBaseline;
  Char_t          is2lSS0TauBaseline_ee;
  Char_t          is2lSS0TauBaseline_mm;
  Char_t          is2lSS0TauBaseline_OF;
  Char_t          is2lSS0TauBaseline_me;
  Char_t          is2lSS0TauBaseline_em;
  Char_t          is2lOS1TauBaseline;
  Char_t          is2lOS1TauBaseline_ee;
  Char_t          is2lOS1TauBaseline_mm;
  Char_t          is2lOS1TauBaseline_OF;
  Char_t          is2lOS1TauBaseline_me;
  Char_t          is2lOS1TauBaseline_em;
  Char_t          is2lSS1TauBaseline;
  Char_t          is2lSS1TauBaseline_ee;
  Char_t          is2lSS1TauBaseline_mm;
  Char_t          is2lSS1TauBaseline_OF;
  Char_t          is2lSS1TauBaseline_me;
  Char_t          is2lSS1TauBaseline_em;
  Char_t          is3lBaseline;
  Char_t          is3lBaselineNoZVeto;
  Bool_t          isGNv02;
  Float_t         minOSMll;
  Float_t         minOSSFMll;
  Float_t         jet_flvWtOrdr_0_pt;
  Float_t         jet_flvWtOrdr_0_eta;
  Float_t         jet_flvWtOrdr_0_phi;
  Float_t         jet_flvWtOrdr_0_E;
  Float_t         jet_flvWtOrdr_0_mv2c10;
  Float_t         jet_flvWtOrdr_1_pt;
  Float_t         jet_flvWtOrdr_1_eta;
  Float_t         jet_flvWtOrdr_1_phi;
  Float_t         jet_flvWtOrdr_1_E;
  Float_t         jet_flvWtOrdr_1_mv2c10;
  Float_t         jet_flvWtOrdr_2_pt;
  Float_t         jet_flvWtOrdr_2_eta;
  Float_t         jet_flvWtOrdr_2_phi;
  Float_t         jet_flvWtOrdr_2_E;
  Float_t         jet_flvWtOrdr_2_mv2c10;
  Float_t         DEtall01;
  Float_t         DEtall02;
  Float_t         DEtall12;
  Int_t           lep_flavour;
  Int_t           jet_flavor;
  Int_t           total_charge_2lSS;
  Int_t           isExtConv;
  Int_t           isIntConv;
  Float_t         max_eta;
  Float_t         SDRll;
  Float_t         DeltaR_max_lep;
  Float_t         DPhi_max_lep_met;
  Float_t         DeltaR_min_lep_jet;
  Float_t         minDeltaR_LJ_0;
  Float_t         minDeltaR_LJ_1;
  Float_t         minDeltaR_LJ_2;
  Float_t         Meff;
  Float_t         Mjj_closeW;
  Float_t         DRlb_h;
  Float_t         ptlb_h;
  Float_t         DeltaRLeadjets;
  Float_t         jet1pTbin;
  Float_t         jet2pTbin;
  Float_t         jet3pTbin;
  Float_t         jet3etabin;
  Float_t         jet1MV2c10_bin;
  Float_t         jet2MV2c10_bin;
  Float_t         MvisH;
  Float_t         MvisnonH;
  Float_t         lep_PtVarCone20Rel_1;
  Float_t         DeltaRLeadLepClosestJet;
  Float_t         Ml2l3met;
  Float_t         FSF_4L_tot;
  Float_t         FSF_4L_tot_up;
  Float_t         FSF_4L_tot_down;
  Int_t           FSF_category;
  Float_t         jet2_pt;
  Float_t         jet2_eta;
  Float_t         jet2_phi;
  Float_t         jet3_pt;
  Float_t         jet3_eta;
  Float_t         jet3_phi;
  Int_t           jet0_tagWeightBin;
  Int_t           jet1_tagWeightBin;
  Int_t           jet2_tagWeightBin;
  Int_t           jet3_tagWeightBin;
  Int_t           N_el;
  Int_t           N_mu;
  Float_t         tempDPl0t;
  Float_t         tempDPl1t;
  Float_t         tempDP2l;
  Float_t         tempDPlt_closest;
  Float_t         tempDPlt_furthest;
  Float_t         DeltaRSubLepClosestJet;
  Int_t           SignProduct2sslt;
  Float_t         TransverseMassLeadLepMET;
  Float_t         LD_HTmiss_ETmiss;
  Float_t         Mjj_nonb;
  Float_t         Mtop;
  Float_t         MtopW;
  Float_t         temptopoetcone20overpTlep0;
  Float_t         temptopoetcone20overpTlep1;
  Float_t         tempptvarcone20overpTlep0;
  Float_t         tempptvarcone20overpTlep1;
  Float_t         tau_tight_0;
  Float_t         tau_tight_1;
  Float_t         tau_btag70_0;
  Float_t         tau_btag70_1;
  Float_t         tau_truth_0;
  Float_t         tau_truth_1;
  Float_t         htjets_1l2tau;
  Float_t         jjdrmin_1l2tau;
  Float_t         mtautau_1l2tau;
  Float_t         njets_1l2tau;
  Float_t         nbjets_1l2tau;
  Float_t         DRlj00;
  Float_t         DRjj01;
  Float_t         min_DRl0j;
  Float_t         min_DRl1j;
  Float_t         min_DRl2j;
  Float_t         min_DRl0b;
  Float_t         min_DRl1b;
  Float_t         min_DRl2b;
  Float_t         min_DRlj_new;
  Int_t           nJets_OR_T_overflow;
  Int_t           nJets_OR_T_MV2c10_70_overflow;
  Float_t         DPhij0MET;
  Float_t         lead_BjetPt;
  Float_t         lead_jet_sumBEff;
  Float_t         sublead_jet_sumBEff;
  Float_t         lead_jet_is_lead_antitag;
  Float_t         lead_jet_is_lead_tag;
  Float_t         lep_Pt_1_new;
  Float_t         lep_Pt_2_new;
  Float_t         lep_ID_1_new;
  Float_t         lep_ID_2_new;
  Float_t         EventWeight;
  Char_t          lep_isTightSelected_0;
  Char_t          lep_isTightSelected_1;
  Char_t          lep_isTightSelected_2;
  Char_t          lep_isTightSelected_3;
  Char_t          lep_isTightSelected_2LOS1Tau_0;
  Char_t          lep_isTightSelected_2LOS1Tau_1;
  Char_t          lep_isTightSelectedAndPt_0;
  Char_t          lep_isTightSelectedAndPt_1;
  Char_t          lep_isTightSelectedAndPt_2;
  Char_t          lep_isTightSelectedAndPt_3;
  Char_t          lep_isTightSelectedAndPt_2LOS1Tau_0;
  Char_t          lep_isTightSelectedAndPt_2LOS1Tau_1;
  //vector<int>     *jet_flavor_truth_label_ghost;
  //vector<float> *jet_flavor_weight_MV2c10;
  //vector<float>   *jet_pT;
  //vector<float>   *jet_eta;
  Float_t         MVA2lSSMarseille_weight_ttV;
  Float_t         MVA2lSSMarseille_weight_ttbar;
  Float_t         MVA2lSSMarseille_weight_ttV_2;
  Float_t         MVA2lSSMarseille_weight_ttbar_2;
  Float_t         MVA3lCERN_weight_ttH;
  Float_t         MVA3lCERN_weight_ttW;
  Float_t         MVA3lCERN_weight_ttZ;
  Float_t         MVA3lCERN_weight_VV;
  Float_t         MVA3lCERN_weight_ttbar;
  Float_t         MVA3lCERN_category;
  Bool_t          isLep2SS0tau_ee_QMisID;
  Bool_t          isLep2SS0tau_OF_QMisID;
  Bool_t          is2lttW_VR_ICHEP_QMisID;
  Float_t         total_charge_QMisID;
  Float_t         QMisID_EventWeight;
  Float_t         QMisID_EventWeight_Up;
  Float_t         QMisID_EventWeight_Dn;
  Bool_t          isLep2SS1tau_QMisID;
  Float_t         total_charge_2ltau_QMisID;
  Float_t         QMisID_2ltau_EventWeight;
  Float_t         QMisID_2ltau_EventWeight_Up;
  Float_t         QMisID_2ltau_EventWeight_Dn;
  Char_t          passTrigger;
  Float_t         scale_nom;
  Float_t         scale_varA14up;
  Float_t         scale_varA14down;
  Double_t        mcWeightOrg;
  Double_t        pileupEventWeight_090;
  Double_t        MV2c10_60_EventWeight;
  Double_t        MV2c10_70_EventWeight;
  Double_t        MV2c10_77_EventWeight;
  Double_t        MV2c10_85_EventWeight;
  Double_t        MV2c10_Continuous_EventWeight;
  Double_t        bTagSF_weight_Continuous;
  Double_t        JVT_EventWeight;
  Double_t        SherpaNJetWeight;
  ULong64_t       EventNumber;
  UInt_t          RunNumber;
  UInt_t          lbn;
  UInt_t          bcid;
  Bool_t          passEventCleaning;
  Float_t         m_vxp_x;
  Float_t         m_vxp_y;
  Float_t         m_vxp_z;
  Double_t        higgs_pt;
  Double_t        higgs_eta;
  Double_t        higgs_phi;
  Double_t        higgs_E;
  Double_t        higgs_rapidity;
  Double_t        top_pt;
  Double_t        top_eta;
  Double_t        top_phi;
  Double_t        top_E;
  Double_t        antitop_pt;
  Double_t        antitop_eta;
  Double_t        antitop_phi;
  Double_t        antitop_E;
  Int_t           RunYear;
  Int_t           HF_Classification;
  Int_t           HF_ClassificationTop;
  Int_t           DLF_Classification;
  Int_t           ttbar_HF_EventType;
  Int_t           higgsDecayMode;
  UInt_t          mc_channel_number;
  Char_t          mc_isAFII;
  Float_t         averageIntPerXing_uncorr;
  Float_t         averageIntPerXing;
  Float_t         actualIntPerXing;
  Int_t           m_vxp_n;
  Int_t           m_vxpu_n;
  Float_t         MET_RefFinal_et;
  Float_t         MET_RefFinal_phi;
  Float_t         MET_RefFinal_sumet;
  Char_t          m_hasFSRPhotonLargeDeltaR;
  Char_t          m_hasMEphoton;
  Char_t          m_hasMEphoton_DRgt02_nonhad;
  Char_t          m_MEphoton_OLtty_keepEvent;
  Char_t          m_MEphoton_OLtty_cat1;
  Char_t          m_MEphoton_OLtty_cat2;
  Char_t          m_MEphoton_OLtty_cat3;
  Char_t          m_MEphoton_OLtty_cat4;
  Float_t         m_MEphoton_pt;
  Float_t         m_MEphoton_eta;
  Float_t         m_MEphoton_phi;
  Float_t         m_MEphoton_mother_pt;
  Float_t         m_MEphoton_mother_eta;
  Float_t         m_MEphoton_mother_phi;
  Int_t           m_MEphoton_mother_pdgId;
  Char_t          HLT_2e12_lhloose_L12EM10VH;
  Char_t          HLT_2e12_lhvloose_nod0_L12EM10VH;
  Char_t          HLT_2e17_lhvloose_nod0;
  Char_t          HLT_2e24_lhvloose_nod0;
  Char_t          HLT_e120_lhloose;
  Char_t          HLT_e140_lhloose;
  Char_t          HLT_e140_lhloose_nod0;
  Char_t          HLT_e17_lhloose_mu14;
  Char_t          HLT_e17_lhloose_nod0_mu14;
  Char_t          HLT_e24_lhmedium_L1EM18VH;
  Char_t          HLT_e24_lhmedium_L1EM20VH;
  Char_t          HLT_e24_lhmedium_nod0_L1EM20VH;
  Char_t          HLT_e26_lhtight_iloose;
  Char_t          HLT_e26_lhtight_nod0_ivarloose;
  Char_t          HLT_e60_lhmedium;
  Char_t          HLT_e60_lhmedium_nod0;
  Char_t          HLT_e60_medium;
  Char_t          HLT_e7_lhmedium_mu24;
  Char_t          HLT_e7_medium_mu24;
  Char_t          HLT_mu18_mu8noL1;
  Char_t          HLT_mu20_iloose_L1MU15;
  Char_t          HLT_mu20_mu8noL1;
  Char_t          HLT_mu22_mu8noL1;
  Char_t          HLT_mu24_imedium;
  Char_t          HLT_mu24_ivarloose;
  Char_t          HLT_mu24_ivarmedium;
  Char_t          HLT_mu26_imedium;
  Char_t          HLT_mu26_ivarmedium;
  Char_t          HLT_mu50;
  Char_t          HLT_2e17_lhvloose_nod0_L12EM15VHI;
  Short_t         lep_promptLeptonInput_sv1_jf_ntrkv_0;
  Short_t         lep_promptLeptonInput_sv1_jf_ntrkv_1;
  Short_t         lep_promptLeptonInput_sv1_jf_ntrkv_2;
  Short_t         lep_promptLeptonInput_TrackJetNTrack_0;
  Short_t         lep_promptLeptonInput_TrackJetNTrack_1;
  Short_t         lep_promptLeptonInput_TrackJetNTrack_2;
  Float_t         lep_promptLeptonInput_DL1mu_0;
  Float_t         lep_promptLeptonInput_DL1mu_1;
  Float_t         lep_promptLeptonInput_DL1mu_2;
  Float_t         lep_promptLeptonInput_DRlj_0;
  Float_t         lep_promptLeptonInput_DRlj_1;
  Float_t         lep_promptLeptonInput_DRlj_2;
  Float_t         lep_promptLeptonInput_LepJetPtFrac_0;
  Float_t         lep_promptLeptonInput_LepJetPtFrac_1;
  Float_t         lep_promptLeptonInput_LepJetPtFrac_2;
  Float_t         lep_promptLeptonInput_PtFrac_0;
  Float_t         lep_promptLeptonInput_PtFrac_1;
  Float_t         lep_promptLeptonInput_PtFrac_2;
  Float_t         lep_promptLeptonInput_PtRel_0;
  Float_t         lep_promptLeptonInput_PtRel_1;
  Float_t         lep_promptLeptonInput_PtRel_2;
  Float_t         lep_promptLeptonInput_ip2_0;
  Float_t         lep_promptLeptonInput_ip2_1;
  Float_t         lep_promptLeptonInput_ip2_2;
  Float_t         lep_promptLeptonInput_ip3_0;
  Float_t         lep_promptLeptonInput_ip3_1;
  Float_t         lep_promptLeptonInput_ip3_2;
  Float_t         lep_promptLeptonInput_rnnip_0;
  Float_t         lep_promptLeptonInput_rnnip_1;
  Float_t         lep_promptLeptonInput_rnnip_2;
  Float_t         lep_ID_0;
  Int_t           lep_Index_0;
  UChar_t         lep_ambiguityType_0;
  Float_t         lep_Pt_0;
  Float_t         lep_E_0;
  Float_t         lep_Eta_0;
  Float_t         lep_Phi_0;
  Float_t         lep_EtaBE2_0;
  Float_t         lep_topoEtcone20_0;
  Float_t         lep_topoEtcone30_0;
  Float_t         lep_topoEtcone40_0;
  Float_t         lep_ptVarcone20_0;
  Float_t         lep_ptVarcone30_0;
  Float_t         lep_ptVarcone40_0;
  Float_t         lep_sigd0PV_0;
  Float_t         lep_Z0SinTheta_0;
  Float_t         lep_d0_0;
  Float_t         lep_z0_0;
  Float_t         lep_vz_0;
  Float_t         lep_deltaz0_0;
  Char_t          lep_isTightLH_0;
  Char_t          lep_isMediumLH_0;
  Char_t          lep_isLooseLH_0;
  Char_t          lep_isTight_0;
  Char_t          lep_isMedium_0;
  Char_t          lep_isLoose_0;
  Int_t           lep_isolationLooseTrackOnly_0;
  Int_t           lep_isolationLoose_0;
  Int_t           lep_isolationGradient_0;
  Int_t           lep_isolationGradientLoose_0;
  Int_t           lep_isolationFixedCutTight_0;
  Int_t           lep_isolationFixedCutTightTrackOnly_0;
  Int_t           lep_isolationFixedCutLoose_0;
  Char_t          lep_isTrigMatch_0;
  Char_t          lep_isTrigMatchDLT_0;
  Char_t          lep_isPrompt_0;
  Char_t          lep_isFakeLep_0;
  Char_t          lep_isQMisID_0;
  Char_t          lep_isConvPh_0;
  Char_t          lep_isIntConvPh_0;
  Char_t          lep_isExtConvPh_0;
  Char_t          lep_isISR_FSR_Ph_0;
  Char_t          lep_isBrems_0;
  Char_t          lep_isTruthMatched_0;
  Int_t           lep_truthType_0;
  Int_t           lep_truthOrigin_0;
  Int_t           lep_truthPdgId_0;
  Int_t           lep_truthStatus_0;
  Int_t           lep_truthParentType_0;
  Int_t           lep_truthParentOrigin_0;
  Int_t           lep_truthParentPdgId_0;
  Int_t           lep_truthParentStatus_0;
  Float_t         lep_truthPt_0;
  Float_t         lep_truthEta_0;
  Float_t         lep_truthPhi_0;
  Float_t         lep_truthM_0;
  Float_t         lep_truthE_0;
  Float_t         lep_truthRapidity_0;
  Float_t         lep_SFIDLoose_0;
  Float_t         lep_SFIDTight_0;
  Float_t         lep_SFTrigLoose_0;
  Float_t         lep_SFTrigTight_0;
  Float_t         lep_EffTrigLoose_0;
  Float_t         lep_EffTrigTight_0;
  Float_t         lep_SFIsoLoose_0;
  Float_t         lep_SFIsoTight_0;
  Float_t         lep_SFReco_0;
  Float_t         lep_SFTTVA_0;
  Float_t         lep_SFObjLoose_0;
  Float_t         lep_SFObjTight_0;
  Float_t         lep_chargeIDBDTLoose_0;
  Float_t         lep_chargeIDBDTMedium_0;
  Float_t         lep_chargeIDBDTTight_0;
  Float_t         lep_promptLeptonIso_TagWeight_0;
  Float_t         lep_promptLeptonVeto_TagWeight_0;
  Int_t           lep_nTrackParticles_0;
  Float_t         lep_RadiusCO_0;
  Float_t         lep_SeparationMinDCT_0;
  Float_t         lep_ClosestSiTracknIL_0;
  Float_t         lep_ClosestSiTrackeIL_0;
  Float_t         lep_ClosestSiTracknNIL_0;
  Float_t         lep_ClosestSiTrackeNIL_0;
  Float_t         lep_BestMatchTrackPt_0;
  Float_t         lep_BestMatchTrackEta_0;
  Float_t         lep_BestMatchTrackPhi_0;
  Float_t         lep_BestMatchTrackD0_0;
  Float_t         lep_BestMatchTrackZ0_0;
  Float_t         lep_BestMatchTrackQ_0;
  Float_t         lep_ClosestSiTrackPt_0;
  Float_t         lep_ClosestSiTrackEta_0;
  Float_t         lep_ClosestSiTrackPhi_0;
  Float_t         lep_ClosestSiTrackD0_0;
  Float_t         lep_ClosestSiTrackZ0_0;
  Float_t         lep_ClosestSiTrackQ_0;
  Float_t         lep_Mtrktrk_atConvV_CO_0;
  Float_t         lep_Mtrktrk_atPV_CO_0;
  Float_t         lep_RadiusCOX_0;
  Float_t         lep_RadiusCOY_0;
  Float_t         lep_ID_1;
  Int_t           lep_Index_1;
  UChar_t         lep_ambiguityType_1;
  Float_t         lep_Pt_1;
  Float_t         lep_E_1;
  Float_t         lep_Eta_1;
  Float_t         lep_Phi_1;
  Float_t         lep_EtaBE2_1;
  Float_t         lep_topoEtcone20_1;
  Float_t         lep_topoEtcone30_1;
  Float_t         lep_topoEtcone40_1;
  Float_t         lep_ptVarcone20_1;
  Float_t         lep_ptVarcone30_1;
  Float_t         lep_ptVarcone40_1;
  Float_t         lep_sigd0PV_1;
  Float_t         lep_Z0SinTheta_1;
  Float_t         lep_d0_1;
  Float_t         lep_z0_1;
  Float_t         lep_vz_1;
  Float_t         lep_deltaz0_1;
  Char_t          lep_isTightLH_1;
  Char_t          lep_isMediumLH_1;
  Char_t          lep_isLooseLH_1;
  Char_t          lep_isTight_1;
  Char_t          lep_isMedium_1;
  Char_t          lep_isLoose_1;
  Int_t           lep_isolationLooseTrackOnly_1;
  Int_t           lep_isolationLoose_1;
  Int_t           lep_isolationGradient_1;
  Int_t           lep_isolationGradientLoose_1;
  Int_t           lep_isolationFixedCutTight_1;
  Int_t           lep_isolationFixedCutTightTrackOnly_1;
  Int_t           lep_isolationFixedCutLoose_1;
  Char_t          lep_isTrigMatch_1;
  Char_t          lep_isTrigMatchDLT_1;
  Char_t          lep_isPrompt_1;
  Char_t          lep_isFakeLep_1;
  Char_t          lep_isQMisID_1;
  Char_t          lep_isConvPh_1;
  Char_t          lep_isIntConvPh_1;
  Char_t          lep_isExtConvPh_1;
  Char_t          lep_isISR_FSR_Ph_1;
  Char_t          lep_isBrems_1;
  Char_t          lep_isTruthMatched_1;
  Int_t           lep_truthType_1;
  Int_t           lep_truthOrigin_1;
  Int_t           lep_truthPdgId_1;
  Int_t           lep_truthStatus_1;
  Int_t           lep_truthParentType_1;
  Int_t           lep_truthParentOrigin_1;
  Int_t           lep_truthParentPdgId_1;
  Int_t           lep_truthParentStatus_1;
  Float_t         lep_truthPt_1;
  Float_t         lep_truthEta_1;
  Float_t         lep_truthPhi_1;
  Float_t         lep_truthM_1;
  Float_t         lep_truthE_1;
  Float_t         lep_truthRapidity_1;
  Float_t         lep_SFIDLoose_1;
  Float_t         lep_SFIDTight_1;
  Float_t         lep_SFTrigLoose_1;
  Float_t         lep_SFTrigTight_1;
  Float_t         lep_EffTrigLoose_1;
  Float_t         lep_EffTrigTight_1;
  Float_t         lep_SFIsoLoose_1;
  Float_t         lep_SFIsoTight_1;
  Float_t         lep_SFReco_1;
  Float_t         lep_SFTTVA_1;
  Float_t         lep_SFObjLoose_1;
  Float_t         lep_SFObjTight_1;
  Float_t         lep_chargeIDBDTLoose_1;
  Float_t         lep_chargeIDBDTMedium_1;
  Float_t         lep_chargeIDBDTTight_1;
  Float_t         lep_promptLeptonIso_TagWeight_1;
  Float_t         lep_promptLeptonVeto_TagWeight_1;
  Int_t           lep_nTrackParticles_1;
  Float_t         lep_RadiusCO_1;
  Float_t         lep_SeparationMinDCT_1;
  Float_t         lep_ClosestSiTracknIL_1;
  Float_t         lep_ClosestSiTrackeIL_1;
  Float_t         lep_ClosestSiTracknNIL_1;
  Float_t         lep_ClosestSiTrackeNIL_1;
  Float_t         lep_BestMatchTrackPt_1;
  Float_t         lep_BestMatchTrackEta_1;
  Float_t         lep_BestMatchTrackPhi_1;
  Float_t         lep_BestMatchTrackD0_1;
  Float_t         lep_BestMatchTrackZ0_1;
  Float_t         lep_BestMatchTrackQ_1;
  Float_t         lep_ClosestSiTrackPt_1;
  Float_t         lep_ClosestSiTrackEta_1;
  Float_t         lep_ClosestSiTrackPhi_1;
  Float_t         lep_ClosestSiTrackD0_1;
  Float_t         lep_ClosestSiTrackZ0_1;
  Float_t         lep_ClosestSiTrackQ_1;
  Float_t         lep_Mtrktrk_atConvV_CO_1;
  Float_t         lep_Mtrktrk_atPV_CO_1;
  Float_t         lep_RadiusCOX_1;
  Float_t         lep_RadiusCOY_1;
  Float_t         lep_ID_2;
  Int_t           lep_Index_2;
  UChar_t         lep_ambiguityType_2;
  Float_t         lep_Pt_2;
  Float_t         lep_E_2;
  Float_t         lep_Eta_2;
  Float_t         lep_Phi_2;
  Float_t         lep_EtaBE2_2;
  Float_t         lep_topoEtcone20_2;
  Float_t         lep_topoEtcone30_2;
  Float_t         lep_topoEtcone40_2;
  Float_t         lep_ptVarcone20_2;
  Float_t         lep_ptVarcone30_2;
  Float_t         lep_ptVarcone40_2;
  Float_t         lep_sigd0PV_2;
  Float_t         lep_Z0SinTheta_2;
  Float_t         lep_d0_2;
  Float_t         lep_z0_2;
  Float_t         lep_vz_2;
  Float_t         lep_deltaz0_2;
  Char_t          lep_isTightLH_2;
  Char_t          lep_isMediumLH_2;
  Char_t          lep_isLooseLH_2;
  Char_t          lep_isTight_2;
  Char_t          lep_isMedium_2;
  Char_t          lep_isLoose_2;
  Int_t           lep_isolationLooseTrackOnly_2;
  Int_t           lep_isolationLoose_2;
  Int_t           lep_isolationGradient_2;
  Int_t           lep_isolationGradientLoose_2;
  Int_t           lep_isolationFixedCutTight_2;
  Int_t           lep_isolationFixedCutTightTrackOnly_2;
  Int_t           lep_isolationFixedCutLoose_2;
  Char_t          lep_isTrigMatch_2;
  Char_t          lep_isTrigMatchDLT_2;
  Char_t          lep_isPrompt_2;
  Char_t          lep_isFakeLep_2;
  Char_t          lep_isQMisID_2;
  Char_t          lep_isConvPh_2;
  Char_t          lep_isIntConvPh_2;
  Char_t          lep_isExtConvPh_2;
  Char_t          lep_isISR_FSR_Ph_2;
  Char_t          lep_isBrems_2;
  Char_t          lep_isTruthMatched_2;
  Int_t           lep_truthType_2;
  Int_t           lep_truthOrigin_2;
  Int_t           lep_truthPdgId_2;
  Int_t           lep_truthStatus_2;
  Int_t           lep_truthParentType_2;
  Int_t           lep_truthParentOrigin_2;
  Int_t           lep_truthParentPdgId_2;
  Int_t           lep_truthParentStatus_2;
  Float_t         lep_truthPt_2;
  Float_t         lep_truthEta_2;
  Float_t         lep_truthPhi_2;
  Float_t         lep_truthM_2;
  Float_t         lep_truthE_2;
  Float_t         lep_truthRapidity_2;
  Float_t         lep_SFIDLoose_2;
  Float_t         lep_SFIDTight_2;
  Float_t         lep_SFTrigLoose_2;
  Float_t         lep_SFTrigTight_2;
  Float_t         lep_EffTrigLoose_2;
  Float_t         lep_EffTrigTight_2;
  Float_t         lep_SFIsoLoose_2;
  Float_t         lep_SFIsoTight_2;
  Float_t         lep_SFReco_2;
  Float_t         lep_SFTTVA_2;
  Float_t         lep_SFObjLoose_2;
  Float_t         lep_SFObjTight_2;
  Float_t         lep_chargeIDBDTLoose_2;
  Float_t         lep_chargeIDBDTMedium_2;
  Float_t         lep_chargeIDBDTTight_2;
  Float_t         lep_promptLeptonIso_TagWeight_2;
  Float_t         lep_promptLeptonVeto_TagWeight_2;
  Int_t           lep_nTrackParticles_2;
  Float_t         lep_RadiusCO_2;
  Float_t         lep_SeparationMinDCT_2;
  Float_t         lep_ClosestSiTracknIL_2;
  Float_t         lep_ClosestSiTrackeIL_2;
  Float_t         lep_ClosestSiTracknNIL_2;
  Float_t         lep_ClosestSiTrackeNIL_2;
  Float_t         lep_BestMatchTrackPt_2;
  Float_t         lep_BestMatchTrackEta_2;
  Float_t         lep_BestMatchTrackPhi_2;
  Float_t         lep_BestMatchTrackD0_2;
  Float_t         lep_BestMatchTrackZ0_2;
  Float_t         lep_BestMatchTrackQ_2;
  Float_t         lep_ClosestSiTrackPt_2;
  Float_t         lep_ClosestSiTrackEta_2;
  Float_t         lep_ClosestSiTrackPhi_2;
  Float_t         lep_ClosestSiTrackD0_2;
  Float_t         lep_ClosestSiTrackZ0_2;
  Float_t         lep_ClosestSiTrackQ_2;
  Float_t         lep_Mtrktrk_atConvV_CO_2;
  Float_t         lep_Mtrktrk_atPV_CO_2;
  Float_t         lep_RadiusCOX_2;
  Float_t         lep_RadiusCOY_2;
  Float_t         lep_ID_3;
  Int_t           lep_Index_3;
  UChar_t         lep_ambiguityType_3;
  Float_t         lep_Pt_3;
  Float_t         lep_E_3;
  Float_t         lep_Eta_3;
  Float_t         lep_Phi_3;
  Float_t         lep_EtaBE2_3;
  Float_t         lep_topoEtcone20_3;
  Float_t         lep_topoEtcone30_3;
  Float_t         lep_topoEtcone40_3;
  Float_t         lep_ptVarcone20_3;
  Float_t         lep_ptVarcone30_3;
  Float_t         lep_ptVarcone40_3;
  Float_t         lep_sigd0PV_3;
  Float_t         lep_Z0SinTheta_3;
  Float_t         lep_d0_3;
  Float_t         lep_z0_3;
  Float_t         lep_vz_3;
  Float_t         lep_deltaz0_3;
  Char_t          lep_isTightLH_3;
  Char_t          lep_isMediumLH_3;
  Char_t          lep_isLooseLH_3;
  Char_t          lep_isTight_3;
  Char_t          lep_isMedium_3;
  Char_t          lep_isLoose_3;
  Int_t           lep_isolationLooseTrackOnly_3;
  Int_t           lep_isolationLoose_3;
  Int_t           lep_isolationGradient_3;
  Int_t           lep_isolationGradientLoose_3;
  Int_t           lep_isolationFixedCutTight_3;
  Int_t           lep_isolationFixedCutTightTrackOnly_3;
  Int_t           lep_isolationFixedCutLoose_3;
  Char_t          lep_isTrigMatch_3;
  Char_t          lep_isTrigMatchDLT_3;
  Char_t          lep_isPrompt_3;
  Char_t          lep_isFakeLep_3;
  Char_t          lep_isQMisID_3;
  Char_t          lep_isConvPh_3;
  Char_t          lep_isIntConvPh_3;
  Char_t          lep_isExtConvPh_3;
  Char_t          lep_isISR_FSR_Ph_3;
  Char_t          lep_isBrems_3;
  Char_t          lep_isTruthMatched_3;
  Int_t           lep_truthType_3;
  Int_t           lep_truthOrigin_3;
  Int_t           lep_truthPdgId_3;
  Int_t           lep_truthStatus_3;
  Int_t           lep_truthParentType_3;
  Int_t           lep_truthParentOrigin_3;
  Int_t           lep_truthParentPdgId_3;
  Int_t           lep_truthParentStatus_3;
  Float_t         lep_truthPt_3;
  Float_t         lep_truthEta_3;
  Float_t         lep_truthPhi_3;
  Float_t         lep_truthM_3;
  Float_t         lep_truthE_3;
  Float_t         lep_truthRapidity_3;
  Float_t         lep_SFIDLoose_3;
  Float_t         lep_SFIDTight_3;
  Float_t         lep_SFTrigLoose_3;
  Float_t         lep_SFTrigTight_3;
  Float_t         lep_EffTrigLoose_3;
  Float_t         lep_EffTrigTight_3;
  Float_t         lep_SFIsoLoose_3;
  Float_t         lep_SFIsoTight_3;
  Float_t         lep_SFReco_3;
  Float_t         lep_SFTTVA_3;
  Float_t         lep_SFObjLoose_3;
  Float_t         lep_SFObjTight_3;
  Float_t         lep_chargeIDBDTLoose_3;
  Float_t         lep_chargeIDBDTMedium_3;
  Float_t         lep_chargeIDBDTTight_3;
  Float_t         lep_promptLeptonIso_TagWeight_3;
  Float_t         lep_promptLeptonVeto_TagWeight_3;
  Int_t           lep_nTrackParticles_3;
  Float_t         lep_RadiusCO_3;
  Float_t         lep_SeparationMinDCT_3;
  Float_t         lep_ClosestSiTracknIL_3;
  Float_t         lep_ClosestSiTrackeIL_3;
  Float_t         lep_ClosestSiTracknNIL_3;
  Float_t         lep_ClosestSiTrackeNIL_3;
  Float_t         lep_BestMatchTrackPt_3;
  Float_t         lep_BestMatchTrackEta_3;
  Float_t         lep_BestMatchTrackPhi_3;
  Float_t         lep_BestMatchTrackD0_3;
  Float_t         lep_BestMatchTrackZ0_3;
  Float_t         lep_BestMatchTrackQ_3;
  Float_t         lep_ClosestSiTrackPt_3;
  Float_t         lep_ClosestSiTrackEta_3;
  Float_t         lep_ClosestSiTrackPhi_3;
  Float_t         lep_ClosestSiTrackD0_3;
  Float_t         lep_ClosestSiTrackZ0_3;
  Float_t         lep_ClosestSiTrackQ_3;
  Float_t         lep_Mtrktrk_atConvV_CO_3;
  Float_t         lep_Mtrktrk_atPV_CO_3;
  Float_t         lep_RadiusCOX_3;
  Float_t         lep_RadiusCOY_3;
  vector<unsigned char> *electron_ambiguityType;
  vector<float>   *electron_PromptLeptonIso_TagWeight;
  vector<float>   *electron_PromptLeptonVeto_TagWeight;
  vector<float>   *muon_PromptLeptonIso_TagWeight;
  vector<float>   *muon_PromptLeptonVeto_TagWeight;
  Float_t         tau_MV2c10_0;
  Float_t         tau_MV2c10_1;
  Float_t         tau_pt_0;
  Float_t         tau_eta_0;
  Float_t         tau_phi_0;
  Float_t         tau_charge_0;
  Float_t         tau_E_0;
  Double_t        tau_BDTJetScore_0;
  Int_t           tau_JetBDTSigLoose_0;
  Int_t           tau_JetBDTSigMedium_0;
  Int_t           tau_JetBDTSigTight_0;
  Int_t           tau_isHadronic_0;
  Int_t           tau_numTrack_0;
  Float_t         tau_SFTight_0;
  Float_t         tau_SFLoose_0;
  Int_t           tau_tagWeightBin_0;
  Char_t          tau_fromPV_0;
  Int_t           tau_passEleOLR_0;
  Int_t           tau_passEleBDT_0;
  Int_t           tau_passMuonOLR_0;
  Int_t           tau_truthOrigin_0;
  Int_t           tau_truthType_0;
  Int_t           tau_truthJetFlavour_0;
  Float_t         tau_pt_1;
  Float_t         tau_eta_1;
  Float_t         tau_phi_1;
  Float_t         tau_charge_1;
  Float_t         tau_E_1;
  Double_t        tau_BDTJetScore_1;
  Int_t           tau_JetBDTSigLoose_1;
  Int_t           tau_JetBDTSigMedium_1;
  Int_t           tau_JetBDTSigTight_1;
  Int_t           tau_numTrack_1;
  Int_t           tau_isHadronic_1;
  Float_t         tau_SFTight_1;
  Float_t         tau_SFLoose_1;
  Int_t           tau_tagWeightBin_1;
  Char_t          tau_fromPV_1;
  Int_t           tau_passEleOLR_1;
  Int_t           tau_passEleBDT_1;
  Int_t           tau_passMuonOLR_1;
  Int_t           tau_truthOrigin_1;
  Int_t           tau_truthType_1;
  Int_t           tau_truthJetFlavour_1;
  Float_t         tau_promptTauVeto_0;
  Float_t         tau_promptTauVeto_1;
  Int_t           onelep_type;
  Int_t           dilep_type;
  Int_t           trilep_type;
  Int_t           quadlep_type;
  Int_t           total_charge;
  Int_t           total_leptons;
  Char_t          isQMisIDEvent;
  Char_t          isFakeEvent;
  Char_t          isLepFromPhEvent;
  Float_t         Mll01;
  Float_t         Ptll01;
  Float_t         DRll01;
  Float_t         Mlll012;
  Float_t         Mllll0123;
  Float_t         Mlll013;
  Float_t         Mll02;
  Float_t         Ptll02;
  Float_t         DRll02;
  Float_t         Mlll023;
  Float_t         Mll03;
  Float_t         Ptll03;
  Float_t         DRll03;
  Float_t         Mll12;
  Float_t         Ptll12;
  Float_t         DRll12;
  Float_t         Mlll123;
  Float_t         Mll13;
  Float_t         Ptll13;
  Float_t         DRll13;
  Float_t         Mll23;
  Float_t         Ptll23;
  Float_t         DRll23;
  Char_t          matchDLTll01;
  Char_t          matchDLTll02;
  Char_t          matchDLTll12;
  Char_t          matchDLTll03;
  Char_t          matchDLTll13;
  Char_t          matchDLTll23;
  Float_t         best_Z_Mll;
  Float_t         best_Z_other_Mll;
  Float_t         best_Z_other_MtLepMet;
  Int_t           nJets_OR_T;
  Int_t           nJets_OR;
  Int_t           nJets_OR_T_MV2c20_60;
  Int_t           nJets_OR_T_MV2c20_70;
  Int_t           nJets_OR_T_MV2c20_77;
  Int_t           nJets_OR_T_MV2c20_85;
  Int_t           nJets_OR_MV2c20_85;
  Int_t           nJets_OR_MV2c20_60;
  Int_t           nJets_OR_MV2c20_77;
  Int_t           nJets_OR_MV2c20_70;
  Int_t           nJets_OR_T_MV2c10_60;
  Int_t           nJets_OR_T_MV2c10_70;
  Int_t           nJets_OR_T_MV2c10_77;
  Int_t           nJets_OR_T_MV2c10_85;
  Int_t           nJets_OR_MV2c10_85;
  Int_t           nJets_OR_MV2c10_60;
  Int_t           nJets_OR_MV2c10_77;
  Int_t           nJets_OR_MV2c10_70;
  Int_t           nTaus_OR_Pt25;
  Bool_t          isBlinded;
  Float_t         HT;
  Float_t         HT_lep;
  Float_t         HT_jets;
  Float_t         lead_jetPt;
  Float_t         lead_jetEta;
  Float_t         lead_jetPhi;
  Float_t         lead_jetE;
  Float_t         sublead_jetPt;
  Float_t         sublead_jetEta;
  Float_t         sublead_jetPhi;
  Float_t         sublead_jetE;
  Float_t         lepSFTrigLoose;
  Float_t         lepSFTrigTight;
  Float_t         lepSFTrigTightLoose;
  Float_t         lepSFTrigLooseTight;
  Float_t         lepEffTrigLoose;
  Float_t         lepEffTrigTight;
  Float_t         lepEffTrigTightLoose;
  Float_t         lepEffTrigLooseTight;
  Float_t         lepDataEffTrigLoose;
  Float_t         lepDataEffTrigTight;
  Float_t         lepDataEffTrigTightLoose;
  Float_t         lepDataEffTrigLooseTight;
  Float_t         lepSFObjLoose;
  Float_t         lepSFObjTight;
  Float_t         tauSFTight;
  Float_t         tauSFLoose;
  Char_t          isMMSideband_TT;
  Char_t          isMMSideband_TAntiT;
  Char_t          isMMSideband_AntiTT;
  Char_t          isMMSideband_AntiTAntiT;
  Float_t         MM_EventWeight;
  Float_t         MM_EventWeight_Fake_El_NBJets_VS_Pt_D_QMisID_10_Up;
  Float_t         MM_EventWeight_Fake_El_NBJets_VS_Pt_D_QMisID_10_Dn;
  Float_t         MM_EventWeight_Fake_El_NBJets_VS_Pt_D_QMisID_5_Up;
  Float_t         MM_EventWeight_Fake_El_NBJets_VS_Pt_D_QMisID_5_Dn;
  Float_t         MM_EventWeight_Fake_El_NBJets_VS_Pt_D_QMisID_6_Up;
  Float_t         MM_EventWeight_Fake_El_NBJets_VS_Pt_D_QMisID_6_Dn;
  Float_t         MM_EventWeight_Fake_El_NBJets_VS_Pt_D_QMisID_9_Up;
  Float_t         MM_EventWeight_Fake_El_NBJets_VS_Pt_D_QMisID_9_Dn;
  Float_t         MM_EventWeight_Fake_El_NBJets_VS_Pt_ND_OtherPromptSS_Up;
  Float_t         MM_EventWeight_Fake_El_NBJets_VS_Pt_ND_OtherPromptSS_Dn;
  Float_t         MM_EventWeight_Fake_El_NBJets_VS_Pt_ND_TTV_Up;
  Float_t         MM_EventWeight_Fake_El_NBJets_VS_Pt_ND_TTV_Dn;
  Float_t         MM_EventWeight_Fake_El_NBJets_VS_Pt_ND_VV_Up;
  Float_t         MM_EventWeight_Fake_El_NBJets_VS_Pt_ND_VV_Dn;
  Float_t         MM_EventWeight_Fake_El_NBJets_VS_Pt_N_QMisID_10_Up;
  Float_t         MM_EventWeight_Fake_El_NBJets_VS_Pt_N_QMisID_10_Dn;
  Float_t         MM_EventWeight_Fake_El_NBJets_VS_Pt_N_QMisID_5_Up;
  Float_t         MM_EventWeight_Fake_El_NBJets_VS_Pt_N_QMisID_5_Dn;
  Float_t         MM_EventWeight_Fake_El_NBJets_VS_Pt_N_QMisID_6_Up;
  Float_t         MM_EventWeight_Fake_El_NBJets_VS_Pt_N_QMisID_6_Dn;
  Float_t         MM_EventWeight_Fake_El_NBJets_VS_Pt_N_QMisID_9_Up;
  Float_t         MM_EventWeight_Fake_El_NBJets_VS_Pt_N_QMisID_9_Dn;
  Float_t         MM_EventWeight_Fake_El_NBJets_VS_Pt_Stat_10_Up;
  Float_t         MM_EventWeight_Fake_El_NBJets_VS_Pt_Stat_10_Dn;
  Float_t         MM_EventWeight_Fake_El_NBJets_VS_Pt_Stat_5_Up;
  Float_t         MM_EventWeight_Fake_El_NBJets_VS_Pt_Stat_5_Dn;
  Float_t         MM_EventWeight_Fake_El_NBJets_VS_Pt_Stat_6_Up;
  Float_t         MM_EventWeight_Fake_El_NBJets_VS_Pt_Stat_6_Dn;
  Float_t         MM_EventWeight_Fake_El_NBJets_VS_Pt_Stat_9_Up;
  Float_t         MM_EventWeight_Fake_El_NBJets_VS_Pt_Stat_9_Dn;
  Float_t         MM_EventWeight_Fake_Mu_DistanceClosestJet_VS_Pt_ND_OtherPromptSS_Up;
  Float_t         MM_EventWeight_Fake_Mu_DistanceClosestJet_VS_Pt_ND_OtherPromptSS_Dn;
  Float_t         MM_EventWeight_Fake_Mu_DistanceClosestJet_VS_Pt_ND_TTV_Up;
  Float_t         MM_EventWeight_Fake_Mu_DistanceClosestJet_VS_Pt_ND_TTV_Dn;
  Float_t         MM_EventWeight_Fake_Mu_DistanceClosestJet_VS_Pt_ND_VV_Up;
  Float_t         MM_EventWeight_Fake_Mu_DistanceClosestJet_VS_Pt_ND_VV_Dn;
  Float_t         MM_EventWeight_Fake_Mu_DistanceClosestJet_VS_Pt_Stat_10_Up;
  Float_t         MM_EventWeight_Fake_Mu_DistanceClosestJet_VS_Pt_Stat_10_Dn;
  Float_t         MM_EventWeight_Fake_Mu_DistanceClosestJet_VS_Pt_Stat_13_Up;
  Float_t         MM_EventWeight_Fake_Mu_DistanceClosestJet_VS_Pt_Stat_13_Dn;
  Float_t         MM_EventWeight_Fake_Mu_DistanceClosestJet_VS_Pt_Stat_14_Up;
  Float_t         MM_EventWeight_Fake_Mu_DistanceClosestJet_VS_Pt_Stat_14_Dn;
  Float_t         MM_EventWeight_Fake_Mu_DistanceClosestJet_VS_Pt_Stat_5_Up;
  Float_t         MM_EventWeight_Fake_Mu_DistanceClosestJet_VS_Pt_Stat_5_Dn;
  Float_t         MM_EventWeight_Fake_Mu_DistanceClosestJet_VS_Pt_Stat_6_Up;
  Float_t         MM_EventWeight_Fake_Mu_DistanceClosestJet_VS_Pt_Stat_6_Dn;
  Float_t         MM_EventWeight_Fake_Mu_DistanceClosestJet_VS_Pt_Stat_9_Up;
  Float_t         MM_EventWeight_Fake_Mu_DistanceClosestJet_VS_Pt_Stat_9_Dn;
  Float_t         MM_EventWeight_Real_El_Eta_VS_Pt_ND_FakesOS_Up;
  Float_t         MM_EventWeight_Real_El_Eta_VS_Pt_ND_FakesOS_Dn;
  Float_t         MM_EventWeight_Real_El_Eta_VS_Pt_Stat_10_Up;
  Float_t         MM_EventWeight_Real_El_Eta_VS_Pt_Stat_10_Dn;
  Float_t         MM_EventWeight_Real_El_Eta_VS_Pt_Stat_11_Up;
  Float_t         MM_EventWeight_Real_El_Eta_VS_Pt_Stat_11_Dn;
  Float_t         MM_EventWeight_Real_El_Eta_VS_Pt_Stat_12_Up;
  Float_t         MM_EventWeight_Real_El_Eta_VS_Pt_Stat_12_Dn;
  Float_t         MM_EventWeight_Real_El_Eta_VS_Pt_Stat_13_Up;
  Float_t         MM_EventWeight_Real_El_Eta_VS_Pt_Stat_13_Dn;
  Float_t         MM_EventWeight_Real_El_Eta_VS_Pt_Stat_14_Up;
  Float_t         MM_EventWeight_Real_El_Eta_VS_Pt_Stat_14_Dn;
  Float_t         MM_EventWeight_Real_El_Eta_VS_Pt_Stat_17_Up;
  Float_t         MM_EventWeight_Real_El_Eta_VS_Pt_Stat_17_Dn;
  Float_t         MM_EventWeight_Real_El_Eta_VS_Pt_Stat_18_Up;
  Float_t         MM_EventWeight_Real_El_Eta_VS_Pt_Stat_18_Dn;
  Float_t         MM_EventWeight_Real_El_Eta_VS_Pt_Stat_19_Up;
  Float_t         MM_EventWeight_Real_El_Eta_VS_Pt_Stat_19_Dn;
  Float_t         MM_EventWeight_Real_El_Eta_VS_Pt_Stat_20_Up;
  Float_t         MM_EventWeight_Real_El_Eta_VS_Pt_Stat_20_Dn;
  Float_t         MM_EventWeight_Real_El_Eta_VS_Pt_Stat_21_Up;
  Float_t         MM_EventWeight_Real_El_Eta_VS_Pt_Stat_21_Dn;
  Float_t         MM_EventWeight_Real_El_Eta_VS_Pt_Stat_22_Up;
  Float_t         MM_EventWeight_Real_El_Eta_VS_Pt_Stat_22_Dn;
  Float_t         MM_EventWeight_Real_El_Eta_VS_Pt_Stat_25_Up;
  Float_t         MM_EventWeight_Real_El_Eta_VS_Pt_Stat_25_Dn;
  Float_t         MM_EventWeight_Real_El_Eta_VS_Pt_Stat_26_Up;
  Float_t         MM_EventWeight_Real_El_Eta_VS_Pt_Stat_26_Dn;
  Float_t         MM_EventWeight_Real_El_Eta_VS_Pt_Stat_27_Up;
  Float_t         MM_EventWeight_Real_El_Eta_VS_Pt_Stat_27_Dn;
  Float_t         MM_EventWeight_Real_El_Eta_VS_Pt_Stat_28_Up;
  Float_t         MM_EventWeight_Real_El_Eta_VS_Pt_Stat_28_Dn;
  Float_t         MM_EventWeight_Real_El_Eta_VS_Pt_Stat_29_Up;
  Float_t         MM_EventWeight_Real_El_Eta_VS_Pt_Stat_29_Dn;
  Float_t         MM_EventWeight_Real_El_Eta_VS_Pt_Stat_30_Up;
  Float_t         MM_EventWeight_Real_El_Eta_VS_Pt_Stat_30_Dn;
  Float_t         MM_EventWeight_Real_El_Eta_VS_Pt_Stat_33_Up;
  Float_t         MM_EventWeight_Real_El_Eta_VS_Pt_Stat_33_Dn;
  Float_t         MM_EventWeight_Real_El_Eta_VS_Pt_Stat_34_Up;
  Float_t         MM_EventWeight_Real_El_Eta_VS_Pt_Stat_34_Dn;
  Float_t         MM_EventWeight_Real_El_Eta_VS_Pt_Stat_35_Up;
  Float_t         MM_EventWeight_Real_El_Eta_VS_Pt_Stat_35_Dn;
  Float_t         MM_EventWeight_Real_El_Eta_VS_Pt_Stat_36_Up;
  Float_t         MM_EventWeight_Real_El_Eta_VS_Pt_Stat_36_Dn;
  Float_t         MM_EventWeight_Real_El_Eta_VS_Pt_Stat_37_Up;
  Float_t         MM_EventWeight_Real_El_Eta_VS_Pt_Stat_37_Dn;
  Float_t         MM_EventWeight_Real_El_Eta_VS_Pt_Stat_38_Up;
  Float_t         MM_EventWeight_Real_El_Eta_VS_Pt_Stat_38_Dn;
  Float_t         MM_EventWeight_Real_El_Eta_VS_Pt_Stat_41_Up;
  Float_t         MM_EventWeight_Real_El_Eta_VS_Pt_Stat_41_Dn;
  Float_t         MM_EventWeight_Real_El_Eta_VS_Pt_Stat_42_Up;
  Float_t         MM_EventWeight_Real_El_Eta_VS_Pt_Stat_42_Dn;
  Float_t         MM_EventWeight_Real_El_Eta_VS_Pt_Stat_43_Up;
  Float_t         MM_EventWeight_Real_El_Eta_VS_Pt_Stat_43_Dn;
  Float_t         MM_EventWeight_Real_El_Eta_VS_Pt_Stat_44_Up;
  Float_t         MM_EventWeight_Real_El_Eta_VS_Pt_Stat_44_Dn;
  Float_t         MM_EventWeight_Real_El_Eta_VS_Pt_Stat_45_Up;
  Float_t         MM_EventWeight_Real_El_Eta_VS_Pt_Stat_45_Dn;
  Float_t         MM_EventWeight_Real_El_Eta_VS_Pt_Stat_46_Up;
  Float_t         MM_EventWeight_Real_El_Eta_VS_Pt_Stat_46_Dn;
  Float_t         MM_EventWeight_Real_El_Eta_VS_Pt_Stat_49_Up;
  Float_t         MM_EventWeight_Real_El_Eta_VS_Pt_Stat_49_Dn;
  Float_t         MM_EventWeight_Real_El_Eta_VS_Pt_Stat_50_Up;
  Float_t         MM_EventWeight_Real_El_Eta_VS_Pt_Stat_50_Dn;
  Float_t         MM_EventWeight_Real_El_Eta_VS_Pt_Stat_51_Up;
  Float_t         MM_EventWeight_Real_El_Eta_VS_Pt_Stat_51_Dn;
  Float_t         MM_EventWeight_Real_El_Eta_VS_Pt_Stat_52_Up;
  Float_t         MM_EventWeight_Real_El_Eta_VS_Pt_Stat_52_Dn;
  Float_t         MM_EventWeight_Real_El_Eta_VS_Pt_Stat_53_Up;
  Float_t         MM_EventWeight_Real_El_Eta_VS_Pt_Stat_53_Dn;
  Float_t         MM_EventWeight_Real_El_Eta_VS_Pt_Stat_54_Up;
  Float_t         MM_EventWeight_Real_El_Eta_VS_Pt_Stat_54_Dn;
  Float_t         MM_EventWeight_Real_El_Eta_VS_Pt_Stat_57_Up;
  Float_t         MM_EventWeight_Real_El_Eta_VS_Pt_Stat_57_Dn;
  Float_t         MM_EventWeight_Real_El_Eta_VS_Pt_Stat_58_Up;
  Float_t         MM_EventWeight_Real_El_Eta_VS_Pt_Stat_58_Dn;
  Float_t         MM_EventWeight_Real_El_Eta_VS_Pt_Stat_59_Up;
  Float_t         MM_EventWeight_Real_El_Eta_VS_Pt_Stat_59_Dn;
  Float_t         MM_EventWeight_Real_El_Eta_VS_Pt_Stat_60_Up;
  Float_t         MM_EventWeight_Real_El_Eta_VS_Pt_Stat_60_Dn;
  Float_t         MM_EventWeight_Real_El_Eta_VS_Pt_Stat_61_Up;
  Float_t         MM_EventWeight_Real_El_Eta_VS_Pt_Stat_61_Dn;
  Float_t         MM_EventWeight_Real_El_Eta_VS_Pt_Stat_62_Up;
  Float_t         MM_EventWeight_Real_El_Eta_VS_Pt_Stat_62_Dn;
  Float_t         MM_EventWeight_Real_El_Eta_VS_Pt_Stat_9_Up;
  Float_t         MM_EventWeight_Real_El_Eta_VS_Pt_Stat_9_Dn;
  Float_t         MM_EventWeight_Real_Mu_Pt_ND_FakesOS_Up;
  Float_t         MM_EventWeight_Real_Mu_Pt_ND_FakesOS_Dn;
  Float_t         MM_EventWeight_Real_Mu_Pt_Stat_1_Up;
  Float_t         MM_EventWeight_Real_Mu_Pt_Stat_1_Dn;
  Float_t         MM_EventWeight_Real_Mu_Pt_Stat_2_Up;
  Float_t         MM_EventWeight_Real_Mu_Pt_Stat_2_Dn;
  Float_t         MM_EventWeight_Real_Mu_Pt_Stat_3_Up;
  Float_t         MM_EventWeight_Real_Mu_Pt_Stat_3_Dn;
  Float_t         MM_EventWeight_Real_Mu_Pt_Stat_4_Up;
  Float_t         MM_EventWeight_Real_Mu_Pt_Stat_4_Dn;
  Float_t         MM_EventWeight_Real_Mu_Pt_Stat_5_Up;
  Float_t         MM_EventWeight_Real_Mu_Pt_Stat_5_Dn;
  Float_t         MM_EventWeight_Real_Mu_Pt_Stat_6_Up;
  Float_t         MM_EventWeight_Real_Mu_Pt_Stat_6_Dn;
  Float_t         MM_EventWeight_Real_Mu_Pt_Stat_7_Up;
  Float_t         MM_EventWeight_Real_Mu_Pt_Stat_7_Dn;
  Float_t         MM_EventWeight_Real_El_Pt_Stat_Grouped_Up;
  Float_t         MM_EventWeight_Real_El_Pt_Stat_Grouped_Dn;
  Float_t         MM_EventWeight_Real_El_Pt_Sys_Grouped_Up;
  Float_t         MM_EventWeight_Real_El_Pt_Sys_Grouped_Dn;
  Float_t         MM_EventWeight_Real_Mu_Pt_Stat_Grouped_Up;
  Float_t         MM_EventWeight_Real_Mu_Pt_Stat_Grouped_Dn;
  Float_t         MM_EventWeight_Real_Mu_Pt_Sys_Grouped_Up;
  Float_t         MM_EventWeight_Real_Mu_Pt_Sys_Grouped_Dn;
  Float_t         MM_EventWeight_Fake_El_Pt_Stat_Grouped_Up;
  Float_t         MM_EventWeight_Fake_El_Pt_Stat_Grouped_Dn;
  Float_t         MM_EventWeight_Fake_El_Pt_Sys_Grouped_Up;
  Float_t         MM_EventWeight_Fake_El_Pt_Sys_Grouped_Dn;
  Float_t         MM_EventWeight_Fake_Mu_Pt_Stat_Grouped_Up;
  Float_t         MM_EventWeight_Fake_Mu_Pt_Stat_Grouped_Dn;
  Float_t         MM_EventWeight_Fake_Mu_Pt_Sys_Grouped_Up;
  Float_t         MM_EventWeight_Fake_Mu_Pt_Sys_Grouped_Dn;
  Float_t         QMisID_MM_EventWeight;
  Float_t         QMisID_MM_EventWeight_Up;
  Float_t         QMisID_MM_EventWeight_Dn;
  Int_t           nTruthJets;
  Int_t           nJets;
  vector<float>   *jet_pt;
  vector<float>   *jet_phi;
  vector<float>   *jet_eta;
  vector<float>   *jet_e;
  vector<char>    *jet_passOR;
  vector<char>    *jet_passJVT;
  vector<char>    *jet_passTauOR;
  vector<float>   *jet_flavor_weight_MV2c10;
  vector<int>     *jet_flavor_truth_label;
  vector<int>     *jet_flavor_truth_label_ghost;
  vector<float>   *truth_jet_pt;
  vector<float>   *truth_jet_phi;
  vector<float>   *truth_jet_eta;
  vector<float>   *truth_jet_m;
  vector<int>     *truth_jet_Wcount;
  vector<int>     *truth_jet_Zcount;
  vector<int>     *truth_jet_Tcount;
  vector<int>     *truth_jet_Hcount;
  vector<int>     *truth_jet_BHadronCount;
  vector<int>     *truth_jet_CHadronCount;

  // List of branches
  TBranch        *b_is1L2Tau;   //!
  TBranch        *b_is2LSS0Tau;   //!
  TBranch        *b_pass_tight_2L;   //!
  TBranch        *b_pass_SS_2L;   //!
  TBranch        *b_is2LSS1Tau;   //!
  TBranch        *b_is2LOS1Tau;   //!
  TBranch        *b_is3L;   //!
  TBranch        *b_is3L1Tau;   //!
  TBranch        *b_is4L;   //!
  TBranch        *b_is2lLowNJet0Tau;   //!
  TBranch        *b_is2LSS0Tau_SR_Relaxed;   //!
  TBranch        *b_is2LSS1Tau_SR_Relaxed;   //!
  TBranch        *b_is3L0Tau_SR_Relaxed;   //!
  TBranch        *b_is3L0Tau_SR_Relaxed3j;   //!
  TBranch        *b_is4L_SR_VV_Relaxed;   //!
  TBranch        *b_is4L_SR_ttV_Relaxed;   //!
  TBranch        *b_is4L_SR_ttV_Zenriched;   //!
  TBranch        *b_is3L0Tau_CR_ttbar_Relaxed;   //!
  TBranch        *b_is3L0Tau_CR_VV_Relaxed;   //!
  TBranch        *b_is3L0Tau_CR_ttW_Relaxed;   //!
  TBranch        *b_is2L_VR_ttW_ICHEP;   //!
  TBranch        *b_is2L_VR_ttbarFakes_ICHEP;   //!
  TBranch        *b_is4lCRttZRelaxed;   //!
  TBranch        *b_is2LOS1Tau_CR_ttbar;   //!
  TBranch        *b_is2LOS1Tau_CR_ttZ;   //!
  TBranch        *b_is2lOS0TauBaseline;   //!
  TBranch        *b_is2lOS0TauBaseline_ee;   //!
  TBranch        *b_is2lOS0TauBaseline_mm;   //!
  TBranch        *b_is2lOS0TauBaseline_OF;   //!
  TBranch        *b_is2lOS0TauBaseline_me;   //!
  TBranch        *b_is2lOS0TauBaseline_em;   //!
  TBranch        *b_is2lSS0TauBaseline;   //!
  TBranch        *b_is2lSS0TauBaseline_ee;   //!
  TBranch        *b_is2lSS0TauBaseline_mm;   //!
  TBranch        *b_is2lSS0TauBaseline_OF;   //!
  TBranch        *b_is2lSS0TauBaseline_me;   //!
  TBranch        *b_is2lSS0TauBaseline_em;   //!
  TBranch        *b_is2lOS1TauBaseline;   //!
  TBranch        *b_is2lOS1TauBaseline_ee;   //!
  TBranch        *b_is2lOS1TauBaseline_mm;   //!
  TBranch        *b_is2lOS1TauBaseline_OF;   //!
  TBranch        *b_is2lOS1TauBaseline_me;   //!
  TBranch        *b_is2lOS1TauBaseline_em;   //!
  TBranch        *b_is2lSS1TauBaseline;   //!
  TBranch        *b_is2lSS1TauBaseline_ee;   //!
  TBranch        *b_is2lSS1TauBaseline_mm;   //!
  TBranch        *b_is2lSS1TauBaseline_OF;   //!
  TBranch        *b_is2lSS1TauBaseline_me;   //!
  TBranch        *b_is2lSS1TauBaseline_em;   //!
  TBranch        *b_is3lBaseline;   //!
  TBranch        *b_is3lBaselineNoZVeto;   //!
  TBranch        *b_isGNv02;   //!
  TBranch        *b_minOSMll;   //!
  TBranch        *b_minOSSFMll;   //!
  TBranch        *b_jet_flvWtOrdr_0_pt;   //!
  TBranch        *b_jet_flvWtOrdr_0_eta;   //!
  TBranch        *b_jet_flvWtOrdr_0_phi;   //!
  TBranch        *b_jet_flvWtOrdr_0_E;   //!
  TBranch        *b_jet_flvWtOrdr_0_mv2c10;   //!
  TBranch        *b_jet_flvWtOrdr_1_pt;   //!
  TBranch        *b_jet_flvWtOrdr_1_eta;   //!
  TBranch        *b_jet_flvWtOrdr_1_phi;   //!
  TBranch        *b_jet_flvWtOrdr_1_E;   //!
  TBranch        *b_jet_flvWtOrdr_1_mv2c10;   //!
  TBranch        *b_jet_flvWtOrdr_2_pt;   //!
  TBranch        *b_jet_flvWtOrdr_2_eta;   //!
  TBranch        *b_jet_flvWtOrdr_2_phi;   //!
  TBranch        *b_jet_flvWtOrdr_2_E;   //!
  TBranch        *b_jet_flvWtOrdr_2_mv2c10;   //!
  TBranch        *b_DEtall01;   //!
  TBranch        *b_DEtall02;   //!
  TBranch        *b_DEtall12;   //!
  TBranch        *b_lep_flavour;   //!
  TBranch        *b_jet_flavor;   //!
  TBranch        *b_total_charge_2lSS;   //!
  TBranch        *b_isExtConv;   //!
  TBranch        *b_isIntConv;   //!
  TBranch        *b_max_eta;   //!
  TBranch        *b_SDRll;   //!
  TBranch        *b_DeltaR_max_lep;   //!
  TBranch        *b_DPhi_max_lep_met;   //!
  TBranch        *b_DeltaR_min_lep_jet;   //!
  TBranch        *b_minDeltaR_LJ_0;   //!
  TBranch        *b_minDeltaR_LJ_1;   //!
  TBranch        *b_minDeltaR_LJ_2;   //!
  TBranch        *b_Meff;   //!
  TBranch        *b_Mjj_closeW;   //!
  TBranch        *b_DRlb_h;   //!
  TBranch        *b_ptlb_h;   //!
  TBranch        *b_DeltaRLeadjets;   //!
  TBranch        *b_jet1pTbin;   //!
  TBranch        *b_jet2pTbin;   //!
  TBranch        *b_jet3pTbin;   //!
  TBranch        *b_jet3etabin;   //!
  TBranch        *b_jet1MV2c10_bin;   //!
  TBranch        *b_jet2MV2c10_bin;   //!
  TBranch        *b_MvisH;   //!
  TBranch        *b_MvisnonH;   //!
  TBranch        *b_lep_PtVarCone20Rel_1;   //!
  TBranch        *b_DeltaRLeadLepClosestJet;   //!
  TBranch        *b_Ml2l3met;   //!
  TBranch        *b_FSF_4L_tot;   //!
  TBranch        *b_FSF_4L_tot_up;   //!
  TBranch        *b_FSF_4L_tot_down;   //!
  TBranch        *b_FSF_category;   //!
  TBranch        *b_jet2_pt;   //!
  TBranch        *b_jet2_eta;   //!
  TBranch        *b_jet2_phi;   //!
  TBranch        *b_jet3_pt;   //!
  TBranch        *b_jet3_eta;   //!
  TBranch        *b_jet3_phi;   //!
  TBranch        *b_jet0_tagWeightBin;   //!
  TBranch        *b_jet1_tagWeightBin;   //!
  TBranch        *b_jet2_tagWeightBin;   //!
  TBranch        *b_jet3_tagWeightBin;   //!
  TBranch        *b_N_el;   //!
  TBranch        *b_N_mu;   //!
  TBranch        *b_tempDPl0t;   //!
  TBranch        *b_tempDPl1t;   //!
  TBranch        *b_tempDP2l;   //!
  TBranch        *b_tempDPlt_closest;   //!
  TBranch        *b_tempDPlt_furthest;   //!
  TBranch        *b_DeltaRSubLepClosestJet;   //!
  TBranch        *b_SignProduct2sslt;   //!
  TBranch        *b_TransverseMassLeadLepMET;   //!
  TBranch        *b_LD_HTmiss_ETmiss;   //!
  TBranch        *b_Mjj_nonb;   //!
  TBranch        *b_Mtop;   //!
  TBranch        *b_MtopW;   //!
  TBranch        *b_temptopoetcone20overpTlep0;   //!
  TBranch        *b_temptopoetcone20overpTlep1;   //!
  TBranch        *b_tempptvarcone20overpTlep0;   //!
  TBranch        *b_tempptvarcone20overpTlep1;   //!
  TBranch        *b_tau_tight_0;   //!
  TBranch        *b_tau_tight_1;   //!
  TBranch        *b_tau_btag70_0;   //!
  TBranch        *b_tau_btag70_1;   //!
  TBranch        *b_tau_truth_0;   //!
  TBranch        *b_tau_truth_1;   //!
  TBranch        *b_htjets_1l2tau;   //!
  TBranch        *b_jjdrmin_1l2tau;   //!
  TBranch        *b_mtautau_1l2tau;   //!
  TBranch        *b_njets_1l2tau;   //!
  TBranch        *b_nbjets_1l2tau;   //!
  TBranch        *b_DRlj00;   //!
  TBranch        *b_DRjj01;   //!
  TBranch        *b_min_DRl0j;   //!
  TBranch        *b_min_DRl1j;   //!
  TBranch        *b_min_DRl2j;   //!
  TBranch        *b_min_DRl0b;   //!
  TBranch        *b_min_DRl1b;   //!
  TBranch        *b_min_DRl2b;   //!
  TBranch        *b_min_DRlj_new;   //!
  TBranch        *b_nJets_OR_T_overflow;   //!
  TBranch        *b_nJets_OR_T_MV2c10_70_overflow;   //!
  TBranch        *b_DPhij0MET;   //!
  TBranch        *b_lead_BjetPt;   //!
  TBranch        *b_lead_jet_sumBEff;   //!
  TBranch        *b_sublead_jet_sumBEff;   //!
  TBranch        *b_lead_jet_is_lead_antitag;   //!
  TBranch        *b_lead_jet_is_lead_tag;   //!
  TBranch        *b_lep_Pt_1_new;   //!
  TBranch        *b_lep_Pt_2_new;   //!
  TBranch        *b_lep_ID_1_new;   //!
  TBranch        *b_lep_ID_2_new;   //!
  TBranch        *b_EventWeight;   //!
  TBranch        *b_lep_isTightSelected_0;   //!
  TBranch        *b_lep_isTightSelected_1;   //!
  TBranch        *b_lep_isTightSelected_2;   //!
  TBranch        *b_lep_isTightSelected_3;   //!
  TBranch        *b_lep_isTightSelected_2LOS1Tau_0;   //!
  TBranch        *b_lep_isTightSelected_2LOS1Tau_1;   //!
  TBranch        *b_lep_isTightSelectedAndPt_0;   //!
  TBranch        *b_lep_isTightSelectedAndPt_1;   //!
  TBranch        *b_lep_isTightSelectedAndPt_2;   //!
  TBranch        *b_lep_isTightSelectedAndPt_3;   //!
  TBranch        *b_lep_isTightSelectedAndPt_2LOS1Tau_0;   //!
  TBranch        *b_lep_isTightSelectedAndPt_2LOS1Tau_1;   //!
  //TBranch        *b_jet_flavor_truth_label_ghost;   //!
  //TBranch        *b_jet_flavor_weight_MV2c10; //!
  //TBranch        *b_jet_pT;   //!
  //TBranch        *b_jet_eta;   //!
  TBranch        *b_MVA2lSSMarseille_weight_ttV;   //!
  TBranch        *b_MVA2lSSMarseille_weight_ttbar;   //!
  TBranch        *b_MVA2lSSMarseille_weight_ttV_2;   //!
  TBranch        *b_MVA2lSSMarseille_weight_ttbar_2;   //!
  TBranch        *b_MVA3lCERN_weight_ttH;   //!
  TBranch        *b_MVA3lCERN_weight_ttW;   //!
  TBranch        *b_MVA3lCERN_weight_ttZ;   //!
  TBranch        *b_MVA3lCERN_weight_VV;   //!
  TBranch        *b_MVA3lCERN_weight_ttbar;   //!
  TBranch        *b_MVA3lCERN_category;   //!
  TBranch        *b_isLep2SS0tau_ee_QMisID;   //!
  TBranch        *b_isLep2SS0tau_OF_QMisID;   //!
  TBranch        *b_is2lttW_VR_ICHEP_QMisID;   //!
  TBranch        *b_total_charge_QMisID;   //!
  TBranch        *b_QMisID_EventWeight;   //!
  TBranch        *b_QMisID_EventWeight_Up;   //!
  TBranch        *b_QMisID_EventWeight_Dn;   //!
  TBranch        *b_isLep2SS1tau_QMisID;   //!
  TBranch        *b_total_charge_2ltau_QMisID;   //!
  TBranch        *b_QMisID_2ltau_EventWeight;   //!
  TBranch        *b_QMisID_2ltau_EventWeight_Up;   //!
  TBranch        *b_QMisID_2ltau_EventWeight_Dn;   //!
  TBranch        *b_passTrigger;   //!
  TBranch        *b_scale_nom;   //!
  TBranch        *b_scale_varA14up;   //!
  TBranch        *b_scale_varA14down;   //!
  TBranch        *b_mcWeightOrg;   //!
  TBranch        *b_pileupEventWeight_090;   //!
  TBranch        *b_MV2c10_60_EventWeight;   //!
  TBranch        *b_MV2c10_70_EventWeight;   //!
  TBranch        *b_MV2c10_77_EventWeight;   //!
  TBranch        *b_MV2c10_85_EventWeight;   //!
  TBranch        *b_MV2c10_Continuous_EventWeight;   //!
  TBranch        *b_bTagSF_weight_Continuous;   //!
  TBranch        *b_JVT_EventWeight;   //!
  TBranch        *b_SherpaNJetWeight;   //!
  TBranch        *b_EventNumber;   //!
  TBranch        *b_RunNumber;   //!
  TBranch        *b_lbn;   //!
  TBranch        *b_bcid;   //!
  TBranch        *b_passEventCleaning;   //!
  TBranch        *b_m_vxp_x;   //!
  TBranch        *b_m_vxp_y;   //!
  TBranch        *b_m_vxp_z;   //!
  TBranch        *b_higgs_pt;   //!
  TBranch        *b_higgs_eta;   //!
  TBranch        *b_higgs_phi;   //!
  TBranch        *b_higgs_E;   //!
  TBranch        *b_higgs_rapidity;   //!
  TBranch        *b_top_pt;   //!
  TBranch        *b_top_eta;   //!
  TBranch        *b_top_phi;   //!
  TBranch        *b_top_E;   //!
  TBranch        *b_antitop_pt;   //!
  TBranch        *b_antitop_eta;   //!
  TBranch        *b_antitop_phi;   //!
  TBranch        *b_antitop_E;   //!
  TBranch        *b_RunYear;   //!
  TBranch        *b_HF_Classification;   //!
  TBranch        *b_HF_ClassificationTop;   //!
  TBranch        *b_DLF_Classification;   //!
  TBranch        *b_ttbar_HF_EventType;   //!
  TBranch        *b_higgsDecayMode;   //!
  TBranch        *b_mc_channel_number;   //!
  TBranch        *b_mc_isAFII;   //!
  TBranch        *b_averageIntPerXing_uncorr;   //!
  TBranch        *b_averageIntPerXing;   //!
  TBranch        *b_actualIntPerXing;   //!
  TBranch        *b_m_vxp_n;   //!
  TBranch        *b_m_vxpu_n;   //!
  TBranch        *b_MET_RefFinal_et;   //!
  TBranch        *b_MET_RefFinal_phi;   //!
  TBranch        *b_MET_RefFinal_sumet;   //!
  TBranch        *b_m_hasFSRPhotonLargeDeltaR;   //!
  TBranch        *b_m_hasMEphoton;   //!
  TBranch        *b_m_hasMEphoton_DRgt02_nonhad;   //!
  TBranch        *b_m_MEphoton_OLtty_keepEvent;   //!
  TBranch        *b_m_MEphoton_OLtty_cat1;   //!
  TBranch        *b_m_MEphoton_OLtty_cat2;   //!
  TBranch        *b_m_MEphoton_OLtty_cat3;   //!
  TBranch        *b_m_MEphoton_OLtty_cat4;   //!
  TBranch        *b_m_MEphoton_pt;   //!
  TBranch        *b_m_MEphoton_eta;   //!
  TBranch        *b_m_MEphoton_phi;   //!
  TBranch        *b_m_MEphoton_mother_pt;   //!
  TBranch        *b_m_MEphoton_mother_eta;   //!
  TBranch        *b_m_MEphoton_mother_phi;   //!
  TBranch        *b_m_MEphoton_mother_pdgId;   //!
  TBranch        *b_HLT_2e12_lhloose_L12EM10VH;   //!
  TBranch        *b_HLT_2e12_lhvloose_nod0_L12EM10VH;   //!
  TBranch        *b_HLT_2e17_lhvloose_nod0;   //!
  TBranch        *b_HLT_2e24_lhvloose_nod0;   //!
  TBranch        *b_HLT_e120_lhloose;   //!
  TBranch        *b_HLT_e140_lhloose;   //!
  TBranch        *b_HLT_e140_lhloose_nod0;   //!
  TBranch        *b_HLT_e17_lhloose_mu14;   //!
  TBranch        *b_HLT_e17_lhloose_nod0_mu14;   //!
  TBranch        *b_HLT_e24_lhmedium_L1EM18VH;   //!
  TBranch        *b_HLT_e24_lhmedium_L1EM20VH;   //!
  TBranch        *b_HLT_e24_lhmedium_nod0_L1EM20VH;   //!
  TBranch        *b_HLT_e26_lhtight_iloose;   //!
  TBranch        *b_HLT_e26_lhtight_nod0_ivarloose;   //!
  TBranch        *b_HLT_e60_lhmedium;   //!
  TBranch        *b_HLT_e60_lhmedium_nod0;   //!
  TBranch        *b_HLT_e60_medium;   //!
  TBranch        *b_HLT_e7_lhmedium_mu24;   //!
  TBranch        *b_HLT_e7_medium_mu24;   //!
  TBranch        *b_HLT_mu18_mu8noL1;   //!
  TBranch        *b_HLT_mu20_iloose_L1MU15;   //!
  TBranch        *b_HLT_mu20_mu8noL1;   //!
  TBranch        *b_HLT_mu22_mu8noL1;   //!
  TBranch        *b_HLT_mu24_imedium;   //!
  TBranch        *b_HLT_mu24_ivarloose;   //!
  TBranch        *b_HLT_mu24_ivarmedium;   //!
  TBranch        *b_HLT_mu26_imedium;   //!
  TBranch        *b_HLT_mu26_ivarmedium;   //!
  TBranch        *b_HLT_mu50;   //!
  TBranch        *b_HLT_2e17_lhvloose_nod0_L12EM15VHI;   //!
  TBranch        *b_lep_promptLeptonInput_sv1_jf_ntrkv_0;   //!
  TBranch        *b_lep_promptLeptonInput_sv1_jf_ntrkv_1;   //!
  TBranch        *b_lep_promptLeptonInput_sv1_jf_ntrkv_2;   //!
  TBranch        *b_lep_promptLeptonInput_TrackJetNTrack_0;   //!
  TBranch        *b_lep_promptLeptonInput_TrackJetNTrack_1;   //!
  TBranch        *b_lep_promptLeptonInput_TrackJetNTrack_2;   //!
  TBranch        *b_lep_promptLeptonInput_DL1mu_0;   //!
  TBranch        *b_lep_promptLeptonInput_DL1mu_1;   //!
  TBranch        *b_lep_promptLeptonInput_DL1mu_2;   //!
  TBranch        *b_lep_promptLeptonInput_DRlj_0;   //!
  TBranch        *b_lep_promptLeptonInput_DRlj_1;   //!
  TBranch        *b_lep_promptLeptonInput_DRlj_2;   //!
  TBranch        *b_lep_promptLeptonInput_LepJetPtFrac_0;   //!
  TBranch        *b_lep_promptLeptonInput_LepJetPtFrac_1;   //!
  TBranch        *b_lep_promptLeptonInput_LepJetPtFrac_2;   //!
  TBranch        *b_lep_promptLeptonInput_PtFrac_0;   //!
  TBranch        *b_lep_promptLeptonInput_PtFrac_1;   //!
  TBranch        *b_lep_promptLeptonInput_PtFrac_2;   //!
  TBranch        *b_lep_promptLeptonInput_PtRel_0;   //!
  TBranch        *b_lep_promptLeptonInput_PtRel_1;   //!
  TBranch        *b_lep_promptLeptonInput_PtRel_2;   //!
  TBranch        *b_lep_promptLeptonInput_ip2_0;   //!
  TBranch        *b_lep_promptLeptonInput_ip2_1;   //!
  TBranch        *b_lep_promptLeptonInput_ip2_2;   //!
  TBranch        *b_lep_promptLeptonInput_ip3_0;   //!
  TBranch        *b_lep_promptLeptonInput_ip3_1;   //!
  TBranch        *b_lep_promptLeptonInput_ip3_2;   //!
  TBranch        *b_lep_promptLeptonInput_rnnip_0;   //!
  TBranch        *b_lep_promptLeptonInput_rnnip_1;   //!
  TBranch        *b_lep_promptLeptonInput_rnnip_2;   //!
  TBranch        *b_lep_ID_0;   //!
  TBranch        *b_lep_Index_0;   //!
  TBranch        *b_lep_ambiguityType_0;   //!
  TBranch        *b_lep_Pt_0;   //!
  TBranch        *b_lep_E_0;   //!
  TBranch        *b_lep_Eta_0;   //!
  TBranch        *b_lep_Phi_0;   //!
  TBranch        *b_lep_EtaBE2_0;   //!
  TBranch        *b_lep_topoEtcone20_0;   //!
  TBranch        *b_lep_topoEtcone30_0;   //!
  TBranch        *b_lep_topoEtcone40_0;   //!
  TBranch        *b_lep_ptVarcone20_0;   //!
  TBranch        *b_lep_ptVarcone30_0;   //!
  TBranch        *b_lep_ptVarcone40_0;   //!
  TBranch        *b_lep_sigd0PV_0;   //!
  TBranch        *b_lep_Z0SinTheta_0;   //!
  TBranch        *b_lep_d0_0;   //!
  TBranch        *b_lep_z0_0;   //!
  TBranch        *b_lep_vz_0;   //!
  TBranch        *b_lep_deltaz0_0;   //!
  TBranch        *b_lep_isTightLH_0;   //!
  TBranch        *b_lep_isMediumLH_0;   //!
  TBranch        *b_lep_isLooseLH_0;   //!
  TBranch        *b_lep_isTight_0;   //!
  TBranch        *b_lep_isMedium_0;   //!
  TBranch        *b_lep_isLoose_0;   //!
  TBranch        *b_lep_isolationLooseTrackOnly_0;   //!
  TBranch        *b_lep_isolationLoose_0;   //!
  TBranch        *b_lep_isolationGradient_0;   //!
  TBranch        *b_lep_isolationGradientLoose_0;   //!
  TBranch        *b_lep_isolationFixedCutTight_0;   //!
  TBranch        *b_lep_isolationFixedCutTightTrackOnly_0;   //!
  TBranch        *b_lep_isolationFixedCutLoose_0;   //!
  TBranch        *b_lep_isTrigMatch_0;   //!
  TBranch        *b_lep_isTrigMatchDLT_0;   //!
  TBranch        *b_lep_isPrompt_0;   //!
  TBranch        *b_lep_isFakeLep_0;   //!
  TBranch        *b_lep_isQMisID_0;   //!
  TBranch        *b_lep_isConvPh_0;   //!
  TBranch        *b_lep_isIntConvPh_0;   //!
  TBranch        *b_lep_isExtConvPh_0;   //!
  TBranch        *b_lep_isISR_FSR_Ph_0;   //!
  TBranch        *b_lep_isBrems_0;   //!
  TBranch        *b_lep_isTruthMatched_0;   //!
  TBranch        *b_lep_truthType_0;   //!
  TBranch        *b_lep_truthOrigin_0;   //!
  TBranch        *b_lep_truthPdgId_0;   //!
  TBranch        *b_lep_truthStatus_0;   //!
  TBranch        *b_lep_truthParentType_0;   //!
  TBranch        *b_lep_truthParentOrigin_0;   //!
  TBranch        *b_lep_truthParentPdgId_0;   //!
  TBranch        *b_lep_truthParentStatus_0;   //!
  TBranch        *b_lep_truthPt_0;   //!
  TBranch        *b_lep_truthEta_0;   //!
  TBranch        *b_lep_truthPhi_0;   //!
  TBranch        *b_lep_truthM_0;   //!
  TBranch        *b_lep_truthE_0;   //!
  TBranch        *b_lep_truthRapidity_0;   //!
  TBranch        *b_lep_SFIDLoose_0;   //!
  TBranch        *b_lep_SFIDTight_0;   //!
  TBranch        *b_lep_SFTrigLoose_0;   //!
  TBranch        *b_lep_SFTrigTight_0;   //!
  TBranch        *b_lep_EffTrigLoose_0;   //!
  TBranch        *b_lep_EffTrigTight_0;   //!
  TBranch        *b_lep_SFIsoLoose_0;   //!
  TBranch        *b_lep_SFIsoTight_0;   //!
  TBranch        *b_lep_SFReco_0;   //!
  TBranch        *b_lep_SFTTVA_0;   //!
  TBranch        *b_lep_SFObjLoose_0;   //!
  TBranch        *b_lep_SFObjTight_0;   //!
  TBranch        *b_lep_chargeIDBDTLoose_0;   //!
  TBranch        *b_lep_chargeIDBDTMedium_0;   //!
  TBranch        *b_lep_chargeIDBDTTight_0;   //!
  TBranch        *b_lep_promptLeptonIso_TagWeight_0;   //!
  TBranch        *b_lep_promptLeptonVeto_TagWeight_0;   //!
  TBranch        *b_lep_nTrackParticles_0;   //!
  TBranch        *b_lep_RadiusCO_0; //!
  TBranch        *b_lep_SeparationMinDCT_0; //!
  TBranch        *b_lep_ClosestSiTracknIL_0; //!
  TBranch        *b_lep_ClosestSiTrackeIL_0; //!
  TBranch        *b_lep_ClosestSiTracknNIL_0; //!
  TBranch        *b_lep_ClosestSiTrackeNIL_0; //!
  TBranch        *b_lep_BestMatchTrackPt_0; //!
  TBranch        *b_lep_BestMatchTrackEta_0; //!
  TBranch        *b_lep_BestMatchTrackPhi_0; //!
  TBranch        *b_lep_BestMatchTrackD0_0; //!
  TBranch        *b_lep_BestMatchTrackZ0_0; //!
  TBranch        *b_lep_BestMatchTrackQ_0; //!
  TBranch        *b_lep_ClosestSiTrackPt_0; //!
  TBranch        *b_lep_ClosestSiTrackEta_0; //!
  TBranch        *b_lep_ClosestSiTrackPhi_0; //!
  TBranch        *b_lep_ClosestSiTrackD0_0; //!
  TBranch        *b_lep_ClosestSiTrackZ0_0; //!
  TBranch        *b_lep_ClosestSiTrackQ_0; //!
  TBranch        *b_lep_Mtrktrk_atConvV_CO_0; //!
  TBranch        *b_lep_Mtrktrk_atPV_CO_0; //!
  TBranch        *b_lep_RadiusCOX_0; //!
  TBranch        *b_lep_RadiusCOY_0; //!
  TBranch        *b_lep_ID_1;   //!
  TBranch        *b_lep_Index_1;   //!
  TBranch        *b_lep_ambiguityType_1;   //!
  TBranch        *b_lep_Pt_1;   //!
  TBranch        *b_lep_E_1;   //!
  TBranch        *b_lep_Eta_1;   //!
  TBranch        *b_lep_Phi_1;   //!
  TBranch        *b_lep_EtaBE2_1;   //!
  TBranch        *b_lep_topoEtcone20_1;   //!
  TBranch        *b_lep_topoEtcone30_1;   //!
  TBranch        *b_lep_topoEtcone40_1;   //!
  TBranch        *b_lep_ptVarcone20_1;   //!
  TBranch        *b_lep_ptVarcone30_1;   //!
  TBranch        *b_lep_ptVarcone40_1;   //!
  TBranch        *b_lep_sigd0PV_1;   //!
  TBranch        *b_lep_Z0SinTheta_1;   //!
  TBranch        *b_lep_d0_1;   //!
  TBranch        *b_lep_z0_1;   //!
  TBranch        *b_lep_vz_1;   //!
  TBranch        *b_lep_deltaz0_1;   //!
  TBranch        *b_lep_isTightLH_1;   //!
  TBranch        *b_lep_isMediumLH_1;   //!
  TBranch        *b_lep_isLooseLH_1;   //!
  TBranch        *b_lep_isTight_1;   //!
  TBranch        *b_lep_isMedium_1;   //!
  TBranch        *b_lep_isLoose_1;   //!
  TBranch        *b_lep_isolationLooseTrackOnly_1;   //!
  TBranch        *b_lep_isolationLoose_1;   //!
  TBranch        *b_lep_isolationGradient_1;   //!
  TBranch        *b_lep_isolationGradientLoose_1;   //!
  TBranch        *b_lep_isolationFixedCutTight_1;   //!
  TBranch        *b_lep_isolationFixedCutTightTrackOnly_1;   //!
  TBranch        *b_lep_isolationFixedCutLoose_1;   //!
  TBranch        *b_lep_isTrigMatch_1;   //!
  TBranch        *b_lep_isTrigMatchDLT_1;   //!
  TBranch        *b_lep_isPrompt_1;   //!
  TBranch        *b_lep_isFakeLep_1;   //!
  TBranch        *b_lep_isQMisID_1;   //!
  TBranch        *b_lep_isConvPh_1;   //!
  TBranch        *b_lep_isIntConvPh_1;   //!
  TBranch        *b_lep_isExtConvPh_1;   //!
  TBranch        *b_lep_isISR_FSR_Ph_1;   //!
  TBranch        *b_lep_isBrems_1;   //!
  TBranch        *b_lep_isTruthMatched_1;   //!
  TBranch        *b_lep_truthType_1;   //!
  TBranch        *b_lep_truthOrigin_1;   //!
  TBranch        *b_lep_truthPdgId_1;   //!
  TBranch        *b_lep_truthStatus_1;   //!
  TBranch        *b_lep_truthParentType_1;   //!
  TBranch        *b_lep_truthParentOrigin_1;   //!
  TBranch        *b_lep_truthParentPdgId_1;   //!
  TBranch        *b_lep_truthParentStatus_1;   //!
  TBranch        *b_lep_truthPt_1;   //!
  TBranch        *b_lep_truthEta_1;   //!
  TBranch        *b_lep_truthPhi_1;   //!
  TBranch        *b_lep_truthM_1;   //!
  TBranch        *b_lep_truthE_1;   //!
  TBranch        *b_lep_truthRapidity_1;   //!
  TBranch        *b_lep_SFIDLoose_1;   //!
  TBranch        *b_lep_SFIDTight_1;   //!
  TBranch        *b_lep_SFTrigLoose_1;   //!
  TBranch        *b_lep_SFTrigTight_1;   //!
  TBranch        *b_lep_EffTrigLoose_1;   //!
  TBranch        *b_lep_EffTrigTight_1;   //!
  TBranch        *b_lep_SFIsoLoose_1;   //!
  TBranch        *b_lep_SFIsoTight_1;   //!
  TBranch        *b_lep_SFReco_1;   //!
  TBranch        *b_lep_SFTTVA_1;   //!
  TBranch        *b_lep_SFObjLoose_1;   //!
  TBranch        *b_lep_SFObjTight_1;   //!
  TBranch        *b_lep_chargeIDBDTLoose_1;   //!
  TBranch        *b_lep_chargeIDBDTMedium_1;   //!
  TBranch        *b_lep_chargeIDBDTTight_1;   //!
  TBranch        *b_lep_promptLeptonIso_TagWeight_1;   //!
  TBranch        *b_lep_promptLeptonVeto_TagWeight_1;   //!
  TBranch        *b_lep_nTrackParticles_1;   //!
  TBranch        *b_lep_RadiusCO_1; //!
  TBranch        *b_lep_SeparationMinDCT_1; //!
  TBranch        *b_lep_ClosestSiTracknIL_1; //!
  TBranch        *b_lep_ClosestSiTrackeIL_1; //!
  TBranch        *b_lep_ClosestSiTracknNIL_1; //!
  TBranch        *b_lep_ClosestSiTrackeNIL_1; //!
  TBranch        *b_lep_BestMatchTrackPt_1; //!
  TBranch        *b_lep_BestMatchTrackEta_1; //!
  TBranch        *b_lep_BestMatchTrackPhi_1; //!
  TBranch        *b_lep_BestMatchTrackD0_1; //!
  TBranch        *b_lep_BestMatchTrackZ0_1; //!
  TBranch        *b_lep_BestMatchTrackQ_1; //!
  TBranch        *b_lep_ClosestSiTrackPt_1; //!
  TBranch        *b_lep_ClosestSiTrackEta_1; //!
  TBranch        *b_lep_ClosestSiTrackPhi_1; //!
  TBranch        *b_lep_ClosestSiTrackD0_1; //!
  TBranch        *b_lep_ClosestSiTrackZ0_1; //!
  TBranch        *b_lep_ClosestSiTrackQ_1; //!
  TBranch        *b_lep_Mtrktrk_atConvV_CO_1; //!
  TBranch        *b_lep_Mtrktrk_atPV_CO_1; //!
  TBranch        *b_lep_RadiusCOX_1; //!
  TBranch        *b_lep_RadiusCOY_1; //!
  TBranch        *b_lep_ID_2;   //!
  TBranch        *b_lep_Index_2;   //!
  TBranch        *b_lep_ambiguityType_2;   //!
  TBranch        *b_lep_Pt_2;   //!
  TBranch        *b_lep_E_2;   //!
  TBranch        *b_lep_Eta_2;   //!
  TBranch        *b_lep_Phi_2;   //!
  TBranch        *b_lep_EtaBE2_2;   //!
  TBranch        *b_lep_topoEtcone20_2;   //!
  TBranch        *b_lep_topoEtcone30_2;   //!
  TBranch        *b_lep_topoEtcone40_2;   //!
  TBranch        *b_lep_ptVarcone20_2;   //!
  TBranch        *b_lep_ptVarcone30_2;   //!
  TBranch        *b_lep_ptVarcone40_2;   //!
  TBranch        *b_lep_sigd0PV_2;   //!
  TBranch        *b_lep_Z0SinTheta_2;   //!
  TBranch        *b_lep_d0_2;   //!
  TBranch        *b_lep_z0_2;   //!
  TBranch        *b_lep_vz_2;   //!
  TBranch        *b_lep_deltaz0_2;   //!
  TBranch        *b_lep_isTightLH_2;   //!
  TBranch        *b_lep_isMediumLH_2;   //!
  TBranch        *b_lep_isLooseLH_2;   //!
  TBranch        *b_lep_isTight_2;   //!
  TBranch        *b_lep_isMedium_2;   //!
  TBranch        *b_lep_isLoose_2;   //!
  TBranch        *b_lep_isolationLooseTrackOnly_2;   //!
  TBranch        *b_lep_isolationLoose_2;   //!
  TBranch        *b_lep_isolationGradient_2;   //!
  TBranch        *b_lep_isolationGradientLoose_2;   //!
  TBranch        *b_lep_isolationFixedCutTight_2;   //!
  TBranch        *b_lep_isolationFixedCutTightTrackOnly_2;   //!
  TBranch        *b_lep_isolationFixedCutLoose_2;   //!
  TBranch        *b_lep_isTrigMatch_2;   //!
  TBranch        *b_lep_isTrigMatchDLT_2;   //!
  TBranch        *b_lep_isPrompt_2;   //!
  TBranch        *b_lep_isFakeLep_2;   //!
  TBranch        *b_lep_isQMisID_2;   //!
  TBranch        *b_lep_isConvPh_2;   //!
  TBranch        *b_lep_isIntConvPh_2;   //!
  TBranch        *b_lep_isExtConvPh_2;   //!
  TBranch        *b_lep_isISR_FSR_Ph_2;   //!
  TBranch        *b_lep_isBrems_2;   //!
  TBranch        *b_lep_isTruthMatched_2;   //!
  TBranch        *b_lep_truthType_2;   //!
  TBranch        *b_lep_truthOrigin_2;   //!
  TBranch        *b_lep_truthPdgId_2;   //!
  TBranch        *b_lep_truthStatus_2;   //!
  TBranch        *b_lep_truthParentType_2;   //!
  TBranch        *b_lep_truthParentOrigin_2;   //!
  TBranch        *b_lep_truthParentPdgId_2;   //!
  TBranch        *b_lep_truthParentStatus_2;   //!
  TBranch        *b_lep_truthPt_2;   //!
  TBranch        *b_lep_truthEta_2;   //!
  TBranch        *b_lep_truthPhi_2;   //!
  TBranch        *b_lep_truthM_2;   //!
  TBranch        *b_lep_truthE_2;   //!
  TBranch        *b_lep_truthRapidity_2;   //!
  TBranch        *b_lep_SFIDLoose_2;   //!
  TBranch        *b_lep_SFIDTight_2;   //!
  TBranch        *b_lep_SFTrigLoose_2;   //!
  TBranch        *b_lep_SFTrigTight_2;   //!
  TBranch        *b_lep_EffTrigLoose_2;   //!
  TBranch        *b_lep_EffTrigTight_2;   //!
  TBranch        *b_lep_SFIsoLoose_2;   //!
  TBranch        *b_lep_SFIsoTight_2;   //!
  TBranch        *b_lep_SFReco_2;   //!
  TBranch        *b_lep_SFTTVA_2;   //!
  TBranch        *b_lep_SFObjLoose_2;   //!
  TBranch        *b_lep_SFObjTight_2;   //!
  TBranch        *b_lep_chargeIDBDTLoose_2;   //!
  TBranch        *b_lep_chargeIDBDTMedium_2;   //!
  TBranch        *b_lep_chargeIDBDTTight_2;   //!
  TBranch        *b_lep_promptLeptonIso_TagWeight_2;   //!
  TBranch        *b_lep_promptLeptonVeto_TagWeight_2;   //!
  TBranch        *b_lep_nTrackParticles_2;   //!
  TBranch        *b_lep_RadiusCO_2; //!
  TBranch        *b_lep_SeparationMinDCT_2; //!
  TBranch        *b_lep_ClosestSiTracknIL_2; //!
  TBranch        *b_lep_ClosestSiTrackeIL_2; //!
  TBranch        *b_lep_ClosestSiTracknNIL_2; //!
  TBranch        *b_lep_ClosestSiTrackeNIL_2; //!
  TBranch        *b_lep_BestMatchTrackPt_2; //!
  TBranch        *b_lep_BestMatchTrackEta_2; //!
  TBranch        *b_lep_BestMatchTrackPhi_2; //!
  TBranch        *b_lep_BestMatchTrackD0_2; //!
  TBranch        *b_lep_BestMatchTrackZ0_2; //!
  TBranch        *b_lep_BestMatchTrackQ_2; //!
  TBranch        *b_lep_ClosestSiTrackPt_2; //!
  TBranch        *b_lep_ClosestSiTrackEta_2; //!
  TBranch        *b_lep_ClosestSiTrackPhi_2; //!
  TBranch        *b_lep_ClosestSiTrackD0_2; //!
  TBranch        *b_lep_ClosestSiTrackZ0_2; //!
  TBranch        *b_lep_ClosestSiTrackQ_2; //!
  TBranch        *b_lep_Mtrktrk_atConvV_CO_2; //!
  TBranch        *b_lep_Mtrktrk_atPV_CO_2; //!
  TBranch        *b_lep_RadiusCOX_2; //!
  TBranch        *b_lep_RadiusCOY_2; //!
  TBranch        *b_lep_ID_3;   //!
  TBranch        *b_lep_Index_3;   //!
  TBranch        *b_lep_ambiguityType_3;   //!
  TBranch        *b_lep_Pt_3;   //!
  TBranch        *b_lep_E_3;   //!
  TBranch        *b_lep_Eta_3;   //!
  TBranch        *b_lep_Phi_3;   //!
  TBranch        *b_lep_EtaBE2_3;   //!
  TBranch        *b_lep_topoEtcone20_3;   //!
  TBranch        *b_lep_topoEtcone30_3;   //!
  TBranch        *b_lep_topoEtcone40_3;   //!
  TBranch        *b_lep_ptVarcone20_3;   //!
  TBranch        *b_lep_ptVarcone30_3;   //!
  TBranch        *b_lep_ptVarcone40_3;   //!
  TBranch        *b_lep_sigd0PV_3;   //!
  TBranch        *b_lep_Z0SinTheta_3;   //!
  TBranch        *b_lep_d0_3;   //!
  TBranch        *b_lep_z0_3;   //!
  TBranch        *b_lep_vz_3;   //!
  TBranch        *b_lep_deltaz0_3;   //!
  TBranch        *b_lep_isTightLH_3;   //!
  TBranch        *b_lep_isMediumLH_3;   //!
  TBranch        *b_lep_isLooseLH_3;   //!
  TBranch        *b_lep_isTight_3;   //!
  TBranch        *b_lep_isMedium_3;   //!
  TBranch        *b_lep_isLoose_3;   //!
  TBranch        *b_lep_isolationLooseTrackOnly_3;   //!
  TBranch        *b_lep_isolationLoose_3;   //!
  TBranch        *b_lep_isolationGradient_3;   //!
  TBranch        *b_lep_isolationGradientLoose_3;   //!
  TBranch        *b_lep_isolationFixedCutTight_3;   //!
  TBranch        *b_lep_isolationFixedCutTightTrackOnly_3;   //!
  TBranch        *b_lep_isolationFixedCutLoose_3;   //!
  TBranch        *b_lep_isTrigMatch_3;   //!
  TBranch        *b_lep_isTrigMatchDLT_3;   //!
  TBranch        *b_lep_isPrompt_3;   //!
  TBranch        *b_lep_isFakeLep_3;   //!
  TBranch        *b_lep_isQMisID_3;   //!
  TBranch        *b_lep_isConvPh_3;   //!
  TBranch        *b_lep_isIntConvPh_3;   //!
  TBranch        *b_lep_isExtConvPh_3;   //!
  TBranch        *b_lep_isISR_FSR_Ph_3;   //!
  TBranch        *b_lep_isBrems_3;   //!
  TBranch        *b_lep_isTruthMatched_3;   //!
  TBranch        *b_lep_truthType_3;   //!
  TBranch        *b_lep_truthOrigin_3;   //!
  TBranch        *b_lep_truthPdgId_3;   //!
  TBranch        *b_lep_truthStatus_3;   //!
  TBranch        *b_lep_truthParentType_3;   //!
  TBranch        *b_lep_truthParentOrigin_3;   //!
  TBranch        *b_lep_truthParentPdgId_3;   //!
  TBranch        *b_lep_truthParentStatus_3;   //!
  TBranch        *b_lep_truthPt_3;   //!
  TBranch        *b_lep_truthEta_3;   //!
  TBranch        *b_lep_truthPhi_3;   //!
  TBranch        *b_lep_truthM_3;   //!
  TBranch        *b_lep_truthE_3;   //!
  TBranch        *b_lep_truthRapidity_3;   //!
  TBranch        *b_lep_SFIDLoose_3;   //!
  TBranch        *b_lep_SFIDTight_3;   //!
  TBranch        *b_lep_SFTrigLoose_3;   //!
  TBranch        *b_lep_SFTrigTight_3;   //!
  TBranch        *b_lep_EffTrigLoose_3;   //!
  TBranch        *b_lep_EffTrigTight_3;   //!
  TBranch        *b_lep_SFIsoLoose_3;   //!
  TBranch        *b_lep_SFIsoTight_3;   //!
  TBranch        *b_lep_SFReco_3;   //!
  TBranch        *b_lep_SFTTVA_3;   //!
  TBranch        *b_lep_SFObjLoose_3;   //!
  TBranch        *b_lep_SFObjTight_3;   //!
  TBranch        *b_lep_chargeIDBDTLoose_3;   //!
  TBranch        *b_lep_chargeIDBDTMedium_3;   //!
  TBranch        *b_lep_chargeIDBDTTight_3;   //!
  TBranch        *b_lep_promptLeptonIso_TagWeight_3;   //!
  TBranch        *b_lep_promptLeptonVeto_TagWeight_3;   //!
  TBranch        *b_lep_nTrackParticles_3;   //!
  TBranch        *b_lep_RadiusCO_3; //!
  TBranch        *b_lep_SeparationMinDCT_3; //!
  TBranch        *b_lep_ClosestSiTracknIL_3; //!
  TBranch        *b_lep_ClosestSiTrackeIL_3; //!
  TBranch        *b_lep_ClosestSiTracknNIL_3; //!
  TBranch        *b_lep_ClosestSiTrackeNIL_3; //!
  TBranch        *b_lep_BestMatchTrackPt_3; //!
  TBranch        *b_lep_BestMatchTrackEta_3; //!
  TBranch        *b_lep_BestMatchTrackPhi_3; //!
  TBranch        *b_lep_BestMatchTrackD0_3; //!
  TBranch        *b_lep_BestMatchTrackZ0_3; //!
  TBranch        *b_lep_BestMatchTrackQ_3; //!
  TBranch        *b_lep_ClosestSiTrackPt_3; //!
  TBranch        *b_lep_ClosestSiTrackEta_3; //!
  TBranch        *b_lep_ClosestSiTrackPhi_3; //!
  TBranch        *b_lep_ClosestSiTrackD0_3; //!
  TBranch        *b_lep_ClosestSiTrackZ0_3; //!
  TBranch        *b_lep_ClosestSiTrackQ_3; //!
  TBranch        *b_lep_Mtrktrk_atConvV_CO_3; //!
  TBranch        *b_lep_Mtrktrk_atPV_CO_3; //!
  TBranch        *b_lep_RadiusCOX_3; //!
  TBranch        *b_lep_RadiusCOY_3; //!
  TBranch        *b_electron_ambiguityType;   //!
  TBranch        *b_electron_PromptLeptonIso_TagWeight;   //!
  TBranch        *b_electron_PromptLeptonVeto_TagWeight;   //!
  TBranch        *b_muon_PromptLeptonIso_TagWeight;   //!
  TBranch        *b_muon_PromptLeptonVeto_TagWeight;   //!
  TBranch        *b_tau_MV2c10_0;   //!
  TBranch        *b_tau_MV2c10_1;   //!
  TBranch        *b_tau_pt_0;   //!
  TBranch        *b_tau_eta_0;   //!
  TBranch        *b_tau_phi_0;   //!
  TBranch        *b_tau_charge_0;   //!
  TBranch        *b_tau_E_0;   //!
  TBranch        *b_tau_BDTJetScore_0;   //!
  TBranch        *b_tau_JetBDTSigLoose_0;   //!
  TBranch        *b_tau_JetBDTSigMedium_0;   //!
  TBranch        *b_tau_JetBDTSigTight_0;   //!
  TBranch        *b_tau_isHadronic_0;   //!
  TBranch        *b_tau_numTrack_0;   //!
  TBranch        *b_tau_SFTight_0;   //!
  TBranch        *b_tau_SFLoose_0;   //!
  TBranch        *b_tau_tagWeightBin_0;   //!
  TBranch        *b_tau_fromPV_0;   //!
  TBranch        *b_tau_passEleOLR_0;   //!
  TBranch        *b_tau_passEleBDT_0;   //!
  TBranch        *b_tau_passMuonOLR_0;   //!
  TBranch        *b_tau_truthOrigin_0;   //!
  TBranch        *b_tau_truthType_0;   //!
  TBranch        *b_tau_truthJetFlavour_0;   //!
  TBranch        *b_tau_pt_1;   //!
  TBranch        *b_tau_eta_1;   //!
  TBranch        *b_tau_phi_1;   //!
  TBranch        *b_tau_charge_1;   //!
  TBranch        *b_tau_E_1;   //!
  TBranch        *b_tau_BDTJetScore_1;   //!
  TBranch        *b_tau_JetBDTSigLoose_1;   //!
  TBranch        *b_tau_JetBDTSigMedium_1;   //!
  TBranch        *b_tau_JetBDTSigTight_1;   //!
  TBranch        *b_tau_numTrack_1;   //!
  TBranch        *b_tau_isHadronic_1;   //!
  TBranch        *b_tau_SFTight_1;   //!
  TBranch        *b_tau_SFLoose_1;   //!
  TBranch        *b_tau_tagWeightBin_1;   //!
  TBranch        *b_tau_fromPV_1;   //!
  TBranch        *b_tau_passEleOLR_1;   //!
  TBranch        *b_tau_passEleBDT_1;   //!
  TBranch        *b_tau_passMuonOLR_1;   //!
  TBranch        *b_tau_truthOrigin_1;   //!
  TBranch        *b_tau_truthType_1;   //!
  TBranch        *b_tau_truthJetFlavour_1;   //!
  TBranch        *b_tau_promptTauVeto_0;   //!
  TBranch        *b_tau_promptTauVeto_1;   //!
  TBranch        *b_onelep_type;   //!
  TBranch        *b_dilep_type;   //!
  TBranch        *b_trilep_type;   //!
  TBranch        *b_quadlep_type;   //!
  TBranch        *b_total_charge;   //!
  TBranch        *b_total_leptons;   //!
  TBranch        *b_isQMisIDEvent;   //!
  TBranch        *b_isFakeEvent;   //!
  TBranch        *b_isLepFromPhEvent;   //!
  TBranch        *b_Mll01;   //!
  TBranch        *b_Ptll01;   //!
  TBranch        *b_DRll01;   //!
  TBranch        *b_Mlll012;   //!
  TBranch        *b_Mllll0123;   //!
  TBranch        *b_Mlll013;   //!
  TBranch        *b_Mll02;   //!
  TBranch        *b_Ptll02;   //!
  TBranch        *b_DRll02;   //!
  TBranch        *b_Mlll023;   //!
  TBranch        *b_Mll03;   //!
  TBranch        *b_Ptll03;   //!
  TBranch        *b_DRll03;   //!
  TBranch        *b_Mll12;   //!
  TBranch        *b_Ptll12;   //!
  TBranch        *b_DRll12;   //!
  TBranch        *b_Mlll123;   //!
  TBranch        *b_Mll13;   //!
  TBranch        *b_Ptll13;   //!
  TBranch        *b_DRll13;   //!
  TBranch        *b_Mll23;   //!
  TBranch        *b_Ptll23;   //!
  TBranch        *b_DRll23;   //!
  TBranch        *b_matchDLTll01;   //!
  TBranch        *b_matchDLTll02;   //!
  TBranch        *b_matchDLTll12;   //!
  TBranch        *b_matchDLTll03;   //!
  TBranch        *b_matchDLTll13;   //!
  TBranch        *b_matchDLTll23;   //!
  TBranch        *b_best_Z_Mll;   //!
  TBranch        *b_best_Z_other_Mll;   //!
  TBranch        *b_best_Z_other_MtLepMet;   //!
  TBranch        *b_nJets_OR_T;   //!
  TBranch        *b_nJets_OR;   //!
  TBranch        *b_nJets_OR_T_MV2c20_60;   //!
  TBranch        *b_nJets_OR_T_MV2c20_70;   //!
  TBranch        *b_nJets_OR_T_MV2c20_77;   //!
  TBranch        *b_nJets_OR_T_MV2c20_85;   //!
  TBranch        *b_nJets_OR_MV2c20_85;   //!
  TBranch        *b_nJets_OR_MV2c20_60;   //!
  TBranch        *b_nJets_OR_MV2c20_77;   //!
  TBranch        *b_nJets_OR_MV2c20_70;   //!
  TBranch        *b_nJets_OR_T_MV2c10_60;   //!
  TBranch        *b_nJets_OR_T_MV2c10_70;   //!
  TBranch        *b_nJets_OR_T_MV2c10_77;   //!
  TBranch        *b_nJets_OR_T_MV2c10_85;   //!
  TBranch        *b_nJets_OR_MV2c10_85;   //!
  TBranch        *b_nJets_OR_MV2c10_60;   //!
  TBranch        *b_nJets_OR_MV2c10_77;   //!
  TBranch        *b_nJets_OR_MV2c10_70;   //!
  TBranch        *b_nTaus_OR_Pt25;   //!
  TBranch        *b_isBlinded;   //!
  TBranch        *b_HT;   //!
  TBranch        *b_HT_lep;   //!
  TBranch        *b_HT_jets;   //!
  TBranch        *b_lead_jetPt;   //!
  TBranch        *b_lead_jetEta;   //!
  TBranch        *b_lead_jetPhi;   //!
  TBranch        *b_lead_jetE;   //!
  TBranch        *b_sublead_jetPt;   //!
  TBranch        *b_sublead_jetEta;   //!
  TBranch        *b_sublead_jetPhi;   //!
  TBranch        *b_sublead_jetE;   //!
  TBranch        *b_lepSFTrigLoose;   //!
  TBranch        *b_lepSFTrigTight;   //!
  TBranch        *b_lepSFTrigTightLoose;   //!
  TBranch        *b_lepSFTrigLooseTight;   //!
  TBranch        *b_lepEffTrigLoose;   //!
  TBranch        *b_lepEffTrigTight;   //!
  TBranch        *b_lepEffTrigTightLoose;   //!
  TBranch        *b_lepEffTrigLooseTight;   //!
  TBranch        *b_lepDataEffTrigLoose;   //!
  TBranch        *b_lepDataEffTrigTight;   //!
  TBranch        *b_lepDataEffTrigTightLoose;   //!
  TBranch        *b_lepDataEffTrigLooseTight;   //!
  TBranch        *b_lepSFObjLoose;   //!
  TBranch        *b_lepSFObjTight;   //!
  TBranch        *b_tauSFTight;   //!
  TBranch        *b_tauSFLoose;   //!
  TBranch        *b_isMMSideband_TT;   //!
  TBranch        *b_isMMSideband_TAntiT;   //!
  TBranch        *b_isMMSideband_AntiTT;   //!
  TBranch        *b_isMMSideband_AntiTAntiT;   //!
  TBranch        *b_MM_EventWeight;   //!
  TBranch        *b_MM_EventWeight_Fake_El_NBJets_VS_Pt_D_QMisID_10_Up;   //!
  TBranch        *b_MM_EventWeight_Fake_El_NBJets_VS_Pt_D_QMisID_10_Dn;   //!
  TBranch        *b_MM_EventWeight_Fake_El_NBJets_VS_Pt_D_QMisID_5_Up;   //!
  TBranch        *b_MM_EventWeight_Fake_El_NBJets_VS_Pt_D_QMisID_5_Dn;   //!
  TBranch        *b_MM_EventWeight_Fake_El_NBJets_VS_Pt_D_QMisID_6_Up;   //!
  TBranch        *b_MM_EventWeight_Fake_El_NBJets_VS_Pt_D_QMisID_6_Dn;   //!
  TBranch        *b_MM_EventWeight_Fake_El_NBJets_VS_Pt_D_QMisID_9_Up;   //!
  TBranch        *b_MM_EventWeight_Fake_El_NBJets_VS_Pt_D_QMisID_9_Dn;   //!
  TBranch        *b_MM_EventWeight_Fake_El_NBJets_VS_Pt_ND_OtherPromptSS_Up;   //!
  TBranch        *b_MM_EventWeight_Fake_El_NBJets_VS_Pt_ND_OtherPromptSS_Dn;   //!
  TBranch        *b_MM_EventWeight_Fake_El_NBJets_VS_Pt_ND_TTV_Up;   //!
  TBranch        *b_MM_EventWeight_Fake_El_NBJets_VS_Pt_ND_TTV_Dn;   //!
  TBranch        *b_MM_EventWeight_Fake_El_NBJets_VS_Pt_ND_VV_Up;   //!
  TBranch        *b_MM_EventWeight_Fake_El_NBJets_VS_Pt_ND_VV_Dn;   //!
  TBranch        *b_MM_EventWeight_Fake_El_NBJets_VS_Pt_N_QMisID_10_Up;   //!
  TBranch        *b_MM_EventWeight_Fake_El_NBJets_VS_Pt_N_QMisID_10_Dn;   //!
  TBranch        *b_MM_EventWeight_Fake_El_NBJets_VS_Pt_N_QMisID_5_Up;   //!
  TBranch        *b_MM_EventWeight_Fake_El_NBJets_VS_Pt_N_QMisID_5_Dn;   //!
  TBranch        *b_MM_EventWeight_Fake_El_NBJets_VS_Pt_N_QMisID_6_Up;   //!
  TBranch        *b_MM_EventWeight_Fake_El_NBJets_VS_Pt_N_QMisID_6_Dn;   //!
  TBranch        *b_MM_EventWeight_Fake_El_NBJets_VS_Pt_N_QMisID_9_Up;   //!
  TBranch        *b_MM_EventWeight_Fake_El_NBJets_VS_Pt_N_QMisID_9_Dn;   //!
  TBranch        *b_MM_EventWeight_Fake_El_NBJets_VS_Pt_Stat_10_Up;   //!
  TBranch        *b_MM_EventWeight_Fake_El_NBJets_VS_Pt_Stat_10_Dn;   //!
  TBranch        *b_MM_EventWeight_Fake_El_NBJets_VS_Pt_Stat_5_Up;   //!
  TBranch        *b_MM_EventWeight_Fake_El_NBJets_VS_Pt_Stat_5_Dn;   //!
  TBranch        *b_MM_EventWeight_Fake_El_NBJets_VS_Pt_Stat_6_Up;   //!
  TBranch        *b_MM_EventWeight_Fake_El_NBJets_VS_Pt_Stat_6_Dn;   //!
  TBranch        *b_MM_EventWeight_Fake_El_NBJets_VS_Pt_Stat_9_Up;   //!
  TBranch        *b_MM_EventWeight_Fake_El_NBJets_VS_Pt_Stat_9_Dn;   //!
  TBranch        *b_MM_EventWeight_Fake_Mu_DistanceClosestJet_VS_Pt_ND_OtherPromptSS_Up;   //!
  TBranch        *b_MM_EventWeight_Fake_Mu_DistanceClosestJet_VS_Pt_ND_OtherPromptSS_Dn;   //!
  TBranch        *b_MM_EventWeight_Fake_Mu_DistanceClosestJet_VS_Pt_ND_TTV_Up;   //!
  TBranch        *b_MM_EventWeight_Fake_Mu_DistanceClosestJet_VS_Pt_ND_TTV_Dn;   //!
  TBranch        *b_MM_EventWeight_Fake_Mu_DistanceClosestJet_VS_Pt_ND_VV_Up;   //!
  TBranch        *b_MM_EventWeight_Fake_Mu_DistanceClosestJet_VS_Pt_ND_VV_Dn;   //!
  TBranch        *b_MM_EventWeight_Fake_Mu_DistanceClosestJet_VS_Pt_Stat_10_Up;   //!
  TBranch        *b_MM_EventWeight_Fake_Mu_DistanceClosestJet_VS_Pt_Stat_10_Dn;   //!
  TBranch        *b_MM_EventWeight_Fake_Mu_DistanceClosestJet_VS_Pt_Stat_13_Up;   //!
  TBranch        *b_MM_EventWeight_Fake_Mu_DistanceClosestJet_VS_Pt_Stat_13_Dn;   //!
  TBranch        *b_MM_EventWeight_Fake_Mu_DistanceClosestJet_VS_Pt_Stat_14_Up;   //!
  TBranch        *b_MM_EventWeight_Fake_Mu_DistanceClosestJet_VS_Pt_Stat_14_Dn;   //!
  TBranch        *b_MM_EventWeight_Fake_Mu_DistanceClosestJet_VS_Pt_Stat_5_Up;   //!
  TBranch        *b_MM_EventWeight_Fake_Mu_DistanceClosestJet_VS_Pt_Stat_5_Dn;   //!
  TBranch        *b_MM_EventWeight_Fake_Mu_DistanceClosestJet_VS_Pt_Stat_6_Up;   //!
  TBranch        *b_MM_EventWeight_Fake_Mu_DistanceClosestJet_VS_Pt_Stat_6_Dn;   //!
  TBranch        *b_MM_EventWeight_Fake_Mu_DistanceClosestJet_VS_Pt_Stat_9_Up;   //!
  TBranch        *b_MM_EventWeight_Fake_Mu_DistanceClosestJet_VS_Pt_Stat_9_Dn;   //!
  TBranch        *b_MM_EventWeight_Real_El_Eta_VS_Pt_ND_FakesOS_Up;   //!
  TBranch        *b_MM_EventWeight_Real_El_Eta_VS_Pt_ND_FakesOS_Dn;   //!
  TBranch        *b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_10_Up;   //!
  TBranch        *b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_10_Dn;   //!
  TBranch        *b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_11_Up;   //!
  TBranch        *b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_11_Dn;   //!
  TBranch        *b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_12_Up;   //!
  TBranch        *b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_12_Dn;   //!
  TBranch        *b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_13_Up;   //!
  TBranch        *b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_13_Dn;   //!
  TBranch        *b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_14_Up;   //!
  TBranch        *b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_14_Dn;   //!
  TBranch        *b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_17_Up;   //!
  TBranch        *b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_17_Dn;   //!
  TBranch        *b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_18_Up;   //!
  TBranch        *b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_18_Dn;   //!
  TBranch        *b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_19_Up;   //!
  TBranch        *b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_19_Dn;   //!
  TBranch        *b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_20_Up;   //!
  TBranch        *b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_20_Dn;   //!
  TBranch        *b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_21_Up;   //!
  TBranch        *b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_21_Dn;   //!
  TBranch        *b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_22_Up;   //!
  TBranch        *b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_22_Dn;   //!
  TBranch        *b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_25_Up;   //!
  TBranch        *b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_25_Dn;   //!
  TBranch        *b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_26_Up;   //!
  TBranch        *b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_26_Dn;   //!
  TBranch        *b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_27_Up;   //!
  TBranch        *b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_27_Dn;   //!
  TBranch        *b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_28_Up;   //!
  TBranch        *b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_28_Dn;   //!
  TBranch        *b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_29_Up;   //!
  TBranch        *b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_29_Dn;   //!
  TBranch        *b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_30_Up;   //!
  TBranch        *b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_30_Dn;   //!
  TBranch        *b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_33_Up;   //!
  TBranch        *b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_33_Dn;   //!
  TBranch        *b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_34_Up;   //!
  TBranch        *b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_34_Dn;   //!
  TBranch        *b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_35_Up;   //!
  TBranch        *b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_35_Dn;   //!
  TBranch        *b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_36_Up;   //!
  TBranch        *b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_36_Dn;   //!
  TBranch        *b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_37_Up;   //!
  TBranch        *b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_37_Dn;   //!
  TBranch        *b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_38_Up;   //!
  TBranch        *b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_38_Dn;   //!
  TBranch        *b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_41_Up;   //!
  TBranch        *b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_41_Dn;   //!
  TBranch        *b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_42_Up;   //!
  TBranch        *b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_42_Dn;   //!
  TBranch        *b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_43_Up;   //!
  TBranch        *b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_43_Dn;   //!
  TBranch        *b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_44_Up;   //!
  TBranch        *b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_44_Dn;   //!
  TBranch        *b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_45_Up;   //!
  TBranch        *b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_45_Dn;   //!
  TBranch        *b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_46_Up;   //!
  TBranch        *b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_46_Dn;   //!
  TBranch        *b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_49_Up;   //!
  TBranch        *b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_49_Dn;   //!
  TBranch        *b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_50_Up;   //!
  TBranch        *b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_50_Dn;   //!
  TBranch        *b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_51_Up;   //!
  TBranch        *b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_51_Dn;   //!
  TBranch        *b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_52_Up;   //!
  TBranch        *b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_52_Dn;   //!
  TBranch        *b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_53_Up;   //!
  TBranch        *b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_53_Dn;   //!
  TBranch        *b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_54_Up;   //!
  TBranch        *b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_54_Dn;   //!
  TBranch        *b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_57_Up;   //!
  TBranch        *b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_57_Dn;   //!
  TBranch        *b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_58_Up;   //!
  TBranch        *b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_58_Dn;   //!
  TBranch        *b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_59_Up;   //!
  TBranch        *b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_59_Dn;   //!
  TBranch        *b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_60_Up;   //!
  TBranch        *b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_60_Dn;   //!
  TBranch        *b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_61_Up;   //!
  TBranch        *b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_61_Dn;   //!
  TBranch        *b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_62_Up;   //!
  TBranch        *b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_62_Dn;   //!
  TBranch        *b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_9_Up;   //!
  TBranch        *b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_9_Dn;   //!
  TBranch        *b_MM_EventWeight_Real_Mu_Pt_ND_FakesOS_Up;   //!
  TBranch        *b_MM_EventWeight_Real_Mu_Pt_ND_FakesOS_Dn;   //!
  TBranch        *b_MM_EventWeight_Real_Mu_Pt_Stat_1_Up;   //!
  TBranch        *b_MM_EventWeight_Real_Mu_Pt_Stat_1_Dn;   //!
  TBranch        *b_MM_EventWeight_Real_Mu_Pt_Stat_2_Up;   //!
  TBranch        *b_MM_EventWeight_Real_Mu_Pt_Stat_2_Dn;   //!
  TBranch        *b_MM_EventWeight_Real_Mu_Pt_Stat_3_Up;   //!
  TBranch        *b_MM_EventWeight_Real_Mu_Pt_Stat_3_Dn;   //!
  TBranch        *b_MM_EventWeight_Real_Mu_Pt_Stat_4_Up;   //!
  TBranch        *b_MM_EventWeight_Real_Mu_Pt_Stat_4_Dn;   //!
  TBranch        *b_MM_EventWeight_Real_Mu_Pt_Stat_5_Up;   //!
  TBranch        *b_MM_EventWeight_Real_Mu_Pt_Stat_5_Dn;   //!
  TBranch        *b_MM_EventWeight_Real_Mu_Pt_Stat_6_Up;   //!
  TBranch        *b_MM_EventWeight_Real_Mu_Pt_Stat_6_Dn;   //!
  TBranch        *b_MM_EventWeight_Real_Mu_Pt_Stat_7_Up;   //!
  TBranch        *b_MM_EventWeight_Real_Mu_Pt_Stat_7_Dn;   //!
  TBranch        *b_MM_EventWeight_Real_El_Pt_Stat_Grouped_Up;   //!
  TBranch        *b_MM_EventWeight_Real_El_Pt_Stat_Grouped_Dn;   //!
  TBranch        *b_MM_EventWeight_Real_El_Pt_Sys_Grouped_Up;   //!
  TBranch        *b_MM_EventWeight_Real_El_Pt_Sys_Grouped_Dn;   //!
  TBranch        *b_MM_EventWeight_Real_Mu_Pt_Stat_Grouped_Up;   //!
  TBranch        *b_MM_EventWeight_Real_Mu_Pt_Stat_Grouped_Dn;   //!
  TBranch        *b_MM_EventWeight_Real_Mu_Pt_Sys_Grouped_Up;   //!
  TBranch        *b_MM_EventWeight_Real_Mu_Pt_Sys_Grouped_Dn;   //!
  TBranch        *b_MM_EventWeight_Fake_El_Pt_Stat_Grouped_Up;   //!
  TBranch        *b_MM_EventWeight_Fake_El_Pt_Stat_Grouped_Dn;   //!
  TBranch        *b_MM_EventWeight_Fake_El_Pt_Sys_Grouped_Up;   //!
  TBranch        *b_MM_EventWeight_Fake_El_Pt_Sys_Grouped_Dn;   //!
  TBranch        *b_MM_EventWeight_Fake_Mu_Pt_Stat_Grouped_Up;   //!
  TBranch        *b_MM_EventWeight_Fake_Mu_Pt_Stat_Grouped_Dn;   //!
  TBranch        *b_MM_EventWeight_Fake_Mu_Pt_Sys_Grouped_Up;   //!
  TBranch        *b_MM_EventWeight_Fake_Mu_Pt_Sys_Grouped_Dn;   //!
  TBranch        *b_QMisID_MM_EventWeight;   //!
  TBranch        *b_QMisID_MM_EventWeight_Up;   //!
  TBranch        *b_QMisID_MM_EventWeight_Dn;   //!
  TBranch        *b_nTruthJets; //!
  TBranch        *b_nJets; //!
  TBranch        *b_jet_pt; //!
  TBranch        *b_jet_phi; //!
  TBranch        *b_jet_eta; //!
  TBranch        *b_jet_e; //!
  TBranch        *b_jet_passOR; //!
  TBranch        *b_jet_passJVT; //!
  TBranch        *b_jet_passTauOR; //!
  TBranch        *b_jet_flavor_weight_MV2c10; //!
  TBranch        *b_jet_flavor_truth_label; //!
  TBranch        *b_jet_flavor_truth_label_ghost; //!
  TBranch        *b_truth_jet_pt; //!
  TBranch        *b_truth_jet_phi; //!
  TBranch        *b_truth_jet_eta; //!
  TBranch        *b_truth_jet_m; //!
  TBranch        *b_truth_jet_Wcount; //!
  TBranch        *b_truth_jet_Zcount; //!
  TBranch        *b_truth_jet_Tcount; //!
  TBranch        *b_truth_jet_Hcount; //!
  TBranch        *b_truth_jet_BHadronCount; //!
  TBranch        *b_truth_jet_CHadronCount; //!

  /////////////////////////////
  // Histograms booking:
  ////////////////////////////

  // CutFlow Histograms
  h_2SS0tau_CutFlow = new TH1D("h_2SS0tau_CutFlow", ";Cut;Events", 11, -0.5, 10.5);
  h_2SS0tau_CutFlow_Raw = new TH1D("h_2SS0tau_CutFlow_Raw", ";Cut;Events", 11, -0.5, 10.5);

  // 1D Histograms...
  h_lep_Pt_0 = new TH1D("h_lep_Pt_0", "Leading lepton Pt;p_{T}(l_{0})[GeV];Events", 100, 0, 1000);

  //Add histogram to all1DHists...
  all1DHists.push_back(h_2SS0tau_CutFlow);
  all1DHists.push_back(h_2SS0tau_CutFlow_Raw);
  all1DHists.push_back(h_lep_Pt_0);


  // Load input fil...
  f = TFile::Open(inpFile.c_str());
  if (f == 0) {
    printf("Error: cannot open %s \n", inpFile.c_str());
    return;
  } else {
    printf("Success: %s opened \n", inpFile.c_str());
  }

  fChain = (TTree*)f->Get(syst.c_str());

  // Set object pointer
  //jet_flavor_truth_label_ghost = 0;
  //jet_flavor_weight_MV2c10 = 0;
  //jet_pT = 0;
  //jet_eta = 0;
  electron_ambiguityType = 0;
  electron_PromptLeptonIso_TagWeight = 0;
  electron_PromptLeptonVeto_TagWeight = 0;
  muon_PromptLeptonIso_TagWeight = 0;
  muon_PromptLeptonVeto_TagWeight = 0;
  jet_pt = 0;
  jet_phi = 0;
  jet_eta = 0;
  jet_e = 0;
  jet_passOR = 0;
  jet_passJVT = 0;
  jet_passTauOR = 0;
  jet_flavor_weight_MV2c10 = 0;
  jet_flavor_truth_label = 0;
  jet_flavor_truth_label_ghost = 0;
  truth_jet_pt = 0;
  truth_jet_phi = 0;
  truth_jet_eta = 0;
  truth_jet_m = 0;
  truth_jet_Wcount = 0;
  truth_jet_Zcount = 0;
  truth_jet_Tcount = 0;
  truth_jet_Hcount = 0;
  truth_jet_BHadronCount = 0;
  truth_jet_CHadronCount = 0;
    
  // Set branches addresses
  fChain->SetBranchAddress("is1L2Tau", &is1L2Tau, &b_is1L2Tau);
  fChain->SetBranchAddress("is2LSS0Tau", &is2LSS0Tau, &b_is2LSS0Tau);
  fChain->SetBranchAddress("pass_tight_2L", &pass_tight_2L, &b_pass_tight_2L);
  fChain->SetBranchAddress("pass_SS_2L", &pass_SS_2L, &b_pass_SS_2L);
  fChain->SetBranchAddress("is2LSS1Tau", &is2LSS1Tau, &b_is2LSS1Tau);
  fChain->SetBranchAddress("is2LOS1Tau", &is2LOS1Tau, &b_is2LOS1Tau);
  fChain->SetBranchAddress("is3L", &is3L, &b_is3L);
  fChain->SetBranchAddress("is3L1Tau", &is3L1Tau, &b_is3L1Tau);
  fChain->SetBranchAddress("is4L", &is4L, &b_is4L);
  fChain->SetBranchAddress("is2lLowNJet0Tau", &is2lLowNJet0Tau, &b_is2lLowNJet0Tau);
  fChain->SetBranchAddress("is2LSS0Tau_SR_Relaxed", &is2LSS0Tau_SR_Relaxed, &b_is2LSS0Tau_SR_Relaxed);
  fChain->SetBranchAddress("is2LSS1Tau_SR_Relaxed", &is2LSS1Tau_SR_Relaxed, &b_is2LSS1Tau_SR_Relaxed);
  fChain->SetBranchAddress("is3L0Tau_SR_Relaxed", &is3L0Tau_SR_Relaxed, &b_is3L0Tau_SR_Relaxed);
  fChain->SetBranchAddress("is3L0Tau_SR_Relaxed3j", &is3L0Tau_SR_Relaxed3j, &b_is3L0Tau_SR_Relaxed3j);
  fChain->SetBranchAddress("is4L_SR_VV_Relaxed", &is4L_SR_VV_Relaxed, &b_is4L_SR_VV_Relaxed);
  fChain->SetBranchAddress("is4L_SR_ttV_Relaxed", &is4L_SR_ttV_Relaxed, &b_is4L_SR_ttV_Relaxed);
  fChain->SetBranchAddress("is4L_SR_ttV_Zenriched", &is4L_SR_ttV_Zenriched, &b_is4L_SR_ttV_Zenriched);
  fChain->SetBranchAddress("is3L0Tau_CR_ttbar_Relaxed", &is3L0Tau_CR_ttbar_Relaxed, &b_is3L0Tau_CR_ttbar_Relaxed);
  fChain->SetBranchAddress("is3L0Tau_CR_VV_Relaxed", &is3L0Tau_CR_VV_Relaxed, &b_is3L0Tau_CR_VV_Relaxed);
  fChain->SetBranchAddress("is3L0Tau_CR_ttW_Relaxed", &is3L0Tau_CR_ttW_Relaxed, &b_is3L0Tau_CR_ttW_Relaxed);
  fChain->SetBranchAddress("is2L_VR_ttW_ICHEP", &is2L_VR_ttW_ICHEP, &b_is2L_VR_ttW_ICHEP);
  fChain->SetBranchAddress("is2L_VR_ttbarFakes_ICHEP", &is2L_VR_ttbarFakes_ICHEP, &b_is2L_VR_ttbarFakes_ICHEP);
  fChain->SetBranchAddress("is4lCRttZRelaxed", &is4lCRttZRelaxed, &b_is4lCRttZRelaxed);
  fChain->SetBranchAddress("is2LOS1Tau_CR_ttbar", &is2LOS1Tau_CR_ttbar, &b_is2LOS1Tau_CR_ttbar);
  fChain->SetBranchAddress("is2LOS1Tau_CR_ttZ", &is2LOS1Tau_CR_ttZ, &b_is2LOS1Tau_CR_ttZ);
  fChain->SetBranchAddress("is2lOS0TauBaseline", &is2lOS0TauBaseline, &b_is2lOS0TauBaseline);
  fChain->SetBranchAddress("is2lOS0TauBaseline_ee", &is2lOS0TauBaseline_ee, &b_is2lOS0TauBaseline_ee);
  fChain->SetBranchAddress("is2lOS0TauBaseline_mm", &is2lOS0TauBaseline_mm, &b_is2lOS0TauBaseline_mm);
  fChain->SetBranchAddress("is2lOS0TauBaseline_OF", &is2lOS0TauBaseline_OF, &b_is2lOS0TauBaseline_OF);
  fChain->SetBranchAddress("is2lOS0TauBaseline_me", &is2lOS0TauBaseline_me, &b_is2lOS0TauBaseline_me);
  fChain->SetBranchAddress("is2lOS0TauBaseline_em", &is2lOS0TauBaseline_em, &b_is2lOS0TauBaseline_em);
  fChain->SetBranchAddress("is2lSS0TauBaseline", &is2lSS0TauBaseline, &b_is2lSS0TauBaseline);
  fChain->SetBranchAddress("is2lSS0TauBaseline_ee", &is2lSS0TauBaseline_ee, &b_is2lSS0TauBaseline_ee);
  fChain->SetBranchAddress("is2lSS0TauBaseline_mm", &is2lSS0TauBaseline_mm, &b_is2lSS0TauBaseline_mm);
  fChain->SetBranchAddress("is2lSS0TauBaseline_OF", &is2lSS0TauBaseline_OF, &b_is2lSS0TauBaseline_OF);
  fChain->SetBranchAddress("is2lSS0TauBaseline_me", &is2lSS0TauBaseline_me, &b_is2lSS0TauBaseline_me);
  fChain->SetBranchAddress("is2lSS0TauBaseline_em", &is2lSS0TauBaseline_em, &b_is2lSS0TauBaseline_em);
  fChain->SetBranchAddress("is2lOS1TauBaseline", &is2lOS1TauBaseline, &b_is2lOS1TauBaseline);
  fChain->SetBranchAddress("is2lOS1TauBaseline_ee", &is2lOS1TauBaseline_ee, &b_is2lOS1TauBaseline_ee);
  fChain->SetBranchAddress("is2lOS1TauBaseline_mm", &is2lOS1TauBaseline_mm, &b_is2lOS1TauBaseline_mm);
  fChain->SetBranchAddress("is2lOS1TauBaseline_OF", &is2lOS1TauBaseline_OF, &b_is2lOS1TauBaseline_OF);
  fChain->SetBranchAddress("is2lOS1TauBaseline_me", &is2lOS1TauBaseline_me, &b_is2lOS1TauBaseline_me);
  fChain->SetBranchAddress("is2lOS1TauBaseline_em", &is2lOS1TauBaseline_em, &b_is2lOS1TauBaseline_em);
  fChain->SetBranchAddress("is2lSS1TauBaseline", &is2lSS1TauBaseline, &b_is2lSS1TauBaseline);
  fChain->SetBranchAddress("is2lSS1TauBaseline_ee", &is2lSS1TauBaseline_ee, &b_is2lSS1TauBaseline_ee);
  fChain->SetBranchAddress("is2lSS1TauBaseline_mm", &is2lSS1TauBaseline_mm, &b_is2lSS1TauBaseline_mm);
  fChain->SetBranchAddress("is2lSS1TauBaseline_OF", &is2lSS1TauBaseline_OF, &b_is2lSS1TauBaseline_OF);
  fChain->SetBranchAddress("is2lSS1TauBaseline_me", &is2lSS1TauBaseline_me, &b_is2lSS1TauBaseline_me);
  fChain->SetBranchAddress("is2lSS1TauBaseline_em", &is2lSS1TauBaseline_em, &b_is2lSS1TauBaseline_em);
  fChain->SetBranchAddress("is3lBaseline", &is3lBaseline, &b_is3lBaseline);
  fChain->SetBranchAddress("is3lBaselineNoZVeto", &is3lBaselineNoZVeto, &b_is3lBaselineNoZVeto);
  fChain->SetBranchAddress("isGNv02", &isGNv02, &b_isGNv02);
  fChain->SetBranchAddress("minOSMll", &minOSMll, &b_minOSMll);
  fChain->SetBranchAddress("minOSSFMll", &minOSSFMll, &b_minOSSFMll);
  fChain->SetBranchAddress("jet_flvWtOrdr_0_pt", &jet_flvWtOrdr_0_pt, &b_jet_flvWtOrdr_0_pt);
  fChain->SetBranchAddress("jet_flvWtOrdr_0_eta", &jet_flvWtOrdr_0_eta, &b_jet_flvWtOrdr_0_eta);
  fChain->SetBranchAddress("jet_flvWtOrdr_0_phi", &jet_flvWtOrdr_0_phi, &b_jet_flvWtOrdr_0_phi);
  fChain->SetBranchAddress("jet_flvWtOrdr_0_E", &jet_flvWtOrdr_0_E, &b_jet_flvWtOrdr_0_E);
  fChain->SetBranchAddress("jet_flvWtOrdr_0_mv2c10", &jet_flvWtOrdr_0_mv2c10, &b_jet_flvWtOrdr_0_mv2c10);
  fChain->SetBranchAddress("jet_flvWtOrdr_1_pt", &jet_flvWtOrdr_1_pt, &b_jet_flvWtOrdr_1_pt);
  fChain->SetBranchAddress("jet_flvWtOrdr_1_eta", &jet_flvWtOrdr_1_eta, &b_jet_flvWtOrdr_1_eta);
  fChain->SetBranchAddress("jet_flvWtOrdr_1_phi", &jet_flvWtOrdr_1_phi, &b_jet_flvWtOrdr_1_phi);
  fChain->SetBranchAddress("jet_flvWtOrdr_1_E", &jet_flvWtOrdr_1_E, &b_jet_flvWtOrdr_1_E);
  fChain->SetBranchAddress("jet_flvWtOrdr_1_mv2c10", &jet_flvWtOrdr_1_mv2c10, &b_jet_flvWtOrdr_1_mv2c10);
  fChain->SetBranchAddress("jet_flvWtOrdr_2_pt", &jet_flvWtOrdr_2_pt, &b_jet_flvWtOrdr_2_pt);
  fChain->SetBranchAddress("jet_flvWtOrdr_2_eta", &jet_flvWtOrdr_2_eta, &b_jet_flvWtOrdr_2_eta);
  fChain->SetBranchAddress("jet_flvWtOrdr_2_phi", &jet_flvWtOrdr_2_phi, &b_jet_flvWtOrdr_2_phi);
  fChain->SetBranchAddress("jet_flvWtOrdr_2_E", &jet_flvWtOrdr_2_E, &b_jet_flvWtOrdr_2_E);
  fChain->SetBranchAddress("jet_flvWtOrdr_2_mv2c10", &jet_flvWtOrdr_2_mv2c10, &b_jet_flvWtOrdr_2_mv2c10);
  fChain->SetBranchAddress("DEtall01", &DEtall01, &b_DEtall01);
  fChain->SetBranchAddress("DEtall02", &DEtall02, &b_DEtall02);
  fChain->SetBranchAddress("DEtall12", &DEtall12, &b_DEtall12);
  fChain->SetBranchAddress("lep_flavour", &lep_flavour, &b_lep_flavour);
  fChain->SetBranchAddress("jet_flavor", &jet_flavor, &b_jet_flavor);
  fChain->SetBranchAddress("total_charge_2lSS", &total_charge_2lSS, &b_total_charge_2lSS);
  fChain->SetBranchAddress("isExtConv", &isExtConv, &b_isExtConv);
  fChain->SetBranchAddress("isIntConv", &isIntConv, &b_isIntConv);
  fChain->SetBranchAddress("max_eta", &max_eta, &b_max_eta);
  fChain->SetBranchAddress("SDRll", &SDRll, &b_SDRll);
  fChain->SetBranchAddress("DeltaR_max_lep", &DeltaR_max_lep, &b_DeltaR_max_lep);
  fChain->SetBranchAddress("DPhi_max_lep_met", &DPhi_max_lep_met, &b_DPhi_max_lep_met);
  fChain->SetBranchAddress("DeltaR_min_lep_jet", &DeltaR_min_lep_jet, &b_DeltaR_min_lep_jet);
  fChain->SetBranchAddress("minDeltaR_LJ_0", &minDeltaR_LJ_0, &b_minDeltaR_LJ_0);
  fChain->SetBranchAddress("minDeltaR_LJ_1", &minDeltaR_LJ_1, &b_minDeltaR_LJ_1);
  fChain->SetBranchAddress("minDeltaR_LJ_2", &minDeltaR_LJ_2, &b_minDeltaR_LJ_2);
  fChain->SetBranchAddress("Meff", &Meff, &b_Meff);
  fChain->SetBranchAddress("Mjj_closeW", &Mjj_closeW, &b_Mjj_closeW);
  fChain->SetBranchAddress("DRlb_h", &DRlb_h, &b_DRlb_h);
  fChain->SetBranchAddress("ptlb_h", &ptlb_h, &b_ptlb_h);
  fChain->SetBranchAddress("DeltaRLeadjets", &DeltaRLeadjets, &b_DeltaRLeadjets);
  fChain->SetBranchAddress("jet1pTbin", &jet1pTbin, &b_jet1pTbin);
  fChain->SetBranchAddress("jet2pTbin", &jet2pTbin, &b_jet2pTbin);
  fChain->SetBranchAddress("jet3pTbin", &jet3pTbin, &b_jet3pTbin);
  fChain->SetBranchAddress("jet3etabin", &jet3etabin, &b_jet3etabin);
  fChain->SetBranchAddress("jet1MV2c10_bin", &jet1MV2c10_bin, &b_jet1MV2c10_bin);
  fChain->SetBranchAddress("jet2MV2c10_bin", &jet2MV2c10_bin, &b_jet2MV2c10_bin);
  fChain->SetBranchAddress("MvisH", &MvisH, &b_MvisH);
  fChain->SetBranchAddress("MvisnonH", &MvisnonH, &b_MvisnonH);
  fChain->SetBranchAddress("lep_PtVarCone20Rel_1", &lep_PtVarCone20Rel_1, &b_lep_PtVarCone20Rel_1);
  fChain->SetBranchAddress("DeltaRLeadLepClosestJet", &DeltaRLeadLepClosestJet, &b_DeltaRLeadLepClosestJet);
  fChain->SetBranchAddress("Ml2l3met", &Ml2l3met, &b_Ml2l3met);
  fChain->SetBranchAddress("FSF_4L_tot", &FSF_4L_tot, &b_FSF_4L_tot);
  fChain->SetBranchAddress("FSF_4L_tot_up", &FSF_4L_tot_up, &b_FSF_4L_tot_up);
  fChain->SetBranchAddress("FSF_4L_tot_down", &FSF_4L_tot_down, &b_FSF_4L_tot_down);
  fChain->SetBranchAddress("FSF_category", &FSF_category, &b_FSF_category);
  fChain->SetBranchAddress("jet2_pt", &jet2_pt, &b_jet2_pt);
  fChain->SetBranchAddress("jet2_eta", &jet2_eta, &b_jet2_eta);
  fChain->SetBranchAddress("jet2_phi", &jet2_phi, &b_jet2_phi);
  fChain->SetBranchAddress("jet3_pt", &jet3_pt, &b_jet3_pt);
  fChain->SetBranchAddress("jet3_eta", &jet3_eta, &b_jet3_eta);
  fChain->SetBranchAddress("jet3_phi", &jet3_phi, &b_jet3_phi);
  fChain->SetBranchAddress("jet0_tagWeightBin", &jet0_tagWeightBin, &b_jet0_tagWeightBin);
  fChain->SetBranchAddress("jet1_tagWeightBin", &jet1_tagWeightBin, &b_jet1_tagWeightBin);
  fChain->SetBranchAddress("jet2_tagWeightBin", &jet2_tagWeightBin, &b_jet2_tagWeightBin);
  fChain->SetBranchAddress("jet3_tagWeightBin", &jet3_tagWeightBin, &b_jet3_tagWeightBin);
  fChain->SetBranchAddress("N_el", &N_el, &b_N_el);
  fChain->SetBranchAddress("N_mu", &N_mu, &b_N_mu);
  fChain->SetBranchAddress("tempDPl0t", &tempDPl0t, &b_tempDPl0t);
  fChain->SetBranchAddress("tempDPl1t", &tempDPl1t, &b_tempDPl1t);
  fChain->SetBranchAddress("tempDP2l", &tempDP2l, &b_tempDP2l);
  fChain->SetBranchAddress("tempDPlt_closest", &tempDPlt_closest, &b_tempDPlt_closest);
  fChain->SetBranchAddress("tempDPlt_furthest", &tempDPlt_furthest, &b_tempDPlt_furthest);
  fChain->SetBranchAddress("DeltaRSubLepClosestJet", &DeltaRSubLepClosestJet, &b_DeltaRSubLepClosestJet);
  fChain->SetBranchAddress("SignProduct2sslt", &SignProduct2sslt, &b_SignProduct2sslt);
  fChain->SetBranchAddress("TransverseMassLeadLepMET", &TransverseMassLeadLepMET, &b_TransverseMassLeadLepMET);
  fChain->SetBranchAddress("LD_HTmiss_ETmiss", &LD_HTmiss_ETmiss, &b_LD_HTmiss_ETmiss);
  fChain->SetBranchAddress("Mjj_nonb", &Mjj_nonb, &b_Mjj_nonb);
  fChain->SetBranchAddress("Mtop", &Mtop, &b_Mtop);
  fChain->SetBranchAddress("MtopW", &MtopW, &b_MtopW);
  fChain->SetBranchAddress("temptopoetcone20overpTlep0", &temptopoetcone20overpTlep0, &b_temptopoetcone20overpTlep0);
  fChain->SetBranchAddress("temptopoetcone20overpTlep1", &temptopoetcone20overpTlep1, &b_temptopoetcone20overpTlep1);
  fChain->SetBranchAddress("tempptvarcone20overpTlep0", &tempptvarcone20overpTlep0, &b_tempptvarcone20overpTlep0);
  fChain->SetBranchAddress("tempptvarcone20overpTlep1", &tempptvarcone20overpTlep1, &b_tempptvarcone20overpTlep1);
  fChain->SetBranchAddress("tau_tight_0", &tau_tight_0, &b_tau_tight_0);
  fChain->SetBranchAddress("tau_tight_1", &tau_tight_1, &b_tau_tight_1);
  fChain->SetBranchAddress("tau_btag70_0", &tau_btag70_0, &b_tau_btag70_0);
  fChain->SetBranchAddress("tau_btag70_1", &tau_btag70_1, &b_tau_btag70_1);
  fChain->SetBranchAddress("tau_truth_0", &tau_truth_0, &b_tau_truth_0);
  fChain->SetBranchAddress("tau_truth_1", &tau_truth_1, &b_tau_truth_1);
  fChain->SetBranchAddress("htjets_1l2tau", &htjets_1l2tau, &b_htjets_1l2tau);
  fChain->SetBranchAddress("jjdrmin_1l2tau", &jjdrmin_1l2tau, &b_jjdrmin_1l2tau);
  fChain->SetBranchAddress("mtautau_1l2tau", &mtautau_1l2tau, &b_mtautau_1l2tau);
  fChain->SetBranchAddress("njets_1l2tau", &njets_1l2tau, &b_njets_1l2tau);
  fChain->SetBranchAddress("nbjets_1l2tau", &nbjets_1l2tau, &b_nbjets_1l2tau);
  fChain->SetBranchAddress("DRlj00", &DRlj00, &b_DRlj00);
  fChain->SetBranchAddress("DRjj01", &DRjj01, &b_DRjj01);
  fChain->SetBranchAddress("min_DRl0j", &min_DRl0j, &b_min_DRl0j);
  fChain->SetBranchAddress("min_DRl1j", &min_DRl1j, &b_min_DRl1j);
  fChain->SetBranchAddress("min_DRl2j", &min_DRl2j, &b_min_DRl2j);
  fChain->SetBranchAddress("min_DRl0b", &min_DRl0b, &b_min_DRl0b);
  fChain->SetBranchAddress("min_DRl1b", &min_DRl1b, &b_min_DRl1b);
  fChain->SetBranchAddress("min_DRl2b", &min_DRl2b, &b_min_DRl2b);
  fChain->SetBranchAddress("min_DRlj_new", &min_DRlj_new, &b_min_DRlj_new);
  fChain->SetBranchAddress("nJets_OR_T_overflow", &nJets_OR_T_overflow, &b_nJets_OR_T_overflow);
  fChain->SetBranchAddress("nJets_OR_T_MV2c10_70_overflow", &nJets_OR_T_MV2c10_70_overflow, &b_nJets_OR_T_MV2c10_70_overflow);
  fChain->SetBranchAddress("DPhij0MET", &DPhij0MET, &b_DPhij0MET);
  fChain->SetBranchAddress("lead_BjetPt", &lead_BjetPt, &b_lead_BjetPt);
  fChain->SetBranchAddress("lead_jet_sumBEff", &lead_jet_sumBEff, &b_lead_jet_sumBEff);
  fChain->SetBranchAddress("sublead_jet_sumBEff", &sublead_jet_sumBEff, &b_sublead_jet_sumBEff);
  fChain->SetBranchAddress("lead_jet_is_lead_antitag", &lead_jet_is_lead_antitag, &b_lead_jet_is_lead_antitag);
  fChain->SetBranchAddress("lead_jet_is_lead_tag", &lead_jet_is_lead_tag, &b_lead_jet_is_lead_tag);
  fChain->SetBranchAddress("lep_Pt_1_new", &lep_Pt_1_new, &b_lep_Pt_1_new);
  fChain->SetBranchAddress("lep_Pt_2_new", &lep_Pt_2_new, &b_lep_Pt_2_new);
  fChain->SetBranchAddress("lep_ID_1_new", &lep_ID_1_new, &b_lep_ID_1_new);
  fChain->SetBranchAddress("lep_ID_2_new", &lep_ID_2_new, &b_lep_ID_2_new);
  fChain->SetBranchAddress("EventWeight", &EventWeight, &b_EventWeight);
  fChain->SetBranchAddress("lep_isTightSelected_0", &lep_isTightSelected_0, &b_lep_isTightSelected_0);
  fChain->SetBranchAddress("lep_isTightSelected_1", &lep_isTightSelected_1, &b_lep_isTightSelected_1);
  fChain->SetBranchAddress("lep_isTightSelected_2", &lep_isTightSelected_2, &b_lep_isTightSelected_2);
  fChain->SetBranchAddress("lep_isTightSelected_3", &lep_isTightSelected_3, &b_lep_isTightSelected_3);
  fChain->SetBranchAddress("lep_isTightSelected_2LOS1Tau_0", &lep_isTightSelected_2LOS1Tau_0, &b_lep_isTightSelected_2LOS1Tau_0);
  fChain->SetBranchAddress("lep_isTightSelected_2LOS1Tau_1", &lep_isTightSelected_2LOS1Tau_1, &b_lep_isTightSelected_2LOS1Tau_1);
  fChain->SetBranchAddress("lep_isTightSelectedAndPt_0", &lep_isTightSelectedAndPt_0, &b_lep_isTightSelectedAndPt_0);
  fChain->SetBranchAddress("lep_isTightSelectedAndPt_1", &lep_isTightSelectedAndPt_1, &b_lep_isTightSelectedAndPt_1);
  fChain->SetBranchAddress("lep_isTightSelectedAndPt_2", &lep_isTightSelectedAndPt_2, &b_lep_isTightSelectedAndPt_2);
  fChain->SetBranchAddress("lep_isTightSelectedAndPt_3", &lep_isTightSelectedAndPt_3, &b_lep_isTightSelectedAndPt_3);
  fChain->SetBranchAddress("lep_isTightSelectedAndPt_2LOS1Tau_0", &lep_isTightSelectedAndPt_2LOS1Tau_0, &b_lep_isTightSelectedAndPt_2LOS1Tau_0);
  fChain->SetBranchAddress("lep_isTightSelectedAndPt_2LOS1Tau_1", &lep_isTightSelectedAndPt_2LOS1Tau_1, &b_lep_isTightSelectedAndPt_2LOS1Tau_1);
  //fChain->SetBranchAddress("jet_flavor_truth_label_ghost", &jet_flavor_truth_label_ghost, &b_jet_flavor_truth_label_ghost);
  //fChain->SetBranchAddress("jet_flavor_weight_MV2c10", &jet_flavor_weight_MV2c10, &b_jet_flavor_weight_MV2c10);
  //fChain->SetBranchAddress("jet_pT", &jet_pT, &b_jet_pT);
  //fChain->SetBranchAddress("jet_eta", &jet_eta, &b_jet_eta);
  fChain->SetBranchAddress("MVA2lSSMarseille_weight_ttV", &MVA2lSSMarseille_weight_ttV, &b_MVA2lSSMarseille_weight_ttV);
  fChain->SetBranchAddress("MVA2lSSMarseille_weight_ttbar", &MVA2lSSMarseille_weight_ttbar, &b_MVA2lSSMarseille_weight_ttbar);
  fChain->SetBranchAddress("MVA2lSSMarseille_weight_ttV_2", &MVA2lSSMarseille_weight_ttV_2, &b_MVA2lSSMarseille_weight_ttV_2);
  fChain->SetBranchAddress("MVA2lSSMarseille_weight_ttbar_2", &MVA2lSSMarseille_weight_ttbar_2, &b_MVA2lSSMarseille_weight_ttbar_2);
  fChain->SetBranchAddress("MVA3lCERN_weight_ttH", &MVA3lCERN_weight_ttH, &b_MVA3lCERN_weight_ttH);
  fChain->SetBranchAddress("MVA3lCERN_weight_ttW", &MVA3lCERN_weight_ttW, &b_MVA3lCERN_weight_ttW);
  fChain->SetBranchAddress("MVA3lCERN_weight_ttZ", &MVA3lCERN_weight_ttZ, &b_MVA3lCERN_weight_ttZ);
  fChain->SetBranchAddress("MVA3lCERN_weight_VV", &MVA3lCERN_weight_VV, &b_MVA3lCERN_weight_VV);
  fChain->SetBranchAddress("MVA3lCERN_weight_ttbar", &MVA3lCERN_weight_ttbar, &b_MVA3lCERN_weight_ttbar);
  fChain->SetBranchAddress("MVA3lCERN_category", &MVA3lCERN_category, &b_MVA3lCERN_category);
  fChain->SetBranchAddress("isLep2SS0tau_ee_QMisID", &isLep2SS0tau_ee_QMisID, &b_isLep2SS0tau_ee_QMisID);
  fChain->SetBranchAddress("isLep2SS0tau_OF_QMisID", &isLep2SS0tau_OF_QMisID, &b_isLep2SS0tau_OF_QMisID);
  fChain->SetBranchAddress("is2lttW_VR_ICHEP_QMisID", &is2lttW_VR_ICHEP_QMisID, &b_is2lttW_VR_ICHEP_QMisID);
  fChain->SetBranchAddress("total_charge_QMisID", &total_charge_QMisID, &b_total_charge_QMisID);
  fChain->SetBranchAddress("QMisID_EventWeight", &QMisID_EventWeight, &b_QMisID_EventWeight);
  fChain->SetBranchAddress("QMisID_EventWeight_Up", &QMisID_EventWeight_Up, &b_QMisID_EventWeight_Up);
  fChain->SetBranchAddress("QMisID_EventWeight_Dn", &QMisID_EventWeight_Dn, &b_QMisID_EventWeight_Dn);
  fChain->SetBranchAddress("isLep2SS1tau_QMisID", &isLep2SS1tau_QMisID, &b_isLep2SS1tau_QMisID);
  fChain->SetBranchAddress("total_charge_2ltau_QMisID", &total_charge_2ltau_QMisID, &b_total_charge_2ltau_QMisID);
  fChain->SetBranchAddress("QMisID_2ltau_EventWeight", &QMisID_2ltau_EventWeight, &b_QMisID_2ltau_EventWeight);
  fChain->SetBranchAddress("QMisID_2ltau_EventWeight_Up", &QMisID_2ltau_EventWeight_Up, &b_QMisID_2ltau_EventWeight_Up);
  fChain->SetBranchAddress("QMisID_2ltau_EventWeight_Dn", &QMisID_2ltau_EventWeight_Dn, &b_QMisID_2ltau_EventWeight_Dn);
  fChain->SetBranchAddress("passTrigger", &passTrigger, &b_passTrigger);
  fChain->SetBranchAddress("scale_nom", &scale_nom, &b_scale_nom);
  fChain->SetBranchAddress("scale_varA14up", &scale_varA14up, &b_scale_varA14up);
  fChain->SetBranchAddress("scale_varA14down", &scale_varA14down, &b_scale_varA14down);
  fChain->SetBranchAddress("mcWeightOrg", &mcWeightOrg, &b_mcWeightOrg);
  fChain->SetBranchAddress("pileupEventWeight_090", &pileupEventWeight_090, &b_pileupEventWeight_090);
  fChain->SetBranchAddress("MV2c10_60_EventWeight", &MV2c10_60_EventWeight, &b_MV2c10_60_EventWeight);
  fChain->SetBranchAddress("MV2c10_70_EventWeight", &MV2c10_70_EventWeight, &b_MV2c10_70_EventWeight);
  fChain->SetBranchAddress("MV2c10_77_EventWeight", &MV2c10_77_EventWeight, &b_MV2c10_77_EventWeight);
  fChain->SetBranchAddress("MV2c10_85_EventWeight", &MV2c10_85_EventWeight, &b_MV2c10_85_EventWeight);
  fChain->SetBranchAddress("MV2c10_Continuous_EventWeight", &MV2c10_Continuous_EventWeight, &b_MV2c10_Continuous_EventWeight);
  fChain->SetBranchAddress("bTagSF_weight_Continuous", &bTagSF_weight_Continuous, &b_bTagSF_weight_Continuous);
  fChain->SetBranchAddress("JVT_EventWeight", &JVT_EventWeight, &b_JVT_EventWeight);
  fChain->SetBranchAddress("SherpaNJetWeight", &SherpaNJetWeight, &b_SherpaNJetWeight);
  fChain->SetBranchAddress("EventNumber", &EventNumber, &b_EventNumber);
  fChain->SetBranchAddress("RunNumber", &RunNumber, &b_RunNumber);
  fChain->SetBranchAddress("lbn", &lbn, &b_lbn);
  fChain->SetBranchAddress("bcid", &bcid, &b_bcid);
  fChain->SetBranchAddress("passEventCleaning", &passEventCleaning, &b_passEventCleaning);
  fChain->SetBranchAddress("m_vxp_x", &m_vxp_x, &b_m_vxp_x);
  fChain->SetBranchAddress("m_vxp_y", &m_vxp_y, &b_m_vxp_y);
  fChain->SetBranchAddress("m_vxp_z", &m_vxp_z, &b_m_vxp_z);
  fChain->SetBranchAddress("higgs_pt", &higgs_pt, &b_higgs_pt);
  fChain->SetBranchAddress("higgs_eta", &higgs_eta, &b_higgs_eta);
  fChain->SetBranchAddress("higgs_phi", &higgs_phi, &b_higgs_phi);
  fChain->SetBranchAddress("higgs_E", &higgs_E, &b_higgs_E);
  fChain->SetBranchAddress("higgs_rapidity", &higgs_rapidity, &b_higgs_rapidity);
  fChain->SetBranchAddress("top_pt", &top_pt, &b_top_pt);
  fChain->SetBranchAddress("top_eta", &top_eta, &b_top_eta);
  fChain->SetBranchAddress("top_phi", &top_phi, &b_top_phi);
  fChain->SetBranchAddress("top_E", &top_E, &b_top_E);
  fChain->SetBranchAddress("antitop_pt", &antitop_pt, &b_antitop_pt);
  fChain->SetBranchAddress("antitop_eta", &antitop_eta, &b_antitop_eta);
  fChain->SetBranchAddress("antitop_phi", &antitop_phi, &b_antitop_phi);
  fChain->SetBranchAddress("antitop_E", &antitop_E, &b_antitop_E);
  fChain->SetBranchAddress("RunYear", &RunYear, &b_RunYear);
  fChain->SetBranchAddress("HF_Classification", &HF_Classification, &b_HF_Classification);
  fChain->SetBranchAddress("HF_ClassificationTop", &HF_ClassificationTop, &b_HF_ClassificationTop);
  fChain->SetBranchAddress("DLF_Classification", &DLF_Classification, &b_DLF_Classification);
  fChain->SetBranchAddress("ttbar_HF_EventType", &ttbar_HF_EventType, &b_ttbar_HF_EventType);
  fChain->SetBranchAddress("higgsDecayMode", &higgsDecayMode, &b_higgsDecayMode);
  fChain->SetBranchAddress("mc_channel_number", &mc_channel_number, &b_mc_channel_number);
  fChain->SetBranchAddress("mc_isAFII", &mc_isAFII, &b_mc_isAFII);
  fChain->SetBranchAddress("averageIntPerXing_uncorr", &averageIntPerXing_uncorr, &b_averageIntPerXing_uncorr);
  fChain->SetBranchAddress("averageIntPerXing", &averageIntPerXing, &b_averageIntPerXing);
  fChain->SetBranchAddress("actualIntPerXing", &actualIntPerXing, &b_actualIntPerXing);
  fChain->SetBranchAddress("m_vxp_n", &m_vxp_n, &b_m_vxp_n);
  fChain->SetBranchAddress("m_vxpu_n", &m_vxpu_n, &b_m_vxpu_n);
  fChain->SetBranchAddress("MET_RefFinal_et", &MET_RefFinal_et, &b_MET_RefFinal_et);
  fChain->SetBranchAddress("MET_RefFinal_phi", &MET_RefFinal_phi, &b_MET_RefFinal_phi);
  fChain->SetBranchAddress("MET_RefFinal_sumet", &MET_RefFinal_sumet, &b_MET_RefFinal_sumet);
  fChain->SetBranchAddress("m_hasFSRPhotonLargeDeltaR", &m_hasFSRPhotonLargeDeltaR, &b_m_hasFSRPhotonLargeDeltaR);
  fChain->SetBranchAddress("m_hasMEphoton", &m_hasMEphoton, &b_m_hasMEphoton);
  fChain->SetBranchAddress("m_hasMEphoton_DRgt02_nonhad", &m_hasMEphoton_DRgt02_nonhad, &b_m_hasMEphoton_DRgt02_nonhad);
  fChain->SetBranchAddress("m_MEphoton_OLtty_keepEvent", &m_MEphoton_OLtty_keepEvent, &b_m_MEphoton_OLtty_keepEvent);
  fChain->SetBranchAddress("m_MEphoton_OLtty_cat1", &m_MEphoton_OLtty_cat1, &b_m_MEphoton_OLtty_cat1);
  fChain->SetBranchAddress("m_MEphoton_OLtty_cat2", &m_MEphoton_OLtty_cat2, &b_m_MEphoton_OLtty_cat2);
  fChain->SetBranchAddress("m_MEphoton_OLtty_cat3", &m_MEphoton_OLtty_cat3, &b_m_MEphoton_OLtty_cat3);
  fChain->SetBranchAddress("m_MEphoton_OLtty_cat4", &m_MEphoton_OLtty_cat4, &b_m_MEphoton_OLtty_cat4);
  fChain->SetBranchAddress("m_MEphoton_pt", &m_MEphoton_pt, &b_m_MEphoton_pt);
  fChain->SetBranchAddress("m_MEphoton_eta", &m_MEphoton_eta, &b_m_MEphoton_eta);
  fChain->SetBranchAddress("m_MEphoton_phi", &m_MEphoton_phi, &b_m_MEphoton_phi);
  fChain->SetBranchAddress("m_MEphoton_mother_pt", &m_MEphoton_mother_pt, &b_m_MEphoton_mother_pt);
  fChain->SetBranchAddress("m_MEphoton_mother_eta", &m_MEphoton_mother_eta, &b_m_MEphoton_mother_eta);
  fChain->SetBranchAddress("m_MEphoton_mother_phi", &m_MEphoton_mother_phi, &b_m_MEphoton_mother_phi);
  fChain->SetBranchAddress("m_MEphoton_mother_pdgId", &m_MEphoton_mother_pdgId, &b_m_MEphoton_mother_pdgId);
  fChain->SetBranchAddress("HLT_2e12_lhloose_L12EM10VH", &HLT_2e12_lhloose_L12EM10VH, &b_HLT_2e12_lhloose_L12EM10VH);
  fChain->SetBranchAddress("HLT_2e12_lhvloose_nod0_L12EM10VH", &HLT_2e12_lhvloose_nod0_L12EM10VH, &b_HLT_2e12_lhvloose_nod0_L12EM10VH);
  fChain->SetBranchAddress("HLT_2e17_lhvloose_nod0", &HLT_2e17_lhvloose_nod0, &b_HLT_2e17_lhvloose_nod0);
  fChain->SetBranchAddress("HLT_2e24_lhvloose_nod0", &HLT_2e24_lhvloose_nod0, &b_HLT_2e24_lhvloose_nod0);
  fChain->SetBranchAddress("HLT_e120_lhloose", &HLT_e120_lhloose, &b_HLT_e120_lhloose);
  fChain->SetBranchAddress("HLT_e140_lhloose", &HLT_e140_lhloose, &b_HLT_e140_lhloose);
  fChain->SetBranchAddress("HLT_e140_lhloose_nod0", &HLT_e140_lhloose_nod0, &b_HLT_e140_lhloose_nod0);
  fChain->SetBranchAddress("HLT_e17_lhloose_mu14", &HLT_e17_lhloose_mu14, &b_HLT_e17_lhloose_mu14);
  fChain->SetBranchAddress("HLT_e17_lhloose_nod0_mu14", &HLT_e17_lhloose_nod0_mu14, &b_HLT_e17_lhloose_nod0_mu14);
  fChain->SetBranchAddress("HLT_e24_lhmedium_L1EM18VH", &HLT_e24_lhmedium_L1EM18VH, &b_HLT_e24_lhmedium_L1EM18VH);
  fChain->SetBranchAddress("HLT_e24_lhmedium_L1EM20VH", &HLT_e24_lhmedium_L1EM20VH, &b_HLT_e24_lhmedium_L1EM20VH);
  fChain->SetBranchAddress("HLT_e24_lhmedium_nod0_L1EM20VH", &HLT_e24_lhmedium_nod0_L1EM20VH, &b_HLT_e24_lhmedium_nod0_L1EM20VH);
  fChain->SetBranchAddress("HLT_e26_lhtight_iloose", &HLT_e26_lhtight_iloose, &b_HLT_e26_lhtight_iloose);
  fChain->SetBranchAddress("HLT_e26_lhtight_nod0_ivarloose", &HLT_e26_lhtight_nod0_ivarloose, &b_HLT_e26_lhtight_nod0_ivarloose);
  fChain->SetBranchAddress("HLT_e60_lhmedium", &HLT_e60_lhmedium, &b_HLT_e60_lhmedium);
  fChain->SetBranchAddress("HLT_e60_lhmedium_nod0", &HLT_e60_lhmedium_nod0, &b_HLT_e60_lhmedium_nod0);
  fChain->SetBranchAddress("HLT_e60_medium", &HLT_e60_medium, &b_HLT_e60_medium);
  fChain->SetBranchAddress("HLT_e7_lhmedium_mu24", &HLT_e7_lhmedium_mu24, &b_HLT_e7_lhmedium_mu24);
  fChain->SetBranchAddress("HLT_e7_medium_mu24", &HLT_e7_medium_mu24, &b_HLT_e7_medium_mu24);
  fChain->SetBranchAddress("HLT_mu18_mu8noL1", &HLT_mu18_mu8noL1, &b_HLT_mu18_mu8noL1);
  fChain->SetBranchAddress("HLT_mu20_iloose_L1MU15", &HLT_mu20_iloose_L1MU15, &b_HLT_mu20_iloose_L1MU15);
  fChain->SetBranchAddress("HLT_mu20_mu8noL1", &HLT_mu20_mu8noL1, &b_HLT_mu20_mu8noL1);
  fChain->SetBranchAddress("HLT_mu22_mu8noL1", &HLT_mu22_mu8noL1, &b_HLT_mu22_mu8noL1);
  fChain->SetBranchAddress("HLT_mu24_imedium", &HLT_mu24_imedium, &b_HLT_mu24_imedium);
  fChain->SetBranchAddress("HLT_mu24_ivarloose", &HLT_mu24_ivarloose, &b_HLT_mu24_ivarloose);
  fChain->SetBranchAddress("HLT_mu24_ivarmedium", &HLT_mu24_ivarmedium, &b_HLT_mu24_ivarmedium);
  fChain->SetBranchAddress("HLT_mu26_imedium", &HLT_mu26_imedium, &b_HLT_mu26_imedium);
  fChain->SetBranchAddress("HLT_mu26_ivarmedium", &HLT_mu26_ivarmedium, &b_HLT_mu26_ivarmedium);
  fChain->SetBranchAddress("HLT_mu50", &HLT_mu50, &b_HLT_mu50);
  fChain->SetBranchAddress("HLT_2e17_lhvloose_nod0_L12EM15VHI", &HLT_2e17_lhvloose_nod0_L12EM15VHI, &b_HLT_2e17_lhvloose_nod0_L12EM15VHI);
  fChain->SetBranchAddress("lep_promptLeptonInput_sv1_jf_ntrkv_0", &lep_promptLeptonInput_sv1_jf_ntrkv_0, &b_lep_promptLeptonInput_sv1_jf_ntrkv_0);
  fChain->SetBranchAddress("lep_promptLeptonInput_sv1_jf_ntrkv_1", &lep_promptLeptonInput_sv1_jf_ntrkv_1, &b_lep_promptLeptonInput_sv1_jf_ntrkv_1);
  fChain->SetBranchAddress("lep_promptLeptonInput_sv1_jf_ntrkv_2", &lep_promptLeptonInput_sv1_jf_ntrkv_2, &b_lep_promptLeptonInput_sv1_jf_ntrkv_2);
  fChain->SetBranchAddress("lep_promptLeptonInput_TrackJetNTrack_0", &lep_promptLeptonInput_TrackJetNTrack_0, &b_lep_promptLeptonInput_TrackJetNTrack_0);
  fChain->SetBranchAddress("lep_promptLeptonInput_TrackJetNTrack_1", &lep_promptLeptonInput_TrackJetNTrack_1, &b_lep_promptLeptonInput_TrackJetNTrack_1);
  fChain->SetBranchAddress("lep_promptLeptonInput_TrackJetNTrack_2", &lep_promptLeptonInput_TrackJetNTrack_2, &b_lep_promptLeptonInput_TrackJetNTrack_2);
  fChain->SetBranchAddress("lep_promptLeptonInput_DL1mu_0", &lep_promptLeptonInput_DL1mu_0, &b_lep_promptLeptonInput_DL1mu_0);
  fChain->SetBranchAddress("lep_promptLeptonInput_DL1mu_1", &lep_promptLeptonInput_DL1mu_1, &b_lep_promptLeptonInput_DL1mu_1);
  fChain->SetBranchAddress("lep_promptLeptonInput_DL1mu_2", &lep_promptLeptonInput_DL1mu_2, &b_lep_promptLeptonInput_DL1mu_2);
  fChain->SetBranchAddress("lep_promptLeptonInput_DRlj_0", &lep_promptLeptonInput_DRlj_0, &b_lep_promptLeptonInput_DRlj_0);
  fChain->SetBranchAddress("lep_promptLeptonInput_DRlj_1", &lep_promptLeptonInput_DRlj_1, &b_lep_promptLeptonInput_DRlj_1);
  fChain->SetBranchAddress("lep_promptLeptonInput_DRlj_2", &lep_promptLeptonInput_DRlj_2, &b_lep_promptLeptonInput_DRlj_2);
  fChain->SetBranchAddress("lep_promptLeptonInput_LepJetPtFrac_0", &lep_promptLeptonInput_LepJetPtFrac_0, &b_lep_promptLeptonInput_LepJetPtFrac_0);
  fChain->SetBranchAddress("lep_promptLeptonInput_LepJetPtFrac_1", &lep_promptLeptonInput_LepJetPtFrac_1, &b_lep_promptLeptonInput_LepJetPtFrac_1);
  fChain->SetBranchAddress("lep_promptLeptonInput_LepJetPtFrac_2", &lep_promptLeptonInput_LepJetPtFrac_2, &b_lep_promptLeptonInput_LepJetPtFrac_2);
  fChain->SetBranchAddress("lep_promptLeptonInput_PtFrac_0", &lep_promptLeptonInput_PtFrac_0, &b_lep_promptLeptonInput_PtFrac_0);
  fChain->SetBranchAddress("lep_promptLeptonInput_PtFrac_1", &lep_promptLeptonInput_PtFrac_1, &b_lep_promptLeptonInput_PtFrac_1);
  fChain->SetBranchAddress("lep_promptLeptonInput_PtFrac_2", &lep_promptLeptonInput_PtFrac_2, &b_lep_promptLeptonInput_PtFrac_2);
  fChain->SetBranchAddress("lep_promptLeptonInput_PtRel_0", &lep_promptLeptonInput_PtRel_0, &b_lep_promptLeptonInput_PtRel_0);
  fChain->SetBranchAddress("lep_promptLeptonInput_PtRel_1", &lep_promptLeptonInput_PtRel_1, &b_lep_promptLeptonInput_PtRel_1);
  fChain->SetBranchAddress("lep_promptLeptonInput_PtRel_2", &lep_promptLeptonInput_PtRel_2, &b_lep_promptLeptonInput_PtRel_2);
  fChain->SetBranchAddress("lep_promptLeptonInput_ip2_0", &lep_promptLeptonInput_ip2_0, &b_lep_promptLeptonInput_ip2_0);
  fChain->SetBranchAddress("lep_promptLeptonInput_ip2_1", &lep_promptLeptonInput_ip2_1, &b_lep_promptLeptonInput_ip2_1);
  fChain->SetBranchAddress("lep_promptLeptonInput_ip2_2", &lep_promptLeptonInput_ip2_2, &b_lep_promptLeptonInput_ip2_2);
  fChain->SetBranchAddress("lep_promptLeptonInput_ip3_0", &lep_promptLeptonInput_ip3_0, &b_lep_promptLeptonInput_ip3_0);
  fChain->SetBranchAddress("lep_promptLeptonInput_ip3_1", &lep_promptLeptonInput_ip3_1, &b_lep_promptLeptonInput_ip3_1);
  fChain->SetBranchAddress("lep_promptLeptonInput_ip3_2", &lep_promptLeptonInput_ip3_2, &b_lep_promptLeptonInput_ip3_2);
  fChain->SetBranchAddress("lep_promptLeptonInput_rnnip_0", &lep_promptLeptonInput_rnnip_0, &b_lep_promptLeptonInput_rnnip_0);
  fChain->SetBranchAddress("lep_promptLeptonInput_rnnip_1", &lep_promptLeptonInput_rnnip_1, &b_lep_promptLeptonInput_rnnip_1);
  fChain->SetBranchAddress("lep_promptLeptonInput_rnnip_2", &lep_promptLeptonInput_rnnip_2, &b_lep_promptLeptonInput_rnnip_2);
  fChain->SetBranchAddress("lep_ID_0", &lep_ID_0, &b_lep_ID_0);
  fChain->SetBranchAddress("lep_Index_0", &lep_Index_0, &b_lep_Index_0);
  fChain->SetBranchAddress("lep_ambiguityType_0", &lep_ambiguityType_0, &b_lep_ambiguityType_0);
  fChain->SetBranchAddress("lep_Pt_0", &lep_Pt_0, &b_lep_Pt_0);
  fChain->SetBranchAddress("lep_E_0", &lep_E_0, &b_lep_E_0);
  fChain->SetBranchAddress("lep_Eta_0", &lep_Eta_0, &b_lep_Eta_0);
  fChain->SetBranchAddress("lep_Phi_0", &lep_Phi_0, &b_lep_Phi_0);
  fChain->SetBranchAddress("lep_EtaBE2_0", &lep_EtaBE2_0, &b_lep_EtaBE2_0);
  fChain->SetBranchAddress("lep_topoEtcone20_0", &lep_topoEtcone20_0, &b_lep_topoEtcone20_0);
  fChain->SetBranchAddress("lep_topoEtcone30_0", &lep_topoEtcone30_0, &b_lep_topoEtcone30_0);
  fChain->SetBranchAddress("lep_topoEtcone40_0", &lep_topoEtcone40_0, &b_lep_topoEtcone40_0);
  fChain->SetBranchAddress("lep_ptVarcone20_0", &lep_ptVarcone20_0, &b_lep_ptVarcone20_0);
  fChain->SetBranchAddress("lep_ptVarcone30_0", &lep_ptVarcone30_0, &b_lep_ptVarcone30_0);
  fChain->SetBranchAddress("lep_ptVarcone40_0", &lep_ptVarcone40_0, &b_lep_ptVarcone40_0);
  fChain->SetBranchAddress("lep_sigd0PV_0", &lep_sigd0PV_0, &b_lep_sigd0PV_0);
  fChain->SetBranchAddress("lep_Z0SinTheta_0", &lep_Z0SinTheta_0, &b_lep_Z0SinTheta_0);
  fChain->SetBranchAddress("lep_d0_0", &lep_d0_0, &b_lep_d0_0);
  fChain->SetBranchAddress("lep_z0_0", &lep_z0_0, &b_lep_z0_0);
  fChain->SetBranchAddress("lep_vz_0", &lep_vz_0, &b_lep_vz_0);
  fChain->SetBranchAddress("lep_deltaz0_0", &lep_deltaz0_0, &b_lep_deltaz0_0);
  fChain->SetBranchAddress("lep_isTightLH_0", &lep_isTightLH_0, &b_lep_isTightLH_0);
  fChain->SetBranchAddress("lep_isMediumLH_0", &lep_isMediumLH_0, &b_lep_isMediumLH_0);
  fChain->SetBranchAddress("lep_isLooseLH_0", &lep_isLooseLH_0, &b_lep_isLooseLH_0);
  fChain->SetBranchAddress("lep_isTight_0", &lep_isTight_0, &b_lep_isTight_0);
  fChain->SetBranchAddress("lep_isMedium_0", &lep_isMedium_0, &b_lep_isMedium_0);
  fChain->SetBranchAddress("lep_isLoose_0", &lep_isLoose_0, &b_lep_isLoose_0);
  fChain->SetBranchAddress("lep_isolationLooseTrackOnly_0", &lep_isolationLooseTrackOnly_0, &b_lep_isolationLooseTrackOnly_0);
  fChain->SetBranchAddress("lep_isolationLoose_0", &lep_isolationLoose_0, &b_lep_isolationLoose_0);
  fChain->SetBranchAddress("lep_isolationGradient_0", &lep_isolationGradient_0, &b_lep_isolationGradient_0);
  fChain->SetBranchAddress("lep_isolationGradientLoose_0", &lep_isolationGradientLoose_0, &b_lep_isolationGradientLoose_0);
  fChain->SetBranchAddress("lep_isolationFixedCutTight_0", &lep_isolationFixedCutTight_0, &b_lep_isolationFixedCutTight_0);
  fChain->SetBranchAddress("lep_isolationFixedCutTightTrackOnly_0", &lep_isolationFixedCutTightTrackOnly_0, &b_lep_isolationFixedCutTightTrackOnly_0);
  fChain->SetBranchAddress("lep_isolationFixedCutLoose_0", &lep_isolationFixedCutLoose_0, &b_lep_isolationFixedCutLoose_0);
  fChain->SetBranchAddress("lep_isTrigMatch_0", &lep_isTrigMatch_0, &b_lep_isTrigMatch_0);
  fChain->SetBranchAddress("lep_isTrigMatchDLT_0", &lep_isTrigMatchDLT_0, &b_lep_isTrigMatchDLT_0);
  fChain->SetBranchAddress("lep_isPrompt_0", &lep_isPrompt_0, &b_lep_isPrompt_0);
  fChain->SetBranchAddress("lep_isFakeLep_0", &lep_isFakeLep_0, &b_lep_isFakeLep_0);
  fChain->SetBranchAddress("lep_isQMisID_0", &lep_isQMisID_0, &b_lep_isQMisID_0);
  fChain->SetBranchAddress("lep_isConvPh_0", &lep_isConvPh_0, &b_lep_isConvPh_0);
  fChain->SetBranchAddress("lep_isIntConvPh_0", &lep_isIntConvPh_0, &b_lep_isIntConvPh_0);
  fChain->SetBranchAddress("lep_isExtConvPh_0", &lep_isExtConvPh_0, &b_lep_isExtConvPh_0);
  fChain->SetBranchAddress("lep_isISR_FSR_Ph_0", &lep_isISR_FSR_Ph_0, &b_lep_isISR_FSR_Ph_0);
  fChain->SetBranchAddress("lep_isBrems_0", &lep_isBrems_0, &b_lep_isBrems_0);
  fChain->SetBranchAddress("lep_isTruthMatched_0", &lep_isTruthMatched_0, &b_lep_isTruthMatched_0);
  fChain->SetBranchAddress("lep_truthType_0", &lep_truthType_0, &b_lep_truthType_0);
  fChain->SetBranchAddress("lep_truthOrigin_0", &lep_truthOrigin_0, &b_lep_truthOrigin_0);
  fChain->SetBranchAddress("lep_truthPdgId_0", &lep_truthPdgId_0, &b_lep_truthPdgId_0);
  fChain->SetBranchAddress("lep_truthStatus_0", &lep_truthStatus_0, &b_lep_truthStatus_0);
  fChain->SetBranchAddress("lep_truthParentType_0", &lep_truthParentType_0, &b_lep_truthParentType_0);
  fChain->SetBranchAddress("lep_truthParentOrigin_0", &lep_truthParentOrigin_0, &b_lep_truthParentOrigin_0);
  fChain->SetBranchAddress("lep_truthParentPdgId_0", &lep_truthParentPdgId_0, &b_lep_truthParentPdgId_0);
  fChain->SetBranchAddress("lep_truthParentStatus_0", &lep_truthParentStatus_0, &b_lep_truthParentStatus_0);
  fChain->SetBranchAddress("lep_truthPt_0", &lep_truthPt_0, &b_lep_truthPt_0);
  fChain->SetBranchAddress("lep_truthEta_0", &lep_truthEta_0, &b_lep_truthEta_0);
  fChain->SetBranchAddress("lep_truthPhi_0", &lep_truthPhi_0, &b_lep_truthPhi_0);
  fChain->SetBranchAddress("lep_truthM_0", &lep_truthM_0, &b_lep_truthM_0);
  fChain->SetBranchAddress("lep_truthE_0", &lep_truthE_0, &b_lep_truthE_0);
  fChain->SetBranchAddress("lep_truthRapidity_0", &lep_truthRapidity_0, &b_lep_truthRapidity_0);
  fChain->SetBranchAddress("lep_SFIDLoose_0", &lep_SFIDLoose_0, &b_lep_SFIDLoose_0);
  fChain->SetBranchAddress("lep_SFIDTight_0", &lep_SFIDTight_0, &b_lep_SFIDTight_0);
  fChain->SetBranchAddress("lep_SFTrigLoose_0", &lep_SFTrigLoose_0, &b_lep_SFTrigLoose_0);
  fChain->SetBranchAddress("lep_SFTrigTight_0", &lep_SFTrigTight_0, &b_lep_SFTrigTight_0);
  fChain->SetBranchAddress("lep_EffTrigLoose_0", &lep_EffTrigLoose_0, &b_lep_EffTrigLoose_0);
  fChain->SetBranchAddress("lep_EffTrigTight_0", &lep_EffTrigTight_0, &b_lep_EffTrigTight_0);
  fChain->SetBranchAddress("lep_SFIsoLoose_0", &lep_SFIsoLoose_0, &b_lep_SFIsoLoose_0);
  fChain->SetBranchAddress("lep_SFIsoTight_0", &lep_SFIsoTight_0, &b_lep_SFIsoTight_0);
  fChain->SetBranchAddress("lep_SFReco_0", &lep_SFReco_0, &b_lep_SFReco_0);
  fChain->SetBranchAddress("lep_SFTTVA_0", &lep_SFTTVA_0, &b_lep_SFTTVA_0);
  fChain->SetBranchAddress("lep_SFObjLoose_0", &lep_SFObjLoose_0, &b_lep_SFObjLoose_0);
  fChain->SetBranchAddress("lep_SFObjTight_0", &lep_SFObjTight_0, &b_lep_SFObjTight_0);
  fChain->SetBranchAddress("lep_chargeIDBDTLoose_0", &lep_chargeIDBDTLoose_0, &b_lep_chargeIDBDTLoose_0);
  fChain->SetBranchAddress("lep_chargeIDBDTMedium_0", &lep_chargeIDBDTMedium_0, &b_lep_chargeIDBDTMedium_0);
  fChain->SetBranchAddress("lep_chargeIDBDTTight_0", &lep_chargeIDBDTTight_0, &b_lep_chargeIDBDTTight_0);
  fChain->SetBranchAddress("lep_promptLeptonIso_TagWeight_0", &lep_promptLeptonIso_TagWeight_0, &b_lep_promptLeptonIso_TagWeight_0);
  fChain->SetBranchAddress("lep_promptLeptonVeto_TagWeight_0", &lep_promptLeptonVeto_TagWeight_0, &b_lep_promptLeptonVeto_TagWeight_0);
  fChain->SetBranchAddress("lep_nTrackParticles_0", &lep_nTrackParticles_0, &b_lep_nTrackParticles_0);
  fChain->SetBranchAddress("lep_RadiusCO_0", &lep_RadiusCO_0, &b_lep_RadiusCO_0);
  fChain->SetBranchAddress("lep_SeparationMinDCT_0", &lep_SeparationMinDCT_0, &b_lep_SeparationMinDCT_0);
  fChain->SetBranchAddress("lep_ClosestSiTracknIL_0", &lep_ClosestSiTracknIL_0, &b_lep_ClosestSiTracknIL_0);
  fChain->SetBranchAddress("lep_ClosestSiTrackeIL_0", &lep_ClosestSiTrackeIL_0, &b_lep_ClosestSiTrackeIL_0);
  fChain->SetBranchAddress("lep_ClosestSiTracknNIL_0", &lep_ClosestSiTracknNIL_0, &b_lep_ClosestSiTracknNIL_0);
  fChain->SetBranchAddress("lep_ClosestSiTrackeNIL_0", &lep_ClosestSiTrackeNIL_0, &b_lep_ClosestSiTrackeNIL_0);
  fChain->SetBranchAddress("lep_BestMatchTrackPt_0", &lep_BestMatchTrackPt_0, &b_lep_BestMatchTrackPt_0);
  fChain->SetBranchAddress("lep_BestMatchTrackEta_0", &lep_BestMatchTrackEta_0, &b_lep_BestMatchTrackEta_0);
  fChain->SetBranchAddress("lep_BestMatchTrackPhi_0", &lep_BestMatchTrackPhi_0, &b_lep_BestMatchTrackPhi_0);
  fChain->SetBranchAddress("lep_BestMatchTrackD0_0", &lep_BestMatchTrackD0_0, &b_lep_BestMatchTrackD0_0);
  fChain->SetBranchAddress("lep_BestMatchTrackZ0_0", &lep_BestMatchTrackZ0_0, &b_lep_BestMatchTrackZ0_0);
  fChain->SetBranchAddress("lep_BestMatchTrackQ_0", &lep_BestMatchTrackQ_0, &b_lep_BestMatchTrackQ_0);
  fChain->SetBranchAddress("lep_ClosestSiTrackPt_0", &lep_ClosestSiTrackPt_0, &b_lep_ClosestSiTrackPt_0);
  fChain->SetBranchAddress("lep_ClosestSiTrackEta_0", &lep_ClosestSiTrackEta_0, &b_lep_ClosestSiTrackEta_0);
  fChain->SetBranchAddress("lep_ClosestSiTrackPhi_0", &lep_ClosestSiTrackPhi_0, &b_lep_ClosestSiTrackPhi_0);
  fChain->SetBranchAddress("lep_ClosestSiTrackD0_0", &lep_ClosestSiTrackD0_0, &b_lep_ClosestSiTrackD0_0);
  fChain->SetBranchAddress("lep_ClosestSiTrackZ0_0", &lep_ClosestSiTrackZ0_0, &b_lep_ClosestSiTrackZ0_0);
  fChain->SetBranchAddress("lep_ClosestSiTrackQ_0", &lep_ClosestSiTrackQ_0, &b_lep_ClosestSiTrackQ_0);
  fChain->SetBranchAddress("lep_Mtrktrk_atConvV_CO_0", &lep_Mtrktrk_atConvV_CO_0, &b_lep_Mtrktrk_atConvV_CO_0);
  fChain->SetBranchAddress("lep_Mtrktrk_atPV_CO_0", &lep_Mtrktrk_atPV_CO_0, &b_lep_Mtrktrk_atPV_CO_0);
  fChain->SetBranchAddress("lep_RadiusCOX_0", &lep_RadiusCOX_0, &b_lep_RadiusCOX_0);
  fChain->SetBranchAddress("lep_RadiusCOY_0", &lep_RadiusCOY_0, &b_lep_RadiusCOY_0);
  fChain->SetBranchAddress("lep_ID_1", &lep_ID_1, &b_lep_ID_1);
  fChain->SetBranchAddress("lep_Index_1", &lep_Index_1, &b_lep_Index_1);
  fChain->SetBranchAddress("lep_ambiguityType_1", &lep_ambiguityType_1, &b_lep_ambiguityType_1);
  fChain->SetBranchAddress("lep_Pt_1", &lep_Pt_1, &b_lep_Pt_1);
  fChain->SetBranchAddress("lep_E_1", &lep_E_1, &b_lep_E_1);
  fChain->SetBranchAddress("lep_Eta_1", &lep_Eta_1, &b_lep_Eta_1);
  fChain->SetBranchAddress("lep_Phi_1", &lep_Phi_1, &b_lep_Phi_1);
  fChain->SetBranchAddress("lep_EtaBE2_1", &lep_EtaBE2_1, &b_lep_EtaBE2_1);
  fChain->SetBranchAddress("lep_topoEtcone20_1", &lep_topoEtcone20_1, &b_lep_topoEtcone20_1);
  fChain->SetBranchAddress("lep_topoEtcone30_1", &lep_topoEtcone30_1, &b_lep_topoEtcone30_1);
  fChain->SetBranchAddress("lep_topoEtcone40_1", &lep_topoEtcone40_1, &b_lep_topoEtcone40_1);
  fChain->SetBranchAddress("lep_ptVarcone20_1", &lep_ptVarcone20_1, &b_lep_ptVarcone20_1);
  fChain->SetBranchAddress("lep_ptVarcone30_1", &lep_ptVarcone30_1, &b_lep_ptVarcone30_1);
  fChain->SetBranchAddress("lep_ptVarcone40_1", &lep_ptVarcone40_1, &b_lep_ptVarcone40_1);
  fChain->SetBranchAddress("lep_sigd0PV_1", &lep_sigd0PV_1, &b_lep_sigd0PV_1);
  fChain->SetBranchAddress("lep_Z0SinTheta_1", &lep_Z0SinTheta_1, &b_lep_Z0SinTheta_1);
  fChain->SetBranchAddress("lep_d0_1", &lep_d0_1, &b_lep_d0_1);
  fChain->SetBranchAddress("lep_z0_1", &lep_z0_1, &b_lep_z0_1);
  fChain->SetBranchAddress("lep_vz_1", &lep_vz_1, &b_lep_vz_1);
  fChain->SetBranchAddress("lep_deltaz0_1", &lep_deltaz0_1, &b_lep_deltaz0_1);
  fChain->SetBranchAddress("lep_isTightLH_1", &lep_isTightLH_1, &b_lep_isTightLH_1);
  fChain->SetBranchAddress("lep_isMediumLH_1", &lep_isMediumLH_1, &b_lep_isMediumLH_1);
  fChain->SetBranchAddress("lep_isLooseLH_1", &lep_isLooseLH_1, &b_lep_isLooseLH_1);
  fChain->SetBranchAddress("lep_isTight_1", &lep_isTight_1, &b_lep_isTight_1);
  fChain->SetBranchAddress("lep_isMedium_1", &lep_isMedium_1, &b_lep_isMedium_1);
  fChain->SetBranchAddress("lep_isLoose_1", &lep_isLoose_1, &b_lep_isLoose_1);
  fChain->SetBranchAddress("lep_isolationLooseTrackOnly_1", &lep_isolationLooseTrackOnly_1, &b_lep_isolationLooseTrackOnly_1);
  fChain->SetBranchAddress("lep_isolationLoose_1", &lep_isolationLoose_1, &b_lep_isolationLoose_1);
  fChain->SetBranchAddress("lep_isolationGradient_1", &lep_isolationGradient_1, &b_lep_isolationGradient_1);
  fChain->SetBranchAddress("lep_isolationGradientLoose_1", &lep_isolationGradientLoose_1, &b_lep_isolationGradientLoose_1);
  fChain->SetBranchAddress("lep_isolationFixedCutTight_1", &lep_isolationFixedCutTight_1, &b_lep_isolationFixedCutTight_1);
  fChain->SetBranchAddress("lep_isolationFixedCutTightTrackOnly_1", &lep_isolationFixedCutTightTrackOnly_1, &b_lep_isolationFixedCutTightTrackOnly_1);
  fChain->SetBranchAddress("lep_isolationFixedCutLoose_1", &lep_isolationFixedCutLoose_1, &b_lep_isolationFixedCutLoose_1);
  fChain->SetBranchAddress("lep_isTrigMatch_1", &lep_isTrigMatch_1, &b_lep_isTrigMatch_1);
  fChain->SetBranchAddress("lep_isTrigMatchDLT_1", &lep_isTrigMatchDLT_1, &b_lep_isTrigMatchDLT_1);
  fChain->SetBranchAddress("lep_isPrompt_1", &lep_isPrompt_1, &b_lep_isPrompt_1);
  fChain->SetBranchAddress("lep_isFakeLep_1", &lep_isFakeLep_1, &b_lep_isFakeLep_1);
  fChain->SetBranchAddress("lep_isQMisID_1", &lep_isQMisID_1, &b_lep_isQMisID_1);
  fChain->SetBranchAddress("lep_isConvPh_1", &lep_isConvPh_1, &b_lep_isConvPh_1);
  fChain->SetBranchAddress("lep_isIntConvPh_1", &lep_isIntConvPh_1, &b_lep_isIntConvPh_1);
  fChain->SetBranchAddress("lep_isExtConvPh_1", &lep_isExtConvPh_1, &b_lep_isExtConvPh_1);
  fChain->SetBranchAddress("lep_isISR_FSR_Ph_1", &lep_isISR_FSR_Ph_1, &b_lep_isISR_FSR_Ph_1);
  fChain->SetBranchAddress("lep_isBrems_1", &lep_isBrems_1, &b_lep_isBrems_1);
  fChain->SetBranchAddress("lep_isTruthMatched_1", &lep_isTruthMatched_1, &b_lep_isTruthMatched_1);
  fChain->SetBranchAddress("lep_truthType_1", &lep_truthType_1, &b_lep_truthType_1);
  fChain->SetBranchAddress("lep_truthOrigin_1", &lep_truthOrigin_1, &b_lep_truthOrigin_1);
  fChain->SetBranchAddress("lep_truthPdgId_1", &lep_truthPdgId_1, &b_lep_truthPdgId_1);
  fChain->SetBranchAddress("lep_truthStatus_1", &lep_truthStatus_1, &b_lep_truthStatus_1);
  fChain->SetBranchAddress("lep_truthParentType_1", &lep_truthParentType_1, &b_lep_truthParentType_1);
  fChain->SetBranchAddress("lep_truthParentOrigin_1", &lep_truthParentOrigin_1, &b_lep_truthParentOrigin_1);
  fChain->SetBranchAddress("lep_truthParentPdgId_1", &lep_truthParentPdgId_1, &b_lep_truthParentPdgId_1);
  fChain->SetBranchAddress("lep_truthParentStatus_1", &lep_truthParentStatus_1, &b_lep_truthParentStatus_1);
  fChain->SetBranchAddress("lep_truthPt_1", &lep_truthPt_1, &b_lep_truthPt_1);
  fChain->SetBranchAddress("lep_truthEta_1", &lep_truthEta_1, &b_lep_truthEta_1);
  fChain->SetBranchAddress("lep_truthPhi_1", &lep_truthPhi_1, &b_lep_truthPhi_1);
  fChain->SetBranchAddress("lep_truthM_1", &lep_truthM_1, &b_lep_truthM_1);
  fChain->SetBranchAddress("lep_truthE_1", &lep_truthE_1, &b_lep_truthE_1);
  fChain->SetBranchAddress("lep_truthRapidity_1", &lep_truthRapidity_1, &b_lep_truthRapidity_1);
  fChain->SetBranchAddress("lep_SFIDLoose_1", &lep_SFIDLoose_1, &b_lep_SFIDLoose_1);
  fChain->SetBranchAddress("lep_SFIDTight_1", &lep_SFIDTight_1, &b_lep_SFIDTight_1);
  fChain->SetBranchAddress("lep_SFTrigLoose_1", &lep_SFTrigLoose_1, &b_lep_SFTrigLoose_1);
  fChain->SetBranchAddress("lep_SFTrigTight_1", &lep_SFTrigTight_1, &b_lep_SFTrigTight_1);
  fChain->SetBranchAddress("lep_EffTrigLoose_1", &lep_EffTrigLoose_1, &b_lep_EffTrigLoose_1);
  fChain->SetBranchAddress("lep_EffTrigTight_1", &lep_EffTrigTight_1, &b_lep_EffTrigTight_1);
  fChain->SetBranchAddress("lep_SFIsoLoose_1", &lep_SFIsoLoose_1, &b_lep_SFIsoLoose_1);
  fChain->SetBranchAddress("lep_SFIsoTight_1", &lep_SFIsoTight_1, &b_lep_SFIsoTight_1);
  fChain->SetBranchAddress("lep_SFReco_1", &lep_SFReco_1, &b_lep_SFReco_1);
  fChain->SetBranchAddress("lep_SFTTVA_1", &lep_SFTTVA_1, &b_lep_SFTTVA_1);
  fChain->SetBranchAddress("lep_SFObjLoose_1", &lep_SFObjLoose_1, &b_lep_SFObjLoose_1);
  fChain->SetBranchAddress("lep_SFObjTight_1", &lep_SFObjTight_1, &b_lep_SFObjTight_1);
  fChain->SetBranchAddress("lep_chargeIDBDTLoose_1", &lep_chargeIDBDTLoose_1, &b_lep_chargeIDBDTLoose_1);
  fChain->SetBranchAddress("lep_chargeIDBDTMedium_1", &lep_chargeIDBDTMedium_1, &b_lep_chargeIDBDTMedium_1);
  fChain->SetBranchAddress("lep_chargeIDBDTTight_1", &lep_chargeIDBDTTight_1, &b_lep_chargeIDBDTTight_1);
  fChain->SetBranchAddress("lep_promptLeptonIso_TagWeight_1", &lep_promptLeptonIso_TagWeight_1, &b_lep_promptLeptonIso_TagWeight_1);
  fChain->SetBranchAddress("lep_promptLeptonVeto_TagWeight_1", &lep_promptLeptonVeto_TagWeight_1, &b_lep_promptLeptonVeto_TagWeight_1);
  fChain->SetBranchAddress("lep_nTrackParticles_1", &lep_nTrackParticles_1, &b_lep_nTrackParticles_1);
  fChain->SetBranchAddress("lep_RadiusCO_1", &lep_RadiusCO_1, &b_lep_RadiusCO_1);
  fChain->SetBranchAddress("lep_SeparationMinDCT_1", &lep_SeparationMinDCT_1, &b_lep_SeparationMinDCT_1);
  fChain->SetBranchAddress("lep_ClosestSiTracknIL_1", &lep_ClosestSiTracknIL_1, &b_lep_ClosestSiTracknIL_1);
  fChain->SetBranchAddress("lep_ClosestSiTrackeIL_1", &lep_ClosestSiTrackeIL_1, &b_lep_ClosestSiTrackeIL_1);
  fChain->SetBranchAddress("lep_ClosestSiTracknNIL_1", &lep_ClosestSiTracknNIL_1, &b_lep_ClosestSiTracknNIL_1);
  fChain->SetBranchAddress("lep_ClosestSiTrackeNIL_1", &lep_ClosestSiTrackeNIL_1, &b_lep_ClosestSiTrackeNIL_1);
  fChain->SetBranchAddress("lep_BestMatchTrackPt_1", &lep_BestMatchTrackPt_1, &b_lep_BestMatchTrackPt_1);
  fChain->SetBranchAddress("lep_BestMatchTrackEta_1", &lep_BestMatchTrackEta_1, &b_lep_BestMatchTrackEta_1);
  fChain->SetBranchAddress("lep_BestMatchTrackPhi_1", &lep_BestMatchTrackPhi_1, &b_lep_BestMatchTrackPhi_1);
  fChain->SetBranchAddress("lep_BestMatchTrackD0_1", &lep_BestMatchTrackD0_1, &b_lep_BestMatchTrackD0_1);
  fChain->SetBranchAddress("lep_BestMatchTrackZ0_1", &lep_BestMatchTrackZ0_1, &b_lep_BestMatchTrackZ0_1);
  fChain->SetBranchAddress("lep_BestMatchTrackQ_1", &lep_BestMatchTrackQ_1, &b_lep_BestMatchTrackQ_1);
  fChain->SetBranchAddress("lep_ClosestSiTrackPt_1", &lep_ClosestSiTrackPt_1, &b_lep_ClosestSiTrackPt_1);
  fChain->SetBranchAddress("lep_ClosestSiTrackEta_1", &lep_ClosestSiTrackEta_1, &b_lep_ClosestSiTrackEta_1);
  fChain->SetBranchAddress("lep_ClosestSiTrackPhi_1", &lep_ClosestSiTrackPhi_1, &b_lep_ClosestSiTrackPhi_1);
  fChain->SetBranchAddress("lep_ClosestSiTrackD0_1", &lep_ClosestSiTrackD0_1, &b_lep_ClosestSiTrackD0_1);
  fChain->SetBranchAddress("lep_ClosestSiTrackZ0_1", &lep_ClosestSiTrackZ0_1, &b_lep_ClosestSiTrackZ0_1);
  fChain->SetBranchAddress("lep_ClosestSiTrackQ_1", &lep_ClosestSiTrackQ_1, &b_lep_ClosestSiTrackQ_1);
  fChain->SetBranchAddress("lep_Mtrktrk_atConvV_CO_1", &lep_Mtrktrk_atConvV_CO_1, &b_lep_Mtrktrk_atConvV_CO_1);
  fChain->SetBranchAddress("lep_Mtrktrk_atPV_CO_1", &lep_Mtrktrk_atPV_CO_1, &b_lep_Mtrktrk_atPV_CO_1);
  fChain->SetBranchAddress("lep_RadiusCOX_1", &lep_RadiusCOX_1, &b_lep_RadiusCOX_1);
  fChain->SetBranchAddress("lep_RadiusCOY_1", &lep_RadiusCOY_1, &b_lep_RadiusCOY_1);
  fChain->SetBranchAddress("lep_ID_2", &lep_ID_2, &b_lep_ID_2);
  fChain->SetBranchAddress("lep_Index_2", &lep_Index_2, &b_lep_Index_2);
  fChain->SetBranchAddress("lep_ambiguityType_2", &lep_ambiguityType_2, &b_lep_ambiguityType_2);
  fChain->SetBranchAddress("lep_Pt_2", &lep_Pt_2, &b_lep_Pt_2);
  fChain->SetBranchAddress("lep_E_2", &lep_E_2, &b_lep_E_2);
  fChain->SetBranchAddress("lep_Eta_2", &lep_Eta_2, &b_lep_Eta_2);
  fChain->SetBranchAddress("lep_Phi_2", &lep_Phi_2, &b_lep_Phi_2);
  fChain->SetBranchAddress("lep_EtaBE2_2", &lep_EtaBE2_2, &b_lep_EtaBE2_2);
  fChain->SetBranchAddress("lep_topoEtcone20_2", &lep_topoEtcone20_2, &b_lep_topoEtcone20_2);
  fChain->SetBranchAddress("lep_topoEtcone30_2", &lep_topoEtcone30_2, &b_lep_topoEtcone30_2);
  fChain->SetBranchAddress("lep_topoEtcone40_2", &lep_topoEtcone40_2, &b_lep_topoEtcone40_2);
  fChain->SetBranchAddress("lep_ptVarcone20_2", &lep_ptVarcone20_2, &b_lep_ptVarcone20_2);
  fChain->SetBranchAddress("lep_ptVarcone30_2", &lep_ptVarcone30_2, &b_lep_ptVarcone30_2);
  fChain->SetBranchAddress("lep_ptVarcone40_2", &lep_ptVarcone40_2, &b_lep_ptVarcone40_2);
  fChain->SetBranchAddress("lep_sigd0PV_2", &lep_sigd0PV_2, &b_lep_sigd0PV_2);
  fChain->SetBranchAddress("lep_Z0SinTheta_2", &lep_Z0SinTheta_2, &b_lep_Z0SinTheta_2);
  fChain->SetBranchAddress("lep_d0_2", &lep_d0_2, &b_lep_d0_2);
  fChain->SetBranchAddress("lep_z0_2", &lep_z0_2, &b_lep_z0_2);
  fChain->SetBranchAddress("lep_vz_2", &lep_vz_2, &b_lep_vz_2);
  fChain->SetBranchAddress("lep_deltaz0_2", &lep_deltaz0_2, &b_lep_deltaz0_2);
  fChain->SetBranchAddress("lep_isTightLH_2", &lep_isTightLH_2, &b_lep_isTightLH_2);
  fChain->SetBranchAddress("lep_isMediumLH_2", &lep_isMediumLH_2, &b_lep_isMediumLH_2);
  fChain->SetBranchAddress("lep_isLooseLH_2", &lep_isLooseLH_2, &b_lep_isLooseLH_2);
  fChain->SetBranchAddress("lep_isTight_2", &lep_isTight_2, &b_lep_isTight_2);
  fChain->SetBranchAddress("lep_isMedium_2", &lep_isMedium_2, &b_lep_isMedium_2);
  fChain->SetBranchAddress("lep_isLoose_2", &lep_isLoose_2, &b_lep_isLoose_2);
  fChain->SetBranchAddress("lep_isolationLooseTrackOnly_2", &lep_isolationLooseTrackOnly_2, &b_lep_isolationLooseTrackOnly_2);
  fChain->SetBranchAddress("lep_isolationLoose_2", &lep_isolationLoose_2, &b_lep_isolationLoose_2);
  fChain->SetBranchAddress("lep_isolationGradient_2", &lep_isolationGradient_2, &b_lep_isolationGradient_2);
  fChain->SetBranchAddress("lep_isolationGradientLoose_2", &lep_isolationGradientLoose_2, &b_lep_isolationGradientLoose_2);
  fChain->SetBranchAddress("lep_isolationFixedCutTight_2", &lep_isolationFixedCutTight_2, &b_lep_isolationFixedCutTight_2);
  fChain->SetBranchAddress("lep_isolationFixedCutTightTrackOnly_2", &lep_isolationFixedCutTightTrackOnly_2, &b_lep_isolationFixedCutTightTrackOnly_2);
  fChain->SetBranchAddress("lep_isolationFixedCutLoose_2", &lep_isolationFixedCutLoose_2, &b_lep_isolationFixedCutLoose_2);
  fChain->SetBranchAddress("lep_isTrigMatch_2", &lep_isTrigMatch_2, &b_lep_isTrigMatch_2);
  fChain->SetBranchAddress("lep_isTrigMatchDLT_2", &lep_isTrigMatchDLT_2, &b_lep_isTrigMatchDLT_2);
  fChain->SetBranchAddress("lep_isPrompt_2", &lep_isPrompt_2, &b_lep_isPrompt_2);
  fChain->SetBranchAddress("lep_isFakeLep_2", &lep_isFakeLep_2, &b_lep_isFakeLep_2);
  fChain->SetBranchAddress("lep_isQMisID_2", &lep_isQMisID_2, &b_lep_isQMisID_2);
  fChain->SetBranchAddress("lep_isConvPh_2", &lep_isConvPh_2, &b_lep_isConvPh_2);
  fChain->SetBranchAddress("lep_isIntConvPh_2", &lep_isIntConvPh_2, &b_lep_isIntConvPh_2);
  fChain->SetBranchAddress("lep_isExtConvPh_2", &lep_isExtConvPh_2, &b_lep_isExtConvPh_2);
  fChain->SetBranchAddress("lep_isISR_FSR_Ph_2", &lep_isISR_FSR_Ph_2, &b_lep_isISR_FSR_Ph_2);
  fChain->SetBranchAddress("lep_isBrems_2", &lep_isBrems_2, &b_lep_isBrems_2);
  fChain->SetBranchAddress("lep_isTruthMatched_2", &lep_isTruthMatched_2, &b_lep_isTruthMatched_2);
  fChain->SetBranchAddress("lep_truthType_2", &lep_truthType_2, &b_lep_truthType_2);
  fChain->SetBranchAddress("lep_truthOrigin_2", &lep_truthOrigin_2, &b_lep_truthOrigin_2);
  fChain->SetBranchAddress("lep_truthPdgId_2", &lep_truthPdgId_2, &b_lep_truthPdgId_2);
  fChain->SetBranchAddress("lep_truthStatus_2", &lep_truthStatus_2, &b_lep_truthStatus_2);
  fChain->SetBranchAddress("lep_truthParentType_2", &lep_truthParentType_2, &b_lep_truthParentType_2);
  fChain->SetBranchAddress("lep_truthParentOrigin_2", &lep_truthParentOrigin_2, &b_lep_truthParentOrigin_2);
  fChain->SetBranchAddress("lep_truthParentPdgId_2", &lep_truthParentPdgId_2, &b_lep_truthParentPdgId_2);
  fChain->SetBranchAddress("lep_truthParentStatus_2", &lep_truthParentStatus_2, &b_lep_truthParentStatus_2);
  fChain->SetBranchAddress("lep_truthPt_2", &lep_truthPt_2, &b_lep_truthPt_2);
  fChain->SetBranchAddress("lep_truthEta_2", &lep_truthEta_2, &b_lep_truthEta_2);
  fChain->SetBranchAddress("lep_truthPhi_2", &lep_truthPhi_2, &b_lep_truthPhi_2);
  fChain->SetBranchAddress("lep_truthM_2", &lep_truthM_2, &b_lep_truthM_2);
  fChain->SetBranchAddress("lep_truthE_2", &lep_truthE_2, &b_lep_truthE_2);
  fChain->SetBranchAddress("lep_truthRapidity_2", &lep_truthRapidity_2, &b_lep_truthRapidity_2);
  fChain->SetBranchAddress("lep_SFIDLoose_2", &lep_SFIDLoose_2, &b_lep_SFIDLoose_2);
  fChain->SetBranchAddress("lep_SFIDTight_2", &lep_SFIDTight_2, &b_lep_SFIDTight_2);
  fChain->SetBranchAddress("lep_SFTrigLoose_2", &lep_SFTrigLoose_2, &b_lep_SFTrigLoose_2);
  fChain->SetBranchAddress("lep_SFTrigTight_2", &lep_SFTrigTight_2, &b_lep_SFTrigTight_2);
  fChain->SetBranchAddress("lep_EffTrigLoose_2", &lep_EffTrigLoose_2, &b_lep_EffTrigLoose_2);
  fChain->SetBranchAddress("lep_EffTrigTight_2", &lep_EffTrigTight_2, &b_lep_EffTrigTight_2);
  fChain->SetBranchAddress("lep_SFIsoLoose_2", &lep_SFIsoLoose_2, &b_lep_SFIsoLoose_2);
  fChain->SetBranchAddress("lep_SFIsoTight_2", &lep_SFIsoTight_2, &b_lep_SFIsoTight_2);
  fChain->SetBranchAddress("lep_SFReco_2", &lep_SFReco_2, &b_lep_SFReco_2);
  fChain->SetBranchAddress("lep_SFTTVA_2", &lep_SFTTVA_2, &b_lep_SFTTVA_2);
  fChain->SetBranchAddress("lep_SFObjLoose_2", &lep_SFObjLoose_2, &b_lep_SFObjLoose_2);
  fChain->SetBranchAddress("lep_SFObjTight_2", &lep_SFObjTight_2, &b_lep_SFObjTight_2);
  fChain->SetBranchAddress("lep_chargeIDBDTLoose_2", &lep_chargeIDBDTLoose_2, &b_lep_chargeIDBDTLoose_2);
  fChain->SetBranchAddress("lep_chargeIDBDTMedium_2", &lep_chargeIDBDTMedium_2, &b_lep_chargeIDBDTMedium_2);
  fChain->SetBranchAddress("lep_chargeIDBDTTight_2", &lep_chargeIDBDTTight_2, &b_lep_chargeIDBDTTight_2);
  fChain->SetBranchAddress("lep_promptLeptonIso_TagWeight_2", &lep_promptLeptonIso_TagWeight_2, &b_lep_promptLeptonIso_TagWeight_2);
  fChain->SetBranchAddress("lep_promptLeptonVeto_TagWeight_2", &lep_promptLeptonVeto_TagWeight_2, &b_lep_promptLeptonVeto_TagWeight_2);
  fChain->SetBranchAddress("lep_nTrackParticles_2", &lep_nTrackParticles_2, &b_lep_nTrackParticles_2);
  fChain->SetBranchAddress("lep_RadiusCO_2", &lep_RadiusCO_2, &b_lep_RadiusCO_2);
  fChain->SetBranchAddress("lep_SeparationMinDCT_2", &lep_SeparationMinDCT_2, &b_lep_SeparationMinDCT_2);
  fChain->SetBranchAddress("lep_ClosestSiTracknIL_2", &lep_ClosestSiTracknIL_2, &b_lep_ClosestSiTracknIL_2);
  fChain->SetBranchAddress("lep_ClosestSiTrackeIL_2", &lep_ClosestSiTrackeIL_2, &b_lep_ClosestSiTrackeIL_2);
  fChain->SetBranchAddress("lep_ClosestSiTracknNIL_2", &lep_ClosestSiTracknNIL_2, &b_lep_ClosestSiTracknNIL_2);
  fChain->SetBranchAddress("lep_ClosestSiTrackeNIL_2", &lep_ClosestSiTrackeNIL_2, &b_lep_ClosestSiTrackeNIL_2);
  fChain->SetBranchAddress("lep_BestMatchTrackPt_2", &lep_BestMatchTrackPt_2, &b_lep_BestMatchTrackPt_2);
  fChain->SetBranchAddress("lep_BestMatchTrackEta_2", &lep_BestMatchTrackEta_2, &b_lep_BestMatchTrackEta_2);
  fChain->SetBranchAddress("lep_BestMatchTrackPhi_2", &lep_BestMatchTrackPhi_2, &b_lep_BestMatchTrackPhi_2);
  fChain->SetBranchAddress("lep_BestMatchTrackD0_2", &lep_BestMatchTrackD0_2, &b_lep_BestMatchTrackD0_2);
  fChain->SetBranchAddress("lep_BestMatchTrackZ0_2", &lep_BestMatchTrackZ0_2, &b_lep_BestMatchTrackZ0_2);
  fChain->SetBranchAddress("lep_BestMatchTrackQ_2", &lep_BestMatchTrackQ_2, &b_lep_BestMatchTrackQ_2);
  fChain->SetBranchAddress("lep_ClosestSiTrackPt_2", &lep_ClosestSiTrackPt_2, &b_lep_ClosestSiTrackPt_2);
  fChain->SetBranchAddress("lep_ClosestSiTrackEta_2", &lep_ClosestSiTrackEta_2, &b_lep_ClosestSiTrackEta_2);
  fChain->SetBranchAddress("lep_ClosestSiTrackPhi_2", &lep_ClosestSiTrackPhi_2, &b_lep_ClosestSiTrackPhi_2);
  fChain->SetBranchAddress("lep_ClosestSiTrackD0_2", &lep_ClosestSiTrackD0_2, &b_lep_ClosestSiTrackD0_2);
  fChain->SetBranchAddress("lep_ClosestSiTrackZ0_2", &lep_ClosestSiTrackZ0_2, &b_lep_ClosestSiTrackZ0_2);
  fChain->SetBranchAddress("lep_ClosestSiTrackQ_2", &lep_ClosestSiTrackQ_2, &b_lep_ClosestSiTrackQ_2);
  fChain->SetBranchAddress("lep_Mtrktrk_atConvV_CO_2", &lep_Mtrktrk_atConvV_CO_2, &b_lep_Mtrktrk_atConvV_CO_2);
  fChain->SetBranchAddress("lep_Mtrktrk_atPV_CO_2", &lep_Mtrktrk_atPV_CO_2, &b_lep_Mtrktrk_atPV_CO_2);
  fChain->SetBranchAddress("lep_RadiusCOX_2", &lep_RadiusCOX_2, &b_lep_RadiusCOX_2);
  fChain->SetBranchAddress("lep_RadiusCOY_2", &lep_RadiusCOY_2, &b_lep_RadiusCOY_2);
  fChain->SetBranchAddress("lep_ID_3", &lep_ID_3, &b_lep_ID_3);
  fChain->SetBranchAddress("lep_Index_3", &lep_Index_3, &b_lep_Index_3);
  fChain->SetBranchAddress("lep_ambiguityType_3", &lep_ambiguityType_3, &b_lep_ambiguityType_3);
  fChain->SetBranchAddress("lep_Pt_3", &lep_Pt_3, &b_lep_Pt_3);
  fChain->SetBranchAddress("lep_E_3", &lep_E_3, &b_lep_E_3);
  fChain->SetBranchAddress("lep_Eta_3", &lep_Eta_3, &b_lep_Eta_3);
  fChain->SetBranchAddress("lep_Phi_3", &lep_Phi_3, &b_lep_Phi_3);
  fChain->SetBranchAddress("lep_EtaBE2_3", &lep_EtaBE2_3, &b_lep_EtaBE2_3);
  fChain->SetBranchAddress("lep_topoEtcone20_3", &lep_topoEtcone20_3, &b_lep_topoEtcone20_3);
  fChain->SetBranchAddress("lep_topoEtcone30_3", &lep_topoEtcone30_3, &b_lep_topoEtcone30_3);
  fChain->SetBranchAddress("lep_topoEtcone40_3", &lep_topoEtcone40_3, &b_lep_topoEtcone40_3);
  fChain->SetBranchAddress("lep_ptVarcone20_3", &lep_ptVarcone20_3, &b_lep_ptVarcone20_3);
  fChain->SetBranchAddress("lep_ptVarcone30_3", &lep_ptVarcone30_3, &b_lep_ptVarcone30_3);
  fChain->SetBranchAddress("lep_ptVarcone40_3", &lep_ptVarcone40_3, &b_lep_ptVarcone40_3);
  fChain->SetBranchAddress("lep_sigd0PV_3", &lep_sigd0PV_3, &b_lep_sigd0PV_3);
  fChain->SetBranchAddress("lep_Z0SinTheta_3", &lep_Z0SinTheta_3, &b_lep_Z0SinTheta_3);
  fChain->SetBranchAddress("lep_d0_3", &lep_d0_3, &b_lep_d0_3);
  fChain->SetBranchAddress("lep_z0_3", &lep_z0_3, &b_lep_z0_3);
  fChain->SetBranchAddress("lep_vz_3", &lep_vz_3, &b_lep_vz_3);
  fChain->SetBranchAddress("lep_deltaz0_3", &lep_deltaz0_3, &b_lep_deltaz0_3);
  fChain->SetBranchAddress("lep_isTightLH_3", &lep_isTightLH_3, &b_lep_isTightLH_3);
  fChain->SetBranchAddress("lep_isMediumLH_3", &lep_isMediumLH_3, &b_lep_isMediumLH_3);
  fChain->SetBranchAddress("lep_isLooseLH_3", &lep_isLooseLH_3, &b_lep_isLooseLH_3);
  fChain->SetBranchAddress("lep_isTight_3", &lep_isTight_3, &b_lep_isTight_3);
  fChain->SetBranchAddress("lep_isMedium_3", &lep_isMedium_3, &b_lep_isMedium_3);
  fChain->SetBranchAddress("lep_isLoose_3", &lep_isLoose_3, &b_lep_isLoose_3);
  fChain->SetBranchAddress("lep_isolationLooseTrackOnly_3", &lep_isolationLooseTrackOnly_3, &b_lep_isolationLooseTrackOnly_3);
  fChain->SetBranchAddress("lep_isolationLoose_3", &lep_isolationLoose_3, &b_lep_isolationLoose_3);
  fChain->SetBranchAddress("lep_isolationGradient_3", &lep_isolationGradient_3, &b_lep_isolationGradient_3);
  fChain->SetBranchAddress("lep_isolationGradientLoose_3", &lep_isolationGradientLoose_3, &b_lep_isolationGradientLoose_3);
  fChain->SetBranchAddress("lep_isolationFixedCutTight_3", &lep_isolationFixedCutTight_3, &b_lep_isolationFixedCutTight_3);
  fChain->SetBranchAddress("lep_isolationFixedCutTightTrackOnly_3", &lep_isolationFixedCutTightTrackOnly_3, &b_lep_isolationFixedCutTightTrackOnly_3);
  fChain->SetBranchAddress("lep_isolationFixedCutLoose_3", &lep_isolationFixedCutLoose_3, &b_lep_isolationFixedCutLoose_3);
  fChain->SetBranchAddress("lep_isTrigMatch_3", &lep_isTrigMatch_3, &b_lep_isTrigMatch_3);
  fChain->SetBranchAddress("lep_isTrigMatchDLT_3", &lep_isTrigMatchDLT_3, &b_lep_isTrigMatchDLT_3);
  fChain->SetBranchAddress("lep_isPrompt_3", &lep_isPrompt_3, &b_lep_isPrompt_3);
  fChain->SetBranchAddress("lep_isFakeLep_3", &lep_isFakeLep_3, &b_lep_isFakeLep_3);
  fChain->SetBranchAddress("lep_isQMisID_3", &lep_isQMisID_3, &b_lep_isQMisID_3);
  fChain->SetBranchAddress("lep_isConvPh_3", &lep_isConvPh_3, &b_lep_isConvPh_3);
  fChain->SetBranchAddress("lep_isIntConvPh_3", &lep_isIntConvPh_3, &b_lep_isIntConvPh_3);
  fChain->SetBranchAddress("lep_isExtConvPh_3", &lep_isExtConvPh_3, &b_lep_isExtConvPh_3);
  fChain->SetBranchAddress("lep_isISR_FSR_Ph_3", &lep_isISR_FSR_Ph_3, &b_lep_isISR_FSR_Ph_3);
  fChain->SetBranchAddress("lep_isBrems_3", &lep_isBrems_3, &b_lep_isBrems_3);
  fChain->SetBranchAddress("lep_isTruthMatched_3", &lep_isTruthMatched_3, &b_lep_isTruthMatched_3);
  fChain->SetBranchAddress("lep_truthType_3", &lep_truthType_3, &b_lep_truthType_3);
  fChain->SetBranchAddress("lep_truthOrigin_3", &lep_truthOrigin_3, &b_lep_truthOrigin_3);
  fChain->SetBranchAddress("lep_truthPdgId_3", &lep_truthPdgId_3, &b_lep_truthPdgId_3);
  fChain->SetBranchAddress("lep_truthStatus_3", &lep_truthStatus_3, &b_lep_truthStatus_3);
  fChain->SetBranchAddress("lep_truthParentType_3", &lep_truthParentType_3, &b_lep_truthParentType_3);
  fChain->SetBranchAddress("lep_truthParentOrigin_3", &lep_truthParentOrigin_3, &b_lep_truthParentOrigin_3);
  fChain->SetBranchAddress("lep_truthParentPdgId_3", &lep_truthParentPdgId_3, &b_lep_truthParentPdgId_3);
  fChain->SetBranchAddress("lep_truthParentStatus_3", &lep_truthParentStatus_3, &b_lep_truthParentStatus_3);
  fChain->SetBranchAddress("lep_truthPt_3", &lep_truthPt_3, &b_lep_truthPt_3);
  fChain->SetBranchAddress("lep_truthEta_3", &lep_truthEta_3, &b_lep_truthEta_3);
  fChain->SetBranchAddress("lep_truthPhi_3", &lep_truthPhi_3, &b_lep_truthPhi_3);
  fChain->SetBranchAddress("lep_truthM_3", &lep_truthM_3, &b_lep_truthM_3);
  fChain->SetBranchAddress("lep_truthE_3", &lep_truthE_3, &b_lep_truthE_3);
  fChain->SetBranchAddress("lep_truthRapidity_3", &lep_truthRapidity_3, &b_lep_truthRapidity_3);
  fChain->SetBranchAddress("lep_SFIDLoose_3", &lep_SFIDLoose_3, &b_lep_SFIDLoose_3);
  fChain->SetBranchAddress("lep_SFIDTight_3", &lep_SFIDTight_3, &b_lep_SFIDTight_3);
  fChain->SetBranchAddress("lep_SFTrigLoose_3", &lep_SFTrigLoose_3, &b_lep_SFTrigLoose_3);
  fChain->SetBranchAddress("lep_SFTrigTight_3", &lep_SFTrigTight_3, &b_lep_SFTrigTight_3);
  fChain->SetBranchAddress("lep_EffTrigLoose_3", &lep_EffTrigLoose_3, &b_lep_EffTrigLoose_3);
  fChain->SetBranchAddress("lep_EffTrigTight_3", &lep_EffTrigTight_3, &b_lep_EffTrigTight_3);
  fChain->SetBranchAddress("lep_SFIsoLoose_3", &lep_SFIsoLoose_3, &b_lep_SFIsoLoose_3);
  fChain->SetBranchAddress("lep_SFIsoTight_3", &lep_SFIsoTight_3, &b_lep_SFIsoTight_3);
  fChain->SetBranchAddress("lep_SFReco_3", &lep_SFReco_3, &b_lep_SFReco_3);
  fChain->SetBranchAddress("lep_SFTTVA_3", &lep_SFTTVA_3, &b_lep_SFTTVA_3);
  fChain->SetBranchAddress("lep_SFObjLoose_3", &lep_SFObjLoose_3, &b_lep_SFObjLoose_3);
  fChain->SetBranchAddress("lep_SFObjTight_3", &lep_SFObjTight_3, &b_lep_SFObjTight_3);
  fChain->SetBranchAddress("lep_chargeIDBDTLoose_3", &lep_chargeIDBDTLoose_3, &b_lep_chargeIDBDTLoose_3);
  fChain->SetBranchAddress("lep_chargeIDBDTMedium_3", &lep_chargeIDBDTMedium_3, &b_lep_chargeIDBDTMedium_3);
  fChain->SetBranchAddress("lep_chargeIDBDTTight_3", &lep_chargeIDBDTTight_3, &b_lep_chargeIDBDTTight_3);
  fChain->SetBranchAddress("lep_promptLeptonIso_TagWeight_3", &lep_promptLeptonIso_TagWeight_3, &b_lep_promptLeptonIso_TagWeight_3);
  fChain->SetBranchAddress("lep_promptLeptonVeto_TagWeight_3", &lep_promptLeptonVeto_TagWeight_3, &b_lep_promptLeptonVeto_TagWeight_3);
  fChain->SetBranchAddress("lep_nTrackParticles_3", &lep_nTrackParticles_3, &b_lep_nTrackParticles_3);
  fChain->SetBranchAddress("lep_RadiusCO_3", &lep_RadiusCO_3, &b_lep_RadiusCO_3);
  fChain->SetBranchAddress("lep_SeparationMinDCT_3", &lep_SeparationMinDCT_3, &b_lep_SeparationMinDCT_3);
  fChain->SetBranchAddress("lep_ClosestSiTracknIL_3", &lep_ClosestSiTracknIL_3, &b_lep_ClosestSiTracknIL_3);
  fChain->SetBranchAddress("lep_ClosestSiTrackeIL_3", &lep_ClosestSiTrackeIL_3, &b_lep_ClosestSiTrackeIL_3);
  fChain->SetBranchAddress("lep_ClosestSiTracknNIL_3", &lep_ClosestSiTracknNIL_3, &b_lep_ClosestSiTracknNIL_3);
  fChain->SetBranchAddress("lep_ClosestSiTrackeNIL_3", &lep_ClosestSiTrackeNIL_3, &b_lep_ClosestSiTrackeNIL_3);
  fChain->SetBranchAddress("lep_BestMatchTrackPt_3", &lep_BestMatchTrackPt_3, &b_lep_BestMatchTrackPt_3);
  fChain->SetBranchAddress("lep_BestMatchTrackEta_3", &lep_BestMatchTrackEta_3, &b_lep_BestMatchTrackEta_3);
  fChain->SetBranchAddress("lep_BestMatchTrackPhi_3", &lep_BestMatchTrackPhi_3, &b_lep_BestMatchTrackPhi_3);
  fChain->SetBranchAddress("lep_BestMatchTrackD0_3", &lep_BestMatchTrackD0_3, &b_lep_BestMatchTrackD0_3);
  fChain->SetBranchAddress("lep_BestMatchTrackZ0_3", &lep_BestMatchTrackZ0_3, &b_lep_BestMatchTrackZ0_3);
  fChain->SetBranchAddress("lep_BestMatchTrackQ_3", &lep_BestMatchTrackQ_3, &b_lep_BestMatchTrackQ_3);
  fChain->SetBranchAddress("lep_ClosestSiTrackPt_3", &lep_ClosestSiTrackPt_3, &b_lep_ClosestSiTrackPt_3);
  fChain->SetBranchAddress("lep_ClosestSiTrackEta_3", &lep_ClosestSiTrackEta_3, &b_lep_ClosestSiTrackEta_3);
  fChain->SetBranchAddress("lep_ClosestSiTrackPhi_3", &lep_ClosestSiTrackPhi_3, &b_lep_ClosestSiTrackPhi_3);
  fChain->SetBranchAddress("lep_ClosestSiTrackD0_3", &lep_ClosestSiTrackD0_3, &b_lep_ClosestSiTrackD0_3);
  fChain->SetBranchAddress("lep_ClosestSiTrackZ0_3", &lep_ClosestSiTrackZ0_3, &b_lep_ClosestSiTrackZ0_3);
  fChain->SetBranchAddress("lep_ClosestSiTrackQ_3", &lep_ClosestSiTrackQ_3, &b_lep_ClosestSiTrackQ_3);
  fChain->SetBranchAddress("lep_Mtrktrk_atConvV_CO_3", &lep_Mtrktrk_atConvV_CO_3, &b_lep_Mtrktrk_atConvV_CO_3);
  fChain->SetBranchAddress("lep_Mtrktrk_atPV_CO_3", &lep_Mtrktrk_atPV_CO_3, &b_lep_Mtrktrk_atPV_CO_3);
  fChain->SetBranchAddress("lep_RadiusCOX_3", &lep_RadiusCOX_3, &b_lep_RadiusCOX_3);
  fChain->SetBranchAddress("lep_RadiusCOY_3", &lep_RadiusCOY_3, &b_lep_RadiusCOY_3);
  fChain->SetBranchAddress("electron_ambiguityType", &electron_ambiguityType, &b_electron_ambiguityType);
  fChain->SetBranchAddress("electron_PromptLeptonIso_TagWeight", &electron_PromptLeptonIso_TagWeight, &b_electron_PromptLeptonIso_TagWeight);
  fChain->SetBranchAddress("electron_PromptLeptonVeto_TagWeight", &electron_PromptLeptonVeto_TagWeight, &b_electron_PromptLeptonVeto_TagWeight);
  fChain->SetBranchAddress("muon_PromptLeptonIso_TagWeight", &muon_PromptLeptonIso_TagWeight, &b_muon_PromptLeptonIso_TagWeight);
  fChain->SetBranchAddress("muon_PromptLeptonVeto_TagWeight", &muon_PromptLeptonVeto_TagWeight, &b_muon_PromptLeptonVeto_TagWeight);
  fChain->SetBranchAddress("tau_MV2c10_0", &tau_MV2c10_0, &b_tau_MV2c10_0);
  fChain->SetBranchAddress("tau_MV2c10_1", &tau_MV2c10_1, &b_tau_MV2c10_1);
  fChain->SetBranchAddress("tau_pt_0", &tau_pt_0, &b_tau_pt_0);
  fChain->SetBranchAddress("tau_eta_0", &tau_eta_0, &b_tau_eta_0);
  fChain->SetBranchAddress("tau_phi_0", &tau_phi_0, &b_tau_phi_0);
  fChain->SetBranchAddress("tau_charge_0", &tau_charge_0, &b_tau_charge_0);
  fChain->SetBranchAddress("tau_E_0", &tau_E_0, &b_tau_E_0);
  fChain->SetBranchAddress("tau_BDTJetScore_0", &tau_BDTJetScore_0, &b_tau_BDTJetScore_0);
  fChain->SetBranchAddress("tau_JetBDTSigLoose_0", &tau_JetBDTSigLoose_0, &b_tau_JetBDTSigLoose_0);
  fChain->SetBranchAddress("tau_JetBDTSigMedium_0", &tau_JetBDTSigMedium_0, &b_tau_JetBDTSigMedium_0);
  fChain->SetBranchAddress("tau_JetBDTSigTight_0", &tau_JetBDTSigTight_0, &b_tau_JetBDTSigTight_0);
  fChain->SetBranchAddress("tau_isHadronic_0", &tau_isHadronic_0, &b_tau_isHadronic_0);
  fChain->SetBranchAddress("tau_numTrack_0", &tau_numTrack_0, &b_tau_numTrack_0);
  fChain->SetBranchAddress("tau_SFTight_0", &tau_SFTight_0, &b_tau_SFTight_0);
  fChain->SetBranchAddress("tau_SFLoose_0", &tau_SFLoose_0, &b_tau_SFLoose_0);
  fChain->SetBranchAddress("tau_tagWeightBin_0", &tau_tagWeightBin_0, &b_tau_tagWeightBin_0);
  fChain->SetBranchAddress("tau_fromPV_0", &tau_fromPV_0, &b_tau_fromPV_0);
  fChain->SetBranchAddress("tau_passEleOLR_0", &tau_passEleOLR_0, &b_tau_passEleOLR_0);
  fChain->SetBranchAddress("tau_passEleBDT_0", &tau_passEleBDT_0, &b_tau_passEleBDT_0);
  fChain->SetBranchAddress("tau_passMuonOLR_0", &tau_passMuonOLR_0, &b_tau_passMuonOLR_0);
  fChain->SetBranchAddress("tau_truthOrigin_0", &tau_truthOrigin_0, &b_tau_truthOrigin_0);
  fChain->SetBranchAddress("tau_truthType_0", &tau_truthType_0, &b_tau_truthType_0);
  fChain->SetBranchAddress("tau_truthJetFlavour_0", &tau_truthJetFlavour_0, &b_tau_truthJetFlavour_0);
  fChain->SetBranchAddress("tau_pt_1", &tau_pt_1, &b_tau_pt_1);
  fChain->SetBranchAddress("tau_eta_1", &tau_eta_1, &b_tau_eta_1);
  fChain->SetBranchAddress("tau_phi_1", &tau_phi_1, &b_tau_phi_1);
  fChain->SetBranchAddress("tau_charge_1", &tau_charge_1, &b_tau_charge_1);
  fChain->SetBranchAddress("tau_E_1", &tau_E_1, &b_tau_E_1);
  fChain->SetBranchAddress("tau_BDTJetScore_1", &tau_BDTJetScore_1, &b_tau_BDTJetScore_1);
  fChain->SetBranchAddress("tau_JetBDTSigLoose_1", &tau_JetBDTSigLoose_1, &b_tau_JetBDTSigLoose_1);
  fChain->SetBranchAddress("tau_JetBDTSigMedium_1", &tau_JetBDTSigMedium_1, &b_tau_JetBDTSigMedium_1);
  fChain->SetBranchAddress("tau_JetBDTSigTight_1", &tau_JetBDTSigTight_1, &b_tau_JetBDTSigTight_1);
  fChain->SetBranchAddress("tau_numTrack_1", &tau_numTrack_1, &b_tau_numTrack_1);
  fChain->SetBranchAddress("tau_isHadronic_1", &tau_isHadronic_1, &b_tau_isHadronic_1);
  fChain->SetBranchAddress("tau_SFTight_1", &tau_SFTight_1, &b_tau_SFTight_1);
  fChain->SetBranchAddress("tau_SFLoose_1", &tau_SFLoose_1, &b_tau_SFLoose_1);
  fChain->SetBranchAddress("tau_tagWeightBin_1", &tau_tagWeightBin_1, &b_tau_tagWeightBin_1);
  fChain->SetBranchAddress("tau_fromPV_1", &tau_fromPV_1, &b_tau_fromPV_1);
  fChain->SetBranchAddress("tau_passEleOLR_1", &tau_passEleOLR_1, &b_tau_passEleOLR_1);
  fChain->SetBranchAddress("tau_passEleBDT_1", &tau_passEleBDT_1, &b_tau_passEleBDT_1);
  fChain->SetBranchAddress("tau_passMuonOLR_1", &tau_passMuonOLR_1, &b_tau_passMuonOLR_1);
  fChain->SetBranchAddress("tau_truthOrigin_1", &tau_truthOrigin_1, &b_tau_truthOrigin_1);
  fChain->SetBranchAddress("tau_truthType_1", &tau_truthType_1, &b_tau_truthType_1);
  fChain->SetBranchAddress("tau_truthJetFlavour_1", &tau_truthJetFlavour_1, &b_tau_truthJetFlavour_1);
  fChain->SetBranchAddress("tau_promptTauVeto_0", &tau_promptTauVeto_0, &b_tau_promptTauVeto_0);
  fChain->SetBranchAddress("tau_promptTauVeto_1", &tau_promptTauVeto_1, &b_tau_promptTauVeto_1);
  fChain->SetBranchAddress("onelep_type", &onelep_type, &b_onelep_type);
  fChain->SetBranchAddress("dilep_type", &dilep_type, &b_dilep_type);
  fChain->SetBranchAddress("trilep_type", &trilep_type, &b_trilep_type);
  fChain->SetBranchAddress("quadlep_type", &quadlep_type, &b_quadlep_type);
  fChain->SetBranchAddress("total_charge", &total_charge, &b_total_charge);
  fChain->SetBranchAddress("total_leptons", &total_leptons, &b_total_leptons);
  fChain->SetBranchAddress("isQMisIDEvent", &isQMisIDEvent, &b_isQMisIDEvent);
  fChain->SetBranchAddress("isFakeEvent", &isFakeEvent, &b_isFakeEvent);
  fChain->SetBranchAddress("isLepFromPhEvent", &isLepFromPhEvent, &b_isLepFromPhEvent);
  fChain->SetBranchAddress("Mll01", &Mll01, &b_Mll01);
  fChain->SetBranchAddress("Ptll01", &Ptll01, &b_Ptll01);
  fChain->SetBranchAddress("DRll01", &DRll01, &b_DRll01);
  fChain->SetBranchAddress("Mlll012", &Mlll012, &b_Mlll012);
  fChain->SetBranchAddress("Mllll0123", &Mllll0123, &b_Mllll0123);
  fChain->SetBranchAddress("Mlll013", &Mlll013, &b_Mlll013);
  fChain->SetBranchAddress("Mll02", &Mll02, &b_Mll02);
  fChain->SetBranchAddress("Ptll02", &Ptll02, &b_Ptll02);
  fChain->SetBranchAddress("DRll02", &DRll02, &b_DRll02);
  fChain->SetBranchAddress("Mlll023", &Mlll023, &b_Mlll023);
  fChain->SetBranchAddress("Mll03", &Mll03, &b_Mll03);
  fChain->SetBranchAddress("Ptll03", &Ptll03, &b_Ptll03);
  fChain->SetBranchAddress("DRll03", &DRll03, &b_DRll03);
  fChain->SetBranchAddress("Mll12", &Mll12, &b_Mll12);
  fChain->SetBranchAddress("Ptll12", &Ptll12, &b_Ptll12);
  fChain->SetBranchAddress("DRll12", &DRll12, &b_DRll12);
  fChain->SetBranchAddress("Mlll123", &Mlll123, &b_Mlll123);
  fChain->SetBranchAddress("Mll13", &Mll13, &b_Mll13);
  fChain->SetBranchAddress("Ptll13", &Ptll13, &b_Ptll13);
  fChain->SetBranchAddress("DRll13", &DRll13, &b_DRll13);
  fChain->SetBranchAddress("Mll23", &Mll23, &b_Mll23);
  fChain->SetBranchAddress("Ptll23", &Ptll23, &b_Ptll23);
  fChain->SetBranchAddress("DRll23", &DRll23, &b_DRll23);
  fChain->SetBranchAddress("matchDLTll01", &matchDLTll01, &b_matchDLTll01);
  fChain->SetBranchAddress("matchDLTll02", &matchDLTll02, &b_matchDLTll02);
  fChain->SetBranchAddress("matchDLTll12", &matchDLTll12, &b_matchDLTll12);
  fChain->SetBranchAddress("matchDLTll03", &matchDLTll03, &b_matchDLTll03);
  fChain->SetBranchAddress("matchDLTll13", &matchDLTll13, &b_matchDLTll13);
  fChain->SetBranchAddress("matchDLTll23", &matchDLTll23, &b_matchDLTll23);
  fChain->SetBranchAddress("best_Z_Mll", &best_Z_Mll, &b_best_Z_Mll);
  fChain->SetBranchAddress("best_Z_other_Mll", &best_Z_other_Mll, &b_best_Z_other_Mll);
  fChain->SetBranchAddress("best_Z_other_MtLepMet", &best_Z_other_MtLepMet, &b_best_Z_other_MtLepMet);
  fChain->SetBranchAddress("nJets_OR_T", &nJets_OR_T, &b_nJets_OR_T);
  fChain->SetBranchAddress("nJets_OR", &nJets_OR, &b_nJets_OR);
  fChain->SetBranchAddress("nJets_OR_T_MV2c20_60", &nJets_OR_T_MV2c20_60, &b_nJets_OR_T_MV2c20_60);
  fChain->SetBranchAddress("nJets_OR_T_MV2c20_70", &nJets_OR_T_MV2c20_70, &b_nJets_OR_T_MV2c20_70);
  fChain->SetBranchAddress("nJets_OR_T_MV2c20_77", &nJets_OR_T_MV2c20_77, &b_nJets_OR_T_MV2c20_77);
  fChain->SetBranchAddress("nJets_OR_T_MV2c20_85", &nJets_OR_T_MV2c20_85, &b_nJets_OR_T_MV2c20_85);
  fChain->SetBranchAddress("nJets_OR_MV2c20_85", &nJets_OR_MV2c20_85, &b_nJets_OR_MV2c20_85);
  fChain->SetBranchAddress("nJets_OR_MV2c20_60", &nJets_OR_MV2c20_60, &b_nJets_OR_MV2c20_60);
  fChain->SetBranchAddress("nJets_OR_MV2c20_77", &nJets_OR_MV2c20_77, &b_nJets_OR_MV2c20_77);
  fChain->SetBranchAddress("nJets_OR_MV2c20_70", &nJets_OR_MV2c20_70, &b_nJets_OR_MV2c20_70);
  fChain->SetBranchAddress("nJets_OR_T_MV2c10_60", &nJets_OR_T_MV2c10_60, &b_nJets_OR_T_MV2c10_60);
  fChain->SetBranchAddress("nJets_OR_T_MV2c10_70", &nJets_OR_T_MV2c10_70, &b_nJets_OR_T_MV2c10_70);
  fChain->SetBranchAddress("nJets_OR_T_MV2c10_77", &nJets_OR_T_MV2c10_77, &b_nJets_OR_T_MV2c10_77);
  fChain->SetBranchAddress("nJets_OR_T_MV2c10_85", &nJets_OR_T_MV2c10_85, &b_nJets_OR_T_MV2c10_85);
  fChain->SetBranchAddress("nJets_OR_MV2c10_85", &nJets_OR_MV2c10_85, &b_nJets_OR_MV2c10_85);
  fChain->SetBranchAddress("nJets_OR_MV2c10_60", &nJets_OR_MV2c10_60, &b_nJets_OR_MV2c10_60);
  fChain->SetBranchAddress("nJets_OR_MV2c10_77", &nJets_OR_MV2c10_77, &b_nJets_OR_MV2c10_77);
  fChain->SetBranchAddress("nJets_OR_MV2c10_70", &nJets_OR_MV2c10_70, &b_nJets_OR_MV2c10_70);
  fChain->SetBranchAddress("nTaus_OR_Pt25", &nTaus_OR_Pt25, &b_nTaus_OR_Pt25);
  fChain->SetBranchAddress("isBlinded", &isBlinded, &b_isBlinded);
  fChain->SetBranchAddress("HT", &HT, &b_HT);
  fChain->SetBranchAddress("HT_lep", &HT_lep, &b_HT_lep);
  fChain->SetBranchAddress("HT_jets", &HT_jets, &b_HT_jets);
  fChain->SetBranchAddress("lead_jetPt", &lead_jetPt, &b_lead_jetPt);
  fChain->SetBranchAddress("lead_jetEta", &lead_jetEta, &b_lead_jetEta);
  fChain->SetBranchAddress("lead_jetPhi", &lead_jetPhi, &b_lead_jetPhi);
  fChain->SetBranchAddress("lead_jetE", &lead_jetE, &b_lead_jetE);
  fChain->SetBranchAddress("sublead_jetPt", &sublead_jetPt, &b_sublead_jetPt);
  fChain->SetBranchAddress("sublead_jetEta", &sublead_jetEta, &b_sublead_jetEta);
  fChain->SetBranchAddress("sublead_jetPhi", &sublead_jetPhi, &b_sublead_jetPhi);
  fChain->SetBranchAddress("sublead_jetE", &sublead_jetE, &b_sublead_jetE);
  fChain->SetBranchAddress("lepSFTrigLoose", &lepSFTrigLoose, &b_lepSFTrigLoose);
  fChain->SetBranchAddress("lepSFTrigTight", &lepSFTrigTight, &b_lepSFTrigTight);
  fChain->SetBranchAddress("lepSFTrigTightLoose", &lepSFTrigTightLoose, &b_lepSFTrigTightLoose);
  fChain->SetBranchAddress("lepSFTrigLooseTight", &lepSFTrigLooseTight, &b_lepSFTrigLooseTight);
  fChain->SetBranchAddress("lepEffTrigLoose", &lepEffTrigLoose, &b_lepEffTrigLoose);
  fChain->SetBranchAddress("lepEffTrigTight", &lepEffTrigTight, &b_lepEffTrigTight);
  fChain->SetBranchAddress("lepEffTrigTightLoose", &lepEffTrigTightLoose, &b_lepEffTrigTightLoose);
  fChain->SetBranchAddress("lepEffTrigLooseTight", &lepEffTrigLooseTight, &b_lepEffTrigLooseTight);
  fChain->SetBranchAddress("lepDataEffTrigLoose", &lepDataEffTrigLoose, &b_lepDataEffTrigLoose);
  fChain->SetBranchAddress("lepDataEffTrigTight", &lepDataEffTrigTight, &b_lepDataEffTrigTight);
  fChain->SetBranchAddress("lepDataEffTrigTightLoose", &lepDataEffTrigTightLoose, &b_lepDataEffTrigTightLoose);
  fChain->SetBranchAddress("lepDataEffTrigLooseTight", &lepDataEffTrigLooseTight, &b_lepDataEffTrigLooseTight);
  fChain->SetBranchAddress("lepSFObjLoose", &lepSFObjLoose, &b_lepSFObjLoose);
  fChain->SetBranchAddress("lepSFObjTight", &lepSFObjTight, &b_lepSFObjTight);
  fChain->SetBranchAddress("tauSFTight", &tauSFTight, &b_tauSFTight);
  fChain->SetBranchAddress("tauSFLoose", &tauSFLoose, &b_tauSFLoose);
  fChain->SetBranchAddress("isMMSideband_TT", &isMMSideband_TT, &b_isMMSideband_TT);
  fChain->SetBranchAddress("isMMSideband_TAntiT", &isMMSideband_TAntiT, &b_isMMSideband_TAntiT);
  fChain->SetBranchAddress("isMMSideband_AntiTT", &isMMSideband_AntiTT, &b_isMMSideband_AntiTT);
  fChain->SetBranchAddress("isMMSideband_AntiTAntiT", &isMMSideband_AntiTAntiT, &b_isMMSideband_AntiTAntiT);
  fChain->SetBranchAddress("MM_EventWeight", &MM_EventWeight, &b_MM_EventWeight);
  fChain->SetBranchAddress("MM_EventWeight_Fake_El_NBJets_VS_Pt_D_QMisID_10_Up", &MM_EventWeight_Fake_El_NBJets_VS_Pt_D_QMisID_10_Up, &b_MM_EventWeight_Fake_El_NBJets_VS_Pt_D_QMisID_10_Up);
  fChain->SetBranchAddress("MM_EventWeight_Fake_El_NBJets_VS_Pt_D_QMisID_10_Dn", &MM_EventWeight_Fake_El_NBJets_VS_Pt_D_QMisID_10_Dn, &b_MM_EventWeight_Fake_El_NBJets_VS_Pt_D_QMisID_10_Dn);
  fChain->SetBranchAddress("MM_EventWeight_Fake_El_NBJets_VS_Pt_D_QMisID_5_Up", &MM_EventWeight_Fake_El_NBJets_VS_Pt_D_QMisID_5_Up, &b_MM_EventWeight_Fake_El_NBJets_VS_Pt_D_QMisID_5_Up);
  fChain->SetBranchAddress("MM_EventWeight_Fake_El_NBJets_VS_Pt_D_QMisID_5_Dn", &MM_EventWeight_Fake_El_NBJets_VS_Pt_D_QMisID_5_Dn, &b_MM_EventWeight_Fake_El_NBJets_VS_Pt_D_QMisID_5_Dn);
  fChain->SetBranchAddress("MM_EventWeight_Fake_El_NBJets_VS_Pt_D_QMisID_6_Up", &MM_EventWeight_Fake_El_NBJets_VS_Pt_D_QMisID_6_Up, &b_MM_EventWeight_Fake_El_NBJets_VS_Pt_D_QMisID_6_Up);
  fChain->SetBranchAddress("MM_EventWeight_Fake_El_NBJets_VS_Pt_D_QMisID_6_Dn", &MM_EventWeight_Fake_El_NBJets_VS_Pt_D_QMisID_6_Dn, &b_MM_EventWeight_Fake_El_NBJets_VS_Pt_D_QMisID_6_Dn);
  fChain->SetBranchAddress("MM_EventWeight_Fake_El_NBJets_VS_Pt_D_QMisID_9_Up", &MM_EventWeight_Fake_El_NBJets_VS_Pt_D_QMisID_9_Up, &b_MM_EventWeight_Fake_El_NBJets_VS_Pt_D_QMisID_9_Up);
  fChain->SetBranchAddress("MM_EventWeight_Fake_El_NBJets_VS_Pt_D_QMisID_9_Dn", &MM_EventWeight_Fake_El_NBJets_VS_Pt_D_QMisID_9_Dn, &b_MM_EventWeight_Fake_El_NBJets_VS_Pt_D_QMisID_9_Dn);
  fChain->SetBranchAddress("MM_EventWeight_Fake_El_NBJets_VS_Pt_ND_OtherPromptSS_Up", &MM_EventWeight_Fake_El_NBJets_VS_Pt_ND_OtherPromptSS_Up, &b_MM_EventWeight_Fake_El_NBJets_VS_Pt_ND_OtherPromptSS_Up);
  fChain->SetBranchAddress("MM_EventWeight_Fake_El_NBJets_VS_Pt_ND_OtherPromptSS_Dn", &MM_EventWeight_Fake_El_NBJets_VS_Pt_ND_OtherPromptSS_Dn, &b_MM_EventWeight_Fake_El_NBJets_VS_Pt_ND_OtherPromptSS_Dn);
  fChain->SetBranchAddress("MM_EventWeight_Fake_El_NBJets_VS_Pt_ND_TTV_Up", &MM_EventWeight_Fake_El_NBJets_VS_Pt_ND_TTV_Up, &b_MM_EventWeight_Fake_El_NBJets_VS_Pt_ND_TTV_Up);
  fChain->SetBranchAddress("MM_EventWeight_Fake_El_NBJets_VS_Pt_ND_TTV_Dn", &MM_EventWeight_Fake_El_NBJets_VS_Pt_ND_TTV_Dn, &b_MM_EventWeight_Fake_El_NBJets_VS_Pt_ND_TTV_Dn);
  fChain->SetBranchAddress("MM_EventWeight_Fake_El_NBJets_VS_Pt_ND_VV_Up", &MM_EventWeight_Fake_El_NBJets_VS_Pt_ND_VV_Up, &b_MM_EventWeight_Fake_El_NBJets_VS_Pt_ND_VV_Up);
  fChain->SetBranchAddress("MM_EventWeight_Fake_El_NBJets_VS_Pt_ND_VV_Dn", &MM_EventWeight_Fake_El_NBJets_VS_Pt_ND_VV_Dn, &b_MM_EventWeight_Fake_El_NBJets_VS_Pt_ND_VV_Dn);
  fChain->SetBranchAddress("MM_EventWeight_Fake_El_NBJets_VS_Pt_N_QMisID_10_Up", &MM_EventWeight_Fake_El_NBJets_VS_Pt_N_QMisID_10_Up, &b_MM_EventWeight_Fake_El_NBJets_VS_Pt_N_QMisID_10_Up);
  fChain->SetBranchAddress("MM_EventWeight_Fake_El_NBJets_VS_Pt_N_QMisID_10_Dn", &MM_EventWeight_Fake_El_NBJets_VS_Pt_N_QMisID_10_Dn, &b_MM_EventWeight_Fake_El_NBJets_VS_Pt_N_QMisID_10_Dn);
  fChain->SetBranchAddress("MM_EventWeight_Fake_El_NBJets_VS_Pt_N_QMisID_5_Up", &MM_EventWeight_Fake_El_NBJets_VS_Pt_N_QMisID_5_Up, &b_MM_EventWeight_Fake_El_NBJets_VS_Pt_N_QMisID_5_Up);
  fChain->SetBranchAddress("MM_EventWeight_Fake_El_NBJets_VS_Pt_N_QMisID_5_Dn", &MM_EventWeight_Fake_El_NBJets_VS_Pt_N_QMisID_5_Dn, &b_MM_EventWeight_Fake_El_NBJets_VS_Pt_N_QMisID_5_Dn);
  fChain->SetBranchAddress("MM_EventWeight_Fake_El_NBJets_VS_Pt_N_QMisID_6_Up", &MM_EventWeight_Fake_El_NBJets_VS_Pt_N_QMisID_6_Up, &b_MM_EventWeight_Fake_El_NBJets_VS_Pt_N_QMisID_6_Up);
  fChain->SetBranchAddress("MM_EventWeight_Fake_El_NBJets_VS_Pt_N_QMisID_6_Dn", &MM_EventWeight_Fake_El_NBJets_VS_Pt_N_QMisID_6_Dn, &b_MM_EventWeight_Fake_El_NBJets_VS_Pt_N_QMisID_6_Dn);
  fChain->SetBranchAddress("MM_EventWeight_Fake_El_NBJets_VS_Pt_N_QMisID_9_Up", &MM_EventWeight_Fake_El_NBJets_VS_Pt_N_QMisID_9_Up, &b_MM_EventWeight_Fake_El_NBJets_VS_Pt_N_QMisID_9_Up);
  fChain->SetBranchAddress("MM_EventWeight_Fake_El_NBJets_VS_Pt_N_QMisID_9_Dn", &MM_EventWeight_Fake_El_NBJets_VS_Pt_N_QMisID_9_Dn, &b_MM_EventWeight_Fake_El_NBJets_VS_Pt_N_QMisID_9_Dn);
  fChain->SetBranchAddress("MM_EventWeight_Fake_El_NBJets_VS_Pt_Stat_10_Up", &MM_EventWeight_Fake_El_NBJets_VS_Pt_Stat_10_Up, &b_MM_EventWeight_Fake_El_NBJets_VS_Pt_Stat_10_Up);
  fChain->SetBranchAddress("MM_EventWeight_Fake_El_NBJets_VS_Pt_Stat_10_Dn", &MM_EventWeight_Fake_El_NBJets_VS_Pt_Stat_10_Dn, &b_MM_EventWeight_Fake_El_NBJets_VS_Pt_Stat_10_Dn);
  fChain->SetBranchAddress("MM_EventWeight_Fake_El_NBJets_VS_Pt_Stat_5_Up", &MM_EventWeight_Fake_El_NBJets_VS_Pt_Stat_5_Up, &b_MM_EventWeight_Fake_El_NBJets_VS_Pt_Stat_5_Up);
  fChain->SetBranchAddress("MM_EventWeight_Fake_El_NBJets_VS_Pt_Stat_5_Dn", &MM_EventWeight_Fake_El_NBJets_VS_Pt_Stat_5_Dn, &b_MM_EventWeight_Fake_El_NBJets_VS_Pt_Stat_5_Dn);
  fChain->SetBranchAddress("MM_EventWeight_Fake_El_NBJets_VS_Pt_Stat_6_Up", &MM_EventWeight_Fake_El_NBJets_VS_Pt_Stat_6_Up, &b_MM_EventWeight_Fake_El_NBJets_VS_Pt_Stat_6_Up);
  fChain->SetBranchAddress("MM_EventWeight_Fake_El_NBJets_VS_Pt_Stat_6_Dn", &MM_EventWeight_Fake_El_NBJets_VS_Pt_Stat_6_Dn, &b_MM_EventWeight_Fake_El_NBJets_VS_Pt_Stat_6_Dn);
  fChain->SetBranchAddress("MM_EventWeight_Fake_El_NBJets_VS_Pt_Stat_9_Up", &MM_EventWeight_Fake_El_NBJets_VS_Pt_Stat_9_Up, &b_MM_EventWeight_Fake_El_NBJets_VS_Pt_Stat_9_Up);
  fChain->SetBranchAddress("MM_EventWeight_Fake_El_NBJets_VS_Pt_Stat_9_Dn", &MM_EventWeight_Fake_El_NBJets_VS_Pt_Stat_9_Dn, &b_MM_EventWeight_Fake_El_NBJets_VS_Pt_Stat_9_Dn);
  fChain->SetBranchAddress("MM_EventWeight_Fake_Mu_DistanceClosestJet_VS_Pt_ND_OtherPromptSS_Up", &MM_EventWeight_Fake_Mu_DistanceClosestJet_VS_Pt_ND_OtherPromptSS_Up, &b_MM_EventWeight_Fake_Mu_DistanceClosestJet_VS_Pt_ND_OtherPromptSS_Up);
  fChain->SetBranchAddress("MM_EventWeight_Fake_Mu_DistanceClosestJet_VS_Pt_ND_OtherPromptSS_Dn", &MM_EventWeight_Fake_Mu_DistanceClosestJet_VS_Pt_ND_OtherPromptSS_Dn, &b_MM_EventWeight_Fake_Mu_DistanceClosestJet_VS_Pt_ND_OtherPromptSS_Dn);
  fChain->SetBranchAddress("MM_EventWeight_Fake_Mu_DistanceClosestJet_VS_Pt_ND_TTV_Up", &MM_EventWeight_Fake_Mu_DistanceClosestJet_VS_Pt_ND_TTV_Up, &b_MM_EventWeight_Fake_Mu_DistanceClosestJet_VS_Pt_ND_TTV_Up);
  fChain->SetBranchAddress("MM_EventWeight_Fake_Mu_DistanceClosestJet_VS_Pt_ND_TTV_Dn", &MM_EventWeight_Fake_Mu_DistanceClosestJet_VS_Pt_ND_TTV_Dn, &b_MM_EventWeight_Fake_Mu_DistanceClosestJet_VS_Pt_ND_TTV_Dn);
  fChain->SetBranchAddress("MM_EventWeight_Fake_Mu_DistanceClosestJet_VS_Pt_ND_VV_Up", &MM_EventWeight_Fake_Mu_DistanceClosestJet_VS_Pt_ND_VV_Up, &b_MM_EventWeight_Fake_Mu_DistanceClosestJet_VS_Pt_ND_VV_Up);
  fChain->SetBranchAddress("MM_EventWeight_Fake_Mu_DistanceClosestJet_VS_Pt_ND_VV_Dn", &MM_EventWeight_Fake_Mu_DistanceClosestJet_VS_Pt_ND_VV_Dn, &b_MM_EventWeight_Fake_Mu_DistanceClosestJet_VS_Pt_ND_VV_Dn);
  fChain->SetBranchAddress("MM_EventWeight_Fake_Mu_DistanceClosestJet_VS_Pt_Stat_10_Up", &MM_EventWeight_Fake_Mu_DistanceClosestJet_VS_Pt_Stat_10_Up, &b_MM_EventWeight_Fake_Mu_DistanceClosestJet_VS_Pt_Stat_10_Up);
  fChain->SetBranchAddress("MM_EventWeight_Fake_Mu_DistanceClosestJet_VS_Pt_Stat_10_Dn", &MM_EventWeight_Fake_Mu_DistanceClosestJet_VS_Pt_Stat_10_Dn, &b_MM_EventWeight_Fake_Mu_DistanceClosestJet_VS_Pt_Stat_10_Dn);
  fChain->SetBranchAddress("MM_EventWeight_Fake_Mu_DistanceClosestJet_VS_Pt_Stat_13_Up", &MM_EventWeight_Fake_Mu_DistanceClosestJet_VS_Pt_Stat_13_Up, &b_MM_EventWeight_Fake_Mu_DistanceClosestJet_VS_Pt_Stat_13_Up);
  fChain->SetBranchAddress("MM_EventWeight_Fake_Mu_DistanceClosestJet_VS_Pt_Stat_13_Dn", &MM_EventWeight_Fake_Mu_DistanceClosestJet_VS_Pt_Stat_13_Dn, &b_MM_EventWeight_Fake_Mu_DistanceClosestJet_VS_Pt_Stat_13_Dn);
  fChain->SetBranchAddress("MM_EventWeight_Fake_Mu_DistanceClosestJet_VS_Pt_Stat_14_Up", &MM_EventWeight_Fake_Mu_DistanceClosestJet_VS_Pt_Stat_14_Up, &b_MM_EventWeight_Fake_Mu_DistanceClosestJet_VS_Pt_Stat_14_Up);
  fChain->SetBranchAddress("MM_EventWeight_Fake_Mu_DistanceClosestJet_VS_Pt_Stat_14_Dn", &MM_EventWeight_Fake_Mu_DistanceClosestJet_VS_Pt_Stat_14_Dn, &b_MM_EventWeight_Fake_Mu_DistanceClosestJet_VS_Pt_Stat_14_Dn);
  fChain->SetBranchAddress("MM_EventWeight_Fake_Mu_DistanceClosestJet_VS_Pt_Stat_5_Up", &MM_EventWeight_Fake_Mu_DistanceClosestJet_VS_Pt_Stat_5_Up, &b_MM_EventWeight_Fake_Mu_DistanceClosestJet_VS_Pt_Stat_5_Up);
  fChain->SetBranchAddress("MM_EventWeight_Fake_Mu_DistanceClosestJet_VS_Pt_Stat_5_Dn", &MM_EventWeight_Fake_Mu_DistanceClosestJet_VS_Pt_Stat_5_Dn, &b_MM_EventWeight_Fake_Mu_DistanceClosestJet_VS_Pt_Stat_5_Dn);
  fChain->SetBranchAddress("MM_EventWeight_Fake_Mu_DistanceClosestJet_VS_Pt_Stat_6_Up", &MM_EventWeight_Fake_Mu_DistanceClosestJet_VS_Pt_Stat_6_Up, &b_MM_EventWeight_Fake_Mu_DistanceClosestJet_VS_Pt_Stat_6_Up);
  fChain->SetBranchAddress("MM_EventWeight_Fake_Mu_DistanceClosestJet_VS_Pt_Stat_6_Dn", &MM_EventWeight_Fake_Mu_DistanceClosestJet_VS_Pt_Stat_6_Dn, &b_MM_EventWeight_Fake_Mu_DistanceClosestJet_VS_Pt_Stat_6_Dn);
  fChain->SetBranchAddress("MM_EventWeight_Fake_Mu_DistanceClosestJet_VS_Pt_Stat_9_Up", &MM_EventWeight_Fake_Mu_DistanceClosestJet_VS_Pt_Stat_9_Up, &b_MM_EventWeight_Fake_Mu_DistanceClosestJet_VS_Pt_Stat_9_Up);
  fChain->SetBranchAddress("MM_EventWeight_Fake_Mu_DistanceClosestJet_VS_Pt_Stat_9_Dn", &MM_EventWeight_Fake_Mu_DistanceClosestJet_VS_Pt_Stat_9_Dn, &b_MM_EventWeight_Fake_Mu_DistanceClosestJet_VS_Pt_Stat_9_Dn);
  fChain->SetBranchAddress("MM_EventWeight_Real_El_Eta_VS_Pt_ND_FakesOS_Up", &MM_EventWeight_Real_El_Eta_VS_Pt_ND_FakesOS_Up, &b_MM_EventWeight_Real_El_Eta_VS_Pt_ND_FakesOS_Up);
  fChain->SetBranchAddress("MM_EventWeight_Real_El_Eta_VS_Pt_ND_FakesOS_Dn", &MM_EventWeight_Real_El_Eta_VS_Pt_ND_FakesOS_Dn, &b_MM_EventWeight_Real_El_Eta_VS_Pt_ND_FakesOS_Dn);
  fChain->SetBranchAddress("MM_EventWeight_Real_El_Eta_VS_Pt_Stat_10_Up", &MM_EventWeight_Real_El_Eta_VS_Pt_Stat_10_Up, &b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_10_Up);
  fChain->SetBranchAddress("MM_EventWeight_Real_El_Eta_VS_Pt_Stat_10_Dn", &MM_EventWeight_Real_El_Eta_VS_Pt_Stat_10_Dn, &b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_10_Dn);
  fChain->SetBranchAddress("MM_EventWeight_Real_El_Eta_VS_Pt_Stat_11_Up", &MM_EventWeight_Real_El_Eta_VS_Pt_Stat_11_Up, &b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_11_Up);
  fChain->SetBranchAddress("MM_EventWeight_Real_El_Eta_VS_Pt_Stat_11_Dn", &MM_EventWeight_Real_El_Eta_VS_Pt_Stat_11_Dn, &b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_11_Dn);
  fChain->SetBranchAddress("MM_EventWeight_Real_El_Eta_VS_Pt_Stat_12_Up", &MM_EventWeight_Real_El_Eta_VS_Pt_Stat_12_Up, &b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_12_Up);
  fChain->SetBranchAddress("MM_EventWeight_Real_El_Eta_VS_Pt_Stat_12_Dn", &MM_EventWeight_Real_El_Eta_VS_Pt_Stat_12_Dn, &b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_12_Dn);
  fChain->SetBranchAddress("MM_EventWeight_Real_El_Eta_VS_Pt_Stat_13_Up", &MM_EventWeight_Real_El_Eta_VS_Pt_Stat_13_Up, &b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_13_Up);
  fChain->SetBranchAddress("MM_EventWeight_Real_El_Eta_VS_Pt_Stat_13_Dn", &MM_EventWeight_Real_El_Eta_VS_Pt_Stat_13_Dn, &b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_13_Dn);
  fChain->SetBranchAddress("MM_EventWeight_Real_El_Eta_VS_Pt_Stat_14_Up", &MM_EventWeight_Real_El_Eta_VS_Pt_Stat_14_Up, &b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_14_Up);
  fChain->SetBranchAddress("MM_EventWeight_Real_El_Eta_VS_Pt_Stat_14_Dn", &MM_EventWeight_Real_El_Eta_VS_Pt_Stat_14_Dn, &b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_14_Dn);
  fChain->SetBranchAddress("MM_EventWeight_Real_El_Eta_VS_Pt_Stat_17_Up", &MM_EventWeight_Real_El_Eta_VS_Pt_Stat_17_Up, &b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_17_Up);
  fChain->SetBranchAddress("MM_EventWeight_Real_El_Eta_VS_Pt_Stat_17_Dn", &MM_EventWeight_Real_El_Eta_VS_Pt_Stat_17_Dn, &b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_17_Dn);
  fChain->SetBranchAddress("MM_EventWeight_Real_El_Eta_VS_Pt_Stat_18_Up", &MM_EventWeight_Real_El_Eta_VS_Pt_Stat_18_Up, &b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_18_Up);
  fChain->SetBranchAddress("MM_EventWeight_Real_El_Eta_VS_Pt_Stat_18_Dn", &MM_EventWeight_Real_El_Eta_VS_Pt_Stat_18_Dn, &b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_18_Dn);
  fChain->SetBranchAddress("MM_EventWeight_Real_El_Eta_VS_Pt_Stat_19_Up", &MM_EventWeight_Real_El_Eta_VS_Pt_Stat_19_Up, &b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_19_Up);
  fChain->SetBranchAddress("MM_EventWeight_Real_El_Eta_VS_Pt_Stat_19_Dn", &MM_EventWeight_Real_El_Eta_VS_Pt_Stat_19_Dn, &b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_19_Dn);
  fChain->SetBranchAddress("MM_EventWeight_Real_El_Eta_VS_Pt_Stat_20_Up", &MM_EventWeight_Real_El_Eta_VS_Pt_Stat_20_Up, &b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_20_Up);
  fChain->SetBranchAddress("MM_EventWeight_Real_El_Eta_VS_Pt_Stat_20_Dn", &MM_EventWeight_Real_El_Eta_VS_Pt_Stat_20_Dn, &b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_20_Dn);
  fChain->SetBranchAddress("MM_EventWeight_Real_El_Eta_VS_Pt_Stat_21_Up", &MM_EventWeight_Real_El_Eta_VS_Pt_Stat_21_Up, &b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_21_Up);
  fChain->SetBranchAddress("MM_EventWeight_Real_El_Eta_VS_Pt_Stat_21_Dn", &MM_EventWeight_Real_El_Eta_VS_Pt_Stat_21_Dn, &b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_21_Dn);
  fChain->SetBranchAddress("MM_EventWeight_Real_El_Eta_VS_Pt_Stat_22_Up", &MM_EventWeight_Real_El_Eta_VS_Pt_Stat_22_Up, &b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_22_Up);
  fChain->SetBranchAddress("MM_EventWeight_Real_El_Eta_VS_Pt_Stat_22_Dn", &MM_EventWeight_Real_El_Eta_VS_Pt_Stat_22_Dn, &b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_22_Dn);
  fChain->SetBranchAddress("MM_EventWeight_Real_El_Eta_VS_Pt_Stat_25_Up", &MM_EventWeight_Real_El_Eta_VS_Pt_Stat_25_Up, &b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_25_Up);
  fChain->SetBranchAddress("MM_EventWeight_Real_El_Eta_VS_Pt_Stat_25_Dn", &MM_EventWeight_Real_El_Eta_VS_Pt_Stat_25_Dn, &b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_25_Dn);
  fChain->SetBranchAddress("MM_EventWeight_Real_El_Eta_VS_Pt_Stat_26_Up", &MM_EventWeight_Real_El_Eta_VS_Pt_Stat_26_Up, &b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_26_Up);
  fChain->SetBranchAddress("MM_EventWeight_Real_El_Eta_VS_Pt_Stat_26_Dn", &MM_EventWeight_Real_El_Eta_VS_Pt_Stat_26_Dn, &b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_26_Dn);
  fChain->SetBranchAddress("MM_EventWeight_Real_El_Eta_VS_Pt_Stat_27_Up", &MM_EventWeight_Real_El_Eta_VS_Pt_Stat_27_Up, &b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_27_Up);
  fChain->SetBranchAddress("MM_EventWeight_Real_El_Eta_VS_Pt_Stat_27_Dn", &MM_EventWeight_Real_El_Eta_VS_Pt_Stat_27_Dn, &b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_27_Dn);
  fChain->SetBranchAddress("MM_EventWeight_Real_El_Eta_VS_Pt_Stat_28_Up", &MM_EventWeight_Real_El_Eta_VS_Pt_Stat_28_Up, &b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_28_Up);
  fChain->SetBranchAddress("MM_EventWeight_Real_El_Eta_VS_Pt_Stat_28_Dn", &MM_EventWeight_Real_El_Eta_VS_Pt_Stat_28_Dn, &b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_28_Dn);
  fChain->SetBranchAddress("MM_EventWeight_Real_El_Eta_VS_Pt_Stat_29_Up", &MM_EventWeight_Real_El_Eta_VS_Pt_Stat_29_Up, &b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_29_Up);
  fChain->SetBranchAddress("MM_EventWeight_Real_El_Eta_VS_Pt_Stat_29_Dn", &MM_EventWeight_Real_El_Eta_VS_Pt_Stat_29_Dn, &b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_29_Dn);
  fChain->SetBranchAddress("MM_EventWeight_Real_El_Eta_VS_Pt_Stat_30_Up", &MM_EventWeight_Real_El_Eta_VS_Pt_Stat_30_Up, &b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_30_Up);
  fChain->SetBranchAddress("MM_EventWeight_Real_El_Eta_VS_Pt_Stat_30_Dn", &MM_EventWeight_Real_El_Eta_VS_Pt_Stat_30_Dn, &b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_30_Dn);
  fChain->SetBranchAddress("MM_EventWeight_Real_El_Eta_VS_Pt_Stat_33_Up", &MM_EventWeight_Real_El_Eta_VS_Pt_Stat_33_Up, &b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_33_Up);
  fChain->SetBranchAddress("MM_EventWeight_Real_El_Eta_VS_Pt_Stat_33_Dn", &MM_EventWeight_Real_El_Eta_VS_Pt_Stat_33_Dn, &b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_33_Dn);
  fChain->SetBranchAddress("MM_EventWeight_Real_El_Eta_VS_Pt_Stat_34_Up", &MM_EventWeight_Real_El_Eta_VS_Pt_Stat_34_Up, &b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_34_Up);
  fChain->SetBranchAddress("MM_EventWeight_Real_El_Eta_VS_Pt_Stat_34_Dn", &MM_EventWeight_Real_El_Eta_VS_Pt_Stat_34_Dn, &b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_34_Dn);
  fChain->SetBranchAddress("MM_EventWeight_Real_El_Eta_VS_Pt_Stat_35_Up", &MM_EventWeight_Real_El_Eta_VS_Pt_Stat_35_Up, &b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_35_Up);
  fChain->SetBranchAddress("MM_EventWeight_Real_El_Eta_VS_Pt_Stat_35_Dn", &MM_EventWeight_Real_El_Eta_VS_Pt_Stat_35_Dn, &b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_35_Dn);
  fChain->SetBranchAddress("MM_EventWeight_Real_El_Eta_VS_Pt_Stat_36_Up", &MM_EventWeight_Real_El_Eta_VS_Pt_Stat_36_Up, &b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_36_Up);
  fChain->SetBranchAddress("MM_EventWeight_Real_El_Eta_VS_Pt_Stat_36_Dn", &MM_EventWeight_Real_El_Eta_VS_Pt_Stat_36_Dn, &b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_36_Dn);
  fChain->SetBranchAddress("MM_EventWeight_Real_El_Eta_VS_Pt_Stat_37_Up", &MM_EventWeight_Real_El_Eta_VS_Pt_Stat_37_Up, &b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_37_Up);
  fChain->SetBranchAddress("MM_EventWeight_Real_El_Eta_VS_Pt_Stat_37_Dn", &MM_EventWeight_Real_El_Eta_VS_Pt_Stat_37_Dn, &b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_37_Dn);
  fChain->SetBranchAddress("MM_EventWeight_Real_El_Eta_VS_Pt_Stat_38_Up", &MM_EventWeight_Real_El_Eta_VS_Pt_Stat_38_Up, &b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_38_Up);
  fChain->SetBranchAddress("MM_EventWeight_Real_El_Eta_VS_Pt_Stat_38_Dn", &MM_EventWeight_Real_El_Eta_VS_Pt_Stat_38_Dn, &b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_38_Dn);
  fChain->SetBranchAddress("MM_EventWeight_Real_El_Eta_VS_Pt_Stat_41_Up", &MM_EventWeight_Real_El_Eta_VS_Pt_Stat_41_Up, &b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_41_Up);
  fChain->SetBranchAddress("MM_EventWeight_Real_El_Eta_VS_Pt_Stat_41_Dn", &MM_EventWeight_Real_El_Eta_VS_Pt_Stat_41_Dn, &b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_41_Dn);
  fChain->SetBranchAddress("MM_EventWeight_Real_El_Eta_VS_Pt_Stat_42_Up", &MM_EventWeight_Real_El_Eta_VS_Pt_Stat_42_Up, &b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_42_Up);
  fChain->SetBranchAddress("MM_EventWeight_Real_El_Eta_VS_Pt_Stat_42_Dn", &MM_EventWeight_Real_El_Eta_VS_Pt_Stat_42_Dn, &b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_42_Dn);
  fChain->SetBranchAddress("MM_EventWeight_Real_El_Eta_VS_Pt_Stat_43_Up", &MM_EventWeight_Real_El_Eta_VS_Pt_Stat_43_Up, &b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_43_Up);
  fChain->SetBranchAddress("MM_EventWeight_Real_El_Eta_VS_Pt_Stat_43_Dn", &MM_EventWeight_Real_El_Eta_VS_Pt_Stat_43_Dn, &b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_43_Dn);
  fChain->SetBranchAddress("MM_EventWeight_Real_El_Eta_VS_Pt_Stat_44_Up", &MM_EventWeight_Real_El_Eta_VS_Pt_Stat_44_Up, &b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_44_Up);
  fChain->SetBranchAddress("MM_EventWeight_Real_El_Eta_VS_Pt_Stat_44_Dn", &MM_EventWeight_Real_El_Eta_VS_Pt_Stat_44_Dn, &b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_44_Dn);
  fChain->SetBranchAddress("MM_EventWeight_Real_El_Eta_VS_Pt_Stat_45_Up", &MM_EventWeight_Real_El_Eta_VS_Pt_Stat_45_Up, &b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_45_Up);
  fChain->SetBranchAddress("MM_EventWeight_Real_El_Eta_VS_Pt_Stat_45_Dn", &MM_EventWeight_Real_El_Eta_VS_Pt_Stat_45_Dn, &b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_45_Dn);
  fChain->SetBranchAddress("MM_EventWeight_Real_El_Eta_VS_Pt_Stat_46_Up", &MM_EventWeight_Real_El_Eta_VS_Pt_Stat_46_Up, &b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_46_Up);
  fChain->SetBranchAddress("MM_EventWeight_Real_El_Eta_VS_Pt_Stat_46_Dn", &MM_EventWeight_Real_El_Eta_VS_Pt_Stat_46_Dn, &b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_46_Dn);
  fChain->SetBranchAddress("MM_EventWeight_Real_El_Eta_VS_Pt_Stat_49_Up", &MM_EventWeight_Real_El_Eta_VS_Pt_Stat_49_Up, &b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_49_Up);
  fChain->SetBranchAddress("MM_EventWeight_Real_El_Eta_VS_Pt_Stat_49_Dn", &MM_EventWeight_Real_El_Eta_VS_Pt_Stat_49_Dn, &b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_49_Dn);
  fChain->SetBranchAddress("MM_EventWeight_Real_El_Eta_VS_Pt_Stat_50_Up", &MM_EventWeight_Real_El_Eta_VS_Pt_Stat_50_Up, &b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_50_Up);
  fChain->SetBranchAddress("MM_EventWeight_Real_El_Eta_VS_Pt_Stat_50_Dn", &MM_EventWeight_Real_El_Eta_VS_Pt_Stat_50_Dn, &b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_50_Dn);
  fChain->SetBranchAddress("MM_EventWeight_Real_El_Eta_VS_Pt_Stat_51_Up", &MM_EventWeight_Real_El_Eta_VS_Pt_Stat_51_Up, &b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_51_Up);
  fChain->SetBranchAddress("MM_EventWeight_Real_El_Eta_VS_Pt_Stat_51_Dn", &MM_EventWeight_Real_El_Eta_VS_Pt_Stat_51_Dn, &b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_51_Dn);
  fChain->SetBranchAddress("MM_EventWeight_Real_El_Eta_VS_Pt_Stat_52_Up", &MM_EventWeight_Real_El_Eta_VS_Pt_Stat_52_Up, &b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_52_Up);
  fChain->SetBranchAddress("MM_EventWeight_Real_El_Eta_VS_Pt_Stat_52_Dn", &MM_EventWeight_Real_El_Eta_VS_Pt_Stat_52_Dn, &b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_52_Dn);
  fChain->SetBranchAddress("MM_EventWeight_Real_El_Eta_VS_Pt_Stat_53_Up", &MM_EventWeight_Real_El_Eta_VS_Pt_Stat_53_Up, &b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_53_Up);
  fChain->SetBranchAddress("MM_EventWeight_Real_El_Eta_VS_Pt_Stat_53_Dn", &MM_EventWeight_Real_El_Eta_VS_Pt_Stat_53_Dn, &b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_53_Dn);
  fChain->SetBranchAddress("MM_EventWeight_Real_El_Eta_VS_Pt_Stat_54_Up", &MM_EventWeight_Real_El_Eta_VS_Pt_Stat_54_Up, &b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_54_Up);
  fChain->SetBranchAddress("MM_EventWeight_Real_El_Eta_VS_Pt_Stat_54_Dn", &MM_EventWeight_Real_El_Eta_VS_Pt_Stat_54_Dn, &b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_54_Dn);
  fChain->SetBranchAddress("MM_EventWeight_Real_El_Eta_VS_Pt_Stat_57_Up", &MM_EventWeight_Real_El_Eta_VS_Pt_Stat_57_Up, &b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_57_Up);
  fChain->SetBranchAddress("MM_EventWeight_Real_El_Eta_VS_Pt_Stat_57_Dn", &MM_EventWeight_Real_El_Eta_VS_Pt_Stat_57_Dn, &b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_57_Dn);
  fChain->SetBranchAddress("MM_EventWeight_Real_El_Eta_VS_Pt_Stat_58_Up", &MM_EventWeight_Real_El_Eta_VS_Pt_Stat_58_Up, &b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_58_Up);
  fChain->SetBranchAddress("MM_EventWeight_Real_El_Eta_VS_Pt_Stat_58_Dn", &MM_EventWeight_Real_El_Eta_VS_Pt_Stat_58_Dn, &b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_58_Dn);
  fChain->SetBranchAddress("MM_EventWeight_Real_El_Eta_VS_Pt_Stat_59_Up", &MM_EventWeight_Real_El_Eta_VS_Pt_Stat_59_Up, &b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_59_Up);
  fChain->SetBranchAddress("MM_EventWeight_Real_El_Eta_VS_Pt_Stat_59_Dn", &MM_EventWeight_Real_El_Eta_VS_Pt_Stat_59_Dn, &b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_59_Dn);
  fChain->SetBranchAddress("MM_EventWeight_Real_El_Eta_VS_Pt_Stat_60_Up", &MM_EventWeight_Real_El_Eta_VS_Pt_Stat_60_Up, &b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_60_Up);
  fChain->SetBranchAddress("MM_EventWeight_Real_El_Eta_VS_Pt_Stat_60_Dn", &MM_EventWeight_Real_El_Eta_VS_Pt_Stat_60_Dn, &b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_60_Dn);
  fChain->SetBranchAddress("MM_EventWeight_Real_El_Eta_VS_Pt_Stat_61_Up", &MM_EventWeight_Real_El_Eta_VS_Pt_Stat_61_Up, &b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_61_Up);
  fChain->SetBranchAddress("MM_EventWeight_Real_El_Eta_VS_Pt_Stat_61_Dn", &MM_EventWeight_Real_El_Eta_VS_Pt_Stat_61_Dn, &b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_61_Dn);
  fChain->SetBranchAddress("MM_EventWeight_Real_El_Eta_VS_Pt_Stat_62_Up", &MM_EventWeight_Real_El_Eta_VS_Pt_Stat_62_Up, &b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_62_Up);
  fChain->SetBranchAddress("MM_EventWeight_Real_El_Eta_VS_Pt_Stat_62_Dn", &MM_EventWeight_Real_El_Eta_VS_Pt_Stat_62_Dn, &b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_62_Dn);
  fChain->SetBranchAddress("MM_EventWeight_Real_El_Eta_VS_Pt_Stat_9_Up", &MM_EventWeight_Real_El_Eta_VS_Pt_Stat_9_Up, &b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_9_Up);
  fChain->SetBranchAddress("MM_EventWeight_Real_El_Eta_VS_Pt_Stat_9_Dn", &MM_EventWeight_Real_El_Eta_VS_Pt_Stat_9_Dn, &b_MM_EventWeight_Real_El_Eta_VS_Pt_Stat_9_Dn);
  fChain->SetBranchAddress("MM_EventWeight_Real_Mu_Pt_ND_FakesOS_Up", &MM_EventWeight_Real_Mu_Pt_ND_FakesOS_Up, &b_MM_EventWeight_Real_Mu_Pt_ND_FakesOS_Up);
  fChain->SetBranchAddress("MM_EventWeight_Real_Mu_Pt_ND_FakesOS_Dn", &MM_EventWeight_Real_Mu_Pt_ND_FakesOS_Dn, &b_MM_EventWeight_Real_Mu_Pt_ND_FakesOS_Dn);
  fChain->SetBranchAddress("MM_EventWeight_Real_Mu_Pt_Stat_1_Up", &MM_EventWeight_Real_Mu_Pt_Stat_1_Up, &b_MM_EventWeight_Real_Mu_Pt_Stat_1_Up);
  fChain->SetBranchAddress("MM_EventWeight_Real_Mu_Pt_Stat_1_Dn", &MM_EventWeight_Real_Mu_Pt_Stat_1_Dn, &b_MM_EventWeight_Real_Mu_Pt_Stat_1_Dn);
  fChain->SetBranchAddress("MM_EventWeight_Real_Mu_Pt_Stat_2_Up", &MM_EventWeight_Real_Mu_Pt_Stat_2_Up, &b_MM_EventWeight_Real_Mu_Pt_Stat_2_Up);
  fChain->SetBranchAddress("MM_EventWeight_Real_Mu_Pt_Stat_2_Dn", &MM_EventWeight_Real_Mu_Pt_Stat_2_Dn, &b_MM_EventWeight_Real_Mu_Pt_Stat_2_Dn);
  fChain->SetBranchAddress("MM_EventWeight_Real_Mu_Pt_Stat_3_Up", &MM_EventWeight_Real_Mu_Pt_Stat_3_Up, &b_MM_EventWeight_Real_Mu_Pt_Stat_3_Up);
  fChain->SetBranchAddress("MM_EventWeight_Real_Mu_Pt_Stat_3_Dn", &MM_EventWeight_Real_Mu_Pt_Stat_3_Dn, &b_MM_EventWeight_Real_Mu_Pt_Stat_3_Dn);
  fChain->SetBranchAddress("MM_EventWeight_Real_Mu_Pt_Stat_4_Up", &MM_EventWeight_Real_Mu_Pt_Stat_4_Up, &b_MM_EventWeight_Real_Mu_Pt_Stat_4_Up);
  fChain->SetBranchAddress("MM_EventWeight_Real_Mu_Pt_Stat_4_Dn", &MM_EventWeight_Real_Mu_Pt_Stat_4_Dn, &b_MM_EventWeight_Real_Mu_Pt_Stat_4_Dn);
  fChain->SetBranchAddress("MM_EventWeight_Real_Mu_Pt_Stat_5_Up", &MM_EventWeight_Real_Mu_Pt_Stat_5_Up, &b_MM_EventWeight_Real_Mu_Pt_Stat_5_Up);
  fChain->SetBranchAddress("MM_EventWeight_Real_Mu_Pt_Stat_5_Dn", &MM_EventWeight_Real_Mu_Pt_Stat_5_Dn, &b_MM_EventWeight_Real_Mu_Pt_Stat_5_Dn);
  fChain->SetBranchAddress("MM_EventWeight_Real_Mu_Pt_Stat_6_Up", &MM_EventWeight_Real_Mu_Pt_Stat_6_Up, &b_MM_EventWeight_Real_Mu_Pt_Stat_6_Up);
  fChain->SetBranchAddress("MM_EventWeight_Real_Mu_Pt_Stat_6_Dn", &MM_EventWeight_Real_Mu_Pt_Stat_6_Dn, &b_MM_EventWeight_Real_Mu_Pt_Stat_6_Dn);
  fChain->SetBranchAddress("MM_EventWeight_Real_Mu_Pt_Stat_7_Up", &MM_EventWeight_Real_Mu_Pt_Stat_7_Up, &b_MM_EventWeight_Real_Mu_Pt_Stat_7_Up);
  fChain->SetBranchAddress("MM_EventWeight_Real_Mu_Pt_Stat_7_Dn", &MM_EventWeight_Real_Mu_Pt_Stat_7_Dn, &b_MM_EventWeight_Real_Mu_Pt_Stat_7_Dn);
  fChain->SetBranchAddress("MM_EventWeight_Real_El_Pt_Stat_Grouped_Up", &MM_EventWeight_Real_El_Pt_Stat_Grouped_Up, &b_MM_EventWeight_Real_El_Pt_Stat_Grouped_Up);
  fChain->SetBranchAddress("MM_EventWeight_Real_El_Pt_Stat_Grouped_Dn", &MM_EventWeight_Real_El_Pt_Stat_Grouped_Dn, &b_MM_EventWeight_Real_El_Pt_Stat_Grouped_Dn);
  fChain->SetBranchAddress("MM_EventWeight_Real_El_Pt_Sys_Grouped_Up", &MM_EventWeight_Real_El_Pt_Sys_Grouped_Up, &b_MM_EventWeight_Real_El_Pt_Sys_Grouped_Up);
  fChain->SetBranchAddress("MM_EventWeight_Real_El_Pt_Sys_Grouped_Dn", &MM_EventWeight_Real_El_Pt_Sys_Grouped_Dn, &b_MM_EventWeight_Real_El_Pt_Sys_Grouped_Dn);
  fChain->SetBranchAddress("MM_EventWeight_Real_Mu_Pt_Stat_Grouped_Up", &MM_EventWeight_Real_Mu_Pt_Stat_Grouped_Up, &b_MM_EventWeight_Real_Mu_Pt_Stat_Grouped_Up);
  fChain->SetBranchAddress("MM_EventWeight_Real_Mu_Pt_Stat_Grouped_Dn", &MM_EventWeight_Real_Mu_Pt_Stat_Grouped_Dn, &b_MM_EventWeight_Real_Mu_Pt_Stat_Grouped_Dn);
  fChain->SetBranchAddress("MM_EventWeight_Real_Mu_Pt_Sys_Grouped_Up", &MM_EventWeight_Real_Mu_Pt_Sys_Grouped_Up, &b_MM_EventWeight_Real_Mu_Pt_Sys_Grouped_Up);
  fChain->SetBranchAddress("MM_EventWeight_Real_Mu_Pt_Sys_Grouped_Dn", &MM_EventWeight_Real_Mu_Pt_Sys_Grouped_Dn, &b_MM_EventWeight_Real_Mu_Pt_Sys_Grouped_Dn);
  fChain->SetBranchAddress("MM_EventWeight_Fake_El_Pt_Stat_Grouped_Up", &MM_EventWeight_Fake_El_Pt_Stat_Grouped_Up, &b_MM_EventWeight_Fake_El_Pt_Stat_Grouped_Up);
  fChain->SetBranchAddress("MM_EventWeight_Fake_El_Pt_Stat_Grouped_Dn", &MM_EventWeight_Fake_El_Pt_Stat_Grouped_Dn, &b_MM_EventWeight_Fake_El_Pt_Stat_Grouped_Dn);
  fChain->SetBranchAddress("MM_EventWeight_Fake_El_Pt_Sys_Grouped_Up", &MM_EventWeight_Fake_El_Pt_Sys_Grouped_Up, &b_MM_EventWeight_Fake_El_Pt_Sys_Grouped_Up);
  fChain->SetBranchAddress("MM_EventWeight_Fake_El_Pt_Sys_Grouped_Dn", &MM_EventWeight_Fake_El_Pt_Sys_Grouped_Dn, &b_MM_EventWeight_Fake_El_Pt_Sys_Grouped_Dn);
  fChain->SetBranchAddress("MM_EventWeight_Fake_Mu_Pt_Stat_Grouped_Up", &MM_EventWeight_Fake_Mu_Pt_Stat_Grouped_Up, &b_MM_EventWeight_Fake_Mu_Pt_Stat_Grouped_Up);
  fChain->SetBranchAddress("MM_EventWeight_Fake_Mu_Pt_Stat_Grouped_Dn", &MM_EventWeight_Fake_Mu_Pt_Stat_Grouped_Dn, &b_MM_EventWeight_Fake_Mu_Pt_Stat_Grouped_Dn);
  fChain->SetBranchAddress("MM_EventWeight_Fake_Mu_Pt_Sys_Grouped_Up", &MM_EventWeight_Fake_Mu_Pt_Sys_Grouped_Up, &b_MM_EventWeight_Fake_Mu_Pt_Sys_Grouped_Up);
  fChain->SetBranchAddress("MM_EventWeight_Fake_Mu_Pt_Sys_Grouped_Dn", &MM_EventWeight_Fake_Mu_Pt_Sys_Grouped_Dn, &b_MM_EventWeight_Fake_Mu_Pt_Sys_Grouped_Dn);
  fChain->SetBranchAddress("QMisID_MM_EventWeight", &QMisID_MM_EventWeight, &b_QMisID_MM_EventWeight);
  fChain->SetBranchAddress("QMisID_MM_EventWeight_Up", &QMisID_MM_EventWeight_Up, &b_QMisID_MM_EventWeight_Up);
  fChain->SetBranchAddress("QMisID_MM_EventWeight_Dn", &QMisID_MM_EventWeight_Dn, &b_QMisID_MM_EventWeight_Dn);
  fChain->SetBranchAddress("nTruthJets", &nTruthJets, &b_nTruthJets);
  fChain->SetBranchAddress("nJets", &nJets, &b_nJets);
  fChain->SetBranchAddress("jet_pt", &jet_pt, &b_jet_pt);
  fChain->SetBranchAddress("jet_phi", &jet_phi, &b_jet_phi);
  fChain->SetBranchAddress("jet_eta", &jet_eta, &b_jet_eta);
  fChain->SetBranchAddress("jet_e", &jet_e, &b_jet_e);
  fChain->SetBranchAddress("jet_passOR", &jet_passOR, &b_jet_passOR);
  fChain->SetBranchAddress("jet_passJVT", &jet_passJVT, &b_jet_passJVT);
  fChain->SetBranchAddress("jet_passTauOR", &jet_passTauOR, &b_jet_passTauOR);
  fChain->SetBranchAddress("jet_flavor_weight_MV2c10", &jet_flavor_weight_MV2c10, &b_jet_flavor_weight_MV2c10);
  fChain->SetBranchAddress("jet_flavor_truth_label", &jet_flavor_truth_label, &b_jet_flavor_truth_label);
  fChain->SetBranchAddress("jet_flavor_truth_label_ghost", &jet_flavor_truth_label_ghost, &b_jet_flavor_truth_label_ghost);
  fChain->SetBranchAddress("truth_jet_pt", &truth_jet_pt, &b_truth_jet_pt);
  fChain->SetBranchAddress("truth_jet_phi", &truth_jet_phi, &b_truth_jet_phi);
  fChain->SetBranchAddress("truth_jet_eta", &truth_jet_eta, &b_truth_jet_eta);
  fChain->SetBranchAddress("truth_jet_m", &truth_jet_m, &b_truth_jet_m);
  fChain->SetBranchAddress("truth_jet_Wcount", &truth_jet_Wcount, &b_truth_jet_Wcount);
  fChain->SetBranchAddress("truth_jet_Zcount", &truth_jet_Zcount, &b_truth_jet_Zcount);
  fChain->SetBranchAddress("truth_jet_Tcount", &truth_jet_Tcount, &b_truth_jet_Tcount);
  fChain->SetBranchAddress("truth_jet_Hcount", &truth_jet_Hcount, &b_truth_jet_Hcount);
  fChain->SetBranchAddress("truth_jet_BHadronCount", &truth_jet_BHadronCount, &b_truth_jet_BHadronCount);
  fChain->SetBranchAddress("truth_jet_CHadronCount", &truth_jet_CHadronCount, &b_truth_jet_CHadronCount);

  // ********************************************************************************************
  // Determine if the sample is Data or not:
  inputFileName = inpFile.substr(inpFile.find_last_of("/")+1, inpFile.find_last_of(".") - (inpFile.find_last_of("/")+1));
  //isData = (inputFileName.find("data") != std::string::npos); // Note that QmisID must be classified as data.
  isData = ( (prefix == "qmisidDD") || (prefix == "data") || (inputFileName.find("data") != std::string::npos) ); // Note that QmisID must be classified as data.
    
  doQMisID = (prefix == "qmisidDD");
  doTFfakes_convInt = (fakeTypes == "_convInt");
  doTFfakes_convExt = (fakeTypes == "_convExt");
  doTFfakes_hf_e = (fakeTypes == "_hf_e");
  doTFfakes_hf_mu = (fakeTypes == "_hf_mu");
  doTFfakes_lf_e = (fakeTypes == "_lf_e");
  doTFfakes_lf_mu = (fakeTypes == "_lf_mu");
    
  if (debug) printf("Is Data? %u (1 = Yes / 0 = No)\n", isData);
    
  name = prefix + "_" + inputFileName + ".root";

  // ********************************************************************************************
  TFile *newfile = new TFile("skim.root","recreate");
  TTree *newtree = fChain->CloneTree(0);

  // get the number of entries in the tree...
  if ( nEvents == 0 ) nentries = fChain->GetEntries();
  else nentries = nEvents;

  if (debug) printf("Number of entries in tree = %llu\n", nentries);

  // Check if the output path exists...
  if (outputPath == "") {
    printf("No output path has been given. Exiting... \n");
    return;
  } else if (stat(outputPath.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode)) {
    outFile = new TFile((outputPath + "/" + name).c_str(),"NEW");
  } else {
    printf("The given output path '%s' doesn't exists. Creating it... \n", outputPath.c_str());
    int dir_err = mkdir(outputPath.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    if (-1 == dir_err) {
      printf("Error creating the output directory '%s' \n", outputPath.c_str());
      return;
    } else {
      printf("The output directory: '%s' was successfuly created! \n", outputPath.c_str());
      outFile = new TFile((outputPath + "/" + name).c_str(),"NEW");
    }
  }

  // Event loop...
  for ( ULong64_t i=0; i < nentries; i++ ) {      // This is the event loop.

    // Reset event-related variables here...
    master_selection = false;
    TemplateFit_CR = false;
    common_selection = false;
    passAtLeastOneLep = false;
    passEventClean = false;
    passTrig = false;
    passDilepton = false;
    pass2LSS = false;
    passTrigMatch = false;
    passTightLep = false;
    passNJets = false;
    passNbtagJets = false;
    passTauCut = false;
    passLessThan3Bjets = false;
    passLepPtCut = false;
    is2lSS_2Tau_NonPrompt = false;
    passTightLep_NonPrompt = false;
    passPromptLep = false;
    passZCR = false;
    passDRllCut = false;
    passEtaCut = false;
    passMllCut = false;
    passQMisIDLep = false;
    passTtbarConv = false;
    passConvInt = false;
    lep_isFake_0 = false;
    lep_isFake_1 = false;
    isTight_0 = false;
    isTight_1 = false;
    isNotIntConvElect_0 = false;
    isNotIntConvElect_1 = false;
    isNotExtConvElect_0 = false;
    isNotExtConvElect_1 = false;
    passHasMEphoton_tty = false;
    passElecDefinition = false;
    lep_isHFfake_0 = false;
    lep_isHFfake_1 = false;

    // Event categorization....
    is2mu1b5j = false;
    isOF1b5j = false;
    is2e1b5j = false;
    is2mu2b5j = false;
    isOF2b5j = false;
    is2e2b5j = false;
    is2mu1b4j = false;
    isOF1b4j = false;
    is2e1b4j = false;
    is2mu2b4j = false;
    isOF2b4j = false;
    is2e2b4j = false;

    percent = (i+1)*100/nentries;
    event_weight = 1.;
    lepPairFlav = -999;
    maxEta = -999;
    m0, m1, px0, px1, py0, py1, pz0, pz1, e0, e1, mll = 0.;
    RadUp_weight = 1.;
    ttW_weight = 1.;
    lumi_weight = 1.;
    fakes_HF_weight = 1.;
    fakes_Conv_weight = 1.;

    if ((i+1)%10000 == 0) {
      if ( (nEvents > 0) || debug ) printf("Processing event %llu of %llu (%.2f %%)... \n", i+1, nentries, percent);
    }

    fChain->GetEntry(i);

    // Event selection for the 2lSS+0tau...
    passAtLeastOneLep = (onelep_type || dilep_type || trilep_type || quadlep_type);

    passEventClean = passEventCleaning;

    passTrig =  (((RunYear==2015&&(HLT_2e12_lhloose_L12EM10VH||HLT_e17_lhloose_mu14||HLT_mu18_mu8noL1)) ||
		  (RunYear==2016&&(HLT_2e17_lhvloose_nod0||HLT_e17_lhloose_nod0_mu14||HLT_mu22_mu8noL1)) ||
		  (RunYear==2017&&(HLT_2e24_lhvloose_nod0||HLT_e17_lhloose_nod0_mu14||HLT_mu22_mu8noL1)))); // Here 'HLT_2e17_lhvloose_nod0' changed to 'HLT_2e24_lhvloose_nod0' for the DLT strategy!!!!

    passDilepton = dilep_type; // dilep_type = 1(mumu) - dilep_type = 3(ee) - dilep_type = 2(OF)

    pass2LSS = (lep_ID_0*lep_ID_1 > 0);

    //passTrigMatch = (/*lep_isTrigMatch_0 || lep_isTrigMatch_1 ||*/ matchDLTll01);
    passTrigMatch = is1L2Tau?(lep_isTrigMatch_0||lep_isTrigMatch_1||lep_isTrigMatch_2||lep_isTrigMatch_3):(matchDLTll01||matchDLTll02||matchDLTll12||matchDLTll03||matchDLTll13||matchDLTll23);

    // IF THIS FLAG IS REMOVED, YOU MUST CHANGE THE 'lepSFObjTight' FACTOR FOR 'lepSFObjLoose' IN THE EVENT WEIGHT DEFINITION
    passTightLep =  (((abs(lep_ID_0)==13 && lep_isMedium_0 && lep_isolationFixedCutLoose_0 && lep_promptLeptonVeto_TagWeight_0<-0.5) ||
		      (abs(lep_ID_0)==11 && lep_isolationFixedCutLoose_0 && lep_isTightLH_0 && lep_chargeIDBDTTight_0>0.7 && lep_ambiguityType_0 == 0 && lep_promptLeptonVeto_TagWeight_0<-0.7))&&
		     ((abs(lep_ID_1)==13 && lep_isMedium_1 && lep_isolationFixedCutLoose_1 && lep_promptLeptonVeto_TagWeight_1<-0.5) ||
		      (abs(lep_ID_1)==11 && lep_isolationFixedCutLoose_1 && lep_isTightLH_1 && lep_chargeIDBDTTight_1>0.7 && lep_ambiguityType_1 == 0 && lep_promptLeptonVeto_TagWeight_1<-0.7)));

    passElecDefinition =(((abs(lep_ID_0)==13) ||
			  (abs(lep_ID_0)==11 && (!((lep_Mtrktrk_atPV_CO_0<0.1 && lep_Mtrktrk_atPV_CO_0>0) && !(lep_RadiusCO_0>20 && (lep_Mtrktrk_atConvV_CO_0<0.1 && lep_Mtrktrk_atConvV_CO_0>0))) && !(lep_RadiusCO_0>20 && (lep_Mtrktrk_atConvV_CO_0<0.1 && lep_Mtrktrk_atConvV_CO_0>0))))) &&
			 ((abs(lep_ID_1)==13) ||
			  (abs(lep_ID_1)==11 && (!((lep_Mtrktrk_atPV_CO_1<0.1 && lep_Mtrktrk_atPV_CO_1>0) && !(lep_RadiusCO_1>20 && (lep_Mtrktrk_atConvV_CO_1<0.1 && lep_Mtrktrk_atConvV_CO_1>0))) && !(lep_RadiusCO_1>20 && (lep_Mtrktrk_atConvV_CO_1<0.1 && lep_Mtrktrk_atConvV_CO_1>0))))));

    passNJets = (nJets_OR_T >= 4);

    passNbtagJets = (nJets_OR_T_MV2c10_70 >= 1);

    passTauCut = (nTaus_OR_Pt25 == 0);

    passLessThan3Bjets = nJets_OR_T_MV2c10_70 < 3;

    passLepPtCut = (lep_Pt_0 > 20000. && lep_Pt_1 > 20000.);

    passDRllCut = (((dilep_type==3 || dilep_type==2) && DRll01>0.5) || (dilep_type==1));

    passEtaCut =    (((abs(lep_ID_0)==13 && lep_isMedium_0) || (abs(lep_ID_0)==11 && abs(lep_Eta_0)<2.0)) &&
		     ((abs(lep_ID_1)==13 && lep_isMedium_1) || (abs(lep_ID_1)==11 && abs(lep_Eta_1)<2.0)));

    passMllCut = (Mll01 > 12e3);

    // End of event selection....

    // Other relevant selections:
    passZCR = ( (abs((Mll01/1000.) - Z_mass) <= 10.) && ( (abs(lep_ID_0) == 11 && abs(lep_ID_1) == 11) || (abs(lep_ID_0) == 13 && abs(lep_ID_1) == 13) ) && (lep_ID_0*lep_ID_1 < 0) );
    // Selection for the Z - Control Region = passAtLeastOneLep && passEventClean && passTrig && passTrigMatch && passTightLep && passZCR

    isNotIntConvElect_0 =	(abs(lep_ID_0)==13 ||
				 (abs(lep_ID_0)==11 && (!((lep_Mtrktrk_atPV_CO_0<0.1 && lep_Mtrktrk_atPV_CO_0>0) && !(lep_RadiusCO_0>20 && (lep_Mtrktrk_atConvV_CO_0<0.1 && lep_Mtrktrk_atConvV_CO_0>0))))));

    isNotIntConvElect_1 =	(abs(lep_ID_1)==13 ||
				 (abs(lep_ID_1)==11 && (!((lep_Mtrktrk_atPV_CO_1<0.1 && lep_Mtrktrk_atPV_CO_1>0) && !(lep_RadiusCO_1>20 && (lep_Mtrktrk_atConvV_CO_1<0.1 && lep_Mtrktrk_atConvV_CO_1>0))))));

    isNotExtConvElect_0 =	(abs(lep_ID_0)==13 ||
				 (abs(lep_ID_0)==11 && (!(lep_RadiusCO_0>20 && (lep_Mtrktrk_atConvV_CO_0<0.1 && lep_Mtrktrk_atConvV_CO_0>0)))));

    isNotExtConvElect_1 =	(abs(lep_ID_1)==13 ||
				 (abs(lep_ID_1)==11 && (!(lep_RadiusCO_1>20 && (lep_Mtrktrk_atConvV_CO_1<0.1 && lep_Mtrktrk_atConvV_CO_1>0)))));

    // Individual lepton tight cuts:
    isTight_0 = ((abs(lep_ID_0)==13 && lep_isMedium_0 && lep_isolationFixedCutLoose_0 && lep_promptLeptonVeto_TagWeight_0<-0.5) ||
		 (abs(lep_ID_0)==11 && lep_isolationFixedCutLoose_0 && lep_isTightLH_0 && lep_chargeIDBDTTight_0>0.7 && lep_ambiguityType_0 == 0 && lep_promptLeptonVeto_TagWeight_0<-0.7));

    isTight_1 = ((abs(lep_ID_1)==13 && lep_isMedium_1 && lep_isolationFixedCutLoose_1 && lep_promptLeptonVeto_TagWeight_1<-0.5) ||
		 (abs(lep_ID_1)==11 && lep_isolationFixedCutLoose_1 && lep_isTightLH_1 && lep_chargeIDBDTTight_1>0.7 && lep_ambiguityType_1 == 0 && lep_promptLeptonVeto_TagWeight_1<-0.7));

    // Use this???
    passPromptLep =     ((lep_isPrompt_0||(abs(lep_ID_0)==13&&lep_truthOrigin_0==0)||(abs(lep_ID_0)==11&&lep_truthOrigin_0==5&&lep_truthParentPdgId_0==lep_ID_0&&lep_truthParentType_0==2))&&
			 (lep_isPrompt_1||(abs(lep_ID_1)==13&&lep_truthOrigin_1==0)||(abs(lep_ID_1)==11&&lep_truthOrigin_1==5&&lep_truthParentPdgId_1==lep_ID_1&&lep_truthParentType_1==2)));

    passQMisIDLep =     ((!lep_isQMisID_1)&&(!lep_isQMisID_0));

    passTtbarConv =     ((lep_truthOrigin_0==5 && !(lep_truthParentPdgId_0==lep_ID_0&&lep_truthParentType_0==2)) ||
			 (lep_truthOrigin_1==5 && !(lep_truthParentPdgId_1==lep_ID_1&&lep_truthParentType_1==2)));

    passConvInt =	( (lep_truthParentType_0==21 && lep_truthParentOrigin_0==0) || (lep_truthParentType_1==21 && lep_truthParentOrigin_1==0) );

    passHasMEphoton_tty = 	( ( ( mc_channel_number == 410470 || mc_channel_number == 410472 ) && !m_hasMEphoton_DRgt02_nonhad ) ||
				  ( mc_channel_number == 410389 && m_hasMEphoton_DRgt02_nonhad ) ||
				  ( mc_channel_number != 410470 && mc_channel_number != 410472 && mc_channel_number != 410389 ) );

    lep_isFake_0 = !(lep_isPrompt_0 || ((abs(lep_ID_0)==13 && lep_truthOrigin_0==0) || (abs(lep_ID_0)==11 && lep_truthOrigin_0==5 && lep_truthParentPdgId_0==lep_ID_0 && lep_truthParentType_0==2)));

    lep_isFake_1 = !(lep_isPrompt_1 || ((abs(lep_ID_1)==13 && lep_truthOrigin_1==0) || (abs(lep_ID_1)==11 && lep_truthOrigin_1==5 && lep_truthParentPdgId_1==lep_ID_1 && lep_truthParentType_1==2)));

    lep_isHFfake_0 = ( ( lep_truthOrigin_0>=25 && lep_truthOrigin_0<=29 ) || lep_truthOrigin_0==32 || lep_truthOrigin_0==33 );

    lep_isHFfake_1 = ( ( lep_truthOrigin_1>=25 && lep_truthOrigin_1<=29 ) || lep_truthOrigin_1==32 || lep_truthOrigin_1==33 );

    // other weights:
    RadUp_weight = ( ( 1 + ( ( (-0.1)*(lep_Pt_0>20e3 && lep_Pt_0<30e3) + (-0.05)*(lep_Pt_0>30e3 && lep_Pt_0<40e3) + 0.1*(lep_Pt_0>40e3 && lep_Pt_0<80e3) + 0.15*(lep_Pt_0>80e3) ) * (!lep_isPrompt_0 && !(abs(lep_ID_0)==13 && lep_truthOrigin_0==0) && !(abs(lep_ID_0)==11 && lep_truthOrigin_0==5 && lep_truthParentPdgId_0==lep_ID_0 && lep_truthParentType_0==2))) ) *
		     ( 1 + ( ( (-0.1)*(lep_Pt_1>20e3 && lep_Pt_1<30e3) + (-0.05)*(lep_Pt_1>30e3 && lep_Pt_1<40e3) + 0.1*(lep_Pt_1>40e3 && lep_Pt_1<80e3) + 0.15*(lep_Pt_1>80e3) ) * (!lep_isPrompt_1 && !(abs(lep_ID_1)==13 && lep_truthOrigin_1==0) && !(abs(lep_ID_1)==11 && lep_truthOrigin_1==5 && lep_truthParentPdgId_1==lep_ID_1 && lep_truthParentType_1==2))) ));

    ttW_weight = ( Norm_ttW * ( (mc_channel_number == 410144 || mc_channel_number == 410155 || mc_channel_number == 413008) && scaleTtW ) + 1 * ( !(mc_channel_number == 410144 || mc_channel_number == 410155 || mc_channel_number == 413008) || !scaleTtW ) );

    lumi_weight = ( 36074.6*( RunYear==2015 || RunYear==2016 ) + 43813.7*( RunYear==2017 ) );

    fakes_HF_weight = ( (1.21114)*(HT > 150000. && HT < 250000.) + (1.01753)*(HT > 250000. && HT < 400000.) + (0.931628)*(HT > 400000. && HT < 1000000.) ); //HF ee+mue
    //fakes_HF_weight = ( (1.31653)*(HT > 150000. && HT < 250000.) + (0.921031)*(HT > 250000. && HT < 400000.) + (0.772919)*(HT > 400000. && HT < 1000000.) ); //HF emu+mumu
    //fakes_Conv_weight = ( (0.332325)*(lep_Pt_0 > 0. && lep_Pt_0 < 50000.) + (0.541724)*(lep_Pt_0 > 50000. && lep_Pt_0 < 100000.) + (0.884869)*(lep_Pt_0 > 100000. && lep_Pt_0 < 1000000.) ); //HF ee+mue
    //fakes_Conv_weight = ( (0.803525)*(HT > 0. && HT < 400000.) + (0.692888)*(HT > 400000. && HT < 1000000.) ); //HF ee+mue

    // Calculate event weight...
    if (!isData) event_weight *= (  pileupEventWeight_090 *
				    scale_nom *
				    JVT_EventWeight *
				    MV2c10_70_EventWeight *
				    SherpaNJetWeight *
				    ( (isTight_0 && isTight_1)*lepSFObjTight + (!isTight_0 && !isTight_1)*lepSFObjLoose + (!isTight_0 && isTight_1)*(lep_SFObjLoose_0*lep_SFObjTight_1) + (isTight_0 && !isTight_1)*(lep_SFObjTight_0*lep_SFObjLoose_1) ) *
				    lepSFTrigTight *
				    ttW_weight *
				    lumi_weight
                                    );

    // Common selection...
    common_selection = (
			passAtLeastOneLep
			//&& passEventClean
			//&& passTrig
			&& passDilepton
			&& passTightLep
			//&& ( ( !isTight_0 && isTight_1 ) || ( isTight_0 && !isTight_1 ) )	// Testing semi-loose selection (at least one lepton must be loose)
			&& passDRllCut
			&& passEtaCut
			&& passMllCut
			&& passTrigMatch
			&& passNJets
			&& passNbtagJets
			&& passTauCut
			&& passLepPtCut
			&& passElecDefinition    // For HF syst uncomment this line and comment for CO syst...
			//&& ( isNotIntConvElect_0 && isNotIntConvElect_1 ) // ExtCO
			//&& ( isNotExtConvElect_0 && isNotExtConvElect_1 ) // IntCO
			//&& ( ((abs(lep_ID_0) == 11 && lep_chargeIDBDTTight_0 > 0.7) || abs(lep_ID_0) == 13) && ((abs(lep_ID_1) == 11 && lep_chargeIDBDTTight_1 > 0.7) || abs(lep_ID_1) == 13) ) // Keep the QMisID BDT cut for loose selection
			);

    // Master selection is the common selection + additional process-specific selections...
    // This here now is the standard pre-MVA 2lSS0tau selection
    master_selection = (
			common_selection
			&& pass2LSS
			&& ( (isData && (tau_MV2c10_0 < 0.82 && total_charge)) || !isData )
			&& ( isData || ( (prefix.find("qmisidMC") != std::string::npos) && !passQMisIDLep) || ( (prefix.find("qmisidMC") == std::string::npos) && passQMisIDLep) )
			//// TIGHT CUT PLAY GROUND BELOW...
			//&& ( (abs(lep_ID_0) == 11 && lep_ambiguityType_0 > 0) || (abs(lep_ID_1) == 11 && lep_ambiguityType_1 > 0) ) // CO
			);

    if ( doQMisID ) {
      master_selection = (
			  common_selection
			  //// TIGHT CUT PLAY GROUND BELOW...
			  //&& ( (abs(lep_ID_0) == 11 && lep_ambiguityType_0 > 0) || (abs(lep_ID_1) == 11 && lep_ambiguityType_1 > 0) ) // CO
			  );
      event_weight *= (is2lOS0TauBaseline_ee || is2lOS0TauBaseline_OF) * QMisID_MM_EventWeight;
    }

    if ( doTFfakes_convInt ) {
      master_selection = (
			  common_selection
			  && pass2LSS
			  && ( mc_channel_number != 410472 ) // in 2lss fakes we don't use the ttbar 410472
			  //&& passHasMEphoton_tty
			  && total_charge
			  && (lep_isFake_0 || lep_isFake_1)
			  && passTtbarConv
			  && passConvInt
			  && passQMisIDLep
			  //// TIGHT CUT PLAY GROUND BELOW...
			  //&& ( (abs(lep_ID_0) == 11 && lep_ambiguityType_0 > 0) || (abs(lep_ID_1) == 11 && lep_ambiguityType_1 > 0) ) // CO
			  );
      event_weight *= ( ( Norm_ttbar_ConvInt * scaleFakes ) + ( 1.0 * !scaleFakes ) );
    }

    if ( doTFfakes_convExt ) {
      master_selection = (
			  common_selection
			  && pass2LSS
			  && ( mc_channel_number != 410472 ) // in 2lss fakes we don't use the ttbar 410472
			  //&& passHasMEphoton_tty
			  && total_charge
			  && (lep_isFake_0 || lep_isFake_1)
			  && passTtbarConv
			  && !passConvInt
			  && passQMisIDLep
			  //// TIGHT CUT PLAY GROUND BELOW...
			  //&& ( (abs(lep_ID_0) == 11 && lep_ambiguityType_0 > 0) || (abs(lep_ID_1) == 11 && lep_ambiguityType_1 > 0) ) // CO
			  );
      event_weight *= ( ( Norm_ttbar_ConvExt * scaleFakes ) + ( 1.0 * !scaleFakes ) );
    }

    if ( doTFfakes_hf_e ) {
      master_selection = (
			  common_selection
			  && pass2LSS
			  && ( mc_channel_number != 410472 ) // in 2lss fakes we don't use the ttbar 410472
			  //&& passHasMEphoton_tty
			  && total_charge
			  && ( ( abs(lep_ID_0)==11 && lep_isFake_0 && lep_isHFfake_0 ) || ( abs(lep_ID_1)==11 && lep_isFake_1 && lep_isHFfake_1 ) )
			  && !passTtbarConv
			  && (nJets_OR_T_MV2c10_70>=1)
			  && passQMisIDLep
			  //// TIGHT CUT PLAY GROUND BELOW...
			  //&& ( (abs(lep_ID_0) == 11 && lep_ambiguityType_0 > 0) || (abs(lep_ID_1) == 11 && lep_ambiguityType_1 > 0) ) // CO
			  );
      event_weight *= ( ( Norm_ttbar_HF_e * scaleFakes ) + ( 1.0 * !scaleFakes ) );
    }

    if ( doTFfakes_hf_mu ) {
      master_selection = (
			  common_selection
			  && pass2LSS
			  && ( mc_channel_number != 410472 ) // in 2lss fakes we don't use the ttbar 410472
			  //&& passHasMEphoton_tty
			  && total_charge
			  && ( ( abs(lep_ID_0)==13 && lep_isFake_0 && lep_isHFfake_0 ) || ( abs(lep_ID_1)==13 && lep_isFake_1 && lep_isHFfake_1 ) )
			  && !passTtbarConv
			  && (nJets_OR_T_MV2c10_70>=1)
			  && passQMisIDLep
			  //// TIGHT CUT PLAY GROUND BELOW...
			  //&& ( (abs(lep_ID_0) == 11 && lep_ambiguityType_0 > 0) || (abs(lep_ID_1) == 11 && lep_ambiguityType_1 > 0) ) // CO
			  );
      event_weight *= ( ( Norm_ttbar_HF_mu * scaleFakes ) + ( 1.0 * !scaleFakes ) );
    }

    if ( doTFfakes_lf_e ) {
      master_selection = (
			  common_selection
			  && pass2LSS
			  && ( mc_channel_number != 410472 ) // in 2lss fakes we don't use the ttbar 410472
			  //&& passHasMEphoton_tty
			  && total_charge
			  && ( ( abs(lep_ID_0)==11 && lep_isFake_0 && !lep_isHFfake_0 ) || ( abs(lep_ID_1)==11 && lep_isFake_1 && !lep_isHFfake_1 ) )
			  && !passTtbarConv
			  && (nJets_OR_T_MV2c10_70>=1)
			  && passQMisIDLep
			  //// TIGHT CUT PLAY GROUND BELOW...
			  //&& ( (abs(lep_ID_0) == 11 && lep_ambiguityType_0 > 0) || (abs(lep_ID_1) == 11 && lep_ambiguityType_1 > 0) ) // CO
			  );
      event_weight *= ( ( Norm_ttbar_LF_e * scaleFakes ) + ( 1.0 * !scaleFakes ) );
    }

    if ( doTFfakes_lf_mu ) {
      master_selection = (
			  common_selection
			  && pass2LSS
			  && ( mc_channel_number != 410472 ) // in 2lss fakes we don't use the ttbar 410472
			  //&& passHasMEphoton_tty
			  && total_charge
			  && ( ( abs(lep_ID_0)==13 && lep_isFake_0 && !lep_isHFfake_0 ) || ( abs(lep_ID_1)==13 && lep_isFake_1 && !lep_isHFfake_1 ) )
			  && !passTtbarConv
			  && (nJets_OR_T_MV2c10_70>=1)
			  && passQMisIDLep
			  //// TIGHT CUT PLAY GROUND BELOW...
			  //&& ( (abs(lep_ID_0) == 11 && lep_ambiguityType_0 > 0) || (abs(lep_ID_1) == 11 && lep_ambiguityType_1 > 0) ) // CO
			  );
      event_weight *= ( ( Norm_ttbar_LF_mu * scaleFakes ) + ( 1.0 * !scaleFakes ) );
    }

    // dilep_type = 1(mumu) - dilep_type = 3(ee) - dilep_type = 2(OF)
    lepPairFlav = 1*(dilep_type==3) + 2*(dilep_type==1) + 3*(dilep_type==2 && abs(lep_ID_0)==11) + 4*(dilep_type==2 && abs(lep_ID_0)==13);


    // Fill the CutFlows First...
    h_2SS0tau_CutFlow->Fill(0., event_weight);
    h_2SS0tau_CutFlow_Raw->Fill(0.);
    if (passAtLeastOneLep) {
      h_2SS0tau_CutFlow->Fill(1., event_weight);
      h_2SS0tau_CutFlow_Raw->Fill(1.);
    }
    if (passAtLeastOneLep && passEventClean) {
      h_2SS0tau_CutFlow->Fill(2., event_weight);
      h_2SS0tau_CutFlow_Raw->Fill(2.);
    }
    if (passAtLeastOneLep && passEventClean && passTrig) {
      h_2SS0tau_CutFlow->Fill(3., event_weight);
      h_2SS0tau_CutFlow_Raw->Fill(3.);
    }
    if (passAtLeastOneLep && passEventClean && passTrig && passDilepton && pass2LSS ) {
      h_2SS0tau_CutFlow->Fill(4., event_weight);
      h_2SS0tau_CutFlow_Raw->Fill(4.);
    }
    if (passAtLeastOneLep && passEventClean && passTrig && passDilepton && pass2LSS && passTrigMatch) {
      h_2SS0tau_CutFlow->Fill(5., event_weight);
      h_2SS0tau_CutFlow_Raw->Fill(5.);
    }
    if (passAtLeastOneLep && passEventClean && passTrig && passDilepton && pass2LSS && passTrigMatch && passTightLep) {
      h_2SS0tau_CutFlow->Fill(6., event_weight);
      h_2SS0tau_CutFlow_Raw->Fill(6.);
    }
    if (passAtLeastOneLep && passEventClean && passTrig && passDilepton && pass2LSS && passTrigMatch && passTightLep && passNJets) {
      h_2SS0tau_CutFlow->Fill(7., event_weight);
      h_2SS0tau_CutFlow_Raw->Fill(7.);
    }
    if (passAtLeastOneLep && passEventClean && passTrig && passDilepton && pass2LSS && passTrigMatch && passTightLep && passNJets && passNbtagJets) {
      h_2SS0tau_CutFlow->Fill(8., event_weight);
      h_2SS0tau_CutFlow_Raw->Fill(8.);
    }
    if (passAtLeastOneLep && passEventClean && passTrig && passDilepton && pass2LSS && passTrigMatch && passTightLep && passNJets && passNbtagJets && passTauCut) {
      h_2SS0tau_CutFlow->Fill(9., event_weight);
      h_2SS0tau_CutFlow_Raw->Fill(9.);
    }

    if ( master_selection /*&& nJets_OR_T_MV2c10_70 >=2 && ( nJets_OR_T==2 || nJets_OR_T==3 )/*nJets_OR_T>3 && total_charge>0*/ ) {

      h_2SS0tau_CutFlow->Fill(10., event_weight);
      h_2SS0tau_CutFlow_Raw->Fill(10.);

      h_lep_Pt_0->Fill(lep_Pt_0/1000., event_weight);

      newtree->Fill();

    } // end of statndard selection
	
  } // End of event loop...

  newtree->AutoSave();
  delete newfile;

  // Save the histograms to the root file and close it...
  h_2SS0tau_CutFlow->Write("h_2SS0tau_CutFlow");
  h_2SS0tau_CutFlow_Raw->Write("h_2SS0tau_CutFlow_Raw");
  h_lep_Pt_0->Write("h_lep_Pt_0");
  f->Close();
  outFile->Close();
  delete outFile;
  delete f;
  for (auto hist : all1DHists) delete hist;
  for (auto hist2 : all2DHists) delete hist2;
  all1DHists.clear();
  all2DHists.clear();
}
