/**
 * @file   THeaderTimestampProcessor.cc
 * @brief  generate timestamp from header
 *
 * @date   Created       : 2018-06-25 21:09:50 JST
 *         Last Modified : 2018-06-25 21:16:38 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2018 Shinsuke OTA
 */

#include "THeaderTimestampProcessor.h"
#include <TEventHeader.h>
#include <TSimpleData.h>
#include <TMath.h>

using art::THeaderTimestampProcessor;

ClassImp(THeaderTimestampProcessor)

THeaderTimestampProcessor::THeaderTimestampProcessor()
: fFirstTimestamp(-1)
{
   RegisterInputCollection("Input","name of headerr input",fInputName,TString("eventheader"),
                           &fInput,TEventHeader::Class_Name());
   RegisterOutputCollection("Output","name of output timestamp",fOutputName,TString("timestamp"),
                            &fOutput,TSimpleData::Class_Name());
   RegisterProcessorParameter("Frequency","frequency",fFrequency,0.);

}

THeaderTimestampProcessor::~THeaderTimestampProcessor()
{
}

THeaderTimestampProcessor::THeaderTimestampProcessor(const THeaderTimestampProcessor& rhs)
{
}

THeaderTimestampProcessor& THeaderTimestampProcessor::operator=(const THeaderTimestampProcessor& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}

void THeaderTimestampProcessor::Init(TEventCollection *col)
{
   const Double_t eps = TMath::Limits<Double_t>::Epsilon();
   if (fFrequency < eps) {
      SetStateError(TString::Format("Bad frequency '%f'",fFrequency));
      return;
   }
}

void THeaderTimestampProcessor::Process()
{
   fOutput->Clear("C");
   Long64_t timestamp = (*fInput)->GetTimestamp();
   if (fFirstTimestamp == -1) {
      fFirstTimestamp = timestamp;
   }
   fOutput->SetValue((timestamp - fFirstTimestamp) / fFrequency);
}
