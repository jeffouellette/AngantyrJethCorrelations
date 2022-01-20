#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TF1.h>

#include <iostream>

#include <Utilities.h>
#include <ArrayTemplates.h>

#include "header.h"

using namespace PythiaAngantyrStudy;

typedef TGraphAsymmErrors TGAE;

TH1D****  h_trk_pt_yield_pp       = nullptr;
TH1D**    h_trk_pt_yield_pp_bkg   = nullptr;
TH1D****  h_trk_pt_yield_pp_sig   = nullptr;
TH1D***** h_trk_pt_yield_pPb      = nullptr;
TH1D***   h_trk_pt_yield_pPb_bkg  = nullptr;
TH1D***** h_trk_pt_yield_pPb_sig  = nullptr;
TH1D***** h_trk_pt_yield_ratio    = nullptr;
TF1*****  f_trk_pt_yield_ratio    = nullptr;

TH1D****  h_trk_dphi_yield_pp       = nullptr;
TH1D**    h_trk_dphi_yield_pp_bkg   = nullptr;
TH1D****  h_trk_dphi_yield_pp_sig   = nullptr;
TH1D***** h_trk_dphi_yield_pPb      = nullptr;
TH1D***   h_trk_dphi_yield_pPb_bkg  = nullptr;
TH1D***** h_trk_dphi_yield_pPb_sig  = nullptr;
TH1D***** h_trk_dphi_yield_ratio    = nullptr;

TH1D***  h_jet_pt_yield_pp     = nullptr;
TH1D**** h_jet_pt_yield_pPb    = nullptr;
TH1D**** h_jet_pt_yield_ratio  = nullptr;


std::vector <TString> pp_configs = {"_allowRescatter"}; // {"_allowRescatter", ""};
std::vector <TString> pPb_configs = {"_allowRescatter_withNPDF", "_allowRescatter"}; // {"_allowRescatter_withNPDF", "_allowRescatter", "_withNPDF", ""};

short GetppDen (TString c) {
  if (c == "_allowRescatter" || c == "_allowRescatter_withNPDF")
    return 0;
  if (c == "_withNPDF" || c == "")
    return 1;
  return -1;
}



