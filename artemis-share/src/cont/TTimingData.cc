/**
 * @file   TTimingData.cc
 * @brief
 *
 * @date   Created:       2013-11-15 14:37:05
 *         Last Modified: 2014-01-28 13:43:19
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved.
 */

#include "TTimingData.h"
#include "constant.h"

#include <iostream>
#include <TClass.h>

using art::TTimingData;

ClassImp(art::TTimingData)

Int_t TTimingData::fSortType  = TTimingData::kID;
Int_t TTimingData::fSortOrder = TTimingData::kASC;

// Default constructor
TTimingData::TTimingData() 
   : fTiming(kInvalidD) {
   TDataObject::SetID(kInvalidI);
}

TTimingData::~TTimingData() {
}

// Copy constructor
TTimingData::TTimingData(const TTimingData& rhs) 
   : TDataObject(rhs), fTiming(rhs.fTiming) {
}

// Assignment operator
TTimingData& TTimingData::operator=(const TTimingData& rhs) {
   if (this != &rhs) {
      rhs.Copy(*this);
   }
   return *this;
}

void TTimingData::Clear(Option_t *opt) {
   TDataObject::Clear(opt);
   TDataObject::SetID(kInvalidI);
   this->SetTiming(kInvalidD);
}

Int_t TTimingData::Compare(const TObject *obj) const {
   using art::IsValid;

   TTimingData *data= (TTimingData*) obj;
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

void TTimingData::Print(Option_t*) const {
   std::cout << "OBJ: " << IsA()->GetName() << "\t"
	     << "ID: " << GetID() << "\t"
	     << "Timing: " << GetTiming() << std::endl;
}

