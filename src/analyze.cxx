#ifndef __analyze_cxx__
#define __analyze_cxx__

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

using namespace PythiaAngyntyrStudy;



void ScaleHist (TH1D* h, const double sf, const bool doWidth) {
  const int nb = h->GetNbinsX ();
  for (int iX = 1; iX <= nb; iX++) {
    h->SetBinContent (iX, h->GetBinContent (iX) * sf / (doWidth ? h->GetBinWidth (iX) : 1.));
    h->SetBinError (iX, h->GetBinError (iX) * sf / (doWidth ? h->GetBinWidth (iX) : 1.));
  }
  return;
}



void ScaleHist (TH2D* h, const double sf, const bool doWidth) {
  const int nbx = h->GetNbinsX (); const int nby = h->GetNbinsY ();
  for (int iX = 1; iX <= nbx; iX++) {
    for (int iY = 1; iY <= nby; iY++) {
      h->SetBinContent (iX, iY, h->GetBinContent (iX, iY) * sf / (doWidth ? h->GetXaxis ()->GetBinWidth (iX) * h->GetYaxis ()->GetBinWidth (iY) : 1.));
      h->SetBinError (iX, iY, h->GetBinError (iX, iY) * sf / (doWidth ? h->GetXaxis ()->GetBinWidth (iX) * h->GetYaxis ()->GetBinWidth (iY) : 1.));
    }
  }
  return;
}



double GetMaxRapidity (const double mass, const double pT) {
  // Strategy:
  // for a particular mass of particle, get its rapidity at the edge of the
  // ATLAS tracker (eta = +2.5).
  // That value is the maximum rapidity of that particle you can observe, y_max,
  // across all system boosts.
  // Since the p+Pb center of mass boost is +0.465, you restrict to just 
  // particles with |rapidity - y_system| < y_max - 0.465.

  const double en = std::sqrt (std::pow (mass, 2) + std::pow (pT * std::cosh ((double)2.5), 2));
  const double pz = pT * std::sinh ((double)2.5);

  return 0.5 * std::log ((en + pz) / (en - pz));
}




