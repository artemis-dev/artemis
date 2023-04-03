/* @file TChargeRiseTimeCompensateProcessor.cc
 * @brief charge correction by using rise time.
 * The correlation between charge (ratio) and rise time should be given as polynomial.
 *
 * @date Create        : 2019-07-22 23:22:11 JST
 *       Last Modified : 2019-07-24 16:23:45 JST (ota)
 * @author: Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 */


#include "TChargeRiseTimeCompensateProcessor.h"
#include "TCatPulseShape.h"

ClassImp(art::TChargeRiseTimeCompensateProcessor)

using art::TChargeRiseTimeCompensateProcessor;

TChargeRiseTimeCompensateProcessor::TChargeRiseTimeCompensateProcessor()
{
   Register(fInput("Input","name of input collection","atpulse"));
   Register(fCoeff("Coefficients","Coefficients of polynomial of correlation, [0th, 1st, 2nd, ...]",DoubleVec_t(0)));
}

TChargeRiseTimeCompensateProcessor::~TChargeRiseTimeCompensateProcessor()
{


}

void TChargeRiseTimeCompensateProcessor::Process()
{
   if (fInput->GetEntriesFast() == 0) return;
   
   const DoubleVec_t &coeff = static_cast<DoubleVec_t>(fCoeff);

   for (Int_t i = 0, n = fInput->GetEntriesFast(); i < n; ++i) {
      TCatPulseShape *pulse = static_cast<TCatPulseShape*>(fInput->UncheckedAt(i));
      double c = pulse->GetCharge();
      double t = pulse->GetRiseTime();
      double w = coeff.back();
      for (int ip = coeff.size() - 2; ip >= 0; --ip) {
         w = w * t + coeff[ip];
      }
      pulse->SetCharge(c / w / coeff[0]);
   }
}
