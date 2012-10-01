/* $Id:$ */
/**
 * @file   TCatSimpleData.h
 * @date   Created : Feb 11, 2012 14:11:07 JST
 *   Last Modified : Feb 11, 2012 17:27:42 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#ifndef TCATSIMPLEDATA_H
#define TCATSIMPLEDATA_H

#include <TCatObject.h>
#include <TCatTimingData.h>
#include <TCatChargeData.h>

class TCatSimpleData  :   public TCatObject {
public:
   typedef enum { kID, kCharge, kTime } ESimpleDataSortType;
public:
   TCatSimpleData();
   ~TCatSimpleData();
   TCatSimpleData(const TCatSimpleData& rhs);

   virtual Int_t Compare(const TObject*) const;

   virtual Double_t GetTiming() const { return fTiming; }
   virtual void     SetTiming(const Double_t &val) { fTiming = val; }

   virtual Double_t GetCharge() const { return fCharge; }
   virtual void     SetCharge(const Double_t &val) { fCharge = val; }

   virtual void Copy(TObject& dest) const;
   
protected:
   Double_t fTiming;
   Double_t fCharge;
   
   ClassDef(TCatSimpleData,1);
};
#endif // end of #ifdef TCATDATAOBJECT_H
