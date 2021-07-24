#ifndef __gen_cent_classes_cxx__
#define __gen_cent_classes_cxx__

#include <TFile.h>
#include <TChain.h>
#include <TH1D.h>
#include <TH2D.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>

#include <math.h>

#include <AtlasStyle.h>

#include <Utilities.h>
#include <MyColors.h>

#include "header.h"

using namespace PythiaAngantyrStudy;


int centPercs[] = {100, 99, 98, 97, 96, 95, 94, 93, 92, 91, 90, 89, 88, 87, 86, 85, 84, 83, 82, 81, 80, 79, 78, 77, 76, 75, 74, 73, 72, 71, 70, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
int numCentBins = sizeof (centPercs) / sizeof (centPercs[0]) - 1;

TH1D* h_fcal_et_negEta_pp = nullptr;
TH1D* h_fcal_et_posEta_pp = nullptr;
TH1D* h_fcal_et_tot_pp = nullptr;
TH1D* h_fcal_et_negEta_pp_bkg = nullptr;
TH1D* h_fcal_et_posEta_pp_bkg = nullptr;
TH1D* h_fcal_et_tot_pp_bkg = nullptr;
TH1D* h_fcal_et_negEta_pPb = nullptr;
TH1D* h_fcal_et_posEta_pPb = nullptr;
TH1D* h_fcal_et_tot_pPb = nullptr;
TH1D* h_fcal_et_negEta_pPb_bkg = nullptr;
TH1D* h_fcal_et_posEta_pPb_bkg = nullptr;
TH1D* h_fcal_et_tot_pPb_bkg = nullptr;
TH1D* h_ncoll_pPb = nullptr;
TH1D* h_ncoll_pPb_bkg = nullptr;


int main (int argc, char** argv) {

  SetAtlasStyle ();

  TFile* inFile = nullptr;


  inFile = new TFile ("rootFiles/pp/cent_hists.root", "read");
  h_fcal_et_tot_pp = (TH1D*) inFile->Get ("h_fcal_et_tot_pp");
  h_fcal_et_negEta_pp = (TH1D*) inFile->Get ("h_fcal_et_negEta_pp");
  h_fcal_et_posEta_pp = (TH1D*) inFile->Get ("h_fcal_et_posEta_pp");

  inFile = new TFile ("rootFiles/pPb/cent_hists.root", "read");
  h_fcal_et_tot_pPb = (TH1D*) inFile->Get ("h_fcal_et_tot_pPb");
  h_fcal_et_negEta_pPb = (TH1D*) inFile->Get ("h_fcal_et_negEta_pPb");
  h_fcal_et_posEta_pPb = (TH1D*) inFile->Get ("h_fcal_et_posEta_pPb");
  h_ncoll_pPb = (TH1D*) inFile->Get ("h_ncoll_pPb");


  double xq[101];
  double yq[101];
  TString percs[101];
  for (int iPerc = 0; iPerc < 101; iPerc++) {
    xq[iPerc] = 0.00;
    xq[iPerc] = ((double)iPerc) / 100.;
    percs[iPerc] = Form ("%i%%", 100-iPerc);
  }


  {
    TCanvas* c = new TCanvas ();

    c->SetLogy ();

    TH1D* h = nullptr;
    ofstream cutsfile;
  
    h = (TH1D*) h_fcal_et_tot_pp->Clone ("htemp");
    h->GetQuantiles (101, yq, xq);
  
    cutsfile.open ("aux/ppFCalCentCuts.dat");
    for (int i = 0; i < 101; i++) {
      std::cout << xq[i] << ", " << yq[i] << std::endl;
      cutsfile << std::setw (6) << percs[i] << "\t" << yq[i] << "\n";
    }
    cutsfile.close ();

    h->SetLineColor (colors[0]);
    h->SetLineWidth (2);
    h->DrawCopy ("hist");
    SaferDelete (&h);


    h = (TH1D*) h_fcal_et_posEta_pPb->Clone ("htemp");
    h->GetQuantiles (101, yq, xq);
  
    cutsfile.open ("aux/pPbFCalCentCuts.dat");
    for (int i = 0; i < 101; i++) {
      std::cout << xq[i] << ", " << yq[i] << std::endl;
      cutsfile << std::setw (6) << percs[i] << "\t" << yq[i] << "\n";
    }
    cutsfile.close ();

    h->SetLineColor (colors[3]);
    h->SetLineWidth (2);
    h->DrawCopy ("hist same");
    SaferDelete (&h);

  
    myText (0.55, 0.88, kBlack, "#bf{Pythia 8.306} Angantyr, #sqrt{s_{NN}} = 5.02 TeV", 0.030);
    myText (0.55, 0.84, kBlack, "SoftQCD:Nondiffractive = #bf{on}", 0.030);
    myText (0.55, 0.80, colors[0], "#it{pp}", 0.030);
    myText (0.55, 0.76, colors[3], "#it{p}+Pb", 0.030);


    c->SaveAs ("Plots/FcalDists.pdf");
  }



  {
    TCanvas* c = new TCanvas ();

    c->SetLogy ();

    TH1D* h = nullptr;
    ofstream cutsfile;

    h = (TH1D*) h_ncoll_pPb->Clone ("htemp");
    h->GetQuantiles (101, yq, xq);
  
    cutsfile.open ("aux/pPbNCollCuts.dat");
    for (int i = 0; i < 101; i++) {
      std::cout << xq[i] << ", " << yq[i] << std::endl;
      cutsfile << std::setw (6) << percs[i] << "\t" << yq[i] << "\n";
    }
    cutsfile.close ();

    h->SetLineColor (myCyan);
    h->SetLineWidth (2);
    h->DrawCopy ("hist");
    SaferDelete (&h);

    myText (0.55, 0.88, kBlack, "#bf{Pythia 8.306} Angantyr", 0.032);
    myText (0.55, 0.84, kBlack, "#it{p}+Pb, #sqrt{s_{NN}} = 5.02 TeV", 0.032);
    myText (0.55, 0.80, kBlack, "SoftQCD:Nondiffractive = #bf{on}", 0.032);


    c->SaveAs ("Plots/NcollDists.pdf");
  }



  return 0;
}

#endif
