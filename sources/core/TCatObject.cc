/* $Id:$ */
/**
 * @file   TCatObject.cc
 * @date   Created : Jan 31, 2012 11:31:50 JST
 *   Last Modified : Feb 11, 2012 17:28:34 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TCatObject.h"

ClassImp(TCatObject);

Int_t TCatObject::fSortType = TCatObject::kID;

TCatObject::TCatObject()
   : TNamed(), fCategoryID(-1), fDetectorID(-1), fOptLevel(0) 
{
}
TCatObject::~TCatObject()
{
}

Int_t TCatObject::Compare(const TObject *obj) const
{
   TCatObject *catobj = (TCatObject*) obj;
   if (!obj) return 1;
   if (fDetectorID > catobj->fDetectorID) return 1;
   if (fDetectorID == catobj->fDetectorID) return 0;
   return -1;
}
