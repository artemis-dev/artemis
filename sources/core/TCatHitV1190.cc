/* $Id:$ */
/**
 * @file   TCatHitV1190.cc
 * @date   Created : Feb 04, 2012 18:04:05 JST
 *   Last Modified : Feb 04, 2012 23:47:31 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TCatHitV1190.h"

ClassImp(TCatHitV1190);

TCatHitV1190::TCatHitV1190()
   : fStatus(kInvalid) {
}
TCatHitV1190::~TCatHitV1190()
{
}

TCatHitV1190::TCatHitV1190(
   const Int_t &cid,   const Int_t &did,
   const Int_t &leading, const Int_t &trailing,
   const EStatus& status) 
   : fLeading(leading), fTrailing(trailing), fStatus(status)
{
   SetCategoryID(cid);
   SetDetectorID(did);
   if(IsValid()) {
      fWidth = fTrailing - fLeading;
   } else {
      fWidth = 0;
   }
}

