/**
 * @file   TMWDCHitData.h
 * @brief  MWDC Hit Data (time, charge, wireID, drift length)
 *
 * @date   Created:       2013-10-17 13:17:25
 *         Last Modified: 2014-02-24 14:54:34
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved
 */

#ifndef TMWDCHITDATA_H
#define TMWDCHITDATA_H

#include "TTimingChargeData.h"

namespace art{
   class TMWDCHitData;
}

class art::TMWDCHitData : public TTimingChargeData {
public:
   typedef enum {kID,kCharge,kTime,kDriftLength} EMWDCHitSortType; //! keep consistency with enum in TTimingChargeData

   // Default constructor
   TMWDCHitData();
   // Default destructor
   virtual ~TMWDCHitData();
   // Assignment operator
   TMWDCHitData& operator=(const TMWDCHitData& rhs);

   Double_t GetDriftLength() const { return fDriftLength; }
   void SetDriftLength( const Double_t val ) { fDriftLength = val; }

   virtual void Copy(TObject& dest) const {
      TTimingChargeData::Copy(dest);
      TMWDCHitData &cobj = *(TMWDCHitData*)&dest;
      cobj.fDriftLength = fDriftLength;
   }

   virtual void Clear(Option_t *opt="");
   
   virtual Bool_t IsSortable() const {return kTRUE;}
   virtual Int_t Compare(const TObject* obj) const;

   virtual void Print(Option_t*) const;

protected:
   Double_t fDriftLength;

   ClassDef(TMWDCHitData,1) // container for each hit of MWDC wire
};

#endif // TMWDCHITDATA_H
