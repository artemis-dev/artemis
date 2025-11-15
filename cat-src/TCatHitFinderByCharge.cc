/**
 * @file   TCatHitFinderByCharge.cc
 * @brief  hit finder by charge
 *
 * @date   Created       : 2015-01-02 08:44:46 JST
 *         Last Modified : Jan 03, 2015 09:56:27 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2015 Shinsuke OTA
 */

#include "TCatHitFinderByCharge.h"
#include <TClonesArray.h>
#include <TCatPulseShape.h>

using art::TCatHitFinderByCharge;

const Float_t TCatHitFinderByCharge::fgDefaultChargeThreshold = kDefaultChargeThreshold;

ClassImp(TCatHitFinderByCharge)

TCatHitFinderByCharge::TCatHitFinderByCharge()
{
   RegisterInputCollection("InputCollection","name of input collection",fInputName,TString("atsub"),
                           &fInput,TClonesArray::Class_Name(),art::TCatPulseShape::Class_Name());
   RegisterOutputCollection("OutputCollection","name of output collection",fOutputName,TString("athit"),
                            &fOutput,TClonesArray::Class_Name(),art::TCatPulseShape::Class_Name());

   RegisterProcessorParameter("ChargeThreshold","charge threshold for selection by charge",
                             fChargeThreshold,fgDefaultChargeThreshold);

}

TCatHitFinderByCharge::~TCatHitFinderByCharge()
{
}

TCatHitFinderByCharge::TCatHitFinderByCharge(const TCatHitFinderByCharge& rhs)
{
}

TCatHitFinderByCharge& TCatHitFinderByCharge::operator=(const TCatHitFinderByCharge& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}

void TCatHitFinderByCharge::Process()
{
   fOutput->Clear("C");
   Int_t nHit = (*fInput)->GetEntriesFast();

   for (Int_t iHit = 0; iHit != nHit; iHit++) {
      TCatPulseShape *pulse = (TCatPulseShape*) (*fInput)->UncheckedAt(iHit);
      if (fChargeThreshold > pulse->GetCharge()) {
         continue;
      }
      TCatPulseShape *outpulse = (TCatPulseShape*) fOutput->ConstructedAt(fOutput->GetEntriesFast());
      pulse->Copy(*outpulse);
   }
}
