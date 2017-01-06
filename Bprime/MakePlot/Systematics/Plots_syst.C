/**********************************************************************************************
                                       Giorgia Rauco 
                                        06.11.2015

1) define the paths of the histos files in #define PATH
2) create a folder called Plots_syst, where the output plots will be saved
3) the macro has 3 options:
          i) channel: semileptonic, fullhadronic
         ii) syst: jes, mistag, btag
        iii) histo: metFinal, metFinal_outtop, metFinal_2lep, metFinal_met_0btag, metFinal_SR_1lep
4) to run the macro (example):  root -l -b -q 'ttDM_syst.C("semileptonic", "jes", "metFinal")'
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
  gROOT->ForceStyle();
    
  //open TFiles for all background components and for noSyst, SystUp and SystDown
  TFile *QCD_nom = new TFile((PATH+string("QCDTT_v2_")+channel+".root").c_str());
  //TFile *TT_nom = new TFile((PATH+string("TT_v2_")+channel+".root").c_str());

  TFile *QCD_up = new TFile((PATH+string("QCDTT_v2_")+channel+"_"+syst+"Up.root").c_str());
  //TFile *TT_up = new TFile((PATH+string("TT_v2_")+channel+"_"+syst+"Up.root").c_str());

  TFile *QCD_down = new TFile((PATH+string("QCDTT_v2_")+channel+"_"+syst+"Down.root").c_str());
  //TFile *TT_down = new TFile((PATH+string("TT_v2_")+channel+"_"+syst+"Down.root").c_str());
      
  //get histograms
  TH1F *h_QCD_nom = (TH1F*) QCD_nom->Get(histo);
  //  TH1F *h_TT_nom = (TH1F*) TT_nom->Get(histo);
  TH1F *h_QCD_up = (TH1F*) QCD_up->Get(histo);
  //TH1F *h_TT_up = (TH1F*) TT_up->Get(histo);
  TH1F *h_QCD_down = (TH1F*) QCD_down->Get(histo);
  //TH1F *h_TT_down = (TH1F*) TT_down->Get(histo);
  
  //canvas booking
  TCanvas *canvas = new TCanvas("canvas", "canvas", 600, 600);
  TPad *pad1 = new TPad("pad1", "pad1", 0., 0.3, 1.,1.);
  pad1->SetBottomMargin(0.);
  pad1->Draw();
  pad1->cd();
  gPad->SetLogy();
  gStyle->SetOptStat(0);
  
  //  h_QCD_nom->Add(h_TT_nom);
  //h_QCD_up->Add(h_TT_up);
  //h_QCD_down->Add(h_TT_down);

  h_QCD_nom->GetYaxis()->SetTitle("Events");
  h_QCD_nom->GetYaxis()->SetTitleOffset(0.8);
  h_QCD_nom->GetXaxis()->SetTitle("m(B') (GeV)");
  h_QCD_nom->SetTitle("");
  h_QCD_nom->SetMaximum(100*h_QCD_up->GetMaximum());
  h_QCD_nom->SetMinimum(2);

  h_QCD_nom->SetFillColor(kGray);
  h_QCD_nom->SetLineColor(kGray);
  h_QCD_up->SetLineColor(kRed);

 h_QCD_down->SetLineColor(kBlue);
  h_QCD_up->SetMarkerColor(kRed);
  h_QCD_down->SetMarkerColor(kBlue);
  h_QCD_up->SetMarkerStyle(20);
  h_QCD_down->SetMarkerStyle(24);
  h_QCD_up->SetMarkerSize(1);
  h_QCD_down->SetMarkerSize(1);
  
  h_QCD_nom->Draw("histo");
  h_QCD_up->DrawCopy("ex0 same");
  h_QCD_down->DrawCopy("ex0 same");

  leg = new TLegend(0.64,0.65,0.87,0.85); 
  leg->SetTextSize(0.04);
  leg->SetFillColor(0);
  leg->SetBorderSize(0);
  leg->AddEntry(h_QCD_nom,"NOM","f");
  leg->AddEntry(h_QCD_up,(syst + string("up")).c_str(),"ep");
  leg->AddEntry(h_QCD_down,(syst + string("down")).c_str(),"ep");
  leg->Draw();

  TPaveText *cat = new TPaveText(.15,.67,.35,.75, "brNDC");
  cat->AddText("0 forward jets category");
  cat->SetBorderSize(0);
  cat->Draw();

  TPaveText *type = new TPaveText(.15,.75,.35,.8, "brNDC");
  type->AddText("Data");
  type->SetBorderSize(0);
  //type->Draw();

  pt = new TPaveText(0.2,0.91,0.3,0.98, "NDC");
  pt->SetFillColor(0);
  pt->SetBorderSize(0);
  pt->SetTextSize(0.04); 
  TString txt = "channel & region";
  if(strcmp(histo, "h_bprimemass_SR")==0){
    txt=((string("signal region")).c_str());
  }
  text = pt->AddText(txt);
  // pt->Draw();

  //summary
  /* cout << "***********************************************" << endl;
  cout << "systdown:" << h_QCD_down->Integral() << endl;
  cout << "systup:" << h_QCD_up->Integral() << endl;
  cout << "nom:" << h_QCD_nom->Integral() << endl;
  cout << "==> normalization efficiency:" << (h_QCD_up->Integral()-h_QCD_down->Integral())/(2*h_QCD_nom->Integral()) * 100<< endl;*/
  cout << "***********************************************" << endl;


    TString cmsText     = "CMS";
  float cmsTextFont   = 61;  // default is helvetic-bold                                                                                                                                                                     
  bool writeExtraText = true;
  TString extraText   = "Preliminary";
  float extraTextFont = 52;  // default is helvetica-italics                                                                                                                                                                 
  // text sizes and text offsets with respect to the top frame in unit of the top margin size                                                                                                                                
  float lumiTextSize     = 0.6;
  float lumiTextOffset   = 0.2;
  float cmsTextSize      = 0.45;
  float cmsTextOffset    = 0.1;  // only used in outOfFrame version                                                                                                                                                          
  float relPosX    = 0.045;
  float relPosY    = 0.035;
  float relExtraQCD_pythia = 1.2;
  // ratio of "CMS" and extra text size                                                                                                                                                                                      
  float extraOverCmsTextSize  = 0.76;
  TString lumi_13TeV = "27.2 fb^{-1} (13 TeV)";
  TString lumi_8TeV  = "19.7 fb^{-1}";
  TString lumi_7TeV  = "5.1 fb^{-1}";
  TString lumiText;
  lumiText = "#sqrt{s} = 13 TeV ";
  float t = pad1->GetTopMargin();
  float b = pad1->GetBottomMargin();
  float r = pad1->GetRightMargin();
  float l = pad1->GetLeftMargin();
  TLatex latex;
  latex.SetNDC();
  latex.SetTextAngle(0);
  latex.SetTextColor(kBlack);
  float extraTextSize = extraOverCmsTextSize*cmsTextSize;
  latex.SetTextFont(42);
  latex.SetTextAlign(31);
  latex.SetTextSize(lumiTextSize*t*1.0);
  latex.DrawLatex(1-r,1-t+lumiTextOffset*t,lumi_13TeV);
  latex.SetTextFont(cmsTextFont);
  latex.SetTextAlign(11);
  latex.SetTextSize(cmsTextSize*t);
  latex.DrawLatex(l+0.03, 1-t+lumiTextOffset*t,cmsText);
  latex.SetTextFont(extraTextFont);
  latex.SetTextSize(extraTextSize*t);
  latex.DrawLatex(l+0.1, 1-t+lumiTextOffset*t, extraText);
  
  canvas->cd();

  //ratio pad booking
  TPad *pad2 = new TPad("pad2","pad2",0,0.03,1,0.25);
  pad2->SetBottomMargin(0.4);
  pad2->SetTopMargin(0.);
  // pad2->SetGridy();
  pad2->Draw();
  pad2->cd();
  
  h_QCD_up->SetStats(0);
  h_QCD_up->Add(h_QCD_nom,-1);
  h_QCD_up->Divide((h_QCD_nom));
  h_QCD_down->SetLineColor(kBlue);
  h_QCD_up->SetFillStyle(3003);
  h_QCD_up->GetYaxis()->SetNdivisions(505);
  h_QCD_up->GetYaxis()->SetTitle("#frac{shifted-nom}{nom}");
  h_QCD_up->GetYaxis()->SetTitleOffset(0.4);
  h_QCD_up->GetYaxis()->SetTitleSize(0.1);
  h_QCD_up->GetYaxis()->SetLabelSize(0.08);
  h_QCD_up->GetXaxis()->SetLabelSize(0.1);
  h_QCD_up->GetXaxis()->SetTitleOffset(0.9);
  h_QCD_up->GetXaxis()->SetTitleSize(0.15);
  h_QCD_up->GetXaxis()->SetTitle("m(B') (GeV)");
  h_QCD_up->SetTitle("");
  
  h_QCD_up->Draw("ex0");
  
  h_QCD_down->SetStats(0);
  h_QCD_down->Add(h_QCD_nom,-1);
  h_QCD_down->Divide((h_QCD_nom));
  h_QCD_up->SetLineColor(kRed);                                                                                            
  h_QCD_down->SetFillStyle(3003);
  h_QCD_down->Draw("ex0 same");

  h_QCD_up->SetMaximum(1.1);
  h_QCD_up->SetMinimum(-1.1);

  canvas->Update();
  
  TLine *line = new TLine(0,0.0,2000,0.0);
  line->SetLineColor(kBlack);
  line->Draw();
  
  canvas->cd();
  
  // canvas->Print((string("Plots_syst/")+syst+string("_")+histo+string("_")+channel+string(".png")).c_str());
  canvas->Print((string("Plots_syst/")+syst+string("_")+histo+string("_")+channel+string(".pdf")).c_str());
  
}
