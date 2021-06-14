#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>

#include <iostream>

#include <Utilities.h>

#include "header.h"

using namespace PythiaAngyntyrStudy;

typedef TGraphAsymmErrors TGAE;

TH1D* h_trk_pt_ns_yield_pPb;
TH1D* h_trk_pt_ns_yield_pPb_bkg;
TH1D* h_trk_pt_ns_yield_pPb_sig;
TH1D* h_trk_pt_ns_yield_pp;
TH1D* h_trk_pt_ns_yield_pp_bkg;
TH1D* h_trk_pt_ns_yield_pp_sig;
TH1D* h_trk_pt_ns_yield_ratio; // ratio of track yield in a jet / all tracks
TH1D* h_trk_pt_as_yield_pPb;
TH1D* h_trk_pt_as_yield_pPb_bkg;
TH1D* h_trk_pt_as_yield_pPb_sig;
TH1D* h_trk_pt_as_yield_pp;
TH1D* h_trk_pt_as_yield_pp_bkg;
TH1D* h_trk_pt_as_yield_pp_sig;
TH1D* h_trk_pt_as_yield_ratio; // ratio of track yield in a jet / all tracks

TH1D* h_trk_dphi_pt_gt2_yield_pPb;
TH1D* h_trk_dphi_pt_gt2_yield_pPb_bkg;
TH1D* h_trk_dphi_pt_gt2_yield_pPb_sig;
TH1D* h_trk_dphi_pt_gt2_yield_pp;
TH1D* h_trk_dphi_pt_gt2_yield_pp_bkg;
TH1D* h_trk_dphi_pt_gt2_yield_pp_sig;
TH1D* h_trk_dphi_pt_gt2_yield_ratio;
TH1D* h_trk_dphi_pt_lt2_yield_pPb;
TH1D* h_trk_dphi_pt_lt2_yield_pPb_bkg;
TH1D* h_trk_dphi_pt_lt2_yield_pPb_sig;
TH1D* h_trk_dphi_pt_lt2_yield_pp;
TH1D* h_trk_dphi_pt_lt2_yield_pp_bkg;
TH1D* h_trk_dphi_pt_lt2_yield_pp_sig;
TH1D* h_trk_dphi_pt_lt2_yield_ratio; 

TH1D* h_jet_pt_yield_pPb;
TH1D* h_jet_pt_yield_pp;
TH1D* h_jet_pt_yield_ratio;

TH1D* h_jet_yield_pPb;
TH1D* h_jet_yield_pp;


