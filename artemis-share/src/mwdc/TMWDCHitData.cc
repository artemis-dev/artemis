/**
 * @file   TMWDCHitData.cc
 * @brief
 *
 * @date   Created:       2013-10-17 13:46:49
 *         Last Modified: 2014-01-08 13:32:48
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved.
 */

#include "TMWDCHitData.h"
#include "constant.h"

#include <iostream>
#include <TClass.h>

using art::TMWDCHitData;

ClassImp(art::TMWDCHitData);

// Default constructor
TMWDCHitData::TMWDCHitData() 
   : fDriftLength(kInvalidD) {
}

// Default destructor
TMWDCHitData::~TMWDCHitData() {
}

// Assignment operator
TMWDCHitData& TMWDCHitData::operator=(const TMWDCHitData& rhs) {
   if (this != &rhs) {
      ((TMWDCHitData&)rhs).Copy(*this);
   }
   return *this;
}


void TMWDCHitData::Clear(Option_t *opt) {
   TTimingChargeData::Clear(opt);
   fDriftLength = kInvalidD;
}

Int_t TMWDCHitData::Compare(const TObject *obj) const {
   using art::IsValid;

   const TMWDCHitData *data = (TMWDCHitData*) obj;
   Int_t ret = 0;
   if(!obj) {
      return -1;
   } else {
      switch (fSortType) {
	 case kDriftLength:
	    if (!IsValid(fDriftLength)) {
	       if (!IsValid(data->GetDriftLength())) {
		  return 0;
	       }
	       return 1;
	    } else if (!IsValid(data->GetDriftLength())) {
	       return -1;
	    }
	    if (fDriftLength > data->GetDriftLength()) {
	       ret = 1;
	    } else if (fDriftLength == data->GetDriftLength()) {
	       ret = 0;
	    } else {
	       ret = -1;
	    }
	    break;
	 default:
	    return TTimingChargeData::Compare(obj);
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

void TMWDCHitData::Print(Option_t*) const {
   std::cout << "OBJ: " << IsA()->GetName() << "\t"
	     << "WireID: " << GetID() << "\t"
	     << "Timing: " << GetTiming() << "\t"
	     << "Charge: " << GetCharge() << std::endl;
}
