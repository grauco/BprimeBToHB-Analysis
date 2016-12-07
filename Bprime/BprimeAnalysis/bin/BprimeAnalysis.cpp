#include "TFile.h"
#include "TChain.h"
#include "TTree.h"
#include "TBranch.h"
#include "TH1.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TLorentzVector.h"
#include <vector>
#include <assert.h>
#include <TMVA/Reader.h>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <cassert>
#include <sstream>
#include <string>
#include "TFileCollection.h"
#include "THashList.h"
#include "TBenchmark.h"

#include "DataFormats/Math/interface/deltaPhi.h"
#include "PhysicsTools/Utilities/interface/LumiReWeighting.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "Bprime/BprimeAnalysis/interface/Weights.h"
#include "Bprime/BprimeAnalysis/interface/MT2Utility.h"
#include "Bprime/BprimeAnalysis/interface/mt2w_bisect.h"
#include "Bprime/BprimeAnalysis/interface/mt2bl_bisect.h"
#include "Bprime/BprimeAnalysis/interface/Mt2Com_bisect.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "Bprime/BprimeAnalysis/interface/kFactors.h"

using namespace std;

typedef vector<double> vdouble;
typedef vector<float> vfloat;
typedef vector<int> vint;
typedef vector<bool> vbool;
typedef vector<string> vstring;

enum weightedSysts { NOSYST=0, BTAGUP = 1,BTAGDOWN=2,MISTAGUP=3,MISTAGDOWN=4, PUUP=5, PUDOWN=6, LEPUP=7, LEPDOWN=8, MAXSYSTS=9};
enum theoSysts {SCALEUP=101,SCALEDOWN=102, NNPDF1=100, NNPDF2=102};
int wLimit =150;

struct systWeights{
  void initHistogramsSysts(TH1F** histo, TString name, TString, int, float, float);
  void createFilesSysts(TFile ** allFiles, TString basename, TString opt="RECREATE");
  void fillHistogramsSysts(TH1F** histo, float v, float W, double *wcats= NULL,bool verbose=false);
  void fillHistogramsSysts(TH1F** histo, float v, float W,  float *systWeights, int nFirstSysts=(int)MAXSYSTS, double *wcats=NULL, bool verbose=false);
  void writeHistogramsSysts(TH1F** histo, TFile ** allFiles );
  void writeSingleHistogramSysts(TH1F* histo,TFile ** allMyFiles);
  void setMax(int max);
  void setMaxNonPDF(int max);
  void setSystValue(string name, double value, bool mult=false);
  void setSystValue(int systPlace, double value, bool mult=false);
  void setPDFWeights(float * wpdfs, int numPDFs, float wzero=1.0,bool mult=true);
  void setQ2Weights(float q2up, float q2down, float wzero=1.0,bool mult=true);
  void setTWeight(float tweight, float wtotsample=1.0,bool mult=true);
  void setVHFWeight(int vhf,bool mult=true, double shiftval=0.65);
  void setPDFValue(int numPDF, double value);
  double getPDFValue(int numPDF);
  void setWeight(string name, double value, bool mult=false);
  void setWeight(int systPlace, double value, bool mult=false);
  void prepareDefault(bool addDefault, bool addPDF, bool addQ2, bool addTopPt, bool addVHF, bool addTTSplit, int numPDF=102);
  void addSyst(string name);
  void addSystNonPDF(string name);
  void setWCats(double *wcats);
  
  void addkFact(string name);
  void setkFact(string name,float kfact_nom, float kfact_up,float kfact_down,  bool mult=true);

  void copySysts(systWeights sys);
  void calcPDFHisto(TH1F** histos, TH1F* singleHisto, double scalefactor=1.0, int c = 0);
  void setOnlyNominal(bool useOnlyNominal=false);
  bool onlyNominal;
  bool addPDF, addQ2, addTopPt, addVHF, addTTSplit;
  int maxSysts, maxSystsNonPDF;
  int nPDF;
  int nCategories;
  float weightedSysts[150];
  double wCats[10];
  string weightedNames[150];
  string categoriesNames[10];
};

void callme(){
  std::cout<<" NaN value"<<std::endl;
}

