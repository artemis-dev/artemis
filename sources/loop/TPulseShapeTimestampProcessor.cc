/**
 * @file   TPulseShapeTimestampProcessor.cc
 * @brief  generate timestamp pulse shape with slow sacler
 *
 * @date   Created       : 2018-07-06 15:51:35 JST
 *         Last Modified : 2018-07-06 19:08:37 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2018 Shinsuke OTA
 */

#include "TPulseShapeTimestampProcessor.h"
#include <TCatPulseShape.h>
#include <TSimpleData.h>
#include <TScalerData.h>
#include <TClonesArray.h>

using art::TPulseShapeTimestampProcessor;

ClassImp(TPulseShapeTimestampProcessor)

TPulseShapeTimestampProcessor::TPulseShapeTimestampProcessor()
{
   RegisterInputCollection("Input","name of pulse shape input",fInputName,TString("at_timestamp"),
                           &fInput,TClonesArray::Class_Name(),TCatPulseShape::Class_Name());
   RegisterInputCollection("ScalerInput","name of scaler input",fScalerInputName,TString("scaler"),
                           &fScalerInput,TScalerData::Class_Name());
   RegisterOutputCollection("Output","name of output timestamp",fOutputName,TString("timestamp"),
                            &fOutput,TSimpleData::Class_Name());
   RegisterProcessorParameter("FastID","id of fast frequency",fFastID,-1);
   RegisterOptionalParameter("SlowID","id of slow frequency",fSlowID,-1);
   RegisterProcessorParameter("FastFrequency","list of fast frequency",fFastFrequency,DoubleVec_t(0));
   RegisterOptionalParameter("SlowFrequency","slow frequency",fSlowFrequency,0.);
   RegisterProcessorParameter("Bits","number of bits",fBits,32);
   RegisterProcessorParameter("MaxFastID","maximum id of input pulse (ignored if FastID is not -1)",fMaxFastID,0);
}

TPulseShapeTimestampProcessor::~TPulseShapeTimestampProcessor()
{
}

TPulseShapeTimestampProcessor::TPulseShapeTimestampProcessor(const TPulseShapeTimestampProcessor& rhs)
{
}

TPulseShapeTimestampProcessor& TPulseShapeTimestampProcessor::operator=(const TPulseShapeTimestampProcessor& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}


void TPulseShapeTimestampProcessor::Init(TEventCollection* col)
{
   const Int_t numChannel = (*fScalerInput)->GetNumChannel();
   const Double_t eps = TMath::Limits<Double_t>::Epsilon();

   if (fFastFrequency.size() <= fMaxFastID) {
      SetStateError(TString::Format("the nubmer of fast frequencies is smaller than maximum fast id: %d < %d",
                                    fFastFrequency.size(), fMaxFastID));
      return;
   }
   if (fFastID < 0) {
      Info("Init","All the input pulse shapes are used to calculate timestamp");
      if (fMaxFastID <= 0) {
         SetStateError(TString::Format("maximum fast id should be set if all the pulse are used"));
         return;
      }
   } else {
      Info("Init","Input pulse shapes with id == %d is used to calculate timestamp",fFastID);
      fMaxFastID = fFastID;
   }      

   if (fSlowID >= numChannel) {
      SetStateError(TString::Format("Slow frequency id '%d' exceeds the limit (%d)",fSlowID,numChannel));
      return;
   }

   if (fSlowFrequency < eps) {
      SetStateError(TString::Format("Bad slow frequency '%f' for id '%d'",fSlowFrequency,fSlowID));
      return;
   }
   Info("Init","Slow: ID %d, Freq %.5g",fSlowID,fSlowFrequency);

   Int_t size = fMaxFastID + 1;
   fFastSaved = DoubleVec_t(size);
   fMaxTimeByFastClock = DoubleVec_t(size);
   fFirstTimestamp = DoubleVec_t(size,-1.);
   for (Int_t i = 0; i < size; ++i) {
      fMaxTimeByFastClock[i] = TMath::Power(2,fBits)/fFastFrequency[i];
   }
}


void TPulseShapeTimestampProcessor::BeginOfRun()
{
   fNumCycles = 0;
   fSlowSaved = 0.;
}

void TPulseShapeTimestampProcessor::Process() {
   fOutput->Clear("C");
   Double_t times = 0.;
   const Double_t slow = (*fScalerInput)->GetValue(fSlowID);
   const Int_t nHits = (*fInput)->GetEntriesFast();
   Int_t nUsed = 0;
   for (Int_t iHit = 0; iHit < nHits; ++iHit) {
      art::TCatPulseShape *pulse = static_cast<art::TCatPulseShape*>((*fInput)->UncheckedAt(iHit));
      const Int_t id = pulse->GetID();
      const Double_t fast = pulse->GetTime();
      if (fFastID >= 0 && id != fFastID) {
         continue;
      }
      nUsed++;
      if (fFirstTimestamp[id] < 0) {
         fFirstTimestamp[id] = fast;
      }
      if (fFastSaved[id] > TMath::Limits<Double_t>::Epsilon() && 
          fast < fFastSaved[id]) {
         // increment the number of cycle when loop over the counter
         fNumCycles++;
      }
      // add cycle when the time more than maximum time pasts
      if (fSlowSaved > TMath::Limits<Double_t>::Epsilon()) {
         fNumCycles += TMath::Floor((slow - fSlowSaved) / fSlowFrequency / fMaxTimeByFastClock[id]);
      }
      fFastSaved[id] = fast;
      fSlowSaved = slow;
      times += (fast - fFirstTimestamp[id]) / fFastFrequency[id]  + fNumCycles * fMaxTimeByFastClock[id];
   }
   fOutput->SetValue(times / nUsed);
}
