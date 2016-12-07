from utils import *

TT_v2 = sample()
TT_v2.files = outlist (d,"TT_v2")
TT_v2.files = TT_v2.files[:1]
TT_v2.jpref = jetLabel 
TT_v2.jp = jetLabel 
TT_v2.skimEff = 1.
TT_v2.sigma = 831.76
#TT_v2.color = 9
TT_v2.color = ROOT.kBlue + 2
TT_v2.style = 1
TT_v2.fill = 1001
TT_v2.leglabel = "t#bar{t}"
TT_v2.label = "TT_v2"

