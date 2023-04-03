/**
 * @file   TGenericData.cc
 * @brief
 *
 * @date   Created:       2014-01-28 13:40:23
 *         Last Modified: 2014-01-28 13:51:15
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2014 KAWASE Shoichiro All rights reserved.
 */

#include "TGenericData.h"

#include <TClass.h>

using art::TGenericData;

ClassImp(art::TGenericData)

Int_t TGenericData::fSortType  = TGenericData::kID;
Int_t TGenericData::fSortOrder = TGenericData::kASC;

TGenericData::~TGenericData() {}

Int_t TGenericData::Compare(const TObject *obj) const {
   using art::IsValid;

   TGenericData *data= (TGenericData*) obj;
   Int_t ret = 0;
   if(!obj) {
      return -1;
   } else {
      switch (fSortType) {
	 case kDatum:
	    if (!IsValid(this->Get())) {
	       if (!IsValid(data->Get())) {
		  return 0;
	       }
	       return 1;
	    } else if (!IsValid(data->Get())) {
	       return -1;
	    }
	    if (this->Get() > data->Get()) {
	       ret = 1;
	    } else if (this->Get() == data->Get()) {
	       ret = 0;
	    } else {
	       ret = -1;
	    }
	    break;
	 case kID:
	    if (!IsValid(this->GetID())) {
	       if (!IsValid(data->GetID())) {
		  return 0;
	       }
	       return 1;
	    } else if (!IsValid(data->GetID())) {
	       return -1;
	    }
	    if (this->GetID() > data->GetID()) {
	       ret = 1;
	    } else if (this->GetID() == data->GetID()) {
	       ret = 0;
	    } else {
	       ret = -1;
	    }
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
void TGenericData::Print(Option_t*) const {
   Printf("OBJ: %s\tID: %d\tDatum: %f\n",IsA()->GetName(),GetID(),Get());
}
