/**
 * @file   TTimingTimestampAdapter.h
 * @brief  adopt timing as timestamp (update object)
 *
 * @date   Created       : 2018-07-18 12:49:50 JST
 *         Last Modified : 2018-07-18 12:58:18 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2018 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_99CC2BF2_BE29_4DF9_B377_5259BCF1C501
#define INCLUDE_GUARD_UUID_99CC2BF2_BE29_4DF9_B377_5259BCF1C501

#include "TProcessor.h"

namespace art {
   class TTimingTimestampAdapter;
}

class art::TTimingTimestampAdapter : public TProcessor {
public:
   TTimingTimestampAdapter();
   virtual ~TTimingTimestampAdapter();

   TTimingTimestampAdapter(const TTimingTimestampAdapter& rhs);
   TTimingTimestampAdapter& operator=(const TTimingTimestampAdapter& rhs);

   virtual void Init(TEventCollection *col);
   virtual void Process();

protected:

   TString fInputName; // name of input
   TClonesArray **fInput; //! input


private:

   ClassDef(TTimingTimestampAdapter,1) // adopt timing as timestamp
};

#endif // INCLUDE_GUARD_UUID_99CC2BF2_BE29_4DF9_B377_5259BCF1C501
