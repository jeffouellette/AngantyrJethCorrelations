#ifndef __header_h__
#define __header_h__

#include <string>
#include <fstream>


namespace PythiaAngantyrStudy {

const TString workPath = TString (std::getenv ("gpfs")) + "/AngantyrJethCorrelations";


const double redPthBins[] = {0.5, 1, 1.5, 2, 4, 6, 10, 20, 30, 60};
const int nRedPthBins = sizeof (redPthBins) / sizeof (redPthBins[0]) - 1;

//const double pthBins[] = {0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95, 1, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2, 2.2, 2.4, 2.6, 2.8, 3, 3.25, 3.5, 3.75, 4, 4.5, 5, 5.5, 6, 7, 8, 9, 10, 11, 12, 14, 16, 18, 20, 25, 30, 35, 40, 45, 50, 55, 60, 70, 80}; // 60 elements
//const int nPthBins = sizeof (pthBins) / sizeof (pthBins[0]) - 1;
const int nPthBins = 60;
const double* pthBins = logspace (0.2, 90, nPthBins);

const short nDPhiBins = 24;
const double* dPhiBins = linspace (0, M_PI, nDPhiBins);
const int nPtJBins = 30;
const double* pTJBins = logspace (8, 140, nPtJBins);

std::string GetRedPthStr (int iPth) {
  switch (iPth) {
    case 0: return "gt0p5_lt1";
    case 1: return "gt1_lt1p5";
    case 2: return "gt1p5_lt2";
    case 3: return "gt2_lt4";
    case 4: return "gt4_lt6";
    case 5: return "gt6_lt10";
    case 6: return "gt10_lt20";
    case 7: return "gt20_lt30";
    case 8: return "gt30_lt60";
    default: return "gt0_ltFLTMAX";
  }
}

std::string FormatCounts (int counts) {
  if (counts < 1000) return "";
  else if (1000 <= counts && counts < 10000) {
    std::string countsStr = FormatMeasurement (counts, 0, 1);
    countsStr = countsStr.substr(0, 1) + "k";
    return countsStr;
  }
  else if (10000 <= counts && counts < 100000) {
    std::string countsStr = FormatMeasurement (counts, 0, 2);
    countsStr = countsStr.substr(0, 2) + "k";
    return countsStr;
  }
  else if (100000 <= counts && counts < 1000000) {
    std::string countsStr = FormatMeasurement (counts, 0, 3);
    countsStr = countsStr.substr(0, 3) + "k";
    return countsStr;
  }
  else return "";
} 


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


/**
 * Returns the bin corresponding to this value in the array.
 * Returns -1 if value is less than 0th bin entry.
 */
short GetBin (const float val, const float* arr, const int len) {
  if (val < arr[0])
    return -1;
  short i = 0;
  while (i < len) {
    i++;
    if (val < arr[i])
      break;
  }
  return i-1;
}


/**
 * Initializes centrality bins from a given file at given percentages.
 */
float* InitCentCuts (const char* fName, const int* percs, const int nPercs) {

  std::ifstream cutsfile;
  cutsfile.open (fName);

  float* cuts = new float[nPercs+1];
  int iCent = 0;
  std::string perc, dummy;
  float cut;

  while (cutsfile && iCent <= nPercs) {
    cutsfile >> perc >> dummy;
    cut = std::atof (dummy.c_str ());
    if (strcmp (perc.c_str (), Form ("%i%%", percs[iCent])) == 0)
      cuts[iCent++] = cut;
  }

  return cuts;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FCal-derived centrality classes for mixing pp events
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//int ppMixPercs[] = {100, 95, 90, 85, 80, 75, 70, 65, 60, 55, 50, 45, 40, 35, 30, 25, 20, 15, 10, 5, 0};
int ppMixPercs[] = {100, 99, 98, 97, 96, 95, 94, 93, 92, 91, 90, 89, 88, 87, 86, 85, 84, 83, 82, 81, 80, 79, 78, 77, 76, 75, 74, 73, 72, 71, 70, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
int nppMixBins = sizeof (ppMixPercs) / sizeof (ppMixPercs[0]) - 1;

float* ppCuts = InitCentCuts (Form ("%s/aux/ppFcalCuts.dat", workPath.Data ()), ppMixPercs, nppMixBins);
float* ppBkgCuts = InitCentCuts (Form ("%s/aux/ppBkgFcalCuts.dat", workPath.Data ()), ppMixPercs, nppMixBins);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FCal-derived centrality classes for mixing pPb events
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//int pPbMixPercs[] = {100, 95, 90, 85, 80, 75, 70, 65, 60, 55, 50, 45, 40, 35, 30, 25, 20, 15, 10, 5, 0};
int pPbMixPercs[] = {100, 99, 98, 97, 96, 95, 94, 93, 92, 91, 90, 89, 88, 87, 86, 85, 84, 83, 82, 81, 80, 79, 78, 77, 76, 75, 74, 73, 72, 71, 70, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
int npPbMixBins = sizeof (pPbMixPercs) / sizeof (pPbMixPercs[0]) - 1;

float* pPbCuts = InitCentCuts (Form ("%s/aux/pPbFcalCuts.dat", workPath.Data ()), pPbMixPercs, npPbMixBins);
float* pPbBkgCuts = InitCentCuts (Form ("%s/aux/pPbBkgFcalCuts.dat", workPath.Data ()), pPbMixPercs, npPbMixBins);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Ncoll-derived centrality classes for mixing pPb events
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//int pPbNcollPercs[] = {100, 95, 90, 85, 80, 75, 70, 65, 60, 55, 50, 45, 40, 35, 30, 25, 20, 15, 10, 5, 0};
int pPbNcollPercs[] = {100, 80, 60, 40, 20, 0};
int npPbNcollBins = sizeof (pPbNcollPercs) / sizeof (pPbNcollPercs[0]) - 1;

float* pPbNcollCuts = InitCentCuts (Form ("%s/aux/pPbNCollCuts.dat", workPath.Data ()), pPbNcollPercs, npPbNcollBins);
float* pPbBkgNcollCuts = InitCentCuts (Form ("%s/aux/pPbBkgNCollCuts.dat", workPath.Data ()), pPbNcollPercs, npPbNcollBins);


} // end namespace pullstudy


#endif
