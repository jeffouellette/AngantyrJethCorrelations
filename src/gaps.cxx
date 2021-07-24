#ifndef __gaps_cxx__
#define __gaps_cxx__

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

#include <Utilities.h>

#include "header.h"

using namespace PythiaAngantyrStudy;



int main (int argc, char** argv) {

  if (argc < 5) {
    std::cout << " usage: gaps NAME INFILEPATTERN OUTFILENAME" << std::endl;
    return 0;
  }

  const string name = std::string (argv[1]);
  const string inFilePattern = std::string (argv[2]);
  const string outFileName = std::string (argv[3]);

  const bool ispPb = (name.find ("pPb") != std::string::npos);

  std::cout << "ispPb = " << (ispPb ? "true" : "false") << std::endl;

  const float boost = (ispPb ? 0.465 : 0);

  std::cout << "boost = " << boost << std::endl;

  std::cout << "InFilePattern = " << inFilePattern << std::endl;

  TChain* inTree = new TChain ("tree", "tree");
  inTree->Add (inFilePattern.c_str ());

  std::cout << "Added " << inTree->GetListOfFiles ()->GetEntries () << " files to TChain, with " << inTree->GetEntries () << " events" << std::endl;

  int ncoll = 1;
  float fcal_et_negEta = 0;
  float fcal_et_posEta = 0;
  int code;
  float gap_negEta = 0;
  float gap_posEta = 0;

  const int nEvents = inTree->GetEntries ();

  inTree->SetBranchAddress ("code",           &code);

  inTree->SetBranchAddress ("fcal_et_negEta", &fcal_et_negEta);
  inTree->SetBranchAddress ("fcal_et_posEta", &fcal_et_posEta);

  inTree->SetBranchAddress ("gap_negEta",     &gap_negEta);
  inTree->SetBranchAddress ("gap_posEta",     &gap_posEta);

  if (name.find ("pPb") != std::string::npos)
    inTree->SetBranchAddress ("ncoll",        &ncoll);

  TH1D* h_fcal_et_negEta = nullptr;
  TH1D* h_fcal_et_posEta = nullptr;
  TH1D* h_fcal_et_tot = nullptr;
  TH1D* h_ncoll = nullptr;

  TH1D* h_gap_negEta[4];
  TH1D* h_gap_posEta[4];


  TFile* outFile = new TFile (outFileName.c_str (), "recreate");

  h_fcal_et_negEta = new TH1D (Form ("h_fcal_et_negEta_%s", name.c_str ()), ";#Sigma#it{E}_{T}^{FCal}(#eta<0) [GeV]", 250, -30, 220);
  h_fcal_et_negEta->Sumw2 ();
  h_fcal_et_posEta = new TH1D (Form ("h_fcal_et_posEta_%s", name.c_str ()), ";#Sigma#it{E}_{T}^{FCal}(#eta>0) [GeV]", 250, -30, 220);
  h_fcal_et_posEta->Sumw2 ();
  h_fcal_et_tot = new TH1D (Form ("h_fcal_et_tot_%s", name.c_str ()), ";#Sigma#it{E}_{T}^{FCal} [GeV]", 250, -30, 220);
  h_fcal_et_tot->Sumw2 ();

  for (int i = 0; i < 4; i++) {

    std::string evtType;
    if      (i == 0)  evtType = "nondiff";
    else if (i == 1)  evtType = "single_diff";
    else if (i == 2)  evtType = "double_diff";
    else if (i == 3)  evtType = "central_diff";

    h_gap_negEta[i] = new TH1D (Form ("h_gap_negEta_%s_%s", evtType.c_str (), name.c_str ()), ";#Delta#it{#eta}_{neg.};Events", 98, -0.05, 9.85);
    h_gap_negEta[i]->Sumw2 ();
    h_gap_posEta[i] = new TH1D (Form ("h_gap_posEta_%s_%s", evtType.c_str (), name.c_str ()), ";#Delta#it{#eta}_{pos.};Events", 98, -0.05, 9.85);
    h_gap_posEta[i]->Sumw2 ();
  }

  h_ncoll = new TH1D (Form ("h_ncoll_%s", name.c_str ()), ";N_{coll}", 100, -0.5, 99.5);
  h_ncoll->Sumw2 ();


  for (int iEvent = 0; iEvent < nEvents; iEvent++) {
    if (nEvents > 100 && iEvent % (nEvents / 100) == 0)
      cout << iEvent / (nEvents / 100) << "\% done...\r" << flush;

    inTree->GetEntry (iEvent);

    h_fcal_et_negEta->Fill (fcal_et_negEta);
    h_fcal_et_posEta->Fill (fcal_et_posEta);
    h_fcal_et_tot->Fill (fcal_et_negEta + fcal_et_posEta);

    int i = -1;
    if      (code == 101)                 i = 0; // non-diffractive
    else if (code == 103 || code == 104)  i = 1; // single diffractive
    else if (code == 105)                 i = 2; // double diffractive
    else if (code == 106)                 i = 3; // central diffractive

    if (i != -1) {
      h_gap_negEta[i]->Fill (gap_negEta);
      h_gap_posEta[i]->Fill (gap_posEta);
    }

    h_ncoll->Fill (ncoll);
  } // end loop over iEvent

  outFile->cd ();

  // now save histograms to a rootfile
  h_fcal_et_negEta->Write ();
  h_fcal_et_posEta->Write ();
  h_fcal_et_tot->Write ();

  h_ncoll->Write ();

  for (int i = 0; i < 4; i++) {
    h_gap_negEta[i]->Write ();
    h_gap_posEta[i]->Write ();
  }
  
  outFile->Close ();

  return 0;
}

#endif
