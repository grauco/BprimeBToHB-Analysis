from utils import *


#ZJets = sample()
#ZJets.files = outlist (d,"ZJets")
#ZJets.jpref = jetLabel 
#ZJets.jp = jetLabel 
#ZJets.sf = 0.0747229
#ZJets.skimEff = 1.
#ZJets.sigma = 409.4874 
#ZJets.color = ROOT.kYellow -9
#ZJets.style = 1
#ZJets.fill = 1001
#ZJets.leglabel = "ZJets"
#ZJets.label = "ZNuNu"


ZJets_100_200 = sample()
ZJets_100_200.files = outlist (d,"ZJets_100_200")
ZJets_100_200.jpref = jetLabel 
ZJets_100_200.jp = jetLabel 
ZJets_100_200.sf = 1#0.0747229
ZJets_100_200.skimEff = 1.
ZJets_100_200.sigma = 280.47
#ZJets_100_200.sigma = 409.4874
ZJets_100_200.color = ROOT.kYellow -9
ZJets_100_200.style = 1
ZJets_100_200.fill = 1001
ZJets_100_200.leglabel = "ZJets"
ZJets_100_200.label = "ZJets_100_200"

ZJets_200_400 = sample()
ZJets_200_400.files = outlist (d,"ZJets_200_400")
ZJets_200_400.jpref = jetLabel 
ZJets_200_400.jp = jetLabel 
ZJets_200_400.sf = 1#0.0221710
ZJets_200_400.skimEff = 1.
#ZJets_200_400.sigma = 110.7792 
ZJets_200_400.sigma = 77.47 
ZJets_200_400.color = ROOT.kYellow -9
ZJets_200_400.style = 1
ZJets_200_400.fill = 1001
ZJets_200_400.leglabel = "ZJets"
ZJets_200_400.label = "ZJets_200_400"

ZJets_400_600 = sample()
ZJets_400_600.files = outlist (d,"ZJets_400_600")
ZJets_400_600.jpref = jetLabel 
ZJets_400_600.jp = jetLabel 
ZJets_400_600.sf = 1#0.0027042
ZJets_400_600.skimEff = 1.
#ZJets_400_600.sigma = 13.17701
ZJets_400_600.sigma = 10.73 
ZJets_400_600.color = ROOT.kYellow -9
ZJets_400_600.style = 1
ZJets_400_600.fill = 1001
ZJets_400_600.leglabel = "ZJets"
ZJets_400_600.label = "ZJets_400_600"

ZJets_600_800 = sample()
ZJets_600_800.files = outlist (d,"ZJets_600_800")
ZJets_600_800.jpref = jetLabel 
ZJets_600_800.jp = jetLabel 
ZJets_600_800.sf = 1#0.0009820
ZJets_600_800.skimEff = 1.
#ZJets_600_800.sigma = 4.520187
ZJets_600_800.sigma = 2.559
ZJets_600_800.color = ROOT.kYellow -9
ZJets_600_800.style = 1
ZJets_600_800.fill = 1001
ZJets_600_800.leglabel = "ZJets"
ZJets_600_800.label = "ZJets_600_800"

ZJets_800_1200 = sample()
ZJets_800_1200.files = outlist (d,"ZJets_800_1200")
ZJets_800_1200.jpref = jetLabel
ZJets_800_1200.jp = jetLabel
ZJets_800_1200.sf = 1#0.0009820
ZJets_800_1200.skimEff = 1.
#ZJets_800_1200.sigma = 4.520187                                                                              
ZJets_800_1200.sigma = 1.1796
ZJets_800_1200.color = ROOT.kYellow -9
ZJets_800_1200.style = 1
ZJets_800_1200.fill = 1001
ZJets_800_1200.leglabel = "ZJets"
ZJets_800_1200.label = "ZJets_800_1200"


ZJets_1200_2500 = sample()
ZJets_1200_2500.files = outlist (d,"ZJets_1200_2500")
ZJets_1200_2500.jpref = jetLabel
ZJets_1200_2500.jp = jetLabel
ZJets_1200_2500.sf = 1#0.0009820
ZJets_1200_2500.skimEff = 1.
#ZJets_1200_2500.sigma = 4.520187                                                                               
ZJets_1200_2500.sigma = 0.28833
ZJets_1200_2500.color = ROOT.kYellow -9
ZJets_1200_2500.style = 1
ZJets_1200_2500.fill = 1001
ZJets_1200_2500.leglabel = "ZJets"
ZJets_1200_2500.label = "ZJets_1200_2500"

ZJets_2500_Inf = sample()
ZJets_2500_Inf.files = outlist (d,"ZJets_2500_Inf")
ZJets_2500_Inf.jpref = jetLabel
ZJets_2500_Inf.jp = jetLabel
ZJets_2500_Inf.sf = 1#0.0009820
ZJets_2500_Inf.skimEff = 1.
#ZJets_2500_Inf.sigma = 4.520187                                                                            
ZJets_2500_Inf.sigma = 0.006945
ZJets_2500_Inf.color = ROOT.kYellow -9
ZJets_2500_Inf.style = 1
ZJets_2500_Inf.fill = 1001
ZJets_2500_Inf.leglabel = "ZJets"
ZJets_2500_Inf.label = "ZJets_2500_Inf"


ZJets = sample()
ZJets.color = ROOT.kMagenta + 2
ZJets.style = 1
ZJets.fill = 1001
ZJets.leglabel = "Z#rightarrow#nu#nu"
ZJets.label = "ZJets"
#ZJets.components = [ ZJets]
ZJets.components = [  ZJets_100_200,  ZJets_200_400,  ZJets_400_600,  ZJets_800_1200,  ZJets_2500_Inf]

