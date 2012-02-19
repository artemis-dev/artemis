/* $Id:$ */
/**
 * @file   TCatSimpleData.cc
 * @date   Created : Feb 11, 2012 14:11:06 JST
 *   Last Modified : Feb 11, 2012 18:07:36 JST
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
   Int_t ret = -1;
   if (!obj) { 
      ret = 1;
   } else {
      switch (fSortType) {
      case kCharge:
         if (fCharge > data->fCharge) {
            ret = 1;
         } else if (fCharge == data->fCharge) {
            ret = 0;
         } else {
            ret = -1;
         }
         break;
      case kTime:
         if (fTime > data->fTime) {
            ret = 1;
         } else if (fTime == data->fTime) {
            ret = 0;
         } else {
            ret = -1;
         }
         break;
      default:
         return TCatObject::Compare(obj);
         break;
      }
   }
   switch (fSortOrder) {
   case kASC:
      return ret;
   case kDESC:
      return -ret;
   }
}