int main(int argc, char **argv) {

  TBenchmark bench;
  
  std::cout<<"Let's start"<<endl;
 
  string sample(argv[1]) ;
  std::cout<<"sample: "<<sample<<endl;

  string path(argv[2]);
  std::cout<<"File list to open: "<<path<<endl;

  string channel(argv[3]);
  std::cout<<"channel: "<<channel<<endl;
  
  string cat(argv[4]);
  std::cout<<"category:" <<cat<<endl;
    
  string sys(argv[5]);
  std::cout<<"systematics: "<<sys<<endl;

  string sync(argv[6]);
  std::cout<<"synchro: "<<sync<<endl;

  string isData(argv[7]);
  std::cout<<"isData: "<<isData<<endl;

  std::string outdir(argv[8]);
  std::cout<<"Output directory: "<<outdir<<endl;

  //  std::string outdir(argv[8]);
  //std::cout<<"Output directory: "<<outdir<<endl;

  //  TString path_ = path + "/trees*.root";
  TString path_ = path ; 
  std::cout<<"File to open: "<<path_<<endl;
  
  std::cout << "Loading file collection from " << path << std::endl;
  TFileCollection fc(sample.c_str(),sample.c_str(),path.c_str());
  std::cout << "Files found : " << fc.GetNFiles() << std::endl;

  TH1F * initproduct(TH1F * h_A,TH1F* h_B, int rebinA = 1, int rebinB=1,double integral = -1.);
  TH1F * makeproduct(TH1F * h_A,TH1F* h_B, int rebinA = 1, int rebinB=1,double integral = -1.);
  TH1F * makeproduct(TH2F * h);

  TString syststr = "";
  string syststrname = "";
  // if (lepton != "" && lepton != "any")lepstr= "_"+ lepton;
  if (sys != "noSys"){syststr= "_"+ sys; syststrname= "_"+ sys;}
  
  string reportDir = outdir+"/txt";
  string reportName = reportDir+"/SelectedEvents_"+channel+"_"+sample+syststrname+".txt";
  
  TString weightedSystsNames (weightedSysts sy);
  void initHistogramsSysts(TH1F* histo[(int)MAXSYSTS],TString, TString, int, float, float , bool useOnlyNominal=false);
  void createFilesSysts(  TFile * allFiles[(int)MAXSYSTS], TString basename, bool useOnlyNominal=false, TString opt="RECREATE");
  void fillHistogramsSysts(TH1F* histo[(int)MAXSYSTS], float v, float W, float systWeight[(int)MAXSYSTS] , bool useOnlyNominal=false);
  void writeHistogramsSysts(TH1F* histo[(int)MAXSYSTS], TFile * allFiles[(int)MAXSYSTS] , bool useOnlyNominal=false);
  void writeSingleHistogramSysts(TH1F* histo,TFile * allMyFiles[(int)MAXSYSTS],bool useOnlyNominal=false);

  systWeights syst0B, syst1B, systZero;
  int maxSysts=0;
  bool addPDF=false,addQ2=false, addTopPt=false,addVHF=false,addWZNLO=false, addTTSplit=false;
  addPDF=true;
  addQ2=true;

  int nPDF=102;
  if(isData=="DATA"){addPDF=false, addQ2=false;}
  systZero.prepareDefault(true, addQ2, addPDF, addTopPt,addVHF,addTTSplit);
  
  maxSysts= systZero.maxSysts;

  ofstream fileout;
  fileout.open(reportName.c_str(),ios::in | ios::out | ios::trunc);
  fileout<<"RunNumber EvtNumber Lumi "<<std::endl;

  TFile * allMyFiles[maxSysts];
  TString outfile = outdir + "/res/"+sample + "_" +cat+"_"+channel+".root";
  //TFile fout(outfile, "recreate");
  
  std::cout<<"File to open: "<<path_<<endl;
  TString treePath = "DMTreesDumper/ttDM__noSyst";
  TString treePathNEvents = "DMTreesDumper/WeightHistory";
  if(sys=="jesUp") treePath = "DMTreesDumper/ttDM__jes__up";
  else if(sys=="jesDown") treePath = "DMTreesDumper/ttDM__jes__down";
  else if(sys=="jerUp") treePath = "DMTreesDumper/ttDM__jer__up";
  else if(sys=="jerDown")treePath = "DMTreesDumper/ttDM__jer__down";
   
  bench.Start("NEvents");
  TChain chainNEvents(treePathNEvents);
  chainNEvents.AddFileInfoList(fc.GetList());
  //Int_t nEvents = (Int_t)chainNEvents.GetEntries();
  Int_t nEvents = (Int_t)chainNEvents.GetEntries();
  bench.Stop("NEvents");
  bench.Print("NEvents");

  bench.Start("NEventsPrePres");

  TH1D totalWeightTop("w_top_total","Top pt reweighting: overall sample weight",2000,0,2.0);
  chainNEvents.Project("w_top_total","Event_T_Weight","Event_T_Weight!=1.00");
  double topWeight=totalWeightTop.GetMean();
  cout << "totaltopweight is "<< topWeight<<endl;
  if(topWeight==0)topWeight=1;
  
  TChain chain(treePath);
  chain.AddFileInfoList(fc.GetList());
  Int_t nEventsPrePres = (Int_t)chain.GetEntries();
  //nEventsPrePres = 10;
  std::cout<<"--> --> Number of Events: "<<nEvents<< " after preselection "<< nEventsPrePres << endl;
  bench.Stop("NEventsPrePres");
  bench.Print("NEventsPrePres");

  //Int_t nEvents = (Int_t)chain.GetEntries();
  //std::cout<<"Number of Events: "<<nEvents<< endl;
  //nEvents=10;
  // nEvents = std::min(nEvents, 10000000);
  int sizeMax=50;
  Int_t jetSize, fatjetSize;
  float passTrigHT_800v3(0.),  passSingleMu(0.), Ht(0.);
  float  k_fact(1.), ZPt(-1.), WPt(-1);
  
  float numTrueInt(0), nPV(0.), nGoodPV(0.);
  float w(1.), runNumber(0.), lumiSec(0.);
  double evtNumber(0.);
  float n_closure(0.), nA(0.), nB(0.), nC(0.), nD(0.), n_trig(0.), n_cat(0.), n_Bosonjets(0.), n_bjets(0.), n_ht(0.),n_fatjet(0.), n_jet(0.);
  float jete[sizeMax], jetpt[sizeMax], jetphi[sizeMax], jeteta[sizeMax], jetcsv[sizeMax], jetiscsvm[sizeMax];
  float /*fatjetdoublebtag[sizeMax],*/fatjete[sizeMax], fatjetpt[sizeMax], fatjeteta[sizeMax], fatjetphi[sizeMax],  fatjetprunedmass[sizeMax], fatjetnj[sizeMax], fatjetnCSVsubj[sizeMax], fatjetnCSVsubjtm[sizeMax],fatjettau2OVERtau1[sizeMax], fatjetistype1[sizeMax], fatjetistype2[sizeMax], fatjettau3OVERtau2[sizeMax];
  
  float bWeightZero = 1.,bWeightZeroBTagUp= 1., bWeightZeroBTagDown=1.0, bWeightZeroMisTagUp=1.0,bWeightZeroMisTagDown=1.0;
  //  float bWeight2 = 1.,bWeight2BTagUp= 1., bWeight2BTagDown=1.0, bWeight2MisTagUp=1.0,bWeight2MisTagDown=1.0;
  float bWeight1 = 1.,bWeight1BTagUp= 1., bWeight1BTagDown=1.0, bWeight1MisTagUp=1.0,bWeight1MisTagDown=1.0;
  float bWeight1L = 1.,bWeight1LBTagUp= 1., bWeight1LBTagDown=1.0, bWeight1LMisTagUp=1.0,bWeight1LMisTagDown=1.0;
  //float bWeight2 = 1.,bWeight2BTagUp= 1., bWeight2BTagDown=1.0, bWeight2MisTagUp=1.0,bWeight2MisTagDown=1.0;
  //  float bWeight2L = 1.,bWeight2LBTagUp= 1., bWeight2LBTagDown=1.0, bWeight2LMisTagUp=1.0,bWeight2LMisTagDown=1.0;
  float w_pdfs[nPDF];
  float w_zero,w_q2up,w_q2down, w_top,eventFlavour=0.,NMCLeptons=-1;
  int vhf=0;
  int nCSVMJets30 =0.;
  
  chain.SetBranchAddress("jetsAK4CHSTight_E", jete);
  chain.SetBranchAddress("jetsAK4CHSTight_Pt", jetpt);
  chain.SetBranchAddress("jetsAK4CHSTight_Phi", jetphi);
  chain.SetBranchAddress("jetsAK4CHSTight_Eta", jeteta);
  chain.SetBranchAddress("jetsAK4CHSTight_CSVv2", jetcsv);
  chain.SetBranchAddress("jetsAK4CHSTight_IsCSVM", jetiscsvm);
  chain.SetBranchAddress("jetsAK4CHSTight_size", &jetSize);
  
  chain.SetBranchAddress("jetsAK8CHS_E", fatjete);
  chain.SetBranchAddress("jetsAK8CHS_Pt", fatjetpt);
  chain.SetBranchAddress("jetsAK8CHS_Eta", fatjeteta);
  chain.SetBranchAddress("jetsAK8CHS_Phi", fatjetphi);
  chain.SetBranchAddress("jetsAK8CHS_prunedMass", fatjetprunedmass);
  chain.SetBranchAddress("jetsAK8CHS_nJ", fatjetnj);
  //chain.SetBranchAddress("jetsAK8CHS_DoubleBAK8", fatjetdoublebtag);
  chain.SetBranchAddress("jetsAK8CHS_nCSVsubj_tm", fatjetnCSVsubjtm);
  chain.SetBranchAddress("jetsAK8CHS_nCSVsubj", fatjetnCSVsubj);
  chain.SetBranchAddress("jetsAK8CHS_tau2OVERtau1",fatjettau2OVERtau1);
  chain.SetBranchAddress("jetsAK8CHS_size", &fatjetSize);
  chain.SetBranchAddress("jetsAK8CHS_isType2", fatjetistype2);
  chain.SetBranchAddress("jetsAK8CHS_isType1", fatjetistype1);
  chain.SetBranchAddress("jetsAK8CHS_tau3OVERtau2", fatjettau3OVERtau2);

  chain.SetBranchAddress("Event_passesHLT_PFHT600_v3", &passSingleMu);
  chain.SetBranchAddress("Event_passesHLT_PFHT800_v2", &passTrigHT_800v3);
  //chain.SetBranchAddress("Event_passesHLT_PFHT900_v1", &passTrigHT_800v3);

  chain.SetBranchAddress("Event_Ht", &Ht);
  
  chain.SetBranchAddress("Event_RunNumber", &runNumber);
  chain.SetBranchAddress("Event_LumiBlock", &lumiSec);
  chain.SetBranchAddress("Event_EventNumber", &evtNumber);
  chain.SetBranchAddress("Event_nTruePV",&numTrueInt);
  chain.SetBranchAddress("Event_nPV",&nPV);
  chain.SetBranchAddress("Event_nGoodPV", &nGoodPV);
  chain.SetBranchAddress("Event_nCSVMJetsCut30", &nCSVMJets30);
  chain.SetBranchAddress("Event_Z_Pt", &ZPt);
  chain.SetBranchAddress("Event_W_Pt", &WPt);
  
  const char* Wlabel = "WJets";
  const char* Zlabel = "ZJets";
  const char* DYlabel = "DY";

  bool iswzjets=false;
  double shiftval = 0.0;
  if( !strncmp(sample.c_str(), Wlabel , strlen(Wlabel))) {chain.SetBranchAddress("Event_W_Weight", &k_fact);iswzjets=true;shiftval=0.2;}
  if( !strncmp(sample.c_str(), Zlabel , strlen(Zlabel))) {chain.SetBranchAddress("Event_Z_Weight", &k_fact);iswzjets=true;shiftval=0.2;}
  if( !strncmp(sample.c_str(), DYlabel , strlen(DYlabel))) {chain.SetBranchAddress("Event_Z_Weight", &k_fact);iswzjets=true;shiftval=0.2;}

  std::cout<<"k-factor "<<k_fact<<std::endl;

  if(isData=="MC") chain.SetBranchAddress("Event_LHEWeight0", &w_zero);
  if(addQ2){
    chain.SetBranchAddress("Event_LHEWeight4", &w_q2up);
    chain.SetBranchAddress("Event_LHEWeight8", &w_q2down);
  }
  if(addPDF){
    for (int p = 1; p <= nPDF;++p){
      stringstream pdfss;
      pdfss<<(p+8);
      string pstr =(pdfss.str());
      //chain.SetBranchAddress(("Event_LHEWeight"+pstr).c_str(), &w_pdfs[p-1]);
      w_pdfs[p-1] =0;
    }
    chain.SetBranchAddress("Event_T_Weight",&w_top);
  }
  if(iswzjets && addVHF){
    chain.SetBranchAddress("Event_eventFlavour",&eventFlavour);
  }
  if(addTTSplit) chain.SetBranchAddress("Event_NMCLeptons",&NMCLeptons);
  
  /*chain.SetBranchAddress("Event_bWeight2CSVM", &bWeight2);

  chain.SetBranchAddress("Event_bWeightMisTagUp2CSVM", &bWeight2MisTagUp);
  chain.SetBranchAddress("Event_bWeightMisTagDown2CSVM", &bWeight2MisTagDown);
  chain.SetBranchAddress("Event_bWeightBTagUp2CSVM", &bWeight2BTagUp);
  chain.SetBranchAddress("Event_bWeightBTagDown2CSVM", &bWeight2BTagDown);
  */
  chain.SetBranchAddress("Event_bWeight0CSVM", &bWeightZero);

  chain.SetBranchAddress("Event_bWeightMisTagUp0CSVM", &bWeightZeroMisTagUp);
  chain.SetBranchAddress("Event_bWeightMisTagDown0CSVM", &bWeightZeroMisTagDown);
  chain.SetBranchAddress("Event_bWeightBTagUp0CSVM", &bWeightZeroBTagUp);
  chain.SetBranchAddress("Event_bWeightBTagDown0CSVM", &bWeightZeroBTagDown);

  chain.SetBranchAddress("Event_bWeight1CSVM", &bWeight1);

  chain.SetBranchAddress("Event_bWeightMisTagUp1CSVM", &bWeight1MisTagUp);
  chain.SetBranchAddress("Event_bWeightMisTagDown1CSVM", &bWeight1MisTagDown);
  chain.SetBranchAddress("Event_bWeightBTagUp1CSVM", &bWeight1BTagUp);
  chain.SetBranchAddress("Event_bWeightBTagDown1CSVM", &bWeight1BTagDown);

  chain.SetBranchAddress("Event_bWeight1CSVL", &bWeight1L);
  chain.SetBranchAddress("Event_bWeightMisTagUp1CSVL", &bWeight1LMisTagUp);
  chain.SetBranchAddress("Event_bWeightMisTagDown1CSVL", &bWeight1LMisTagDown);
  chain.SetBranchAddress("Event_bWeightBTagUp1CSVL", &bWeight1LBTagUp);
  chain.SetBranchAddress("Event_bWeightBTagDown1CSVL", &bWeight1LBTagDown);
  
  /********************************************************************************/
  /**************                    Histogram booking              ***************/
  /********************************************************************************/


  
  TH1F *h_cutFlow  = new TH1F("h_cutFlow","cutFlow",6,-0.5,5.5);
  TH1F *h_nPV   [maxSysts] ;systZero.initHistogramsSysts(h_nPV,"h_nPV","nPV",50,0,50);
  TH1F *h_nPV_w   [maxSysts] ;systZero.initHistogramsSysts(h_nPV_w,"h_nPV_w","nPV",50,0,50);

  //PRESELECTION PLOTS

  TH1F *h_AK8jetpt [maxSysts];systZero.initHistogramsSysts(h_AK8jetpt,"h_AK8pt","AK8pt",100,0,1000);
  TH1F *h_AK8jeteta [maxSysts];systZero.initHistogramsSysts(h_AK8jeteta,"h_AK8eta","AK8eta",100,-5,5);
  TH1F *h_AK8jetphi [maxSysts];systZero.initHistogramsSysts(h_AK8jetphi,"h_AK8phi","AK8phi",100,-7,7);
  TH1F *h_AK8jetmult [maxSysts];systZero.initHistogramsSysts(h_AK8jetmult,"h_AK8mult","AK8 mult",20,-0.5,19.5);
  TH1F *h_AK8jetmultaft [maxSysts];systZero.initHistogramsSysts(h_AK8jetmultaft,"h_AK8mult aft","AK8 mult aft",20,-0.5,19.5);

  TH1F *h_AK4jetpt [maxSysts];systZero.initHistogramsSysts(h_AK4jetpt,"h_AK4pt","AK4pt",100,0,1000);
  TH1F *h_AK4jeteta [maxSysts];systZero.initHistogramsSysts(h_AK4jeteta,"h_AK4eta","AK4eta",100,-5,5);
  TH1F *h_AK4jetphi [maxSysts];systZero.initHistogramsSysts(h_AK4jetphi,"h_AK4phi","AK4phi",100,-7,7);
  TH1F *h_AK4jetcsv [maxSysts];systZero.initHistogramsSysts(h_AK4jetcsv,"h_AK4csv","AK4csv",100,0,1);
  TH1F *h_AK4jetmult [maxSysts];systZero.initHistogramsSysts(h_AK4jetmult,"h_AK4mult","AK4 mult",20,-0.5,19.5);
  TH1F *h_AK4bjetmult [maxSysts];systZero.initHistogramsSysts(h_AK4bjetmult,"h_AK4bmult","AK4 b-tagged mult",20,-0.5,19.5);
  TH1F *h_AK4jetmultaft [maxSysts];systZero.initHistogramsSysts(h_AK4jetmultaft,"h_AK4mult aft","AK4 mult aft",20,-0.5,19.5);
  TH1F *h_AK4fwjets [maxSysts]; systZero.initHistogramsSysts(h_AK4fwjets, "h_AK4fwjets", "AK4 forward jets multiplicity", 10, -0.5, 9.5);
  
  TH1F *h_nsubj [maxSysts];systZero.initHistogramsSysts(h_nsubj,"h_nsubj","nsubj",3,-0.5,2.5);
  TH1F *h_tau2tau1 [maxSysts];systZero.initHistogramsSysts(h_tau2tau1,"h_tau2tau1","tau2tau1",100,0,1);
  TH1F *h_prunedmass [maxSysts];systZero.initHistogramsSysts(h_prunedmass,"h_prunedmass","prunedmass",100,0,500);

  TH1F *h_deltaRbH [maxSysts];systZero.initHistogramsSysts(h_deltaRbH ,"h_deltaRbH","deltaRbH",100,0,5);

  TH1F *h_Ht_trigbef   [maxSysts] ;systZero.initHistogramsSysts(h_Ht_trigbef,"h_Ht_trigbef","Ht",300,0,3000);
  TH1F *h_Ht_trigaft   [maxSysts] ;systZero.initHistogramsSysts(h_Ht_trigaft,"h_Ht_trigaft","Ht",300,0,3000);
  
  TH2F *h_bprimemassvsht = new TH2F("h_bprimemassvsht","h_bprimemassvsht", 200, 0, 2000, 300, 0, 3000);
  TH2F *h_nsubjvsmh = new TH2F("h_nsubjvsmh", "h_nsubjvsmh", 3, -0.5, 2.5, 100, 0, 300);
  //SIGNAL REGION PLOTS

  TH1F *h_bprimept_SR [maxSysts];systZero.initHistogramsSysts(h_bprimept_SR,"h_bprimept_SR","bprimept",100,0,1000);
  TH1F *h_bprimeeta_SR[maxSysts];systZero.initHistogramsSysts(h_bprimeeta_SR,"h_bprimeeta_SR","bprimeeta",100,-5,5);
  TH1F *h_bprimephi_SR [maxSysts];systZero.initHistogramsSysts(h_bprimephi_SR,"h_bprimephi_SR","bprimephi",100,-7,7);
  
  TH1F *h_nsubj_SR [maxSysts];systZero.initHistogramsSysts(h_nsubj_SR,"h_nsubj_SR","nsubj",3,-0.5,2.5);
  TH1F *h_tau2tau1_SR [maxSysts];systZero.initHistogramsSysts(h_tau2tau1_SR,"h_tau2tau1_SR","tau2tau1",100,0,1);
  TH1F *h_prunedmass_SR [maxSysts];systZero.initHistogramsSysts(h_prunedmass_SR,"h_prunedmass_SR","prunedmass",100,0,500);
  
  TH1F *h_AK8jet_selh_pt_SR [maxSysts];systZero.initHistogramsSysts(h_AK8jet_selh_pt_SR,"h_AK8_selh_pt_SR","AK8pt",100,0,1000);
  TH1F *h_AK8jet_selh_eta_SR [maxSysts];systZero.initHistogramsSysts(h_AK8jet_selh_eta_SR,"h_AK8_selh_eta_SR","AK8eta",100,-5,5);
  TH1F *h_AK8jet_selh_phi_SR [maxSysts];systZero.initHistogramsSysts(h_AK8jet_selh_phi_SR,"h_AK8_selh_phi_SR","AK8phi",100,-7,7);

  TH1F *h_AK4jet_selb_pt_SR [maxSysts];systZero.initHistogramsSysts(h_AK4jet_selb_pt_SR,"h_AK4_selb_pt_SR","AK4pt",100,0,1000);
  TH1F *h_AK4jet_selb_eta_SR [maxSysts];systZero.initHistogramsSysts(h_AK4jet_selb_eta_SR,"h_AK4_selb_eta_SR","AK4eta",100,-5,5);
  TH1F *h_AK4jet_selb_phi_SR [maxSysts];systZero.initHistogramsSysts(h_AK4jet_selb_phi_SR,"h_AK4_selb_phi_SR","AK4phi",100,-7,7);

  TH1F *h_bprimemass_SR   [maxSysts] ;systZero.initHistogramsSysts(h_bprimemass_SR,"h_bprimemass_SR","bprimemass",200,0,2000);
  TH1F *h_Ht   [maxSysts] ;systZero.initHistogramsSysts(h_Ht,"h_Ht","Ht",300,0,3000);
  
  //CONTROL REGION PLOTS
  
  TH1F *h_Ht_CRB   [maxSysts] ;systZero.initHistogramsSysts(h_Ht_CRB,"h_Ht_CRB","Ht",300,0,3000);
  TH1F *h_Ht_CRC   [maxSysts] ;systZero.initHistogramsSysts(h_Ht_CRC,"h_Ht_CRC","Ht",300,0,3000);
  TH1F *h_Ht_CRD   [maxSysts] ;systZero.initHistogramsSysts(h_Ht_CRD,"h_Ht_CRD","Ht",300,0,3000);
  
  TH1F *h_bprimemass_CRB   [maxSysts] ;systZero.initHistogramsSysts(h_bprimemass_CRB,"h_bprimemass_CRB","bprimemass",200,0,2000);
  TH1F *h_bprimemass_CRC   [maxSysts] ;systZero.initHistogramsSysts(h_bprimemass_CRC,"h_bprimemass_CRC","bprimemass",200,0,2000);
  TH1F *h_bprimemass_CRD   [maxSysts] ;systZero.initHistogramsSysts(h_bprimemass_CRD,"h_bprimemass_CRD","bprimemass",200,0,2000);

  TH1F *h_bprimept_CRB [maxSysts];systZero.initHistogramsSysts(h_bprimept_CRB,"h_bprimept_CRB","bprimept",100,0,1000);
  TH1F *h_bprimeeta_CRB[maxSysts];systZero.initHistogramsSysts(h_bprimeeta_CRB,"h_bprimeeta_CRB","bprimeeta",100,-5,5);
  TH1F *h_bprimephi_CRB [maxSysts];systZero.initHistogramsSysts(h_bprimephi_CRB,"h_bprimephi_CRB","bprimephi",100,-7,7);
  
  TH1F *h_nsubj_CRB [maxSysts];systZero.initHistogramsSysts(h_nsubj_CRB,"h_nsubj_CRB","nsubj",3,-0.5,2.5);
  TH1F *h_tau2tau1_CRB [maxSysts];systZero.initHistogramsSysts(h_tau2tau1_CRB,"h_tau2tau1_CRB","tau2tau1",100,0,1);
  TH1F *h_prunedmass_CRB [maxSysts];systZero.initHistogramsSysts(h_prunedmass_CRB,"h_prunedmass_CRB","prunedmass",100,0,500);
  
  TH1F *h_AK8jet_selh_pt_CRB [maxSysts];systZero.initHistogramsSysts(h_AK8jet_selh_pt_CRB,"h_AK8_selh_pt_CRB","AK8pt",100,0,1000);
  TH1F *h_AK8jet_selh_eta_CRB [maxSysts];systZero.initHistogramsSysts(h_AK8jet_selh_eta_CRB,"h_AK8_selh_eta_CRB","AK8eta",100,-5,5);
  TH1F *h_AK8jet_selh_phi_CRB [maxSysts];systZero.initHistogramsSysts(h_AK8jet_selh_phi_CRB,"h_AK8_selh_phi_CRB","AK8phi",100,-7,7);

  TH1F *h_AK4jet_selb_pt_CRB [maxSysts];systZero.initHistogramsSysts(h_AK4jet_selb_pt_CRB,"h_AK4_selb_pt_CRB","AK4pt",100,0,1000);
  TH1F *h_AK4jet_selb_eta_CRB [maxSysts];systZero.initHistogramsSysts(h_AK4jet_selb_eta_CRB,"h_AK4_selb_eta_CRB","AK4eta",100,-5,5);
  TH1F *h_AK4jet_selb_phi_CRB [maxSysts];systZero.initHistogramsSysts(h_AK4jet_selb_phi_CRB,"h_AK4_selb_phi_CRB","AK4phi",100,-7,7);

  TH1F *h_bprimept_CRC [maxSysts];systZero.initHistogramsSysts(h_bprimept_CRC,"h_bprimept_CRC","bprimept",100,0,1000);
  TH1F *h_bprimeeta_CRC[maxSysts];systZero.initHistogramsSysts(h_bprimeeta_CRC,"h_bprimeeta_CRC","bprimeeta",100,-5,5);
  TH1F *h_bprimephi_CRC [maxSysts];systZero.initHistogramsSysts(h_bprimephi_CRC,"h_bprimephi_CRC","bprimephi",100,-7,7);
  
  TH1F *h_nsubj_CRC [maxSysts];systZero.initHistogramsSysts(h_nsubj_CRC,"h_nsubj_CRC","nsubj",3,-0.5,2.5);
  TH1F *h_tau2tau1_CRC [maxSysts];systZero.initHistogramsSysts(h_tau2tau1_CRC,"h_tau2tau1_CRC","tau2tau1",100,0,1);
  TH1F *h_prunedmass_CRC [maxSysts];systZero.initHistogramsSysts(h_prunedmass_CRC,"h_prunedmass_CRC","prunedmass",100,0,500);
  
  TH1F *h_AK8jet_selh_pt_CRC [maxSysts];systZero.initHistogramsSysts(h_AK8jet_selh_pt_CRC,"h_AK8_selh_pt_CRC","AK8pt",100,0,1000);
  TH1F *h_AK8jet_selh_eta_CRC [maxSysts];systZero.initHistogramsSysts(h_AK8jet_selh_eta_CRC,"h_AK8_selh_eta_CRC","AK8eta",100,-5,5);
  TH1F *h_AK8jet_selh_phi_CRC [maxSysts];systZero.initHistogramsSysts(h_AK8jet_selh_phi_CRC,"h_AK8_selh_phi_CRC","AK8phi",100,-7,7);

  TH1F *h_AK4jet_selb_pt_CRC [maxSysts];systZero.initHistogramsSysts(h_AK4jet_selb_pt_CRC,"h_AK4_selb_pt_CRC","AK4pt",100,0,1000);
  TH1F *h_AK4jet_selb_eta_CRC [maxSysts];systZero.initHistogramsSysts(h_AK4jet_selb_eta_CRC,"h_AK4_selb_eta_CRC","AK4eta",100,-5,5);
  TH1F *h_AK4jet_selb_phi_CRC [maxSysts];systZero.initHistogramsSysts(h_AK4jet_selb_phi_CRC,"h_AK4_selb_phi_CRC","AK4phi",100,-7,7);

  TH1F *h_bprimept_CRD [maxSysts];systZero.initHistogramsSysts(h_bprimept_CRD,"h_bprimept_CRD","bprimept",100,0,1000);
  TH1F *h_bprimeeta_CRD[maxSysts];systZero.initHistogramsSysts(h_bprimeeta_CRD,"h_bprimeeta_CRD","bprimeeta",100,-5,5);
  TH1F *h_bprimephi_CRD [maxSysts];systZero.initHistogramsSysts(h_bprimephi_CRD,"h_bprimephi_CRD","bprimephi",100,-7,7);
  
  TH1F *h_nsubj_CRD [maxSysts];systZero.initHistogramsSysts(h_nsubj_CRD,"h_nsubj_CRD","nsubj",3,-0.5,2.5);
  TH1F *h_tau2tau1_CRD [maxSysts];systZero.initHistogramsSysts(h_tau2tau1_CRD,"h_tau2tau1_CRD","tau2tau1",100,0,1);
  TH1F *h_prunedmass_CRD [maxSysts];systZero.initHistogramsSysts(h_prunedmass_CRD,"h_prunedmass_CRD","prunedmass",100,0,500);
  
  TH1F *h_AK8jet_selh_pt_CRD [maxSysts];systZero.initHistogramsSysts(h_AK8jet_selh_pt_CRD,"h_AK8_selh_pt_CRD","AK8pt",100,0,1000);
  TH1F *h_AK8jet_selh_eta_CRD [maxSysts];systZero.initHistogramsSysts(h_AK8jet_selh_eta_CRD,"h_AK8_selh_eta_CRD","AK8eta",100,-5,5);
  TH1F *h_AK8jet_selh_phi_CRD [maxSysts];systZero.initHistogramsSysts(h_AK8jet_selh_phi_CRD,"h_AK8_selh_phi_CRD","AK8phi",100,-7,7);

  TH1F *h_AK4jet_selb_pt_CRD [maxSysts];systZero.initHistogramsSysts(h_AK4jet_selb_pt_CRD,"h_AK4_selb_pt_CRD","AK4pt",100,0,1000);
  TH1F *h_AK4jet_selb_eta_CRD [maxSysts];systZero.initHistogramsSysts(h_AK4jet_selb_eta_CRD,"h_AK4_selb_eta_CRD","AK4eta",100,-5,5);
  TH1F *h_AK4jet_selb_phi_CRD [maxSysts];systZero.initHistogramsSysts(h_AK4jet_selb_phi_CRD,"h_AK4_selb_phi_CRD","AK4phi",100,-7,7);
  
  float systWeights1B[maxSysts], systWeights0B[maxSysts];
  //float systWeightsNoSyst[maxSysts];

  //for pile-up reweighting
  float w_pu;
  float LHEWeightSign[1] = {1.};

  //TFile * allMyFiles[maxSysts];
  string outFileName =outdir + "/res/"+sample + "_" +cat+ "_" +channel+".root";//sample + "_" +cat+"_"+channel+".root";

  bool onlyNominal=false;
  systZero.setOnlyNominal(onlyNominal);
  syst1B.setOnlyNominal(onlyNominal);

  systZero.createFilesSysts(allMyFiles,outdir+"/res/"+sample + "_" + cat +"_" +channel+syststr);

  edm::LumiReWeighting LumiWeights_, LumiWeightsUp_, LumiWeightsDown_;
 
  if(isData=="MC"){
    LumiWeights_ = edm::LumiReWeighting("data/puMC.root", "data/MyDataPileupHistogram.root","MC_pu","pileup");
    LumiWeightsUp_ = edm::LumiReWeighting("data/puMC.root", "data/MyDataPileupHistogramUP.root","MC_pu","pileup");
    LumiWeightsDown_ = edm::LumiReWeighting("data/puMC.root", "data/MyDataPileupHistogramDOWN.root","MC_pu","pileup");
  }
  
  kFactor wQCD("WQCD");
  kFactor wEWK("WEWK");
  kFactor wQCDrenUp("WQCDrenUp");
  kFactor wQCDrenDown("WQCDrenDown");
  kFactor wQCDfacUp("WQCDfacUp");
  kFactor wQCDfacDown("WQCDfacDown");
  
  kFactor zQCD("ZQCD");
  kFactor zEWK("ZEWK");
  kFactor zQCDrenUp("ZQCDrenUp");
  kFactor zQCDrenDown("ZQCDrenDown");
  kFactor zQCDfacUp("ZQCDfacUp");
  kFactor zQCDfacDown("ZQCDfacDown");
  
  float kfact(1.);
  float kfact_qcd(1.),  kfact_ewk(1.);
  float kfact_qcdrenUp(1.), kfact_qcdrenDown(1.), kfact_qcdfacUp(1.), kfact_qcdfacDown(1.), kfact_ewkUp(1.) ,kfact_ewkDown(1.);

  for(Int_t i=0; i<nEventsPrePres; i++ )
    {
      if(i%100000==1 ){
	cout<<"Running on event: "<<i<<endl; 
      }

      w = LHEWeightSign[0];
      //cout << "w step 1: " << w << endl;
      
      chain.GetEntry(i);

       if( !strncmp(sample.c_str(), Wlabel , strlen(Wlabel))) {
        kfact_qcd = wQCD.getkFact(WPt);
        kfact_ewk = wEWK.getkFact(WPt);

	kfact_qcdrenUp = wQCDrenUp.getSyst(WPt); 
	kfact_qcdrenDown = wQCDrenDown.getSyst(WPt); 
	kfact_qcdfacUp = wQCDfacUp.getSyst(WPt); 
	kfact_qcdfacDown = wQCDfacDown.getSyst(WPt); 

	kfact_ewkUp=1.;
	kfact_ewkDown=kfact_ewk;

	kfact =  kfact_qcd *  kfact_ewk;
	k_fact = kfact;
     }

     else if( !strncmp(sample.c_str(), Zlabel , strlen(Zlabel))  or !strncmp(sample.c_str(), Zlabel , strlen(DYlabel)) ) {
        kfact_qcd = zQCD.getkFact(ZPt);
        kfact_ewk = zEWK.getkFact(ZPt);

	kfact_qcdrenUp = zQCDrenUp.getSyst(ZPt); 
	kfact_qcdrenDown = zQCDrenDown.getSyst(ZPt); 
	kfact_qcdfacUp = zQCDfacUp.getSyst(ZPt); 
	kfact_qcdfacDown = zQCDfacDown.getSyst(ZPt); 

	kfact_ewkUp=1.;
	kfact_ewkDown=kfact_ewk;
	
	kfact =  kfact_qcd *  kfact_ewk;
	k_fact = kfact;
     }
     else{

	kfact =  1.;
     }
      
      
      if(isData=="MC"){
	w = LHEWeightSign[0];
	w_pu = LumiWeights_.weight(numTrueInt);
	w = w * w_pu;
	
	if(sample=="TT"){
	  w*=w_top/topWeight;
	}
	 
	vhf=(int)eventFlavour;
      }
      //cout << "w_pu step 2: " << w_pu << endl;
      //cout << "w step 2: " << w<< endl;
      //cout << "isData? " << isData << endl;

      double  wcats[10]={1.,1.,1.,1.,1.,1.,1.,1.,1.,1.};
     
      if(isData=="DATA"){
       
	systZero.setWeight(0,1.);
	systZero.setWeight("btagUp",1.);
	systZero.setWeight("btagDown",1.);
	systZero.setWeight("mistagUp",1.);
	systZero.setWeight("mistagDown",1.);
	systZero.setWeight("puDown",1.);
	systZero.setWeight("puUp",1.);
	systZero.setWeight("lepDown",1.);
	systZero.setWeight("lepUp",1.);

	syst0B.copySysts(systZero);
        syst0B.setWeight("btagUp",1.);
        syst0B.setWeight("btagDown",1.);

        systWeights0B[NOSYST]=1.;
        systWeights0B[BTAGUP]=1.;
	systWeights0B[BTAGDOWN]=1.;
        systWeights0B[MISTAGUP]=1.;
        systWeights0B[MISTAGDOWN]=1.;
        systWeights0B[PUUP]=1.;
        systWeights0B[PUDOWN]=1.;
        systWeights0B[LEPUP]=1.;
        systWeights0B[LEPDOWN]=1.;

	syst1B.copySysts(systZero);
	syst1B.setWeight("btagUp",1.);
	syst1B.setWeight("btagDown",1.);
	
	systWeights1B[NOSYST]=1.;
	systWeights1B[BTAGUP]=1.;
	systWeights1B[BTAGDOWN]=1.;
	systWeights1B[MISTAGUP]=1.;
	systWeights1B[MISTAGDOWN]=1.;
	systWeights1B[PUUP]=1.;
	systWeights1B[PUDOWN]=1.;
	systWeights1B[LEPUP]=1.;
	systWeights1B[LEPDOWN]=1.;
	wcats[0]=1.0;   
	if(addTTSplit){
	   wcats[0]=1.0;
	   wcats[1]=double(NMCLeptons==0);
	   wcats[2]=double(NMCLeptons==1);
	   wcats[3]=double(NMCLeptons==2);
	   systZero.setWCats(wcats);
	}
      }

      if(isData=="MC"){
	
	double puUpFact=(LumiWeightsUp_.weight(numTrueInt))/(LumiWeights_.weight(numTrueInt));
	double puDownFact=(LumiWeightsDown_.weight(numTrueInt))/(LumiWeights_.weight(numTrueInt));
	
	if(numTrueInt>49){
	  cout << " --> numTrueInt very high!!" << endl;
	  puUpFact =0;
	  puDownFact=0;
	}
	
	systZero.setWeight(0,1.);
	systZero.setWeight("btagUp",1.);
	systZero.setWeight("btagDown",1.);
	systZero.setWeight("mistagUp",1.);
	systZero.setWeight("mistagDown",1.);
	systZero.setWeight("puUp",1.);
	systZero.setWeight("puDown",1.);
	systZero.setWeight("lepDown",1.);
	systZero.setWeight("lepUp",1.);
	wcats[0]=1.0;
	if(addTTSplit){
	   wcats[0]=1.0;
	   wcats[1]=double(NMCLeptons==0);
	   wcats[2]=double(NMCLeptons==1);
	   wcats[3]=double(NMCLeptons==2);
	   systZero.setWCats(wcats);
	}
	
	if(addPDF)systZero.setPDFWeights(w_pdfs,nPDF,w_zero,true);
	if(addQ2)systZero.setQ2Weights(w_q2up,w_q2down,w_zero,true);
	if(addTopPt)systZero.setTWeight(w_top,topWeight,true);
	if(addVHF)systZero.setVHFWeight(vhf,true,shiftval);
	if(addWZNLO){
	  systZero.setkFact("QCDRen",kfact_qcd,kfact_qcdrenUp,kfact_qcdrenDown);
	  systZero.setkFact("QCDFac",kfact_qcd,kfact_qcdfacUp,kfact_qcdfacDown);
	  systZero.setkFact("EWK",kfact_ewk,kfact_ewkUp,kfact_ewkDown);
	}
	
	syst1B.copySysts(systZero);
	syst1B.setWeight(0,bWeight1);
	syst1B.setWeight("btagUp",bWeight1BTagUp);
	syst1B.setWeight("btagDown",bWeight1BTagDown);
	syst1B.setWeight("mistagUp",bWeight1MisTagUp);
	syst1B.setWeight("mistagDown",bWeight1MisTagDown);
	syst1B.setWeight("puUp",bWeight1 * puUpFact);
	syst1B.setWeight("puDown",bWeight1 * puDownFact);
	syst1B.setWeight("lepUp",1);
	syst1B.setWeight("lepDown",1);
	
	if(addTTSplit){
	  syst1B.setWCats(wcats);
	   //	   syst1B.setWeight("0lep",float(NMCLeptons==0),true);	   syst1B.setWeight("1lep",float(NMCLeptons==1),true);	   syst1B.setWeight("2lep",float(NMCLeptons==2),true);
	}
	 if(addPDF)syst1B.setPDFWeights(w_pdfs,nPDF,w_zero,true);
	 if(addQ2)syst1B.setQ2Weights(w_q2up,w_q2down,w_zero,true);
	 if(addTopPt)syst1B.setTWeight(w_top,topWeight,true);
	 if(addVHF)syst1B.setVHFWeight(vhf,true,shiftval);
	 if(addWZNLO){
	   syst1B.setkFact("QCDRen",kfact_qcd,kfact_qcdrenUp,kfact_qcdrenDown);
	   syst1B.setkFact("QCDFac",kfact_qcd,kfact_qcdfacUp,kfact_qcdfacDown);
	   syst1B.setkFact("EWK",kfact_ewk,kfact_ewkUp,kfact_ewkDown);
	 }

	 systWeights1B[PUUP]= puUpFact * bWeight1;
	 systWeights1B[PUDOWN]=puDownFact * bWeight1;
	 systWeights1B[LEPUP]= 1.;//bWeight1 * elIDweightUp * muIDweightUp * elID2weightUp * muID2weightUp* muIsoweightUp * muIso2weightUp* elTrigweightUp * muTrigweightUp;
	 systWeights1B[LEPDOWN]= 1.;//bWeight1 * elIDweightDown * muIDweightDown * elID2weightDown * muID2weightDown* muIsoweightDown * muIso2weightDown* elTrigweightDown * muTrigweightDown;
	 systWeights1B[NOSYST]=bWeight1;
	 systWeights1B[BTAGUP]=bWeight1BTagUp;
	 systWeights1B[BTAGDOWN]=bWeight1BTagDown;
	 systWeights1B[MISTAGUP]=bWeight1MisTagUp;
	 systWeights1B[MISTAGDOWN]=bWeight1MisTagDown;
	 
	 /*systWeights1B[NOSYST]=1.;
	 systWeights1B[BTAGUP]=1.;
	 systWeights1B[BTAGDOWN]=1.;
	 systWeights1B[MISTAGUP]=1.;
	 systWeights1B[MISTAGDOWN]=1.;
	 systWeights1B[PUUP]=1.;
	 systWeights1B[PUDOWN]=1.;
	 systWeights1B[LEPUP]=1.;
	 systWeights1B[LEPDOWN]=1.;*/
      

	 syst0B.copySysts(systZero);
	 syst0B.setWeight(0,bWeightZero);
	 syst0B.setWeight("btagUp",bWeightZeroBTagUp);
	 syst0B.setWeight("btagDown",bWeightZeroBTagDown);
	 syst0B.setWeight("mistagUp",bWeightZeroMisTagUp);
	 syst0B.setWeight("mistagDown",bWeightZeroMisTagDown);
	 syst0B.setWeight("puUp",bWeightZero * puUpFact);
	 syst0B.setWeight("puDown",bWeightZero * puDownFact);
	 syst0B.setWeight("lepUp",1);
	 syst0B.setWeight("lepDown",1);

	 if(addTTSplit){
	   syst0B.setWCats(wcats);
           //      syst0B.setWeight("0lep",float(NMCLeptons==0),true);     syst0B.setWeight("1lep",float(NMCLeptons==1),true);     syst0B.setWeight("2lep",float(NMCLeptons==2),true);                                                                                                                              
	 }
	 if(addPDF)syst0B.setPDFWeights(w_pdfs,nPDF,w_zero,true);
	 if(addQ2)syst0B.setQ2Weights(w_q2up,w_q2down,w_zero,true);
	 if(addTopPt)syst0B.setTWeight(w_top,topWeight,true);
	 if(addVHF)syst0B.setVHFWeight(vhf,true,shiftval);
	 if(addWZNLO){
	   syst0B.setkFact("QCDRen",kfact_qcd,kfact_qcdrenUp,kfact_qcdrenDown);
	   syst0B.setkFact("QCDFac",kfact_qcd,kfact_qcdfacUp,kfact_qcdfacDown);
	   syst0B.setkFact("EWK",kfact_ewk,kfact_ewkUp,kfact_ewkDown);
	 }
	 
	 systWeights0B[PUUP]= puUpFact * bWeightZero;
         systWeights0B[PUDOWN]=puDownFact * bWeightZero;
         systWeights0B[LEPUP]= 1.;//bWeight1 * elIDweightUp * muIDweightUp * elID2weightUp * muID2weightUp* muIsoweightUp * muIso2weightUp* elTrigweightUp *muTrigweightUp;                                                                                                                                             
         systWeights0B[LEPDOWN]= 1.;//bWeight1 * elIDweightDown * muIDweightDown * elID2weightDown * muID2weightDown* muIsoweightDown * muIso2weightDown* el\TrigweightDown * muTrigweightDown;                                                                                                                 
	 systWeights0B[NOSYST]=bWeightZero;
         systWeights0B[BTAGUP]=bWeightZeroBTagUp;
         systWeights0B[BTAGDOWN]=bWeightZeroBTagDown;
         systWeights0B[MISTAGUP]=bWeightZeroMisTagUp;
         systWeights0B[MISTAGDOWN]=bWeightZeroMisTagDown;

	 /*systWeights0B[NOSYST]=1.;                                                                                                                        
         systWeights0B[BTAGUP]=1.;                                                                                                                      
	 systWeights0B[BTAGDOWN]=1.;                                                                                                                       
         systWeights0B[MISTAGUP]=1.;                                                                                                                     
         systWeights0B[MISTAGDOWN]=1.;                                                                                                                  
	 systWeights0B[PUUP]=1.;                                                                                                                     
         systWeights0B[PUDOWN]=1.;                                                                                                                       
	 systWeights0B[LEPUP]=1.;                                                                                                                     
	 systWeights0B[LEPDOWN]=1.;*/
      }
 
      /*systWeightsNoSyst[PUUP]=1.0;
      systWeightsNoSyst[PUDOWN]=1.0;
      systWeightsNoSyst[LEPUP]=1.0;
      systWeightsNoSyst[LEPDOWN]=1.0;
      //systWeightsNoSyst[ISOUP]=1.0;
      //systWeightsNoSyst[ISODOWN]=1.0;
      //systWeightsNoSyst[TRIGUP]=1.0;
      //systWeightsNoSyst[TRIGDOWN]=1.0;
      systWeightsNoSyst[NOSYST]=1.0;
      systWeightsNoSyst[BTAGUP]=1.0;
      systWeightsNoSyst[BTAGDOWN]=1.0;
      systWeightsNoSyst[MISTAGUP]=1.0;
      systWeightsNoSyst[MISTAGDOWN]=1.0;*/

      std::vector<TLorentzVector> jets, bjets, fatjets, Bosonjets;                                                                                                        
      struct btag{
	TLorentzVector vect;
	float csv;                                                                                                                                                     
	float deltar;                                                                                                                                                  
      }; 

      std::vector<btag> bvects, b_selvects, bV_selvects;

      struct Bosontag{
	TLorentzVector vect;
	float prunedmass;
	float nsubjetiness;
	float nsubjetiness32;
	int nsubjets;
	float doubleb;
      };


      int maxJetLoop = min(15, jetSize);
      int maxFatJetLoop = min(10, fatjetSize);
     
      //if((passTrigHT_800v3)>0.){
      //if(passSingleMu>0.){ 
      //if(passSingleMu>0){
      //if(1>0){
      if((isData=="DATA" /*&& passTrigHT_800v3>0.*/) || (isData=="MC")){
      n_trig += w;
      //cout << bWeight1 << endl;
      //cout << bWeightZero << " " <<  w << endl;
     
      //syst0B.fillHistogramsSysts(h_Ht_trigbef,Ht,w,systWeights0B);
      //syst1B.fillHistogramsSysts(h_nPV_w,nGoodPV,w,systWeights1B);

      int countqcjets=0;
      for (int j = 0; j <maxJetLoop;++j){
	if(jetpt[j]>30. && fabs(jeteta[j])<5) {
	  
	  if(jetcsv[j]>0.800){
	    TLorentzVector bjet;
	    bjet.SetPtEtaPhiE(jetpt[j], jeteta[j], jetphi[j], jete[j]);
	    btag b;
	    b.vect = bjet;
	    bvects.push_back(b);
	  }
	  if(fabs(jeteta[j])>2.4 ){
	    countqcjets++;
	  }
	}                                                                                                                               
      }
      syst0B.fillHistogramsSysts(h_AK4fwjets,countqcjets,w,systWeights0B);

	if((cat=="cat_inclusive" && countqcjets>-1) || (cat=="cat0" && countqcjets==0) || (cat=="cat1" && countqcjets>0)){ 

	  n_cat += w;                                                                                                                                           
	  int countjets=0, countfatjets=0;
	  for (int j = 0; j <maxJetLoop;++j){
	    if(jetpt[j]>30. &&  fabs(jeteta[j])<5) {
	      countjets++;
	    }
	  }

	  for(int j=0; j<maxFatJetLoop; ++j){
	    if(fatjetpt[j]>300 && fabs(fatjeteta[j])<2.4){
	      countfatjets++;
	    }
	  }

	  syst0B.fillHistogramsSysts(h_AK8jetmult,countfatjets,w,systWeights0B);
	  syst0B.fillHistogramsSysts(h_AK4jetmult,countjets,w,systWeights0B);
	  
	  if(countfatjets>0){
	    n_fatjet+=1;

	    if(countjets>3){
	      n_jet+=1;

	      //cout << i << " " << bWeight1 << endl;
	      //syst1B.fillHistogramsSysts(h_nPV_w,nGoodPV,w,systWeights1B);
	     
	      syst0B.fillHistogramsSysts(h_nPV,nGoodPV,1.,systWeights0B);
	      syst0B.fillHistogramsSysts(h_nPV_w,nGoodPV,w,systWeights0B);
	      syst0B.fillHistogramsSysts(h_Ht_trigbef,Ht,w,systWeights0B);
	      if(passTrigHT_800v3>0) syst0B.fillHistogramsSysts(h_Ht_trigaft,Ht,w,systWeights0B);
	      if(Ht>0){
        
		std::vector<Bosontag> Bosonvects, Boson_selvects, BosonV_selvects;
	
		syst0B.fillHistogramsSysts(h_AK8jetmultaft,countfatjets,w,systWeights0B);
		syst0B.fillHistogramsSysts(h_AK4jetmultaft,countjets,w,systWeights0B);
	
		int numberJet=0;
		for(int i=0; i<maxJetLoop;++i){
		  if(jetpt[i]>30.){
		    syst0B.fillHistogramsSysts(h_AK4jetpt,jetpt[i],w,systWeights0B);
		    syst0B.fillHistogramsSysts(h_AK4jeteta,jeteta[i],w,systWeights0B);
		    syst0B.fillHistogramsSysts(h_AK4jetphi,jetphi[i],w,systWeights0B);
		    syst0B.fillHistogramsSysts(h_AK4jetcsv,jetcsv[i],w,systWeights0B);
		  }
		}
		int countbjets=0;
		int countBosonjets=0;
		for(int j=0; j<maxFatJetLoop; ++j){
		  if(fatjetpt[j]>300 && fabs(fatjeteta[j])<2.4){
		    syst0B.fillHistogramsSysts(h_AK8jetpt,fatjetpt[j],w,systWeights0B);
		    syst0B.fillHistogramsSysts(h_AK8jeteta,fatjeteta[j],w,systWeights0B);
		    syst0B.fillHistogramsSysts(h_AK8jetphi,fatjetphi[j],w,systWeights0B);
		    syst0B.fillHistogramsSysts(h_nsubj,fatjetnCSVsubj[j],w,systWeights0B);
		    syst0B.fillHistogramsSysts(h_tau2tau1,fatjettau2OVERtau1[j],w,systWeights0B);
		    syst0B.fillHistogramsSysts(h_prunedmass,fatjetprunedmass[j],w,systWeights0B);
		    
		    numberJet++;
		    TLorentzVector jetak8;
		    jetak8.SetPtEtaPhiE(fatjetpt[j], fatjeteta[j], fatjetphi[j], fatjete[j]);

		    TLorentzVector Bosonjet;      
		    countBosonjets++;
		    Bosonjet.SetPtEtaPhiE(fatjetpt[j], fatjeteta[j], fatjetphi[j], fatjete[j]);
		    Bosonjets.push_back(Bosonjet);
		    Bosontag Boson;
		    Boson.vect = Bosonjet;
		    Boson.prunedmass = fatjetprunedmass[j];
		    Boson.nsubjetiness = fatjettau2OVERtau1[j];
		    Boson.nsubjetiness32 = fatjettau3OVERtau2[j];
		    Boson.nsubjets = fatjetnCSVsubj[j];
		    Bosonvects.push_back(Boson);
		    //int countbjets=0;
		    for(int i=0; i<maxJetLoop;++i){
		      if(jetpt[i]>30. /*&& jetcsv[i]>0.8 */&&  fabs(jeteta[i])<2.4){
		    			
			TLorentzVector jetak4;
			jetak4.SetPtEtaPhiE(jetpt[i], jeteta[i], jetphi[i], jete[i]);
			if(jetcsv[i]>0.8){countbjets++;}
			float deltaR_bH2 = -999;
			btag b_sel, bV_sel;
			deltaR_bH2=(Bosonjet).DeltaR(jetak4);
			if(1>0){
			  if (jetcsv[i]>0.8)syst1B.fillHistogramsSysts(h_deltaRbH,deltaR_bH2,w,systWeights1B);
			  if(deltaR_bH2 > 1.2 && jetcsv[i]>0.8){
			    b_sel.vect = jetak4;
			    b_sel.deltar = deltaR_bH2;
			    b_selvects.push_back(b_sel);
			    Boson_selvects.push_back(Boson);
			  }
			}
			if(deltaR_bH2 > 1.2 && jetcsv[i]>0.8){
			  bV_sel.vect = jetak4;
                          bV_sel.deltar = deltaR_bH2;
                          bV_selvects.push_back(b_sel);
                          BosonV_selvects.push_back(Boson);
			}
			
		      }
		    } 
 		  }
		}//end of fat jet loop
		//TH1F *h_AK4jetmult [maxSysts];systZero.initHistogramsSysts(h_AK4jetmult,"h_AK4mult","AK4 mult",20,-0.5,19.5);
		syst0B.fillHistogramsSysts(h_AK4bjetmult,countbjets,w,systWeights0B);
		//if(bV_selvects.size()>0){
		if(1>0){
		  float Bprimemass = 0;
		if(Bosonvects.size()>0){
		  
		  if(Bosonvects[0].nsubjetiness<0.6 && Bosonvects[0].nsubjets==2 && ( Bosonvects[0].prunedmass>105 &&  Bosonvects[0].prunedmass<135)){
		    n_Bosonjets+=w;
		    if(b_selvects.size()>0){
		      n_bjets+=w;
		    }
		  }
		}
		
		
		if(nCSVMJets30>0 && b_selvects.size()>0 && Boson_selvects.size()>0){
		  TLorentzVector Bprime;
		  Bprime = b_selvects[0].vect + Boson_selvects[0].vect;
		  Bprimemass = (b_selvects[0].vect + Boson_selvects[0].vect).M();
		  
		  //fillHistogramsSysts(h_bprimemassvsht, Bprimemass, Ht, w, systWeights1B);
		  h_bprimemassvsht->Fill(Bprimemass, Ht);
		  h_nsubjvsmh->Fill(Boson_selvects[0].nsubjets, Boson_selvects[0].prunedmass);

		  if(Boson_selvects[0].nsubjetiness<0.6 && Boson_selvects[0].nsubjets==2 && ( Boson_selvects[0].prunedmass>105 &&  Boson_selvects[0].prunedmass<135)){
		    syst1B.fillHistogramsSysts(h_Ht,Ht,w,systWeights1B);
		    if(Ht>1250){   
		      n_ht+=w;
		      nA+=w;
		      
		      syst1B.fillHistogramsSysts(h_bprimemass_SR,Bprimemass,w,systWeights1B);
		      syst1B.fillHistogramsSysts(h_bprimept_SR,Bprime.Pt(),w,systWeights1B);
		      syst1B.fillHistogramsSysts(h_bprimeeta_SR,Bprime.Eta(),w,systWeights1B);
		      syst1B.fillHistogramsSysts(h_bprimephi_SR,Bprime.Phi(),w,systWeights1B);

		      syst1B.fillHistogramsSysts(h_AK4jet_selb_pt_SR,((b_selvects[0].vect).Pt()),w,systWeights1B);
		      syst1B.fillHistogramsSysts(h_AK4jet_selb_eta_SR,((b_selvects[0].vect).Eta()),w,systWeights1B);
		      syst1B.fillHistogramsSysts(h_AK4jet_selb_phi_SR,((b_selvects[0].vect).Phi()),w,systWeights1B);
		      
		      syst1B.fillHistogramsSysts(h_AK8jet_selh_pt_SR,((Boson_selvects[0].vect).Pt()),w,systWeights1B);
		      syst1B.fillHistogramsSysts(h_AK8jet_selh_eta_SR,((Boson_selvects[0].vect).Eta()),w,systWeights1B);
		      syst1B.fillHistogramsSysts(h_AK8jet_selh_phi_SR,((Boson_selvects[0].vect).Phi()),w,systWeights1B);
		      
		      syst1B.fillHistogramsSysts(h_nsubj_SR,Boson_selvects[0].nsubjets,w,systWeights1B);
		      syst1B.fillHistogramsSysts(h_tau2tau1_SR,Boson_selvects[0].nsubjetiness,w,systWeights1B);
		      syst1B.fillHistogramsSysts(h_prunedmass_SR,Boson_selvects[0].prunedmass,w,systWeights1B);
		      
		    }
		  }
		  if(Boson_selvects[0].nsubjetiness<0.6 && Boson_selvects[0].nsubjets ==2  && ( Boson_selvects[0].prunedmass<105 || Boson_selvects[0].prunedmass>135)){ //REGION B
		    syst1B.fillHistogramsSysts(h_Ht_CRB,Ht,w,systWeights1B);

		    if(Ht>1250){
		      nB++;
		      syst1B.fillHistogramsSysts(h_bprimemass_CRB,Bprimemass,w,systWeights1B);
		      syst1B.fillHistogramsSysts(h_bprimept_CRB,Bprime.Pt(),w,systWeights1B);
		      syst1B.fillHistogramsSysts(h_bprimeeta_CRB,Bprime.Eta(),w,systWeights1B);
		      syst1B.fillHistogramsSysts(h_bprimephi_CRB,Bprime.Phi(),w,systWeights1B);
		      
		      syst1B.fillHistogramsSysts(h_AK4jet_selb_pt_CRB,((b_selvects[0].vect).Pt()),w,systWeights1B);
		      syst1B.fillHistogramsSysts(h_AK4jet_selb_eta_CRB,((b_selvects[0].vect).Eta()),w,systWeights1B);
		      syst1B.fillHistogramsSysts(h_AK4jet_selb_phi_CRB,((b_selvects[0].vect).Phi()),w,systWeights1B);
		      
		      syst1B.fillHistogramsSysts(h_AK8jet_selh_pt_CRB,((Boson_selvects[0].vect).Pt()),w,systWeights1B);
		      syst1B.fillHistogramsSysts(h_AK8jet_selh_eta_CRB,((Boson_selvects[0].vect).Eta()),w,systWeights1B);
		      syst1B.fillHistogramsSysts(h_AK8jet_selh_phi_CRB,((Boson_selvects[0].vect).Phi()),w,systWeights1B);
		      
		      syst1B.fillHistogramsSysts(h_nsubj_CRB,Boson_selvects[0].nsubjets,w,systWeights1B);
		      syst1B.fillHistogramsSysts(h_tau2tau1_CRB,Boson_selvects[0].nsubjetiness,w,systWeights1B);
		      syst1B.fillHistogramsSysts(h_prunedmass_CRB,Boson_selvects[0].prunedmass,w,systWeights1B);
		  
		    }
		  }
		  if(Boson_selvects[0].nsubjetiness<0.6 && Boson_selvects[0].nsubjets ==1  && (  Boson_selvects[0].prunedmass>105 &&  Boson_selvects[0].prunedmass<135 )){ //REGION C
		    syst1B.fillHistogramsSysts(h_Ht_CRC,Ht,w,systWeights1B);
		    if(Ht>1250){
		      nC++;
		      syst1B.fillHistogramsSysts(h_bprimemass_CRC,Bprimemass,w,systWeights1B);
		      syst1B.fillHistogramsSysts(h_bprimept_CRC,Bprime.Pt(),w,systWeights1B);
		      syst1B.fillHistogramsSysts(h_bprimeeta_CRC,Bprime.Eta(),w,systWeights1B);
		      syst1B.fillHistogramsSysts(h_bprimephi_CRC,Bprime.Phi(),w,systWeights1B);
		      
		      syst1B.fillHistogramsSysts(h_AK4jet_selb_pt_CRC,((b_selvects[0].vect).Pt()),w,systWeights1B);
		      syst1B.fillHistogramsSysts(h_AK4jet_selb_eta_CRC,((b_selvects[0].vect).Eta()),w,systWeights1B);
		      syst1B.fillHistogramsSysts(h_AK4jet_selb_phi_CRC,((b_selvects[0].vect).Phi()),w,systWeights1B);
		      
		      syst1B.fillHistogramsSysts(h_AK8jet_selh_pt_CRC,((Boson_selvects[0].vect).Pt()),w,systWeights1B);
		      syst1B.fillHistogramsSysts(h_AK8jet_selh_eta_CRC,((Boson_selvects[0].vect).Eta()),w,systWeights1B);
		      syst1B.fillHistogramsSysts(h_AK8jet_selh_phi_CRC,((Boson_selvects[0].vect).Phi()),w,systWeights1B);
		      
		      syst1B.fillHistogramsSysts(h_nsubj_CRC,Boson_selvects[0].nsubjets,w,systWeights1B);
		      syst1B.fillHistogramsSysts(h_tau2tau1_CRC,Boson_selvects[0].nsubjetiness,w,systWeights1B);
		      syst1B.fillHistogramsSysts(h_prunedmass_CRC,Boson_selvects[0].prunedmass,w,systWeights1B);
		     
		    }
		  }
		  if(Boson_selvects[0].nsubjetiness<0.6 && Boson_selvects[0].nsubjets==1 && ( Boson_selvects[0].prunedmass<105 || Boson_selvects[0].prunedmass > 135)){ //REGION D
		    syst1B.fillHistogramsSysts(h_Ht_CRD,Ht,w,systWeights1B);
		    if(Ht>1250){
		      nD++;

		      syst1B.fillHistogramsSysts(h_bprimemass_CRD,Bprimemass,w,systWeights1B);
		      syst1B.fillHistogramsSysts(h_bprimept_CRD,Bprime.Pt(),w,systWeights1B);
		      syst1B.fillHistogramsSysts(h_bprimeeta_CRD,Bprime.Eta(),w,systWeights1B);
		      syst1B.fillHistogramsSysts(h_bprimephi_CRD,Bprime.Phi(),w,systWeights1B);
		      
		      syst1B.fillHistogramsSysts(h_AK4jet_selb_pt_CRD,((b_selvects[0].vect).Pt()),w,systWeights1B);
		      syst1B.fillHistogramsSysts(h_AK4jet_selb_eta_CRD,((b_selvects[0].vect).Eta()),w,systWeights1B);
		      syst1B.fillHistogramsSysts(h_AK4jet_selb_phi_CRD,((b_selvects[0].vect).Phi()),w,systWeights1B);
		      
		      syst1B.fillHistogramsSysts(h_AK8jet_selh_pt_CRD,((Boson_selvects[0].vect).Pt()),w,systWeights1B);
		      syst1B.fillHistogramsSysts(h_AK8jet_selh_eta_CRD,((Boson_selvects[0].vect).Eta()),w,systWeights1B);
		      syst1B.fillHistogramsSysts(h_AK8jet_selh_phi_CRD,((Boson_selvects[0].vect).Phi()),w,systWeights1B);
		      
		      syst1B.fillHistogramsSysts(h_nsubj_CRD,Boson_selvects[0].nsubjets,w,systWeights1B);
		      syst1B.fillHistogramsSysts(h_tau2tau1_CRD,Boson_selvects[0].nsubjetiness,w,systWeights1B);
		      syst1B.fillHistogramsSysts(h_prunedmass_CRD,Boson_selvects[0].prunedmass,w,systWeights1B);
		    }
		  }	  
		} 
	      } 
	    } 
	  }
	} 
	}
	}
      
      fileout
	<<std::fixed<<std::setprecision(0)
	<<runNumber<<"   "
	<<evtNumber<<"   "
	<<lumiSec<<"   "
	<<(double) n_closure/nEventsPrePres<< "   "
       	<<std::endl;

       
      
    }//end of loop over events

  h_cutFlow->SetBinContent(0,nEventsPrePres);
  h_cutFlow->GetXaxis()->SetBinLabel(0,"no selection");
  h_cutFlow->SetBinContent(1, n_trig);
  h_cutFlow->GetXaxis()->SetBinLabel(1, "trigger");
  h_cutFlow->SetBinContent(2, n_fatjet);
  h_cutFlow->GetXaxis()->SetBinLabel(2, "AK8 jet");
  h_cutFlow->SetBinContent(3, n_jet);
  h_cutFlow->GetXaxis()->SetBinLabel(3, "AK4 jet");
  h_cutFlow->SetBinContent(4, n_Bosonjets);
  h_cutFlow->GetXaxis()->SetBinLabel(4, "Higgs jet");
  h_cutFlow->SetBinContent(5, n_bjets);
  h_cutFlow->GetXaxis()->SetBinLabel(5, "bjet");
  h_cutFlow->SetBinContent(6, n_ht);
  h_cutFlow->GetXaxis()->SetBinLabel(6, "Ht");

  h_nsubjvsmh->Write();
  h_bprimemassvsht->Write();

  systZero.writeSingleHistogramSysts(h_cutFlow, allMyFiles);

  syst0B.writeHistogramsSysts(h_nPV_w, allMyFiles);
  syst0B.writeHistogramsSysts(h_nPV, allMyFiles);
  syst0B.writeHistogramsSysts(h_AK4fwjets, allMyFiles);
  syst0B.writeHistogramsSysts(h_AK4jetmult, allMyFiles);
  syst0B.writeHistogramsSysts(h_AK4bjetmult, allMyFiles);
  syst0B.writeHistogramsSysts(h_AK4jetmultaft, allMyFiles);
  syst0B.writeHistogramsSysts(h_AK8jetmult, allMyFiles);
  syst0B.writeHistogramsSysts(h_AK8jetmultaft, allMyFiles);

  syst0B.writeHistogramsSysts(h_Ht_trigbef, allMyFiles);
  syst0B.writeHistogramsSysts(h_Ht_trigaft, allMyFiles);
  syst1B.writeHistogramsSysts(h_Ht, allMyFiles);
  syst1B.writeHistogramsSysts(h_Ht_CRB, allMyFiles);
  syst1B.writeHistogramsSysts(h_Ht_CRC, allMyFiles);
  syst1B.writeHistogramsSysts(h_Ht_CRD, allMyFiles);
  syst1B.writeHistogramsSysts(h_bprimemass_SR, allMyFiles);
  syst1B.writeHistogramsSysts(h_bprimemass_CRB, allMyFiles);
  syst1B.writeHistogramsSysts(h_bprimemass_CRC, allMyFiles);
  syst1B.writeHistogramsSysts(h_bprimemass_CRD, allMyFiles);

  syst1B.writeHistogramsSysts(h_bprimept_SR, allMyFiles);
  syst1B.writeHistogramsSysts(h_bprimept_CRB, allMyFiles);
  syst1B.writeHistogramsSysts(h_bprimept_CRC, allMyFiles);
  syst1B.writeHistogramsSysts(h_bprimept_CRD, allMyFiles);

  syst1B.writeHistogramsSysts(h_bprimeeta_SR, allMyFiles);
  syst1B.writeHistogramsSysts(h_bprimeeta_CRB, allMyFiles);
  syst1B.writeHistogramsSysts(h_bprimeeta_CRC, allMyFiles);
  syst1B.writeHistogramsSysts(h_bprimeeta_CRD, allMyFiles);

  syst1B.writeHistogramsSysts(h_bprimephi_SR, allMyFiles);
  syst1B.writeHistogramsSysts(h_bprimephi_CRB, allMyFiles);
  syst1B.writeHistogramsSysts(h_bprimephi_CRC, allMyFiles);
  syst1B.writeHistogramsSysts(h_bprimephi_CRD, allMyFiles);

  syst0B.writeHistogramsSysts(h_nsubj, allMyFiles);
  syst0B.writeHistogramsSysts(h_tau2tau1, allMyFiles);
  syst0B.writeHistogramsSysts(h_prunedmass, allMyFiles);

  syst0B.writeHistogramsSysts(h_AK8jetpt, allMyFiles);
  syst0B.writeHistogramsSysts(h_AK8jeteta, allMyFiles);
  syst0B.writeHistogramsSysts(h_AK8jetphi, allMyFiles);
  syst0B.writeHistogramsSysts(h_AK4jetpt, allMyFiles);
  syst0B.writeHistogramsSysts(h_AK4jetphi, allMyFiles);
  syst0B.writeHistogramsSysts(h_AK4jeteta, allMyFiles);
  syst0B.writeHistogramsSysts(h_AK4jetcsv, allMyFiles);
  syst0B.writeHistogramsSysts(h_deltaRbH, allMyFiles);

  syst1B.writeHistogramsSysts(h_AK4jet_selb_pt_SR, allMyFiles);
  syst1B.writeHistogramsSysts(h_AK4jet_selb_eta_SR, allMyFiles);
  syst1B.writeHistogramsSysts(h_AK4jet_selb_phi_SR, allMyFiles);

  syst1B.writeHistogramsSysts(h_AK8jet_selh_pt_SR, allMyFiles);
  syst1B.writeHistogramsSysts(h_AK8jet_selh_eta_SR, allMyFiles);
  syst1B.writeHistogramsSysts(h_AK8jet_selh_phi_SR, allMyFiles);

  syst1B.writeHistogramsSysts(h_nsubj_SR, allMyFiles);
  syst1B.writeHistogramsSysts(h_tau2tau1_SR, allMyFiles);
  syst1B.writeHistogramsSysts(h_prunedmass_SR, allMyFiles);
  
  syst1B.writeHistogramsSysts(h_bprimemass_CRB, allMyFiles);
  syst1B.writeHistogramsSysts(h_bprimept_CRB, allMyFiles);
  syst1B.writeHistogramsSysts(h_bprimeeta_CRB, allMyFiles);
  syst1B.writeHistogramsSysts(h_bprimephi_CRB, allMyFiles);

  syst1B.writeHistogramsSysts(h_AK4jet_selb_pt_CRB, allMyFiles);
  syst1B.writeHistogramsSysts(h_AK4jet_selb_eta_CRB, allMyFiles);
  syst1B.writeHistogramsSysts(h_AK4jet_selb_phi_CRB, allMyFiles);

  syst1B.writeHistogramsSysts(h_AK8jet_selh_pt_CRB, allMyFiles);
  syst1B.writeHistogramsSysts(h_AK8jet_selh_eta_CRB, allMyFiles);
  syst1B.writeHistogramsSysts(h_AK8jet_selh_phi_CRB, allMyFiles);

  syst1B.writeHistogramsSysts(h_nsubj_CRB, allMyFiles);
  syst1B.writeHistogramsSysts(h_tau2tau1_CRB, allMyFiles);
  syst1B.writeHistogramsSysts(h_prunedmass_CRB, allMyFiles);

  syst1B.writeHistogramsSysts(h_bprimemass_CRC, allMyFiles);
  syst1B.writeHistogramsSysts(h_bprimept_CRC, allMyFiles);
  syst1B.writeHistogramsSysts(h_bprimeeta_CRC, allMyFiles);
  syst1B.writeHistogramsSysts(h_bprimephi_CRC, allMyFiles);

  syst1B.writeHistogramsSysts(h_AK4jet_selb_pt_CRC, allMyFiles);
  syst1B.writeHistogramsSysts(h_AK4jet_selb_eta_CRC, allMyFiles);
  syst1B.writeHistogramsSysts(h_AK4jet_selb_phi_CRC, allMyFiles);

  syst1B.writeHistogramsSysts(h_AK8jet_selh_pt_CRC, allMyFiles);
  syst1B.writeHistogramsSysts(h_AK8jet_selh_eta_CRC, allMyFiles);
  syst1B.writeHistogramsSysts(h_AK8jet_selh_phi_CRC, allMyFiles);

  syst1B.writeHistogramsSysts(h_nsubj_CRC, allMyFiles);
  syst1B.writeHistogramsSysts(h_tau2tau1_CRC, allMyFiles);
  syst1B.writeHistogramsSysts(h_prunedmass_CRC, allMyFiles);

  syst1B.writeHistogramsSysts(h_bprimemass_CRD, allMyFiles);
  syst1B.writeHistogramsSysts(h_bprimept_CRD, allMyFiles);
  syst1B.writeHistogramsSysts(h_bprimeeta_CRD, allMyFiles);
  syst1B.writeHistogramsSysts(h_bprimephi_CRD, allMyFiles);

  syst1B.writeHistogramsSysts(h_AK4jet_selb_pt_CRD, allMyFiles);
  syst1B.writeHistogramsSysts(h_AK4jet_selb_eta_CRD, allMyFiles);
  syst1B.writeHistogramsSysts(h_AK4jet_selb_phi_CRD, allMyFiles);

  syst1B.writeHistogramsSysts(h_AK8jet_selh_pt_CRD, allMyFiles);
  syst1B.writeHistogramsSysts(h_AK8jet_selh_eta_CRD, allMyFiles);
  syst1B.writeHistogramsSysts(h_AK8jet_selh_phi_CRD, allMyFiles);

  syst1B.writeHistogramsSysts(h_nsubj_CRD, allMyFiles);
  syst1B.writeHistogramsSysts(h_tau2tau1_CRD, allMyFiles);
  syst1B.writeHistogramsSysts(h_prunedmass_CRD, allMyFiles);
  //fout.cd();

  //fout.Close();
  fileout.close();
  
  std::cout<< "---> "<<sample<<std::endl;
  
  std::cout<< "**************************** "<<std::endl;
  std::cout<< "* Cutflow in Signal Region *    "<<std::endl;
  std::cout<< "**************************** "<<std::endl;
  
  std::cout<< "Number of Events           : "<<nEventsPrePres<<std::endl;
  std::cout<< "Events after trigger cut   : "<<n_trig<<std::endl;
  std::cout<< "Events after 1 ak8 jet     : "<<n_fatjet<<std::endl;
  std::cout<< "Events after 3 ak4 jets    : "<<n_jet<<std::endl;
  std::cout<< "Events after 1 higgs jet   : "<<n_Bosonjets<<std::endl;
  std::cout<< "Events after 1 b jet       : "<<n_bjets<<std::endl;
  std::cout<< "Events after Ht cut        : "<<n_ht<<std::endl;

  std::cout<< "****************************** "<<std::endl;
  std::cout<< "* Final yields in SR and CRs * "<<std::endl;
  std::cout<< "****************************** "<<std::endl;
  
  std::cout<< "Events in region subjet b-tagging        : "<<nA<<std::endl;
  std::cout<< "Events in region double b-tagging        : "<<nB<<std::endl;
  std::cout<< "Events in region C        : "<<nC<<std::endl;
  std::cout<< "Events in region D        : "<<nD<<std::endl;
  

  std::cout<< "Contamination in Closure Region: " << (float)n_closure/nA << std::endl;
  
}//end of main

