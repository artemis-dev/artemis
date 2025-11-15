/* $Id:$ */
/**
 * @file   TCatSimplestData.cc
 * @date   Created : Feb 11, 2012 14:11:06 JST
 *   Last Modified : Feb 11, 2012 18:07:36 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TCatSimplestData.h"

ClassImp(TCatSimplestData);

TCatSimplestData::TCatSimplestData()
{
}
TCatSimplestData::~TCatSimplestData()
{
}
TCatSimplestData::TCatSimplestData(const TCatSimplestData& rhs)
   : TObject(rhs)
{
   fTiming = rhs.fTiming;
   fCharge = rhs.fCharge;
}

Int_t TCatSimplestData::Compare(const TCatSimplestData *obj) const {

   TCatSimplestData *data = (TCatSimplestData*) obj;
   Int_t ret = -1;
   if (!obj) { 
      ret = 1;
   } else {
      switch (fSortType) {
      case kCharge:
	if (fCharge > data->GetCharge()) {
            ret = 1;
	} else if (fCharge == data->GetCharge()) {
	  ret = 0;
	} else {
	  ret = -1;
	}
         break;
      case kTime:
	if (fTiming > data->GetTiming()) {
	  ret = 1;
	} else if (fTiming == data->GetTiming()) {
            ret = 0;
         } else {
            ret = -1;
         }
         break;
      default:
         return TObject::Compare(obj);
         break;
      }
   }
   switch (fSortOrder) {
   case kASC:
      return ret;
   case kDESC:
      return -ret;
   }
    
   return 0;
}


Int_t TCatSimplestData::fSortType = TCatSimplestData::kID;
Int_t TCatSimplestData::fSortOrder = TCatSimplestData::kDESC;

