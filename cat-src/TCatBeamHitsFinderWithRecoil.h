/**
 * @file   TCatBeamHitsFinderWithRecoil.h
 * @brief find hits within whole beam hits,  produsing the corresponding recoil particle
 *
 * @date   Created       : 2017-09-22 21:18:42 JST
 *         Last Modified : 2018-02-10 04:18:04 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2017 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_92F794D9_3404_45A3_BEB6_7CBDE23B9242
#define INCLUDE_GUARD_UUID_92F794D9_3404_45A3_BEB6_7CBDE23B9242

#include "TProcessor.h"

namespace art {
   class TCatBeamHitsFinderWithRecoil;
}

class TClonesArray;

class art::TCatBeamHitsFinderWithRecoil : public TProcessor {
public:
   TCatBeamHitsFinderWithRecoil();
   virtual ~TCatBeamHitsFinderWithRecoil();

   TCatBeamHitsFinderWithRecoil(const TCatBeamHitsFinderWithRecoil& rhs);
   TCatBeamHitsFinderWithRecoil& operator=(const TCatBeamHitsFinderWithRecoil& rhs);

   virtual void Process();

protected:
   TString fNameInput; // name of input beam hits collection
   TClonesArray** fInput; //! input beam hits

   TString fNameRecoil; // name of reference recoil track
   TClonesArray** fRecoil; //! reference recoil track (TCatTpcTrack)

   TString fNameOutput; // name of output beam hits collection
   TClonesArray* fOutput; //! output beam hits

   Double_t fMaxDistance; // maximum distance from recoil track (mm)

private:

   ClassDef(TCatBeamHitsFinderWithRecoil,1) //
};

#endif // INCLUDE_GUARD_UUID_92F794D9_3404_45A3_BEB6_7CBDE23B9242
