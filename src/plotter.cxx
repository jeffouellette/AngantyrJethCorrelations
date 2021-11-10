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

#include <Utilities.h>
#include <ArrayTemplates.h>
#include <MyColors.h>

#include "header.h"

using namespace PythiaAngantyrStudy;

typedef TGraphAsymmErrors TGAE;

TH1D**  h_trk_pt_yield_pp      = nullptr;
TH1D**  h_trk_pt_yield_pp_bkg  = nullptr;
TH1D**  h_trk_pt_yield_pp_sig  = nullptr;
TH1D*** h_trk_pt_yield_pPb     = nullptr;
TH1D*** h_trk_pt_yield_pPb_bkg = nullptr;
TH1D*** h_trk_pt_yield_pPb_sig = nullptr;
TH1D*** h_trk_pt_yield_ratio   = nullptr;

TH1D**  h_trk_dphi_yield_pp       = nullptr;
TH1D**  h_trk_dphi_yield_pp_bkg   = nullptr;
TH1D**  h_trk_dphi_yield_pp_sig   = nullptr;
TH1D*** h_trk_dphi_yield_pPb      = nullptr;
TH1D*** h_trk_dphi_yield_pPb_bkg  = nullptr;
TH1D*** h_trk_dphi_yield_pPb_sig  = nullptr;
TH1D*** h_trk_dphi_yield_ratio    = nullptr;

TH1D*  h_jet_pt_yield_pp    = nullptr;
TH1D** h_jet_pt_yield_pPb   = nullptr;
TH1D** h_jet_pt_yield_ratio = nullptr;


