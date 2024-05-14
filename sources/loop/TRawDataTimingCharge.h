/**
 * @file   TRawDataTimingCharge.h
 * @brief  raw data with timing and charge
 *
 * @date   Created       : 2015-10-04 17:57:15 JST
 *         Last Modified : 2015-10-04 18:26:02 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2015 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_0297691A_7BD8_4621_A584_E16BC0F2D7A5
#define INCLUDE_GUARD_UUID_0297691A_7BD8_4621_A584_E16BC0F2D7A5

#include "TRawDataObject.h"

namespace art {
   class TRawDataTimingCharge;
}

class art::TRawDataTimingCharge : public TRawDataObject {
public:
   TRawDataTimingCharge();
   virtual ~TRawDataTimingCharge();

   TRawDataTimingCharge(const TRawDataTimingCharge& rhs);
   TRawDataTimingCharge& operator=(const TRawDataTimingCharge& rhs);

   Double_t GetTiming() const { return fTiming; }
   Double_t GetCharge() const { return fCharge; }
   
   void SetTiming(Double_t timing) { fTiming = timing; }
   void SetCharge(Double_t charge) { fCharge = charge; }

   virtual Double_t GetValue(Int_t idx = 0) const { 
     if (idx == 0) return fTiming;
     if (idx == 1) return fCharge;
   }
   virtual Int_t GetNumValues() const { return 2; }

     
      
protected:
   Double_t fCharge;
   Double_t fTiming;

private:

   ClassDef(TRawDataTimingCharge,1) // raw data with timing and charge
};

#endif // INCLUDE_GUARD_UUID_0297691A_7BD8_4621_A584_E16BC0F2D7A5
