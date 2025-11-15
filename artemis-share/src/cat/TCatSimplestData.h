/* $Id:$ */
/**
 * @file   TCatSimplestData.h
 * @date   Created : Feb 11, 2012 14:11:07 JST
 *   Last Modified : Feb 11, 2012 17:27:42 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#ifndef TCATSIMPLESTDATA_H
#define TCATSIMPLESTDATA_H

#include <TObject.h>

class TCatSimplestData : public TObject {
public:
  typedef enum { kID, kCharge, kTime } ESimpleDataSortType; //!
  typedef enum { kASC, kDESC } ESortOrder;//!
public:
   TCatSimplestData();
   ~TCatSimplestData();
   TCatSimplestData(const TCatSimplestData& rhs);

   virtual Int_t Compare(const TCatSimplestData*) const;

   virtual Double_t GetTiming() const { return fTiming; }
   virtual void     SetTiming(const Double_t &val) { fTiming = val; }

   virtual Double_t GetCharge() const { return fCharge; }
   virtual void     SetCharge(const Double_t &val) { fCharge = val; }

   virtual Int_t GetDetID() const { return fDetectorID; }
   virtual void  SetDetID(const Int_t &val) { fDetectorID = val; }

   static void SetSortOrder(const Int_t &order) { fSortOrder = order; }
   static void SetSortType(const Int_t &type) { fSortType = type; }


//   virtual Int_t GetCatID() const { return fCatID; }
//   virtual void  SetCatID(const Int_t &val) { fCatID = val; }

   virtual void Copy(TObject& dest) const {
     TObject::Copy(dest);
     TCatSimplestData &cobj = *(TCatSimplestData*)&dest;
     cobj.fTiming = fTiming;
     cobj.fCharge = fCharge;
     cobj.fDetectorID = fDetectorID;
   };
   
protected:
   Double_t fTiming;
   Double_t fCharge;
   Int_t fDetectorID;
   static Int_t fSortType; //!
   static Int_t fSortOrder; //!
   
   ClassDef(TCatSimplestData,1);
};
#endif // end of #ifdef TCATDATAOBJECT_H
