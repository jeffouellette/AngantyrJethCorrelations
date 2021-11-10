#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>

#include <iostream>

#include <Utilities.h>
#include <ArrayTemplates.h>

#include "header.h"

using namespace PythiaAngantyrStudy;

typedef TGraphAsymmErrors TGAE;

TH1D**  h_trk_pt_yield_pp       = nullptr;
TH1D**  h_trk_pt_yield_pp_bkg   = nullptr;
TH1D**  h_trk_pt_yield_pp_sig   = nullptr;
TH1D*** h_trk_pt_yield_pPb      = nullptr;
TH1D*** h_trk_pt_yield_pPb_bkg  = nullptr;
TH1D*** h_trk_pt_yield_pPb_sig  = nullptr;
TH1D*** h_trk_pt_yield_ratio    = nullptr;

TH1D**  h_trk_dphi_yield_pp       = nullptr;
TH1D**  h_trk_dphi_yield_pp_bkg   = nullptr;
TH1D**  h_trk_dphi_yield_pp_sig   = nullptr;
TH1D*** h_trk_dphi_yield_pPb      = nullptr;
TH1D*** h_trk_dphi_yield_pPb_bkg  = nullptr;
TH1D*** h_trk_dphi_yield_pPb_sig  = nullptr;
TH1D*** h_trk_dphi_yield_ratio    = nullptr;

TH1D*   h_jet_pt_yield_pp     = nullptr;
TH1D**  h_jet_pt_yield_pPb    = nullptr;
TH1D**  h_jet_pt_yield_ratio  = nullptr;

TH1D*   h_jet_yield_pp  = nullptr;
TH1D**  h_jet_yield_pPb = nullptr;


