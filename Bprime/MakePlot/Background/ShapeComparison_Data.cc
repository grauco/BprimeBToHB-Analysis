#include "TH1.h" 
#include "TMath.h"
#include "TF1.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TFile.h"
#include "TStyle.h"
#include "TDirectory.h"
#include "TString.h"
#include "TAxis.h"

void ShapeComparison_Data(bool fromData, TString cat){

  //  TString cat[2] = {"cat0", "cat1"};
  //cout << "1"<< endl;
  //  for(int j=0; j<2; j++){
  TH1::SetDefaultSumw2 (kTRUE);
  //cout << "1"<< endl;
     
  TFile *qcd = new TFile("output/singleh/histos/QCD_v2_"+cat+"_singleH.root");
  TFile *tt = new TFile("output/singleh/histos/TT_v2_"+cat+"_singleH.root");
  TFile *data = new TFile("output/singleh/histos/Data_"+cat+"_singleH.root");

  TH1F *h_a = (TH1F*)qcd->Get("h_bprimemass_SR");   
  TH1F *h_att = (TH1F*)tt->Get("h_bprimemass_SR");
  TH1F *h_c = (TH1F*)qcd->Get("h_bprimemass_CRC");
  TH1F *h_ctt = (TH1F*)tt->Get("h_bprimemass_CRC");

  if(fromData==1){
    TH1F *h_c = (TH1F*)data->Get("h_bprimemass_CRC");
    TH1F *h_ctt = (TH1F*)data->Get("h_bprimemass_CRC");
  }

  h_a->Sumw2();
  h_c->Sumw2();

  TCanvas *c1 = new TCanvas("c1","",600,600);
  TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
  pad1->SetBottomMargin(0); // Upper and lower plot are joined
  pad1->Draw();             // Draw the upper pad: pad1
  pad1->cd();               // pad1 becomes the current pad
  gStyle->SetOptStat(0);

  h_c->Add(h_ctt);

  h_c->Scale(1/h_c->Integral()); //normalize the summed estimated bkgs in reg C
  h_a->Scale(1/h_a->Integral()); // normalize real bkg in reg A
  h_att->Scale(1/h_att->Integral()); // normalize real bkg in reg A

  TH1F *hbkg = (TH1F*)h_a->Clone("hbkg");
  hbkg->Add(h_att); // sum and save the true bkgs (qcd + tt)
  hbkg->Scale(1/hbkg->Integral()); // normalized the summed bkgs

  h_c->GetXaxis()->SetRangeUser(600,2000);                                                                                                 
  h_a->GetXaxis()->SetRangeUser(600,2000); 
  h_att->GetXaxis()->SetRangeUser(600,2000);

  h_a->SetMaximum(10*h_a->GetMaximum());
  h_c->SetMaximum(10*h_c->GetMaximum());

  h_c->SetTitle("");
  h_c->GetYaxis()->SetTitle("Events/200.00");
  h_c->GetYaxis()->SetTitleOffset(0.8);
  h_c->SetFillColor(kWhite);
  h_c->SetLineColor(kRed+2);
  h_c->SetMarkerColor(kRed+2);
  h_c->SetLineWidth(3);
  h_c->DrawCopy("histo");
  
  h_c->SetFillColor(kRed+2);
  h_c->SetFillStyle(3004);
  h_c->SetMarkerSize(0);
  h_c->Draw("e2same");
 
  h_a->SetLineColor(kRed-8);
  h_a->SetFillColor(kWhite);
  h_a->SetMarkerStyle(26);
  h_a->SetLineWidth(2);
  h_a->Draw("ep same");
  h_a->SetMarkerSize(1);
  h_a->SetMarkerColor(kRed-8);
  //h_c->GetXaxis()->SetTitle("H_{T} [GeV]");

  //  h_atttt
  h_att->SetLineColor(kRed-4);
  h_att->SetFillColor(kWhite);
  h_att->SetMarkerStyle(25);
  h_att->SetLineWidth(2);
  h_att->Draw("ep same");
  h_att->SetMarkerSize(1);
  h_att->SetMarkerColor(kRed-4);
 
  hbkg->SetLineColor(kBlack);
  hbkg->SetFillColor(kWhite);
  hbkg->SetLineWidth(2);
  hbkg->Draw("ep same");
  hbkg->SetMarkerSize(1);
  hbkg->SetMarkerColor(kBlack);

  h_att->SetMinimum(0.0001);
  h_c->SetMinimum(0.0003);
  
  leg = new TLegend(0.45,0.65,0.87,0.85); 
  leg->SetTextSize(0.03);
  leg->SetFillColor(0);
  leg->SetBorderSize(0);
  leg->AddEntry(h_c,"expected QCD+TT from in reg C","l");
  leg->AddEntry(h_a,"QCD MC in reg A","ep");
  leg->AddEntry(h_att,"TT MC in reg A", "ep");
  leg->AddEntry(hbkg, "QCD+TT MC in reg A", "ep");
  leg->Draw();


  pt = new TPaveText(0.65,0.60,0.85,0.65, "NDC");
  pt->SetFillColor(0);
  pt->SetBorderSize(0);
  pt->SetTextSize(0.03); 
 
  gPad->SetLogy();
 
  TPaveText *categ = new TPaveText(.15,.07,.35,.15, "brNDC");
  if(strcmp(cat,"cat1")==0) categ->AddText("at least 1 forward jet category");
  if(strcmp(cat,"cat0")==0) categ->AddText("0 forward jets category");
  categ->SetBorderSize(0);
  categ->Draw();

  TPaveText *type = new TPaveText(.15,.15,.35,.2, "brNDC");
  if(fromData==0){
    type->AddText("MC");
  }else {
    type ->AddText("Data");
  }
  type->SetBorderSize(0);
  type->Draw();

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


  c1->cd();          // Go back to the main canvas before defining pad2

  TPad *pad2 = new TPad("pad2", "pad2", 0, 0.03, 1, 0.25);
  pad2->SetTopMargin(0);
  pad2->SetBottomMargin(0.2);
  pad2->SetGridx(); // vertical grid
  pad2->Draw();
  pad2->cd();

   
 TH1F *h3 = (TH1F*)h_c->Clone("h3");
 h3->SetLineColor(kBlack);
 h3->SetMinimum(-0.10);  // Define Y ..
 h3->SetMaximum(2.10); // .. range
 h3->Sumw2();
 h3->SetStats(0);      // No statistics on lower plot
 //h_a->Add(h_att);
 h3->Divide(hbkg);
 h3->SetMarkerSize(1);
 h3->SetLineWidth(1);
 h3->SetMarkerStyle(20);
 h3->SetMarkerColor(kBlack);
 h3->Draw("ex0");       // Draw the ratio plot
 
 // h_c settings
 h_c->SetLineColor(kRed+2);
 h_c->SetLineWidth(2);
 
 // Y axis h_c plot settings
 h_c->GetYaxis()->SetTitleSize(20);
 h_c->GetYaxis()->SetTitleFont(43);
 h_c->GetYaxis()->SetTitleOffset(1.55);
 
 // h_a settings
 h_a->SetLineColor(kRed-8);
 h_a->SetLineWidth(2);
 
 // Ratio plot (h3) settings
 h3->SetTitle(""); // Remove the ratio title
 
 // Y axis ratio plot settings
 h3->GetYaxis()->SetTitle("Data/MC");
 h3->GetXaxis()->SetTitle("m(B') (GeV)");
 h3->GetYaxis()->SetNdivisions(505);
 h3->GetYaxis()->SetTitleSize(20);
 h3->GetYaxis()->SetTitleFont(43);
 h3->GetYaxis()->SetTitleOffset(1.2);
 h3->GetYaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
 h3->GetYaxis()->SetLabelSize(15);
 
 // X axis ratio plot settings
 h3->GetXaxis()->SetTitleSize(17);
 h3->GetXaxis()->SetTitleFont(43);
 h3->GetXaxis()->SetTitleOffset(3.5);
 h3->GetXaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
 h3->GetXaxis()->SetLabelSize(15);

 c1->Update();

 TLine *line = new TLine(600,1.0,2000,1.0);
 line->SetLineColor(kBlack);
 line->Draw();

 c1->Print("Plots_bkg/BkgEst_shape_Data_"+cat+".pdf");
 
 //}
}
