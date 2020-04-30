/**
 * @file   TScalerTimestampProcessor.cc
 * @brief  generate timestamp from scaler
 *
 * @date   Created       : 2018-06-25 20:21:53 JST
 *         Last Modified : 2018-06-29 01:51:24 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2018 Shinsuke OTA
 */

#include "TScalerTimestampProcessor.h"
#include <TScalerData.h>
#include <TSimpleData.h>
#include <TMath.h>

using art::TScalerTimestampProcessor;

ClassImp(TScalerTimestampProcessor)

TScalerTimestampProcessor::TScalerTimestampProcessor()
: fNumCycles(0), fFastSaved(0), fSlowSaved(0), fMaxTimeByFastClock(0)
{

   RegisterInputCollection("Input","name of scaler input",fInputName,TString("scaler"),
                           &fInput,TScalerData::Class_Name());
   RegisterOutputCollection("Output","name of output timestamp",fOutputName,TString("timestamp"),
                            &fOutput,TSimpleData::Class_Name());
   RegisterProcessorParameter("FastID","id of fast frequency",fFastID,-1);
   RegisterOptionalParameter("SlowID","id of slow frequency",fSlowID,-1);
   RegisterProcessorParameter("FastFrequency","fast frequency",fFastFrequency,0.);
   RegisterOptionalParameter("SlowFrequency","slow frequency",fSlowFrequency,0.);
   RegisterProcessorParameter("Bits","number of bits",fBits,32);
}

TScalerTimestampProcessor::~TScalerTimestampProcessor()
{
}

TScalerTimestampProcessor::TScalerTimestampProcessor(const TScalerTimestampProcessor& rhs)
{
}

TScalerTimestampProcessor& TScalerTimestampProcessor::operator=(const TScalerTimestampProcessor& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}


void TScalerTimestampProcessor::Init(TEventCollection* col)
{
   const Int_t numChannel = (*fInput)->GetNumChannel();
   const Double_t eps = TMath::Limits<Double_t>::Epsilon();
   if (fFastID < 0 || fFastFrequency < eps ) {
      SetStateError(TString::Format("Fast frequency id should be set"));
      return;
   }

   if (fFastID >= numChannel) {
      SetStateError(TString::Format("Fast frequency id '%d' exceeds the limit (%d)",fFastID,numChannel));
      return;
   }

   if (fSlowID >= numChannel) {
      SetStateError(TString::Format("Slow frequency id '%d' exceeds the limit (%d)",fSlowID,numChannel));
      return;
   }

   if (fFastFrequency < eps) {
      SetStateError(TString::Format("Bad fast frequency '%f' for id '%d'",fFastFrequency,fFastID));
      return;
   }
   if (fSlowFrequency < eps) {
      SetStateError(TString::Format("Bad slow frequency '%f' for id '%d'",fSlowFrequency,fSlowID));
      return;
   }

   Info("Init","Fast: ID %d, Freq %.5g",fFastID,fFastFrequency);
   Info("Init","Slow: ID %d, Freq %.5g",fSlowID,fSlowFrequency);
   
      
}


void TScalerTimestampProcessor::BeginOfRun()
{
   fNumCycles = 0;
   fFastSaved = 0.;
   fSlowSaved = 0.;
   fMaxTimeByFastClock = TMath::Power(2,fBits)/fFastFrequency;
}
void TScalerTimestampProcessor::Process() {
   fOutput->Clear("C");
   Double_t fast = (*fInput)->GetValue(fFastID);
   Double_t slow = (*fInput)->GetValue(fSlowID);
   if (fFastSaved > TMath::Limits<Double_t>::Epsilon() &&
      fast < fFastSaved) {
      // increment the number of cycle when loop over the counter
      fNumCycles++;
   }
   // add cycle when the time more than maximum time pasts
   if (fSlowSaved > TMath::Limits<Double_t>::Epsilon()) {
      fNumCycles += TMath::Floor((slow - fSlowSaved) / fSlowFrequency / fMaxTimeByFastClock);
   }
   fFastSaved = fast;
   fSlowSaved = slow;
   fOutput->SetValue(fast / fFastFrequency + fNumCycles * fMaxTimeByFastClock);
}
