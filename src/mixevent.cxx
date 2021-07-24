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

#include <Utilities.h>
#include <ArrayTemplates.h>

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


  const int nEvents = tagTree->GetEntries () / 4; // only use 25% of events for debugging
  const int nMix = mixTree->GetEntries ();

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

  if (ispPb)
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
  if (ispPb)
    mixTree->SetBranchAddress ("ncoll",           &ncoll_mix);

  const short nCentBins = (ispPb ? npPbNcollBins : 1);

  int** trk_pt_counts = Get2DArray <int> (3, nPthBins);
  int** trk_dphi_counts = Get2DArray <int> (nRedPthBins, nDPhiBins);

  int* n_evts = Get1DArray <int> (nCentBins); // number of events used at each centrallity

  for (int iDPhi = 0; iDPhi < 3; iDPhi++)
    for (int iPth = 0; iPth < nPthBins; iPth++)
      trk_pt_counts[iDPhi][iPth] = 0;

  for (int iRedPth = 0; iRedPth < nRedPthBins; iRedPth++)
    for (int iDPhi = 0; iDPhi < nDPhiBins; iDPhi++)
      trk_dphi_counts[iRedPth][iDPhi] = 0;


  TFile* outFile = new TFile (outFileName.c_str (), "recreate");

  TH1D*** h_trk_pt_yield = Get2DArray <TH1D*> (3, nCentBins); // near-side, perpendicular, and away-side
  TH2D*** h2_trk_pt_cov  = Get2DArray <TH2D*> (3, nCentBins);

  TH1D*** h_trk_dphi_yield = Get2DArray <TH1D*> (nRedPthBins, nCentBins); // many different pT bins
  TH2D*** h2_trk_dphi_cov  = Get2DArray <TH2D*> (nRedPthBins, nCentBins);

  for (int iCent = 0; iCent < nCentBins; iCent++) {

    const std::string centStr = (nCentBins == 1 ? "" : Form ("cent%i_", iCent));
 
    for (int iDPhi = 0; iDPhi < 3; iDPhi++) {
      const std::string dphiStr = (iDPhi == 0 ? "ns" : (iDPhi == 1 ? "perp" : "as"));

      h_trk_pt_yield[iDPhi][iCent] = new TH1D (Form ("h_trk_pt_%s_yield_%s%s", dphiStr.c_str (), centStr.c_str (), name.c_str ()), ";#it{p}_{T}^{ch} [GeV];", nPthBins, pthBins);
      h_trk_pt_yield[iDPhi][iCent]->Sumw2 ();
      h2_trk_pt_cov[iDPhi][iCent] = new TH2D (Form ("h2_trk_pt_%s_cov_%s%s", dphiStr.c_str (), centStr.c_str (), name.c_str ()), ";#it{p}_{T}^{ch} [GeV];#it{p}_{T}^{ch} [GeV]", nPthBins, pthBins, nPthBins, pthBins);
      h2_trk_pt_cov[iDPhi][iCent]->Sumw2 ();
    }

    for (int iPth = 0; iPth < nRedPthBins; iPth++) {
      const std::string pthStr = GetRedPthStr (iPth);

      h_trk_dphi_yield[iPth][iCent] = new TH1D (Form ("h_trk_dphi_%s_yield_%s%s", pthStr.c_str (), centStr.c_str (), name.c_str ()), ";#Delta#phi_{ch, jet};", nDPhiBins, dPhiBins);
      h_trk_dphi_yield[iPth][iCent]->Sumw2 ();
      h2_trk_dphi_cov[iPth][iCent] = new TH2D (Form ("h2_trk_dphi_%s_cov_%s%s", pthStr.c_str (), centStr.c_str (), name.c_str ()), ";#Delta#phi_{ch, jet};", nDPhiBins, dPhiBins, nDPhiBins, dPhiBins);
      h2_trk_dphi_cov[iPth][iCent]->Sumw2 ();
    }

  }


  //int iMinNcoll = 0;
  //while (iMinNcoll < npPbNcollBins && pPbNcollPercs[iMinNcoll++] != 20);
  //const double minNcoll = pPbNcollCuts[iMinNcoll];

  //const float jetr = 0.4;
  int iMix = 0;
  for (int iEvent = 0; iEvent < nEvents; iEvent++) {
    if (nEvents > 100 && iEvent % (nEvents / 100) == 0)
      cout << iEvent / (nEvents / 100) << "\% done...\r" << flush;

    tagTree->GetEntry (iEvent);

    const short iCent = (ispPb ? GetBin (ncoll_tag, pPbBkgNcollCuts, npPbNcollBins) : 0);
    //if (ispPb && ncoll_tag < minNcoll)
    //  continue;

    // find an appropriate mixed event by matching the centrality bins
    const int iTagCent = (ispPb ? GetBin (fcal_et_posEta_tag, pPbBkgCuts, npPbMixBins) : GetBin (fcal_et_negEta_tag + fcal_et_posEta_tag, ppBkgCuts, nppMixBins));
    //const int iTagCent = (ispPb ? GetBin (fcal_et_posEta_tag, pPbBkgCuts, npPbMixBins) : GetBin (fcal_et_negEta_tag + fcal_et_posEta_tag, ppBkgCuts, nppMixBins));
    const int iMixOld = iMix; // edge case: if we've looped over all events, then we will never find a good match. So we need the value of iMix before looping. This guarantees something "sensible", but not 100% correct.

    bool isGoodEvent = false;
    do {
      iMix = (iMix + 1) % nMix;
      mixTree->GetEntry (iMix);
      // require centrality bins are the same.
      isGoodEvent = (iTagCent == (ispPb ? GetBin (fcal_et_posEta_mix, pPbBkgCuts, npPbMixBins) : GetBin (fcal_et_negEta_mix + fcal_et_posEta_mix, ppBkgCuts, nppMixBins)));
      //isGoodEvent = (ispPb ? (ncoll_tag - ncoll_mix)/ncoll_tag < 0.05 : iTagCent == GetBin (fcal_et_negEta_mix + fcal_et_posEta_mix, ppBkgCuts, nppMixBins));
      //if (ispPb) isGoodEvent = isGoodEvent && (iCent == GetBin (ncoll_mix, pPbBkgNcollCuts, npPbNcollBins));
    } while (iMix != iMixOld && !isGoodEvent);
    if (!isGoodEvent) {
      std::cout << "No good event --> skipping event! iTagCent = " << iTagCent << std::endl;
      continue;
    } // else we have a good mixed event match so proceed to histogram filling


    int jetCount = 0;

    for (int iJ = 0; iJ < akt4_jet_n; iJ++) {
      const float jpt = akt4_jet_pt[iJ];
      const float jeta = akt4_jet_eta[iJ];
      const float jphi = akt4_jet_phi[iJ];

      if (std::fabs (jeta) > 2.8)
        continue;

      if (jpt < 60)
        continue;

      jetCount++;

      // now loop over the particles in the recorded event
      for (int iPart = 0; iPart < part_n; iPart++) {

        const float trk_pt = part_pt[iPart];
        const float trk_eta = part_eta[iPart];
        const float dphi = DeltaPhi (part_phi[iPart], jphi);

        if (std::fabs (part_eta[iPart]) > 2.5)
          continue;

        if (std::fabs (trk_eta - boost) > 2.035) // 2.035 = 2.5 - 0.465
          continue;

        for (short iDPhi = 0; iDPhi < nDPhiBins; iDPhi++) {
          if (dPhiBins[iDPhi] <= dphi && dphi < dPhiBins[iDPhi+1]) {
            for (short iRedPth = 0; iRedPth < nRedPthBins; iRedPth++) {
              if (redPthBins[iRedPth] <= trk_pt && trk_pt < redPthBins[iRedPth+1]) {
                trk_dphi_counts[iRedPth][iDPhi]++;
                break;
              }
            }
            break;
          }
        }

        for (short iPth = 0; iPth < nPthBins; iPth++) {
          if (pthBins[iPth] <= trk_pt && trk_pt < pthBins[iPth+1]) {
            if (dphi >= 7.*M_PI/8.)
              trk_pt_counts[0][iPth]++;
            else if (dphi < M_PI/8.)
              trk_pt_counts[2][iPth]++;
            else if (M_PI/3. < dphi && dphi < 2.*M_PI/3.)
              trk_pt_counts[1][iPth]++;
            break;
          }
        }
      } // end loop over iPart

    } // end loop over jets


    TH1D* h = nullptr;
    TH2D* h2 = nullptr;
    int* arr = nullptr;

    if (jetCount > 0) {
      n_evts[iCent]++;

      for (int iDPhi = 0; iDPhi < 3; iDPhi++) {
        h = h_trk_pt_yield[iDPhi][iCent];
        h2 = h2_trk_pt_cov[iDPhi][iCent];
        arr = trk_pt_counts[iDPhi];
        for (short iX = 0; iX < h->GetNbinsX (); iX++) {
          h->SetBinContent (iX+1, h->GetBinContent (iX+1) + ((double)arr[iX]) / jetCount);
          for (short iY = 0; iY < h->GetNbinsX (); iY++)
            h2->SetBinContent (iX+1, iY+1, h2->GetBinContent (iX+1, iY+1) + ((double)(arr[iX])*(arr[iY]))/(jetCount*jetCount));
        }
      }
  
      for (int iRedPth = 0; iRedPth < nRedPthBins; iRedPth++) {
        h = h_trk_dphi_yield[iRedPth][iCent];
        h2 = h2_trk_dphi_cov[iRedPth][iCent];
        arr = trk_dphi_counts[iRedPth];
        for (short iX = 0; iX < h->GetNbinsX (); iX++) {
          h->SetBinContent (iX+1, h->GetBinContent (iX+1) + ((double)arr[iX]));
          for (short iY = 0; iY < h->GetNbinsX (); iY++)
            h2->SetBinContent (iX+1, iY+1, h2->GetBinContent (iX+1, iY+1) + ((double)(arr[iX])*(arr[iY]))/(jetCount*jetCount));
        }
      }

      for (int iDPhi = 0; iDPhi < 3; iDPhi++)
        for (int iPth = 0; iPth < nPthBins; iPth++)
          trk_pt_counts[iDPhi][iPth] = 0;
  
      for (int iRedPth = 0; iRedPth < nRedPthBins; iRedPth++)
        for (int iDPhi = 0; iDPhi < nDPhiBins; iDPhi++)
          trk_dphi_counts[iRedPth][iDPhi] = 0;
    }

  } // end loop over iEvent

  std::cout << std::endl;



  std::cout << "n_evts = ";
  for (int iCent = 0; iCent < nCentBins-1; iCent++)  std::cout << n_evts[iCent] << ", " << std::endl;
  std::cout << n_evts[nCentBins-1] << std::endl;

  {
    TH2D* h2 = nullptr;
    TH1D* h = nullptr;

    for (int iCent = 0; iCent < nCentBins; iCent++) {

      const float n_evt = (float) n_evts[iCent];

      for (int iDPhi = 0; iDPhi < 3; iDPhi++) {
        h2 = h2_trk_pt_cov[iDPhi][iCent];
        h = h_trk_pt_yield[iDPhi][iCent];
        ScaleHist (h, iDPhi % 2 == 0 ? 8./M_PI : 3./M_PI, true);
        ScaleHist (h2, std::pow (iDPhi % 2 == 0  ? 8./M_PI : 3./M_PI, 2.), true);
        for (int iX = 1; iX <= h2->GetNbinsX (); iX++)
          for (int iY = 1; iY <= h2->GetNbinsY (); iY++)
            h2->SetBinContent (iX, iY, h2->GetBinContent (iX, iY) - (h->GetBinContent (iX))*(h->GetBinContent (iY))/n_evt);
        ScaleHist (h, 1./n_evt, false);
        ScaleHist (h2, 1./(n_evt*(n_evt-1)), false);
        SetVariances (h, h2);
      }


      for (int iRedPth = 0; iRedPth < nRedPthBins; iRedPth++) {
        h2 = h2_trk_dphi_cov[iRedPth][iCent];
        h = h_trk_dphi_yield[iRedPth][iCent];
        for (int iX = 1; iX <= h2->GetNbinsX (); iX++)
          for (int iY = 1; iY <= h2->GetNbinsY (); iY++)
            h2->SetBinContent (iX, iY, h2->GetBinContent (iX, iY) - (h->GetBinContent (iX))*(h->GetBinContent (iY))/n_evt);
        ScaleHist (h, 1./n_evt, true);
        ScaleHist (h2, 1./(n_evt*(n_evt-1)), true);
        SetVariances (h, h2);
      }
    }

  }



  // now save histograms to a rootfile
  outFile->cd ();

  for (int iCent = 0; iCent < nCentBins; iCent++) {
    for (int iDPhi = 0; iDPhi < 3; iDPhi++) {
      h_trk_pt_yield[iDPhi][iCent]->Write ();
      h2_trk_pt_cov[iDPhi][iCent]->Write ();
    }
    for (int iRedPth = 0; iRedPth < nRedPthBins; iRedPth++) {
      h_trk_dphi_yield[iRedPth][iCent]->Write ();
      h2_trk_dphi_cov[iRedPth][iCent]->Write ();
    }
  }
  
  outFile->Close ();

  return 0;
}

#endif
