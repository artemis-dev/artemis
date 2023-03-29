/**
 * @file   TChargeValidateProcessor.cc
 * @brief  charge validation
 *
 * @date   Created       : 2014-06-25 17:10:00 JST
 *         Last Modified : 2015-05-12 15:10:13 JST (ota)
 * @author  <sharaq08@shg01.ribfdaq>
 *
 *    (C) 2014
 */

#include "TChargeValidateProcessor.h"
#include <TClonesArray.h>
#include <ICharge.h>
#include <TDataObject.h>
#include <TTimingChargeData.h>

using art::TChargeValidateProcessor;

ClassImp(TChargeValidateProcessor)

TChargeValidateProcessor::TChargeValidateProcessor()
: fInput(NULL), fOutput(NULL)
{
   FloatVec_t def(2,0.);
   RegisterInputCollection("InputCollection","name of input collection",fInputName,TString("input"),
                           &fInput,TClonesArray::Class_Name(),ICharge::Class_Name());
   RegisterOutputCollection("OutputCollection","name of input collection",fOutputName,TString("output"));
   RegisterProcessorParameter("Range","Charge range [min, max]",fRange,def);
   RegisterOptionalParameter("OutputInvalid","flag to output invalid data if non zero  (default : 0) ",fOutputInvalid,kFALSE);
   

}

TChargeValidateProcessor::~TChargeValidateProcessor()
{
}

TChargeValidateProcessor::TChargeValidateProcessor(const TChargeValidateProcessor& rhs)
{
}

TChargeValidateProcessor& TChargeValidateProcessor::operator=(const TChargeValidateProcessor& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}
void TChargeValidateProcessor::Init(TEventCollection *col)
{
   // checking the range validity
   if (fRange.size() != 2) {
      SetStateError(TString::Format("Invalid size of range : %d",(Int_t)fRange.size()));
      return;
   }

   if (fRange[0] > fRange[1]) {
      SetStateError(TString::Format("Invalid order of range : [0] %f should be less than  [1] %f",fRange[0],fRange[1]));
      return;
   }
   

   if (fOutput) delete fOutput;
   fOutput = new TClonesArray((*fInput)->GetClass());
   col->Add(fOutputName,fOutput,fOutputIsTransparent);
}

void TChargeValidateProcessor::Process()
{
   TClonesArray *input = *fInput;
   Int_t num = input->GetEntriesFast();
   fOutput->Clear("C");
   for (Int_t i=0; i!=num; i++) {
      TDataObject *obj = static_cast<TDataObject*>(input->At(i));
      Double_t charge = 0.;
      ICharge *cobj = NULL;
      if ((cobj = dynamic_cast<ICharge*>(obj)) == NULL) {
         Error("Process","Unexpected: dynamic_cast to ICharge failed");
         return;
      }
      charge = cobj->GetCharge();
      if (charge < fRange[0] || fRange[1] < charge) {
         if (!fOutputInvalid) continue;
         TDataObject *outobj = (TDataObject*)fOutput->ConstructedAt(fOutput->GetEntriesFast());
         obj->Copy(*outobj);
         outobj->Invalidate();
      } else  {
         TDataObject *outobj = (TDataObject*)fOutput->ConstructedAt(fOutput->GetEntriesFast());
         obj->Copy(*outobj);
         outobj->Validate();
      }
   }
   TTimingChargeData::SetSortOrder(TTimingChargeData::kDESC);
   TTimingChargeData::SetSortType(TTimingChargeData::kCharge);
   fOutput->Sort();

}
