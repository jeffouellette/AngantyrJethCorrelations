#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TF1.h>
#include <TVirtualFitter.h>
#include <TGraphErrors.h>
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

typedef TGraphErrors TGE;

TH1D***  h_trk_pt_yield_pp      = nullptr;
TH1D**   h_trk_pt_yield_pp_bkg  = nullptr;
TH1D***  h_trk_pt_yield_pp_sig  = nullptr;
TH1D**** h_trk_pt_yield_pPb     = nullptr;
TH1D***  h_trk_pt_yield_pPb_bkg = nullptr;
TH1D**** h_trk_pt_yield_pPb_sig = nullptr;
TH1D**** h_trk_pt_yield_ratio   = nullptr;
TF1**** f_trk_pt_yield_ratio    = nullptr;

TH1D***  h_trk_dphi_yield_pp       = nullptr;
TH1D**   h_trk_dphi_yield_pp_bkg   = nullptr;
TH1D***  h_trk_dphi_yield_pp_sig   = nullptr;
TH1D**** h_trk_dphi_yield_pPb      = nullptr;
TH1D***  h_trk_dphi_yield_pPb_bkg  = nullptr;
TH1D**** h_trk_dphi_yield_pPb_sig  = nullptr;
TH1D**** h_trk_dphi_yield_ratio    = nullptr;

TH1D**  h_jet_pt_yield_pp    = nullptr;
TH1D*** h_jet_pt_yield_pPb   = nullptr;
TH1D*** h_jet_pt_yield_ratio = nullptr;


