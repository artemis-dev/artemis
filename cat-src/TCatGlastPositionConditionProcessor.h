/* @file TCatGlastPositionConditionProcessor.h
 * @brief generate position information for glast silicon detector
 *
 * @date Create        : 2019-05-16 22:08:35 JST
 *       Last Modified : 2019-06-14 01:18:55 JST (ota)
 * @author: Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 */

#ifndef INCLUDE_GUARD_UUID_34C3BF4B_B478_409A_9BFC_868C2F8CF60F
#define INCLUDE_GUARD_UUID_34C3BF4B_B478_409A_9BFC_868C2F8CF60F

#include "TProcessor.h"

namespace art {
   namespace ribf113 {
      class TCatGlastPositionConditionProcessor;
   }
   class TDetectorGeometryInfo;
}


class art::ribf113::TCatGlastPositionConditionProcessor : public TProcessor {
public:
   TCatGlastPositionConditionProcessor();
   virtual ~TCatGlastPositionConditionProcessor();

   virtual void Init(TEventCollection *col);

protected:
   OutputInfo<TClonesArray,TDetectorGeometryInfo> fGeometry;
   Parameter<DoubleVec_t> fOffsetLeft;
   Parameter<DoubleVec_t> fOffsetRight;
   Parameter<Double_t> fDeadLayer;

   ClassDef(TCatGlastPositionConditionProcessor,1)
}; // end of TCatGlastPositionConditionProcessor

#endif // #ifndef INCLUDE_GUARD_UUID_34C3BF4B_B478_409A_9BFC_868C2F8CF60F}
