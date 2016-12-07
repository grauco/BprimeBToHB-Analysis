######################################
#
# Annapaola de Cosa, January 2015
#
######################################


from utils import *

#SingleTopT_tchan = sample()
#SingleTopT_tchan.files = outlist (d,"SingleTop_T_tchan")
#SingleTopT_tchan.skimEff = 1
#SingleTopT_tchan.sigma =  70.29828*4.65068
#SingleTopT_tchan.sigma =  136.02 * 0.324
#SingleTopT_tchan.color = ROOT.kYellow -7
#SingleTopT_tchan.jpref = jetLabel 
#SingleTopT_tchan.jp = jetLabel
#SingleTopT_tchan.style = 1
#SingleTopT_tchan.fill = 1001#
#SingleTopT_tchan.leglabel = "SingleTop"
#SingleTopT_tchan.label = "SingleTop_T_tchan"

#SingleTopTbar_tchan = sample()
#SingleTopTbar_tchan.files = outlist (d,"SingleTop_Tbar_tchan")
#SingleTopTbar_tchan.skimEff = 1.
#SingleTopTbar_tchan.sigma = 80.95 * 0.324
#SingleTopTbar_tchan.sigma = 26.2278*3.85
#SingleTopTbar_tchan.color = ROOT.kYellow -7
#SingleTopTbar_tchan.jpref = jetLabel 
#SingleTopTbar_tchan.jp = jetLabel
#SingleTopTbar_tchan.style = 1
#SingleTopTbar_tchan.fill = 1001
#SingleTopTbar_tchan.leglabel = "SingleTop"
#SingleTopTbar_tchan.label = "SingleTop_Tbar_tchan"

#SingleTopT_tW = sample()
#SingleTopT_tW.files = outlist (d,"T_tW")
#SingleTopT_tW.skimEff = 1.0
#SingleTopT_tW.sigma = 35.6
#SingleTopT_tW.jpref = jetLabel 
#SingleTopT_tW.jp = jetLabel
#SingleTopT_tW.color = ROOT.kYellow - 7
#SingleTopT_tW.style = 1
#SingleTopT_tW.fill = 1001
#SingleTopT_tW.leglabel = "SingleTop"
#SingleTopT_tW.label = "SingleTop_T_tW"

#SingleTopTbar_tW = sample()
#SingleTopTbar_tW.files = outlist (d,"SingleTop_Tbar_tW")
#SingleTopTbar_tW.skimEff = 1.
#SingleTopTbar_tW.sigma = 35.6
#SingleTopTbar_tW.jpref = jetLabel 
#SingleTopTbar_tW.jp = jetLabel
#SingleTopTbar_tW.color = ROOT.kYellow -7
#SingleTopTbar_tW.style = 1
#SingleTopTbar_tW.fill = 1001
#SingleTopTbar_tW.leglabel = "SingleTop"
#SingleTopTbar_tW.label = "SingleTop_Tbar_tW"


WJets_100_200 = sample()
WJets_100_200.files = outlist (d,"WJets_100_200")
WJets_100_200.skimEff = 1.
WJets_100_200.sigma = 1345
#WJets_100_200.sigma = 2234.91
WJets_100_200.jpref = jetLabel 
WJets_100_200.jp = jetLabel
WJets_100_200.color = ROOT.kYellow -7
WJets_100_200.style = 1
WJets_100_200.fill = 1001
WJets_100_200.leglabel = "WJets"
WJets_100_200.label = "WJets_100_200"

WJets_200_400 = sample()
WJets_200_400.files = outlist (d,"WJets_200_400")
WJets_200_400.skimEff = 1.
#WJets_200_400.sigma = 580.068
WJets_200_400.sigma = 359.7
WJets_200_400.jpref = jetLabel 
WJets_200_400.jp = jetLabel
WJets_200_400.color = ROOT.kYellow -7
WJets_200_400.style = 1
WJets_200_400.fill = 1001
WJets_200_400.leglabel = "WJets"
WJets_200_400.label = "WJets_200_400"

