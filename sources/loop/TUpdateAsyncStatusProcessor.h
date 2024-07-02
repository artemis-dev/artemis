/* $Id:$ */
/**
 * @file   TAsyncStatus.cc
 * @date   Created : 2024/07/02 07:32:17
 * @author Shinsuke OTA <ota@rcnp.osaka-u.ac.jp
 *  
 *  
 *    Copyright (C)2024
 */

#ifndef TUPDATEASYNCSTATUSPROCESOR_H
#define TUPDATEASYNCSTATUSPROCESOR_H

#include "TProcessor.h"
#include "ITiming.h"
#include "TAsyncStatus.h"
#include "TEventHeader.h"

namespace art {
  class TUpdateAsyncStatusProcessor;
}


class art::TUpdateAsyncStatusProcessor : public TProcessor {
 public:
  TUpdateAsyncStatusProcessor();
  virtual ~TUpdateAsyncStatusProcessor() = default;

  virtual void Process();
protected:
  InputData<TClonesArray,ITiming> fInputOn;
  //  InputData<TClonesArray,ITiming> fInputOff;
  InputData<TEventHeader> fEventHeader;
  Parameter<double> fTimestampUnit;
  OutputData<TClonesArray,TAsyncStatus> fOutput;
  

  ClassDef(TUpdateAsyncStatusProcessor,1) ; // update asynchronous status data

};

#endif // TUPDATEASYNCSTATUSPROCESOR_H
