/* @file TChargeValidator.h
 * @brief description
 *
 * @date Create        : 2019-06-07 10:48:44 JST
 *       Last Modified : 2019-05-15 11:05:47 JST (ota)
 * @author: Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 */

#ifndef INCLUDE_GUARD_UUID_71B90E69_5081_47A3_B092_842838CE82BB
#define INCLUDE_GUARD_UUID_71B90E69_5081_47A3_B092_842838CE82BB

#include "TProcessor.h"

namespace art {
   class TChargeValidator;
   class ICharge;
}


class art::TChargeValidator : public TProcessor {
public:
   TChargeValidator();
   virtual ~TChargeValidator();

   virtual void Init(TEventCollection *col);
   virtual void Process();

protected:
   InputData<TClonesArray,ICharge> fInput;
   OutputData<TClonesArray,ICharge> fOutput;
   Parameter<DoubleVec_t> fChargeRange;
   

   ClassDef(TChargeValidator,1)
}; // end of TChargeValidator

#endif // #ifndef INCLUDE_GUARD_UUID_71B90E69_5081_47A3_B092_842838CE82BB}
