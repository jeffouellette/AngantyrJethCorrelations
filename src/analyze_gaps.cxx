#ifndef __analyze_gaps_cxx__
#define __analyze_gaps_cxx__

#include <TFile.h>
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


int main (int argc, char** argv) {

  SetAtlasStyle ();

  TFile* inFile = nullptr;

  TH1D* h_gap_negEta_pp[4];
  TH1D* h_gap_posEta_pp[4];
  TH1D* h_gap_negEta_pPb[4];
  TH1D* h_gap_posEta_pPb[4];

  inFile = new TFile ("rootFiles/pp/gaps_hists.root", "read");
  for (int i = 0; i < 4; i++) {

    std::string evtType;
    if      (i == 0)  evtType = "nondiff";
    else if (i == 1)  evtType = "single_diff";
    else if (i == 2)  evtType = "double_diff";
    else if (i == 3)  evtType = "central_diff";

    h_gap_negEta_pp[i] = (TH1D*) inFile->Get (Form ("h_gap_negEta_%s_pp", evtType.c_str ()));
    h_gap_posEta_pp[i] = (TH1D*) inFile->Get (Form ("h_gap_posEta_%s_pp", evtType.c_str ()));

  }

  inFile = new TFile ("rootFiles/pPb/gaps_hists.root", "read");
  for (int i = 0; i < 4; i++) {

    std::string evtType;
    if      (i == 0)  evtType = "nondiff";
    else if (i == 1)  evtType = "single_diff";
    else if (i == 2)  evtType = "double_diff";
    else if (i == 3)  evtType = "central_diff";

    h_gap_negEta_pPb[i] = (TH1D*) inFile->Get (Form ("h_gap_negEta_%s_pPb", evtType.c_str ()));
    h_gap_posEta_pPb[i] = (TH1D*) inFile->Get (Form ("h_gap_posEta_%s_pPb", evtType.c_str ()));

  }


  {
    TCanvas* c = new TCanvas ("c", "", 800, 800);

    c->SetLogy ();

    TH1D* h = nullptr;
    ofstream cutsfile;
  
    for (int i = 0; i < 4; i++) {
      h = (TH1D*) h_gap_negEta_pp[i]->Clone ("htemp");
      h->Add (h_gap_posEta_pp[i]);

      h->GetXaxis ()->SetTitle ("#Delta#it{#eta}_{p}");

      h->SetLineColor (colors[i]);
      h->SetLineWidth (2);
      h->DrawCopy (i == 0 ? "hist" : "hist same");
      SaferDelete (&h);
    }


    myText (0.55, 0.89, kBlack, "#bf{Pythia 8.306} Angantyr", 0.03);
    myText (0.55, 0.85, kBlack, "#it{pp}, #sqrt{s} = 5.02 TeV", 0.03);
    myText (0.55, 0.81, kBlack, "SoftQCD:Inelastic = #bf{on}", 0.03);
    myText (0.55, 0.77, colors[0], "Nondiffractive", 0.03);
    myText (0.55, 0.73, colors[1], "Single diffractive", 0.03);
    myText (0.55, 0.69, colors[2], "Double diffractive", 0.03);
    myText (0.55, 0.65, colors[3], "Central diffractive", 0.03);


    c->SaveAs ("Plots/pp_GapDists.pdf");
  }



  {
    TCanvas* c = new TCanvas ("c3", "", 800, 800);

    c->SetLogy ();

    TH1D* h = nullptr;
    ofstream cutsfile;
  
    for (int i = 0; i < 4; i++) {
      h = (TH1D*) h_gap_posEta_pPb[i]->Clone ("htemp");

      h->GetXaxis ()->SetTitle ("#Delta#it{#eta}_{Pb}");

      h->SetLineColor (colors[i]);
      h->SetLineWidth (2);
      h->DrawCopy (i == 0 ? "hist" : "hist same");
      SaferDelete (&h);
    }


    myText (0.55, 0.89, kBlack, "#bf{Pythia 8.306} Angantyr", 0.03);
    myText (0.55, 0.85, kBlack, "#it{p}+Pb, #sqrt{s_{NN}} = 5.02 TeV", 0.03);
    myText (0.55, 0.81, kBlack, "SoftQCD:Inelastic = #bf{on}", 0.03);
    myText (0.55, 0.77, colors[0], "Nondiffractive", 0.03);
    myText (0.55, 0.73, colors[1], "Single diffractive", 0.03);
    myText (0.55, 0.69, colors[2], "Double diffractive", 0.03);
    myText (0.55, 0.65, colors[3], "Central diffractive", 0.03);


    c->SaveAs ("Plots/pPb_PbSideGapDists.pdf");
  }



  {
    TCanvas* c = new TCanvas ("c2", "", 800, 800);

    c->SetLogy ();

    TH1D* h = nullptr;
    ofstream cutsfile;
  
    for (int i = 0; i < 4; i++) {
      h = (TH1D*) h_gap_negEta_pPb[i]->Clone ("htemp");

      h->GetXaxis ()->SetTitle ("#Delta#it{#eta}_{p}");

      h->SetLineColor (colors[i]);
      h->SetLineWidth (2);
      h->DrawCopy (i == 0 ? "hist" : "hist same");
      SaferDelete (&h);
    }


    myText (0.55, 0.89, kBlack, "#bf{Pythia 8.306} Angantyr", 0.03);
    myText (0.55, 0.85, kBlack, "#it{p}+Pb, #sqrt{s_{NN}} = 5.02 TeV", 0.03);
    myText (0.55, 0.81, kBlack, "SoftQCD:Inelastic = #bf{on}", 0.03);
    myText (0.55, 0.77, colors[0], "Nondiffractive", 0.03);
    myText (0.55, 0.73, colors[1], "Single diffractive", 0.03);
    myText (0.55, 0.69, colors[2], "Double diffractive", 0.03);
    myText (0.55, 0.65, colors[3], "Central diffractive", 0.03);


    c->SaveAs ("Plots/pPb_pSideGapDists.pdf");
  }



  return 0;
}

#endif
