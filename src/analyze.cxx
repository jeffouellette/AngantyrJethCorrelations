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

#include <Utilities.h>
#include <ArrayTemplates.h>

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

  const bool ispPb = (name.find ("pPb") != std::string::npos);

  std::cout << "ispPb = " << (ispPb ? "true" : "false") << std::endl;

  const float boost = (ispPb ? -0.465 : 0);

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

  float fcal_et_negEta = 0;
  float fcal_et_posEta = 0;

  int ncoll = 0;

  const int nEvents = inTree->GetEntries () / 4; // only use 25% of events for debugging

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

  inTree->SetBranchAddress ("fcal_et_negEta",  &fcal_et_negEta);
  inTree->SetBranchAddress ("fcal_et_posEta",  &fcal_et_posEta);
  if (ispPb)
    inTree->SetBranchAddress ("ncoll",         &ncoll);

  const short nCentBins = (ispPb ? npPbNcollBins : 0);

  int** trk_pt_counts = Get2DArray <int> (3, nPthBins);
  int** trk_dphi_counts = Get2DArray <int> (nRedPthBins, nDPhiBins);
  int* jet_ptj_counts = Get1DArray <int> (nPtJBins);

  //int* n_evts = Get1DArray <int> (nCentBins); // number of events used at each centrallity
  int* n_jets = Get1DArray <int> (nCentBins + 1); // number of jets sampled at each centrallity

  for (int iDPhi = 0; iDPhi < 3; iDPhi++)
    for (int iPth = 0; iPth < nPthBins; iPth++)
      trk_pt_counts[iDPhi][iPth] = 0;

  for (int iRedPth = 0; iRedPth < nRedPthBins; iRedPth++)
    for (int iDPhi = 0; iDPhi < nDPhiBins; iDPhi++)
      trk_dphi_counts[iRedPth][iDPhi] = 0;

  for (int i = 0; i < nPtJBins; i++)
    jet_ptj_counts[i] = 0;


  TFile* outFile = new TFile (outFileName.c_str (), "recreate");

  TH1D*** h_trk_pt_yield = Get2DArray <TH1D*> (3, nCentBins + 1); // near-side, perpendicular, and away-side
  TH2D*** h2_trk_pt_cov  = Get2DArray <TH2D*> (3, nCentBins + 1);

  TH1D*** h_trk_dphi_yield = Get2DArray <TH1D*> (nRedPthBins, nCentBins + 1); // many different pT bins
  TH2D*** h2_trk_dphi_cov  = Get2DArray <TH2D*> (nRedPthBins, nCentBins + 1);

  TH1D** h_jet_pt_yield = Get1DArray <TH1D*> (nCentBins + 1);
  TH2D** h2_jet_pt_cov  = Get1DArray <TH2D*> (nCentBins + 1);

  TH1D** h_jet_yield = Get1DArray <TH1D*> (nCentBins + 1);


  for (int iCent = 0; iCent < nCentBins + 1; iCent++) {

    const std::string centStr = (nCentBins == 0 ? "" : Form ("cent%i_", iCent));
 
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

    h_jet_pt_yield[iCent] = new TH1D (Form ("h_jet_pt_yield_%s%s", centStr.c_str (), name.c_str ()), ";#it{p}_{T}^{jet} [GeV]", nPtJBins, pTJBins);
    h_jet_pt_yield[iCent]->Sumw2 ();
    h2_jet_pt_cov[iCent] = new TH2D (Form ("h2_jet_pt_cov_%s%s", centStr.c_str (), name.c_str ()), ";#it{p}_{T}^{jet} [GeV];#it{p}_{T}^{jet} [GeV]", nPtJBins, pTJBins, nPtJBins, pTJBins);
    h2_jet_pt_cov[iCent]->Sumw2 ();

    h_jet_yield[iCent] = new TH1D (Form ("h_jet_yield_%s%s", centStr.c_str (), name.c_str ()), "", 9, -0.5, 8.5);
    h_jet_yield[iCent]->Sumw2 ();

  }


  //int iMinNcoll = 0;
  //while (iMinNcoll < npPbNcollBins && pPbNcollPercs[iMinNcoll++] != 20);
  //const double minNcoll = pPbNcollCuts[iMinNcoll];

  //const float jetr = 0.4;
  for (int iEvent = 0; iEvent < nEvents; iEvent++) {
    if (nEvents > 100 && iEvent % (nEvents / 100) == 0)
      cout << iEvent / (nEvents / 100) << "\% done...\r" << flush;

    inTree->GetEntry (iEvent);

    const short iCent = (ispPb ? GetBin (ncoll, pPbBkgNcollCuts, npPbNcollBins) : 0);
    //if (ispPb && ncoll < minNcoll)
    //  continue;

    bool takeEvent = false;
    int jets = 0;
    for (int iJ = 0; iJ < akt4_jet_n; iJ++) {
      const float jpt = akt4_jet_pt[iJ];
      const float jeta = akt4_jet_eta[iJ];
      const float jphi = akt4_jet_phi[iJ];

      if (std::fabs (jeta) > 2.8)
        continue;

      for (short i = 0; i < nPtJBins; i++) {
        if (pTJBins[i] <= jpt && jpt < pTJBins[i+1]) {
          jet_ptj_counts[i]++;
          break;
        }
      }

      if (jpt < 60)
        continue;

      takeEvent = true; 
      n_jets[iCent]++;
      jets++;

      // now loop over the particles in the recorded event
      for (int iPart = 0; iPart < part_n; iPart++) {

        const float trk_pt = part_pt[iPart];
        const float trk_eta = part_eta[iPart];
        const float dphi = DeltaPhi (part_phi[iPart], jphi);

        if (std::fabs (part_eta[iPart]) > 2.5)
          continue;

        if (std::fabs (trk_eta - boost) > 2.5-0.465)
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
            if (dphi <= M_PI/8.)
              trk_pt_counts[0][iPth]++;
            else if (M_PI/3. < dphi && dphi < 2.*M_PI/3.)
              trk_pt_counts[1][iPth]++;
            else if (dphi >= 7.*M_PI/8.)
              trk_pt_counts[2][iPth]++;
            break;
          }
        }
      } // end loop over iPart

    } // end loop over jets


    TH1D* h = nullptr;
    TH2D* h2 = nullptr;
    int* arr = nullptr;

    if (takeEvent) {

      for (int iDPhi = 0; iDPhi < 3; iDPhi++) {
        h = h_trk_pt_yield[iDPhi][iCent];
        h2 = h2_trk_pt_cov[iDPhi][iCent];
        arr = trk_pt_counts[iDPhi];
        for (short iX = 0; iX < h->GetNbinsX (); iX++)
          arr[iX] = arr[iX] / jets;
        for (short iX = 0; iX < h->GetNbinsX (); iX++) {
          h->SetBinContent (iX+1, h->GetBinContent (iX+1) + (jets)*(arr[iX]));
          for (short iY = 0; iY < h->GetNbinsX (); iY++)
            h2->SetBinContent (iX+1, iY+1, h2->GetBinContent (iX+1, iY+1) + (jets)*(arr[iX])*(arr[iY]));
        } // end loop over iX
      } // end loop over iDPhi

      for (int iRedPth = 0; iRedPth < nRedPthBins; iRedPth++) {
        h = h_trk_dphi_yield[iRedPth][iCent];
        h2 = h2_trk_dphi_cov[iRedPth][iCent];
        arr = trk_dphi_counts[iRedPth];
        for (short iX = 0; iX < h->GetNbinsX (); iX++)
          arr[iX] = arr[iX] / jets;
        for (short iX = 0; iX < h->GetNbinsX (); iX++) {
          h->SetBinContent (iX+1, h->GetBinContent (iX+1) + (jets)*(arr[iX]));
          for (short iY = 0; iY < h->GetNbinsX (); iY++)
            h2->SetBinContent (iX+1, iY+1, h2->GetBinContent (iX+1, iY+1) + (jets)*(arr[iX])*(arr[iY]));
        } // end loop over iX
      } // end loop over iRedPth

      for (int iDPhi = 0; iDPhi < 3; iDPhi++)
        for (int iPth = 0; iPth < nPthBins; iPth++)
          trk_pt_counts[iDPhi][iPth] = 0;

      for (int iRedPth = 0; iRedPth < nRedPthBins; iRedPth++)
        for (int iDPhi = 0; iDPhi < nDPhiBins; iDPhi++)
          trk_dphi_counts[iRedPth][iDPhi] = 0;
    }

    {
      h = h_jet_pt_yield[iCent];
      h2 = h2_jet_pt_cov[iCent];
      arr = jet_ptj_counts;
      for (int iX = 0; iX < h->GetNbinsX (); iX++) {
        h->SetBinContent (iX+1, h->GetBinContent (iX+1) + arr[iX]);
        for (int iY = 0; iY < h->GetNbinsX (); iY++)
          h2->SetBinContent (iX+1, iY+1, h2->GetBinContent (iX+1, iY+1) + (arr[iX])*(arr[iY]));
      } // end loop over iX
    }

    for (int i = 0; i < nPtJBins; i++)
      jet_ptj_counts[i] = 0;

    h_jet_yield[iCent]->Fill (jets);

  } // end loop over iEvent

  std::cout << std::endl;



  for (int iCent = 0; iCent < nCentBins; iCent++)
    n_jets[nCentBins] += n_jets[iCent];
  std::cout << "n_jets = ";
  for (int iCent = 0; iCent < nCentBins; iCent++)  std::cout << n_jets[iCent] << ", " << std::endl;
  std::cout << n_jets[nCentBins] << std::endl;



  // create centrality-integrated plot by summing other centralities
  for (int iCent = 0; iCent < nCentBins; iCent++) {
    for (int iDPhi = 0; iDPhi < 3; iDPhi++) {
      h_trk_pt_yield[iDPhi][nCentBins]->Add (h_trk_pt_yield[iDPhi][iCent]);
      h2_trk_pt_cov[iDPhi][nCentBins]->Add (h2_trk_pt_cov[iDPhi][iCent]);
    }
    for (int iRedPth = 0; iRedPth < nRedPthBins; iRedPth++) {
      h_trk_dphi_yield[iRedPth][nCentBins]->Add (h_trk_dphi_yield[iRedPth][iCent]);
      h2_trk_dphi_cov[iRedPth][nCentBins]->Add (h2_trk_dphi_cov[iRedPth][iCent]);
    }
    h_jet_pt_yield[nCentBins]->Add (h_jet_pt_yield[iCent]);
    h2_jet_pt_cov[nCentBins]->Add (h2_jet_pt_cov[iCent]);
    h_jet_yield[nCentBins]->Add (h_jet_yield[iCent]);
  }



  {
    TH2D* h2 = nullptr;
    TH1D* h = nullptr;

    for (int iCent = 0; iCent < nCentBins + 1; iCent++) {

      const float n = (float) n_jets[iCent];

      for (int iDPhi = 0; iDPhi < 3; iDPhi++) {
        h2 = h2_trk_pt_cov[iDPhi][iCent];
        h = h_trk_pt_yield[iDPhi][iCent];
        CalcUncertainties (h, h2, n);
        for (int iX = 1; iX <= h->GetNbinsX (); iX++) {
          h->SetBinContent (iX, h->GetBinContent (iX) / h->GetBinWidth (iX));
          h->SetBinError   (iX, h->GetBinError   (iX) / h->GetBinWidth (iX));
          for (int iY = 1; iY <= h->GetNbinsX (); iY++) {
            h2->SetBinContent (iX, iY, h2->GetBinContent (iX, iY) / (h2->GetXaxis ()->GetBinWidth (iX) * h2->GetYaxis ()->GetBinWidth (iY)));
            h2->SetBinError   (iX, iY, h2->GetBinError   (iX, iY) / (h2->GetXaxis ()->GetBinWidth (iX) * h2->GetYaxis ()->GetBinWidth (iY)));
          } // end loop over iY
        } // end loop over iX
      }


      for (int iRedPth = 0; iRedPth < nRedPthBins; iRedPth++) {
        h2 = h2_trk_dphi_cov[iRedPth][iCent];
        h = h_trk_dphi_yield[iRedPth][iCent];
        CalcUncertainties (h, h2, n);
        for (int iX = 1; iX <= h->GetNbinsX (); iX++) {
          h->SetBinContent (iX, h->GetBinContent (iX) / h->GetBinWidth (iX));
          h->SetBinError   (iX, h->GetBinError   (iX) / h->GetBinWidth (iX));
          for (int iY = 1; iY <= h->GetNbinsX (); iY++) {
            h2->SetBinContent (iX, iY, h2->GetBinContent (iX, iY) / (h2->GetXaxis ()->GetBinWidth (iX) * h2->GetYaxis ()->GetBinWidth (iY)));
            h2->SetBinError   (iX, iY, h2->GetBinError   (iX, iY) / (h2->GetXaxis ()->GetBinWidth (iX) * h2->GetYaxis ()->GetBinWidth (iY)));
          } // end loop over iY
        } // end loop over iX
      }
    }

  }



  // now save histograms to a rootfile
  outFile->cd ();

  for (int iCent = 0; iCent < nCentBins + 1; iCent++) {
    for (int iDPhi = 0; iDPhi < 3; iDPhi++) {
      h_trk_pt_yield[iDPhi][iCent]->Write ();
      //h2_trk_pt_cov[iDPhi][iCent]->Write ();
    }
    for (int iRedPth = 0; iRedPth < nRedPthBins; iRedPth++) {
      h_trk_dphi_yield[iRedPth][iCent]->Write ();
      //h2_trk_dphi_cov[iRedPth][iCent]->Write ();
    }
    h_jet_pt_yield[iCent]->Write ();
    //h2_jet_pt_cov[iCent]->Write ();
    h_jet_yield[iCent]->Write (); 
  }
  
  outFile->Close ();

  return 0;
}

#endif