int main () {

  const short nCentBins = npPbNcollBins;

  h_trk_pt_yield_pp       = Get1DArray <TH1D*> (3);
  h_trk_pt_yield_pp_bkg   = Get1DArray <TH1D*> (3);
  h_trk_pt_yield_pp_sig   = Get1DArray <TH1D*> (3);
  h_trk_pt_yield_pPb      = Get2DArray <TH1D*> (3, nCentBins);
  h_trk_pt_yield_pPb_bkg  = Get2DArray <TH1D*> (3, nCentBins);
  h_trk_pt_yield_pPb_sig  = Get2DArray <TH1D*> (3, nCentBins);
  h_trk_pt_yield_ratio    = Get2DArray <TH1D*> (3, nCentBins);
  
  h_trk_dphi_yield_pp       = Get1DArray <TH1D*> (nRedPthBins);
  h_trk_dphi_yield_pp_bkg   = Get1DArray <TH1D*> (nRedPthBins);
  h_trk_dphi_yield_pp_sig   = Get1DArray <TH1D*> (nRedPthBins);
  h_trk_dphi_yield_pPb      = Get2DArray <TH1D*> (nRedPthBins, nCentBins);
  h_trk_dphi_yield_pPb_bkg  = Get2DArray <TH1D*> (nRedPthBins, nCentBins);
  h_trk_dphi_yield_pPb_sig  = Get2DArray <TH1D*> (nRedPthBins, nCentBins);
  h_trk_dphi_yield_ratio    = Get2DArray <TH1D*> (nRedPthBins, nCentBins);
  
  h_jet_pt_yield_pp     = nullptr;
  h_jet_pt_yield_pPb    = Get1DArray <TH1D*> (nCentBins);
  h_jet_pt_yield_ratio  = Get1DArray <TH1D*> (nCentBins);
  
  h_jet_yield_pp  = nullptr;
  h_jet_yield_pPb = Get1DArray <TH1D*> (nCentBins);



  TFile* inFile = nullptr;
  TFile* outFile = new TFile ("rootFiles/finalHists.root", "recreate");


  {
    inFile = new TFile ("rootFiles/pp/hists.root", "read");

    for (int iDPhi = 0; iDPhi < 3; iDPhi++) {
      const std::string dphiStr = (iDPhi == 0 ? "ns" : (iDPhi == 1 ? "perp" : "as"));

      h_trk_pt_yield_pp[iDPhi] = (TH1D*) inFile->Get (Form ("h_trk_pt_%s_yield_pp", dphiStr.c_str ()))->Clone (Form ("h_trk_pt_%s_pp", dphiStr.c_str ()));
    }

    for (int iPth = 0; iPth < nRedPthBins; iPth++) {
      const std::string pthStr = GetRedPthStr (iPth);

      h_trk_dphi_yield_pp[iPth] = (TH1D*) inFile->Get (Form ("h_trk_dphi_%s_yield_pp", pthStr.c_str ()))->Clone (Form ("h_trk_dphi_%s_pp", pthStr.c_str ()));
    }

    h_jet_pt_yield_pp = (TH1D*) inFile->Get ("h_jet_pt_yield_pp")->Clone ("h_jet_pt_pp");

    h_jet_yield_pp = (TH1D*) inFile->Get ("h_jet_yield_pp");
  }


  {
    inFile = new TFile ("rootFiles/pp/hists_bkg.root", "read");

    for (int iDPhi = 0; iDPhi < 3; iDPhi++) {
      const std::string dphiStr = (iDPhi == 0 ? "ns" : (iDPhi == 1 ? "perp" : "as"));

      h_trk_pt_yield_pp_bkg[iDPhi] = (TH1D*) inFile->Get (Form ("h_trk_pt_%s_yield_pp", dphiStr.c_str ()))->Clone (Form ("h_trk_pt_%s_pp_bkg", dphiStr.c_str ()));
    }

    for (int iPth = 0; iPth < nRedPthBins; iPth++) {
      const std::string pthStr = GetRedPthStr (iPth);

      h_trk_dphi_yield_pp_bkg[iPth] = (TH1D*) inFile->Get (Form ("h_trk_dphi_%s_yield_pp", pthStr.c_str ()))->Clone (Form ("h_trk_dphi_%s_pp_bkg", pthStr.c_str ()));
    }
  }


  {
    inFile = new TFile ("rootFiles/pPb/hists.root", "read");

    for (int iCent = 0; iCent < nCentBins; iCent++) {
      const std::string centStr = Form ("cent%i", iCent);

      for (int iDPhi = 0; iDPhi < 3; iDPhi++) {
        const std::string dphiStr = (iDPhi == 0 ? "ns" : (iDPhi == 1 ? "perp" : "as"));

        h_trk_pt_yield_pPb[iDPhi][iCent] = (TH1D*) inFile->Get (Form ("h_trk_pt_%s_yield_cent%i_pPb", dphiStr.c_str (), iCent))->Clone (Form ("h_trk_pt_%s_cent%i_pPb", dphiStr.c_str (), iCent));
      }

      for (int iPth = 0; iPth < nRedPthBins; iPth++) {
        const std::string pthStr = GetRedPthStr (iPth);

        h_trk_dphi_yield_pPb[iPth][iCent] = (TH1D*) inFile->Get (Form ("h_trk_dphi_%s_yield_cent%i_pPb", pthStr.c_str (), iCent))->Clone (Form ("h_trk_dphi_%s_cent%i_pPb", pthStr.c_str (), iCent));
      }

      h_jet_pt_yield_pPb[iCent] = (TH1D*) inFile->Get (Form ("h_jet_pt_yield_cent%i_pPb", iCent))->Clone (Form ("h_jet_pt_cent%i_pPb", iCent));

      h_jet_yield_pPb[iCent] = (TH1D*) inFile->Get (Form ("h_jet_yield_cent%i_pPb", iCent));
    }
  }


  {
    inFile = new TFile ("rootFiles/pPb/hists_bkg.root", "read");

    for (int iCent = 0; iCent < nCentBins; iCent++) {
      const std::string centStr = Form ("cent%i", iCent);

      for (int iDPhi = 0; iDPhi < 3; iDPhi++) {
        const std::string dphiStr = (iDPhi == 0 ? "ns" : (iDPhi == 1 ? "perp" : "as"));

        h_trk_pt_yield_pPb_bkg[iDPhi][iCent] = (TH1D*) inFile->Get (Form ("h_trk_pt_%s_yield_cent%i_pPb", dphiStr.c_str (), iCent))->Clone (Form ("h_trk_pt_%s_cent%i_pPb_bkg", dphiStr.c_str (), iCent));
      }

      for (int iPth = 0; iPth < nRedPthBins; iPth++) {
        const std::string pthStr = GetRedPthStr (iPth);

        h_trk_dphi_yield_pPb_bkg[iPth][iCent] = (TH1D*) inFile->Get (Form ("h_trk_dphi_%s_yield_cent%i_pPb", pthStr.c_str (), iCent))->Clone (Form ("h_trk_dphi_%s_cent%i_pPb_bkg", pthStr.c_str (), iCent));
      }
    }
  }



  outFile->cd ();


 
  for (int iDPhi = 0; iDPhi < 3; iDPhi++) {
    const std::string dphiStr = (iDPhi == 0 ? "ns" : (iDPhi == 1 ? "perp" : "as"));

    h_trk_pt_yield_pp_sig[iDPhi] = (TH1D*) h_trk_pt_yield_pp[iDPhi]->Clone (Form ("h_trk_pt_%s_pp_sig", dphiStr.c_str ()));
    //h_trk_pt_yield_pp_sig[iDPhi]->Add (h_trk_pt_yield_pp_bkg[iDPhi], -1);

    for (int iCent = 0; iCent < nCentBins; iCent++) {
      const std::string centStr = Form ("cent%i", iCent);

      h_trk_pt_yield_pPb_sig[iDPhi][iCent] = (TH1D*) h_trk_pt_yield_pPb[iDPhi][iCent]->Clone (Form ("h_trk_pt_%s_cent%i_pPb_sig", dphiStr.c_str (), iCent));
      //h_trk_pt_yield_pPb_sig[iDPhi][iCent]->Add (h_trk_pt_yield_pPb_bkg[iDPhi][iCent], -1);

      h_trk_pt_yield_ratio[iDPhi][iCent] = (TH1D*) h_trk_pt_yield_pPb_sig[iDPhi][iCent]->Clone (Form ("h_trk_pt_%s_cent%i_ratio", dphiStr.c_str (), iCent));
      h_trk_pt_yield_ratio[iDPhi][iCent]->Divide (h_trk_pt_yield_pp_sig[iDPhi]);
    }
  }

  for (int iPth = 0; iPth < nRedPthBins; iPth++) {
    const std::string pthStr = GetRedPthStr (iPth);

    h_trk_dphi_yield_pp_sig[iPth] = (TH1D*) h_trk_dphi_yield_pp[iPth]->Clone (Form ("h_trk_dphi_%s_pp_sig", pthStr.c_str ()));
    //h_trk_dphi_yield_pp_sig[iPth]->Add (h_trk_dphi_yield_pp_bkg[iPth], -1);

    for (int iCent = 0; iCent < nCentBins; iCent++) {
      const std::string centStr = Form ("cent%i", iCent);

      h_trk_dphi_yield_pPb_sig[iPth][iCent] = (TH1D*) h_trk_dphi_yield_pPb[iPth][iCent]->Clone (Form ("h_trk_dphi_%s_cent%i_pPb_sig", pthStr.c_str (), iCent));
      //h_trk_dphi_yield_pPb_sig[iPth][iCent]->Add (h_trk_dphi_yield_pPb_bkg[iPth][iCent], -1);

      h_trk_dphi_yield_ratio[iPth][iCent] = (TH1D*) h_trk_dphi_yield_pPb_sig[iPth][iCent]->Clone (Form ("h_trk_dphi_%s_cent%i_ratio", pthStr.c_str (), iCent));
      h_trk_dphi_yield_ratio[iPth][iCent]->Divide (h_trk_dphi_yield_pp_sig[iPth]);
    }
  }

  for (int iCent = 0; iCent < nCentBins; iCent++) {
    const std::string centStr = Form ("cent%i", iCent);

    h_jet_pt_yield_ratio[iCent] = (TH1D*) h_jet_pt_yield_pPb[iCent]->Clone (Form ("h_jet_pt_cent%i_ratio", iCent));
    h_jet_pt_yield_ratio[iCent]->Divide (h_jet_pt_yield_pp);
  }



  for (int iDPhi = 0; iDPhi < 3; iDPhi++) {

    h_trk_pt_yield_pp[iDPhi]->Write ();
    h_trk_pt_yield_pp_bkg[iDPhi]->Write ();
    h_trk_pt_yield_pp_sig[iDPhi]->Write ();

    for (int iCent = 0; iCent < nCentBins; iCent++) {

      h_trk_pt_yield_pPb[iDPhi][iCent]->Write ();
      h_trk_pt_yield_pPb_bkg[iDPhi][iCent]->Write ();
      h_trk_pt_yield_pPb_sig[iDPhi][iCent]->Write ();
      h_trk_pt_yield_ratio[iDPhi][iCent]->Write ();

    }
  }


  for (int iPth = 0; iPth < nRedPthBins; iPth++) {

    h_trk_dphi_yield_pp[iPth]->Write ();
    h_trk_dphi_yield_pp_bkg[iPth]->Write ();
    h_trk_dphi_yield_pp_sig[iPth]->Write ();

    for (int iCent = 0; iCent < nCentBins; iCent++) {

      h_trk_dphi_yield_pPb[iPth][iCent]->Write ();
      h_trk_dphi_yield_pPb_bkg[iPth][iCent]->Write ();
      h_trk_dphi_yield_pPb_sig[iPth][iCent]->Write ();
      h_trk_dphi_yield_ratio[iPth][iCent]->Write ();

    }
  }


  h_jet_pt_yield_pp->Write ();

  for (int iCent = 0; iCent < nCentBins; iCent++) {

    h_jet_pt_yield_pPb[iCent]->Write ();
    h_jet_pt_yield_ratio[iCent]->Write ();

  }


  outFile->Close ();

  return 0;
}
