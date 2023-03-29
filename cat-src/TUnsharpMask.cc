/* @file TUnsharpMask.cc
 * @brief unsharp mask
 *
 * @date Create        : 2020-07-29 14:35:05 JST
 *       Last Modified : 2020-07-29 16:24:10 JST (ota)
 * @author: Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 */


#include "TUnsharpMask.h"
#include "TMath.h"


using art::util::TUnsharpMask;

TUnsharpMask::TUnsharpMask(double sigma, int halfNum, double amount, double threshold)
   : fSigma(sigma), fHalfNumPixel(halfNum), fAmount(amount), fThreshold(threshold)
{
   for (int j = -halfNum; j <= halfNum; j++) {
      fMask1.push_back(TMath::Gaus(j, sigma, true));
      fMask2.push_back(TMath::Gaus(j, sigma * 3, true));
   }
}

TUnsharpMask::~TUnsharpMask()
{
}

void TUnsharpMask::operator() (const std::vector<double>& in, std::vector<double>& out)
{
   const int n = in.size();
   out.resize(n);
   for (int i = 0; i < n; ++i) {
      double val1 = 0, val2 = 0;
      for (int j = -fHalfNumPixel; j <= fHalfNumPixel; ++j) {
         int idx = (i + j >= n) ? 2 * n - (i + j + 1) : TMath::Abs(i + j);
         val1 += fMask1[j+fHalfNumPixel] * in[idx];
         val2 += fMask2[j+fHalfNumPixel] * in[idx];
      }
      out[i] = TMath::Abs(val1 - val2) < fThreshold ? val1 : val1 + fAmount * (val1 - val2);         
   }
}