int main () {

  TFile* inFile = nullptr;


  inFile = new TFile ("rootFiles/pp/hists.root", "read");

  h_trk_pt_ns_yield_pp = (TH1D*) inFile->Get ("h_trk_pt_ns_yield_pp")->Clone ("h_trk_pt_ns_pp");
  h_trk_pt_as_yield_pp = (TH1D*) inFile->Get ("h_trk_pt_as_yield_pp")->Clone ("h_trk_pt_as_pp");

  h_trk_dphi_pt_gt2_yield_pp = (TH1D*) inFile->Get ("h_trk_dphi_pt_gt2_yield_pp")->Clone ("h_trk_dphi_pt_gt2_pp");
  h_trk_dphi_pt_lt2_yield_pp = (TH1D*) inFile->Get ("h_trk_dphi_pt_lt2_yield_pp")->Clone ("h_trk_dphi_pt_lt2_pp");

  h_jet_pt_yield_pp = (TH1D*) inFile->Get ("h_jet_pt_yield_pp")->Clone ("h_jet_pt_pp");

  h_jet_yield_pp = (TH1D*) inFile->Get ("h_jet_yield_pp")->Clone ("h_jet_pp");


  inFile = new TFile ("rootFiles/pp/hists_bkg.root", "read");

  h_trk_pt_ns_yield_pp_bkg = (TH1D*) inFile->Get ("h_trk_pt_ns_yield_pp_bkg")->Clone ("h_trk_pt_ns_pp_bkg");
  h_trk_pt_as_yield_pp_bkg = (TH1D*) inFile->Get ("h_trk_pt_as_yield_pp_bkg")->Clone ("h_trk_pt_as_pp_bkg");

  h_trk_dphi_pt_gt2_yield_pp_bkg = (TH1D*) inFile->Get ("h_trk_dphi_pt_gt2_yield_pp_bkg")->Clone ("h_trk_dphi_pt_gt2_pp_bkg");
  h_trk_dphi_pt_lt2_yield_pp_bkg = (TH1D*) inFile->Get ("h_trk_dphi_pt_lt2_yield_pp_bkg")->Clone ("h_trk_dphi_pt_lt2_pp_bkg");


  inFile = new TFile ("rootFiles/pPb/hists.root", "read");

  h_trk_pt_ns_yield_pPb = (TH1D*) inFile->Get ("h_trk_pt_ns_yield_pPb")->Clone ("h_trk_pt_ns_pPb");
  h_trk_pt_as_yield_pPb = (TH1D*) inFile->Get ("h_trk_pt_as_yield_pPb")->Clone ("h_trk_pt_as_pPb");

  h_trk_dphi_pt_gt2_yield_pPb = (TH1D*) inFile->Get ("h_trk_dphi_pt_gt2_yield_pPb")->Clone ("h_trk_dphi_pt_gt2_pPb");
  h_trk_dphi_pt_lt2_yield_pPb = (TH1D*) inFile->Get ("h_trk_dphi_pt_lt2_yield_pPb")->Clone ("h_trk_dphi_pt_lt2_pPb");

  h_jet_pt_yield_pPb = (TH1D*) inFile->Get ("h_jet_pt_yield_pPb")->Clone ("h_jet_pt_pPb");

  h_jet_yield_pPb = (TH1D*) inFile->Get ("h_jet_yield_pPb")->Clone ("h_jet_pPb");


  inFile = new TFile ("rootFiles/pPb/hists_bkg.root", "read");

  h_trk_pt_ns_yield_pPb_bkg = (TH1D*) inFile->Get ("h_trk_pt_ns_yield_pPb_bkg")->Clone ("h_trk_pt_ns_pPb_bkg");
  h_trk_pt_as_yield_pPb_bkg = (TH1D*) inFile->Get ("h_trk_pt_as_yield_pPb_bkg")->Clone ("h_trk_pt_as_pPb_bkg");

  h_trk_dphi_pt_gt2_yield_pPb_bkg = (TH1D*) inFile->Get ("h_trk_dphi_pt_gt2_yield_pPb_bkg")->Clone ("h_trk_dphi_pt_gt2_pPb_bkg");
  h_trk_dphi_pt_lt2_yield_pPb_bkg = (TH1D*) inFile->Get ("h_trk_dphi_pt_lt2_yield_pPb_bkg")->Clone ("h_trk_dphi_pt_lt2_pPb_bkg");



  h_trk_pt_ns_yield_pPb_sig = (TH1D*) h_trk_pt_ns_yield_pPb->Clone ("h_trk_pt_ns_yield_pPb_sig");
  h_trk_pt_ns_yield_pPb_sig->Add (h_trk_pt_ns_yield_pPb_bkg, -1);
  h_trk_pt_ns_yield_pp_sig = (TH1D*) h_trk_pt_ns_yield_pp->Clone ("h_trk_pt_ns_yield_pp_sig");
  h_trk_pt_ns_yield_pp_sig->Add (h_trk_pt_ns_yield_pp_bkg, -1);

  h_trk_pt_as_yield_pPb_sig = (TH1D*) h_trk_pt_as_yield_pPb->Clone ("h_trk_pt_as_yield_pPb_sig");
  h_trk_pt_as_yield_pPb_sig->Add (h_trk_pt_as_yield_pPb_bkg, -1);
  h_trk_pt_as_yield_pp_sig = (TH1D*) h_trk_pt_as_yield_pp->Clone ("h_trk_pt_as_yield_pp_sig");
  h_trk_pt_as_yield_pp_sig->Add (h_trk_pt_as_yield_pp_bkg, -1);

  h_trk_dphi_pt_gt2_yield_pPb_sig = (TH1D*) h_trk_dphi_pt_gt2_yield_pPb->Clone ("h_trk_dphi_pt_gt2_yield_pPb_sig");
  h_trk_dphi_pt_gt2_yield_pPb_sig->Add (h_trk_dphi_pt_gt2_yield_pPb_bkg, -1);
  h_trk_dphi_pt_gt2_yield_pp_sig = (TH1D*) h_trk_dphi_pt_gt2_yield_pp->Clone ("h_trk_dphi_pt_gt2_yield_pp_sig");
  h_trk_dphi_pt_gt2_yield_pp_sig->Add (h_trk_dphi_pt_gt2_yield_pp_bkg, -1);

  h_trk_dphi_pt_lt2_yield_pPb_sig = (TH1D*) h_trk_dphi_pt_lt2_yield_pPb->Clone ("h_trk_dphi_pt_lt2_yield_pPb_sig");
  h_trk_dphi_pt_lt2_yield_pPb_sig->Add (h_trk_dphi_pt_lt2_yield_pPb_bkg, -1);
  h_trk_dphi_pt_lt2_yield_pp_sig = (TH1D*) h_trk_dphi_pt_lt2_yield_pp->Clone ("h_trk_dphi_pt_lt2_yield_pp_sig");
  h_trk_dphi_pt_lt2_yield_pp_sig->Add (h_trk_dphi_pt_lt2_yield_pp_bkg, -1);

  h_trk_pt_ns_yield_ratio = (TH1D*) h_trk_pt_ns_yield_pPb_sig->Clone ("h_trk_pt_ns_ratio");
  h_trk_pt_ns_yield_ratio->Divide (h_trk_pt_ns_yield_pp_sig);
  h_trk_pt_as_yield_ratio = (TH1D*) h_trk_pt_as_yield_pPb_sig->Clone ("h_trk_pt_as_ratio");
  h_trk_pt_as_yield_ratio->Divide (h_trk_pt_as_yield_pp_sig);

  h_trk_dphi_pt_gt2_yield_ratio = (TH1D*) h_trk_dphi_pt_gt2_yield_pPb_sig->Clone ("h_trk_dphi_pt_gt2_ratio");
  h_trk_dphi_pt_gt2_yield_ratio->Divide (h_trk_dphi_pt_gt2_yield_pp_sig);
  h_trk_dphi_pt_lt2_yield_ratio = (TH1D*) h_trk_dphi_pt_lt2_yield_pPb_sig->Clone ("h_trk_dphi_pt_lt2_ratio");
  h_trk_dphi_pt_lt2_yield_ratio->Divide (h_trk_dphi_pt_lt2_yield_pp_sig);

  h_jet_pt_yield_ratio = (TH1D*) h_jet_pt_yield_pPb->Clone ("h_jet_pt_ratio");
  h_jet_pt_yield_ratio->Divide (h_jet_pt_yield_pp);


  TFile* outFile = new TFile ("rootFiles/finalHists.root", "recreate");

  h_trk_pt_ns_yield_pPb->Write ();
  h_trk_pt_ns_yield_pPb_bkg->Write ();
  h_trk_pt_ns_yield_pPb_sig->Write ();
  h_trk_pt_ns_yield_pp->Write ();
  h_trk_pt_ns_yield_pp_bkg->Write ();
  h_trk_pt_ns_yield_pp_sig->Write ();
  h_trk_pt_ns_yield_ratio->Write ();

  h_trk_pt_as_yield_pPb->Write ();
  h_trk_pt_as_yield_pPb_bkg->Write ();
  h_trk_pt_as_yield_pPb_sig->Write ();
  h_trk_pt_as_yield_pp->Write ();
  h_trk_pt_as_yield_pp_bkg->Write ();
  h_trk_pt_as_yield_pp_sig->Write ();
  h_trk_pt_as_yield_ratio->Write ();

  h_trk_dphi_pt_gt2_yield_pPb->Write ();
  h_trk_dphi_pt_gt2_yield_pPb_bkg->Write ();
  h_trk_dphi_pt_gt2_yield_pPb_sig->Write ();
  h_trk_dphi_pt_gt2_yield_pp->Write ();
  h_trk_dphi_pt_gt2_yield_pp_bkg->Write ();
  h_trk_dphi_pt_gt2_yield_pp_sig->Write ();
  h_trk_dphi_pt_gt2_yield_ratio->Write ();

  h_trk_dphi_pt_lt2_yield_pPb->Write ();
  h_trk_dphi_pt_lt2_yield_pPb_bkg->Write ();
  h_trk_dphi_pt_lt2_yield_pPb_sig->Write ();
  h_trk_dphi_pt_lt2_yield_pp->Write ();
  h_trk_dphi_pt_lt2_yield_pp_bkg->Write ();
  h_trk_dphi_pt_lt2_yield_pp_sig->Write ();
  h_trk_dphi_pt_lt2_yield_ratio->Write ();

  h_jet_pt_yield_pPb->Write ();
  h_jet_pt_yield_pp->Write ();
  h_jet_pt_yield_ratio->Write ();

  outFile->Close ();

  return 0;
}
