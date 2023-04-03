/**
 * @file   TEventSkipProcessor.h
 * @brief  skip some events in the beging and/or at the end of run
 *
 * @date   Created       : 2015-06-03 16:56:48 JST
 *         Last Modified : 2015-06-03 17:17:21 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2015 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_BEF9BA13_D6B4_4431_9F55_3CF689077A0F
#define INCLUDE_GUARD_UUID_BEF9BA13_D6B4_4431_9F55_3CF689077A0F

#include "TProcessor.h"

namespace art {
   class TEventSkipProcessor;
}

class art::TEventSkipProcessor : public TProcessor {
public:
   TEventSkipProcessor();
   virtual ~TEventSkipProcessor();

   TEventSkipProcessor(const TEventSkipProcessor& rhs);
   TEventSkipProcessor& operator=(const TEventSkipProcessor& rhs);

   virtual void Process();
   virtual void PostLoop();

protected:

private:
   Int_t fStart;
   Int_t fMaxNumEvent;
   Int_t fNumEvent;
   Int_t fNumEventAnalyzed;
   ClassDef(TEventSkipProcessor,1) // skip some events in the beging and/or at the end of run
};

#endif // INCLUDE_GUARD_UUID_BEF9BA13_D6B4_4431_9F55_3CF689077A0F
