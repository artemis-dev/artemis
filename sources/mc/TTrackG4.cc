/**
 * @file   TTrackG4.cc
 * @brief  track information for Geant4
 *
 * @date   Created       : 2014-10-28 14:52:44 JST
 *         Last Modified : Oct 28, 2014 15:09:01 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 Shinsuke OTA
 */

#include "TTrackG4.h"

using art::TTrackG4;

ClassImp(TTrackG4)

TTrackG4::TTrackG4()
: fID(-1), fPreTime(0.), fPostTime(0.),fEnergy(0),
   fPreStep(0.,0.,0.),fPostStep(0.,0.,0.),fTrackID(-1),fParentID(-1),fPDGEncoding(-1)
{
}

TTrackG4::TTrackG4(Int_t id, Double_t pretime, Double_t posttime, Double_t energy,
         Double_t preX, Double_t preY, Double_t preZ,
         Double_t postX, Double_t postY, Double_t postZ,
         Int_t trackid, Int_t parentid, Int_t PDGEncoding)
   : fID(id), fPreTime(pretime), fPostTime(posttime), fEnergy(energy),
     fPreStep(preX,preY,preZ), fPostStep(postX,postY,postZ),fTrackID(trackid), fParentID(parentid),fPDGEncoding(PDGEncoding)
{
}
     


TTrackG4::~TTrackG4()
{
}

TTrackG4::TTrackG4(const TTrackG4& rhs)
{
}

TTrackG4& TTrackG4::operator=(const TTrackG4& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}
