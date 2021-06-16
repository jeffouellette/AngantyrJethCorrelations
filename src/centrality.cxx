#ifndef __centrality_cxx__
#define __centrality_cxx__

#include <TFile.h>
#include <TChain.h>
#include <TH1D.h>
#include <TH2D.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <math.h>

#include <AtlasUtils.h>

#include <GlobalParams.h>
#include <Utilities.h>

#include "header.h"

using namespace PythiaAngantyrStudy;



int main (int argc, char** argv) {

  if (argc < 5) {
    std::cout << " usage: centrality NAME MINJPT INFILEPATTERN OUTFILENAME" << std::endl;
    return 0;
  }

  const string name = std::string (argv[1]);
  const float minjpt = atof (argv[2]);
  const string inFilePattern = std::string (argv[3]);
  const string outFileName = std::string (argv[4]);

  const bool ispPb = (name.find ("pPb") != std::string::npos);

  std::cout << "ispPb = " << (ispPb ? "true" : "false") << std::endl;

  const float boost = (ispPb ? 0.465 : 0);

  std::cout << "boost = " << boost << std::endl;

  std::cout << "InFilePattern = " << inFilePattern << std::endl;

  TChain* inTree = new TChain ("tree", "tree");
  inTree->Add (inFilePattern.c_str ());

  std::cout << "Added " << inTree->GetListOfFiles ()->GetEntries () << " files to TChain, with " << inTree->GetEntries () << " events" << std::endl;

  float fcal_et_negEta = 0;
  float fcal_et_posEta = 0;

  int akt4_jet_n = 0;
  float akt4_jet_pt[100];
  float akt4_jet_eta[100];
  float akt4_jet_phi[100];
  float akt4_jet_e[100];
  float akt4_jet_m[100];

  const int nEvents = inTree->GetEntries ();
  double sumWgtsEvents = 0, sumWgtsSqEvents = 0;
  int nJetEvents = 0;
  double sumWgtsJetEvents = 0, sumWgtsSqJetEvents = 0;

  inTree->SetBranchAddress ("akt4_jet_n",   &akt4_jet_n);
  inTree->SetBranchAddress ("akt4_jet_pt",  &akt4_jet_pt);
  inTree->SetBranchAddress ("akt4_jet_eta", &akt4_jet_eta);
  inTree->SetBranchAddress ("akt4_jet_phi", &akt4_jet_phi);
  inTree->SetBranchAddress ("akt4_jet_e",   &akt4_jet_e);
  inTree->SetBranchAddress ("akt4_jet_m",   &akt4_jet_m);

  inTree->SetBranchAddress ("fcal_et_negEta", &fcal_et_negEta);
  inTree->SetBranchAddress ("fcal_et_posEta", &fcal_et_posEta);


  TH1D* h_fcal_et_negEta = nullptr;
  TH1D* h_fcal_et_posEta = nullptr;
  TH1D* h_fcal_et_tot = nullptr;


  TFile* outFile = new TFile (outFileName.c_str (), "recreate");

  h_fcal_et_negEta = new TH1D (Form ("h_fcal_et_negEta_%s", name.c_str ()), ";#Sigma#it{E}_{T}^{FCal}(#eta<0) [GeV]", 250, -30, 220);
  h_fcal_et_negEta->Sumw2 ();
  h_fcal_et_posEta = new TH1D (Form ("h_fcal_et_posEta_%s", name.c_str ()), ";#Sigma#it{E}_{T}^{FCal}(#eta>0) [GeV]", 250, -30, 220);
  h_fcal_et_posEta->Sumw2 ();
  h_fcal_et_tot = new TH1D (Form ("h_fcal_et_tot_%s", name.c_str ()), ";#Sigma#it{E}_{T}^{FCal} [GeV]", 250, -30, 220);
  h_fcal_et_tot->Sumw2 ();


  for (int iEvent = 0; iEvent < nEvents; iEvent++) {
    if (nEvents > 100 && iEvent % (nEvents / 100) == 0)
      cout << iEvent / (nEvents / 100) << "\% done...\r" << flush;

    inTree->GetEntry (iEvent);

    bool trigEvent = (minjpt <= 0);
    int iJ = -1;
    while (iJ++ < akt4_jet_n && !trigEvent) trigEvent = (fabs (akt4_jet_eta[iJ]) < 2.8 && akt4_jet_pt[iJ] > minjpt);

    if (!trigEvent) continue;

    h_fcal_et_negEta->Fill (fcal_et_negEta);
    h_fcal_et_posEta->Fill (fcal_et_posEta);
    h_fcal_et_tot->Fill (fcal_et_negEta + fcal_et_posEta);
  } // end loop over iEvent

  outFile->cd ();

  // now save histograms to a rootfile
  h_fcal_et_negEta->Write ();
  h_fcal_et_posEta->Write ();
  h_fcal_et_tot->Write ();
  
  outFile->Close ();

  return 0;
}

#endif
