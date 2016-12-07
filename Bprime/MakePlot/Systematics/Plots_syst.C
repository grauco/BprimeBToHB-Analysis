/**********************************************************************************************
                                       Giorgia Rauco 
                                        06.11.2015

1) define the paths of the histos files in #define PATH
2) create a folder called Plots_syst, where the output plots will be saved
3) the macro has 3 options:
          i) channel: car1
         ii) syst: jes, mistag, btag
        iii) histo: h_Ht, h_bprimemass_SR
4) to run the macro (example):  root -l -b -q 'plots_syst.C("cat1", "jes", "h_Ht")'
5) plot.csh, script already including all the combination
***************************************************************** *******************************/

#include "TStyle.h"
#include "TColor.h"
#include "TLorentzVector.h"
#include "TClonesArray.h"
#include "TTree.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TF1.h"
#include "TAxis.h"
#include "TString.h"
#include "THStack.h"
#include "TPaveText.h"

#include <math.h>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string.h>

//define path for taking histos
#define PATH "output/singleh/histos/"

void Plots_syst(const char* channel, const char* syst, const char* histo){
  
  //open TFiles for all background components and for noSyst, SystUp and SystDown
  TFile *QCD_nom = new TFile((PATH+string("QCD_v2_")+channel+".root").c_str());
  TFile *TT_nom = new TFile((PATH+string("TT_v2_")+channel+".root").c_str());

  TFile *QCD_up = new TFile((PATH+string("QCD_v2_")+channel+"_"+syst+"Up.root").c_str());
  TFile *TT_up = new TFile((PATH+string("TT_v2_")+channel+"_"+syst+"Up.root").c_str());

  TFile *QCD_down = new TFile((PATH+string("QCD_v2_")+channel+"_"+syst+"Down.root").c_str());
  TFile *TT_down = new TFile((PATH+string("TT_v2_")+channel+"_"+syst+"Down.root").c_str());
      
  //get histograms
  TH1F *h_QCD_nom = (TH1F*) QCD_nom->Get(histo);
  TH1F *h_TT_nom = (TH1F*) TT_nom->Get(histo);

  TH1F *h_QCD_up = (TH1F*) QCD_up->Get(histo);
  TH1F *h_TT_up = (TH1F*) TT_up->Get(histo);

  TH1F *h_QCD_down = (TH1F*) QCD_down->Get(histo);
  TH1F *h_TT_down = (TH1F*) TT_down->Get(histo);

  //h_QCD_up->Rebin(2);
  //h_QCD_down->Rebin(2);
  //h_QCD_nom->Rebin(2);
  //h_TT_up->Rebin(2);
  //h_TT_down->Rebin(2);
  //h_TT_nom->Rebin(2);

  h_QCD_up->SetLineColor(kBlue);
  h_QCD_up->SetMarkerColor(kBlue);
  h_QCD_up->SetMarkerStyle(20);
  h_QCD_up->SetMarkerSize(2);
  h_QCD_up->SetLineWidth(2);
  h_QCD_down->SetLineColor(kRed);
  h_QCD_down->SetMarkerColor(kRed);
  h_QCD_down->SetMarkerStyle(21);
  h_QCD_down->SetLineWidth(2);
  h_QCD_nom->SetLineColor(kGray);
  h_QCD_nom->SetFillColor(kGray);
  
  //canvas booking
  TCanvas *canvas = new TCanvas("canvas", "canvas", 900, 700);
  TPad *pad1 = new TPad("pad1", "pad1", 0., 0.3, 1.,1.);
  pad1->SetBottomMargin(0.);
  pad1->Draw();
  pad1->cd();
  
  gPad->SetLogy();
  gStyle->SetOptStat(0);
  
  h_QCD_nom->Add(h_TT_nom);
  h_QCD_up->Add(h_TT_up);
  h_QCD_down->Add(h_TT_down);

  h_QCD_nom->Draw("histo");
  h_QCD_up->DrawCopy("ep same");
  h_QCD_down->DrawCopy("ep same");

  h_QCD_nom->GetYaxis()->SetTitle("Events");
  h_QCD_nom->GetYaxis()->SetTitleOffset(0.8);
  h_QCD_nom->GetXaxis()->SetTitle("H_{T} (GeV)");
  h_QCD_nom->SetTitle("");
  h_QCD_nom->SetMaximum(100*h_QCD_up->GetMaximum());
  h_QCD_nom->SetMinimum(1.1);

  leg = new TLegend(0.64,0.65,0.87,0.85); 
  leg->SetTextSize(0.04);
  leg->SetFillColor(0);
  leg->SetBorderSize(0);
  leg->AddEntry(h_QCD_nom,"NOM","f");
  leg->AddEntry(h_QCD_up,(syst + string("up")).c_str(),"ep");
  leg->AddEntry(h_QCD_down,(syst + string("down")).c_str(),"ep");
  leg->Draw();

  pt = new TPaveText(0.2,0.91,0.3,0.98, "NDC");
  pt->SetFillColor(0);
  pt->SetBorderSize(0);
  pt->SetTextSize(0.04); 
  TString txt = "channel & region";
  if(strcmp(histo, "h_bprimemass_SR")==0){
    txt=((string("signal region")).c_str());
  }
  text = pt->AddText(txt);
  pt->Draw();

  //summary
  cout << "***********************************************" << endl;
  cout << "systdown:" << h_QCD_down->Integral() << endl;
  cout << "systup:" << h_QCD_up->Integral() << endl;
  cout << "nom:" << h_QCD_nom->Integral() << endl;
  cout << "==> normalization efficiency:" << (h_QCD_up->Integral()-h_QCD_down->Integral())/(2*h_QCD_nom->Integral()) * 100<< endl;
  cout << "***********************************************" << endl;
 
  canvas->cd();

  //ratio pad booking
  TPad *pad2 = new TPad("pad2","pad2",0,0.,1,0.3);
  pad2->SetBottomMargin(0.4);
  pad2->SetTopMargin(0.);
  pad2->SetGridy();
  pad2->Draw();
  pad2->cd();
  
  h_QCD_up->SetStats(0);
  h_QCD_up->Add(h_QCD_nom,-1);
  h_QCD_up->Divide((h_QCD_nom));
  h_QCD_up->SetLineColor(kBlue);
  h_QCD_up->SetFillStyle(3003);
  h_QCD_up->GetYaxis()->SetTitle("(shifted-nom)/nom");
  h_QCD_up->GetYaxis()->SetTitleOffset(0.8);
  h_QCD_up->GetXaxis()->SetTitle("B' mass (GeV)");
  h_QCD_up->SetTitle("");
  
  h_QCD_up->Draw("histo");
  
  h_QCD_down->SetStats(0);
  h_QCD_down->Add(h_QCD_nom,-1);
  h_QCD_down->Divide((h_QCD_nom));
  h_QCD_down->SetLineColor(kRed);                                                                                            
  h_QCD_down->SetFillStyle(3003);
  h_QCD_down->Draw("histo same");

  //h_QCD_up->SetMaximum(1.7*(std::max(h_QCD_up->GetMaximum(), abs(h_QCD_up->GetMinimum()))));
  //h_QCD_up->SetMinimum(-1.7*(std::max(h_QCD_up->GetMaximum(), abs(h_QCD_up->GetMinimum()))));
  h_QCD_up->SetMaximum(1);
  h_QCD_up->SetMinimum(-1);

  canvas->cd();
  
  canvas->Print((string("Plots_syst/")+syst+string("_")+histo+string("_")+channel+string(".png")).c_str());
  canvas->Print((string("Plots_syst/")+syst+string("_")+histo+string("_")+channel+string(".pdf")).c_str());
  
}