TH1F * initproduct(TH1F * hA,TH1F* hB, int rebinA = 1, int rebinB=1,double integral = -1.){
  int nbinsA = hA->GetNbinsX();
  int nbinsB = hA->GetNbinsX();
  double min = hA->GetBinLowEdge(1)*hB->GetBinLowEdge(1);
  double max = hA->GetBinLowEdge(nbinsA+1)*hB->GetBinLowEdge(nbinsB+1);
  //Get the actual name from the original histograms 
  string name =(string)(hA->GetName()) +"_vs_"+ (string)(hB->GetName());
  
  //Initialize histogram 
  TH1F * result = new TH1F(name.c_str(),name.c_str(),nbinsA*nbinsB,min,max);
  return result;
}

TH1F * makeproduct(TH1F * hA,TH1F* hB, int rebinA = 1, int rebinB=1,double integral = -1.){

  //Make temporary histos to rebin
  //  TH1F *hA = (TH1F*)h_A->Clone("hA");
  // TH1F *hB = (TH1F*)h_B->Clone("hB");

  //  hA->Rebin(rebinA);
  // hB->Rebin(rebinB);
  
  //get nbins from new histos
  int nbinsA = hA->GetNbinsX();
  int nbinsB = hA->GetNbinsX();
  double min = hA->GetBinLowEdge(1)*hB->GetBinLowEdge(1);
  double max = hA->GetBinLowEdge(nbinsA+1)*hB->GetBinLowEdge(nbinsB+1);
  //Get the actual name from the original histograms 
  string name =(string)(hA->GetName()) +"_vs_"+ (string)(hB->GetName());
  
  //Initialize histogram 
  TH1F * result = new TH1F(name.c_str(),name.c_str(),nbinsA*nbinsB,min,max);
  //Fill histogram
  for(int i =1; i<= nbinsA;++i){
    for(int j =1; j<= nbinsB;++j){
      double value = hA->GetBinContent(i)*hB->GetBinContent(j);
      int k = ((i-1)*nbinsB)+j;
      result->SetBinContent(k,value);
    }
  }
  if( integral <= 0.)integral = hB->Integral()/result->Integral();
  else integral = integral / result->Integral();
  result->Scale(integral);
  return result;

}

