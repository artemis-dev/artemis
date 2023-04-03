/**
 * @file   TWalkCorrectionProcessor.cc
 * @brief  walk correction processor for TTimingChargeData
 *
 * @date   Created       : 2014-06-17 02:57:39 JST
 *         Last Modified : Jun 19, 2014 15:05:44 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 Shinsuke OTA
 */

#include "TWalkCorrectionProcessor.h"
#include <TClonesArray.h>
#include <TTimingChargeData.h>
#include <TConverterBase.h>
using art::TWalkCorrectionProcessor;

ClassImp(TWalkCorrectionProcessor)

TWalkCorrectionProcessor::TWalkCorrectionProcessor()
{
   RegisterInputCollection("InputCollection","name of input collection",fInputColName,TString("input"),
                           &fInput,TClonesArray::Class_Name(),TTimingChargeData::Class_Name());
   RegisterOutputCollection("OutputCollection","name of output collection",fOutputColName,TString("output"),
                            &fOutput,TClonesArray::Class_Name(),TTimingChargeData::Class_Name());
   RegisterInputInfo("WalkConverterArray","name of converter array",fConverterName,TString("conv:walk"),
                     &fConverter,TClonesArray::Class_Name(),TConverterBase::Class_Name());

}

TWalkCorrectionProcessor::~TWalkCorrectionProcessor()
{
}

TWalkCorrectionProcessor::TWalkCorrectionProcessor(const TWalkCorrectionProcessor& rhs)
{
}

TWalkCorrectionProcessor& TWalkCorrectionProcessor::operator=(const TWalkCorrectionProcessor& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}

void TWalkCorrectionProcessor::Process()
{
   fOutput->Clear("C");
   Int_t nHit = (*fInput)->GetEntriesFast();
   Int_t nConv = (*fConverter)->GetEntriesFast();
   for (Int_t iHit = 0; iHit != nHit; iHit++) {
      TTimingChargeData *data = (TTimingChargeData*) (*fInput)->At(iHit);
      if (data->GetID() >= nConv) continue;
      TTimingChargeData *out = (TTimingChargeData*) fOutput->ConstructedAt(fOutput->GetEntriesFast());
      TConverterBase *converter = (TConverterBase*) (*fConverter)->At(data->GetID());
      data->Copy(*out);
      out->SetTiming(out->GetTiming()-converter->Convert(out->GetCharge()));
   }
}
