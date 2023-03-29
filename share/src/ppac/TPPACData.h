/**
 * @file   TPPACData.h
 * @brief
 *
 * @date   Created:       2013-10-29 14:58:14
 *         Last Modified: 2014-02-24 14:42:16
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved
 */

#ifndef TPPACDATA_H
#define TPPACDATA_H

#include "TDataObject.h"
#include "ITiming.h"
#include "constant.h"

namespace art {
   class TPPACData;
   class TConverterBase;
}

class art::TPPACData : public TDataObject, public ITiming {
public:
   typedef enum {kID, kTiming} ESortType;
   typedef enum {kASC, kDESC} ESortOrder;
   // quality bit
   static const UInt_t kBadTXSum = (1<<0);
   static const UInt_t kBadTYSum = (1<<1);
   
   // Default constructor
   TPPACData();
   // destructor
   virtual ~TPPACData();
   // Copy constructor
   TPPACData(const TPPACData& rhs);
   // Assignment operator
   TPPACData& operator=(const TPPACData& rhs);

   Double_t GetTX1() const { return fTX1; } 
   void     SetTX1(Double_t val) { fTX1 = val; }
   Double_t GetTX2() const { return fTX2; } 
   void     SetTX2(Double_t val) { fTX2 = val; }
   Double_t GetTY1() const { return fTY1; } 
   void     SetTY1(Double_t val) { fTY1 = val; }
   Double_t GetTY2() const { return fTY2; } 
   void     SetTY2(Double_t val) { fTY2 = val; }
   Double_t GetTAnode() const { return fTAnode; } 
   void     SetTAnode(Double_t val) { fTAnode = val; }

   virtual Double_t GetTiming() const { return fTAnode; } 

   Double_t GetQX1() const { return fQX1; } 
   void     SetQX1(Double_t val) { fQX1 = val; }
   Double_t GetQX2() const { return fQX2; } 
   void     SetQX2(Double_t val) { fQX2 = val; }
   Double_t GetQY1() const { return fQY1; } 
   void     SetQY1(Double_t val) { fQY1 = val; }
   Double_t GetQY2() const { return fQY2; } 
   void     SetQY2(Double_t val) { fQY2 = val; }
   Double_t GetQAnode() const { return fQAnode; } 
   void     SetQAnode(Double_t val) { fQAnode = val; }

   void SetT(Double_t val, Int_t idx) {
      switch (idx) {
	 case 0: SetTX1(val); return;
	 case 1: SetTX2(val); return;
	 case 2: SetTY1(val); return;
	 case 3: SetTY2(val); return;
	 case 4: SetTAnode(val); return;
      }
   }

   void SetQ(Double_t val, Int_t idx) {
      switch (idx) {
	 case 0: SetQX1(val); return;
	 case 1: SetQX2(val); return;
	 case 2: SetQY1(val); return;
	 case 3: SetQY2(val); return;
	 case 4: SetQAnode(val); return;
      }
   };

   Double_t GetT(Int_t idx) const {
      switch (idx) {
	 case 0: return GetTX1();
	 case 1: return GetTX2();
	 case 2: return GetTY1();
	 case 3: return GetTY2();
	 case 4: return GetTAnode();
      }
      return kInvalidD;
   }

   Double_t GetQ(Int_t idx) const {
      switch (idx) {
	 case 0: return GetQX1();
	 case 1: return GetQX2();
	 case 2: return GetQY1();
	 case 3: return GetQY2();
	 case 4: return GetQAnode();
      }
      return kInvalidD;
   };

   Double_t GetX() const {return fX;}
   Double_t GetY() const {return fY;}
   Double_t GetZ() const {return fZ;}
   void SetX(Double_t x) { fX = x; }
   void SetY(Double_t y) { fY = y; }
   void SetZ(Double_t z) { fZ = z; }

   Double_t GetTXDiff() const {return fTXDiff;}
   Double_t GetTYDiff() const {return fTYDiff;}
   void SetTXDiff(Double_t val){ fTXDiff = val;}
   void SetTYDiff(Double_t val){ fTYDiff = val;}

   Double_t GetTXSum() const {return fTXSum;}
   Double_t GetTYSum() const {return fTYSum;}
   Double_t GetQXSum() const {return fQXSum;}
   Double_t GetQYSum() const {return fQYSum;}

   Int_t GetDetID() const { return GetID(); }
   void  SetDetID(Int_t val){ SetID(val); }

   void SetXConverter(const TConverterBase* conv) {fXConv = conv;}
   void SetYConverter(const TConverterBase* conv) {fYConv = conv;}

   void Update();

   virtual void Copy(TObject& dest) const;

   virtual void Clear(Option_t *opt="");

   virtual Bool_t IsSortable() const {return kTRUE;}
   virtual Int_t Compare(const TObject *obj) const;

   static void SetSortOrder(ESortOrder order) {
      fgSortOrder = order;
      if (fgSortOrder == kASC) fgOrderSign = -1;
      if (fgSortOrder == kDESC) fgOrderSign = 1;
   }
   static void SetSortType(ESortType Type) { fgSortType = Type; }
   

   static const Int_t kNRAW = 5; // number of (T,Q) data pair

protected:
   Double_t fTX1;
   Double_t fTX2;
   Double_t fTY1;
   Double_t fTY2;
   Double_t fTAnode;
   Double_t fQX1;
   Double_t fQX2;
   Double_t fQY1;
   Double_t fQY2;
   Double_t fQAnode;
   Double_t fX;
   Double_t fY;
   Double_t fTXDiff;
   Double_t fTYDiff;
   Double_t fTXSum;
   Double_t fTYSum;
   Double_t fQXSum;
   Double_t fQYSum;
   Double_t fZ;

   const TConverterBase *fXConv; //!
   const TConverterBase *fYConv; //!

   static ESortType fgSortType; //!
   static ESortOrder fgSortOrder; //!
   static Int_t fgOrderSign;

   virtual void SetTiming(Double_t) { MayNotUse("SetTiming(Double_t)");}
   virtual void SetTiming(const ITiming&) { MayNotUse("SetTiming(const ITiming&)");}

   ClassDef(TPPACData,2) // PPAC data container
};

#endif // TPPACDATA_H
