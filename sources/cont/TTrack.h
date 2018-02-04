/**
 * @file   TTrack.h
 * @brief
 *
 * @date   Created:       2013-10-17 14:20:09
 *         Last Modified: 2014-06-19 11:01:34 JST (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved
 */

#ifndef TTRACK_H
#define TTRACK_H

#include "TDataObject.h"

namespace art{
   class TTrack;
}

class TVector3;

class art::TTrack : public TDataObject {
public:
   // Default constructor
   TTrack();
   // copy constructor
   TTrack(const TTrack& rhs);
   // Default destructor
   virtual ~TTrack();

   TTrack& operator=(const TTrack& rhs);

   void SetX(Double_t val) { fX = val; }
   Double_t x(Double_t z = 0.) const { return GetX(z); }
   Double_t X(Double_t z = 0.) const { return GetX(z); }
   Double_t GetX(Double_t z = 0.) const { return fX + fA * (z - fZ); }

   void SetY(Double_t val) { fY = val; }
   Double_t y(Double_t z = 0.) const { return GetY(z); }
   Double_t Y(Double_t z = 0.) const { return GetY(z); }
   Double_t GetY(Double_t z = 0.) const { return fY + fB * (z - fZ); }

   void SetZ(Double_t val) { fZ = val; }
   Double_t z() const { return GetZ(); }
   Double_t Z() const { return GetZ(); }
   Double_t GetZ() const { return fZ; }

   void SetA(Double_t val) { fA = val; }
   Double_t a() const { return GetA(); }
   Double_t A() const { return GetA(); }
   Double_t GetA() const { return fA; }

   void SetB(Double_t val) { fB = val; }
   Double_t b() const { return GetB(); }
   Double_t B() const { return GetB(); }
   Double_t GetB() const { return fB; }

   void GetPos(TVector3 *vec) const;
   void SetPos(Double_t x, Double_t y, Double_t z = 0)
   {
      fX = x;
      fY = y;
      fZ = z;
   }

   void SetAngle(Double_t a, Double_t b) { fA = a; fB = b; }

   virtual void CalculatePosAt(Double_t z, TVector3 *vec) const;

   virtual void Copy(TObject &obj) const {
      TDataObject::Copy(obj);
      TTrack &cobj = (TTrack&)obj;
      cobj.fX = fX;
      cobj.fY = fY;
      cobj.fZ = fZ;
      cobj.fA = fA;
      cobj.fB = fB;
   }

   virtual void Print(Option_t* opt = "") const;
   virtual void Clear(Option_t* opt = "");

protected:
   Double_t fX;
   Double_t fY;
   Double_t fZ;
   Double_t fA;
   Double_t fB;

   ClassDef(TTrack,1) // container for track information
};

#endif // TTRACK_H
