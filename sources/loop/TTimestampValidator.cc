/**
 * @file   TTimestampValidator.cc
 * @brief  validate timestamp by absolute value or relative to reference object
 *
 * @date   Created       : 2018-07-18 13:40:53 JST
 *         Last Modified : 2018-07-18 14:17:29 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2018 Shinsuke OTA
 */

#include "TTimestampValidator.h"
#include <TDataObject.h>
#include <TClonesArray.h>
#include <TClass.h>

using art::TTimestampValidator;

ClassImp(TTimestampValidator)

TTimestampValidator::TTimestampValidator()
: fInput(NULL), fReference(NULL), fOutput(NULL)
{
   RegisterInputCollection("InputName","name of input to be validate",fInputName,TString(""),
                           &fInput,TClonesArray::Class_Name(),TDataObject::Class_Name());
   RegisterInputCollection("ReferenceName","(optional) name of reference input",fReferenceName,TString(""));
   RegisterOutputCollection("OutputName","name of output after validate",fOutputName,TString(""));

   RegisterProcessorParameter("SearchWindow","pair of start and end of search window",fSearchWindow,DoubleVec_t(0));
}

TTimestampValidator::~TTimestampValidator()
{
   if (fOutput) delete fOutput;
}

TTimestampValidator::TTimestampValidator(const TTimestampValidator& rhs)
{
}

TTimestampValidator& TTimestampValidator::operator=(const TTimestampValidator& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}



void TTimestampValidator::Init(TEventCollection *col)
{
   if (!fReferenceName.IsNull()) {
      fReference = reinterpret_cast<TClonesArray**>(col->GetObjectRef(fReferenceName));
      if (!fReference) {
         SetStateError(TString::Format("No such input collection for reference: '%s'",fReferenceName.Data()));
         return;
      } else if (!(*fReference)->InheritsFrom(TClonesArray::Class_Name())) {
         SetStateError(TString::Format("Input collection for reference is not TClonesArray: '%s'",(*fReference)->IsA()->GetName()));
         return;
      } else if (!((TClonesArray*)(*fReference))->GetClass()->InheritsFrom(art::TDataObject::Class())) {
         SetStateError(TString::Format("Input collection for reference does not contain art::TDataObject"));
         return;
      }
   }

   if (fSearchWindow.size() != 2) {
      SetStateError(TString::Format("search window size is not two : %d",fSearchWindow.size()));
      return;
   }

   if (fSearchWindow[0] > fSearchWindow[1]) {
      Warning("Init","search window start and end are swaped");
      Double_t tmp = fSearchWindow[0];
      fSearchWindow[0] = fSearchWindow[1];
      fSearchWindow[1] = tmp;
   }
      

   if (fOutputName.IsNull()) {
      SetStateError(TString::Format("name of output collection should be set"));
      return;
   }

   fOutput = new TClonesArray((*fInput)->GetClass());
   col->Add(fOutputName,fOutput,fOutputIsTransparent);
}

void TTimestampValidator::Process()
{
   fOutput->Clear("C");
   Double_t start = fSearchWindow[0];
   Double_t end = fSearchWindow[1];
   for (Int_t ihit = 0, nhits = (*fInput)->GetEntriesFast(); ihit < nhits; ++ihit) {
      TDataObject *input = static_cast<TDataObject*>((*fInput)->UncheckedAt(ihit));
      Double_t timestamp = input->GetTimestamp();
      Bool_t isValid = kFALSE;
      if (fReference) {
         for (Int_t iref = 0, nrefs = (*fReference)->GetEntriesFast(); iref < nrefs; ++iref) {
            TDataObject* ref = static_cast<TDataObject*>((*fReference)->UncheckedAt(iref));
            Double_t refTimestamp = ref->GetTimestamp();
            if (timestamp - refTimestamp < start || timestamp - refTimestamp > end) continue;

            isValid = kTRUE;
            break;
         }
      } else {
         if (timestamp < start || end < timestamp) continue;
         isValid = kTRUE;
      }

      if (!isValid) continue;

      TDataObject *output = static_cast<TDataObject*>(fOutput->ConstructedAt(fOutput->GetEntriesFast()));
      input->Copy(*output);
   }
}