int main () {

  const short nCentBins = npPbNcollBins + 1;

  h_trk_pt_yield_pp       = Get3DArray <TH1D*> (2, 3, 2); // iPtHat, iDphi, config (allowRescatter, noRescatter)
  h_trk_pt_yield_pp_bkg   = Get1DArray <TH1D*> (3);
  h_trk_pt_yield_pp_sig   = Get3DArray <TH1D*> (2, 3, 2);
  h_trk_pt_yield_pPb      = Get4DArray <TH1D*> (2, 3, nCentBins, 4); // iPtHat, iDphi, cent, config (allow rescatter with nPDF, allow rescatter & no nPDF, no rescatter with nPDF, no rescatter & no nPDF)
  h_trk_pt_yield_pPb_bkg  = Get2DArray <TH1D*> (3, nCentBins);
  h_trk_pt_yield_pPb_sig  = Get4DArray <TH1D*> (2, 3, nCentBins, 4);
  h_trk_pt_yield_ratio    = Get4DArray <TH1D*> (2, 3, nCentBins, 4);
  f_trk_pt_yield_ratio    = Get4DArray <TF1*>  (2, 3, nCentBins, 4);
  
  h_trk_dphi_yield_pp       = Get3DArray <TH1D*> (2, nRedPthBins, 2);
  h_trk_dphi_yield_pp_bkg   = Get1DArray <TH1D*> (nRedPthBins);
  h_trk_dphi_yield_pp_sig   = Get3DArray <TH1D*> (2, nRedPthBins, 2);
  h_trk_dphi_yield_pPb      = Get4DArray <TH1D*> (2, nRedPthBins, nCentBins, 4);
  h_trk_dphi_yield_pPb_bkg  = Get2DArray <TH1D*> (nRedPthBins, nCentBins);
  h_trk_dphi_yield_pPb_sig  = Get4DArray <TH1D*> (2, nRedPthBins, nCentBins, 4);
  h_trk_dphi_yield_ratio    = Get4DArray <TH1D*> (2, nRedPthBins, nCentBins, 4);
  
  h_jet_pt_yield_pp     = Get2DArray <TH1D*> (2, 2); // iPtHat, config
  h_jet_pt_yield_pPb    = Get3DArray <TH1D*> (2, nCentBins, 4); // iPtHat, cent, config
  h_jet_pt_yield_ratio  = Get3DArray <TH1D*> (2, nCentBins, 4);
  


  TFile* inFile = nullptr;
  TFile* outFile = new TFile ("rootFiles/finalHists.root", "recreate");


  for (short iPtHat : {0, 1}) {
    const int pTHat = iPtHat == 0 ? 20 : 50;
    short iConfig = 0;
    for (TString config : pp_configs) {

      inFile = new TFile (Form ("rootFiles/pp/hists_HardQCD_pTHat%i%s.root", pTHat, config.Data ()), "read");

      for (int iDPhi = 0; iDPhi < 3; iDPhi++) {
        const std::string dphiStr = (iDPhi == 0 ? "ns" : (iDPhi == 1 ? "perp" : "as"));

        h_trk_pt_yield_pp[iPtHat][iDPhi][iConfig] = (TH1D*) inFile->Get (Form ("h_trk_pt_%s_yield_pp", dphiStr.c_str ()))->Clone (Form ("h_trk_pt_%s_pp%s_pTHat%i", dphiStr.c_str (), config.Data (), pTHat));
      }

      for (int iPth = 0; iPth < nRedPthBins; iPth++) {
        const std::string pthStr = GetRedPthStr (iPth);

        h_trk_dphi_yield_pp[iPtHat][iPth][iConfig] = (TH1D*) inFile->Get (Form ("h_trk_dphi_%s_yield_pp", pthStr.c_str ()))->Clone (Form ("h_trk_dphi_%s_pp%s_pTHat%i", pthStr.c_str (), config.Data (), pTHat));
      }

      h_jet_pt_yield_pp[iPtHat][iConfig] = (TH1D*) inFile->Get ("h_jet_pt_yield_pp")->Clone (Form ("h_jet_pt_pp%s_pTHat%i", config.Data (), pTHat));

      iConfig++;
    } // end loop over config
  } // end loop over iPtHat



  //{
  //  inFile = new TFile ("rootFiles/pp/hists_bkg.root", "read");

  //  for (int iDPhi = 0; iDPhi < 3; iDPhi++) {
  //    const std::string dphiStr = (iDPhi == 0 ? "ns" : (iDPhi == 1 ? "perp" : "as"));

  //    h_trk_pt_yield_pp_bkg[iDPhi] = (TH1D*) inFile->Get (Form ("h_trk_pt_%s_yield_pp", dphiStr.c_str ()))->Clone (Form ("h_trk_pt_%s_pp_bkg", dphiStr.c_str ()));
  //  }

  //  for (int iPth = 0; iPth < nRedPthBins; iPth++) {
  //    const std::string pthStr = GetRedPthStr (iPth);

  //    h_trk_dphi_yield_pp_bkg[iPth] = (TH1D*) inFile->Get (Form ("h_trk_dphi_%s_yield_pp", pthStr.c_str ()))->Clone (Form ("h_trk_dphi_%s_pp_bkg", pthStr.c_str ()));
  //  }
  //}



  for (short iPtHat : {0, 1}) {
    const int pTHat = iPtHat == 0 ? 20 : 50;
    short iConfig = 0;
    for (TString config : pPb_configs) {
      inFile = new TFile (Form ("rootFiles/pPb/hists_HardQCD_pTHat%i%s.root", pTHat, config.Data ()), "read");

      for (int iCent = 0; iCent < nCentBins; iCent++) {
        const std::string centStr = Form ("cent%i", iCent);

        for (int iDPhi = 0; iDPhi < 3; iDPhi++) {
          const std::string dphiStr = (iDPhi == 0 ? "ns" : (iDPhi == 1 ? "perp" : "as"));

          h_trk_pt_yield_pPb[iPtHat][iDPhi][iCent][iConfig] = (TH1D*) inFile->Get (Form ("h_trk_pt_%s_yield_cent%i_pPb", dphiStr.c_str (), iCent))->Clone (Form ("h_trk_pt_%s_cent%i_pPb%s_pTHat%i", dphiStr.c_str (), iCent, config.Data (), pTHat));
        }

        for (int iPth = 0; iPth < nRedPthBins; iPth++) {
          const std::string pthStr = GetRedPthStr (iPth);

          h_trk_dphi_yield_pPb[iPtHat][iPth][iCent][iConfig] = (TH1D*) inFile->Get (Form ("h_trk_dphi_%s_yield_cent%i_pPb", pthStr.c_str (), iCent))->Clone (Form ("h_trk_dphi_%s_cent%i_pPb%s_pTHat%i", pthStr.c_str (), iCent, config.Data (), pTHat));
        }

        h_jet_pt_yield_pPb[iPtHat][iCent][iConfig] = (TH1D*) inFile->Get (Form ("h_jet_pt_yield_cent%i_pPb", iCent))->Clone (Form ("h_jet_pt_cent%i_pPb%s_pTHat%i", iCent, config.Data (), pTHat));
      }

      iConfig++;
    } // end loop over config
  } // end loop over iPtHat



  //{
  //  inFile = new TFile ("rootFiles/pPb/hists_bkg.root", "read");

  //  for (int iCent = 0; iCent < nCentBins; iCent++) {
  //    const std::string centStr = Form ("cent%i", iCent);

  //    for (int iDPhi = 0; iDPhi < 3; iDPhi++) {
  //      const std::string dphiStr = (iDPhi == 0 ? "ns" : (iDPhi == 1 ? "perp" : "as"));

  //      h_trk_pt_yield_pPb_bkg[iDPhi][iCent] = (TH1D*) inFile->Get (Form ("h_trk_pt_%s_yield_cent%i_pPb", dphiStr.c_str (), iCent))->Clone (Form ("h_trk_pt_%s_cent%i_pPb_bkg", dphiStr.c_str (), iCent));
  //    }

  //    for (int iPth = 0; iPth < nRedPthBins; iPth++) {
  //      const std::string pthStr = GetRedPthStr (iPth);

  //      h_trk_dphi_yield_pPb_bkg[iPth][iCent] = (TH1D*) inFile->Get (Form ("h_trk_dphi_%s_yield_cent%i_pPb", pthStr.c_str (), iCent))->Clone (Form ("h_trk_dphi_%s_cent%i_pPb_bkg", pthStr.c_str (), iCent));
  //    }
  //  }
  //}



  outFile->cd ();


 
  // get pTh yield ratios
  for (short iPtHat : {0, 1}) {
    const int pTHat = iPtHat == 0 ? 20 : 50;

    for (int iDPhi = 0; iDPhi < 3; iDPhi++) {
      const std::string dphiStr = (iDPhi == 0 ? "ns" : (iDPhi == 1 ? "perp" : "as"));

      short iConfig = 0;
      for (TString config : pp_configs) {
        h_trk_pt_yield_pp_sig[iPtHat][iDPhi][iConfig] = (TH1D*) h_trk_pt_yield_pp[iPtHat][iDPhi][iConfig]->Clone (Form ("h_trk_pt_%s_pp_sig%s_pTHat%i", dphiStr.c_str (), config.Data (), pTHat));
        //h_trk_pt_yield_pp_sig[iPtHat][iDPhi][iConfig]->Add (h_trk_pt_yield_pp_bkg[iDPhi], -1);
        iConfig++;
      }

      for (int iCent = 0; iCent < nCentBins; iCent++) {
        const std::string centStr = Form ("cent%i", iCent);

        iConfig = 0;
        for (TString config : pPb_configs) {
          h_trk_pt_yield_pPb_sig[iPtHat][iDPhi][iCent][iConfig] = (TH1D*) h_trk_pt_yield_pPb[iPtHat][iDPhi][iCent][iConfig]->Clone (Form ("h_trk_pt_%s_cent%i_pPb_sig%s_pTHat%i", dphiStr.c_str (), iCent, config.Data (), pTHat));
          //h_trk_pt_yield_pPb_sig[iPtHat][iDPhi][iCent][iConfig]->Add (h_trk_pt_yield_pPb_bkg[iDPhi][iCent], -1);

          h_trk_pt_yield_ratio[iPtHat][iDPhi][iCent][iConfig] = (TH1D*) h_trk_pt_yield_pPb_sig[iPtHat][iDPhi][iCent][iConfig]->Clone (Form ("h_trk_pt_%s_cent%i_ratio%s_pTHat%i", dphiStr.c_str (), iCent, config.Data (), pTHat));
          h_trk_pt_yield_ratio[iPtHat][iDPhi][iCent][iConfig]->Divide (h_trk_pt_yield_pp_sig[iPtHat][iDPhi][GetppDen (config)]);

          f_trk_pt_yield_ratio[iPtHat][iDPhi][iCent][iConfig] = new TF1 (Form ("f_trk_pt_%s_cent%i_ratio%s_pTHat%i", dphiStr.c_str (), iCent, config.Data (), pTHat), "[0]+[1]*log(x)+[2]*pow(log(x),2)", 4, 60);
          h_trk_pt_yield_ratio[iPtHat][iDPhi][iCent][iConfig]->Fit (f_trk_pt_yield_ratio[iPtHat][iDPhi][iCent][iConfig], "RN0Q");
          iConfig++;
        }
      }
    } // end loop over iDPhi


    // get dphi yield ratios
    for (int iPth = 0; iPth < nRedPthBins; iPth++) {
      const std::string pthStr = GetRedPthStr (iPth);

      short iConfig = 0;
      for (TString config : pp_configs) {
        h_trk_dphi_yield_pp_sig[iPtHat][iPth][iConfig] = (TH1D*) h_trk_dphi_yield_pp[iPtHat][iPth][iConfig]->Clone (Form ("h_trk_dphi_%s_pp_sig%s_pTHat%i", pthStr.c_str (), config.Data (), pTHat));
        //h_trk_dphi_yield_pp_sig[iPtHat][iPth][iConfig]->Add (h_trk_dphi_yield_pp_bkg[iPth], -1);
        iConfig++;
      }

      for (int iCent = 0; iCent < nCentBins; iCent++) {
        const std::string centStr = Form ("cent%i", iCent);

        iConfig = 0;
        for (TString config : pPb_configs) {
          h_trk_dphi_yield_pPb_sig[iPtHat][iPth][iCent][iConfig] = (TH1D*) h_trk_dphi_yield_pPb[iPtHat][iPth][iCent][iConfig]->Clone (Form ("h_trk_dphi_%s_cent%i_pPb_sig%s_pTHat%i", pthStr.c_str (), iCent, config.Data (), pTHat));
          //h_trk_dphi_yield_pPb_sig[iPtHat][iPth][iCent][iConfig]->Add (h_trk_dphi_yield_pPb_bkg[iPth][iCent], -1);

          h_trk_dphi_yield_ratio[iPtHat][iPth][iCent][iConfig] = (TH1D*) h_trk_dphi_yield_pPb_sig[iPtHat][iPth][iCent][iConfig]->Clone (Form ("h_trk_dphi_%s_cent%i_ratio%s_pTHat%i", pthStr.c_str (), iCent, config.Data (), pTHat));
          h_trk_dphi_yield_ratio[iPtHat][iPth][iCent][iConfig]->Divide (h_trk_dphi_yield_pp_sig[iPtHat][iPth][GetppDen (config)]);
          iConfig++;
        }
      }
    } // end loop over iPth


    // get jet spectrum ratios
    for (int iCent = 0; iCent < nCentBins; iCent++) {
      const std::string centStr = Form ("cent%i", iCent);

      short iConfig = 0;
      for (TString config : pPb_configs) {
        h_jet_pt_yield_ratio[iPtHat][iCent][iConfig] = (TH1D*) h_jet_pt_yield_pPb[iPtHat][iCent][iConfig]->Clone (Form ("h_jet_pt_cent%i_ratio%s_pTHat%i", iCent, config.Data (), pTHat));
        h_jet_pt_yield_ratio[iPtHat][iCent][iConfig]->Divide (h_jet_pt_yield_pp[iPtHat][GetppDen (config)]);
        iConfig++;
      }
    } // end loop over iCent

  } // end loop over iPtHat




  // write everything to output file
  for (short iPtHat : {0, 1}) {
    for (int iDPhi = 0; iDPhi < 3; iDPhi++) {

      short iConfig = 0;
      for (TString config : pp_configs) {
        h_trk_pt_yield_pp[iPtHat][iDPhi][iConfig]->Write ();
        h_trk_pt_yield_pp_sig[iPtHat][iDPhi][iConfig]->Write ();
        iConfig++;
      }
      //h_trk_pt_yield_pp_bkg[iDPhi]->Write ();

      for (int iCent = 0; iCent < nCentBins; iCent++) {

        iConfig = 0;
        for (TString config : pPb_configs) {
          h_trk_pt_yield_pPb[iPtHat][iDPhi][iCent][iConfig]->Write ();
          h_trk_pt_yield_pPb_sig[iPtHat][iDPhi][iCent][iConfig]->Write ();
          h_trk_pt_yield_ratio[iPtHat][iDPhi][iCent][iConfig]->Write ();
          f_trk_pt_yield_ratio[iPtHat][iDPhi][iCent][iConfig]->Write ();
          iConfig++;
        }
        //h_trk_pt_yield_pPb_bkg[iDPhi][iCent][iConfig]->Write ();

      }
    }


    for (int iPth = 0; iPth < nRedPthBins; iPth++) {

      short iConfig = 0;
      for (TString config : pp_configs) {
        h_trk_dphi_yield_pp[iPtHat][iPth][iConfig]->Write ();
        h_trk_dphi_yield_pp_sig[iPtHat][iPth][iConfig]->Write ();
        iConfig++;
      }
      //h_trk_dphi_yield_pp_bkg[iPth]->Write ();

      for (int iCent = 0; iCent < nCentBins; iCent++) {

        iConfig = 0;
        for (TString config : pPb_configs) {
          h_trk_dphi_yield_pPb[iPtHat][iPth][iCent][iConfig]->Write ();
          h_trk_dphi_yield_pPb_sig[iPtHat][iPth][iCent][iConfig]->Write ();
          h_trk_dphi_yield_ratio[iPtHat][iPth][iCent][iConfig]->Write ();
          iConfig++;
        }
        //h_trk_dphi_yield_pPb_bkg[iPth][iCent]->Write ();

      }
    }


    short iConfig = 0;
    for (TString config : pp_configs) {
      h_jet_pt_yield_pp[iPtHat][iConfig]->Write ();
    }

    for (int iCent = 0; iCent < nCentBins; iCent++) {

      iConfig = 0;
      for (TString config : pPb_configs) {
        h_jet_pt_yield_pPb[iPtHat][iCent][iConfig]->Write ();
        h_jet_pt_yield_ratio[iPtHat][iCent][iConfig]->Write ();
        iConfig++;
      }

    }
  }


  outFile->Close ();

  return 0;
}
