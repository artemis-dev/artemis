/* @file TChargeRiseTimeCompensateProcessor.h
 * @brief charge correction by rise time
 *
 * @date Create        : 2019-07-22 22:38:11 JST
 *       Last Modified : 2019-07-22 23:22:02 JST (ota)
 * @author: Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 */

#ifndef INCLUDE_GUARD_UUID_0FD1076D_B2B4_4A0C_B03A_EBCC1C39B691
#define INCLUDE_GUARD_UUID_0FD1076D_B2B4_4A0C_B03A_EBCC1C39B691

#include "TProcessor.h"

namespace art {
   class TChargeRiseTimeCompensateProcessor;
   class TCatPulseShape;
}


class art::TChargeRiseTimeCompensateProcessor : public TProcessor {
public:
   TChargeRiseTimeCompensateProcessor();
   virtual ~TChargeRiseTimeCompensateProcessor();

   virtual void Process();
   
protected:
   InputData<TClonesArray, TCatPulseShape> fInput;
   Parameter<DoubleVec_t> fCoeff;
   
   
   ClassDef(TChargeRiseTimeCompensateProcessor,1)
}; // end of TChargeRiseTimeCompensateProcessor

#endif // #ifndef INCLUDE_GUARD_UUID_0FD1076D_B2B4_4A0C_B03A_EBCC1C39B691}
