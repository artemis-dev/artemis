/**
 * @file   THogeProcessor.h
 * @brief  hoge
 *
 * @date   Created       : 2015-01-31 19:35:11 JST
 *         Last Modified : Feb 03, 2015 17:29:16 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2015 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_C45A531A_1F7B_4B8B_A025_E8A253328107
#define INCLUDE_GUARD_UUID_C45A531A_1F7B_4B8B_A025_E8A253328107

#include "TProcessor.h"

namespace art {
   class THogeProcessor;
   class TEventCollection;
}

class art::THogeProcessor : public TProcessor {
public:
   THogeProcessor();
   virtual ~THogeProcessor();

   THogeProcessor(const THogeProcessor& rhs);
   THogeProcessor& operator=(const THogeProcessor& rhs);

   void Init(TEventCollection *col);
   void Process();

protected:
   Double_t *fRandom; //!
   Float_t  *fRandom2; //!
   Int_t    *fNumRandom; //!
   TString fOutputName;
   TString fOutputName2;
   TString fOutputName3;

private:

   ClassDef(THogeProcessor,1) // hoge
};

#endif // INCLUDE_GUARD_UUID_C45A531A_1F7B_4B8B_A025_E8A253328107
