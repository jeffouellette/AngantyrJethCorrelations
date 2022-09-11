#ifndef __gen_cxx__
#define __gen_cxx__

#include <TFile.h>
#include <TTree.h>
#include <TLorentzVector.h>

#include <sstream>

#include "fastjet/PseudoJet.hh"
#include "fastjet/ClusterSequenceArea.hh"
#include "fastjet/Selector.hh"

#include "fastjet/tools/JetMedianBackgroundEstimator.hh"
#include "fastjet/tools/Subtractor.hh"

#include "fastjet/ClusterSequence.hh"

#include "Pythia8/Pythia.h"
#include "Pythia8/HeavyIons.h"

#include <Utilities.h>
#include <GlobalParams.h>

#include "header.h"

using namespace Pythia8;
using namespace PythiaAngantyrStudy;


int main (int argc, char** argv) {

  if (argc < 8) {
    std::cout << " usage: gen SEED SEEDOFFSET BEAMA BEAMB SQRTS BOOST NEVT CONFIGFILENAME OUTFILENAME" << std::endl;
    return 0;
  }

  // get arguments
  const int seed = atoi (argv[1]) + atoi (argv[2]);
  const int beamA = atoi (argv[3]);
  const int beamB = atoi (argv[4]);
  const float sqrts = atof (argv[5]);
  const float boost = atof (argv[6]);
  const int nEvents = atoi (argv[7]);
  const std::string configFileName = std::string (argv[8]);
  const std::string outFileName = std::string (argv[9]);

  // Generator. Process selection. LHC initialization. Histogram.
  Pythia pythia;

  // fix seed - different for each job but replicable
  pythia.readString ("Random:setSeed = on");
  pythia.readString (Form ("Random:seed = %i", seed));

  // calculate the necessary energy of each beam given CoM boosted rapidity & sqrt(s).
  const float eA = proton_mass * std::cosh (0.5 * std::log ((1. + std::sqrt (1. - 4.*std::pow (proton_mass/sqrts, 2))) / ( 1. - std::sqrt (1. - 4.*std::pow (proton_mass/sqrts, 2)))) + boost);
  const float eB = proton_mass * std::cosh (0.5 * std::log ((1. + std::sqrt (1. - 4.*std::pow (proton_mass/sqrts, 2))) / ( 1. - std::sqrt (1. - 4.*std::pow (proton_mass/sqrts, 2)))) - boost);

  // setup incoming beams
  pythia.readString ("Beams:frameType = 2");
  pythia.readString (Form ("Beams:idA = %i", beamA));
  pythia.readString (Form ("Beams:idB = %i", beamB));
  pythia.readString (Form ("Beams:eA = %g", eA));
  pythia.readString (Form ("Beams:eB = %g", eB));

  // turn on Angantyr (instead of default Pythia)
  pythia.readString ("HeavyIon:mode = 1");
  pythia.readString ("HeavyIon:SigFitNGen = 0"); // these two lines speed up the Angantyr initialization I guess?? (The program tells you to write them in)
  pythia.readString ("HeavyIon:SigFitDefPar = 13.91,1.78,0.22,0.0,0.0,0.0,0.0,0.0");

  // read process configuration from file (e.g. MPI, HardQCD, SoftQCD, electroweak...)
  ifstream configFile;
  configFile.open (configFileName.c_str ());
  std::string line;
  while (getline (configFile, line)) {
    pythia.readString (line.c_str ()); 
  }

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

  bool b_firesMBTrigger = false;

  float b_b = 0;
  int b_ncoll = 0;
  int b_npart1 = 0;
  int b_npart2 = 0;

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

  float b_fcal_et_negEta = 0;
  float b_fcal_et_posEta = 0;

  float b_gap_negEta = 0;
  float b_gap_posEta = 0;

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

  outTree->Branch ("firesMBTrigger", &b_firesMBTrigger, "firesMBTrigger/O");

  if (beamA != 2212 || beamB != 2212) {
    outTree->Branch ("b",         &b_b,           "b/F");
    outTree->Branch ("ncoll",     &b_ncoll,       "ncoll/I");
    outTree->Branch ("npart1",    &b_npart1,      "npart1/I");
    outTree->Branch ("npart2",    &b_npart2,      "npart2/I");
  }

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

  outTree->Branch ("fcal_et_negEta", &b_fcal_et_negEta, "fcal_et_negEta/F");
  outTree->Branch ("fcal_et_posEta", &b_fcal_et_posEta, "fcal_et_posEta/F");

  outTree->Branch ("gap_negEta",  &b_gap_negEta,  "gap_negEta/F");
  outTree->Branch ("gap_posEta",  &b_gap_posEta,  "gap_posEta/F");

  std::vector <fastjet::PseudoJet> particles, hard_event, full_event;
  int* nPartEta = new int[98];

  const float ptmin = 20;

  const bool debugfastjet = false;

  for (int iEvent = 0; iEvent < nEvents; iEvent++) {
    if (nEvents > 100 && iEvent % (nEvents / 100) == 0)
      cout << iEvent / (nEvents / 100) << "\% done...\r" << flush;

    if (!pythia.next ())
      continue;

    particles.clear ();
    hard_event.clear ();
    full_event.clear ();

    b_firesMBTrigger = false;
    b_part_n = 0;
    b_fcal_et_negEta = 0;
    b_fcal_et_posEta = 0;
    for (int iEta = 0; iEta < 98; iEta++) nPartEta[iEta] = 0;

    for (int i = 0; i < pythia.event.size (); i++) {

      if (!pythia.event[i].isFinal ())
        continue; // only use final state particles

      if (std::abs (pythia.event[i].id ()) == 12 || std::abs (pythia.event[i].id ()) == 14 || std::abs (pythia.event[i].id ()) == 16)
        continue; // check that particle is not a neutrino

      if (pythia.event[i].pT () > 0.2 && std::fabs (pythia.event[i].eta ()) < 4.9) {
        int iEta = 0;
        while (-4.8+0.1*iEta < pythia.event[i].eta () && iEta < 98) iEta++;
        if (iEta < 98)
          nPartEta[iEta]++;
      }

      if (std::abs (pythia.event[i].id ()) != 13) { // exclude muons from jet clustering
        particles.push_back (fastjet::PseudoJet (pythia.event[i].px (), pythia.event[i].py (), pythia.event[i].pz (), pythia.event[i].e ()));
        hard_event.push_back (fastjet::PseudoJet (pythia.event[i].px (), pythia.event[i].py (), pythia.event[i].pz (), pythia.event[i].e ()));
        full_event.push_back (fastjet::PseudoJet (pythia.event[i].px (), pythia.event[i].py (), pythia.event[i].pz (), pythia.event[i].e ()));
      }

      if (-4.9 < pythia.event[i].eta () && pythia.event[i].eta () < -3.2)
        b_fcal_et_negEta += std::sqrt (std::pow (pythia.event[i].e (), 2) - std::pow (pythia.event[i].pT () * std::sinh (pythia.event[i].eta ()), 2)); // add particle E_T
      else if (3.2 < pythia.event[i].eta () && pythia.event[i].eta () < 4.9)
        b_fcal_et_posEta += std::sqrt (std::pow (pythia.event[i].e (), 2) - std::pow (pythia.event[i].pT () * std::sinh (pythia.event[i].eta ()), 2)); // add particle E_T

      if (std::fabs (pythia.event[i].id ()) < 2.5 && pythia.event[i].pT () > 0.2)
        b_firesMBTrigger = true;

      if (pythia.event[i].pT () < 2)
        continue; // basic pT cut for my sanity

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


    // calculate rapidity gaps at positive & negative eta
    int iEta = 0;
    while (nPartEta[iEta] == 0) iEta++;
    b_gap_negEta = iEta*0.1;

    iEta = 97;
    while (nPartEta[iEta] == 0) iEta--;
    b_gap_posEta = (97-iEta)*0.1;


    //==================================================================================================
    // NOW TRY JETS WITH UNDERLYING EVENT SUBTRACTION (SEE 07-subtraction.cc from FastJet Examples)
    //==================================================================================================

    // create an area definition for the clustering
    //----------------------------------------------------------
    // ghosts should go up to the acceptance of the detector or
    // (with infinite acceptance) at least 2R beyond the region
    // where you plan to investigate jets.
    double ghost_maxrap = 6.0;
    double particle_maxrap = 5.0;
    
    fastjet::GhostedAreaSpec area_spec (ghost_maxrap);
    fastjet::AreaDefinition area_def (fastjet::active_area, area_spec);
    
    // run the jet clustering with the above jet and area definitions
    // for both the hard and full event
    //
    // We retrieve the jets above 5 GeV in both case (note that the
    // 5-GeV cut we be applied again later on after we subtract the jets
    // from the full event)
    // ----------------------------------------------------------
    fastjet::ClusterSequenceArea clusterSeqAkt2Jets (particles, antiKt2, area_def);
    std::vector <fastjet::PseudoJet> sortedAkt2Jets = fastjet::sorted_by_pt (clusterSeqAkt2Jets.inclusive_jets (ptmin));

    fastjet::ClusterSequenceArea clusterSeqAkt4Jets (particles, antiKt4, area_def);
    std::vector <fastjet::PseudoJet> sortedAkt4Jets = fastjet::sorted_by_pt (clusterSeqAkt4Jets.inclusive_jets (ptmin));

    //fastjet::ClusterSequenceArea clust_seq_hard (hard_event, fastjet::antiKt4, area_def);
    //fastjet::ClusterSequenceArea clust_seq_full (full_event, fastjet::antiKt4, area_def);
    //
    //std::vector <fastjet::PseudoJet> hard_jets = fastjet::sorted_by_pt (clust_seq_hard.inclusive_jets (ptmin));
    //std::vector <fastjet::PseudoJet> full_jets = fastjet::sorted_by_pt (clust_seq_full.inclusive_jets (ptmin));
    std::vector <fastjet::PseudoJet> hard_jets = sortedAkt4Jets;
    
    // Now turn to the estimation of the background (for the full event)
    //
    // There are different ways to do that. In general, this also
    // requires clustering the particles that will be handled internally
    // in FastJet. 
    //
    // The suggested way to proceed is to use a BackgroundEstimator
    // constructed from the following 3 arguments:
    //  - a jet definition used to cluster the particles.
    //    . We strongly recommend using the kt or Cambridge/Aachen
    //      algorithm (a warning will be issued otherwise)
    //    . The choice of the radius is a bit more subtle. R=0.4 has
    //      been chosen to limit the impact of hard jets; in samples of
    //      dominantly sparse events it may cause the UE/pileup to be
    //      underestimated a little, a slightly larger value (0.5 or
    //      0.6) may be better.
    //  - An area definition for which we recommend the use of explicit
    //    ghosts (i.e. active_area_explicit_ghosts)
    //    As mentionned in the area example (06-area.cc), ghosts should
    //    extend sufficiently far in rapidity to cover the jets used in
    //    the computation of the background (see also the comment below)
    //  - A Selector specifying the range over which we will keep the
    //    jets entering the estimation of the background (you should
    //    thus make sure the ghosts extend far enough in rapidity to
    //    cover the range, a warning will be issued otherwise).
    //    In this particular example, the two hardest jets in the event
    //    are removed from the background estimation
    // ----------------------------------------------------------
    fastjet::JetDefinition jet_def_bkgd_kt2 (fastjet::JetAlgorithm::kt_algorithm, 0.2);
    fastjet::JetDefinition jet_def_bkgd_kt4 (fastjet::JetAlgorithm::kt_algorithm, 0.4);

    fastjet::AreaDefinition area_def_bkgd (fastjet::AreaType::active_area_explicit_ghosts, fastjet::GhostedAreaSpec (ghost_maxrap));

    fastjet::Selector selector = fastjet::SelectorAbsRapMax (4.5) * (!fastjet::SelectorNHardest (2));

    fastjet::JetMedianBackgroundEstimator bkgd_estimator_kt2 (selector, jet_def_bkgd_kt2, area_def_bkgd);
    fastjet::JetMedianBackgroundEstimator bkgd_estimator_kt4 (selector, jet_def_bkgd_kt4, area_def_bkgd);
    
    // To help manipulate the background estimator, we also provide a
    // transformer that allows to apply directly the background
    // subtraction on the jets. This will use the background estimator
    // to compute rho for the jets to be subtracted.
    // ----------------------------------------------------------
    fastjet::Subtractor subtractor_kt2 (&bkgd_estimator_kt2);
    fastjet::Subtractor subtractor_kt4 (&bkgd_estimator_kt4);
    
    // since FastJet 3.1.0, rho_m is supported natively in background
    // estimation (both JetMedianBackgroundEstimator and
    // GridMedianBackgroundEstimator).
    //
    // For backward-compatibility reasons its use is by default switched off
    // (as is the enforcement of m>0 for the subtracted jets). The
    // following 2 lines of code switch these on. They are strongly
    // recommended and should become the default in future versions of
    // FastJet.
    //
    // Note that we also illustrate the use of the
    // FASTJET_VERSION_NUMBER macro
    //#if FASTJET_VERSION_NUMBER >= 30100
    subtractor_kt2.set_use_rho_m (true);
    subtractor_kt2.set_safe_mass (true);
    subtractor_kt4.set_use_rho_m (true);
    subtractor_kt4.set_safe_mass (true);
    //#endif

      // Finally, once we have an event, we can just tell the background
    // estimator to use that list of particles
    // This could be done directly when declaring the background
    // estimator but the usage below can more easily be accomodated to a
    // loop over a set of events.
    // ----------------------------------------------------------
    bkgd_estimator_kt2.set_particles (full_event);
    bkgd_estimator_kt4.set_particles (full_event);
    
    // show a summary of what was done so far
    //  - the description of the algorithms, areas and ranges used
    //  - the background properties
    //  - the jets in the hard event
    //----------------------------------------------------------
    if (debugfastjet) {
      cout << "Main clustering:" << endl;
      cout << "  Ran:   " << antiKt4.description () << endl;
      cout << "  Area:  " << area_def.description () << endl;
      cout << "  Particles up to |y|=" << particle_maxrap << endl;
      cout << endl;
      
      cout << "Background estimation:" << endl;
      cout << "  " << bkgd_estimator_kt4.description () << endl << endl;;
      cout << "  Giving, for the full event" << endl;
      cout << "    rho     = " << bkgd_estimator_kt4.rho ()   << endl;
      cout << "    sigma   = " << bkgd_estimator_kt4.sigma () << endl; 
      cout << "    rho_m   = " << bkgd_estimator_kt4.rho_m ()   << endl;
      cout << "    sigma_m = " << bkgd_estimator_kt4.sigma_m () << endl; 
      cout << endl;
    
    
      cout << "Jets above " << ptmin << " GeV in the hard event (" << hard_event.size() << " particles)" << endl;
      cout << "---------------------------------------\n";
      printf("%5s %15s %15s %15s %15s %15s\n","jet #", "rapidity", "phi", "pt", "m", "area");
      for (unsigned int i = 0; i < hard_jets.size(); i++) {
        printf("%5u %15.8f %15.8f %15.8f %15.8f %15.8f\n", i,
         hard_jets[i].rap(), hard_jets[i].phi(), hard_jets[i].pt(), hard_jets[i].m(),
         hard_jets[i].area());
      }
      cout << endl;
      
      // Once the background properties have been computed, subtraction
      // can be applied on the jets. Subtraction is performed on the
      // full 4-vector
      //
      // We output the jets before and after subtraction
      // ----------------------------------------------------------
      cout << "Jets above " << ptmin << " GeV in the full event (" << full_event.size() << " particles)" << endl;
      cout << "---------------------------------------\n";
      printf("%5s %15s %15s %15s %15s %15s %15s %15s %15s %15s\n","jet #", "rapidity", "phi", "pt", "m", "area", "rap_sub", "phi_sub", "pt_sub", "m_sub");

    }

    std::vector<fastjet::PseudoJet> subtractedAkt2Jets = subtractor_kt2 (sortedAkt2Jets);
    // OLD: unsubtracted jets
    //fastjet::ClusterSequence clusterSeqAkt2Jets (particles, antiKt4);
    //std::vector <fastjet::PseudoJet> sortedAkt2Jets = fastjet::sorted_by_pt (clusterSeqAkt2Jets.inclusive_jets ());

    b_akt2_jet_n = 0;
    for (fastjet::PseudoJet jet : subtractedAkt2Jets) {
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

    std::vector<fastjet::PseudoJet> subtractedAkt4Jets = subtractor_kt4 (sortedAkt4Jets);

    // OLD: unsubtracted jets
    //fastjet::ClusterSequence clusterSeqAkt4Jets (particles, antiKt4);
    //std::vector <fastjet::PseudoJet> sortedAkt4Jets = fastjet::sorted_by_pt (clusterSeqAkt4Jets.inclusive_jets ());

    b_akt4_jet_n = 0;
    for (fastjet::PseudoJet jet : subtractedAkt4Jets) {
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

    // see https://pythia.org/doxygen/pythia8303/classPythia8_1_1Info.html or main113.cc in the examples
    if (beamA != 2212 || beamB != 2212) {
      b_b = pythia.info.hiInfo->b ();
      b_ncoll = pythia.info.hiInfo->nCollNDTot ();
      b_npart1 = pythia.info.hiInfo->nAbsProj ();
      b_npart2 = pythia.info.hiInfo->nAbsTarg ();
    }

    outTree->Fill();
  }

  delete[] nPartEta;

  pythia.stat();
  
  outFile->Write();
  outFile->Close();

  return 0;
}

#endif
