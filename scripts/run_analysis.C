
{
	TString file_names[] = {"qcd.root", "dy.root", "zz.root", "wz.root", "ww.root", "wjets.root", "ttbar.root", "single_top.root", "data.root"}
	
	gSystem->Load("MyAnalysis.C");
	TFile f;
	MyAnalysis* m;

	for (int i = 0; i < 9; ++i)
	{	
		f = TFile::Open(file_names[i]);
		m = new MyAnalysis(events, file_names[i]);
		cout << file_names[i] << endl;
		m->Loop();
	}
}