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

TH1D***  h_trk_pt_yield_pp       = nullptr;
TH1D**   h_trk_pt_yield_pp_bkg   = nullptr;
TH1D***  h_trk_pt_yield_pp_sig   = nullptr;
TH1D**** h_trk_pt_yield_pPb      = nullptr;
TH1D***  h_trk_pt_yield_pPb_bkg  = nullptr;
TH1D**** h_trk_pt_yield_pPb_sig  = nullptr;
TH1D**** h_trk_pt_yield_ratio    = nullptr;
TF1****  f_trk_pt_yield_ratio    = nullptr;

TH1D***  h_trk_dphi_yield_pp       = nullptr;
TH1D**   h_trk_dphi_yield_pp_bkg   = nullptr;
TH1D***  h_trk_dphi_yield_pp_sig   = nullptr;
TH1D**** h_trk_dphi_yield_pPb      = nullptr;
TH1D***  h_trk_dphi_yield_pPb_bkg  = nullptr;
TH1D**** h_trk_dphi_yield_pPb_sig  = nullptr;
TH1D**** h_trk_dphi_yield_ratio    = nullptr;

TH1D**  h_jet_pt_yield_pp     = nullptr;
TH1D*** h_jet_pt_yield_pPb    = nullptr;
TH1D*** h_jet_pt_yield_ratio  = nullptr;



