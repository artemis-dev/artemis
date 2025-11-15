/**
 * @file   TCatBeamHitsFinderWithRecoil.cc
 * @brief find hits within whole beam hits,  produsing the corresponding recoil particle
 *
 * @date   Created       : 2017-09-22 21:21:17 JST
 *         Last Modified : 2018-06-19 00:52:04 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2017 Shinsuke OTA
 */

#include "TCatBeamHitsFinderWithRecoil.h"
#include <TClonesArray.h>
#include <TCatPulseShape.h>
#include <TCatTpcTrack.h>

using art::TCatBeamHitsFinderWithRecoil;

ClassImp(TCatBeamHitsFinderWithRecoil)

TCatBeamHitsFinderWithRecoil::TCatBeamHitsFinderWithRecoil()
{
   RegisterInputCollection("Input","name of input beam hits",fNameInput,"atbeamhits",
                           &fInput,TClonesArray::Class_Name(),art::TCatPulseShape::Class_Name());
   RegisterInputCollection("Recoil","name of reference recoil track",fNameRecoil,"atrecoil",
                           &fRecoil,TClonesArray::Class_Name(),art::TCatTpcTrack::Class_Name());
   RegisterOutputCollection("Output","name of output beam hits",fNameOutput,"atbeamhits",
                           &fOutput,TClonesArray::Class_Name(),art::TCatPulseShape::Class_Name());
   RegisterProcessorParameter("MaxDistance","maximum distance from recoil track",fMaxDistance,10.);
}

TCatBeamHitsFinderWithRecoil::~TCatBeamHitsFinderWithRecoil()
{
}

TCatBeamHitsFinderWithRecoil::TCatBeamHitsFinderWithRecoil(const TCatBeamHitsFinderWithRecoil& rhs)
{
}

TCatBeamHitsFinderWithRecoil& TCatBeamHitsFinderWithRecoil::operator=(const TCatBeamHitsFinderWithRecoil& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}

void TCatBeamHitsFinderWithRecoil::Process()
{
   fOutput->Clear("C");
   if ((*fRecoil)->GetEntriesFast() == 0 || (*fInput)->GetEntriesFast() == 0) return;
   if ((*fRecoil)->GetEntriesFast() > 1) return;
   TCatTpcTrack *recoil = static_cast<TCatTpcTrack*>((*fRecoil)->UncheckedAt(0));
   TClonesArray *input = *fInput;

   Double_t a = recoil->GetDirection().Y();
   Double_t b = -recoil->GetDirection().X();
   Double_t c = - a * recoil->GetOriginalStartPoint().X() - b * recoil->GetOriginalStartPoint().Y();
   Double_t denom = TMath::Sqrt(a*a+b*b);

   for (Int_t i = 0, n = input->GetEntriesFast(); i < n; ++i) {
      TCatPulseShape *pulse = static_cast<TCatPulseShape*>((input)->UncheckedAt(i));
      Double_t x0 = pulse->GetX();
      Double_t y0 = pulse->GetY();

      Double_t d = TMath::Abs(a * x0 + b * y0 + c)  / denom;
//      printf("id = %d y0 = %f d = %f\n",pulse->GetID(),y0,d);
      if (d > fMaxDistance) continue;


      TCatPulseShape *out = static_cast<TCatPulseShape*>(fOutput->ConstructedAt(fOutput->GetEntriesFast()));
      pulse->Copy(*out);
   }
}
