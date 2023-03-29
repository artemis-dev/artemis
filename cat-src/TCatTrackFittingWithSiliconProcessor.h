/* @file TCatTrackFittingWithSiliconProcessor.h
 * @brief track fitting with silicon and beam
 *
 * @date Create        : 2019-05-21 11:20:02 JST
 *       Last Modified : 2019-05-15 11:05:47 JST (ota)
 * @author: Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 */

#ifndef INCLUDE_GUARD_UUID_A36B47F6_BAC1_4655_9217_5F431B30563E
#define INCLUDE_GUARD_UUID_A36B47F6_BAC1_4655_9217_5F431B30563E

#include "TProcessor.h"
#include "TCatDiffusedChargeResponseHelper.h"
#include "TCatFunctionChargeResidual.h"

namespace art {
   class TCatTrackFittingWithSiliconProcessor;
   class TCatTpcTrack;
   class TTimingChargePositionData;
}


class art::TCatTrackFittingWithSiliconProcessor : public TProcessor {
public:
   TCatTrackFittingWithSiliconProcessor();
   virtual ~TCatTrackFittingWithSiliconProcessor();

   virtual void Process();

protected:

   InputData<TClonesArray, TCatTpcTrack> fInputRecoil;
   InputData<TClonesArray, TTimingChargePositionData> fInputSilicon;
   InputData<TClonesArray, TCatTpcTrack> fInputBeam;
   OutputData<TClonesArray, TCatTpcTrack> fOutput;
   Parameter<DoubleVec_t> fSigmaCoeff; // sigma coefficient [p0*p0 + p1*p1*N + p2*p2*N*N]
    

   TCatDiffusedChargeResponseHelper fChargeResponseHelper; //!
   TCatFunctionChargeResidual       fFunctionChargeResidual; //!

   
   ClassDef(TCatTrackFittingWithSiliconProcessor,1)
}; // end of TCatTrackFittingWithSiliconProcessor

#endif // #ifndef INCLUDE_GUARD_UUID_A36B47F6_BAC1_4655_9217_5F431B30563E}
