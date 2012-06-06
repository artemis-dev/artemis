/* $Id:$ */
/**
 * @file   TCatObject.cc
 * @date   Created : Jan 31, 2012 11:31:50 JST
 *   Last Modified : Feb 11, 2012 18:05:17 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TCatObject.h"

ClassImp(TCatObject);

Int_t TCatObject::fSortType = TCatObject::kID;
Int_t TCatObject::fSortOrder = TCatObject::kDESC;

TCatObject::TCatObject()
   : fCategoryID(-1), fDetectorID(-1), fOptLevel(0) 
{
}
TCatObject::~TCatObject()
{
}

Int_t TCatObject::Compare(const TObject *obj) const
{
   TCatObject *catobj = (TCatObject*) obj;
   Int_t ret = -1;
   if (!obj) {
      ret = 1;
   } else if (fDetectorID > catobj->fDetectorID) {
      ret = 1;
   } else if (fDetectorID == catobj->fDetectorID) {
      ret = 0;
   }
   switch (fSortOrder) {
   case kASC:
      return ret;
      break;
   case kDESC:
      return -ret;
      break;
   default:
      return 0;
   }
}
