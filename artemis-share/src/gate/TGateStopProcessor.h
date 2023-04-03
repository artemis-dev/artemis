/**
 * @file   TGateStopProcessor.h
 * @brief  stop eventloop by gate
 *
 * @date   Created       : 2014-03-19 12:34:38 JST
 *         Last Modified : 2014-03-19 16:31:27 JST (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 KAWASE Shoichiro
 */

#ifndef INCLUDE_GUARD_UUID_80736DAB_1737_4599_94A8_957397ECB668
#define INCLUDE_GUARD_UUID_80736DAB_1737_4599_94A8_957397ECB668

#include "TProcessor.h"

namespace art {
   class TGateStopProcessor;
   class TGateObject;
}

class art::TGateStopProcessor : public TProcessor {
public:
   TGateStopProcessor();
   virtual ~TGateStopProcessor();

   virtual void Init(TEventCollection* col);
   virtual void Process();
   
protected:

private:
   Int_t fStopIf;
   TString fGateName;
   TGateObject *fGate;
   
   ClassDef(TGateStopProcessor,1) // stop eventloop by gate
};

#endif // INCLUDE_GUARD_UUID_80736DAB_1737_4599_94A8_957397ECB668
