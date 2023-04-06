/**
 * @file   TGenericData.h
 * @brief
 *
 * @date   Created:       2014-01-28 13:26:27
 *         Last Modified: 2014-02-24 14:35:59
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2014 KAWASE Shoichiro All rights reserved
 */

#ifndef TGENERICDATA_H
#define TGENERICDATA_H

#include "TDataObject.h"
#include "constant.h"

namespace art {
   class TGenericData;
}


class art::TGenericData : public TDataObject {
public:
   // Default constructor
   TGenericData() : fDatum(kInvalidD) {
      TDataObject::SetID(kInvalidI);
   }
   virtual ~TGenericData();
   // Copy constructor
   TGenericData(const TGenericData& rhs) 
      : TDataObject(rhs), fDatum(rhs.fDatum) {}
   // Assignment operator
   TGenericData& operator=(const TGenericData& rhs) {
      if (this != &rhs) {
	 rhs.Copy(*this);
      }
      return *this;
   }

   static void SetSortOrder(Int_t order) {fSortOrder = order;}
   static void SetSortType(Int_t type){fSortType = type;}

   virtual Bool_t IsSortable() const {return kTRUE;}

   Double_t Get() const {return fDatum;}
   void Set(Double_t val) { fDatum = val; }

   virtual void Clear(Option_t *opt = "") {
      TDataObject::Clear(opt);
      TDataObject::SetID(kInvalidI);
      this->Set(kInvalidD);
   }

   virtual void Copy(TObject& dest) const {
      TDataObject::Copy(dest);
      TGenericData &cobj = *(TGenericData*)&dest;
      cobj.Set(this->Get());
   }

   virtual Int_t Compare(const TObject *obj) const;

   virtual void Print(Option_t*) const;

   typedef enum {kID, kDatum} EGenericSortType; //!
   typedef enum {kASC, kDESC} ESortOrder; //!

protected:
   Double_t fDatum;
   static Int_t fSortType; //!
   static Int_t fSortOrder; //!

   ClassDef(TGenericData,1) // generic data
};

#endif // TGENERICDATA_H
