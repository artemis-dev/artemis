/**
 * @file   TTwoSidedPlasticData.h
 * @brief
 *
 * @date   Created:       2013-10-11 11:05:20
 *         Last Modified: 2015-04-09 14:24:00 JST (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved
 */

#ifndef TTWOSIDEDPLASTICDATA_H
#define TTWOSIDEDPLASTICDATA_H

#include <TRef.h>

#include "TTimingChargeData.h"
#include "constant.h"

namespace art{
   class TTwoSidedPlasticData;
   class TTimingChargeData;
}

class art::TTwoSidedPlasticData : public art::TTimingChargeData {
public:
   // Default constructor
   TTwoSidedPlasticData();
   virtual ~TTwoSidedPlasticData();
   // Copy constructor
   TTwoSidedPlasticData(const TTwoSidedPlasticData& rhs);
   // Assignment operator
   TTwoSidedPlasticData& operator=(const TTwoSidedPlasticData& rhs);

   virtual void Copy(TObject& dest) const;

   Double_t GetT1() const { return HasValidHit(kT1) ? GetUncheckedT1() : kInvalidD; };
   Double_t GetT2() const { return HasValidHit(kT2) ? GetUncheckedT2() : kInvalidD; };
   Double_t GetQ1() const { return HasValidHit(kQ1) ? GetUncheckedQ1() : kInvalidD; };
   Double_t GetQ2() const { return HasValidHit(kQ2) ? GetUncheckedQ2() : kInvalidD; };
   Double_t GetStoredT1() const { return fT1; };
   Double_t GetStoredT2() const { return fT2; };
   Double_t GetStoredQ1() const { return fQ1; };
   Double_t GetStoredQ2() const { return fQ2; };
   void SetT1(Double_t t) { fT1 = t; };
   void SetT2(Double_t t) { fT2 = t; };
   void SetQ1(Double_t q) { fQ1 = q; };
   void SetQ2(Double_t q) { fQ2 = q; };

   Double_t GetUncheckedT1() const { return GetSide1()->GetTiming(); };
   Double_t GetUncheckedT2() const { return GetSide2()->GetTiming(); };
   Double_t GetUncheckedQ1() const { return GetSide1()->GetCharge(); };
   Double_t GetUncheckedQ2() const { return GetSide2()->GetCharge(); };

   virtual void SetSide1(TTimingChargeData *side1) { fSide1 = side1; }
   virtual void SetSide2(TTimingChargeData *side2) { fSide2 = side2; }
   virtual const TTimingChargeData* GetSide1() const {
      return static_cast<TTimingChargeData*>(fSide1.GetObject());
   }
   virtual const TTimingChargeData* GetSide2() const {
      return static_cast<TTimingChargeData*>(fSide2.GetObject());
   }

   Double_t GetTAve() const {return GetTiming();}
   Double_t GetQAve() const {return GetCharge();}

   virtual void SetTDiff(Double_t val) {fTDiff = val;}
   Double_t GetTDiff() const {return fTDiff;}
   virtual void SetQDiff(Double_t val) {fQDiff = val;}
   Double_t GetQDiff() const {return fQDiff;}

   void SetTDiff1(Double_t d) { fTDiff1 = d; }
   void SetTDiff2(Double_t d) { fTDiff2 = d; }
   

   void  SetHitID(Int_t val) {SetAuxID(val);}
   Int_t GetHitID() const {return GetAuxID();}

   virtual void Clear(Option_t *opt);
//   virtual Int_t Compare(const TObject *obj) const; // inherits TTimingChargeData::Compare

   virtual Double_t GetX() const { return fX; }
   virtual Double_t GetY() const { return fY; }
   virtual Double_t GetZ() const { return fZ; }
   virtual void SetX(Double_t val) { fX = val; }
   virtual void SetY(Double_t val) { fY = val; }
   virtual void SetZ(Double_t val) { fZ = val; }

   virtual void SetPos(Double_t val) {fPos = val;}
   virtual Double_t GetPos() const {return fPos;}

   void InvalidateHit(UInt_t ich) { if (ich < kNumCh) ResetQualityBit(1 << ich); }
   void ValidateHit(UInt_t ich) { if (ich < kNumCh) SetQualityBit(1 << ich); }
   Bool_t HasValidHit(UInt_t ich) const { return ich < kNumCh ? TestQualityBit(1 << ich) : kFALSE; }

   enum EStatusBits { kT1 = 0, kQ1 = 1, kT2 = 2, kQ2 = 3 };

protected:
   Double_t fTDiff; //time difference between two sides
   Double_t fQDiff; //charge difference between two sides
   Double_t fT1;
   Double_t fT2;
   Double_t fQ1;
   Double_t fQ2;
   Double_t fTDiff1;
   Double_t fTDiff2;

   TRef fSide1; // timing charge data of 1st side
   TRef fSide2; // timing charge data of 2nd side

   Double_t fX;
   Double_t fY;
   Double_t fZ;
   Double_t fPos; //position calculated from time or charge information

   static const UInt_t kNumCh = 4; //!

   ClassDef(TTwoSidedPlasticData,4) // Two sided plastic data container
};

#endif // TTWOSIDEDPLASTICDATA_H
