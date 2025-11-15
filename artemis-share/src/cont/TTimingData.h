/**
 * @file   TTimingData.h
 * @brief
 *
 * @date   Created:       2013-11-15 14:09:04
 *         Last Modified: 2014-02-24 14:40:02
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved
 */

#ifndef TTIMINGDATA_H
#define TTIMINGDATA_H

#include "TDataObject.h"
#include "ITiming.h"

namespace art {
   class TTimingData;
}


class art::TTimingData :public TDataObject, public ITiming {
public:
   // Default constructor
   TTimingData();
   // destructor
   virtual ~TTimingData();
   // Copy constructor
   TTimingData(const TTimingData& rhs);
   // Assignment operator
   TTimingData& operator=(const TTimingData& rhs);
   using ITiming::operator=;

   static void SetSortOrder(Int_t order) {fSortOrder = order;}
   static void SetSortType(Int_t type){fSortType = type;}

   virtual Bool_t IsSortable() const {return kTRUE;}

   virtual Double_t GetTiming() const { return fTiming;}
   virtual void SetTiming(Double_t val) { fTiming = val; }
   virtual void SetTiming(const ITiming &obj) { SetTiming(obj.GetTiming()); }

   // obsolete. Use TDataObject::SetID().
   virtual void SetDetID(Int_t val) { SetID(val); }
   // obsolete. Use TDataObject::GetID().
   virtual Int_t GetDetID() const {return GetID();}

   virtual void Clear(Option_t *opt="");

   virtual void Copy(TObject& dest) const {
      TDataObject::Copy(dest);
      TTimingData &cobj = *(TTimingData*)&dest;
      cobj.SetTiming(GetTiming());
   }

   virtual Int_t Compare(const TObject *obj) const;

   virtual void Print(Option_t*) const;

   typedef enum {kID, kTiming} ETimingSortType; //!
   typedef enum {kASC, kDESC} ESortOrder; //!
   
protected:
   Double_t fTiming;
   static Int_t fSortType; //!
   static Int_t fSortOrder; //!

   ClassDef(TTimingData,1) // container for timing information
};

#endif // TTIMINGDATA_H
