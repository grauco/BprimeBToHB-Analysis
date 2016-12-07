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

void ShapeComparison_Data(){
  TFile *qcd = new TFile("output/singleh/histos/QCD_v2_cat1_singleH.root");
  TFile *tt = new TFile("output/singleh/histos/TT_v2_cat1_singleH.root");
  TFile *data = new TFile("output/singleh/histos/Data_cat1_singleH.root");

  TH1F *h_bprimemass_a = (TH1F*)qcd->Get("h_Ht");   
  TH1F *h_bprimemass_att = (TH1F*)tt->Get("h_Ht");
  h_bprimemass_a->Add(h_bprimemass_att);
  TH1F *h_bprimemass_c = (TH1F*)data->Get("h_Ht_CRC");
  TH1F *h_bprimemass_d = (TH1F*)data->Get("h_Ht_CRD");
  TH1F *h_bprimemass_b = (TH1F*)data->Get("h_Ht_CRB");
  
  TH1F *htt_ht = (TH1F*)tt->Get("h_Ht_CRC");

  h_bprimemass_a->Sumw2();
  h_bprimemass_c->Sumw2();
  h_bprimemass_b->Sumw2();
  h_bprimemass_d->Sumw2();
  htt_ht->Sumw2();

  TCanvas *c1 = new TCanvas("c1","",600,600);
  TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
  pad1->SetBottomMargin(0); // Upper and lower plot are joined
  pad1->SetGridx();         // Vertical grid
  pad1->Draw();             // Draw the upper pad: pad1
  pad1->cd();               // pad1 becomes the current pad
  gStyle->SetOptStat(0);
   
  h_bprimemass_c->Add(htt_ht,-1);
  h_bprimemass_c->Scale(1/h_bprimemass_c->Integral());
  h_bprimemass_a->Scale(1/h_bprimemass_a->Integral());
  
  h_bprimemass_c->GetXaxis()->SetRangeUser(400,2400);                                                                                                 
  h_bprimemass_a->GetXaxis()->SetRangeUser(400,2400); 
  
  h_bprimemass_a->SetMaximum(10*h_bprimemass_a->GetMaximum());
  h_bprimemass_c->SetMaximum(10*h_bprimemass_c->GetMaximum());

  h_bprimemass_c->SetTitle("");
  h_bprimemass_c->GetYaxis()->SetTitle("Events/200.00");
  h_bprimemass_c->GetYaxis()->SetTitleOffset(0.8);
  h_bprimemass_c->SetFillColor(kWhite);
  h_bprimemass_c->SetLineColor(kMagenta-10);
  h_bprimemass_c->SetMarkerColor(kMagenta-10);
  h_bprimemass_c->SetLineWidth(2);
  h_bprimemass_c->Draw("histo");
  //  gStyle->SetErrorX(0); 
  h_bprimemass_a->SetLineColor(kMagenta-4);
  h_bprimemass_a->SetFillColor(kWhite);
  h_bprimemass_a->SetLineWidth(2);
  h_bprimemass_a->Draw("ep same");
  h_bprimemass_a->SetMarkerSize(1);
  h_bprimemass_a->SetMarkerColor(kMagenta-4);
  //h_bprimemass_c->GetXaxis()->SetTitle("H_{T} [GeV]");

  h_bprimemass_a->SetMinimum(0.0001);
  h_bprimemass_c->SetMinimum(0.0003);
  
  TLatex* text=new TLatex(0.6, 0.85, Form("KS = %.3f", h_bprimemass_a->KolmogorovTest(h_bprimemass_c, "UO")));
  // TLatex* text=new TLatex(0.6, 0.85, Form("KS = %.3f,   #chi^{2}/ndf = %.2f/%d", h_bprimemass_a->KolmogorovTest(h_bprimemass_c, "UO"), h_bprimemass_a->Chi2Test(h_bprimemass_a, "WW, CHI2"), h_bprimemass_a->GetNbinsX()-2));
  text->SetNDC(); text->SetTextSize(0.03);
  //text->Draw();
  
  leg = new TLegend(0.45,0.65,0.87,0.85); 
  leg->SetTextSize(0.03);
  leg->SetFillColor(0);
  leg->SetBorderSize(0);
  leg->AddEntry(h_bprimemass_c,"expected QCD from Data in reg C","l");
  leg->AddEntry(h_bprimemass_a,"QCD MC","ep");
  leg->Draw();


  pt = new TPaveText(0.65,0.60,0.85,0.65, "NDC");
  pt->SetFillColor(0);
  pt->SetBorderSize(0);
  pt->SetTextSize(0.03); 
 
  gPad->SetLogy();

  c1->cd();          // Go back to the main canvas before defining pad2
  TPad *pad2 = new TPad("pad2", "pad2", 0, 0.05, 1, 0.3);
  pad2->SetTopMargin(0);
  pad2->SetBottomMargin(0.2);
  pad2->SetGridx(); // vertical grid
  pad2->Draw();
  pad2->cd();

   
 TH1F *h3 = (TH1F*)h_bprimemass_c->Clone("h3");
 h3->SetLineColor(kBlack);
 h3->SetMinimum(-0.10);  // Define Y ..
 h3->SetMaximum(2.10); // .. range
 h3->Sumw2();
 h3->SetStats(0);      // No statistics on lower plot
 h3->Divide(h_bprimemass_a);
 h3->SetMarkerStyle(20);
 h3->SetMarkerColor(kBlack);
 h3->Draw("ep");       // Draw the ratio plot

 
 // h_bprimemass_c settings
 h_bprimemass_c->SetLineColor(kMagenta-10);
 h_bprimemass_c->SetLineWidth(2);
 
 // Y axis h_bprimemass_c plot settings
 h_bprimemass_c->GetYaxis()->SetTitleSize(20);
 h_bprimemass_c->GetYaxis()->SetTitleFont(43);
 h_bprimemass_c->GetYaxis()->SetTitleOffset(1.55);
 
 // h_bprimemass_a settings
 h_bprimemass_a->SetLineColor(kMagenta-4);
 h_bprimemass_a->SetLineWidth(2);
 
 // Ratio plot (h3) settings
 h3->SetTitle(""); // Remove the ratio title
 
 // Y axis ratio plot settings
 h3->GetYaxis()->SetTitle("Data/MC");
 h3->GetXaxis()->SetTitle("H_{T} (GeV)");
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
 
 
 c1->Print("Plots_bkg/BkgEst_shape_AB_Data_withTT_cat1.pdf");
 
}
