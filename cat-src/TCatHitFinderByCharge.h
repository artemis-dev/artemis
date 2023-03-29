/**
 * @file   TCatHitFinderByCharge.h
 * @brief  Temporal hit finding by charge
 *
 * @date   Created       : 2015-01-02 08:43:24 JST
 *         Last Modified : 2018-02-04 13:11:48 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2015 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_FE4FE90D_2910_43D8_A0DD_0CC87AA4927F
#define INCLUDE_GUARD_UUID_FE4FE90D_2910_43D8_A0DD_0CC87AA4927F

#include "TProcessor.h"

namespace art {
   class TCatHitFinderByCharge;
}

class TClonesArray;

namespace {
   const Float_t kDefaultChargeThreshold = 30.;
}

class art::TCatHitFinderByCharge : public TProcessor {
public:
   TCatHitFinderByCharge();
   virtual ~TCatHitFinderByCharge();

   TCatHitFinderByCharge(const TCatHitFinderByCharge& rhs);
   TCatHitFinderByCharge& operator=(const TCatHitFinderByCharge& rhs);

   virtual void Process();

protected:
   TClonesArray **fInput; //!
   TString        fInputName;

   TClonesArray  *fOutput; //!
   TString        fOutputName;

   Float_t fChargeThreshold;
   static const Float_t fgDefaultChargeThreshold;
   
private:

   ClassDef(TCatHitFinderByCharge,1) // Temporal hit finding by charge
};

#endif // INCLUDE_GUARD_UUID_FE4FE90D_2910_43D8_A0DD_0CC87AA4927F
