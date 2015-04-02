/**
 * @file   TTrackG4.h
 * @brief  track information in Geant4
 *
 * @date   Created       : 2014-10-28 14:48:56 JST
 *         Last Modified : Oct 28, 2014 15:08:24 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_9C7D9515_07AB_42AA_A7DB_EE2277D9849A
#define INCLUDE_GUARD_UUID_9C7D9515_07AB_42AA_A7DB_EE2277D9849A

#include "TObject.h"
#include <TVector3.h>
namespace art {
   class TTrackG4;
}

class art::TTrackG4 : public TObject {
public:
   TTrackG4();
   TTrackG4(Int_t id, Double_t pretime, Double_t posttime, Double_t energy,
            Double_t preX, Double_t preY, Double_t preZ,
            Double_t postX, Double_t postY, Double_t postZ,
            Int_t trackid, Int_t parentid, Int_t PDGencoding);

   virtual ~TTrackG4();

   TTrackG4(const TTrackG4& rhs);
   TTrackG4& operator=(const TTrackG4& rhs);

   Int_t    fID; // detector id 
   Double_t fPreTime; // time
   Double_t fPostTime; // time
   Double_t fEnergy; // energy deposition in this track
   TVector3 fPreStep; // prestep point
   TVector3 fPostStep; // poststep point
   Int_t    fTrackID;
   Int_t    fParentID;
   Int_t    fPDGEncoding;

   ClassDef(TTrackG4,1) // track information in Geant4
};

#endif // INCLUDE_GUARD_UUID_9C7D9515_07AB_42AA_A7DB_EE2277D9849A
