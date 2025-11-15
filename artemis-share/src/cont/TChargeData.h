/**
 * @file   TChargeData.h
 * @brief
 *
 * @date   Created:       2013-11-15 14:09:04
 *         Last Modified: 2014-06-20 17:06:43 JST (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved
 */

#ifndef TCHARGEDATA_H
#define TCHARGEDATA_H

#include "TDataObject.h"
#include "ICharge.h"

namespace art {
   class TChargeData;
}


class art::TChargeData :public TDataObject, public ICharge {
public:
   // Default constructor
   TChargeData();
   // destructor
   virtual ~TChargeData();
   // Copy constructor
   TChargeData(const TChargeData& rhs);
   // Assignment operator
   TChargeData& operator=(const TChargeData& rhs);
   using ICharge::operator=;

   // set sort order. order must be among EChargeSortType.
   static void SetSortOrder(Int_t order) {fSortOrder = order;}
   // set sort order. type must be among EChargeSortOrder.
   static void SetSortType(Int_t type){fSortType = type;}

   virtual Bool_t IsSortable() const {return kTRUE;}

   virtual Double_t GetCharge() const { return fCharge;}
   virtual void SetCharge(Double_t val) { fCharge = val; }
   virtual void SetCharge(const ICharge &obj) { SetCharge(obj.GetCharge()); }

   // obsolete. use TDataObject::SetID()
   virtual void SetDetID(Int_t val) { SetID(val); }
   // obsolete. use TDataObject::GetID()
   virtual Int_t GetDetID() const {return GetID();}

   virtual void Clear(Option_t* opt = "");

   virtual void Copy(TObject& dest) const {
      TDataObject::Copy(dest);
      TChargeData &cobj = *(TChargeData*)&dest;
      cobj.SetCharge(GetCharge());
   }

   virtual Int_t Compare(const TObject *obj) const;

   virtual void Print(Option_t*) const;

   typedef enum {kID, kCharge} EChargeSortType; //!
   typedef enum {kASC, kDESC} EChargeSortOrder; //!
   
protected:
   Double_t fCharge;

   static Int_t fSortType; //!
   static Int_t fSortOrder; //!

   ClassDef(TChargeData,1) // container for charge information
};

#endif // TCHARGEDATA_H