//void initHistogramsSysts(TH1F* histo, TString name, TString, int, float, float , bool useOnlyNominal=false);

void systWeights::copySysts(systWeights sys){
  for(int i =0; i < sys.maxSysts;++i){
    this->weightedNames[i]=sys.weightedNames[i];
    this->weightedSysts[i]=sys.weightedSysts[i];

  }
  this->setMax(sys.maxSysts);
  this->setMaxNonPDF(sys.maxSystsNonPDF);
  this->nPDF=sys.nPDF;
  this->nCategories=sys.nCategories;  
  this->addQ2=sys.addQ2;
  this->addPDF=sys.addPDF;
  this->addTopPt=sys.addTopPt;
  this->addVHF=sys.addVHF;
  this->addTTSplit=sys.addTTSplit;
}


void systWeights::prepareDefault(bool addDefault, bool addQ2, bool addPDF, bool addTopPt,bool addVHF, bool addTTSplit, int numPDF){
  this->addPDF=addPDF;
  this->addQ2=addQ2;
  this->addTopPt=addTopPt;
  this->addVHF=addVHF;
  this->addTTSplit=addTTSplit;
  this->nPDF=numPDF;
  this->nCategories=1;
  categoriesNames[0]="";
  this->wCats[0]=1.0;
  if(addDefault){
    //    int MAX = this->maxSysts;
    this->weightedNames[0]="";
    this->weightedNames[1]="btagUp";
    this->weightedNames[2]="btagDown";
    this->weightedNames[3]="mistagUp";
    this->weightedNames[4]="mistagDown";
    this->weightedNames[5]="puUp";
    this->weightedNames[6]="puDown";
    this->weightedNames[7]="lepUp";
    this->weightedNames[8]="lepDown";
    //this->weightedNames[9]="isoUp";
    //this->weightedNames[10]="isoDown";
    //this->weightedNames[11]="trigUp";
    //this->weightedNames[12]="trigDown";
    this->setMax(9);
    this->setMaxNonPDF(9);
    this->weightedNames[this->maxSysts]="";
  }
  if(addQ2){
    this->weightedNames[this->maxSysts]= "q2Up";
    this->weightedNames[this->maxSysts+1]= "q2Down";
    this->setMax(this->maxSysts+2);
    this->setMaxNonPDF(this->maxSystsNonPDF+2);
    this->weightedNames[this->maxSysts]= "";
  }

  if(addTopPt){
    this->weightedNames[this->maxSysts]="topPtWeightUp";
    this->weightedNames[this->maxSysts+1]="topPtWeightDown";
    this->setMax(this->maxSysts+2);
    this->setMaxNonPDF(this->maxSystsNonPDF+2);
    this->weightedNames[this->maxSysts]= "";
  }
  

  if(addVHF){
    this->weightedNames[this->maxSysts]="VHFWeightUp";
    this->weightedNames[this->maxSysts+1]="VHFWeightDown";
    this->setMax(this->maxSysts+2);
    this->setMaxNonPDF(this->maxSystsNonPDF+2);
    this->weightedNames[this->maxSysts]= "";
  }

  if(addTTSplit){
    //    this->weightedNames[this->maxSysts]="2lep";
    //    this->weightedNames[this->maxSysts+1]="1lep";
    //    this->weightedNames[this->maxSysts+2]="0lep";
    //    this->setMax(this->maxSysts+3);
    //    this->setMaxNonPDF(this->maxSystsNonPDF+3);
    //    this->weightedNames[this->maxSysts]= "";
    this->nCategories=4;
    categoriesNames[1]="TT0lep";
    categoriesNames[2]="TT1lep";
    categoriesNames[3]="TT2lep";
    this->wCats[1]=1.0;
    this->wCats[2]=1.0;
    this->wCats[3]=1.0;

  }


  /*  if(addkFact){
    this->weightedNames[this->maxSysts]="VHFWeightUp";
    this->weightedNames[this->maxSysts+1]="VHFWeightDown";
    this->setMax(this->maxSysts+2);
    this->setMaxNonPDF(this->maxSystsNonPDF+2);
    this->weightedNames[this->maxSysts]= "";
    }*/

  if(addPDF){
    this->weightedNames[this->maxSysts]= "pdf_totalUp";
    this->weightedNames[this->maxSysts+1]= "pdf_totalDown";
    this->weightedNames[this->maxSysts+2]= "pdf_asUp";
    this->weightedNames[this->maxSysts+3]= "pdf_asDown";
    this->weightedNames[this->maxSysts+4]= "pdf_zmUp";
    this->weightedNames[this->maxSysts+5]= "pdf_zmDown";
    this->setMax(this->maxSysts+6);
    this->setMaxNonPDF(this->maxSystsNonPDF+6);
    int nPDF=this->nPDF;
    for(int i =0; i < nPDF;++i){
      stringstream ss;
      ss<< i+1;
      this->weightedNames[i+this->maxSysts]= "pdf"+ss.str();
    }
    this->setMax(maxSysts+nPDF);
    this->weightedNames[this->maxSysts]= "";
  }
  
}
void systWeights::addSyst(string name){
  this->weightedNames[this->maxSysts]= name;
  this->setMax(maxSysts+1);
  if(name.find("pdf")!=std::string::npos)this->setMaxNonPDF(maxSysts+1);
  this->weightedNames[this->maxSysts]= "";
}

