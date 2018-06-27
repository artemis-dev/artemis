/**
 * @file   THeaderTimestampProcessor.h
 * @brief  generate timestamp from eventheader
 *
 * @date   Created       : 2018-06-25 21:06:47 JST
 *         Last Modified : 2018-06-25 21:16:27 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2018 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_CF3F0010_0CAA_450A_B6CD_D11C2CAC2328
#define INCLUDE_GUARD_UUID_CF3F0010_0CAA_450A_B6CD_D11C2CAC2328

#include "TProcessor.h"

namespace art {
   class THeaderTimestampProcessor;
   class TEventHeader;
   class TSimpleData;
}

class art::THeaderTimestampProcessor : public TProcessor {
public:
   THeaderTimestampProcessor();
   virtual ~THeaderTimestampProcessor();

   THeaderTimestampProcessor(const THeaderTimestampProcessor& rhs);
   THeaderTimestampProcessor& operator=(const THeaderTimestampProcessor& rhs);

   virtual void Init(TEventCollection *col);
   virtual void Process();

protected:

   TString fInputName; // name of input header
   TEventHeader **fInput; //! input event header

   TString fOutputName; // name of output timestamp
   TSimpleData *fOutput; // output
   Double_t fFrequency; // frequency

   Long64_t fFirstTimestamp; //! first timestamp

   

private:

   ClassDef(THeaderTimestampProcessor,1) // generate timestamp from eventheader
};

#endif // INCLUDE_GUARD_UUID_CF3F0010_0CAA_450A_B6CD_D11C2CAC2328
