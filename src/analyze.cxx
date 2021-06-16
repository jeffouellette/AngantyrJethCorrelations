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

using namespace PythiaAngantyrStudy;


int main (int argc, char** argv) {

  if (argc < 4) {
    std::cout << " usage: analyze NAME INFILEPATTERN OUTFILENAME" << std::endl;
    return 0;
  }

  const string name = std::string (argv[1]);
  const string inFilePattern = std::string (argv[2]);
  const string outFileName = std::string (argv[3]);

  const float boost = (name.find ("pPb") != std::string::npos ? -0.465 : 0);

  std::cout << "boost = " << boost << std::endl;

  std::cout << "InFilePattern = " << inFilePattern << std::endl;

  TChain* inTree = new TChain ("tree", "tree");
  inTree->Add (inFilePattern.c_str ());

  std::cout << "Added " << inTree->GetListOfFiles ()->GetEntries () << " files to TChain, with " << inTree->GetEntries () << " events" << std::endl;

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

  const int nEvents = inTree->GetEntries ();
  double sumWgtsEvents = 0, sumWgtsSqEvents = 0;
  double nJetEvents = 0;
  double sumWgtsJetEvents = 0, sumWgtsSqJetEvents = 0;

  //inTree->SetBranchAddress ("code",       &code);
  //inTree->SetBranchAddress ("id1",        &id1);
  //inTree->SetBranchAddress ("id2",        &id2);
  //inTree->SetBranchAddress ("x1pdf",      &x1pdf);
  //inTree->SetBranchAddress ("x2pdf",      &x2pdf);
  //inTree->SetBranchAddress ("Q",          &Q);
  //inTree->SetBranchAddress ("isValence1", &isValence1);
  //inTree->SetBranchAddress ("isValence2", &isValence2);

  //inTree->SetBranchAddress ("akt2_jet_n",   &akt2_jet_n);
  //inTree->SetBranchAddress ("akt2_jet_pt",  &akt2_jet_pt);
  //inTree->SetBranchAddress ("akt2_jet_eta", &akt2_jet_eta);
  //inTree->SetBranchAddress ("akt2_jet_phi", &akt2_jet_phi);
  //inTree->SetBranchAddress ("akt2_jet_e",   &akt2_jet_e);
  //inTree->SetBranchAddress ("akt2_jet_m",   &akt2_jet_m);

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


  TH1D* h_trk_pt_ns_yield = nullptr;
  TH2D* h2_trk_pt_ns_cov = nullptr;
  TH1D* h_trk_pt_as_yield = nullptr;
  TH2D* h2_trk_pt_as_cov = nullptr;

  TH1D* h_trk_dphi_pt_gt2_yield = nullptr;
  TH2D* h2_trk_dphi_pt_gt2_cov = nullptr;
  TH1D* h_trk_dphi_pt_lt2_yield = nullptr;
  TH2D* h2_trk_dphi_pt_lt2_cov = nullptr;

  TH1D* h_jet_pt_yield = nullptr;
  TH2D* h2_jet_pt_cov = nullptr;

  TH1D* h_jet_yield = nullptr;

  double* trk_pt_ns_counts = new double[nPthBins];
  double* trk_pt_as_counts = new double[nPthBins];
  double* trk_dphi_pt_gt2_counts = new double[nDPhiBins];
  double* trk_dphi_pt_lt2_counts = new double[nDPhiBins];
  double* jet_ptj_counts = new double[nPtJBins];

  for (int i = 0; i < nPthBins; i++) {
    trk_pt_ns_counts[i] = 0;
    trk_pt_as_counts[i] = 0;
  }
  for (int i = 0; i < nDPhiBins; i++) {
    trk_dphi_pt_gt2_counts[i] = 0;
    trk_dphi_pt_lt2_counts[i] = 0;
  }
  for (int i = 0; i < nPtJBins; i++) {
    jet_ptj_counts[i] = 0;
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

      if (std::fabs (jeta) > 2.8)
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

      double jwgt = 1.;
      //if (boost == 0.465 && jeta > 1.1 && jeta < 3.6) {
      //  if (InDisabledHEC (jeta, jphi, jetr)) continue;
      //  else jwgt = 2.*M_PI / (3.*M_PI/2. - 2*jetr);
      //}
     
      // now loop over the particles in the recorded event
      for (int iPart = 0; iPart < part_n; iPart++) {

        const float trk_pt = part_pt[iPart];
        const float trk_eta = part_eta[iPart];
        const float dphi = DeltaPhi (part_phi[iPart], jphi);

        if (std::fabs (part_eta[iPart]) > 2.5)
          continue;

        if (std::fabs (trk_eta - boost) > 2.5-0.465)
          continue;

        for (short i = 0; i < nDPhiBins; i++) {
          if (dPhiBins[i] <= dphi && dphi < dPhiBins[i+1]) {
            if (trk_pt > 2)
              trk_dphi_pt_gt2_counts[i] += 1.;
            else if (trk_pt > 0.1)
              trk_dphi_pt_lt2_counts[i] += 1.;
            break;
          }
        }


        for (int i = 0; i < nPthBins; i++) {
          if (pthBins[i] <= trk_pt && trk_pt < pthBins[i+1]) {
            if (dphi >= 7.*M_PI/8.)
              trk_pt_as_counts[i] += 1.;
            else if (dphi < M_PI/8.)
              trk_pt_ns_counts[i] += 1.;
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
      arr = &(trk_pt_ns_counts[0]);
      for (int iX = 0; iX < nPthBins; iX++) {
        h->SetBinContent (iX+1, h->GetBinContent (iX+1) + (ewgt * jwgt) * arr[iX]);
        for (int iY = 0; iY < nPthBins; iY++)
          h2->SetBinContent (iX+1, iY+1, h2->GetBinContent (iX+1, iY+1) + (ewgt * jwgt) * (arr[iX])*(arr[iY]));
      }

      h = h_trk_pt_as_yield;
      h2 = h2_trk_pt_as_cov;
      arr = &(trk_pt_as_counts[0]);
      for (int iX = 0; iX < nPthBins; iX++) {
        h->SetBinContent (iX+1, h->GetBinContent (iX+1) + (ewgt * jwgt) * arr[iX]);
        for (int iY = 0; iY < nPthBins; iY++)
          h2->SetBinContent (iX+1, iY+1, h2->GetBinContent (iX+1, iY+1) + (ewgt * jwgt) * (arr[iX])*(arr[iY]));
      }

      h = h_trk_dphi_pt_gt2_yield;
      h2 = h2_trk_dphi_pt_gt2_cov;
      arr = trk_dphi_pt_gt2_counts;
      for (int iX = 0; iX < nDPhiBins; iX++) {
        h->SetBinContent (iX+1, h->GetBinContent (iX+1) + (ewgt * jwgt) * arr[iX]);
        for (int iY = 0; iY < nDPhiBins; iY++)
          h2->SetBinContent (iX+1, iY+1, h2->GetBinContent (iX+1, iY+1) +(ewgt * jwgt) *  (arr[iX])*(arr[iY]));
      }

      h = h_trk_dphi_pt_lt2_yield;
      h2 = h2_trk_dphi_pt_lt2_cov;
      arr = trk_dphi_pt_lt2_counts;
      for (int iX = 0; iX < nDPhiBins; iX++) {
        h->SetBinContent (iX+1, h->GetBinContent (iX+1) + (ewgt * jwgt) * arr[iX]);
        for (int iY = 0; iY < nDPhiBins; iY++)
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

    for (int iX = 0; iX < nPtJBins; iX++) {
      h_jet_pt_yield->SetBinContent (iX+1, h_jet_pt_yield->GetBinContent (iX+1) + ewgt * jet_ptj_counts[iX]);
      for (int iY = 0; iY < nPtJBins; iY++)
        h2_jet_pt_cov->SetBinContent (iX+1, iY+1, h2_jet_pt_cov->GetBinContent (iX+1, iY+1) + ewgt * (jet_ptj_counts[iX])*(jet_ptj_counts[iY]));
    }

    h_jet_yield->Fill (jetCount);

    for (int i = 0; i < nPtJBins; i++) {
      jet_ptj_counts[i] = 0;
    }

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

  delete[] trk_pt_ns_counts;
  trk_pt_ns_counts = nullptr;
  delete[] trk_pt_as_counts;
  trk_pt_as_counts = nullptr;
  delete[] trk_dphi_pt_gt2_counts;
  trk_dphi_pt_gt2_counts = nullptr;
  delete[] trk_dphi_pt_lt2_counts;
  trk_dphi_pt_lt2_counts = nullptr;
  delete[] jet_ptj_counts;
  jet_ptj_counts = nullptr;

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
