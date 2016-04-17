/* $Id:$ */
/**
 * @file   TCatPulseShape.cc
 * @date   Created : Mar 10, 2013 23:10:50 JST
 *   Last Modified : 2016-04-17 11:50:41 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#include "TCatPulseShape.h"

using art::TCatPulseShape;

ClassImp(TCatPulseShape);

TCatPulseShape::ESortType TCatPulseShape::fgSortType = TCatPulseShape::kID;
Int_t TCatPulseShape::fgSortOrder = TCatPulseShape::kTiming;
TCatPulseShape::TCatPulseShape()
   : fCh(-1),fGeo(-1),fTime(0.),fCharge(0.),fRiseTime(0.),fOffset(0.),fNumSample(0)
#ifndef VEC
   , fSample(NULL), fClock(NULL)
#endif 
{
#ifndef VEC   
   fSample = new Float_t[kMaxSample];
   fClock = new Float_t[kMaxSample];
#endif
   
   
}
TCatPulseShape::~TCatPulseShape()
{
#ifndef VEC      
   if (fSample) delete [] fSample;
   if (fClock) delete[] fClock;
   fSample = fClock = NULL;
#endif
}

void TCatPulseShape::Copy(TObject &obj) const
{
   TCatPulseShape &pulse = (TCatPulseShape&)obj;
   pulse.fCh = fCh;
   pulse.fGeo = fGeo;
   pulse.fTime = fTime;
   pulse.fCharge = fCharge;
   pulse.fOffset = fOffset;
   pulse.fNumSample = fNumSample;
   pulse.fRiseTime = fRiseTime;
   fPos.Copy(pulse.fPos);
   TDataObject::Copy(obj);
}

void TCatPulseShape::Clear(Option_t *option)
{
   fNumSample = 0;
   fTime = fCharge = fOffset = 0.;
   fPos.SetXYZ(0.,0.,0.);
   TDataObject::Clear(option);
#ifdef VEC
   fSample.clear();
   fClock.clear();
#endif
}

Int_t TCatPulseShape::Compare (const TObject *obj) const
{
   Int_t ret = 1;
   const TCatPulseShape *pulse = (const TCatPulseShape*)obj;
   Double_t value = 0;
   Double_t myValue = 0;
   switch (fgSortType) {
   case kTiming:
      value = pulse->fOffset;
      myValue = fOffset;
      break;
   case kCharge:
      value = pulse->fCharge;
      myValue = fCharge;
      break;
   case kID:
      value = pulse->GetID();
      myValue = GetID();
      break;
   }
   if (IsValid()) {
      if (!pulse->IsValid()) {
         ret = -1;
      } else if (value > myValue) {
         ret = -1;
      } else if (value == myValue) {
         ret = 0;
      } else if (value < myValue) {
         ret = 1;
      }
   } else if (pulse->IsValid()) {
      ret = 1;
   } else {
      ret = 0;
   }
   return ret * fgSortOrder;
}
