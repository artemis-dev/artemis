/**
 * @file   TTimingData.cc
 * @brief
 *
 * @date   Created:       2013-11-15 14:37:05
 *         Last Modified: 2014-02-24 15:02:22
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved.
 */

#include "TChargeData.h"
#include "constant.h"

#include <iostream>

#include <TClass.h>

using art::TChargeData;

ClassImp(art::TChargeData)

Int_t TChargeData::fSortType  = TChargeData::kID;
Int_t TChargeData::fSortOrder = TChargeData::kASC;

// Default constructor
TChargeData::TChargeData() 
   : fCharge(kInvalidD) {
   this->SetID(kInvalidI);
}

TChargeData::~TChargeData() {
}

// Copy constructor
TChargeData::TChargeData(const TChargeData& rhs) 
   : TDataObject(rhs), fCharge(rhs.fCharge) {
}

// Assignment operator
TChargeData& TChargeData::operator=(const TChargeData& rhs) {
   if (this != &rhs) {
      rhs.Copy(*this);
   }
   return *this;
}

void TChargeData::Clear(Option_t*) {
   TDataObject::Clear();
   this->SetCharge(kInvalidD);
   this->SetID(kInvalidI);
}

Int_t TChargeData::Compare(const TObject *obj) const {
   using art::IsValid;

   TChargeData *data= (TChargeData*) obj;
   Int_t ret = 0;
   if(!obj) {
      return -1;
   } else {
      switch (fSortType) {
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

void TChargeData::Print(Option_t*) const {
   std::cout << "OBJ: " << IsA()->GetName() << "\t"
	     << "ID: " << GetID() << "\t"
	     << "Charge: " << GetCharge() << std::endl;
}