int main () {

  const short nCentBins = npPbNcollBins;

  h_trk_pt_yield_pp      = Get2DArray <TH1D*> (3, 2);
  h_trk_pt_yield_pp_bkg  = Get1DArray <TH1D*> (3);
  h_trk_pt_yield_pp_sig  = Get2DArray <TH1D*> (3, 2);
  h_trk_pt_yield_pPb     = Get3DArray <TH1D*> (3, nCentBins, 4);
  h_trk_pt_yield_pPb_bkg = Get2DArray <TH1D*> (3, nCentBins);
  h_trk_pt_yield_pPb_sig = Get3DArray <TH1D*> (3, nCentBins, 4);
  h_trk_pt_yield_ratio   = Get3DArray <TH1D*> (3, nCentBins, 4); // ratio of track yield in a jet / all tracks
  f_trk_pt_yield_ratio   = Get3DArray <TF1*>  (3, nCentBins, 4); // ratio of track yield in a jet / all tracks
  
  h_trk_dphi_yield_pp       = Get2DArray <TH1D*> (nRedPthBins, 2);
  h_trk_dphi_yield_pp_bkg   = Get1DArray <TH1D*> (nRedPthBins);
  h_trk_dphi_yield_pp_sig   = Get2DArray <TH1D*> (nRedPthBins, 2);
  h_trk_dphi_yield_pPb      = Get3DArray <TH1D*> (nRedPthBins, nCentBins, 4);
  h_trk_dphi_yield_pPb_bkg  = Get2DArray <TH1D*> (nRedPthBins, nCentBins);
  h_trk_dphi_yield_pPb_sig  = Get3DArray <TH1D*> (nRedPthBins, nCentBins, 4);
  h_trk_dphi_yield_ratio    = Get3DArray <TH1D*> (nRedPthBins, nCentBins, 4);
  
  h_jet_pt_yield_pp    = Get1DArray <TH1D*> (2);
  h_jet_pt_yield_pPb   = Get2DArray <TH1D*> (nCentBins, 4);
  h_jet_pt_yield_ratio = Get2DArray <TH1D*> (nCentBins, 4);


  SetAtlasStyle(); 

  TFile* inFile = nullptr;


  inFile = new TFile ("rootFiles/finalHists.root", "read");
  for (int iDPhi = 0; iDPhi < 3; iDPhi++) {
    const std::string dphiStr = (iDPhi == 0 ? "ns" : (iDPhi == 1 ? "perp" : "as"));

    int iConfig = 0;
    for (TString config : {"_allowRescatter", ""}) {
      h_trk_pt_yield_pp[iDPhi][iConfig] = (TH1D*) inFile->Get (Form ("h_trk_pt_%s_pp%s", dphiStr.c_str (), config.Data ()));
      h_trk_pt_yield_pp_sig[iDPhi][iConfig] = (TH1D*) inFile->Get (Form ("h_trk_pt_%s_pp_sig%s", dphiStr.c_str (), config.Data ()));
      iConfig++;
    }
    h_trk_pt_yield_pp_bkg[iDPhi] = (TH1D*) inFile->Get (Form ("h_trk_pt_%s_pp_bkg", dphiStr.c_str ()));

    for (int iCent = 0; iCent < nCentBins; iCent++) {
      iConfig = 0;
      for (TString config : {"_allowRescatter_withNPDF", "_allowRescatter", "_withNPDF", ""}) {
        h_trk_pt_yield_pPb[iDPhi][iCent][iConfig] = (TH1D*) inFile->Get (Form ("h_trk_pt_%s_cent%i_pPb%s", dphiStr.c_str (), iCent, config.Data ()));
        h_trk_pt_yield_pPb_sig[iDPhi][iCent][iConfig] = (TH1D*) inFile->Get (Form ("h_trk_pt_%s_cent%i_pPb_sig%s", dphiStr.c_str (), iCent, config.Data ()));
        h_trk_pt_yield_ratio[iDPhi][iCent][iConfig] = (TH1D*) inFile->Get (Form ("h_trk_pt_%s_cent%i_ratio%s", dphiStr.c_str (), iCent, config.Data ()));
        //f_trk_pt_yield_ratio[iDPhi][iCent][iConfig] = (TF1*) inFile->Get (Form ("f_trk_pt_%s_cent%i_ratio%s", dphiStr.c_str (), iCent, config.Data ()));
        iConfig++;
      }
      h_trk_pt_yield_pPb_bkg[iDPhi][iCent] = (TH1D*) inFile->Get (Form ("h_trk_pt_%s_cent%i_pPb_bkg", dphiStr.c_str (), iCent));
    }
  }

  for (int iPth = 0; iPth < nRedPthBins; iPth++) {
    const std::string pthStr = GetRedPthStr (iPth);

    int iConfig = 0;
    for (TString config : {"_allowRescatter", ""}) {
      h_trk_dphi_yield_pp[iPth][iConfig] = (TH1D*) inFile->Get (Form ("h_trk_dphi_%s_pp%s", pthStr.c_str (), config.Data ()));
      h_trk_dphi_yield_pp_sig[iPth][iConfig] = (TH1D*) inFile->Get (Form ("h_trk_dphi_%s_pp_sig%s", pthStr.c_str (), config.Data ()));
      iConfig++;
    }
    h_trk_dphi_yield_pp_bkg[iPth] = (TH1D*) inFile->Get (Form ("h_trk_dphi_%s_pp_bkg", pthStr.c_str ()));

    for (int iCent = 0; iCent < nCentBins; iCent++) {
      iConfig = 0;
      for (TString config : {"_allowRescatter_withNPDF", "_allowRescatter", "_withNPDF", ""}) {
        h_trk_dphi_yield_pPb[iPth][iCent][iConfig] = (TH1D*) inFile->Get (Form ("h_trk_dphi_%s_cent%i_pPb", pthStr.c_str (), iCent, config.Data ()));
        h_trk_dphi_yield_pPb_sig[iPth][iCent][iConfig] = (TH1D*) inFile->Get (Form ("h_trk_dphi_%s_cent%i_pPb_sig", pthStr.c_str (), iCent, config.Data ()));
        h_trk_dphi_yield_ratio[iPth][iCent][iConfig] = (TH1D*) inFile->Get (Form ("h_trk_dphi_%s_cent%i_ratio", pthStr.c_str (), iCent, config.Data ()));
        iConfig++;
      }
      h_trk_dphi_yield_pPb_bkg[iPth][iCent] = (TH1D*) inFile->Get (Form ("h_trk_dphi_%s_cent%i_pPb_bkg", pthStr.c_str (), iCent));
    }
  }

  int iConfig = 0;
  for (TString config : {"_allowRescatter", ""}) {
    h_jet_pt_yield_pp[iConfig] = (TH1D*) inFile->Get (Form ("h_jet_pt_pp%s", config.Data ()));
    iConfig++;
  }

  for (int iCent = 0; iCent < nCentBins; iCent++) {
    iConfig = 0;
    for (TString config : {"_allowRescatter_withNPDF", "_allowRescatter", "_withNPDF", ""}) {
      h_jet_pt_yield_pPb[iCent][iConfig] = (TH1D*) inFile->Get (Form ("h_jet_pt_cent%i_pPb%s", iCent, config.Data ()));
      h_jet_pt_yield_ratio[iCent][iConfig] = (TH1D*) inFile->Get (Form ("h_jet_pt_cent%i_ratio%s", iCent, config.Data ()));
      iConfig++;
    }
  }



  for (short cent = 0; cent < nCentBins; cent++) {
    for (int iDPhi = 0; iDPhi < 3; iDPhi++) {
      const std::string dphiStr = (iDPhi == 0 ? "ns" : (iDPhi == 1 ? "perp" : "as"));

      const char* canvasName = Form ("c_trk_pt_cent%i_%s_yield", cent, dphiStr.c_str ());
      TCanvas* c = new TCanvas (canvasName, "", 800, 1100);
      const float fPad = 0.4;
      TPad* uPad = new TPad (Form ("%s_uPad", canvasName), "", 0.0, fPad, 1.0, 1.0);
      TPad* dPad = new TPad (Form ("%s_dPad", canvasName), "", 0.0, 0.0, 1.0, fPad);

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

      h = new TH1D ("htemp", ";#it{p}_{T}^{ch} [GeV];#frac{1}{N_{jet}} #frac{dN_{ch}}{d#it{p}_{T}^{ch}} [GeV^{-1}]", 1, pthBins[0], pthBins[nPthBins]);
      h->GetYaxis ()->SetRangeUser (ymin, ymax);
      h->GetXaxis ()->SetTitleSize (0);
      h->GetXaxis ()->SetLabelSize (0);
      h->GetYaxis ()->SetTitleSize (0.03/(1-fPad));
      h->GetYaxis ()->SetLabelSize (0.03/(1-fPad));
      h->GetYaxis ()->SetTitleOffset (2.3*(1-fPad));

      h->SetLineWidth (0);
      h->DrawCopy ("hist ][");
      SaferDelete (&h);

      myDraw (h_trk_pt_yield_pPb[iDPhi][cent][3], colorfulColors[2], kFullCircle, 1.2);
      myDraw (h_trk_pt_yield_pPb[iDPhi][cent][2], colorfulColors[1], kFullCircle, 1.2);
      myDraw (h_trk_pt_yield_pp[iDPhi][1], colorfulColors[0], kFullCircle, 1.2);

      myText (0.550, 0.880, kBlack, "#bf{Pythia 8.306} Angantyr", 0.022/(1-fPad));
      
      myText (0.570, 0.840, kBlack, "HardQCD:all = #bf{on}", 0.022/(1-fPad));
      myText (0.570, 0.800, kBlack, "PDF:useHardNPDFA = #bf{on}", 0.022/(1-fPad));
      //myText (0.570, 0.760, kBlack, "MPI:allowRescatter = #bf{on}", 0.022/(1-fPad));
      myText (0.220, 0.230, kBlack, Form ("#it{p}+Pb #bf{%i-%i\%}, #sqrt{s_{NN}} = 5.02 TeV", pPbNcollPercs[cent+1], pPbNcollPercs[cent]), 0.022/(1-fPad));
      myText (0.220, 0.180, kBlack, "#it{pp}, #sqrt{s} = 5.02 TeV", 0.022/(1-fPad));
      myText (0.220, 0.130, kBlack, "anti-#it{k}_{T} R=0.4 jets, #it{p}_{T}^{jet} > 60 GeV", 0.022/(1-fPad));
      std::string dphiDescStr = (iDPhi == 0 ? "#Delta#phi_{ch,jet} < #pi/8" : (iDPhi == 1 ? "|#Delta#phi_{ch,jet}-#pi/2| < #pi/6" : "#Delta#phi_{ch,jet} > 7#pi/8"));
      myText (0.220, 0.080, kBlack, Form ("Charged hadrons, %s", dphiDescStr.c_str ()), 0.022/(1-fPad));

      myLineText2 (0.270, 0.450, colorfulColors[1], kFullCircle, "#it{p}+Pb", 1.2, 0.022/(1-fPad), true);
      myLineText2 (0.270, 0.400, colorfulColors[2], kFullCircle, "#it{p}+Pb, nPDF off", 1.2, 0.022/(1-fPad), true);
      myLineText2 (0.270, 0.350, colorfulColors[0], kFullCircle, "#it{pp}", 1.2, 0.022/(1-fPad), true);


      dPad->cd (); 
      dPad->SetLogx ();

      ymin = 0.8;
      ymax = 1.2;

      h = new TH1D ("htemp", ";#it{p}_{T}^{ch} [GeV];#it{I}_{#it{p}Pb}", 1, pthBins[0], pthBins[nPthBins]);
      for (int i = 1; i <= h->GetNbinsX (); i++) h->SetBinContent (i, 1);
      h->GetYaxis ()->SetRangeUser (ymin, ymax);
      h->GetXaxis ()->SetTitleSize (0.04/fPad);
      h->GetXaxis ()->SetLabelSize (0.04/fPad);
      h->GetXaxis ()->SetTitleOffset (3.0*fPad);
      h->GetYaxis ()->SetTitleSize (0.03/fPad);
      h->GetYaxis ()->SetLabelSize (0.03/fPad);
      h->GetYaxis ()->SetTitleOffset (2.3*fPad);
      h->GetYaxis ()->CenterTitle ();

      h->SetLineWidth (1);
      h->SetLineStyle (2);
      h->DrawCopy ("hist ][");
      SaferDelete (&h);

      myDraw (h_trk_pt_yield_ratio[iDPhi][cent][3], colorfulColors[2], kFullCircle, 1.2);
      myDraw (h_trk_pt_yield_ratio[iDPhi][cent][2], colorfulColors[1], kFullCircle, 1.2);

      const TString funcForm = "[0]+[1]*log(x)+[2]*pow(log(x),2)";
      f_trk_pt_yield_ratio[iDPhi][cent][3] = new TF1 (Form ("f_trk_pt_%s_cent%i_ratio", dphiStr.c_str (), cent), funcForm.Data (), 4, 60);
      f_trk_pt_yield_ratio[iDPhi][cent][2] = new TF1 (Form ("f_trk_pt_%s_cent%i_ratio_withNPDF", dphiStr.c_str (), cent), funcForm.Data (), 4, 60);

      h_trk_pt_yield_ratio[iDPhi][cent][3]->Fit (f_trk_pt_yield_ratio[iDPhi][cent][3], "RN0Q");
      myDraw (f_trk_pt_yield_ratio[iDPhi][cent][3], colorfulColors[2], 1, 2, "same c");

      //TH1D* h_ci = (TH1D*) h_trk_pt_yield_ratio[iDPhi][cent][3]->Clone ("h_ci_config1");
      //(TVirtualFitter::GetFitter ())->GetConfidenceIntervals (h_ci, 0.6827);
      //h_ci->SetFillColorAlpha (colorfulColors[2], 0.2);
      //h_ci->DrawCopy ("e3 same");
      //SaferDelete (&h_ci);
      TGE* g_ci = TH1ToTGE (h_trk_pt_yield_ratio[iDPhi][cent][3]);
      (TVirtualFitter::GetFitter ())->GetConfidenceIntervals (g_ci, 0.6827);
      TrimGraph (g_ci, 4, 90);
      g_ci->SetFillColorAlpha (colorfulColors[2], 0.2);
      ((TGE*) g_ci->Clone ())->Draw ("3");
      SaferDelete (&g_ci);

      h_trk_pt_yield_ratio[iDPhi][cent][2]->Fit (f_trk_pt_yield_ratio[iDPhi][cent][2], "RN0Q");
      myDraw (f_trk_pt_yield_ratio[iDPhi][cent][2], colorfulColors[1], 1, 2, "same c");

      //h_ci = (TH1D*) h_trk_pt_yield_ratio[iDPhi][cent][2]->Clone ("h_ci_config1");
      //(TVirtualFitter::GetFitter ())->GetConfidenceIntervals (h_ci, 0.6827);
      //h_ci->SetFillColorAlpha (colorfulColors[1], 0.2);
      //h_ci->DrawCopy ("e3 same");
      //SaferDelete (&h_ci);
      g_ci = TH1ToTGE (h_trk_pt_yield_ratio[iDPhi][cent][2]);
      (TVirtualFitter::GetFitter ())->GetConfidenceIntervals (g_ci, 0.6827);
      TrimGraph (g_ci, 4, 90);
      g_ci->SetFillColorAlpha (colorfulColors[1], 0.2);
      ((TGE*) g_ci->Clone ())->Draw ("3");
      SaferDelete (&g_ci);

      c->SaveAs (Form ("Plots/Ptch_%s_cent%i_yields_comparison.pdf", dphiStr.c_str (), cent)); 
    } // end loop over iDPhi
  } // end loop over cent



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

    myDraw (h_jet_pt_yield_pPb[cent][0], myCyan, kFullCircle, 0.8);
    myDraw (h_jet_pt_yield_pp[0], myPurple, kFullCircle, 0.8);

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

    myDraw (h_jet_pt_yield_ratio[cent][0], kBlack, kFullCircle, 0.8);

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

    myDraw (h_trk_dphi_yield_pPb[iPth][cent][0], myCyan, kFullCircle, 0.8);
    myDraw (h_trk_dphi_yield_pp[iPth][0], myPurple, kFullCircle, 0.8);
    //myDraw (h_trk_dphi_yield_pPb_bkg[iPth][cent], myRed, kFullCircle, 0.8);
    //myDraw (h_trk_dphi_yield_pp_bkg[iPth], myBlue, kFullCircle, 0.8);

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

    myDraw (h_trk_dphi_yield_ratio[iPth][cent][0], kBlack, kFullCircle, 0.8);

    c->SaveAs (Form ("Plots/DPhi_%s_yields_comparison.pdf", GetRedPthStr (iPth).c_str ())); 
  }


  return 0;
}


void plotter () {
  main ();
}
