/* $Id:$ */
/**
 * @file   TBPMTrackDisplayProcessor.cc
 * @date   Created : 2024/07/02 07:32:17
 * @author Shinsuke OTA <ota@rcnp.osaka-u.ac.jp
 *  
 *  
 *    Copyright (C)2024
 */

#ifndef TASYNCSTATUS_H
#define TASYNCSTATUS_H

#include "TDataObject.h"
#include "ITiming.h"

namespace art {
  class TAsyncStatus;
}

class art::TAsyncStatus : public TDataObject, public ITiming
{
public:
  TAsyncStatus();
  virtual ~TAsyncStatus() = default;

  
  virtual uint64_t GetCount() const { return fCount; }
  virtual bool GetStatus() const { return fStatus; }
  virtual void Update(double timing, bool onoff, uint64_t count = -1);
  virtual Double_t GetTiming() const { return fTiming; }
  virtual void SetTiming(double timing) { fTiming = timing; }
  virtual void SetTiming(const ITiming& timing) { SetTiming(timing.GetTiming()); }

protected:
  uint64_t fCount;
  bool     fStatus;
  double   fTiming;

  ClassDef(TAsyncStatus,1); // asynchronous status 
};
  

#endif // TASYNCSTATUS_H


