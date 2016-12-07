from utils import *

BprimeBToHB1600_v2 = sample()
BprimeBToHB1600_v2.files = outlist (d,"BprimeBToHB1600_v2")
BprimeBToHB1600_v2.skimEff = 1.
#BprimeBToHB1600_v2.sigma = 1.
BprimeBToHB1600_v2.sigma = 1#0.319
BprimeBToHB1600_v2.jpref = jetLabel
BprimeBToHB1600_v2.jp = jetLabel
BprimeBToHB1600_v2.color =  ROOT.kRed
BprimeBToHB1600_v2.style = 1
BprimeBToHB1600_v2.fill = 0
BprimeBToHB1600_v2.leglabel = "B'(1600 GeV)"
BprimeBToHB1600_v2.label = "BprimeBToHB1600_v2"
