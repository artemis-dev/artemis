/**
 * @file   TTimingTimestampAdapter.cc
 * @brief  adopt timing to timestamp (update object)
 *
 * @date   Created       : 2018-07-18 12:50:51 JST
 *         Last Modified : 2018-07-18 13:01:58 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2018 Shinsuke OTA
 */

#include "TTimingTimestampAdapter.h"
#include <TClass.h>
#include <TDataObject.h>
#include <ITiming.h>
#include <TClonesArray.h>

using art::TTimingTimestampAdapter;

ClassImp(TTimingTimestampAdapter)

TTimingTimestampAdapter::TTimingTimestampAdapter()
{
   RegisterInputCollection("InputName","name of input",fInputName,TString("input"),
                           &fInput,TClonesArray::Class_Name(),TDataObject::Class_Name());
}

TTimingTimestampAdapter::~TTimingTimestampAdapter()
{
}

TTimingTimestampAdapter::TTimingTimestampAdapter(const TTimingTimestampAdapter& rhs)
{
}

TTimingTimestampAdapter& TTimingTimestampAdapter::operator=(const TTimingTimestampAdapter& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}

void TTimingTimestampAdapter::Init(TEventCollection* /* col */)
{
   TClass *cls = (*fInput)->GetClass();
   if (!cls->InheritsFrom(ITiming::Class())) {
      SetStateError(TString::Format("Class does not inherits from ITiming : %s",cls->GetName()));
      return;
   }
}

void TTimingTimestampAdapter::Process()
{
   TClonesArray *input = *fInput;
   for (Int_t ihit = 0, nhits = (input->GetEntriesFast()); ihit < nhits; ++ihit) {
      TDataObject *data = dynamic_cast<TDataObject*> (input->UncheckedAt(ihit));
      ITiming *timedata = dynamic_cast<ITiming*> (input->UncheckedAt(ihit));
      data->SetTimestamp(timedata->GetTiming());
   }
}
