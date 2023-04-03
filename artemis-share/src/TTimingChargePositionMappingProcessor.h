/* @file TTimingChargePositionMappingProcessor.h
 * @brief mapping timing, charge and position
 *
 * @date Create        : 2019-05-15 11:39:04 JST
 *       Last Modified : 2019-05-15 13:26:50 JST (ota)
 * @author: Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 */


#ifndef INCLUDE_GUARD_UUID_C6858596_6798_42C2_95BE_2E1219DBA8D1
#define INCLUDE_GUARD_UUID_C6858596_6798_42C2_95BE_2E1219DBA8D1

#include "TTimingChargeMappingProcessor.h"

namespace art {
   class TTimingChargePositionMappingProcessor;
}

class TObjArray;

class art::TTimingChargePositionMappingProcessor : public TTimingChargeMappingProcessor {
public:
   TTimingChargePositionMappingProcessor();
   virtual ~TTimingChargePositionMappingProcessor();

   virtual void Process();

protected:
   TString fPositionInfoName;
   TObjArray **fPosition; //!
   

   ClassDef(TTimingChargePositionMappingProcessor,1)

}; // end of TTimingChargePositionMappingProcessor

#endif // #ifndef INCLUDE_GUARD_UUID_C6858596_6798_42C2_95BE_2E1219DBA8D1}
