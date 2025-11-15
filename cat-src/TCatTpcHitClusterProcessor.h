/**
 * @file   TCatTpcHitClusterProcessor.h
 * @brief  make cluster of tpc hits
 *
 * @date   Created       : 2016-04-17 04:45:26 JST
 *         Last Modified : 2020-05-19 16:14:13 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2016 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_EDE8B975_71B1_4949_BC32_A749251709CD_1
#define INCLUDE_GUARD_UUID_EDE8B975_71B1_4949_BC32_A749251709CD_1

#include "TProcessor.h"
#include <TCatTpcTrackUpdateHelper.h>

namespace art {
   class TCatTpcHitClusterProcessor;
   class TCatReadoutPadArray;
   class TCatPulseShape;
   class TCatTpcTrack;
   class TTpcStaticGasProperty;
}

class TVector3;
class TClonesArray;

class art::TCatTpcHitClusterProcessor : public TProcessor {
public:
   TCatTpcHitClusterProcessor();
   virtual ~TCatTpcHitClusterProcessor();

   TCatTpcHitClusterProcessor(const TCatTpcHitClusterProcessor& rhs);
   TCatTpcHitClusterProcessor& operator=(const TCatTpcHitClusterProcessor& rhs);

   virtual void Process();

   virtual void Init(TEventCollection *col);

protected:
   InputData<TClonesArray,TCatPulseShape> fInput; //! input array
   OutputData<TClonesArray,TCatTpcTrack> fOutput;
   InputInfo<TCatReadoutPadArray> fReadoutPadArray;
   InputInfo<TTpcStaticGasProperty> fTpcProperty;
   Parameter<DoubleVec_t> fClusterRadius;
   Parameter<Bool_t> fIsSingle;
   Parameter<Double_t> fMinClusterSize;
   Parameter<Double_t> fMinHeight;
   Parameter<DoubleVec_t> fPulseLengthRange;
   Parameter<Int_t> fSelectIf;
   Parameter<DoubleVec_t> fSelectedRegion;
   Parameter<Bool_t> fDoTrackUpdate;

   TCatTpcTrackUpdateHelper fTrackUpdateHelper;

private:
   

   ClassDef(TCatTpcHitClusterProcessor,1) // select recoil candidate
};

#endif // INCLUDE_GUARD_UUID_EDE8B975_71B1_4949_BC32_A749251709CD_1
