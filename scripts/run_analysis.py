import ROOT
from ROOT import *

file_names = ["qcd.root", "zz.root", "wz.root", "ww.root", "wjets.root", "ttbar.root", "single_top.root", "data.root", ]

gSystem.Load("MyAnalysis.C");

for inf_file in file_names:
	print inf_file
	f = TFile.Open(inf_file)
	
	cmd = 'MyAnalysis m(events, \"'+ str(inf_file) +'\")'
	gROOT.ProcessLine(cmd)
	print 'Processing', inf_file
	gROOT.ProcessLine("m.Loop()")