#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TF1.h>
#include <TGraphAsymmErrors.h>
#include <TLine.h>
#include <TCanvas.h>
#include <TProfile.h>
#include <TLatex.h>
#include <TF1.h>
#include <TBox.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <math.h>

#include <AtlasStyle.h>
#include <AtlasUtils.h>

#include <GlobalParams.h>
#include <Utilities.h>
#include <MyColors.h>

#include "header.h"

using namespace PythiaAngyntyrStudy;

typedef TGraphAsymmErrors TGAE;

TH1D* h_trk_pt_ns_yield_pPb;
TH1D* h_trk_pt_ns_yield_pp;
TH1D* h_trk_pt_ns_yield_ratio; // ratio of track yield in a jet / all tracks
TH1D* h_trk_pt_as_yield_pPb;
TH1D* h_trk_pt_as_yield_pp;
TH1D* h_trk_pt_as_yield_ratio; // ratio of track yield in a jet / all tracks

TH1D* h_trk_dphi_pt_gt2_yield_pPb;
TH1D* h_trk_dphi_pt_gt2_yield_pp;
TH1D* h_trk_dphi_pt_gt2_yield_ratio;
TH1D* h_trk_dphi_pt_lt2_yield_pPb;
TH1D* h_trk_dphi_pt_lt2_yield_pp;
TH1D* h_trk_dphi_pt_lt2_yield_ratio; 
TH1D* h_jet_pt_yield_pPb;
TH1D* h_jet_pt_yield_pp;
TH1D* h_jet_pt_yield_ratio;

TH1D* h_jet_yield_pPb;
TH1D* h_jet_yield_pp;


void SetVariances (TH1D* h, TH2D* h2) {
  const int nb = h->GetNbinsX ();
  assert (nb == h2->GetNbinsX () && nb == h2->GetNbinsY ());

  for (int iX = 1; iX <= nb; iX++) {
    assert (h2->GetBinContent (iX, iX) >= 0);
    h->SetBinError (iX, sqrt (h2->GetBinContent (iX, iX)));
  }
}



