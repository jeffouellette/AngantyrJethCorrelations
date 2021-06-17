#ifndef __header_h__
#define __header_h__

#include <string>
#include <fstream>


namespace PythiaAngantyrStudy {

const double pthBins[] = {0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95, 1, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2, 2.2, 2.4, 2.6, 2.8, 3, 3.25, 3.5, 3.75, 4, 4.5, 5, 5.5, 6, 7, 8, 9, 10, 11, 12, 14, 16, 18, 20, 25, 30, 35, 40, 45, 50, 55, 60, 70, 80};
const int nPthBins = sizeof (pthBins) / sizeof (pthBins[0]) - 1;
const short nDPhiBins = 24;
const double* dPhiBins = linspace (0, M_PI, nDPhiBins);
const int nPtJBins = 30;
const double* pTJBins = logspace (8, 140, nPtJBins);

string FormatCounts (int counts) {
  if (counts < 1000) return "";
  else if (1000 <= counts && counts < 10000) {
    string countsStr = FormatMeasurement (counts, 0, 1);
    countsStr = countsStr.substr(0, 1) + "k";
    return countsStr;
  }
  else if (10000 <= counts && counts < 100000) {
    string countsStr = FormatMeasurement (counts, 0, 2);
    countsStr = countsStr.substr(0, 2) + "k";
    return countsStr;
  }
  else if (100000 <= counts && counts < 1000000) {
    string countsStr = FormatMeasurement (counts, 0, 3);
    countsStr = countsStr.substr(0, 3) + "k";
    return countsStr;
  }
  else return "";
} 

} // end namespace pullstudy


void ScaleHist (TH1D* h, const double sf, const bool doWidth) {
  const int nb = h->GetNbinsX ();
  for (int iX = 1; iX <= nb; iX++) {
    h->SetBinContent (iX, h->GetBinContent (iX) * sf / (doWidth ? h->GetBinWidth (iX) : 1.));
    h->SetBinError (iX, h->GetBinError (iX) * sf / (doWidth ? h->GetBinWidth (iX) : 1.));
  }
  return;
}



void ScaleHist (TH2D* h, const double sf, const bool doWidth) {
  const int nbx = h->GetNbinsX (); const int nby = h->GetNbinsY ();
  for (int iX = 1; iX <= nbx; iX++) {
    for (int iY = 1; iY <= nby; iY++) {
      h->SetBinContent (iX, iY, h->GetBinContent (iX, iY) * sf / (doWidth ? h->GetXaxis ()->GetBinWidth (iX) * h->GetYaxis ()->GetBinWidth (iY) : 1.));
      h->SetBinError (iX, iY, h->GetBinError (iX, iY) * sf / (doWidth ? h->GetXaxis ()->GetBinWidth (iX) * h->GetYaxis ()->GetBinWidth (iY) : 1.));
    }
  }
  return;
}



