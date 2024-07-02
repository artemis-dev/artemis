/* $Id:$ */
/**
 * @file   TUpdateAsyncStatusProcessor.cc
 * @date   Created : 2024/07/02 07:32:17
 * @author Shinsuke OTA <ota@rcnp.osaka-u.ac.jp
 *  
 *  
 *    Copyright (C)2024
 */

#include "TUpdateAsyncStatusProcessor.h"

using art::TUpdateAsyncStatusProcessor;

ClassImp(art::TUpdateAsyncStatusProcessor);


TUpdateAsyncStatusProcessor::TUpdateAsyncStatusProcessor()
{
  Register(fInputOn("OnStatusCollection","name of collection for status on (unit of time should be sec)","spillOnTiming"));
  // Register(fInputOff("OffStatusCollection","name of collection for status off","spillOffTiming"));
  // fInputOff->SetDoAuto(false);

  Register(fEventHeader("EventHeaderName","name of eventheader","eventheader"));
  Register(fTimestampUnit("TimestampUnit","timestamp unit in second",524288.));
  Register(fOutput("OutputCollection","name of output collection","spill"));
}


void TUpdateAsyncStatusProcessor::Process() {

  // do not clear output

  for (uint64_t i = 0, n = fInputOn->GetEntriesFast(); i < n; ++i) {
    auto data = dynamic_cast<TDataObject*>(fInputOn->UncheckedAt(i));
    auto timing = dynamic_cast<ITiming*>(fInputOn->UncheckedAt(i));
    double timestamp = fEventHeader->GetTimestamp() * fTimestampUnit + timing->GetTiming();
    printf("timestamp = %f\n",timestamp);
    // find existing status
    bool isFound = false;
    for (uint64_t j = 0, nj = fOutput->GetEntriesFast(); j < nj; ++j) {
      auto status = static_cast<TAsyncStatus*>(fOutput->UncheckedAt(j));
      if (status->GetID() != data->GetID()) continue;
      isFound = true;
status->Update(timestamp,true);
      break;
    }
    if (!isFound) {
      // insert new status
      auto status = static_cast<TAsyncStatus*>(fOutput->ConstructedAt(fOutput->GetEntries()));
      status->Update(timestamp,true);
    }
  }
}
