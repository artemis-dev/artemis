/* @file TCatTrackFindWithSiliconProcessor.h
 * @brief track finding class for silicon trigger events in ribf113 
 *
 * @date Create        : 2019-05-16 20:30:28 JST
 *       Last Modified : 2019-05-15 11:05:47 JST (ota)
 * @author: Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 */

#ifndef INCLUDE_GUARD_UUID_300C8F3F_96CB_4E69_A5F7_7FA5C6FF8A32
#define INCLUDE_GUARD_UUID_300C8F3F_96CB_4E69_A5F7_7FA5C6FF8A32

#include "TProcessor.h"

namespace art {
   class TCatReadoutPadArray;
   namespace ribf113 {
      class TCatTrackFindWithSiliconProcessor;
   }
}

class TH2;

class art::ribf113::TCatTrackFindWithSiliconProcessor : public TProcessor {
public:
   TCatTrackFindWithSiliconProcessor();
   virtual ~TCatTrackFindWithSiliconProcessor();

   
   virtual void Init(TEventCollection *col);
   virtual void Process();

protected:
   TString fBeamName;
   TString fSiliconName;
   TString fRecoilName;
   TString fOutputName;
   TString fOutputTrackName;
   TString fReadoutPadName;
   Double_t fMaxDistance;
   
   TClonesArray **fInputBeam; //! input TClonesArray of beam track
   TClonesArray **fInputSilicon; //! input TClonesArray of silicon hits
   TClonesArray **fInputRecoil; //! input TClonesArray of recoil hits
   TClonesArray *fOutput; //! output TClonesArray of candidate hits for track
   TClonesArray *fOutputTrack; //! output TClonesArray of candidate track
   TCatReadoutPadArray **fReadoutPadArray; //!
    

   TH2 *fHistHough; //! helper histogram
   TH2 *fHitPattern; //! hit pattern
   Int_t fNumBinsTheta;
   Int_t fNumBinsPhi;
   
   ClassDef(TCatTrackFindWithSiliconProcessor,1)
}; // end of TCatTrackFindWithSiliconProcessor

#endif // #ifndef INCLUDE_GUARD_UUID_300C8F3F_96CB_4E69_A5F7_7FA5C6FF8A32}