int main () {

  SetAtlasStyle(); 

  TFile* inFile = nullptr;


  inFile = new TFile ("rootFiles/pp/hists.root", "read");

  h_trk_pt_ns_yield_pp = (TH1D*) inFile->Get ("h_trk_pt_ns_yield_pp")->Clone ("h_trk_pt_ns_pp");
  h_trk_pt_as_yield_pp = (TH1D*) inFile->Get ("h_trk_pt_as_yield_pp")->Clone ("h_trk_pt_as_pp");

  h_trk_dphi_pt_gt2_yield_pp = (TH1D*) inFile->Get ("h_trk_dphi_pt_gt2_yield_pp")->Clone ("h_trk_dphi_pt_gt2_pp");
  h_trk_dphi_pt_lt2_yield_pp = (TH1D*) inFile->Get ("h_trk_dphi_pt_lt2_yield_pp")->Clone ("h_trk_dphi_pt_lt2_pp");

  h_jet_pt_yield_pp = (TH1D*) inFile->Get ("h_jet_pt_yield_pp")->Clone ("h_jet_pt_pp");

  h_jet_yield_pp = (TH1D*) inFile->Get ("h_jet_yield_pp")->Clone ("h_jet_pp");

  inFile = new TFile ("rootFiles/pPb/hists.root", "read");

  h_trk_pt_ns_yield_pPb = (TH1D*) inFile->Get ("h_trk_pt_ns_yield_pPb")->Clone ("h_trk_pt_ns_pPb");
  h_trk_pt_as_yield_pPb = (TH1D*) inFile->Get ("h_trk_pt_as_yield_pPb")->Clone ("h_trk_pt_as_pPb");

  h_trk_dphi_pt_gt2_yield_pPb = (TH1D*) inFile->Get ("h_trk_dphi_pt_gt2_yield_pPb")->Clone ("h_trk_dphi_pt_gt2_pPb");
  h_trk_dphi_pt_lt2_yield_pPb = (TH1D*) inFile->Get ("h_trk_dphi_pt_lt2_yield_pPb")->Clone ("h_trk_dphi_pt_lt2_pPb");

  h_jet_pt_yield_pPb = (TH1D*) inFile->Get ("h_jet_pt_yield_pPb")->Clone ("h_jet_pt_pPb");

  h_jet_yield_pPb = (TH1D*) inFile->Get ("h_jet_yield_pPb")->Clone ("h_jet_pPb");

  h_trk_pt_ns_yield_ratio = (TH1D*) h_trk_pt_ns_yield_pPb->Clone ("h_trk_pt_ns_ratio");
  h_trk_pt_ns_yield_ratio->Divide (h_trk_pt_ns_yield_pp);
  h_trk_pt_as_yield_ratio = (TH1D*) h_trk_pt_as_yield_pPb->Clone ("h_trk_pt_as_ratio");
  h_trk_pt_as_yield_ratio->Divide (h_trk_pt_as_yield_pp);

  h_trk_dphi_pt_gt2_yield_ratio = (TH1D*) h_trk_dphi_pt_gt2_yield_pPb->Clone ("h_trk_dphi_pt_gt2_ratio");
  h_trk_dphi_pt_gt2_yield_ratio->Divide (h_trk_dphi_pt_gt2_yield_pp);
  h_trk_dphi_pt_lt2_yield_ratio = (TH1D*) h_trk_dphi_pt_lt2_yield_pPb->Clone ("h_trk_dphi_pt_lt2_ratio");
  h_trk_dphi_pt_lt2_yield_ratio->Divide (h_trk_dphi_pt_lt2_yield_pp);

  h_jet_pt_yield_ratio = (TH1D*) h_jet_pt_yield_pPb->Clone ("h_jet_pt_ratio");
  h_jet_pt_yield_ratio->Divide (h_jet_pt_yield_pp);



  {
    const char* canvasName = "c_trk_pt_yield";
    TCanvas* c = new TCanvas (canvasName, "", 800, 800);
    TPad* uPad = new TPad (Form ("%s_uPad", canvasName), "", 0.0, 0.4, 1.0, 1.0);
    TPad* dPad = new TPad (Form ("%s_dPad", canvasName), "", 0.0, 0.0, 1.0, 0.4);

    uPad->SetBottomMargin (0);
    dPad->SetTopMargin (0);
    dPad->SetBottomMargin (0.25);
    uPad->Draw ();
    dPad->Draw ();

    TH1D* h = nullptr; 
    TGAE* g = nullptr;

    uPad->cd (); 
    uPad->SetLogy ();

    float ymin = 2e-6;
    float ymax = 30;

    h = (TH1D*) h_trk_pt_ns_yield_pPb->Clone ("h");
    h->Reset ();
    h->GetYaxis ()->SetRangeUser (ymin, ymax);
    h->GetXaxis ()->SetTitle ("#it{p}_{T}^{ch} [GeV]");
    h->GetXaxis ()->SetTitleSize (0.04/0.6);
    h->GetXaxis ()->SetLabelSize (0.04/0.6);
    h->GetXaxis ()->SetTitleOffset (1.5*0.6);
    h->GetYaxis ()->SetTitle ("d^{2}N_{ch} / d#it{p}_{T} #Delta#phi [GeV^{-1}]");
    h->GetYaxis ()->SetTitle ("(1/N_{jet}) (d^{2}N_{ch} / d#it{p}_{T}^{ch} d#Delta#phi_{ch,jet}) [GeV^{-1}]");
    h->GetYaxis ()->SetTitleSize (0.04/0.6);
    h->GetYaxis ()->SetLabelSize (0.04/0.6);
    h->GetYaxis ()->SetTitleOffset (1.5*0.6);

    h->SetLineWidth (0);
    h->DrawCopy ("hist ][");
    SaferDelete (&h);

    h = (TH1D*) h_trk_pt_ns_yield_pPb->Clone ("h");
    h->SetLineColor (pastels[0]);
    h->SetLineWidth (2);
    h->DrawCopy ("hist ][ same");
    g = make_graph (h);
    g->SetMarkerStyle (kFullCircle);
    g->SetMarkerSize (0.8);
    g->SetMarkerColor (pastels[0]);
    g->SetLineColor (pastels[0]);
    g->SetLineWidth (2);
    ((TGAE*) g->Clone ())->Draw ("p");
    g->SetMarkerStyle (kOpenCircle);
    g->SetMarkerColor (kBlack);
    g->SetLineWidth (0);
    ((TGAE*) g->Clone ())->Draw ("p");
    SaferDelete (&h);
    SaferDelete (&g);

    h = (TH1D*) h_trk_pt_ns_yield_pp->Clone ("h");
    h->SetLineColor (pastels[1]);
    h->SetLineWidth (2);
    h->DrawCopy ("hist ][ same");
    g = make_graph (h);
    g->SetMarkerStyle (kFullCircle);
    g->SetMarkerSize (0.8);
    g->SetMarkerColor (pastels[1]);
    g->SetLineColor (pastels[1]);
    g->SetLineWidth (2);
    ((TGAE*) g->Clone ())->Draw ("p");
    g->SetMarkerStyle (kOpenCircle);
    g->SetMarkerColor (kBlack);
    g->SetLineWidth (0);
    ((TGAE*) g->Clone ())->Draw ("p");
    SaferDelete (&h);
    SaferDelete (&g);

    myText (0.200, 0.290, kBlack, "#bf{Pythia 8.244}, #sqrt{s} = 5.02 TeV", 0.028/0.6);
    myText (0.200, 0.230, kBlack, "#it{p}_{T}^{jet} > 60 GeV, #Delta#phi_{ch,jet} > 7#pi/8", 0.028/0.6);

    myLineText2 (0.25, 0.170, pastels[0], kFullCircle, "#it{pp}", 1.0, 0.028/0.60, true);
    myLineText2 (0.25, 0.110, pastels[1], kFullCircle, "#it{p}+Pb", 1.0, 0.028/0.60, true);
    

    dPad->cd (); 

    ymin = 0.84;
    ymax = 1.16;

    h = (TH1D*) h_trk_pt_ns_yield_ratio->Clone ("h");
    h->Reset ();
    for (int i = 1; i <= h->GetNbinsX (); i++) h->SetBinContent (i, 1);
    h->GetYaxis ()->SetRangeUser (ymin, ymax);
    h->GetXaxis ()->SetTitle ("#it{p}_{T}^{ch} [GeV]");
    h->GetXaxis ()->SetTitleSize (0.04/0.4);
    h->GetXaxis ()->SetLabelSize (0.04/0.4);
    h->GetXaxis ()->SetTitleOffset (2.7*0.4);
    h->GetXaxis ()->SetLabelOffset (-0.04*0.4);
    h->GetYaxis ()->SetTitle ("#it{p}+Pb / #it{pp}");
    h->GetYaxis ()->SetTitleSize (0.04/0.4);
    h->GetYaxis ()->SetLabelSize (0.04/0.4);
    h->GetYaxis ()->SetTitleOffset (1.5*0.4);
    h->GetYaxis ()->CenterTitle ();

    h->SetLineWidth (1);
    h->SetLineStyle (2);
    h->DrawCopy ("hist ][");
    SaferDelete (&h);

    h = (TH1D*) h_trk_pt_ns_yield_ratio;
    g = make_graph (h);
    g->SetLineColor (pastels[1]);
    g->SetLineWidth (2);
    g->SetMarkerColor (pastels[1]);
    g->SetMarkerStyle (kFullCircle);
    g->SetMarkerSize (0.8);
    ((TGAE*) g->Clone ())->Draw ("p");
    g->SetMarkerStyle (kOpenCircle);
    g->SetMarkerColor (kBlack);
    g->SetLineWidth (0);
    ((TGAE*) g->Clone ())->Draw ("p");
    SaferDelete (&g);

    c->SaveAs ("Plots/Ptch_yields_comparison.pdf"); 
  }




  return 0;
}


void plotter () {
  main ();
}
