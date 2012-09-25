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

class TCatSimpleData  : public TCatObject { 
public:
   typedef enum { kID, kCharge, kTime } ESimpleDataSortType;
public:
   TCatSimpleData();
   ~TCatSimpleData();
   TCatSimpleData(const TCatSimpleData& rhs);

   virtual Double_t GetTime() { return fTime; }
   virtual Double_t GetCharge() { return fCharge; }

   virtual void SetTime(const Double_t& val) { fTime = val; }
   virtual void SetCharge(const Double_t& val) { fCharge = val; }

   virtual Int_t Compare(const TObject*) const;

protected:

   Double_t fTime;
   Double_t fCharge;

   ClassDef(TCatSimpleData,1);
};
#endif // end of #ifdef TCATDATAOBJECT_H
