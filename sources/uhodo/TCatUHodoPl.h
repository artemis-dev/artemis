/* $Id:$ */
/**
 * @file   TCatUHodoPl.h
 * @date   Created : Feb 04, 2012 17:04:08 JST
 *   Last Modified : Feb 05, 2012 02:44:50 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#ifndef TCATUHODOPL_H
#define TCATUHODOPL_H

#include <TCatObject.h>

class TCatUHodoPl  : public TCatObject {

public:
   TCatUHodoPl();
   ~TCatUHodoPl();

   void SetTime(const Double_t &time) { fTime = time; }
   void SetCharge(const Double_t &charge) { fCharge = charge; }

   Double_t GetTime() { return fTime; }
   Double_t GetCharge() { return fCharge; }

   virtual Int_t Compare(const TObject *) const;
   virtual Bool_t IsSortable() const { return kTRUE; }
protected:
   Double_t fTime;
   Double_t fCharge;

   ClassDef(TCatUHodoPl,1);
};
#endif // end of #ifdef TCATUHODOPL_H