WJets_400_600 = sample()
WJets_400_600.files = outlist (d,"WJets_400_600")
WJets_400_600.skimEff = 1.
WJets_400_600.sigma = 48.91
#WJets_400_600.sigma = 68.4003
WJets_400_600.jpref = jetLabel 
WJets_400_600.jp = jetLabel
WJets_400_600.color = ROOT.kYellow -7
WJets_400_600.style = 1
WJets_400_600.fill = 1001
WJets_400_600.leglabel = "WJets"
WJets_400_600.label = "WJets_400_600"


WJets_600_800 = sample()
WJets_600_800.files = outlist (d,"WJets_600_800")
WJets_600_800.skimEff = 1.
WJets_600_800.sigma = 12.05
#WJets_600_800.sigma = 68.4003
WJets_600_800.jpref = jetLabel 
WJets_600_800.jp = jetLabel
WJets_600_800.color = ROOT.kYellow -7
WJets_600_800.style = 1
WJets_600_800.fill = 1001
WJets_600_800.leglabel = "WJets"
WJets_600_800.label = "WJets_600_800"


WJets_800_1200 = sample()
WJets_800_1200.files = outlist (d,"WJets_800_1200")
WJets_800_1200.skimEff = 1.
#WJets_800_1200.sigma = 68.4003
WJets_800_1200.sigma = 5.501
WJets_800_1200.jpref = jetLabel 
WJets_800_1200.jp = jetLabel
WJets_800_1200.color = ROOT.kYellow -7
WJets_800_1200.style = 1
WJets_800_1200.fill = 1001
WJets_800_1200.leglabel = "WJets"
WJets_800_1200.label = "WJets_800_1200"


WJets_1200_2500 = sample()
WJets_1200_2500.files = outlist (d,"WJets_1200_2500")
WJets_1200_2500.skimEff = 1.
WJets_1200_2500.sigma = 1.329
#WJets_1200_2500.sigma = 68.4003
WJets_1200_2500.jpref = jetLabel 
WJets_1200_2500.jp = jetLabel
WJets_1200_2500.color = ROOT.kYellow -7
WJets_1200_2500.style = 1
WJets_1200_2500.fill = 1001
WJets_1200_2500.leglabel = "WJets"
WJets_1200_2500.label = "WJets_1200_2500"


WJets_2500_Inf = sample()
WJets_2500_Inf.files = outlist (d,"WJets_2500_Inf")
WJets_2500_Inf.skimEff = 1.
#WJets_2500_Inf.sigma = 23.1363
WJets_2500_Inf.sigma = 0.03216
WJets_2500_Inf.jpref = jetLabel 
WJets_2500_Inf.jp = jetLabel
WJets_2500_Inf.color = ROOT.kYellow -7
WJets_2500_Inf.style = 1
WJets_2500_Inf.fill = 1001
WJets_2500_Inf.leglabel = "WJets"
WJets_2500_Inf.label = "WJets_2500_Inf"


#Single_p = sample()
#Single_p.color = 5
#Single_p.color = ROOT.kOrange -2
#Single_p.style = 1
#Single_p.fill = 1001
#Single_p.leglabel = "Single_p"
#Single_p.label = "Single_p"
#Single_p.components = [Single_pTbar_tW, Single_pT_tW, Single_pT_tchan, Single_pTbar_tchan]
#Single_p.components = [Single_pTbar_tW, Single_pT_tW]





#WJets = sample()
#WJets.files = outlist (d,"WJets")
#WJets.skimEff = 1.
#WJets.sigma =  61526.7*1.46209
#WJets.jpref = jetLabel 
#WJets.jp = jetLabel
#WJets.color = ROOT.kTeal -6
#WJets.style = 1
#WJets.fill = 1001
#WJets.leglabel = "W + Jets"
#WJets.label = "WJets"


WJets = sample()
#WJets.color = 6
WJets.color = ROOT.kYellow-7
WJets.style = 1
WJets.fill = 1001
WJets.leglabel = "W + Jets"
WJets.label = "WJets"
#WJets.components = [WJets]
WJets.components = [WJets_100_200, WJets_200_400, WJets_400_600,  WJets_800_1200, WJets_1200_2500, WJets_2500_Inf]
# WJets.components = [WJets_100_200, WJets_200_400,  WJets_800_1200, WJets_1200_2500 ]