int main () {

  const short nCentBins = npPbNcollBins;

  h_trk_pt_yield_pp      = Get1DArray <TH1D*> (3);
  h_trk_pt_yield_pp_bkg  = Get1DArray <TH1D*> (3);
  h_trk_pt_yield_pp_sig  = Get1DArray <TH1D*> (3);
  h_trk_pt_yield_pPb     = Get2DArray <TH1D*> (3, nCentBins);
  h_trk_pt_yield_pPb_bkg = Get2DArray <TH1D*> (3, nCentBins);
  h_trk_pt_yield_pPb_sig = Get2DArray <TH1D*> (3, nCentBins);
  h_trk_pt_yield_ratio   = Get2DArray <TH1D*> (3, nCentBins); // ratio of track yield in a jet / all tracks
  
  h_trk_dphi_yield_pp       = Get1DArray <TH1D*> (nRedPthBins);
  h_trk_dphi_yield_pp_bkg   = Get1DArray <TH1D*> (nRedPthBins);
  h_trk_dphi_yield_pp_sig   = Get1DArray <TH1D*> (nRedPthBins);
  h_trk_dphi_yield_pPb      = Get2DArray <TH1D*> (nRedPthBins, nCentBins);
  h_trk_dphi_yield_pPb_bkg  = Get2DArray <TH1D*> (nRedPthBins, nCentBins);
  h_trk_dphi_yield_pPb_sig  = Get2DArray <TH1D*> (nRedPthBins, nCentBins);
  h_trk_dphi_yield_ratio    = Get2DArray <TH1D*> (nRedPthBins, nCentBins);
  
  h_jet_pt_yield_pp    = nullptr;
  h_jet_pt_yield_pPb   = Get1DArray <TH1D*> (nCentBins);
  h_jet_pt_yield_ratio = Get1DArray <TH1D*> (nCentBins);


  SetAtlasStyle(); 

  TFile* inFile = nullptr;


  inFile = new TFile ("rootFiles/finalHists.root", "read");
  for (int iDPhi = 0; iDPhi < 3; iDPhi++) {
    const std::string dphiStr = (iDPhi == 0 ? "ns" : (iDPhi == 1 ? "perp" : "as"));

    h_trk_pt_yield_pp[iDPhi] = (TH1D*) inFile->Get (Form ("h_trk_pt_%s_pp", dphiStr.c_str ()));
    h_trk_pt_yield_pp_bkg[iDPhi] = (TH1D*) inFile->Get (Form ("h_trk_pt_%s_pp_bkg", dphiStr.c_str ()));
    h_trk_pt_yield_pp_sig[iDPhi] = (TH1D*) inFile->Get (Form ("h_trk_pt_%s_pp_sig", dphiStr.c_str ()));

    for (int iCent = 0; iCent < nCentBins; iCent++) {
      h_trk_pt_yield_pPb[iDPhi][iCent] = (TH1D*) inFile->Get (Form ("h_trk_pt_%s_cent%i_pPb", dphiStr.c_str (), iCent));
      h_trk_pt_yield_pPb_bkg[iDPhi][iCent] = (TH1D*) inFile->Get (Form ("h_trk_pt_%s_cent%i_pPb_bkg", dphiStr.c_str (), iCent));
      h_trk_pt_yield_pPb_sig[iDPhi][iCent] = (TH1D*) inFile->Get (Form ("h_trk_pt_%s_cent%i_pPb_sig", dphiStr.c_str (), iCent));
      h_trk_pt_yield_ratio[iDPhi][iCent] = (TH1D*) inFile->Get (Form ("h_trk_pt_%s_cent%i_ratio", dphiStr.c_str (), iCent));
    }
  }

  for (int iPth = 0; iPth < nRedPthBins; iPth++) {
    const std::string pthStr = GetRedPthStr (iPth);

    h_trk_dphi_yield_pp[iPth] = (TH1D*) inFile->Get (Form ("h_trk_dphi_%s_pp", pthStr.c_str ()));
    h_trk_dphi_yield_pp_bkg[iPth] = (TH1D*) inFile->Get (Form ("h_trk_dphi_%s_pp_bkg", pthStr.c_str ()));
    h_trk_dphi_yield_pp_sig[iPth] = (TH1D*) inFile->Get (Form ("h_trk_dphi_%s_pp_sig", pthStr.c_str ()));

    for (int iCent = 0; iCent < nCentBins; iCent++) {
      h_trk_dphi_yield_pPb[iPth][iCent] = (TH1D*) inFile->Get (Form ("h_trk_dphi_%s_cent%i_pPb", pthStr.c_str (), iCent));
      h_trk_dphi_yield_pPb_bkg[iPth][iCent] = (TH1D*) inFile->Get (Form ("h_trk_dphi_%s_cent%i_pPb_bkg", pthStr.c_str (), iCent));
      h_trk_dphi_yield_pPb_sig[iPth][iCent] = (TH1D*) inFile->Get (Form ("h_trk_dphi_%s_cent%i_pPb_sig", pthStr.c_str (), iCent));
      h_trk_dphi_yield_ratio[iPth][iCent] = (TH1D*) inFile->Get (Form ("h_trk_dphi_%s_cent%i_ratio", pthStr.c_str (), iCent));
    }
  }

  h_jet_pt_yield_pp = (TH1D*) inFile->Get ("h_jet_pt_pp");

  for (int iCent = 0; iCent < nCentBins; iCent++) {
    h_jet_pt_yield_pPb[iCent] = (TH1D*) inFile->Get (Form ("h_jet_pt_cent%i_pPb", iCent));
    h_jet_pt_yield_ratio[iCent] = (TH1D*) inFile->Get (Form ("h_jet_pt_cent%i_ratio", iCent));
  }



  for (int iDPhi = 0; iDPhi < 3; iDPhi++) {
    const std::string dphiStr = (iDPhi == 0 ? "ns" : (iDPhi == 1 ? "perp" : "as"));

    const int cent = 3;

    const char* canvasName = Form ("c_trk_pt_%s_yield", dphiStr.c_str ());
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

    uPad->cd (); 
    uPad->SetLogx ();
    uPad->SetLogy ();

    float ymin = 2e-6;
    float ymax = 300;

    h = new TH1D ("htemp", ";#it{p}_{T}^{ch} [GeV];#frac{1}{N_{jet}} #frac{d^{2}N_{ch}}{d#it{p}_{T}^{ch}} [GeV^{-1}]", 1, pthBins[0], pthBins[nPthBins]);
    h->GetYaxis ()->SetRangeUser (ymin, ymax);
    h->GetXaxis ()->SetTitleSize (0);
    h->GetXaxis ()->SetLabelSize (0);
    h->GetYaxis ()->SetTitleSize (0.03/0.7);
    h->GetYaxis ()->SetLabelSize (0.03/0.7);
    h->GetYaxis ()->SetTitleOffset (2.3*0.7);

    h->SetLineWidth (0);
    h->DrawCopy ("hist ][");
    SaferDelete (&h);

    myDraw (h_trk_pt_yield_pPb[iDPhi][cent], myCyan, kFullCircle, 0.8);
    myDraw (h_trk_pt_yield_pp[iDPhi], myPurple, kFullCircle, 0.8);
    //myDraw (h_trk_pt_yield_pPb_bkg[iDPhi][cent], myRed, kFullCircle, 0.8);
    //myDraw (h_trk_pt_yield_pp_bkg[iDPhi], myBlue, kFullCircle, 0.8);

    myText (0.550, 0.880, kBlack, "#bf{Pythia 8.306} Angantyr", 0.028/0.7);
    
    //myText (0.550, 0.830, kBlack, "Signal:", 0.024/0.7);
    myText (0.590, 0.780, kBlack, "HardQCD:all = #bf{on}", 0.024/0.7);
    myText (0.590, 0.740, kBlack, "MPI:allowRescatter = #bf{on}", 0.024/0.7);
    //myText (0.550, 0.700, kBlack, "Background:", 0.024/0.7);
    //myText (0.590, 0.660, kBlack, "SoftQCD:nondiffractive = #bf{on}", 0.024/0.7);
    //myText (0.590, 0.620, kBlack, "MPI:allowRescatter = #bf{off}", 0.024/0.7);
    myText (0.220, 0.230, kBlack, "#it{p}+Pb #bf{0-20\%}, #sqrt{s_{NN}} = 5.02 TeV", 0.028/0.7);
    myText (0.220, 0.180, kBlack, "#it{pp}, #sqrt{s} = 5.02 TeV", 0.028/0.7);
    myText (0.220, 0.130, kBlack, "anti-#it{k}_{T} R=0.4 jets, #it{p}_{T}^{jet} > 60 GeV", 0.028/0.7);      
    std::string dphiDescStr = (iDPhi == 0 ? "#Delta#phi_{ch,jet} < #pi/8" : (iDPhi == 1 ? "|#Delta#phi_{ch,jet}-#pi/2| < #pi/6" : "#Delta#phi_{ch,jet} > 7#pi/8"));
    myText (0.220, 0.080, kBlack, Form ("Charged hadrons, %s", dphiDescStr.c_str ()), 0.028/0.7);

    myLineText2 (0.270, 0.450, myCyan, kFullCircle, "#it{p}+Pb total", 1.0, 0.028/0.7, true);
    //myLineText2 (0.270, 0.400, myRed, kFullCircle, "#it{p}+Pb bkgd.", 1.0, 0.028/0.7, true);
    myLineText2 (0.270, 0.400, myPurple, kFullCircle, "#it{pp} total", 1.0, 0.028/0.7, true);
    //myLineText2 (0.270, 0.300, myBlue, kFullCircle, "#it{pp} bkgd.", 1.0, 0.028/0.7, true);


    dPad->cd (); 
    dPad->SetLogx ();

    ymin = 0.6;
    ymax = 1.4;

    h = new TH1D ("htemp", ";#it{p}_{T}^{ch} [GeV];#it{I}_{#it{p}Pb}", 1, pthBins[0], pthBins[nPthBins]);
    for (int i = 1; i <= h->GetNbinsX (); i++) h->SetBinContent (i, 1);
    h->GetYaxis ()->SetRangeUser (ymin, ymax);
    h->GetXaxis ()->SetTitleSize (0.04/0.3);
    h->GetXaxis ()->SetLabelSize (0.04/0.3);
    h->GetXaxis ()->SetTitleOffset (3.0*0.3);
    h->GetYaxis ()->SetTitleSize (0.03/0.3);
    h->GetYaxis ()->SetLabelSize (0.03/0.3);
    h->GetYaxis ()->SetTitleOffset (2.3*0.3);
    h->GetYaxis ()->CenterTitle ();

    h->SetLineWidth (1);
    h->SetLineStyle (2);
    h->DrawCopy ("hist ][");
    SaferDelete (&h);

    myDraw (h_trk_pt_yield_ratio[iDPhi][cent], kBlack, kFullCircle, 0.8);

    c->SaveAs (Form ("Plots/Ptch_%s_yields_comparison.pdf", dphiStr.c_str ())); 
  }



  {
    const int cent = 3;

    const char* canvasName = "c_jet_pt_yield";
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

    uPad->cd (); 
    uPad->SetLogy ();

    float ymin = 3e3;
    float ymax = 3e6;

    h = new TH1D ("htemp", "", 1, pTJBins[0], pTJBins[nPtJBins]);
    h->GetYaxis ()->SetRangeUser (ymin, ymax);
    h->GetXaxis ()->SetTitleSize (0);
    h->GetXaxis ()->SetLabelSize (0);
    h->GetYaxis ()->SetTitle ("#frac{dN_{jet}}{d#it{p}_{T}^{jet}} [GeV^{-1}]");
    h->GetYaxis ()->SetTitleSize (0.03/0.7);
    h->GetYaxis ()->SetLabelSize (0.03/0.7);
    h->GetYaxis ()->SetTitleOffset (2.3*0.7);

    h->SetLineWidth (0);
    h->DrawCopy ("hist ][");
    SaferDelete (&h);

    myDraw (h_jet_pt_yield_pPb[cent], myCyan, kFullCircle, 0.8);
    myDraw (h_jet_pt_yield_pp, myPurple, kFullCircle, 0.8);

    myText (0.600, 0.880, kBlack, "#bf{Pythia 8.306} Angantyr", 0.028/0.7);
    myText (0.600, 0.830, kBlack, "#it{p}+Pb 0-20\%, #sqrt{s_{NN}} = 5.02 TeV", 0.028/0.7);
    myText (0.600, 0.780, kBlack, "#it{pp}, #sqrt{s} = 5.02 TeV", 0.028/0.7);
    myText (0.600, 0.730, kBlack, "anti-#it{k}_{T} R=0.4 jets", 0.028/0.7);      

    myLineText2 (0.740, 0.680, myCyan, kFullCircle, "#it{p}+Pb", 1.0, 0.028/0.7, true);
    myLineText2 (0.740, 0.630, myPurple, kFullCircle, "#it{pp}", 1.0, 0.028/0.7, true);


    dPad->cd (); 

    ymin = 0.0;
    ymax = 1.1;

    h = new TH1D ("htemp", "", 1, pTJBins[0], pTJBins[nPtJBins]);
    h->SetBinContent (1, 1);
    h->GetYaxis ()->SetRangeUser (ymin, ymax);
    h->GetXaxis ()->SetTitle ("#it{p}_{T}^{jet} [GeV]");
    h->GetXaxis ()->SetTitleSize (0.04/0.3);
    h->GetXaxis ()->SetLabelSize (0.04/0.3);
    h->GetXaxis ()->SetTitleOffset (3.0*0.3);
    //h->GetXaxis ()->SetLabelOffset (-0.04*0.3);
    h->GetYaxis ()->SetTitle (Form ("#frac{#color[%i]{N_{p+Pb}}}{#color[%i]{N_{pp}}}", (int)myCyan, (int)myPurple));
    h->GetYaxis ()->SetTitleSize (0.03/0.3);
    h->GetYaxis ()->SetLabelSize (0.03/0.3);
    h->GetYaxis ()->SetTitleOffset (2.3*0.3);
    h->GetYaxis ()->CenterTitle ();

    h->SetLineWidth (1);
    h->SetLineStyle (2);
    h->DrawCopy ("hist ][");
    SaferDelete (&h);

    myDraw (h_jet_pt_yield_ratio[cent], kBlack, kFullCircle, 0.8);

    c->SaveAs ("Plots/PtJ_yields_comparison.pdf"); 
  }



  for (int iPth = 0; iPth < nRedPthBins; iPth++) {
    const int cent = 3;

    const char* canvasName = Form ("c_trk_dphi_%s_yield", GetRedPthStr (iPth).c_str ());
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

    uPad->cd (); 

    float ymax = 0;
    switch (iPth) {
      case 0: { ymax = 30; break; }
      case 1: { ymax = 20; break; }
      case 2: { ymax = 10; break; }
      case 3: { ymax = 20; break; }
      case 4: { ymax = 10; break; }
      case 5: { ymax = 10; break; }
      case 6: { ymax = 10; break; }
      case 7: { ymax = 2; break; }
      case 8: { ymax = 1; break; }
      default: ymax = 30;
    }
    float ymin = -ymax/30.;

    h = new TH1D ("htemp", "", 1, dPhiBins[0], dPhiBins[nDPhiBins]);
    h->GetYaxis ()->SetRangeUser (ymin, ymax);
    h->GetXaxis ()->SetTitleSize (0);
    h->GetXaxis ()->SetLabelSize (0);
    h->GetYaxis ()->SetTitle ("#frac{1}{N_{jet}} #frac{dN_{ch}}{d#Delta#phi_{ch,jet}}");
    h->GetYaxis ()->SetTitleSize (0.03/0.7);
    h->GetYaxis ()->SetLabelSize (0.03/0.7);
    h->GetYaxis ()->SetTitleOffset (2.3*0.7);

    h->SetLineWidth (1);
    h->SetLineStyle (2);
    h->DrawCopy ("hist ][");
    SaferDelete (&h);

    myDraw (h_trk_dphi_yield_pPb[iPth][cent], myCyan, kFullCircle, 0.8);
    myDraw (h_trk_dphi_yield_pp[iPth], myPurple, kFullCircle, 0.8);
    myDraw (h_trk_dphi_yield_pPb_bkg[iPth][cent], myRed, kFullCircle, 0.8);
    myDraw (h_trk_dphi_yield_pp_bkg[iPth], myBlue, kFullCircle, 0.8);

    myText (0.220, 0.880, kBlack, "#bf{Pythia 8.306} Angantyr", 0.028/0.7);
    myText (0.220, 0.830, kBlack, "#it{p}+Pb 0-20\%, #sqrt{s_{NN}} = 5.02 TeV", 0.028/0.7);
    myText (0.220, 0.780, kBlack, "#it{pp}, #sqrt{s} = 5.02 TeV", 0.028/0.7);
    myText (0.220, 0.730, kBlack, "anti-#it{k}_{T} R=0.4 jets, #it{p}_{T}^{jet} > 60 GeV", 0.028/0.7);      
    myText (0.220, 0.680, kBlack, Form ("Charged hadrons, #it{p}_{T}^{ch} = %g-%g GeV", redPthBins[iPth], redPthBins[iPth+1]), 0.028/0.7);

    myLineText2 (0.270, 0.620, myCyan, kFullCircle, "#it{p}+Pb total", 1.0, 0.028/0.7, true);
    myLineText2 (0.270, 0.560, myRed, kFullCircle, "#it{p}+Pb bkgd.", 1.0, 0.028/0.7, true);
    myLineText2 (0.270, 0.500, myPurple, kFullCircle, "#it{pp} total", 1.0, 0.028/0.7, true);
    myLineText2 (0.270, 0.440, myBlue, kFullCircle, "#it{pp} bkgd.", 1.0, 0.028/0.7, true);
    

    dPad->cd (); 

    ymin = 0.83;
    ymax = 1.17;

    h = new TH1D ("htemp", "", 1, dPhiBins[0], dPhiBins[nDPhiBins]);
    for (int i = 1; i <= h->GetNbinsX (); i++) h->SetBinContent (i, 1);
    h->GetYaxis ()->SetRangeUser (ymin, ymax);
    h->GetXaxis ()->SetTitle ("#Delta#phi_{ch, jet}");
    h->GetXaxis ()->SetTitleSize (0.04/0.3);
    h->GetXaxis ()->SetLabelSize (0.04/0.3);
    h->GetXaxis ()->SetTitleOffset (3.0*0.3);
    //h->GetXaxis ()->SetLabelOffset (-0.04*0.3);
    h->GetYaxis ()->SetTitle (Form ("#frac{#color[%i]{N_{tot}^{p+Pb}} #minus #color[%i]{N_{bkgd}^{p+Pb}}}{#color[%i]{N_{tot}^{pp}} #minus #color[%i]{N_{bkgd}^{pp}}}", (int)myCyan, (int)myRed, (int)myPurple, (int)myBlue));
    h->GetYaxis ()->SetTitleSize (0.03/0.3);
    h->GetYaxis ()->SetLabelSize (0.03/0.3);
    h->GetYaxis ()->SetTitleOffset (2.3*0.3);
    h->GetYaxis ()->CenterTitle ();

    h->SetLineWidth (1);
    h->SetLineStyle (2);
    h->DrawCopy ("hist ][");
    SaferDelete (&h);

    myDraw (h_trk_dphi_yield_ratio[iPth][cent], kBlack, kFullCircle, 0.8);

    c->SaveAs (Form ("Plots/DPhi_%s_yields_comparison.pdf", GetRedPthStr (iPth).c_str ())); 
  }


  return 0;
}


void plotter () {
  main ();
}
