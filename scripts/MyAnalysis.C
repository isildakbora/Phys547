#define MyAnalysis_cxx
#include "MyAnalysis.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TLorentzVector.h>
#include <vector>
#include <utility>

#include <MyMuon.C>

#ifdef __MAKECINT__
#pragma link C++ class vector<MyMuon>;
#pragma link C++ class std::vector< TLorentzVector >;
#endif


void MyAnalysis::Loop()
{
//   In a ROOT session, you can do:
//      Root > .L MyAnalysis.C
//      Root > MyAnalysis t
//      Root > t.GetEntry(12); // Fill t data members with entry number 12
//      Root > t.Show();       // Show values of entry 12
//      Root > t.Show(16);     // Read and show values of entry 16
//      Root > t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch


   TH1F* h_iso_muon_multp      = new TH1F("h_iso_muon_multp", "iso_muon_multp", 10, 0, 10);
   TH1F* h_iso_electron_multp  = new TH1F("h_iso_electron_multp", "iso_electron_multp", 10, 0, 10);
   TH1F* h_iso_photon_multp    = new TH1F("h_iso_photon_multp", "iso_photon_multp", 10, 0, 10);
   TH1F* h_bjet_multp          = new TH1F("h_bjet_multp", "bjet_multp", 10, 0, 10);
   TH1F* h_jet_multp           = new TH1F("h_jet_multp", "jet_multp", 10, 0, 10);


   TH1F* h_muon_isolation      = new TH1F("h_muon_isolation", "muon_isolation", 1000, 0, 50);
   TH1F* h_electron_isolation  = new TH1F("h_electron_isolation", "electron_isolation", 1000, 0, 50);

   TH1F* h_muon_pT             = new TH1F("h_muon_pT", "muon_pT", 1000, 0, 1000);
   TH1F* h_electron_pT         = new TH1F("h_electron_pT", "electron_pT", 1000, 0, 1000);
   TH1F* h_photon_pT           = new TH1F("h_photon_pT", "photon_pT", 1000, 0, 1000);
   TH1F* h_jet_pT              = new TH1F("h_jet_pT", "jet_pT", 1000, 0, 1000);
   TH1F* h_bjet_pT             = new TH1F("h_bjet_pT", "bjet_pT", 1000, 0, 1000);

   TH1F* h_iso_dimuon_mass     = new TH1F("h_iso_dimuon_mass", "iso_dimuon_mass", 1000, 0, 1000);
   TH1F* h_W_mass              = new TH1F("h_W_mass", "W_mass", 300, 0, 300);


   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   int iso_lepton_Selected = nentries;
   int n_jet_selected      = nentries;
   double n_selected       = 0;
   double nTotal           = 0;

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry = 0; jentry < nentries; jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      std::cout << "\r" << TMath::FloorNint(100.0 * jentry / nentries) << "% completed: ";

      // if (Cut(ientry) < 0) continue;

      //construct the Lorentz vectors for each object
      int n_bjets = 0;
      int n_jets  = 0;
      int n_non_b_jets  = 0;
      int n_iso_muons = 0;
      int n_iso_electrons = 0;
      int n_iso_photons = 0;

      std::vector<MyMuon> iso_muons;
      std::vector<TLorentzVector> muons;
      std::vector<TLorentzVector> non_b_jets;
      std::vector<TLorentzVector> b_jets;
      std::vector<TLorentzVector> electrons;
      std::vector<TLorentzVector> iso_electrons;
      std::vector<TLorentzVector> photons;
      std::vector<TLorentzVector> iso_photons;

      MyMuon tmp;

      for (int i_muon = 0; i_muon < NMuon; ++i_muon)
      {
         tmp.SetXYZT(Muon_Px[i_muon], Muon_Py[i_muon], Muon_Pz[i_muon], Muon_E[i_muon]);

         if (tmp.Pt() < 20 || tmp.Eta() > 2.1 ) continue;
         
         muons.push_back(tmp);
         h_muon_pT->Fill(muons.back().Pt(), EventWeight);

         if (Muon_Iso[i_muon] < 0.125)
         {
            n_iso_muons += 1;
            tmp.SetXYZT(Muon_Px[i_muon], Muon_Py[i_muon], Muon_Pz[i_muon], Muon_E[i_muon]);
            tmp.SetIsolation(Muon_Iso[i_muon]);
            tmp.SetCharge(Muon_Charge[i_muon]);
            iso_muons.push_back(tmp);
         }
      }

      for (int i_electron = 0; i_electron < NElectron; ++i_electron)
      {
         tmp.SetXYZT(Electron_Px[i_electron], Electron_Py[i_electron], Electron_Pz[i_electron], Electron_E[i_electron]);

         if (tmp.Pt() < 30 || tmp.Eta() > 2.4 ) continue;
         electrons.push_back(tmp);
         h_electron_pT->Fill(electrons.back().Pt(), EventWeight);

         if (Electron_Iso[i_electron] < 0.1)
         {
            n_iso_electrons += 1;
            tmp.SetXYZT(Electron_Px[i_electron], Electron_Py[i_electron], Electron_Pz[i_electron], Electron_E[i_electron]);
            iso_electrons.push_back(tmp);
         }
      }

      for (int i_photon = 0; i_photon < NPhoton; ++i_photon)
      {

         tmp.SetXYZT(Photon_Px[i_photon], Photon_Py[i_photon], Photon_Pz[i_photon], Photon_E[i_photon]);
         photons.push_back(tmp);
         h_photon_pT->Fill(photons.back().Pt(), EventWeight);

         if (Photon_Iso[i_photon] = 0.1)
         {
            n_iso_photons += 1;
            tmp.SetXYZT(Photon_Px[i_photon], Photon_Py[i_photon], Photon_Pz[i_photon], Photon_E[i_photon]);
            iso_photons.push_back(tmp);
         }
      }

      for (int i_jet = 0; i_jet < NJet; ++i_jet)
      {

         if (Jet_btag[i_jet] > 0.8)
         {
            n_bjets += 1;
            tmp.SetXYZT(Jet_Px[i_jet], Jet_Py[i_jet], Jet_Pz[i_jet], Jet_E[i_jet]);
            b_jets.push_back(tmp);
            h_bjet_pT->Fill(b_jets.back().Pt(), EventWeight);
         }
         else
         {
            n_non_b_jets += 1 ;
            tmp.SetXYZT(Jet_Px[i_jet], Jet_Py[i_jet], Jet_Pz[i_jet], Jet_E[i_jet]);
            non_b_jets.push_back(tmp);
            h_jet_pT->Fill(non_b_jets.back().Pt(), EventWeight);

         }
         n_jets += 1;
      }

      //if (iso_lepton_cut){ continue;}

      bool iso_lepton_cut = (n_iso_muons + n_iso_electrons < 1);
      bool n_jet_cut = n_jets < 3;
      bool MET_cut   = TMath::Sqrt(MET_px * * 2 + MET_py * * 2) < 20;
      bool n_b_tag_cut = n_bjets == 0;
      bool non_b_jets_cut = n_non_b_jets < 2;

      if (iso_lepton_cut) {iso_lepton_Selected = iso_lepton_Selected - 1;}
      if (n_jet_cut) {n_jet_selected = n_jet_selected - 1;}
      if (triggerIsoMu24 && !iso_lepton_cut && !MET_cut && !n_b_tag_cut && !non_b_jets_cut)
      {
         n_selected += EventWeight;

         if (inf_name == "ttbar.root" && non_b_jets.size() == 2)
         {
            //printf("number of jets:%i\n", non_b_jets.size() );
            double W_mass = (non_b_jets.at(0) + non_b_jets.at(1) + b_jets.at(0)).M();
            h_W_mass->Fill(W_mass, EventWeight);
         }
      }

      nTotal += EventWeight;

      h_iso_muon_multp->Fill(n_iso_muons, EventWeight);
      h_iso_electron_multp->Fill(n_iso_electrons, EventWeight);
      h_iso_photon_multp->Fill(n_iso_photons, EventWeight);
      h_bjet_multp->Fill(n_bjets, EventWeight);
      h_jet_multp->Fill(n_jets, EventWeight);

      if (n_iso_muons > 1)
      {
         double dimuon_mass = (iso_muons.at(0) + iso_muons.at(1)).M();
         h_iso_dimuon_mass->Fill(dimuon_mass, EventWeight);
      }
   }


   //h_iso_muon_multp->Draw();
   TFile *outf = new TFile("out_" + inf_name, "RECREATE");
   std::cout << inf_name << " iso_lepton_Rejection:" << (nentries - iso_lepton_Selected) / (double)nentries << std::endl;
   std::cout << " n_jet_Rejection:" << (nentries - n_jet_selected) / (double)nentries << std::endl;
   std::cout << " n_expected:" << n_selected  << " nentries:" << nTotal << std::endl;
   h_iso_muon_multp->Write();
   h_iso_electron_multp->Write();
   h_iso_photon_multp->Write();
   h_bjet_multp->Write();
   h_jet_multp->Write();

   h_muon_pT->Write();
   h_electron_pT->Write();
   h_photon_pT->Write();
   h_jet_pT->Write();
   h_bjet_pT->Write();

   h_muon_isolation->Write();
   h_electron_isolation->Write();
   h_iso_dimuon_mass->Write();
   if (inf_name == "ttbar.root") h_W_mass->Write();
   outf->Close();
}