double GetMaxRapidity (const double mass, const double pT) {
  // Strategy:
  // for a particular mass of particle, get its rapidity at the edge of the
  // ATLAS tracker (eta = +2.5).
  // That value is the maximum rapidity of that particle you can observe, y_max,
  // across all system boosts.
  // Since the p+Pb center of mass boost is +0.465, you restrict to just 
  // particles with |rapidity - y_system| < y_max - 0.465.

  const double en = std::sqrt (std::pow (mass, 2) + std::pow (pT * std::cosh ((double)2.5), 2));
  const double pz = pT * std::sinh ((double)2.5);

  return 0.5 * std::log ((en + pz) / (en - pz));
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FCal-derived centrality classes for mixing pp events
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int ppMixPercs[] = {100, 95, 90, 85, 80, 75, 70, 65, 60, 55, 50, 45, 40, 35, 30, 25, 20, 15, 10, 5, 0};
int nppMixBins = sizeof (ppMixPercs) / sizeof (ppMixPercs[0]) - 1;

/**
 * Initializes fcal centrality cuts from latest definitions (stored in aux directory).
 */
double* InitppCuts () {
  std::ifstream cutsfile;
  cutsfile.open ("aux/ppFcalCentCuts.dat");

  double* cuts = new double[nppMixBins+1];
  int iCent = 0;
  std::string perc, dummy;
  double cut;
  while (cutsfile && iCent <= nppMixBins) {
    cutsfile >> perc >> dummy;
    cut = std::atof (dummy.c_str ());
    if (strcmp (perc.c_str (), Form ("%i%%", ppMixPercs[iCent])) == 0)
      cuts[iCent++] = cut;
  }

  return cuts;
}

const double* ppCuts = InitppCuts ();


/**
 * Returns the bin corresponding to this sum fcal et, assuming HS trigger distribution for fcal et.
 * Returns -1 for >100% peripheral collisions (i.e. FCal Et is less than energy for 100% centrality).
 */
short GetppBin (const float fcal_et) {
  if (fcal_et < ppCuts[0])
    return -1;
  short i = 0;
  while (i < nppMixBins) {
    i++;
    if (fcal_et < ppCuts[i])
      break;
  }
  return i-1;
}


/**
 * Initializes fcal centrality cuts from latest definitions (stored in aux directory).
 * These are meant for minimum bias events (since distributions differ with a HS trigger).
 */
double* InitppBkgCuts () {
  std::ifstream cutsfile;
  cutsfile.open ("aux/ppBkgFcalCentCuts.dat");

  double* cuts = new double[nppMixBins+1];
  int iCent = 0;
  std::string perc, dummy;
  double cut;
  while (cutsfile && iCent <= nppMixBins) {
    cutsfile >> perc >> dummy;
    cut = std::atof (dummy.c_str ());
    if (strcmp (perc.c_str (), Form ("%i%%", ppMixPercs[iCent])) == 0)
      cuts[iCent++] = cut;
  }

  return cuts;
}

const double* ppBkgCuts = InitppBkgCuts ();

/**
 * Returns the bin corresponding to this sum fcal et, assuming minimum bias trigger distribution for fcal et.
 * Returns -1 for >100% peripheral collisions (i.e. FCal Et is less than energy for 100% centrality).
 */
short GetppBkgBin (const float fcal_et) {
  if (fcal_et < ppBkgCuts[0])
    return -1;
  short i = 0;
  while (i < nppMixBins) {
    i++;
    if (fcal_et < ppBkgCuts[i])
      break;
  }
  return i-1;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FCal-derived centrality classes for mixing pPb events
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int pPbMixPercs[] = {100, 95, 90, 85, 80, 75, 70, 65, 60, 55, 50, 45, 40, 35, 30, 25, 20, 15, 10, 5, 0};
int npPbMixBins = sizeof (pPbMixPercs) / sizeof (pPbMixPercs[0]) - 1;

/**
 * Initializes fcal centrality cuts from latest definitions (stored in aux directory).
 */
double* InitpPbCuts () {
  std::ifstream cutsfile;
  cutsfile.open ("aux/pPbFcalCentCuts.dat");

  double* cuts = new double[npPbMixBins+1];
  int iCent = 0;
  std::string perc, dummy;
  double cut;
  while (cutsfile && iCent <= npPbMixBins) {
    cutsfile >> perc >> dummy;
    cut = std::atof (dummy.c_str ());
    if (strcmp (perc.c_str (), Form ("%i%%", pPbMixPercs[iCent])) == 0)
      cuts[iCent++] = cut;
  }

  return cuts;
}

const double* pPbCuts = InitpPbCuts ();


/**
 * Returns the bin corresponding to this sum fcal et, assuming HS trigger distribution for fcal et.
 * Returns -1 for >100% peripheral collisions (i.e. FCal Et is less than energy for 100% centrality).
 */
short GetpPbBin (const float fcal_et) {
  if (fcal_et < pPbCuts[0])
    return -1;
  short i = 0;
  while (i < npPbMixBins) {
    i++;
    if (fcal_et < pPbCuts[i])
      break;
  }
  return i-1;
}


/**
 * Initializes fcal centrality cuts from latest definitions (stored in aux directory).
 * These are meant for minimum bias events (since distributions differ with a HS trigger).
 */
double* InitpPbBkgCuts () {
  std::ifstream cutsfile;
  cutsfile.open ("aux/pPbBkgFcalCentCuts.dat");

  double* cuts = new double[npPbMixBins+1];
  int iCent = 0;
  std::string perc, dummy;
  double cut;
  while (cutsfile && iCent <= npPbMixBins) {
    cutsfile >> perc >> dummy;
    cut = std::atof (dummy.c_str ());
    if (strcmp (perc.c_str (), Form ("%i%%", pPbMixPercs[iCent])) == 0)
      cuts[iCent++] = cut;
  }

  return cuts;
}

const double* pPbBkgCuts = InitpPbBkgCuts ();

/**
 * Returns the bin corresponding to this sum fcal et, assuming minimum bias trigger distribution for fcal et.
 * Returns -1 for >100% peripheral collisions (i.e. FCal Et is less than energy for 100% centrality).
 */
short GetpPbBkgBin (const float fcal_et) {
  if (fcal_et < pPbBkgCuts[0])
    return -1;
  short i = 0;
  while (i < npPbMixBins) {
    i++;
    if (fcal_et < pPbBkgCuts[i])
      break;
  }
  return i-1;
}



#endif
