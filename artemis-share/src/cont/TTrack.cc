/**
 * @file   TTrack.cc
 * @brief
 *
 * @date   Created:       2013-10-17 14:42:11
 *         Last Modified: 2014-06-19 11:02:46 JST (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved.
 */

#include "TTrack.h"
#include "constant.h"

#include <iostream>
#include <TClass.h>
#include <TVector3.h>

using art::TTrack;

ClassImp(art::TTrack)

// Default constructor
TTrack::TTrack()
   : fX(kInvalidD), fY(kInvalidD), fZ(kInvalidD),
     fA(kInvalidD), fB(kInvalidD) {}

TTrack::TTrack(const TTrack& rhs)
   : TDataObject(rhs), fX(rhs.fX), fY(rhs.fY), fZ(rhs.fZ),
     fA(rhs.fA), fB(rhs.fB) {}

TTrack& TTrack::operator=(const TTrack& rhs)
{
   if (this != &rhs) {
      ((TTrack&)rhs).Copy(*this);
   }
   return *this;
}

TTrack::~TTrack() {}

void TTrack::GetPos(TVector3 *vec) const
{
   vec->SetXYZ(fX,fY,fZ);
}

void TTrack::CalculatePosAt(Double_t z, TVector3 *vec) const
{
   const Double_t dZ = z - fZ;
   vec->SetXYZ(fX + dZ*fA, fY + dZ*fB, z);
}

void TTrack::Print(Option_t*) const
{
   std::cout << "OBJ: " << IsA()->GetName() << "\t"
             << "(X,Y,Z): (" << GetX() << "," << GetY() << "," << GetZ() << ")" << "\t"
             << "(A,B): (" << GetA() << "," << GetB() << ")" << std::endl;
}

void TTrack::Clear(Option_t*)
{
   fX = fY = fZ = fA = fB = kInvalidD;
}
