/**
 * @file   TTimeDifference.cc
 * @brief
 *
 * @date   Created:       2013-11-11 10:13:23
 *         Last Modified: 2014-01-08 12:49:36
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved.
 */

#include "TTimeDifference.h"

#include "constant.h"

#include <iostream>
#include <TClass.h>

using art::TTimeDifference;

ClassImp(art::TTimeDifference);

Int_t TTimeDifference::fSortType  = TTimeDifference::kID;
Int_t TTimeDifference::fSortOrder = TTimeDifference::kASC;

// Default constructor
TTimeDifference::TTimeDifference() 
   : fHitID0(kInvalidI), fHitID(kInvalidI), fTiming(kInvalidD) {
   this->SetDetID(kInvalidI);
   this->SetDetID0(kInvalidI);
}

TTimeDifference::TTimeDifference(const ITiming &time, const ITiming &time0)
   : fHitID0(kInvalidI), fHitID(kInvalidI), fTiming(time.GetTiming() - time0.GetTiming()) {
   this->SetDetID(kInvalidI);
   this->SetDetID0(kInvalidI);
}

TTimeDifference::~TTimeDifference() {
}

// Copy constructor
TTimeDifference::TTimeDifference(const TTimeDifference& rhs) 
   : TDataObject(rhs), fHitID0(rhs.fHitID0), fHitID(rhs.fHitID), fTiming(rhs.fTiming) {
}

// Assignment operator
TTimeDifference& TTimeDifference::operator=(const TTimeDifference& rhs) {
   if (this != &rhs) {
      ((TTimeDifference&)rhs).Copy(*this);
   }
   return *this;
}

void TTimeDifference::Clear(Option_t *opt) {
   TDataObject::Clear(opt);
   this->SetHitID0(kInvalidI);
   this->SetHitID(kInvalidI);
   this->SetDetID0(kInvalidI);
   this->SetDetID(kInvalidI);
   this->SetTiming(kInvalidD);
}

Int_t TTimeDifference::Compare(const TObject *obj) const {
   using art::IsValid;

   TTimeDifference *data= (TTimeDifference*) obj;
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
	    if (!IsValid(this->GetDetID())) {
	       if (!IsValid(data->GetDetID())) {
		  return 0;
	       }
	       return 1;
	    } else if (!IsValid(data->GetDetID())) {
	       return -1;
	    }
	    if (this->GetDetID() > data->GetDetID()) {
	       ret = 1;
	    } else if (this->GetDetID() == data->GetDetID()) {
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

void TTimeDifference::Print(Option_t*) const {
   std::cout << "OBJ: " << IsA()->GetName() << "\t"
	     << "DetID(HitID): " << GetDetID() << "(" << GetHitID() << ")\t"
	     << "DetID0(HitID0): " << GetDetID0() << "(" << GetHitID0() << ")\t"
	     << "TimeDifference: " << GetTiming() << std::endl;
}
