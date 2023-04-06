/**
 * @file   THitFinder.h
 * @brief  hit finder for tracking detector
 *
 * @date   Created       : 2014-03-08 09:31:23 JST
 *         Last Modified : 2018-02-04 11:53:27 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_FA3A649B_2426_466A_9CC0_600CE9B50690
#define INCLUDE_GUARD_UUID_FA3A649B_2426_466A_9CC0_600CE9B50690

#include "TProcessor.h"

namespace art {
   class THitFinder;
   class TCatReadoutPadArray;
}

class TH3F;
class TH2F;
class TClonesArray;
class art::THitFinder : public TProcessor {
public:
   THitFinder();
   virtual ~THitFinder();

   THitFinder(const THitFinder& rhs);
   THitFinder& operator=(const THitFinder& rhs);

   virtual void Init(TEventCollection *col);
   virtual void Process();

protected:
   
private:
   TClonesArray **fInput; //!
   TClonesArray *fOutput; //!
   TClonesArray *fHitCandidates; //!

   TString  fInputColName;
   TString     fOutputColName;
   TString fReadoutPadName;

   Float_t    fAcceptableOffsetDifference;
   TCatReadoutPadArray *fPads; //!


   
   
   TH3F *hHough; //!
   TH2F *hHough2D; //!
   TH2F *hHough2DXZ; //!
   ClassDef(THitFinder,1) // hit finder for tracking detector
};

#endif // INCLUDE_GUARD_UUID_FA3A649B_2426_466A_9CC0_600CE9B50690
