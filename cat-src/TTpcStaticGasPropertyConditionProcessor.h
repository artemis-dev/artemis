/* @file TTpcStaticGasPropertyConditionProcessor.h
 * @brief condition processor fot static gas property of tpc
 *
 * @date Create        : 2020-04-23 15:06:05 JST
 *       Last Modified : 2020-05-21 17:18:25 JST (ota)
 * @author: Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 */

#ifndef INCLUDE_GUARD_UUID_D9227AE5_0117_4C87_89F6_2711496BB8D1
#define INCLUDE_GUARD_UUID_D9227AE5_0117_4C87_89F6_2711496BB8D1

#include "TProcessor.h"
#include "TTpcStaticGasProperty.h"

namespace art {
   class TTpcStaticGasPropertyConditionProcessor;
}


class art::TTpcStaticGasPropertyConditionProcessor : public TProcessor {
public:
   TTpcStaticGasPropertyConditionProcessor();
   virtual ~TTpcStaticGasPropertyConditionProcessor();

   virtual void Init(TEventCollection *col);
   
protected:
   OutputInfo<TTpcStaticGasProperty> fProperty; //!
   Parameter<double> fDriftVelocity;
   Parameter<double> fTransDiffCoeff;
   Parameter<double> fLongDiffCoeff;
   Parameter<double> fConstTransDiff;
   Parameter<double> fConstLongDiff;
   Parameter<double> fAttachCoeff;
   Parameter<double> fWorkFunction;

   
   ClassDef(TTpcStaticGasPropertyConditionProcessor,1)
}; // end of TTpcStaticGasPropertyConditionProcessor

#endif // #ifndef INCLUDE_GUARD_UUID_D9227AE5_0117_4C87_89F6_2711496BB8D1}