int main (int argc, char** argv) {

  if (argc < 4) {
    std::cout << " usage: analyze NAME INFILEPATTERN OUTFILENAME" << std::endl;
    return 0;
  }

  const string name = std::string (argv[1]);
  const string inFilePattern = std::string (argv[2]);
  const string outFileName = std::string (argv[3]);

  const float boost = (name.find ("pPb") != std::string::npos ? 0.465 : 0);

  std::cout << "boost = " << boost << std::endl;

  std::cout << "InFilePattern = " << inFilePattern << std::endl;

  TChain* inTree = new TChain ("tree", "tree");
  inTree->Add (inFilePattern.c_str ());

  std::cout << "Added " << inTree->GetListOfFiles ()->GetEntries () << " files to TChain, with " << inTree->GetEntries () << " events" << std::endl;

  int akt2_jet_n = 0;
  float akt2_jet_pt[100];
  float akt2_jet_eta[100];
  float akt2_jet_phi[100];
  float akt2_jet_e[100];
  float akt2_jet_m[100];

  int akt4_jet_n = 0;
  float akt4_jet_pt[100];
  float akt4_jet_eta[100];
  float akt4_jet_phi[100];
  float akt4_jet_e[100];
  float akt4_jet_m[100];

  int part_n = 0;
  float part_pt[10000];
  float part_eta[10000];
  float part_y[10000];
  float part_phi[10000];
  float part_e[10000];
  float part_m[10000];

  const int nEvents = inTree->GetEntries ();
  double sumWgtsEvents = 0, sumWgtsSqEvents = 0;
  int nJetEvents = 0;
  double sumWgtsJetEvents = 0, sumWgtsSqJetEvents = 0;

  //inTree->SetBranchAddress ("code",       &code);
  //inTree->SetBranchAddress ("id1",        &id1);
  //inTree->SetBranchAddress ("id2",        &id2);
  //inTree->SetBranchAddress ("x1pdf",      &x1pdf);
  //inTree->SetBranchAddress ("x2pdf",      &x2pdf);
  //inTree->SetBranchAddress ("Q",          &Q);
  //inTree->SetBranchAddress ("isValence1", &isValence1);
  //inTree->SetBranchAddress ("isValence2", &isValence2);

  inTree->SetBranchAddress ("akt2_jet_n",   &akt2_jet_n);
  inTree->SetBranchAddress ("akt2_jet_pt",  &akt2_jet_pt);
  inTree->SetBranchAddress ("akt2_jet_eta", &akt2_jet_eta);
  inTree->SetBranchAddress ("akt2_jet_phi", &akt2_jet_phi);
  inTree->SetBranchAddress ("akt2_jet_e",   &akt2_jet_e);
  inTree->SetBranchAddress ("akt2_jet_m",   &akt2_jet_m);

  inTree->SetBranchAddress ("akt4_jet_n",   &akt4_jet_n);
  inTree->SetBranchAddress ("akt4_jet_pt",  &akt4_jet_pt);
  inTree->SetBranchAddress ("akt4_jet_eta", &akt4_jet_eta);
  inTree->SetBranchAddress ("akt4_jet_phi", &akt4_jet_phi);
  inTree->SetBranchAddress ("akt4_jet_e",   &akt4_jet_e);
  inTree->SetBranchAddress ("akt4_jet_m",   &akt4_jet_m);

  inTree->SetBranchAddress ("part_n",     &part_n);
  inTree->SetBranchAddress ("part_pt",    &part_pt);
  inTree->SetBranchAddress ("part_eta",   &part_eta);
  inTree->SetBranchAddress ("part_y",     &part_y);
  inTree->SetBranchAddress ("part_phi",   &part_phi);
  inTree->SetBranchAddress ("part_e",     &part_e);
  inTree->SetBranchAddress ("part_m",     &part_m);


  TH1D* h_trk_pt_ns_yield;
  TH2D* h2_trk_pt_ns_cov;
  TH1D* h_trk_pt_as_yield;
  TH2D* h2_trk_pt_as_cov;

  TH1D* h_trk_dphi_pt_gt2_yield;
  TH2D* h2_trk_dphi_pt_gt2_cov;
  TH1D* h_trk_dphi_pt_lt2_yield;
  TH2D* h2_trk_dphi_pt_lt2_cov;

  TH1D* h_jet_pt_yield;
  TH2D* h2_jet_pt_cov;

  TH1D* h_jet_yield;

  const double pthBins[] = {0.1, 0.105, 0.11, 0.12, 0.13, 0.14, 0.15, 0.16, 0.17, 0.18, 0.19, 0.2, 0.225, 0.25, 0.275, 0.3, 0.325, 0.35, 0.375, 0.4, 0.45, 0.5, 0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95, 1, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2, 2.25, 2.5, 2.75, 3, 3.25, 3.5, 3.75, 4, 4.5, 5, 5.5, 6, 7, 8, 9, 10, 11, 12, 14, 16, 18, 20, 25, 30, 35, 40, 45, 50, 55, 60};
  const short nPthBins = sizeof (pthBins) / sizeof (pthBins[0]) - 1;
  const short nDPhiBins = 24;
  const double* dPhiBins = linspace (0, M_PI, nDPhiBins);
  const int nPtJBins = 30;
  const double* pTJBins = logspace (8, 140, nPtJBins);

  double trk_pt_ns_counts[nPthBins] = {};
  double trk_pt_as_counts[nPthBins] = {};
  double trk_dphi_pt_gt2_counts[nDPhiBins] = {};
  double trk_dphi_pt_lt2_counts[nDPhiBins] = {};
  double jet_ptj_counts[nPtJBins] = {};


  TFile* outFile = new TFile (outFileName.c_str (), "recreate");

  h_trk_pt_ns_yield = new TH1D (Form ("h_trk_pt_ns_yield_%s", name.c_str ()), ";#it{p}_{T}^{ch} [GeV]", nPthBins, pthBins);
  h_trk_pt_ns_yield->Sumw2 ();
  h2_trk_pt_ns_cov = new TH2D (Form ("h2_trk_pt_ns_cov_%s", name.c_str ()), ";#it{p}_{T}^{ch} [GeV];#it{p}_{T}^{ch} [GeV]", nPthBins, pthBins, nPthBins, pthBins);
  h2_trk_pt_ns_cov->Sumw2 ();
  h_trk_pt_as_yield = new TH1D (Form ("h_trk_pt_as_yield_%s", name.c_str ()), ";#it{p}_{T}^{ch} [GeV]", nPthBins, pthBins);
  h_trk_pt_as_yield->Sumw2 ();
  h2_trk_pt_as_cov = new TH2D (Form ("h2_trk_pt_as_cov_%s", name.c_str ()), ";#it{p}_{T}^{ch} [GeV];#it{p}_{T}^{ch} [GeV]", nPthBins, pthBins, nPthBins, pthBins);
  h2_trk_pt_as_cov->Sumw2 ();

  h_trk_dphi_pt_gt2_yield = new TH1D (Form ("h_trk_dphi_pt_gt2_yield_%s", name.c_str ()), ";#Delta#phi_{ch,#gamma}", nDPhiBins, dPhiBins);
  h_trk_dphi_pt_gt2_yield->Sumw2 ();
  h2_trk_dphi_pt_gt2_cov = new TH2D (Form ("h2_trk_dphi_pt_gt2_cov_%s", name.c_str ()), ";#Delta#phi_{ch,#gamma}", nDPhiBins, dPhiBins, nDPhiBins, dPhiBins);
  h2_trk_dphi_pt_gt2_cov->Sumw2 ();
  h_trk_dphi_pt_lt2_yield = new TH1D (Form ("h_trk_dphi_pt_lt2_yield_%s", name.c_str ()), ";#Delta#phi_{ch,#gamma}", nDPhiBins, dPhiBins);
  h_trk_dphi_pt_lt2_yield->Sumw2 ();
  h2_trk_dphi_pt_lt2_cov = new TH2D (Form ("h2_trk_dphi_pt_lt2_cov_%s", name.c_str ()), ";#Delta#phi_{ch,#gamma}", nDPhiBins, dPhiBins, nDPhiBins, dPhiBins);
  h2_trk_dphi_pt_lt2_cov->Sumw2 ();

  h_jet_pt_yield = new TH1D (Form ("h_jet_pt_yield_%s", name.c_str ()), ";#it{p}_{T}^{jet} [GeV]", nPtJBins, pTJBins);
  h_jet_pt_yield->Sumw2 ();
  h2_jet_pt_cov = new TH2D (Form ("h2_jet_pt_cov_%s", name.c_str ()), ";#it{p}_{T}^{jet} [GeV];#it{p}_{T}^{jet} [GeV]", nPtJBins, pTJBins, nPtJBins, pTJBins);
  h2_jet_pt_cov->Sumw2 ();

  h_jet_yield = new TH1D (Form ("h_jet_yield_%s", name.c_str ()), "", 9, -0.5, 8.5);
  h_jet_yield->Sumw2 ();


  //const float jetr = 0.4;
  for (int iEvent = 0; iEvent < nEvents; iEvent++) {
    if (nEvents > 100 && iEvent % (nEvents / 100) == 0)
      cout << iEvent / (nEvents / 100) << "\% done...\r" << flush;

    inTree->GetEntry (iEvent);

    const double ewgt = 1; // no weights 
    sumWgtsEvents += ewgt;
    sumWgtsSqEvents += ewgt*ewgt;

    // all-particle jets
    int jetCount = 0;

    for (int iJ = 0; iJ < akt4_jet_n; iJ++) {
      const float jpt = akt4_jet_pt[iJ];
      const float jeta = akt4_jet_eta[iJ];
      const float jphi = akt4_jet_phi[iJ];

      if (fabs (jeta) > 2.8)
        continue;

      for (short i = 0; i < nPtJBins; i++) {
        if (pTJBins[i] <= jpt && jpt < pTJBins[i+1]) {
          jet_ptj_counts[i] += 1.;
          break;
        }
      }

      if (jpt < 60)
        continue;

      jetCount++;

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

    for (short iX = 0; iX < nPtJBins; iX++) {
      h_jet_pt_yield->SetBinContent (iX+1, h_jet_pt_yield->GetBinContent (iX+1) + ewgt * jet_ptj_counts[iX]);
      for (short iY = 0; iY < nPtJBins; iY++)
        h2_jet_pt_cov->SetBinContent (iX+1, iY+1, h2_jet_pt_cov->GetBinContent (iX+1, iY+1) + ewgt * (jet_ptj_counts[iX])*(jet_ptj_counts[iY]));
    }

    h_jet_yield->Fill (jetCount);

    for (int i = 0; i < nPtJBins; i++) {
      jet_ptj_counts[i] = 0;
    }

  } // end loop over iEvent



  // convert all counts to doubles (to avoid integer division issues... yay)
  const double fnEvents = (double) nEvents;
  const double fnJetEvents = (double) nJetEvents;

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


    h2 = h2_jet_pt_cov;
    h = h_jet_pt_yield;
    ScaleHist (h, 1., true);
    ScaleHist (h2, 1., true);
    for (int iX = 1; iX <= h2->GetNbinsX (); iX++)
      for (int iY = 1; iY <= h2->GetNbinsY (); iY++)
        h2->SetBinContent (iX, iY, h2->GetBinContent (iX, iY) - (h->GetBinContent (iX))*(h->GetBinContent (iY))/sumWgtsEvents);
    ScaleHist (h, 1./sumWgtsEvents, false);
    ScaleHist (h2, 1./(sumWgtsEvents * (sumWgtsEvents-1)), false);
    SetVariances (h, h2);

  }

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
  h_jet_pt_yield->Write ();
  h2_jet_pt_cov->Write ();
  h_jet_yield->Write (); 
  
  outFile->Close ();

  return 0;
}

#endif
