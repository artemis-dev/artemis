/**
 * @file   TTwoSidedPlasticData.cc
 * @brief
 *
 * @date   Created:       2013-10-11 11:05:53
 *         Last Modified: 2014-06-23 12:25:38 JST (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved.
 */

#include "TTwoSidedPlasticData.h"

using art::TTwoSidedPlasticData;

ClassImp(art::TTwoSidedPlasticData);

// Default constructor
TTwoSidedPlasticData::TTwoSidedPlasticData()
   : fTDiff(kInvalidD), fQDiff(kInvalidD),
     fSide1(NULL), fSide2(NULL),
     fX(kInvalidD), fY(kInvalidD), fZ(kInvalidD),
     fPos(kInvalidD)
{
   TTimingChargeData::SetID(kInvalidI);    // detID
   TTimingChargeData::SetAuxID(kInvalidI); // hitID
}

TTwoSidedPlasticData::~TTwoSidedPlasticData() {}

// Copy constructor
TTwoSidedPlasticData::TTwoSidedPlasticData(const TTwoSidedPlasticData& rhs)
   : TTimingChargeData(rhs),
     fTDiff(rhs.fCharge), fQDiff(rhs.fQDiff),
     fSide1(rhs.fSide1), fSide2(rhs.fSide2),
     fX(rhs.fX), fY(rhs.fY), fZ(rhs.fZ),
     fPos(rhs.fPos) {}

// Assignment operator
TTwoSidedPlasticData& TTwoSidedPlasticData::operator=(const TTwoSidedPlasticData& rhs) {
   if (this != &rhs) {
      ((TTwoSidedPlasticData&)rhs).Copy(*this);
   }
   return *this;
 }

void TTwoSidedPlasticData::Copy(TObject& dest) const {
   TTimingChargeData::Copy(dest);
   TTwoSidedPlasticData &cobj = *(TTwoSidedPlasticData*)&dest;
   cobj.fTDiff  = fTDiff;
   cobj.fQDiff  = fQDiff;
   cobj.fSide1  = fSide1;
   cobj.fSide2  = fSide2;
   cobj.SetX(this->GetX());
   cobj.SetY(this->GetY());
   cobj.SetZ(this->GetZ());
   cobj.SetPos(this->GetPos());
}

void TTwoSidedPlasticData::Clear(Option_t *opt="") {
   TTimingChargeData::Clear(opt);
   TTimingChargeData::SetID(kInvalidI);
   TTimingChargeData::SetAuxID(kInvalidI);
   fTDiff  = kInvalidD;
   fQDiff  = kInvalidD;
   fSide1 = NULL;
   fSide2 = NULL;
   this->SetX(kInvalidD);
   this->SetY(kInvalidD);
   this->SetZ(kInvalidD);
   this->SetPos(kInvalidD);
}