void systWeights::addSystNonPDF(string name){
  this->weightedNames[this->maxSystsNonPDF]= name;
  this->setMaxNonPDF(maxSystsNonPDF+1);
  int nPDF=this->nPDF;
  for(int i =0; i < nPDF;++i){
    stringstream ss;
    ss<< i+1;
    this->weightedNames[i+this->maxSystsNonPDF]= "pdf"+ss.str();
  }
  this->setMax(maxSystsNonPDF+nPDF);
  this->weightedNames[this->maxSysts]= "";
}

void systWeights::addkFact(string name){
  string up=name+"Up";
  string down=name+"Down";
  cout << " adding syst "<< up<<endl;
  this->addSystNonPDF(up);
  this->addSystNonPDF(down);
}

void systWeights::setkFact(string name, float kfact_nom, float kfact_up,float kfact_down, bool mult){
  //  void setkFact(string name,float kfact_nom, float kfact_up,float kfact_down, float w_zero=1.0, mult=true);
  float zerofact=1.0;
  if(mult)zerofact=this->weightedSysts[0];
  string up = name+"Up";
  string down = name+"Down";
  float valueup=kfact_up/kfact_nom;
  float valuedown=kfact_down/kfact_nom;
  //  cout << "setting syst "<< up<<endl;
  //  cout << "values nom "<<kfact_nom<< " up "<< kfact_up << " down "<< kfact_down << " valup "<< valueup<< " valdown "<< valuedown <<" zerofact "<< zerofact<<endl;
  this->setSystValue(up, valueup*zerofact);
  this->setSystValue(down, valuedown*zerofact);

}

