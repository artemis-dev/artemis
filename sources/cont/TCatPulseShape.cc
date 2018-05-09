/* $Id:$ */
/**
 * @file   TCatPulseShape.cc
 * @date   Created : Mar 10, 2013 23:10:50 JST
 *   Last Modified : 2018-03-18 23:35:14 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#include "TCatPulseShape.h"
#include <TBuffer.h>

using art::TCatPulseShape;
using std::vector;

ClassImp(TCatPulseShape)

TCatPulseShape::ESortType TCatPulseShape::fgSortType = TCatPulseShape::kID;
Int_t TCatPulseShape::fgSortOrder = TCatPulseShape::kTiming;
TCatPulseShape::TCatPulseShape()
   : fCh(-1),fGeo(-1),fTime(0.),fCharge(0.),fRiseTime(0.),fOffset(0.),fNumSample(0),
     fPos(0.,0.,0.),fSample(std::vector<Float_t>()), fClock(std::vector<Float_t>()),
     fMaxSample(TMath::QuietNaN()), fMaxSampleOffset(TMath::QuietNaN()),
     fBaseline(TMath::QuietNaN()), fBaselineRMS(TMath::QuietNaN()),
     fLeadingEdgeOffset(TMath::QuietNaN()), fNumMoment(0),
     fMoment(std::vector<Double_t>())
{
   
   
}
TCatPulseShape::~TCatPulseShape()
{
}

void TCatPulseShape::Copy(TObject &obj) const
{
   TCatPulseShape &pulse = (TCatPulseShape&)obj;
   pulse.fCh = fCh;
   pulse.fGeo = fGeo;
   pulse.fTime = fTime;
   pulse.fCharge = fCharge;
   pulse.fRiseTime = fRiseTime;
   pulse.fOffset = fOffset;
   pulse.fNumSample = fNumSample;
   pulse.fMaxSample = fMaxSample;
   pulse.fMaxSampleOffset = fMaxSampleOffset;
   pulse.fBaseline = fBaseline;
   pulse.fBaselineRMS = fBaselineRMS;
   pulse.fLeadingEdgeOffset = fLeadingEdgeOffset;
   pulse.fRiseTime = fRiseTime;
   fPos.Copy(pulse.fPos);
   pulse.fPos = fPos;
   TDataObject::Copy(obj);
}

void TCatPulseShape::Clear(Option_t *option)
{
   fNumSample = 0;
   fTime = fCharge = fOffset = 0.;
   fPos.SetXYZ(0.,0.,0.);
   TDataObject::Clear(option);
   fSample.clear();
   fClock.clear();
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


void TCatPulseShape::Streamer(TBuffer &R__b)
{
   // Stream an object of class art::TCatPulseShape.

   //This works around a msvc bug and should be harmless on other platforms
   typedef ::art::TCatPulseShape thisClass;
   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      //This works around a msvc bug and should be harmless on other platforms
      typedef art::TDataObject baseClass0;
      baseClass0::Streamer(R__b);
      //This works around a msvc bug and should be harmless on other platforms
      typedef art::ICharge baseClass1;
      baseClass1::Streamer(R__b);
      R__b >> fCh;
      R__b >> fGeo;
      R__b >> fTime;
      R__b >> fCharge;
      R__b >> fRiseTime;
      R__b >> fOffset;
      R__b >> fNumSample;
      fPos.Streamer(R__b);
      {
         vector<Float_t> &R__stl =  fSample;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            float R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }
      {
         vector<Float_t> &R__stl =  fClock;
         R__stl.clear();
         int R__i, R__n;
         R__b >> R__n;
         R__stl.reserve(R__n);
         for (R__i = 0; R__i < R__n; R__i++) {
            float R__t;
            R__b >> R__t;
            R__stl.push_back(R__t);
         }
      }

      // reading for version 2
      if (R__v > 2) {
         R__b >> fMaxSample;
         R__b >> fMaxSampleOffset;
         R__b >> fBaseline;
         R__b >> fBaselineRMS;
         R__b >> fLeadingEdgeOffset;
         R__b >> fNumMoment;
         {
            vector<Double_t> &R__stl =  fMoment;
            R__stl.clear();
            int R__i, R__n;
            R__b >> R__n;
            R__stl.reserve(R__n);
            for (R__i = 0; R__i < R__n; R__i++) {
               float R__t;
               R__b >> R__t;
               R__stl.push_back(R__t);
            }
         }
      }
      
      R__b.CheckByteCount(R__s, R__c, thisClass::IsA());
   } else {
      R__c = R__b.WriteVersion(thisClass::IsA(), kTRUE);
      //This works around a msvc bug and should be harmless on other platforms
      typedef art::TDataObject baseClass0;
      baseClass0::Streamer(R__b);
      //This works around a msvc bug and should be harmless on other platforms
      typedef art::ICharge baseClass1;
      baseClass1::Streamer(R__b);
      R__b << fCh;
      R__b << fGeo;
      R__b << fTime;
      R__b << fCharge;
      R__b << fRiseTime;
      R__b << fOffset;
      R__b << fNumSample;
      fPos.Streamer(R__b);
      {
         vector<Float_t> &R__stl =  fSample;
         int R__n=(true) ? int(R__stl.size()) : 0;
         R__b << R__n;
         if(R__n) {
            vector<Float_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         vector<Float_t> &R__stl =  fClock;
         int R__n=(true) ? int(R__stl.size()) : 0;
         R__b << R__n;
         if(R__n) {
            vector<Float_t>::iterator R__k;
            for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
            R__b << (*R__k);
            }
         }
      }
      {
         // for version 2
         R__b << fMaxSample;
         R__b << fMaxSampleOffset;
         R__b << fBaseline;
         R__b << fBaselineRMS;
         R__b << fLeadingEdgeOffset;
         R__b << fNumMoment;
         {
            vector<Double_t> &R__stl =  fMoment;
            int R__n=(true) ? int(R__stl.size()) : 0;
            R__b << R__n;
            if(R__n) {
               vector<Double_t>::iterator R__k;
               for (R__k = R__stl.begin(); R__k != R__stl.end(); ++R__k) {
                  R__b << (*R__k);
               }
            }
         }
      }
      R__b.SetByteCount(R__c, kTRUE);
   }
}

