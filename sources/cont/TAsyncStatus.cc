/* $Id:$ */
/**
 * @file   TAsyncStatus.cc
 * @date   Created : 2024/07/02 07:32:17
 * @author Shinsuke OTA <ota@rcnp.osaka-u.ac.jp
 *  
 *  
 *    Copyright (C)2024
 */

#include "TAsyncStatus.h"

using art::TAsyncStatus;

ClassImp(art::TAsyncStatus)
;

TAsyncStatus::TAsyncStatus()
  : fCount(0)
{
}

void TAsyncStatus::Update(double timing, bool onoff, uint64_t count) {
  SetTiming(timing);
  fStatus = onoff;
  if (count != -1) {
    fCount = count;
  } else {
    fCount++;
  }
}

