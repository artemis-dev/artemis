/**
 * @file   TTimeValidator.cc
 * @brief  validate time or timestamp in certain window
 *
 * @date   Created       : 2015-11-17 13:53:41 JST
 *         Last Modified : Dec 02, 2015 15:26:30 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2015 Shinsuke OTA
 */

#include "TTimeValidator.h"
#include <TClonesArray.h>
#include <ITiming.h>
#include <TDataObject.h>

using art::TTimeValidator;

ClassImp(TTimeValidator)

TTimeValidator::TTimeValidator()
{
   RegisterInputCollection("InputCollection","Input collection",
                           fInputName,TString("input"),&fInput,
                           TClonesArray::Class_Name(),ITiming::Class_Name());
   RegisterOutputCollection("OutputCollection","Output collection",fOutputName,TString("validated"));

   const FloatVec_t range(3,0.);
   RegisterProcessorParameter("ValidTimeRange","[min,max,offset] => Tmin = offset + min etc. ignored if min == max",
                              fValidTimeRange,range);
   

}

TTimeValidator::~TTimeValidator()
{
}

TTimeValidator::TTimeValidator(const TTimeValidator& rhs)
{
}

TTimeValidator& TTimeValidator::operator=(const TTimeValidator& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}


void TTimeValidator::Init(TEventCollection *col)
{
   if (fValidTimeRange[0] > fValidTimeRange[1]) {
      SetStateError("Time range : min > max");
      return;
   }
   fValidTimeMin = fValidTimeRange[2] + fValidTimeRange[0];
   fValidTimeMax = fValidTimeRange[2] + fValidTimeRange[1];
   
   const TClass* inClass = (*fInput)->GetClass();
   if (!inClass->InheritsFrom(TDataObject::Class())) {
      SetStateError(Form("Input class '%s' does not inherits from TDataObject",inClass->GetName()));
      return;
   }
   fOutput = new TClonesArray(inClass);
   col->Add(fOutputName,fOutput,fOutputIsTransparent);
}

void TTimeValidator::Process()
{
   fOutput->Clear("C");
   Int_t nHits = (*fInput)->GetEntriesFast();
   for (Int_t iHit = 0; iHit != nHits; ++iHit) {
      const ITiming *const timingData = dynamic_cast<const ITiming*>((*fInput)->At(iHit));
      const TDataObject *const data = static_cast<TDataObject*>((*fInput)->At(iHit));
      if (!timingData) continue;
      Double_t timing = timingData->GetTiming();
      if (timing < fValidTimeMin || fValidTimeMax < timing) {
         continue;
      }
      TObject *const outData = fOutput->ConstructedAt(fOutput->GetEntriesFast());
      data->Copy(*outData);
   }
}
   
