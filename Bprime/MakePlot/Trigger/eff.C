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

void eff(){
  
  TFile *QCD = new TFile("output/singleh/histos/Data_cat1_singleH.root");

  TH1F *h_QCD_ratio = (TH1F*) QCD->Get("h_Ht_trigaft");
  TH1F *h_QCD_without = (TH1F*) QCD->Get("h_Ht_trigbef");

  h_QCD_ratio->Divide(h_QCD_without);
  h_QCD_ratio->GetXaxis()->SetRangeUser(500,2500);
  TF1 *erf1 = new TF1("erf1", "0.5*1*(1+TMath::Erf( (x-[0]) / ([1]*sqrt(2)) ) )",200.,2000);
  erf1->SetParameter(0,900);
  erf1->SetParameter(1,80);
  //erf1->SetParameter(2,1);
  erf1->SetLineColor(kRed+2);
  h_QCD_ratio->Fit("erf1");
  
  h_QCD_ratio->SetLineColor(kBlack);
  h_QCD_ratio->SetMarkerColor(kBlack);
  h_QCD_ratio->SetMarkerStyle(20);
  h_QCD_ratio->SetMarkerSize(1);
  h_QCD_ratio->SetLineWidth(2);
  
  //canvas booking
  TCanvas *canvas = new TCanvas("canvas", "canvas", 600, 600);
  TPad *pad1 = new TPad("pad1", "pad1", 0.01, 0.01, 1.,1.);
  pad1->SetBottomMargin(0.15);
  pad1->Draw();
  pad1->cd();

  gStyle->SetOptStat(0);
  
  h_QCD_ratio->GetYaxis()->SetTitle("Events (Normalized to unity)");
  h_QCD_ratio->GetYaxis()->SetTitleOffset(1.2);
  h_QCD_ratio->GetXaxis()->SetTitleOffset(1.2);
  h_QCD_ratio->GetXaxis()->SetTitle("H_{T} (GeV)");
  h_QCD_ratio->SetTitle("");
  h_QCD_ratio->SetMinimum(-0.1);
  h_QCD_ratio->SetMaximum(1.1);

  h_QCD_ratio->DrawCopy("ep");

  h_QCD_ratio->GetYaxis()->SetTitle("Events (Normalized to unity)");
  h_QCD_ratio->GetYaxis()->SetTitleOffset(0.8);
  h_QCD_ratio->GetXaxis()->SetTitle("H_{T} (GeV)");
  h_QCD_ratio->SetTitle("");
  h_QCD_ratio->SetMinimum(0.0);
  h_QCD_ratio->SetMaximum(1.1);

  TLegend *leg = new TLegend(0.54,0.25,0.87,0.35); 
  leg->SetTextSize(0.03);
  leg->SetFillColor(0);
  leg->SetBorderSize(0);
  leg->AddEntry(h_QCD_ratio, "PFHT900 efficiency" ,"ep");
  leg->AddEntry(erf1, "ERF fit", "l");
  leg->Draw();

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
  latex.DrawLatex(l+0.14, 1-t+lumiTextOffset*t, extraText);

  TPaveText *cat = new TPaveText(.6,.18,.8,.23, "brNDC");
  cat->AddText("at least 1 forward jet region");
  cat->SetBorderSize(0); 
  cat->Draw();

  canvas->cd();
  
  h_QCD_ratio->GetXaxis()->SetTitle("H_{T} (GeV)");
  h_QCD_ratio->SetTitle("");
  h_QCD_ratio->SetMinimum(0.0);
  h_QCD_ratio->SetMaximum(1.1);

  canvas->Print("Plots/PFHT800_eff_cat1.png");
  canvas->Print("Plots/PFHT800_eff_cat1.pdf");
  
}