int main () {

  const short nCentBins = npPbNcollBins + 1;

  h_trk_pt_yield_pp       = Get2DArray <TH1D*> (3, 2);
  h_trk_pt_yield_pp_bkg   = Get1DArray <TH1D*> (3);
  h_trk_pt_yield_pp_sig   = Get2DArray <TH1D*> (3, 2);
  h_trk_pt_yield_pPb      = Get3DArray <TH1D*> (3, nCentBins, 4);
  h_trk_pt_yield_pPb_bkg  = Get2DArray <TH1D*> (3, nCentBins);
  h_trk_pt_yield_pPb_sig  = Get3DArray <TH1D*> (3, nCentBins, 4);
  h_trk_pt_yield_ratio    = Get3DArray <TH1D*> (3, nCentBins, 4);
  f_trk_pt_yield_ratio    = Get3DArray <TF1*>  (3, nCentBins, 4);
  
  h_trk_dphi_yield_pp       = Get2DArray <TH1D*> (nRedPthBins, 2);
  h_trk_dphi_yield_pp_bkg   = Get1DArray <TH1D*> (nRedPthBins);
  h_trk_dphi_yield_pp_sig   = Get2DArray <TH1D*> (nRedPthBins, 2);
  h_trk_dphi_yield_pPb      = Get3DArray <TH1D*> (nRedPthBins, nCentBins, 4);
  h_trk_dphi_yield_pPb_bkg  = Get2DArray <TH1D*> (nRedPthBins, nCentBins);
  h_trk_dphi_yield_pPb_sig  = Get3DArray <TH1D*> (nRedPthBins, nCentBins, 4);
  h_trk_dphi_yield_ratio    = Get3DArray <TH1D*> (nRedPthBins, nCentBins, 4);
  
  h_jet_pt_yield_pp     = Get1DArray <TH1D*> (2);
  h_jet_pt_yield_pPb    = Get2DArray <TH1D*> (nCentBins, 4);
  h_jet_pt_yield_ratio  = Get2DArray <TH1D*> (nCentBins, 4);
  


  TFile* inFile = nullptr;
  TFile* outFile = new TFile ("rootFiles/finalHists.root", "recreate");


  int iConfig = 0;
  //for (TString config : {"_allowRescatter", ""}) {
  for (TString config : {"_allowRescatter"}) {
    inFile = new TFile (Form ("rootFiles/pp/hists_HardQCD_pTHat50%s.root", config.Data ()), "read");

    for (int iDPhi = 0; iDPhi < 3; iDPhi++) {
      const std::string dphiStr = (iDPhi == 0 ? "ns" : (iDPhi == 1 ? "perp" : "as"));

      h_trk_pt_yield_pp[iDPhi][iConfig] = (TH1D*) inFile->Get (Form ("h_trk_pt_%s_yield_pp", dphiStr.c_str ()))->Clone (Form ("h_trk_pt_%s_pp%s", dphiStr.c_str (), config.Data ()));
    }

    for (int iPth = 0; iPth < nRedPthBins; iPth++) {
      const std::string pthStr = GetRedPthStr (iPth);

      h_trk_dphi_yield_pp[iPth][iConfig] = (TH1D*) inFile->Get (Form ("h_trk_dphi_%s_yield_pp", pthStr.c_str ()))->Clone (Form ("h_trk_dphi_%s_pp%s", pthStr.c_str (), config.Data ()));
    }

    h_jet_pt_yield_pp[iConfig] = (TH1D*) inFile->Get ("h_jet_pt_yield_pp")->Clone (Form ("h_jet_pt_pp%s", config.Data ()));

    iConfig++;
  }



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



  iConfig = 0;
  //for (TString config : {"_allowRescatter_withNPDF", "_allowRescatter", "_withNPDF", ""}) {
  for (TString config : {"_allowRescatter_withNPDF", "_allowRescatter"}) {
    inFile = new TFile (Form ("rootFiles/pPb/hists_HardQCD_pTHat50%s.root", config.Data ()), "read");

    for (int iCent = 0; iCent < nCentBins; iCent++) {
      const std::string centStr = Form ("cent%i", iCent);

      for (int iDPhi = 0; iDPhi < 3; iDPhi++) {
        const std::string dphiStr = (iDPhi == 0 ? "ns" : (iDPhi == 1 ? "perp" : "as"));

        h_trk_pt_yield_pPb[iDPhi][iCent][iConfig] = (TH1D*) inFile->Get (Form ("h_trk_pt_%s_yield_cent%i_pPb", dphiStr.c_str (), iCent))->Clone (Form ("h_trk_pt_%s_cent%i_pPb%s", dphiStr.c_str (), iCent, config.Data ()));
      }

      for (int iPth = 0; iPth < nRedPthBins; iPth++) {
        const std::string pthStr = GetRedPthStr (iPth);

        h_trk_dphi_yield_pPb[iPth][iCent][iConfig] = (TH1D*) inFile->Get (Form ("h_trk_dphi_%s_yield_cent%i_pPb", pthStr.c_str (), iCent))->Clone (Form ("h_trk_dphi_%s_cent%i_pPb%s", pthStr.c_str (), iCent, config.Data ()));
      }

      h_jet_pt_yield_pPb[iCent][iConfig] = (TH1D*) inFile->Get (Form ("h_jet_pt_yield_cent%i_pPb", iCent))->Clone (Form ("h_jet_pt_cent%i_pPb%s", iCent, config.Data ()));
    }

    iConfig++;
  }



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


 
  for (int iDPhi = 0; iDPhi < 3; iDPhi++) {
    const std::string dphiStr = (iDPhi == 0 ? "ns" : (iDPhi == 1 ? "perp" : "as"));

    iConfig = 0;
    //for (TString config : {"_allowRescatter", ""}) {
    for (TString config : {"_allowRescatter"}) {
      h_trk_pt_yield_pp_sig[iDPhi][iConfig] = (TH1D*) h_trk_pt_yield_pp[iDPhi][iConfig]->Clone (Form ("h_trk_pt_%s_pp_sig%s", dphiStr.c_str (), config.Data ()));
      //h_trk_pt_yield_pp_sig[iDPhi][iConfig]->Add (h_trk_pt_yield_pp_bkg[iDPhi], -1);
      iConfig++;
    }

    for (int iCent = 0; iCent < nCentBins; iCent++) {
      const std::string centStr = Form ("cent%i", iCent);

      iConfig = 0;
      //for (TString config : {"_allowRescatter_withNPDF", "_allowRescatter", "_withNPDF", ""}) {
      for (TString config : {"_allowRescatter_withNPDF", "_allowRescatter"}) {
        h_trk_pt_yield_pPb_sig[iDPhi][iCent][iConfig] = (TH1D*) h_trk_pt_yield_pPb[iDPhi][iCent][iConfig]->Clone (Form ("h_trk_pt_%s_cent%i_pPb_sig%s", dphiStr.c_str (), iCent, config.Data ()));
        //h_trk_pt_yield_pPb_sig[iDPhi][iCent][iConfig]->Add (h_trk_pt_yield_pPb_bkg[iDPhi][iCent], -1);

        h_trk_pt_yield_ratio[iDPhi][iCent][iConfig] = (TH1D*) h_trk_pt_yield_pPb_sig[iDPhi][iCent][iConfig]->Clone (Form ("h_trk_pt_%s_cent%i_ratio%s", dphiStr.c_str (), iCent, config.Data ()));
        h_trk_pt_yield_ratio[iDPhi][iCent][iConfig]->Divide (h_trk_pt_yield_pp_sig[iDPhi][0]);

        f_trk_pt_yield_ratio[iDPhi][iCent][iConfig] = new TF1 (Form ("f_trk_pt_%s_cent%i_ratio%s", dphiStr.c_str (), iCent, config.Data ()), "[0]+[1]*log(x)+[2]*pow(log(x),2)", 4, 60);
        h_trk_pt_yield_ratio[iDPhi][iCent][iConfig]->Fit (f_trk_pt_yield_ratio[iDPhi][iCent][iConfig], "RN0Q");
        iConfig++;
      }
    }
  }

  for (int iPth = 0; iPth < nRedPthBins; iPth++) {
    const std::string pthStr = GetRedPthStr (iPth);

    iConfig = 0;
    //for (TString config : {"_allowRescatter", ""}) {
    for (TString config : {"_allowRescatter"}) {
      h_trk_dphi_yield_pp_sig[iPth][iConfig] = (TH1D*) h_trk_dphi_yield_pp[iPth][iConfig]->Clone (Form ("h_trk_dphi_%s_pp_sig%s", pthStr.c_str (), config.Data ()));
      //h_trk_dphi_yield_pp_sig[iPth][iConfig]->Add (h_trk_dphi_yield_pp_bkg[iPth], -1);
      iConfig++;
    }

    for (int iCent = 0; iCent < nCentBins; iCent++) {
      const std::string centStr = Form ("cent%i", iCent);

      iConfig = 0;
      //for (TString config : {"_allowRescatter_withNPDF", "_allowRescatter", "_withNPDF", ""}) {
      for (TString config : {"_allowRescatter_withNPDF", "_allowRescatter"}) {
        h_trk_dphi_yield_pPb_sig[iPth][iCent][iConfig] = (TH1D*) h_trk_dphi_yield_pPb[iPth][iCent][iConfig]->Clone (Form ("h_trk_dphi_%s_cent%i_pPb_sig%s", pthStr.c_str (), iCent, config.Data ()));
        //h_trk_dphi_yield_pPb_sig[iPth][iCent][iConfig]->Add (h_trk_dphi_yield_pPb_bkg[iPth][iCent], -1);

        h_trk_dphi_yield_ratio[iPth][iCent][iConfig] = (TH1D*) h_trk_dphi_yield_pPb_sig[iPth][iCent][iConfig]->Clone (Form ("h_trk_dphi_%s_cent%i_ratio%s", pthStr.c_str (), iCent, config.Data ()));
        h_trk_dphi_yield_ratio[iPth][iCent][iConfig]->Divide (h_trk_dphi_yield_pp_sig[iPth][0]);
        iConfig++;
      }
    }
  }

  for (int iCent = 0; iCent < nCentBins; iCent++) {
    const std::string centStr = Form ("cent%i", iCent);

    iConfig = 0;
    //for (TString config : {"_allowRescatter_withNPDF", "_allowRescatter", "_withNPDF", ""}) {
    for (TString config : {"_allowRescatter_withNPDF", "_allowRescatter"}) {
      h_jet_pt_yield_ratio[iCent][iConfig] = (TH1D*) h_jet_pt_yield_pPb[iCent][iConfig]->Clone (Form ("h_jet_pt_cent%i_ratio%s", iCent, config.Data ()));
      h_jet_pt_yield_ratio[iCent][iConfig]->Divide (h_jet_pt_yield_pp[0]);
      iConfig++;
    }
  }



  for (int iDPhi = 0; iDPhi < 3; iDPhi++) {

    iConfig = 0;
    //for (TString config : {"_allowRescatter", ""}) {
    for (TString config : {"_allowRescatter"}) {
      h_trk_pt_yield_pp[iDPhi][iConfig]->Write ();
      h_trk_pt_yield_pp_sig[iDPhi][iConfig]->Write ();
      iConfig++;
    }
    //h_trk_pt_yield_pp_bkg[iDPhi]->Write ();

    for (int iCent = 0; iCent < nCentBins; iCent++) {

      iConfig = 0;
      //for (TString config : {"_allowRescatter_withNPDF", "_allowRescatter", "_withNPDF", ""}) {
      for (TString config : {"_allowRescatter_withNPDF", "_allowRescatter"}) {
        h_trk_pt_yield_pPb[iDPhi][iCent][iConfig]->Write ();
        h_trk_pt_yield_pPb_sig[iDPhi][iCent][iConfig]->Write ();
        h_trk_pt_yield_ratio[iDPhi][iCent][iConfig]->Write ();
        f_trk_pt_yield_ratio[iDPhi][iCent][iConfig]->Write ();
        iConfig++;
      }
      //h_trk_pt_yield_pPb_bkg[iDPhi][iCent][iConfig]->Write ();

    }
  }


  for (int iPth = 0; iPth < nRedPthBins; iPth++) {

    iConfig = 0;
    //for (TString config : {"_allowRescatter", ""}) {
    for (TString config : {"_allowRescatter"}) {
      h_trk_dphi_yield_pp[iPth][iConfig]->Write ();
      h_trk_dphi_yield_pp_sig[iPth][iConfig]->Write ();
      iConfig++;
    }
    //h_trk_dphi_yield_pp_bkg[iPth]->Write ();

    for (int iCent = 0; iCent < nCentBins; iCent++) {

      iConfig = 0;
      //for (TString config : {"_allowRescatter_withNPDF", "_allowRescatter", "_withNPDF", ""}) {
      for (TString config : {"_allowRescatter_withNPDF", "_allowRescatter"}) {
        h_trk_dphi_yield_pPb[iPth][iCent][iConfig]->Write ();
        h_trk_dphi_yield_pPb_sig[iPth][iCent][iConfig]->Write ();
        h_trk_dphi_yield_ratio[iPth][iCent][iConfig]->Write ();
        iConfig++;
      }
      //h_trk_dphi_yield_pPb_bkg[iPth][iCent]->Write ();

    }
  }


  iConfig = 0;
  for (TString config : {"_allowRescatter"}) {
    h_jet_pt_yield_pp[iConfig]->Write ();
  }

  for (int iCent = 0; iCent < nCentBins; iCent++) {

    iConfig = 0;
    //for (TString config : {"_allowRescatter_withNPDF", "_allowRescatter", "_withNPDF", ""}) {
    for (TString config : {"_allowRescatter_withNPDF", "_allowRescatter"}) {
      h_jet_pt_yield_pPb[iCent][iConfig]->Write ();
      h_jet_pt_yield_ratio[iCent][iConfig]->Write ();
      iConfig++;
    }

  }


  outFile->Close ();

  return 0;
}
