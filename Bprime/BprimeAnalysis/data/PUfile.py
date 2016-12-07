import ROOT
import tdrstyle, CMS_lumi
#from services_sign import Histo, Stack, Legend, deltaPhi, Histo1

ROOT.gROOT.Reset();
ROOT.gROOT.SetStyle('Plain')
ROOT.gStyle.SetPalette(1)
ROOT.gStyle.SetOptStat(0)
ROOT.gROOT.SetBatch()        # don't pop up canvases                                                                         
ROOT.TH1.SetDefaultSumw2()
ROOT.TH1.AddDirectory(False)

tdrstyle.setTDRStyle();

file_mc = ROOT.TFile("puMC.root", "RECREATE")

npu2015_25ns = [
    0.000829312873542,
    0.00124276120498,
    0.00339329181587,
    0.00408224735376,
    0.00383036590008,
    0.00659159288946,
    0.00816022734493,
    0.00943640833116,
    0.0137777376066,
    0.017059392038,
    0.0213193035468,
    0.0247343174676,
    0.0280848773878,
    0.0323308476564,
    0.0370394341409,
    0.0456917721191,
    0.0558762890594,
    0.0576956187107,
    0.0625325287017,
    0.0591603758776,
    0.0656650815128,
    0.0678329011676,
    0.0625142146389,
    0.0548068448797,
    0.0503893295063,
    0.040209818868,
    0.0374446988111,
    0.0299661572042,
    0.0272024759921,
    0.0219328403791,
    0.0179586571619,
    0.0142926728247,
    0.00839941654725,
    0.00522366397213,
    0.00224457976761,
    0.000779274977993,
    0.000197066585944,
    7.16031761328e-05,
    0.0,
    0.0,
    0.0,
    0.0,
    0.0,
    0.0,
    0.0,
    0.0,
    0.0,
    0.0,
    0.0,
    0.0]

MC_pu = ROOT.TH1F("MC_pu","MC_pu",50,0,50)

for i in range(0,50):
    #print " pu ", i , "  ", npu2015_25ns[i]
    MC_pu.Fill(i, npu2015_25ns[i])
    #if(i==52): print "i ", i, " value ", npu2015_25ns[i-1]

MC_pu.Write()
file_mc.Close()


infile = ROOT.TFile.Open("MyDataPileupHistogram.root")
htmp = infile.Get("pileup")

infile2 = ROOT.TFile.Open("MyDataPileupHistogramDOWN.root")
htmp2 = infile2.Get("pileup")

infile3 = ROOT.TFile.Open("MyDataPileupHistogramUP.root")
htmp3 = infile3.Get("pileup")

htmp.Scale(1/htmp.Integral())
htmp.SetLineColor(ROOT.kRed)
htmp2.Scale(1/htmp2.Integral())
htmp2.SetLineColor(ROOT.kRed)
htmp2.SetLineStyle(2)
htmp3.Scale(1/htmp3.Integral())
htmp3.SetLineColor(ROOT.kRed)
htmp3.SetLineStyle(3)

MC_pu.SetLineColor(ROOT.kBlue)

htmp.GetXaxis().SetRangeUser(0,50)
htmp2.GetXaxis().SetRangeUser(0,50)
htmp3.GetXaxis().SetRangeUser(0,50)
MC_pu.GetXaxis().SetRangeUser(0,50)

htmp.SetLineWidth(3)
htmp2.SetLineWidth(3)
htmp3.SetLineWidth(3)
MC_pu.SetLineWidth(3)

H=600
W=700

H_ref = 600
W_ref = 700

T = 0.08*H_ref
B = 0.12*H_ref
L = 0.12*W_ref
R = 0.08*W_ref

tdrstyle.setTDRStyle();

c1 = ROOT.TCanvas("c1","c1",50,50,W,H)
c1.SetFillColor(0);
c1.SetBorderMode(0);
c1.SetFrameFillStyle(0);
c1.SetFrameBorderMode(0);
c1.SetLeftMargin( 0.15 );#L/W                                                                                                                                     
c1.SetRightMargin( R/W );
c1.SetTopMargin( T/H );
c1.SetBottomMargin(0.14);
c1.SetTickx(0);
c1.SetTicky(0);
c1.Draw()

leg_sign = ROOT.TLegend(.68, .58, 0.91, .89)
#leg_sign.SetNColumns(2)
leg_sign.SetFillColor(0)
leg_sign.SetTextSize(0.032)
leg_sign.SetTextFont(42)

leg_sign.AddEntry(htmp, "#splitline{2016 data}{#sigma = 69.20 mb}", "l")
leg_sign.AddEntry(htmp2, "#splitline{2016 data}{#sigma =  65.74 mb}", "l")
leg_sign.AddEntry(htmp3, "#splitline{2016 data}{#sigma = 72.66  mb}", "l")
leg_sign.AddEntry(MC_pu, "2016 MC", "l")

maximum = max([htmp.GetMaximum(),htmp2.GetMaximum(), htmp3.GetMaximum()])
minimum = min([htmp.GetMinimum(), htmp2.GetMinimum(), htmp3.GetMinimum()])

htmp.SetMaximum(maximum*1.3)
if(minimum > 0):htmp.SetMinimum(0.01*minimum)
else: htmp.SetMinimum(0.01)

htmp.GetXaxis().SetLabelFont(42);
htmp.GetYaxis().SetLabelFont(42);
htmp.GetXaxis().SetTitleFont(42);
htmp.GetYaxis().SetTitleFont(42);
htmp.GetXaxis().SetTitleOffset(0.9);
htmp.GetYaxis().SetTitleOffset(1.2);
htmp.SetTitleFont(42);
htmp.SetTitle("");

htmp.GetXaxis().SetLabelSize(0.05);
htmp.GetYaxis().SetLabelSize(0.05);
htmp.GetXaxis().SetTitleSize(0.055);
htmp.GetYaxis().SetTitleSize(0.06);

htmp.SetLineWidth(2)
htmp.GetXaxis().SetTitle("Number of pile up interactions")
htmp.GetYaxis().SetTitle("Normalized events/ bin")

htmp.Draw("hist")
htmp2.Draw("histsame")
htmp3.Draw("histsame")
MC_pu.Draw("histsame")
leg_sign.Draw("same")
    
CMS_lumi.lumi_7TeV = "4.8 fb^{-1}"
CMS_lumi.lumi_8TeV = "18.3 fb^{-1}"
CMS_lumi.writeExtraText = 1
CMS_lumi.extraText = "Preliminary"
CMS_lumi.lumi_sqrtS = " (13 TeV)"

iPeriod = 0
iPos = 11

CMS_lumi.CMS_lumi(c1, iPeriod, iPos)

c1.cd()
c1.Update();
c1.RedrawAxis();
#c1.GetFrame().Draw();

c1.Print("Plots/puNom_69.pdf")
c1.Print("Plots/puNom.png")
c1.Print("Plots/puNom.root")
    
