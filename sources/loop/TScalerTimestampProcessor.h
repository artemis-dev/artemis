/**
 * @file   TScalerTimestampProcessor.h
 * @brief  generate timestamp from scaler
 *
 * @date   Created       : 2018-06-25 19:47:59 JST
 *         Last Modified : 2018-06-27 17:40:54 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2018 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_93A1B318_3A31_4473_9023_29D9D27D74F3
#define INCLUDE_GUARD_UUID_93A1B318_3A31_4473_9023_29D9D27D74F3

#include "TProcessor.h"

namespace art {
   class TScalerTimestampProcessor;
   class TScalerData;
   class TSimpleData;
}

class art::TScalerTimestampProcessor : public TProcessor {
public:
   TScalerTimestampProcessor();
   virtual ~TScalerTimestampProcessor();

   TScalerTimestampProcessor(const TScalerTimestampProcessor& rhs);
   TScalerTimestampProcessor& operator=(const TScalerTimestampProcessor& rhs);

   virtual void Init(TEventCollection *col);
   virtual void Process();

protected:

   TString fInputName; // name of scaler input
   TScalerData **fInput; //!

   TString fOutputName; // name of timestamp output
   TSimpleData *fOutput; //!

   Int_t fFastID; // scaler channel id for fast frequency
   Double_t fFastFrequency; // fast frequency

   Int_t fSlowID; // scaler channel id for slow frequency
   Double_t fSlowFrequency; // slow frequency

   Int_t fBits; // number of bits
   

private:

   

   ClassDef(TScalerTimestampProcessor,1) // generate timestamp from scaler
};

#endif // INCLUDE_GUARD_UUID_93A1B318_3A31_4473_9023_29D9D27D74F3
