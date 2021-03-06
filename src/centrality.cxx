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

#include <Utilities.h>

#include "header.h"

using namespace PythiaAngantyrStudy;



int main (int argc, char** argv) {

  if (argc < 4) {
    std::cout << " usage: centrality NAME INFILEPATTERN OUTFILENAME" << std::endl;
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
  float gap_negEta = 0;
  float gap_posEta = 0;

  const int nEvents = inTree->GetEntries ();

  inTree->SetBranchAddress ("fcal_et_negEta", &fcal_et_negEta);
  inTree->SetBranchAddress ("fcal_et_posEta", &fcal_et_posEta);
  if (name.find ("pPb") != std::string::npos)
    inTree->SetBranchAddress ("ncoll",        &ncoll);
  inTree->SetBranchAddress ("gap_negEta", &gap_negEta);
  inTree->SetBranchAddress ("gap_posEta", &gap_posEta);

  TH1D* h_fcal_et_negEta = nullptr;
  TH1D* h_fcal_et_posEta = nullptr;
  TH1D* h_fcal_et_tot = nullptr;
  TH1D* h_ncoll = nullptr;


  TFile* outFile = new TFile (outFileName.c_str (), "recreate");

  h_fcal_et_negEta = new TH1D (Form ("h_fcal_et_negEta_%s", name.c_str ()), ";#Sigma#it{E}_{T}^{FCal}(#eta<0) [GeV]", 250, -30, 220);
  h_fcal_et_negEta->Sumw2 ();
  h_fcal_et_posEta = new TH1D (Form ("h_fcal_et_posEta_%s", name.c_str ()), ";#Sigma#it{E}_{T}^{FCal}(#eta>0) [GeV]", 250, -30, 220);
  h_fcal_et_posEta->Sumw2 ();
  h_fcal_et_tot = new TH1D (Form ("h_fcal_et_tot_%s", name.c_str ()), ";#Sigma#it{E}_{T}^{FCal} [GeV]", 250, -30, 220);
  h_fcal_et_tot->Sumw2 ();

  h_ncoll = new TH1D (Form ("h_ncoll_%s", name.c_str ()), ";N_{coll}", 100, -0.5, 99.5);
  h_ncoll->Sumw2 ();


  for (int iEvent = 0; iEvent < nEvents; iEvent++) {
    if (nEvents > 100 && iEvent % (nEvents / 100) == 0)
      cout << iEvent / (nEvents / 100) << "\% done...\r" << flush;

    inTree->GetEntry (iEvent);

    if (gap_posEta > 1.4 || gap_negEta > 1.4)
      continue;

    h_fcal_et_negEta->Fill (fcal_et_negEta);
    h_fcal_et_posEta->Fill (fcal_et_posEta);
    h_fcal_et_tot->Fill (fcal_et_negEta + fcal_et_posEta);

    h_ncoll->Fill (ncoll);
  } // end loop over iEvent

  outFile->cd ();

  // now save histograms to a rootfile
  h_fcal_et_negEta->Write ();
  h_fcal_et_posEta->Write ();
  h_fcal_et_tot->Write ();

  h_ncoll->Write ();
  
  outFile->Close ();

  return 0;
}

#endif