void systWeights::setPDFWeights(float * wpdfs, int numPDFs, float wzero,bool mult){
  float zerofact=1.0;
  if(mult)zerofact=this->weightedSysts[0];
  for (int i = 0; i < numPDFs; ++i){
    this->setPDFValue(i,zerofact*wpdfs[i]/wzero);
  }
  this->setSystValue("pdf_asUp", this->getPDFValue(this->nPDF-2)/wzero);
  this->setSystValue("pdf_asDown", zerofact);
  this->setSystValue("pdf_zmUp", this->getPDFValue(this->nPDF-1)/wzero);
  this->setSystValue("pdf_zmDown", zerofact);
  this->setSystValue("pdf_totalUp", zerofact);
  this->setSystValue("pdf_totalDown", zerofact);
}

//void systWeights::setTWeight(float tweight, float totalweight){
void systWeights::setTWeight(float tweight, float wtotsample,bool mult){
  float zerofact=1.0;
  //  cout << " weighted syst 0 is "<< weightedSysts[0]<<endl;
  if(mult)zerofact=this->weightedSysts[0];
  this->setSystValue("topPtWeightUp", zerofact*tweight/wtotsample);
  this->setSystValue("topPtWeightDown", zerofact/tweight*wtotsample);
}

void systWeights::setVHFWeight(int vhf,bool mult,double shiftval){
  float zerofact=1.0;
  double w_shift=0.0;
  //  cout << "vhf is "<<vhf<<endl;
  if (vhf>1)w_shift=shiftval;
  //  cout << " weighted syst 0 is "<< weightedSysts[0]<<endl;
  if(mult)zerofact=this->weightedSysts[0];
  this->setSystValue("VHFWeightUp", zerofact*(1+w_shift));
  this->setSystValue("VHFWeightDown", zerofact*(1-w_shift));
}


