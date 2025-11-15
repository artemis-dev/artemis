/**
 * @file   TTimingChargeData.cc
 * @brief
 *
 * @date   Created:       2013-08-14 10:34:09
 *         Last Modified: 2014-01-08 12:56:45
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved.
 */

#include "TTimingChargeData.h"
#include "constant.h"

#include <iostream>
#include <TClass.h>

using art::TTimingChargeData;

ClassImp(art::TTimingChargeData)

Int_t TTimingChargeData::fSortType  = TTimingChargeData::kID;
Int_t TTimingChargeData::fSortOrder = TTimingChargeData::kDESC;

// Default constructor
TTimingChargeData::TTimingChargeData() 
   : fTiming(kInvalidD), fCharge(kInvalidD) {
   TDataObject::SetID(kInvalidI);
}

TTimingChargeData::~TTimingChargeData() {
}

// Copy constructor
TTimingChargeData::TTimingChargeData(const TTimingChargeData& rhs)
   : TDataObject(rhs), fTiming(rhs.fTiming), fCharge(rhs.fCharge) {
}

// Assignment operator
TTimingChargeData& TTimingChargeData::operator=(const TTimingChargeData& rhs) {
   if (this != &rhs) {
      ((TTimingChargeData&)rhs).Copy(*this);
   }
   return *this;
}

void TTimingChargeData::Clear(Option_t *opt) {
   TDataObject::Clear(opt);
   TDataObject::SetID(kInvalidI);
   this->SetTiming(kInvalidD);
   this->SetCharge(kInvalidD);
}

Int_t TTimingChargeData::Compare(const TObject *obj) const{
   using art::IsValid;

   TTimingChargeData *data= (TTimingChargeData*) obj;
   Int_t ret = 0;
   if(!obj) {
      return -1;
   } else {
      switch (fSortType) {
	 case kTiming:
	    if (!IsValid(this->GetTiming())) {
	       if (!IsValid(data->GetTiming())) {
		  return 0;
	       }
	       return 1;
	    } else if (!IsValid(data->GetTiming())) {
	       return -1;
	    }
	    if (this->GetTiming() > data->GetTiming()) {
	       ret = 1;
	    } else if (this->GetTiming() == data->GetTiming()) {
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
	 case kCharge:
	    if (!IsValid(this->GetCharge())) {
	       if (!IsValid(data->GetCharge())) {
		  return 0;
	       }
	       return 1;
	    } else if (!IsValid(data->GetCharge())) {
	       return -1;
	    }
	    if (this->GetCharge() > data->GetCharge()) {
	       ret = 1;
	    } else if (this->GetCharge() == data->GetCharge()) {
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

void TTimingChargeData::Print(Option_t*) const {
   std::cout << "OBJ: " << IsA()->GetName() << "\t"
	     << "ID: " << GetID() << "\t"
	     << "Timing: " << GetTiming() << "\t"
	     << "Charge: " << GetCharge() << std::endl;
}
