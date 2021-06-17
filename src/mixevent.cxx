#ifndef __mixevent_cxx__
#define __mixevent_cxx__

#include <TFile.h>
#include <TChain.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TF1.h>
#include <TLine.h>
#include <TProfile.h>
#include <TLorentzVector.h>

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

  if (argc < 4) {
    std::cout << " usage: mixevent NAME TAGFILEPATTERN MIXFILEPATTERN OUTFILENAME" << std::endl;
    return 0;
  }

  const string name = std::string (argv[1]);
  const string tagFilePattern = std::string (argv[2]);
  const string mixFilePattern = std::string (argv[3]);
  const string outFileName = std::string (argv[4]);

  const bool ispPb = (name.find ("pPb") != std::string::npos);

  std::cout << "ispPb = " << (ispPb ? "true" : "false") << std::endl;

  const float boost = (ispPb ? -0.465 : 0);

  std::cout << "boost = " << boost << std::endl;

  std::cout << "TagFilePattern = " << tagFilePattern << std::endl;
  std::cout << "MixFilePattern = " << mixFilePattern << std::endl;

  TChain* tagTree = new TChain ("tree", "tree");
  tagTree->Add (tagFilePattern.c_str ());

  std::cout << "Added " << tagTree->GetListOfFiles ()->GetEntries () << " files to tag TChain, with " << tagTree->GetEntries () << " events" << std::endl;


  TChain* mixTree = new TChain ("tree", "tree");
  mixTree->Add (mixFilePattern.c_str ());

  std::cout << "Added " << mixTree->GetListOfFiles ()->GetEntries () << " files to mix TChain, with " << mixTree->GetEntries () << " events" << std::endl;


  //int akt2_jet_n = 0;
  //float akt2_jet_pt[100];
  //float akt2_jet_eta[100];
  //float akt2_jet_phi[100];
  //float akt2_jet_e[100];
  //float akt2_jet_m[100];

  int akt4_jet_n = 0;
  float akt4_jet_pt[1000];
  float akt4_jet_eta[1000];
  float akt4_jet_phi[1000];
  float akt4_jet_e[1000];
  float akt4_jet_m[1000];

  int part_n = 0;
  float part_pt[10000];
  float part_eta[10000];
  float part_y[10000];
  float part_phi[10000];
  float part_e[10000];
  float part_m[10000];

  float fcal_et_negEta_tag = 0;
  float fcal_et_posEta_tag = 0;
  float fcal_et_negEta_mix = 0;
  float fcal_et_posEta_mix = 0;

  int ncoll_tag = 0;
  int ncoll_mix = 0;


  const int nEvents = tagTree->GetEntries ();
  const int nMix = mixTree->GetEntries ();
  double sumWgtsEvents = 0, sumWgtsSqEvents = 0;

  double nJetEvents = 0;
  double sumWgtsJetEvents = 0, sumWgtsSqJetEvents = 0;

  //tagTree->SetBranchAddress ("code",        &code);
  //tagTree->SetBranchAddress ("id1",         &id1);
  //tagTree->SetBranchAddress ("id2",         &id2);
  //tagTree->SetBranchAddress ("x1pdf",       &x1pdf);
  //tagTree->SetBranchAddress ("x2pdf",       &x2pdf);
  //tagTree->SetBranchAddress ("Q",           &Q);
  //tagTree->SetBranchAddress ("isValence1",  &isValence1);
  //tagTree->SetBranchAddress ("isValence2",  &isValence2);

  //tagTree->SetBranchAddress ("akt2_jet_n",    &akt2_jet_n);
  //tagTree->SetBranchAddress ("akt2_jet_pt",   &akt2_jet_pt);
  //tagTree->SetBranchAddress ("akt2_jet_eta",  &akt2_jet_eta);
  //tagTree->SetBranchAddress ("akt2_jet_phi",  &akt2_jet_phi);
  //tagTree->SetBranchAddress ("akt2_jet_e",    &akt2_jet_e);
  //tagTree->SetBranchAddress ("akt2_jet_m",    &akt2_jet_m);

  tagTree->SetBranchAddress ("akt4_jet_n",    &akt4_jet_n);
  tagTree->SetBranchAddress ("akt4_jet_pt",   &akt4_jet_pt);
  tagTree->SetBranchAddress ("akt4_jet_eta",  &akt4_jet_eta);
  tagTree->SetBranchAddress ("akt4_jet_phi",  &akt4_jet_phi);
  tagTree->SetBranchAddress ("akt4_jet_e",    &akt4_jet_e);
  tagTree->SetBranchAddress ("akt4_jet_m",    &akt4_jet_m);

  tagTree->SetBranchAddress ("fcal_et_negEta",  &fcal_et_negEta_tag);
  tagTree->SetBranchAddress ("fcal_et_posEta",  &fcal_et_posEta_tag);

  tagTree->SetBranchAddress ("ncoll",           &ncoll_tag);


  mixTree->SetBranchAddress ("part_n",    &part_n);
  mixTree->SetBranchAddress ("part_pt",   &part_pt);
  mixTree->SetBranchAddress ("part_eta",  &part_eta);
  mixTree->SetBranchAddress ("part_y",    &part_y);
  mixTree->SetBranchAddress ("part_phi",  &part_phi);
  mixTree->SetBranchAddress ("part_e",    &part_e);
  mixTree->SetBranchAddress ("part_m",    &part_m);

  mixTree->SetBranchAddress ("fcal_et_negEta",  &fcal_et_negEta_mix);
  mixTree->SetBranchAddress ("fcal_et_posEta",  &fcal_et_posEta_mix);
  mixTree->SetBranchAddress ("ncoll",           &ncoll_mix);


  TH1D* h_trk_pt_ns_yield;
  TH2D* h2_trk_pt_ns_cov;
  TH1D* h_trk_pt_as_yield;
  TH2D* h2_trk_pt_as_cov;

  TH1D* h_trk_dphi_pt_gt2_yield;
  TH2D* h2_trk_dphi_pt_gt2_cov;
  TH1D* h_trk_dphi_pt_lt2_yield;
  TH2D* h2_trk_dphi_pt_lt2_cov;

  double* trk_pt_ns_counts = new double[nPthBins];
  double* trk_pt_as_counts = new double[nPthBins];
  double* trk_dphi_pt_gt2_counts = new double[nDPhiBins];
  double* trk_dphi_pt_lt2_counts = new double[nDPhiBins];

  for (int i = 0; i < nPthBins; i++) {
    trk_pt_ns_counts[i] = 0;
    trk_pt_as_counts[i] = 0;
  }
  for (int i = 0; i < nDPhiBins; i++) {
    trk_dphi_pt_gt2_counts[i] = 0;
    trk_dphi_pt_lt2_counts[i] = 0;
  }


  TFile* outFile = new TFile (outFileName.c_str (), "recreate");

  h_trk_pt_ns_yield = new TH1D (Form ("h_trk_pt_ns_yield_%s", name.c_str ()), ";#it{p}_{T}^{ch} [GeV]", nPthBins, pthBins);
  h_trk_pt_ns_yield->Sumw2 ();
  h2_trk_pt_ns_cov = new TH2D (Form ("h2_trk_pt_ns_cov_%s", name.c_str ()), ";#it{p}_{T}^{ch} [GeV];#it{p}_{T}^{ch} [GeV]", nPthBins, pthBins, nPthBins, pthBins);
  h2_trk_pt_ns_cov->Sumw2 ();
  h_trk_pt_as_yield = new TH1D (Form ("h_trk_pt_as_yield_%s", name.c_str ()), ";#it{p}_{T}^{ch} [GeV]", nPthBins, pthBins);
  h_trk_pt_as_yield->Sumw2 ();
  h2_trk_pt_as_cov = new TH2D (Form ("h2_trk_pt_as_cov_%s", name.c_str ()), ";#it{p}_{T}^{ch} [GeV];#it{p}_{T}^{ch} [GeV]", nPthBins, pthBins, nPthBins, pthBins);
  h2_trk_pt_as_cov->Sumw2 ();

  h_trk_dphi_pt_gt2_yield = new TH1D (Form ("h_trk_dphi_pt_gt2_yield_%s", name.c_str ()), ";#Delta#phi_{ch, jet}", nDPhiBins, dPhiBins);
  h_trk_dphi_pt_gt2_yield->Sumw2 ();
  h2_trk_dphi_pt_gt2_cov = new TH2D (Form ("h2_trk_dphi_pt_gt2_cov_%s", name.c_str ()), ";#Delta#phi_{ch, jet}", nDPhiBins, dPhiBins, nDPhiBins, dPhiBins);
  h2_trk_dphi_pt_gt2_cov->Sumw2 ();
  h_trk_dphi_pt_lt2_yield = new TH1D (Form ("h_trk_dphi_pt_lt2_yield_%s", name.c_str ()), ";#Delta#phi_{ch, jet}", nDPhiBins, dPhiBins);
  h_trk_dphi_pt_lt2_yield->Sumw2 ();
  h2_trk_dphi_pt_lt2_cov = new TH2D (Form ("h2_trk_dphi_pt_lt2_cov_%s", name.c_str ()), ";#Delta#phi_{ch, jet}", nDPhiBins, dPhiBins, nDPhiBins, dPhiBins);
  h2_trk_dphi_pt_lt2_cov->Sumw2 ();


  //const float jetr = 0.4;
  int iMix = 0;
  for (int iEvent = 0; iEvent < nEvents; iEvent++) {
    if (nEvents > 100 && iEvent % (nEvents / 100) == 0)
      cout << iEvent / (nEvents / 100) << "\% done...\r" << flush;

    tagTree->GetEntry (iEvent);


    // find an appropriate mixed event by matching the centrality bins
    const int iTagCent = (ispPb ? GetpPbBin (fcal_et_posEta_tag) : GetppBin (fcal_et_negEta_tag + fcal_et_posEta_tag));
    const int iMixOld = iMix; // edge case: if we've looped over all events, then we will never find a good match. So we need the value of iMix before looping. This guarantees something "sensible", but not 100% correct.

    bool isGoodEvent = false;
    do {
      iMix = (iMix + 1) % nMix;
      mixTree->GetEntry (iMix);
      // require centrality bins are the same. Bins are defined differently for HS trigger vs. minimum bias due to autocorrelation effect.
      //isGoodEvent = (iTagCent == (ispPb ? GetpPbBkgBin (fcal_et_posEta_mix) : GetppBkgBin (fcal_et_negEta_mix + fcal_et_posEta_mix)));
      isGoodEvent = (ispPb ? (ncoll_tag - ncoll_mix)/ncoll_tag < 0.05 : iTagCent == GetppBkgBin (fcal_et_negEta_mix + fcal_et_posEta_mix));
    } while (iMix != iMixOld && !isGoodEvent);
    if (!isGoodEvent) {
      std::cout << "No good event --> skipping event! iTagCent = " << iTagCent << std::endl;
      continue;
    } // else we have a good mixed event match so proceed to histogram filling


    const double ewgt = 1; // no weights 
    sumWgtsEvents += ewgt;
    sumWgtsSqEvents += ewgt*ewgt;

    for (int iJ = 0; iJ < akt4_jet_n; iJ++) {
      const float jpt = akt4_jet_pt[iJ];
      const float jeta = akt4_jet_eta[iJ];
      const float jphi = akt4_jet_phi[iJ];

      if (fabs (jeta) > 2.8)
        continue;

      if (jpt < 60)
        continue;

      double jwgt = 1;
      //if (boost == 0.465 && jeta > 1.1 && jeta < 3.6) {
      //  if (InDisabledHEC (jeta, jphi, jetr)) continue;
      //  else jwgt = 2.*M_PI / (3.*M_PI/2. - 2*jetr);
      //}
     
      // now loop over the particles in the recorded event
      for (int iPart = 0; iPart < part_n; iPart++) {

        const float trk_pt = part_pt[iPart];
        const float trk_eta = part_eta[iPart];
        const float dphi = DeltaPhi (part_phi[iPart], jphi);

        if (fabs (part_eta[iPart]) > 2.5)
          continue;

        if (std::fabs (trk_eta - boost) > 2.5-0.465)
          continue;

        for (short i = 0; i < nDPhiBins; i++) {
          if (dPhiBins[i] <= dphi && dphi < dPhiBins[i+1]) {
            if (trk_pt > 2)
              trk_dphi_pt_gt2_counts[i] += 1.;
            else if (trk_pt > 0.5)
              trk_dphi_pt_lt2_counts[i] += 1.;
            break;
          }
        }


        for (short i = 0; i < nPthBins; i++) {
          if (pthBins[i] <= trk_pt && trk_pt < pthBins[i+1]) {
            if (dphi >= 7.*M_PI/8.)
              trk_pt_ns_counts[i] += 1.;
            else if (dphi < M_PI/8.)
              trk_pt_as_counts[i] += 1.;
            break;
          }
        }
      } // end loop over iPart

      nJetEvents += jwgt;
      sumWgtsJetEvents += ewgt*jwgt;
      sumWgtsSqJetEvents += ewgt*ewgt*jwgt;

      TH1D* h = nullptr;
      TH2D* h2 = nullptr;
      double* arr = nullptr;

      h = h_trk_pt_ns_yield;
      h2 = h2_trk_pt_ns_cov;
      arr = trk_pt_ns_counts;
      for (short iX = 0; iX < nPthBins; iX++) {
        h->SetBinContent (iX+1, h->GetBinContent (iX+1) + (ewgt * jwgt) * arr[iX]);
        for (short iY = 0; iY < nPthBins; iY++)
          h2->SetBinContent (iX+1, iY+1, h2->GetBinContent (iX+1, iY+1) + (ewgt * jwgt) * (arr[iX])*(arr[iY]));
      }

      h = h_trk_pt_as_yield;
      h2 = h2_trk_pt_as_cov;
      arr = trk_pt_as_counts;
      for (short iX = 0; iX < nPthBins; iX++) {
        h->SetBinContent (iX+1, h->GetBinContent (iX+1) + (ewgt * jwgt) * arr[iX]);
        for (short iY = 0; iY < nPthBins; iY++)
          h2->SetBinContent (iX+1, iY+1, h2->GetBinContent (iX+1, iY+1) + (ewgt * jwgt) * (arr[iX])*(arr[iY]));
      }

      h = h_trk_dphi_pt_gt2_yield;
      h2 = h2_trk_dphi_pt_gt2_cov;
      arr = trk_dphi_pt_gt2_counts;
      for (short iX = 0; iX < nDPhiBins; iX++) {
        h->SetBinContent (iX+1, h->GetBinContent (iX+1) + (ewgt * jwgt) * arr[iX]);
        for (short iY = 0; iY < nDPhiBins; iY++)
          h2->SetBinContent (iX+1, iY+1, h2->GetBinContent (iX+1, iY+1) +(ewgt * jwgt) *  (arr[iX])*(arr[iY]));
      }

      h = h_trk_dphi_pt_lt2_yield;
      h2 = h2_trk_dphi_pt_lt2_cov;
      arr = trk_dphi_pt_lt2_counts;
      for (short iX = 0; iX < nDPhiBins; iX++) {
        h->SetBinContent (iX+1, h->GetBinContent (iX+1) + (ewgt * jwgt) * arr[iX]);
        for (short iY = 0; iY < nDPhiBins; iY++)
          h2->SetBinContent (iX+1, iY+1, h2->GetBinContent (iX+1, iY+1) + (ewgt * jwgt) * (arr[iX])*(arr[iY]));
      }

      for (int i = 0; i < nPthBins; i++) {
        trk_pt_ns_counts[i] = 0;
        trk_pt_as_counts[i] = 0;
      }
      for (int i = 0; i < nDPhiBins; i++) {
        trk_dphi_pt_gt2_counts[i] = 0;
        trk_dphi_pt_lt2_counts[i] = 0;
      }

    } // end loop over jets

  } // end loop over iEvent



  std::cout << "nJetEvents =         " << nJetEvents << std::endl;
  std::cout << "sumWgtsJetEvents =   " << sumWgtsJetEvents << std::endl;
  std::cout << "sumWgtsSqJetEvents = " << sumWgtsSqJetEvents << std::endl;

  {
    TH2D* h2 = nullptr;
    TH1D* h = nullptr;

    h2 = h2_trk_pt_ns_cov;
    h = h_trk_pt_ns_yield;
    ScaleHist (h, 8./M_PI, true);
    ScaleHist (h2, std::pow (8./M_PI, 2.), true);
    for (int iX = 1; iX <= h2->GetNbinsX (); iX++)
      for (int iY = 1; iY <= h2->GetNbinsY (); iY++)
        h2->SetBinContent (iX, iY, h2->GetBinContent (iX, iY) - (h->GetBinContent (iX))*(h->GetBinContent (iY))/sumWgtsJetEvents);
    ScaleHist (h, 1./sumWgtsJetEvents, false);
    ScaleHist (h2, sumWgtsJetEvents / (nJetEvents * (sumWgtsJetEvents*sumWgtsJetEvents - sumWgtsSqJetEvents)), false);
    SetVariances (h, h2);


    h2 = h2_trk_pt_as_cov;
    h = h_trk_pt_as_yield;
    ScaleHist (h, 8./M_PI, true);
    ScaleHist (h2, std::pow (8./M_PI, 2.), true);
    for (int iX = 1; iX <= h2->GetNbinsX (); iX++)
      for (int iY = 1; iY <= h2->GetNbinsY (); iY++)
        h2->SetBinContent (iX, iY, h2->GetBinContent (iX, iY) - (h->GetBinContent (iX))*(h->GetBinContent (iY))/sumWgtsJetEvents);
    ScaleHist (h, 1./sumWgtsJetEvents, false);
    ScaleHist (h2, sumWgtsJetEvents / (nJetEvents * (sumWgtsJetEvents*sumWgtsJetEvents - sumWgtsSqJetEvents)), false);
    SetVariances (h, h2);


    h2 = h2_trk_dphi_pt_gt2_cov;
    h = h_trk_dphi_pt_gt2_yield;
    for (int iX = 1; iX <= h2->GetNbinsX (); iX++)
      for (int iY = 1; iY <= h2->GetNbinsY (); iY++)
        h2->SetBinContent (iX, iY, h2->GetBinContent (iX, iY) - (h->GetBinContent (iX))*(h->GetBinContent (iY))/sumWgtsJetEvents);
    ScaleHist (h, 1./sumWgtsJetEvents, true);
    ScaleHist (h2, sumWgtsJetEvents / (nJetEvents * (sumWgtsJetEvents*sumWgtsJetEvents - sumWgtsSqJetEvents)), true);
    SetVariances (h, h2);


    h2 = h2_trk_dphi_pt_lt2_cov;
    h = h_trk_dphi_pt_lt2_yield;
    for (int iX = 1; iX <= h2->GetNbinsX (); iX++)
      for (int iY = 1; iY <= h2->GetNbinsY (); iY++)
        h2->SetBinContent (iX, iY, h2->GetBinContent (iX, iY) - (h->GetBinContent (iX))*(h->GetBinContent (iY))/sumWgtsEvents);
    ScaleHist (h, 1./sumWgtsEvents, true);
    ScaleHist (h2, sumWgtsEvents / (nEvents * (sumWgtsEvents*sumWgtsEvents - sumWgtsSqEvents)), true);
    SetVariances (h, h2);

  }

  delete[] trk_pt_ns_counts;
  trk_pt_ns_counts = nullptr;
  delete[] trk_pt_as_counts;
  trk_pt_as_counts = nullptr;
  delete[] trk_dphi_pt_gt2_counts;
  trk_dphi_pt_gt2_counts = nullptr;
  delete[] trk_dphi_pt_lt2_counts;
  trk_dphi_pt_lt2_counts = nullptr;

  outFile->cd ();

  // now save histograms to a rootfile
  h_trk_pt_ns_yield->Write ();
  h2_trk_pt_ns_cov->Write ();
  h_trk_pt_as_yield->Write ();
  h2_trk_pt_as_cov->Write ();
  h_trk_dphi_pt_gt2_yield->Write ();
  h2_trk_dphi_pt_gt2_cov->Write ();
  h_trk_dphi_pt_lt2_yield->Write ();
  h2_trk_dphi_pt_lt2_cov->Write ();
  
  outFile->Close ();

  return 0;
}

#endif