void systWeights::setQ2Weights(float q2up, float q2down, float wzero, bool mult){
  float zerofact=1.0;
  if(mult){
    zerofact=this->weightedSysts[0];
    //    cout <<  "zerofact "<< zerofact << endl;
  }
  //  cout <<  "zerofact "<< zerofact << " q2up weight "<< q2up/wzero << " tot to fill "<< zerofact*q2up/wzero<<endl;
  //  cout <<  "zerofact "<< zerofact << " q2down weight "<< q2down/wzero << " tot to fill "<< zerofact*q2down/wzero<<endl;
  this->setSystValue("q2Up", zerofact*q2up/wzero);
  this->setSystValue("q2Down", zerofact*q2down/wzero);
}

double systWeights::getPDFValue(int numPDF){
  if(!addPDF){ cout << "error! No PDF used, this will do nothing."<<endl;return 0.;}
  int MIN = this->maxSystsNonPDF;
  return (double)this->weightedSysts[numPDF+MIN];

}
void systWeights::setPDFValue(int numPDF, double w){
  if(!addPDF){ cout << "error! No PDF used, this will do nothing."<<endl;return;}
  int MIN = this->maxSystsNonPDF;
  this->weightedSysts[numPDF+MIN]=w;

}

void systWeights::calcPDFHisto(TH1F** histo, TH1F* singleHisto, double scalefactor, int c){//EXPERIMENTAL
  if(!addPDF){ cout << "error! No PDF used, this will do nothing."<<endl;return;}
  int MAX = this->maxSysts;
  //  for (int c = 0; c < this->nCategories; c++){
    int MIN = this->maxSystsNonPDF+(MAX+1)*c;
    for(int b = 0; b< singleHisto->GetNbinsX();++b){
      float val = singleHisto->GetBinContent(b);
      //      cout << "bin # "<<b << " val "<<val<<endl;
      float mean = 0, devst=0;
      //      cout << "name is "<< singleHisto->GetName()<<endl;
      for(int i = 0; i<this->nPDF;++i ){
	//cout<< " now looking at pdf # "<<i<< " coordinate is "<< MIN+i<<endl;
	//	cout << "is histo there? "<< histo[i+MIN]<<endl;
	//	cout << " histo should be "<< (histo[i+MIN])->GetName()<<endl;
	mean = mean+ histo[i+MIN]->GetBinContent(b);
      }
      mean = mean/this->nPDF;
      //mean = val;//mean/this->nPDF;
      for(int i = 0; i<this->nPDF;++i ){
	devst+=(mean-histo[i+MIN]->GetBinContent(b))*(mean-histo[i+MIN]->GetBinContent(b));
      }
      devst= sqrt(devst/this->nPDF);
      singleHisto->SetBinContent(b,val+devst*scalefactor);
      //      singleHisto->SetBinContent(b,mean+devst*scalefactor);
    }
    //}
}

void systWeights::initHistogramsSysts(TH1F** histo,TString name, TString title, int nbins, float min, float max){
  for (int c = 0; c < this->nCategories; c++){
    int MAX = this->maxSysts;
    bool useOnlyNominal = this->onlyNominal;
    TString cname= (this->categoriesNames[c]).c_str();
    for(int sy=0;sy<(int)MAX;++sy){
      TString ns= (this->weightedNames[sy]).c_str();
      if(sy==0){
	if(c==0) histo[sy+((MAX+1)*(c))]=new TH1F(name,title,nbins,min,max);
	else histo[sy+((MAX+1)*(c))]=new TH1F(name+"_"+cname,title,nbins,min,max);
      }
      if(sy!=0 && !useOnlyNominal) {
	if(c==0)histo[sy+(MAX+1)*c]=new TH1F(name+"_"+ns,title,nbins,min,max);
	else histo[sy+(MAX+1)*c]=new TH1F(name+"_"+ns+"_"+cname,title,nbins,min,max);
      }
      cout << " initialized histogram "<< histo[sy+(MAX+1)*c]->GetName() <<" sy " << sy << " c  "<< c <<" location " << sy+(MAX+1)*c << endl;

    }
  }
}

void systWeights::setOnlyNominal(bool useOnlyNominal){
  this->onlyNominal=useOnlyNominal;
}

void systWeights::setWCats(double * wcats){
  for(int i =0;i<this->nCategories;++i){
    //    cout << "setting wcat #"<< i << " to be "<<wcats[i]<<endl;
    this->wCats[i]=wcats[i];
  }
 
}

void systWeights::fillHistogramsSysts(TH1F** histo, float v, float w,  float *systWeights, int nFirstSysts, double * wcats, bool verbose){
  if(wcats== NULL){
    wcats = this->wCats;
  }
  //cout << *wcats << endl;
  for (int c = 0; c < this->nCategories; c++){
    int MAX = this->maxSysts;
    bool useOnlyNominal = this->onlyNominal;
    //cout << " filling histo " << histo[0+(MAX+1)*(c)]->GetName()<< endl;
    //cout << "MAX " << MAX <<endl;
    //cout << " filling histo " << histo[0+(MAX+1)*(c)]->GetName()<< " MAX "<<MAX*(1+c)<<" nFirstSysts"<< nFirstSysts<< endl;
    //    cout << "weight 0 "<< systWeights[0]<< " weighted syst 0 "<< this->weightedSysts[0]<<endl;
    for(int sy=0;sy<(int)MAX;++sy){
      //cout << wcats[c] << endl;
      //cout << "sy" << sy << endl;
      //cout << "filling histo " << histo[(int)sy]->GetName()<<endl;
      if(sy!=0 && useOnlyNominal)continue;
      float ws=1.0;
      if(sy<nFirstSysts){
	//cout << "wcats" << "\t" << wcats[c] << endl;
	wcats[c] =1.0;
	//ws=systWeights[sy];
	ws=systWeights[sy]*wcats[c];
	//cout << "wc" << wcats[c] << endl;
	//cout << sy<<"\t"<<systWeights[sy] << endl; 
	//cout << "ws" << ws << endl;
	//cout << sy<<"\t"<<systWeights[sy] << endl;
      }
      else {
	//cout << "wcats" << "\t" << wcats[c] << endl;
	wcats[c] =1.0;
	ws = (this->weightedSysts[(int)sy]*wcats[c]);
	//ws = (this->weightedSysts[(int)sy]);
	//cout << "ws" << ws << endl;
	//cout << this->weightedSysts[(int)sy] << endl;
      }
      //cout << ws << endl;
      //cout << "filling histo " << histo[sy+1]->GetName()<<" value "<< v << " wevt "<< w << " syst number "<< sy<< " name "<< weightedNames[sy]<<" ws value " <<ws<< " wcats" << wcats[c] << endl;    
      //cout <</* "filling histo "<< histo[sy+((MAX+1)*(c))]->GetName()<<*/" value "<< v << " wevt "<< w << " syst number "<< sy<< " name "<< weightedNames[sy]<<" ws value " <<ws<<endl;
      //cout << "c\t" << c << endl;
      //cout << MAX << endl;
      //cout << sy << endl;
      //cout <<sy+((MAX+1)*(c)) << endl;
      //cout << "filling histo " << histo[sy]->GetName()<<endl;
      //histo[1]->Fill(v);
      histo[sy+(MAX+1)*(c)]->Fill(v, w * ws);
      //cout << histo[sy+(MAX+1)*(c)]->Integral() << " " << v << " " << w << " " << ws << endl;
    }
  }
}

void systWeights::fillHistogramsSysts(TH1F** histo, float v, float w, double * wcats, bool verbose ){
  if(wcats==NULL){
    wcats=this->wCats;
  }
  for (int c = 0; c < this->nCategories; c++){
    int MAX = this->maxSysts;
    bool useOnlyNominal = this->onlyNominal;
    for(int sy=0;sy<(int)MAX;++sy){
      if(sy!=0 && useOnlyNominal)continue;
      float ws = (this->weightedSysts[(int)sy])*wcats[c];
      // cout << " filling histogram "<< histo[(int)sy]->GetName() << " with value "<< v <<" and weight "<< w <<" ws "<< ws<<endl;
      histo[sy+(MAX+1)*(c)]->Fill(v, w*ws);
    }
  }
}


