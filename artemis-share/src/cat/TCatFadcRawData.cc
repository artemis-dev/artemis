/* $Id:$ */
/**
 * @file   TCatFadcRawData.cc
 * @date   Created : Feb 09, 2013 03:09:29 JST
 *   Last Modified : Feb 11, 2013 14:12:37 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#include "TCatFadcRawData.h"

ClassImp(TCatFadcRawData);

TCatFadcRawData::TCatFadcRawData()
   : fNumSample(0), fADC(NULL), fClock(NULL)
{
   fADC = new UShort_t[kMaxSample];
   fClock = new UShort_t[kMaxSample];
}

TCatFadcRawData::~TCatFadcRawData()
{
   if (fADC) {
      delete fADC;
      fADC = 0;
   }
   if (fClock) {
      delete fClock;
   }
}

void TCatFadcRawData::Clear(Option_t  *)
{
   fNumSample = 0;
}

