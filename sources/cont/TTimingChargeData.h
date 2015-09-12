/**
 * @file   TTimingChargeData.h
 * @brief
 *
 * @date   Created:       2013-08-14 10:19:28
 *         Last Modified: 2014-02-24 14:39:51
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved
 */

#ifndef TTIMINGCHARGEDATA_H
#define TTIMINGCHARGEDATA_H

#include "TDataObject.h"
#include "ITiming.h"
#include "ICharge.h"

namespace art{
   class TTimingChargeData;
}

class art::TTimingChargeData : public TDataObject, public ITiming, public ICharge {
public:
   typedef enum {kID, kCharge, kTiming} ETimingChargeSortType; //!
   typedef enum {kASC, kDESC} ESortOrder; //!

   // Default constructor
   TTimingChargeData();
   virtual ~TTimingChargeData();
   // Copy constructor
   TTimingChargeData(const TTimingChargeData& rhs);
   // Assignment operator
   virtual TTimingChargeData& operator=(const TTimingChargeData& rhs);

   virtual Int_t Compare(const TObject*) const;

   virtual Double_t GetTiming() const {return fTiming;}
   virtual void     SetTiming(Double_t val){fTiming = val;}
   virtual void     SetTiming(const ITiming& obj){SetTiming(obj.GetTiming());}

   virtual Double_t GetCharge() const {return fCharge;}
   virtual void     SetCharge(Double_t val){fCharge = val;}
   virtual void     SetCharge(const ICharge& obj){SetCharge(obj.GetCharge());}

   // obsolete. use TDataObject::GetID()
   virtual Int_t GetDetID() const {return GetID();}
   // obsolete. use TDataObject::SetID()
   virtual void  SetDetID(Int_t val){ SetID(val);}

   static void SetSortOrder(Int_t order){fSortOrder = order;}
   static void SetSortType(Int_t type){fSortType = type;}

   virtual Bool_t IsSortable() const {return kTRUE;}

   virtual void Copy(TObject& dest) const {
      TDataObject::Copy(dest);
      TTimingChargeData &cobj = *(TTimingChargeData*)&dest;
      cobj.SetTiming(this->GetTiming());
      cobj.SetCharge(this->GetCharge());
   }

   virtual void Clear(Option_t *opt="");

   virtual void Print(Option_t *) const;

protected:
   Double_t fTiming;
   Double_t fCharge;
   static Int_t fSortType;  //!
   static Int_t fSortOrder; //!

   ClassDef(TTimingChargeData,1) // container for timing information with charge
};

#endif // TTIMINGCHARGEDATA_H
