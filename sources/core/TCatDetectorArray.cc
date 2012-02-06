/* $Id:$ */
/**
 * @file   TCatDetectorArray.cc
 * @date   Created : Feb 05, 2012 01:05:28 JST
 *   Last Modified : Feb 05, 2012 02:19:07 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TCatDetectorArray.h"

TCatDetectorArray::TCatDetectorArray()
   : fNumHits(0), fHits(NULL) 
{
}

TCatDetectorArray::TCatDetectorArray(const TCatDetectorArray &rhs) 
   : TCatObject(rhs) {
      fNumHits = rhs.fNumHits;
      fHits = (TClonesArray*) rhs.fHits->Clone();
}

TCatDetectorArray::~TCatDetectorArray()
{
   if (fHits) delete fHits;
}
