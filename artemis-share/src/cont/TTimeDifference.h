/**
 * @file   TTimeDifference.h
 * @brief
 *
 * @date   Created:       2013-11-11 10:11:52
 *         Last Modified: 2014-02-24 14:42:54
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved
 */

/* 
 * This class holds time difference information from det0 to det (and charge of det).
 * fHitID0, fHitID ... order of hit in multihit.
 */

#ifndef TTIMEDIFFERENCE_H
#define TTIMEDIFFERENCE_H

#include "ITiming.h"
#include "TDataObject.h"

namespace art{
   class TTimeDifference;
}


class art::TTimeDifference : public TDataObject, public ITiming {
public:
   // Default constructor
   TTimeDifference();
   // constructor from two ITimings. IDs should be set manually.
   TTimeDifference(const ITiming &time, const ITiming &time0);
   // destructor
   virtual ~TTimeDifference();
   // Copy constructor
   TTimeDifference(const TTimeDifference& rhs);
   // Assignment operator
   TTimeDifference& operator=(const TTimeDifference& rhs);

   virtual void Copy(TObject& dest) const {
      TDataObject::Copy(dest);
      TTimeDifference &cobj = *(TTimeDifference*)&dest;
      cobj.SetHitID0(this->GetHitID0());
      cobj.SetHitID(this->GetHitID());
      cobj.SetTiming(this->GetTiming());
   }

   virtual void Clear(Option_t *opt="");

   virtual Int_t GetHitID0() const {return fHitID0;}
   virtual void SetHitID0(Int_t val){fHitID0 = val;}

   virtual Int_t GetHitID() const {return fHitID;}
   virtual void SetHitID(Int_t val){fHitID = val;}  

   virtual Int_t GetDetID() const {return GetID();}
   virtual void SetDetID(Int_t val){ SetID(val);}

   virtual Int_t GetDetID0() const {return GetAuxID();}
   virtual void SetDetID0(Int_t val){ SetAuxID(val);}

   virtual Double_t GetTiming() const {return fTiming;}
   virtual void SetTiming(Double_t val) {fTiming = val;}
   virtual void SetTiming(const ITiming& obj) {SetTiming(obj.GetTiming());}

   virtual void TakeDifference(const ITiming* time, const ITiming* time0) {
      fTiming = *time - *time0;
   }

   static void SetSortType(Int_t type){fSortType = type;}
   static void SetSortOrder(Int_t order) {fSortOrder = order;}

   virtual Int_t Compare(const TObject *obj) const;

   virtual void Print(Option_t*) const;

   typedef enum {kID,kTiming} ETimeDifferenceSortType; //!
   typedef enum {kASC, kDESC} ESortOrder; //!

protected:
   Int_t fHitID0;
   Int_t fHitID;
   Double_t fTiming;
   static Int_t fSortType; //!
   static Int_t fSortOrder; //!

   ClassDef(TTimeDifference,1) // container for time difference information
};

#endif // TTIMEDIFFERENCE_H
