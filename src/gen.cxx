#ifndef __gen_cxx__
#define __gen_cxx__

#include <TFile.h>
#include <TTree.h>
#include <TLorentzVector.h>

#include <sstream>

#include "fastjet/ClusterSequence.hh"

#include "Pythia8/Pythia.h"

#include <GlobalParams.h>
#include <Utilities.h>

#include "header.h"

using namespace Pythia8;
using namespace PythiaAngantyrStudy;


int main (int argc, char** argv) {

  if (argc < 8) {
    std::cout << " usage: gen SEED SEEDOFFSET BEAMA BEAMB SQRTS MINPTHAT NEVT BOOST FILENAMEOUT" << std::endl;
    return 0;
  }

  // get arguments
  const int seed = atoi (argv[1]) + atoi (argv[2]);
  const int beamA = atoi (argv[3]);
  const int beamB = atoi (argv[4]);
  const float sqrts = atof (argv[5]);
  const float ptHatMin = atof (argv[6]);
  const int nEvents = atoi (argv[7]);
  const float boost = atof (argv[8]);
  const string outFileName = string (argv[9]);

  // Generator. Process selection. LHC initialization. Histogram.
  Pythia pythia;

  pythia.readString ("Random:setSeed = on");
  pythia.readString (Form ("Random:seed = %i", seed));

  const float eA = proton_mass * std::cosh ( 0.5 * std::log ( ( 1. + std::sqrt (1. - 4.*pow (proton_mass/sqrts, 2))) / ( 1. - std::sqrt (1. - 4.*pow (proton_mass/sqrts, 2)))) + boost);
  const float eB = proton_mass * std::cosh ( 0.5 * std::log ( ( 1. + std::sqrt (1. - 4.*pow (proton_mass/sqrts, 2))) / ( 1. - std::sqrt (1. - 4.*pow (proton_mass/sqrts, 2)))) - boost);

  pythia.readString ("Beams:frameType = 2");
  pythia.readString (Form ("Beams:idA = %i", beamA));
  pythia.readString (Form ("Beams:idB = %i", beamB));
  pythia.readString (Form ("Beams:eA = %g", eA));
  pythia.readString (Form ("Beams:eB = %g", eB));
  pythia.readString("HardQCD:all = on");

  pythia.readString (Form ("PhaseSpace:pTHatMin = %g", ptHatMin));

  pythia.init ();

  fastjet::JetDefinition antiKt2 (fastjet::antikt_algorithm, 0.2);
  fastjet::JetDefinition antiKt4 (fastjet::antikt_algorithm, 0.4);

  TFile* outFile = new TFile (outFileName.c_str (), "RECREATE");

  int b_code = 0;
  int b_id1 = 0;
  int b_id2 = 0;
  float b_x1pdf = 0;
  float b_x2pdf = 0;
  float b_Q = 0;
  float b_pTHat = 0;
  bool b_isValence1 = false;
  bool b_isValence2 = false;

  int b_akt2_jet_n = 0;
  float b_akt2_jet_pt[1000];
  float b_akt2_jet_eta[1000];
  float b_akt2_jet_phi[1000];
  float b_akt2_jet_e[1000];
  float b_akt2_jet_m[1000];

  int b_akt4_jet_n = 0;
  float b_akt4_jet_pt[1000];
  float b_akt4_jet_eta[1000];
  float b_akt4_jet_phi[1000];
  float b_akt4_jet_e[1000];
  float b_akt4_jet_m[1000];

  int b_part_n = 0;
  float b_part_pt[10000];
  float b_part_eta[10000];
  float b_part_y[10000];
  float b_part_phi[10000];
  float b_part_e[10000];
  float b_part_m[10000];

  float fcal_et_negEta = 0;
  float fcal_et_posEta = 0;

  TTree* outTree = new TTree ("tree", "tree");

  outTree->Branch ("code",        &b_code,        "code/I");
  outTree->Branch ("id1",         &b_id1,         "id1/I");
  outTree->Branch ("id2",         &b_id2,         "id2/I");
  outTree->Branch ("x1pdf",       &b_x1pdf,       "x1pdf/F");
  outTree->Branch ("x2pdf",       &b_x2pdf,       "x2pdf/F");
  outTree->Branch ("Q",           &b_Q,           "Q/F");
  outTree->Branch ("pTHat",       &b_pTHat,       "pTHat/F");
  outTree->Branch ("isValence1",  &b_isValence1,  "isValence1/O");
  outTree->Branch ("isValence2",  &b_isValence2,  "isValence2/O");

  outTree->Branch ("akt2_jet_n",    &b_akt2_jet_n,    "akt2_jet_n/I");
  outTree->Branch ("akt2_jet_pt",   &b_akt2_jet_pt,   "akt2_jet_pt[akt2_jet_n]/F");
  outTree->Branch ("akt2_jet_eta",  &b_akt2_jet_eta,  "akt2_jet_eta[akt2_jet_n]/F");
  outTree->Branch ("akt2_jet_phi",  &b_akt2_jet_phi,  "akt2_jet_phi[akt2_jet_n]/F");
  outTree->Branch ("akt2_jet_e",    &b_akt2_jet_e,    "akt2_jet_e[akt2_jet_n]/F");
  outTree->Branch ("akt2_jet_m",    &b_akt2_jet_m,    "akt2_jet_m[akt2_jet_n]/F");

  outTree->Branch ("akt4_jet_n",    &b_akt4_jet_n,    "akt4_jet_n/I");
  outTree->Branch ("akt4_jet_pt",   &b_akt4_jet_pt,   "akt4_jet_pt[akt4_jet_n]/F");
  outTree->Branch ("akt4_jet_eta",  &b_akt4_jet_eta,  "akt4_jet_eta[akt4_jet_n]/F");
  outTree->Branch ("akt4_jet_phi",  &b_akt4_jet_phi,  "akt4_jet_phi[akt4_jet_n]/F");
  outTree->Branch ("akt4_jet_e",    &b_akt4_jet_e,    "akt4_jet_e[akt4_jet_n]/F");
  outTree->Branch ("akt4_jet_m",    &b_akt4_jet_m,    "akt4_jet_m[akt4_jet_n]/F");

  outTree->Branch ("part_n",    &b_part_n,    "part_n/I");
  outTree->Branch ("part_pt",   &b_part_pt,   "part_pt[part_n]/F");
  outTree->Branch ("part_eta",  &b_part_eta,  "part_eta[part_n]/F");
  outTree->Branch ("part_y",    &b_part_y,    "part_y[part_n]/F");
  outTree->Branch ("part_phi",  &b_part_phi,  "part_phi[part_n]/F");
  outTree->Branch ("part_e",    &b_part_e,    "part_e[part_n]/F");
  outTree->Branch ("part_m",    &b_part_m,    "part_m[part_n]/F");

  outTree->Branch ("fcal_et_negEta", &fcal_et_negEta, "fcal_et_negEta/F");
  outTree->Branch ("fcal_et_posEta", &fcal_et_posEta, "fcal_et_posEta/F");

  vector <fastjet::PseudoJet> particles;

  for (int iEvent = 0; iEvent < nEvents; iEvent++) {
    if (nEvents > 100 && iEvent % (nEvents / 100) == 0)
      cout << iEvent / (nEvents / 100) << "\% done...\r" << flush;

    if (!pythia.next ())
      continue;

    particles.clear ();

    b_part_n = 0;
    fcal_et_negEta = 0;
    fcal_et_posEta = 0;
    for (int i = 0; i < pythia.event.size (); i++) {

      if (!pythia.event[i].isFinal ())
        continue; // only use final state particles

      if (abs (pythia.event[i].id ()) == 12 || abs (pythia.event[i].id ()) == 14 || abs (pythia.event[i].id ()) == 16)
        continue; // check that particle is not a neutrino

      if (abs (pythia.event[i].id ()) != 13) // exclude muons from jet clustering
        particles.push_back (fastjet::PseudoJet (pythia.event[i].px (), pythia.event[i].py (), pythia.event[i].pz (), pythia.event[i].e ()));

      if (-4.9 < pythia.event[i].eta () && pythia.event[i].eta () < -3.2)
        fcal_et_negEta += std::sqrt (std::pow (pythia.event[i].e (), 2) - std::pow (pythia.event[i].pT () * std::sinh (pythia.event[i].eta ()), 2)); // add particle E_T
      else if (3.2 < pythia.event[i].eta () && pythia.event[i].eta () < 4.9)
        fcal_et_posEta += std::sqrt (std::pow (pythia.event[i].e (), 2) - std::pow (pythia.event[i].pT () * std::sinh (pythia.event[i].eta ()), 2)); // add particle E_T

      if (!(pythia.event[i].isCharged ()))
        continue; // check that particle is charged (is not neutral)

      if (!(pythia.event[i].isHadron ()))
        continue; // check that particle is a hadron

      if (b_part_n >= 10000) {
        std::cout << "Ran out of array space! Skipping a very important particle!" << std::endl;
        continue;
      }

      b_part_pt[b_part_n]   = pythia.event[i].pT ();
      b_part_eta[b_part_n]  = pythia.event[i].eta ();
      b_part_y[b_part_n]    = pythia.event[i].y ();
      b_part_phi[b_part_n]  = pythia.event[i].phi ();
      b_part_e[b_part_n]    = pythia.event[i].e ();
      b_part_m[b_part_n]    = pythia.event[i].mCalc ();
      b_part_n++;
    }


    // now run jet clustering
    fastjet::ClusterSequence clusterSeqAkt2Jets (particles, antiKt2);
    vector<fastjet::PseudoJet> sortedAkt2Jets = fastjet::sorted_by_pt (clusterSeqAkt2Jets.inclusive_jets ());

    b_akt2_jet_n = 0;
    for (fastjet::PseudoJet jet : sortedAkt2Jets) {
      if (b_akt2_jet_n < 1000) {
        b_akt2_jet_pt[b_akt2_jet_n] = jet.perp ();
        b_akt2_jet_eta[b_akt2_jet_n] = jet.pseudorapidity ();
        b_akt2_jet_phi[b_akt2_jet_n] = jet.phi ();
        b_akt2_jet_e[b_akt2_jet_n] = jet.e ();
        b_akt2_jet_m[b_akt2_jet_n] = (jet.m2 () < 0 ? -1 : 1) * std::sqrt (std::fabs (jet.m2 ()));
        b_akt2_jet_n++;
      }
      else if (jet.perp () > 5) {
        std::cout << "Tried to overflow R=0.2 jet array, please check!" << std::endl;
      }
    }

    fastjet::ClusterSequence clusterSeqAkt4Jets (particles, antiKt4);
    vector<fastjet::PseudoJet> sortedAkt4Jets = fastjet::sorted_by_pt (clusterSeqAkt4Jets.inclusive_jets ());

    b_akt4_jet_n = 0;
    for (fastjet::PseudoJet jet : sortedAkt4Jets) {
      if (b_akt4_jet_n < 1000) {
        b_akt4_jet_pt[b_akt4_jet_n] = jet.perp ();
        b_akt4_jet_eta[b_akt4_jet_n] = jet.pseudorapidity ();
        b_akt4_jet_phi[b_akt4_jet_n] = jet.phi ();
        b_akt4_jet_e[b_akt4_jet_n] = jet.e ();
        b_akt4_jet_m[b_akt4_jet_n] = (jet.m2 () < 0 ? -1 : 1) * std::sqrt (std::fabs (jet.m2 ()));
        b_akt4_jet_n++;
      }
      else if (jet.perp () > 5) {
        std::cout << "Tried to overflow R=0.4 jet array, please check!" << std::endl;
      }
    }

    b_code = pythia.info.code ();
    b_id1 = pythia.info.id1pdf ();
    b_id2 = pythia.info.id2pdf ();
    b_x1pdf = pythia.info.x1pdf ();
    b_x2pdf = pythia.info.x2pdf ();
    b_Q = pythia.info.QFac ();
    b_pTHat = pythia.info.pTHat ();
    
    b_isValence1 = pythia.info.isValence1 ();
    b_isValence2 = pythia.info.isValence2 ();

    outTree->Fill();
  }

  pythia.stat();
  
  outFile->Write();
  outFile->Close();

  return 0;
}

#endif
