{
	TCanvas* c1 = new TCanvas("c1","c1",600,700);

	TFile* f 			   = new TFile("out_ttbar.root");
	TH1F*  h_muon_pT_pass  = (TH1F*)f->FindObjectAny("h_muon_pT_pass");
	TH1F*  h_muon_pT_total = (TH1F*)f->FindObjectAny("h_muon_pT_total");

	TEfficiency *eff = new TEfficiency(*h_muon_pT_pass,*h_muon_pT_total);
	eff->Draw();
}