void systWeights::createFilesSysts(  TFile ** allFiles, TString basename, TString opt){
  for (int c = 0; c < this->nCategories; c++){
    int MAX = this->maxSystsNonPDF;
    int MAXTOT = this->maxSystsNonPDF;
    bool useOnlyNominal = this->onlyNominal;
    TString cname= (this->categoriesNames[c]).c_str();
    if (c!=0) cname= "_"+cname;
    for(int sy=0;sy<(int)MAX;++sy){
      TString ns= (this->weightedNames[(int)sy]);
      cout << " creating file for syst "<< ns<<endl;
      if (c!=0)     cout << " category is "<< c<<endl;
      cout << "onlynominal is "<<useOnlyNominal<<endl;

      //    "/afs/cern.ch/user/o/oiorio/public/xAnnapaola/Nov10/res/"+sample + "_" +channel+".root";
      if(sy==0){
	//cout<<" filename is "<< basename+ns+cname+".root"<<endl;
	allFiles[sy+(MAX+1)*c]= TFile::Open((basename+ns+cname+".root"), opt);
      }
      else{
	if(!useOnlyNominal){
	  //if((ns!="1lep") && (ns!="2lep")&& (ns!="0lep")){
	  //	  cout<<" filename is "<< basename+ns+cname+".root"<<endl;
	  allFiles[sy+(MAX+1)*c]= TFile::Open((basename+"_"+ns+cname+".root"), opt);
	}
      }
      //TFile *outTree = TFile::Open(("trees/tree_"+outFileName).c_str(), "RECREATE");
      //cout << " created file at c "<< c << " s "<< sy << " location "<< sy+(MAXTOT+1)*c<< " fname "<<allFiles[sy+(MAXTOT+1)*c]->GetName()<<endl;   
    }
    if(this->addPDF){
      if(!useOnlyNominal)allFiles[MAX+((MAX+1)*c)]= TFile::Open((basename+"_pdf"+cname+".root"), opt);
      //cout << " created file at c "<< c << " s "<< MAX+(MAX+1)*c << " location "<< MAX+(MAX+1)*c<<endl;
      cout<< " fname "<<allFiles[MAX+(MAXTOT+1)*c]->GetName()<<endl;   
    }
  }
  //return allFiles;
}

void systWeights::writeHistogramsSysts(TH1F** histo, TFile **filesout){  
  int MAX= this->maxSystsNonPDF;
  int MAXTOT= this->maxSysts;
  bool useOnlyNominal = this->onlyNominal;
  for (int c = 0; c < this->nCategories; c++){
    TString cname= (this->categoriesNames[c]).c_str();
    if (c!=0) cname= "_"+cname;
    for(int sy=0;sy<(int)MAX;++sy){
      //      cout << "c is now "<< c << " sy "<< sy << " location "<< sy+(MAXTOT+1)*c <<" is histo there? " << histo[sy+(MAXTOT+1)*c] << " file location "<<sy+(MAX+1)*c << " is file there "<< filesout[sy+(MAX+1)*c]<< endl;
      //      cout << " writing histo "<< histo[sy+(MAXTOT+1)*c]->GetName()<< " in file "<< filesout[sy+(MAX+1)*c]->GetName()<<endl;;
      //TString ns= weightedSystsNames((weightedSysts)sy);
      if(!(!useOnlyNominal || sy==0)) continue;
      
      filesout[(int)sy+(MAX+1)*(c)]->cd();
      if(this->addPDF){
	if(this->weightedNames[sy]=="pdf_totalUp")calcPDFHisto(histo, histo[sy+(MAXTOT+1)*(c)],1.0,c);
	if(this->weightedNames[sy]=="pdf_totalDown")calcPDFHisto(histo, histo[sy+(MAXTOT+1)*(c)],-1.0,c);
	;      //this->
      }
      
      histo[sy+(MAXTOT+1)*c]->Write(histo[0]->GetName());
      //    histo[sy]=new TH1F(name+ns,name+ns,nbins,min,max);
      //    filesout[(int)sy]->Close();
    }
    if(this->addPDF){
      if(!useOnlyNominal){
	filesout[MAX+(MAX+1)*(c)]->cd();
	//	cout << " file max is "<< filesout[MAX+(MAX+1)*c]->GetName()<<endl;
	//	int npdf=this->maxSysts-this->maxSystsNonPdf;
	int MAXPDF=this->maxSysts;
	for(int sy=MAX;sy<MAXPDF;++sy){
	  //	  cout << " writing sy "<<sy+(MAXTOT+1)*c<<endl;
	  //	  cout << " histo is there? "<< histo[sy+(MAXTOT+1)*c]<<endl;
	  histo[sy+(MAXTOT+1)*(c)]->Write();
	  //	  cout << " written sy "<< histo[sy+(MAXTOT+1)*c]->GetName()<<endl;
	}
      }
    }
  }
}

void systWeights::writeSingleHistogramSysts(TH1F* histo, TFile **filesout){  
  int MAX= this->maxSystsNonPDF;
  bool useOnlyNominal = this->onlyNominal;
  for (int c = 0; c < this->nCategories; c++){
    TString cname= (this->categoriesNames[c]).c_str();
    if (c!=0) cname= "_"+cname;
    for(int sy=0;sy<(int)MAX;++sy){
      if(!(!useOnlyNominal || sy==0)) continue;
      //cout << " writing histo "<< histo->GetName()<< " in file "<< filesout[(int)sy]->GetName()<<endl;;
      filesout[(int)sy+(MAX+1)*c]->cd();
      histo->Write();
      //    histo[sy]=new TH1F(name+ns,name+ns,nbins,min,max);
    }
    if(this->addPDF){
      if(!useOnlyNominal){
	filesout[MAX+(MAX+1)*c]->cd();
	int MAXPDF=this->maxSysts;
	for(int sy=MAX;sy<MAXPDF;++sy){
	  //      cout << " writing sy "<< histo[sy]->GetName()<<endl;
	  histo->Write();
	  //      cout << " written sy "<< histo[sy]->GetName()<<endl;
	}
      }
    }
  }
}


void systWeights::setMax(int max){
  this->maxSysts =  max;
}
void systWeights::setMaxNonPDF(int max){
  this->maxSystsNonPDF =  max;
}
void systWeights::setSystValue(string name, double value, bool mult){
  float zerofact=1.0;
  if(mult)zerofact=this->weightedSysts[0];
  int MAX = this->maxSysts;
  for(int sy=0;sy<(int)MAX;++sy){
    if(this->weightedNames[(int)sy] ==name){
      this->weightedSysts[(int)sy] =value*zerofact;
    }
  }
}

void systWeights::setSystValue(int place, double value, bool mult){
  float zerofact=1.0;
  if(mult)zerofact=this->weightedSysts[0];
  this->weightedSysts[place] =value*zerofact;
}

void systWeights::setWeight(string name, double value, bool mult){
  this->setSystValue(name, value, mult);
}

void systWeights::setWeight(int place, double value, bool mult){
  this->setSystValue(place, value, mult);
}
//void systWeights::setSystValue(weightedSysts systPlace, double value);

/*
void createFilesSysts(  TFile * allFiles[maxSysts], TString basename, bool useOnlyNominal=false,TString opt = "RECREATE"){
  for(int sy=0;sy<maxSysts;++sy){
    TString ns= weightedSystsNames((weightedSysts)sy);
    //    "/afs/cern.ch/user/o/oiorio/public/xAnnapaola/Nov10/res/"+sample + "_" +channel+".root";
    if(sy==0){
	allFiles[sy]= TFile::Open((basename+ns+".root"), opt);}
    else{
      allFiles[sy]= TFile::Open((basename+"_"+ns+".root"), opt);}
    //TFile *outTree = TFile::Open(("trees/tree_"+outFileName).c_str(), "RECREATE");
    
  }
  //return allFiles;
}

void systZero.writeHistogramsSysts(TH1F* histo[maxSysts], TFile *filesout[(int)MAXSYSTS], bool useOnlyNominal=false){  
  for(int sy=0;sy<(int)MAXSYSTS;++sy){
    //cout << " writing histo "<< histo[(int)sy]->GetName()<< " in file "<< filesout[(int)sy]->GetName()<<endl;;
    //TString ns= weightedSystsNames((weightedSysts)sy);
    filesout[(int)sy]->cd();
    histo[sy]->Write(histo[0]->GetName());
    //    histo[sy]=new TH1F(name+ns,name+ns,nbins,min,max);
  }
}

void writeSingleHistogramSysts(TH1F* histo, TFile *filesout[(int)MAXSYSTS], bool useOnlyNominal=false){  
  for(int sy=0;sy<(int)MAXSYSTS;++sy){
    cout << " writing histo "<< histo->GetName()<< " in file "<< filesout[(int)sy]->GetName()<<endl;;
    //TString ns= weightedSystsNames((weightedSysts)sy);
    filesout[(int)sy]->cd();
    histo->Write();
    //    histo[sy]=new TH1F(name+ns,name+ns,nbins,min,max);
  }
}
*/
//
//{
//  void createFileSyst();
//  void fillHistogramSyst();
//  void writeHistogramSyst();
//  void setMax(int max);
//  void setSystValue(string name, double value);
//  void setSystValue(weightedSysts systPlace, double value);
//  int maxSysts;
//  int[wLimit] weightedSysts;
//  string[wLimit] weightedNames;
//}


TString  weightedSystsNames (weightedSysts sy){
  switch(sy){
  case NOSYST : return "";
  case BTAGUP : return "btagUp";
  case BTAGDOWN : return "btagDown";
  case MISTAGUP : return "mistagUp";
  case MISTAGDOWN : return "mistagDown";
  case PUUP : return "puUp";
  case PUDOWN : return "puDown";
  case LEPUP : return "lepUp";
  case LEPDOWN : return "lepDown";
    //case ISOUP : return "isoUp";
    //case ISODOWN : return "isoDown";
    //case TRIGUP : return "trigUp";
    //case TRIGDOWN : return "trigDown";
  case MAXSYSTS : return "";
  }
  return "noSyst";
}

void  initHistogramsSysts (TH1F* histo[(int)MAXSYSTS],TString name, TString title, int nbins, float min, float max, bool useOnlyNominal=false){
  for(int sy=0;sy<(int)MAXSYSTS;++sy){
    TString ns= weightedSystsNames((weightedSysts)sy);
    histo[sy]=new TH1F(name+ns,title,nbins,min,max);
  }
}

void fillHistogramsSysts(TH1F* histo[(int)MAXSYSTS], float v, float w, float systWeight[(int)MAXSYSTS] , bool useOnlyNominal=false){
  for(int sy=0;sy<(int)MAXSYSTS;++sy){
    float ws = systWeight[(int)sy];
    //    cout << " filling histogram "<< histo[(int)sy]->GetName() << " with value "<< v <<" and weight "<< w <<" ws "<< ws<<endl;
    histo[sy]->Fill(v, w*ws);
  }
}

void createFilesSysts(  TFile * allFiles[(int)MAXSYSTS], TString basename, bool useOnlyNominal=false,TString opt = "RECREATE"){
  for(int sy=0;sy<(int)MAXSYSTS;++sy){
    TString ns= weightedSystsNames((weightedSysts)sy);
    //    "/afs/cern.ch/user/o/oiorio/public/xAnnapaola/Nov10/res/"+sample + "_" +channel+".root";
    if(sy==0){
	allFiles[sy]= TFile::Open((basename+ns+".root"), opt);}
    else{
      if(!useOnlyNominal) allFiles[sy]= TFile::Open((basename+"_"+ns+".root"), opt);}
    //TFile *outTree = TFile::Open(("trees/tree_"+outFileName).c_str(), "RECREATE");
    
  }
  //return allFiles;
}

void writeHistogramsSysts(TH1F* histo[(int)MAXSYSTS], TFile *filesout[(int)MAXSYSTS], bool useOnlyNominal=false){  
  for(int sy=0;sy<(int)MAXSYSTS;++sy){
    //cout << " writing histo "<< histo[(int)sy]->GetName()<< " in file "<< filesout[(int)sy]->GetName()<<endl;;
    //TString ns= weightedSystsNames((weightedSysts)sy);
    filesout[(int)sy]->cd();
    histo[sy]->Write(histo[0]->GetName());
    //    histo[sy]=new TH1F(name+ns,name+ns,nbins,min,max);
  }
}

void writeSingleHistogramSysts(TH1F* histo, TFile *filesout[(int)MAXSYSTS], bool useOnlyNominal=false){  
  for(int sy=0;sy<(int)MAXSYSTS;++sy){
    cout << " writing histo "<< histo->GetName()<< " in file "<< filesout[(int)sy]->GetName()<<endl;;
    //TString ns= weightedSystsNames((weightedSysts)sy);
    filesout[(int)sy]->cd();
    histo->Write();
    //    histo[sy]=new TH1F(name+ns,name+ns,nbins,min,max);
  }
}

TH1F * makeproduct(TH2F * h){

  //Make temporary histos to rebin
  //  TH1F *hA = (TH1F*)h_A->Clone("hA");
  // TH1F *hB = (TH1F*)h_B->Clone("hB");

  //  hA->Rebin(rebinA);
  // hB->Rebin(rebinB);
  
  //get nbins from new histos
  int nbinsA = h->GetNbinsX();
  int nbinsB = h->GetNbinsY();
  double min = h->GetXaxis()->GetBinLowEdge(1)*h->GetYaxis()->GetBinLowEdge(1);
  double max = h->GetXaxis()->GetBinLowEdge(nbinsA+1)*h->GetYaxis()->GetBinLowEdge(nbinsB+1);
  //Get the actual name from the original histograms 
  string name = (string)(h->GetName()) + "_1D";
  
  //Initialize histogram 
  TH1F * result = new TH1F(name.c_str(),name.c_str(),nbinsA*nbinsB,min,max);
  //Fill histogram
  for(int i =1; i<= nbinsA;++i){
    for(int j =1; j<= nbinsB;++j){
      double value = h->GetBinContent(i,j);
      int k = ((i-1)*nbinsB)+j;
      result->SetBinContent(k,value);
    }
  }
  //  if( integral <= 0.)integral = hA->Integral()/result->Integral();
  //else integral = integral / result->Integral();
  //result->Scale(integral);
  return result;

}
