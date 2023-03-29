/* @file TCatEvalTpcChargeResolution.h
 * @brief evaluation of charge resolution
 *
 * @date Create        : 2020-07-22 06:45:32 JST
 *       Last Modified : 2020-07-22 22:22:58 JST (ota)
 * @author: Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 */

#ifndef INCLUDE_GUARD_UUID_7C6F7867_8121_4F25_B51A_26053AE46DE6
#define INCLUDE_GUARD_UUID_7C6F7867_8121_4F25_B51A_26053AE46DE6

#include "TProcessor.h"

namespace art {
   class TCatEvalTpcChargeResolution;
   class TCatTpcTrack;
   class TCatReadoutPadArray;
}

class TH1;


class art::TCatEvalTpcChargeResolution : public TProcessor {
public:
   TCatEvalTpcChargeResolution();
   virtual ~TCatEvalTpcChargeResolution();
   virtual void Init(TEventCollection* col);
   virtual void Process();
   virtual void EndOfRun();
   

protected:
   InputData<TClonesArray,TCatTpcTrack> fInput;
   Parameter<bool> fDoUseHits; // flag to use each hit (track.fHits) instead of representative (track)
   InputInfo<TCatReadoutPadArray> fReadoutPadArray;
   Parameter<DoubleVec_t> fXaxis;
   Parameter<DoubleVec_t> fYaxis;
   Parameter<TString> fOutputFilename;
   
   
   std::vector<TH1*> fHists;
   

   ClassDef(TCatEvalTpcChargeResolution,1)
}; // end of TCatEvalTpcChargeResolution

#endif // #ifndef INCLUDE_GUARD_UUID_7C6F7867_8121_4F25_B51A_26053AE46DE6}
