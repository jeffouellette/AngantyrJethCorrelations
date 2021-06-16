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

using namespace PythiaAngantyrStudy;

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


int main () {

  SetAtlasStyle(); 

  TFile* inFile = nullptr;


  inFile = new TFile ("rootFiles/finalHists.root", "read");

  h_trk_pt_ns_yield_pp = (TH1D*) inFile->Get ("h_trk_pt_ns_pp");
  h_trk_pt_as_yield_pp = (TH1D*) inFile->Get ("h_trk_pt_as_pp");

  h_trk_dphi_pt_gt2_yield_pp = (TH1D*) inFile->Get ("h_trk_dphi_pt_gt2_pp");
  h_trk_dphi_pt_lt2_yield_pp = (TH1D*) inFile->Get ("h_trk_dphi_pt_lt2_pp");

  h_jet_pt_yield_pp = (TH1D*) inFile->Get ("h_jet_pt_pp");

  h_trk_pt_ns_yield_pPb = (TH1D*) inFile->Get ("h_trk_pt_ns_pPb");
  h_trk_pt_as_yield_pPb = (TH1D*) inFile->Get ("h_trk_pt_as_pPb");

  h_trk_dphi_pt_gt2_yield_pPb = (TH1D*) inFile->Get ("h_trk_dphi_pt_gt2_pPb");
  h_trk_dphi_pt_lt2_yield_pPb = (TH1D*) inFile->Get ("h_trk_dphi_pt_lt2_pPb");

  h_jet_pt_yield_pPb = (TH1D*) inFile->Get ("h_jet_pt_pPb");

  h_trk_pt_ns_yield_ratio = (TH1D*) inFile->Get ("h_trk_pt_ns_ratio");
  h_trk_pt_as_yield_ratio = (TH1D*) inFile->Get ("h_trk_pt_as_ratio");

  h_trk_dphi_pt_gt2_yield_ratio = (TH1D*) inFile->Get ("h_trk_dphi_pt_gt2_ratio");
  h_trk_dphi_pt_lt2_yield_ratio = (TH1D*) inFile->Get ("h_trk_dphi_pt_lt2_ratio");

  h_jet_pt_yield_ratio = (TH1D*) h_jet_pt_yield_pPb->Clone ("h_jet_pt_ratio");



  {
    const char* canvasName = "c_trk_pt_yield";
    TCanvas* c = new TCanvas (canvasName, "", 800, 800);
    TPad* uPad = new TPad (Form ("%s_uPad", canvasName), "", 0.0, 0.3, 1.0, 1.0);
    TPad* dPad = new TPad (Form ("%s_dPad", canvasName), "", 0.0, 0.0, 1.0, 0.3);

    uPad->SetBottomMargin (0);
    uPad->SetLeftMargin (0.16);
    dPad->SetTopMargin (0);
    dPad->SetBottomMargin (0.30);
    dPad->SetLeftMargin (0.16);
    uPad->Draw ();
    dPad->Draw ();

    TH1D* h = nullptr; 
    TGAE* g = nullptr;

    uPad->cd (); 
    uPad->SetLogy ();

    float ymin = 2e-6;
    float ymax = 30;

    h = (TH1D*) h_trk_pt_as_yield_pPb->Clone ("h");
    h->Reset ();
    h->GetYaxis ()->SetRangeUser (ymin, ymax);
    h->GetXaxis ()->SetTitleSize (0);
    h->GetXaxis ()->SetLabelSize (0);
    h->GetYaxis ()->SetTitle ("#frac{1}{N_{jet}} #frac{d^{2}N_{ch}}{d#it{p}_{T}^{ch} d#Delta#phi_{ch,jet}} [GeV^{-1}]");
    h->GetYaxis ()->SetTitleSize (0.03/0.7);
    h->GetYaxis ()->SetLabelSize (0.03/0.7);
    h->GetYaxis ()->SetTitleOffset (2.3*0.7);

    h->SetLineWidth (0);
    h->DrawCopy ("hist ][");
    SaferDelete (&h);

    h = (TH1D*) h_trk_pt_as_yield_pPb->Clone ("h");
    myDraw (h, myOrange, kFullCircle, 0.8);
    SaferDelete (&h);

    h = (TH1D*) h_trk_pt_as_yield_pp->Clone ("h");
    myDraw (h, myPurple, kFullCircle, 0.8);
    SaferDelete (&h);

    myText (0.220, 0.290, kBlack, "#bf{Pythia 8.303} Angantyr, #sqrt{s} = 5.02 TeV", 0.028/0.7);
    myText (0.220, 0.230, kBlack, "#it{p}_{T}^{jet} > 60 GeV, #Delta#phi_{ch,jet} > 7#pi/8", 0.028/0.7);

    myLineText2 (0.270, 0.170, myPurple, kFullCircle, "#it{pp} total", 1.0, 0.028/0.7, true);
    myLineText2 (0.270, 0.110, myOrange, kFullCircle, "#it{p}+Pb total", 1.0, 0.028/0.7, true);
    

    dPad->cd (); 

    ymin = 0.83;
    ymax = 1.17;

    h = (TH1D*) h_trk_pt_as_yield_ratio->Clone ("h");
    h->Reset ();
    for (int i = 1; i <= h->GetNbinsX (); i++) h->SetBinContent (i, 1);
    h->GetYaxis ()->SetRangeUser (ymin, ymax);
    h->GetXaxis ()->SetTitle ("#it{p}_{T}^{ch} [GeV]");
    h->GetXaxis ()->SetTitleSize (0.04/0.3);
    h->GetXaxis ()->SetLabelSize (0.04/0.3);
    h->GetXaxis ()->SetTitleOffset (3.0*0.3);
    //h->GetXaxis ()->SetLabelOffset (-0.04*0.3);
    h->GetYaxis ()->SetTitle ("#frac{N_{tot}^{p+Pb} #minus N_{bkgd}^{p+Pb}}{N_{tot}^{pp} #minus N_{bkgd}^{pp}}");
    h->GetYaxis ()->SetTitleSize (0.03/0.3);
    h->GetYaxis ()->SetLabelSize (0.03/0.3);
    h->GetYaxis ()->SetTitleOffset (2.3*0.3);
    h->GetYaxis ()->CenterTitle ();

    h->SetLineWidth (1);
    h->SetLineStyle (2);
    h->DrawCopy ("hist ][");
    SaferDelete (&h);

    h = (TH1D*) h_trk_pt_as_yield_ratio->Clone ("h");
    myDraw (h, myPurple, kFullCircle, 0.8);
    SaferDelete (&h);

    c->SaveAs ("Plots/Ptch_yields_comparison.pdf"); 
  }



  {
    const char* canvasName = "c_trk_dphi_pt_gt2_yield";
    TCanvas* c = new TCanvas (canvasName, "", 800, 800);
    TPad* uPad = new TPad (Form ("%s_uPad", canvasName), "", 0.0, 0.3, 1.0, 1.0);
    TPad* dPad = new TPad (Form ("%s_dPad", canvasName), "", 0.0, 0.0, 1.0, 0.3);

    uPad->SetBottomMargin (0);
    uPad->SetLeftMargin (0.16);
    dPad->SetTopMargin (0);
    dPad->SetBottomMargin (0.30);
    dPad->SetLeftMargin (0.16);
    uPad->Draw ();
    dPad->Draw ();

    TH1D* h = nullptr; 
    TGAE* g = nullptr;

    uPad->cd (); 

    float ymin = -1.;
    float ymax = 30;

    h = (TH1D*) h_trk_dphi_pt_gt2_yield_pPb->Clone ("h");
    h->Reset ();
    h->GetYaxis ()->SetRangeUser (ymin, ymax);
    h->GetXaxis ()->SetTitleSize (0);
    h->GetXaxis ()->SetLabelSize (0);
    h->GetYaxis ()->SetTitle ("#frac{1}{N_{jet}} #frac{dN_{ch}}{d#Delta#phi_{ch,jet}}");
    h->GetYaxis ()->SetTitleSize (0.03/0.7);
    h->GetYaxis ()->SetLabelSize (0.03/0.7);
    h->GetYaxis ()->SetTitleOffset (2.3*0.7);

    h->SetLineWidth (0);
    h->DrawCopy ("hist ][");
    SaferDelete (&h);

    h = (TH1D*) h_trk_dphi_pt_gt2_yield_pPb->Clone ("h");
    myDraw (h, myOrange, kFullCircle, 0.8);
    SaferDelete (&h);

    h = (TH1D*) h_trk_dphi_pt_gt2_yield_pp->Clone ("h");
    myDraw (h, myPurple, kFullCircle, 0.8);
    SaferDelete (&h);

    myText (0.220, 0.880, kBlack, "#bf{Pythia 8.303} Angantyr, #sqrt{s} = 5.02 TeV", 0.028/0.7);
    myText (0.220, 0.820, kBlack, "#it{p}_{T}^{jet} > 60 GeV, #it{p}_{T}^{ch} > 2 GeV", 0.028/0.7);

    myLineText2 (0.270, 0.760, myPurple, kFullCircle, "#it{pp} total", 1.0, 0.028/0.7, true);
    myLineText2 (0.270, 0.700, myOrange, kFullCircle, "#it{p}+Pb total", 1.0, 0.028/0.7, true);
    

    dPad->cd (); 

    ymin = 0.83;
    ymax = 1.17;

    h = (TH1D*) h_trk_dphi_pt_gt2_yield_ratio->Clone ("h");
    h->Reset ();
    for (int i = 1; i <= h->GetNbinsX (); i++) h->SetBinContent (i, 1);
    h->GetYaxis ()->SetRangeUser (ymin, ymax);
    h->GetXaxis ()->SetTitle ("#Delta#phi_{ch, jet}");
    h->GetXaxis ()->SetTitleSize (0.04/0.3);
    h->GetXaxis ()->SetLabelSize (0.04/0.3);
    h->GetXaxis ()->SetTitleOffset (3.0*0.3);
    //h->GetXaxis ()->SetLabelOffset (-0.04*0.3);
    h->GetYaxis ()->SetTitle ("#frac{N_{tot}^{p+Pb} #minus N_{bkgd}^{p+Pb}}{N_{tot}^{pp} #minus N_{bkgd}^{pp}}");
    h->GetYaxis ()->SetTitleSize (0.03/0.3);
    h->GetYaxis ()->SetLabelSize (0.03/0.3);
    h->GetYaxis ()->SetTitleOffset (2.3*0.3);
    h->GetYaxis ()->CenterTitle ();

    h->SetLineWidth (1);
    h->SetLineStyle (2);
    h->DrawCopy ("hist ][");
    SaferDelete (&h);

    h = (TH1D*) h_trk_dphi_pt_gt2_yield_ratio->Clone ("h");
    myDraw (h, myPurple, kFullCircle, 0.8);
    SaferDelete (&h);

    c->SaveAs ("Plots/DPhi_yields_comparison.pdf"); 
  }




  return 0;
}


void plotter () {
  main ();
}
