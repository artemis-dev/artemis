/* $Id:$ */
/**
 * @file   TCatSimpleData.cc
 * @date   Created : Feb 11, 2012 14:11:06 JST
 *   Last Modified : Feb 11, 2012 17:29:50 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TCatSimpleData.h"

ClassImp(TCatSimpleData);

TCatSimpleData::TCatSimpleData()
{
}
TCatSimpleData::~TCatSimpleData()
{
}

Int_t TCatSimpleData::Compare(const TObject *obj) const
{
   TCatSimpleData *data = (TCatSimpleData*) obj;
   if (!obj) return 1;
   switch (fSortType) {
   case kCharge:
      if (fCharge > data->fCharge) return 1;
      if (fCharge == data->fCharge) return 0;
      return -1;
      break;
   case kTime:
      if (fTime > data->fTime) return 1;
      if (fTime == data->fTime) return 0;
      return -1;
      break;
   default:
      return TCatObject::Compare(obj);
      break;
   }
   return -1;
}
