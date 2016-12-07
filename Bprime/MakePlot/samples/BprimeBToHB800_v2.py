from utils import *

BprimeBToHB800_v2 = sample()
BprimeBToHB800_v2.files = outlist (d,"BprimeBToHB800_v2")
BprimeBToHB800_v2.skimEff = 1.
BprimeBToHB800_v2.sigma = 1#3.016
#BprimeBToHB800_v2.sigma = 1.
BprimeBToHB800_v2.jpref = jetLabel 
BprimeBToHB800_v2.jp = jetLabel 
BprimeBToHB800_v2.color =  ROOT.kRed
BprimeBToHB800_v2.style = 1
BprimeBToHB800_v2.fill = 0
BprimeBToHB800_v2.leglabel = "B'(800 GeV)"
BprimeBToHB800_v2.label = "BprimeBToHB800_v2"
