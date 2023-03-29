/* @file TSiliconEventGenerator.h
 * @brief silicon event generator from hits
 *
 * @date Create        : 2019-05-21 20:29:35 JST
 *       Last Modified : 2019-05-15 11:05:47 JST (ota)
 * @author: Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 */

#ifndef INCLUDE_GUARD_UUID_6283BE96_F483_41C6_93DB_42CED3D8AC2E
#define INCLUDE_GUARD_UUID_6283BE96_F483_41C6_93DB_42CED3D8AC2E

#include "TProcessor.h"

namespace art {
   class TSiliconEventGenerator;
   class TCatTpcTrack;
   class TTimingChargePositionData;
   class TRangeTableHelper;
}


class art::TSiliconEventGenerator : public TProcessor {
public:
   TSiliconEventGenerator();
   virtual ~TSiliconEventGenerator();
   virtual void Process();

protected:
   InputData<TClonesArray, TCatTpcTrack> fInput; // input trajectory
   OutputData<TClonesArray, TTimingChargePositionData> fOutput;
   InputInfo<TObjArray> fPosition;
   InputInfo<TRangeTableHelper> fRangeTable;
   Parameter<Double_t> fThreshold;
   
   
   ClassDef(TSiliconEventGenerator,1)
}; // end of TSiliconEventGenerator

#endif // #ifndef INCLUDE_GUARD_UUID_6283BE96_F483_41C6_93DB_42CED3D8AC2E}
