/* $Id:$ */
/**
 * @file   TCatUHodo.h
 * @date   Created : Feb 04, 2012 17:04:20 JST
 *   Last Modified : Feb 05, 2012 11:40:27 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#ifndef TCATUHODO_H
#define TCATUHODO_H

#include <TCatUHodoPl.h>
#include <TCatDetectorArray.h>

class TCatUHodo : public TCatDetectorArray {

public:
   TCatUHodo();
   ~TCatUHodo();

   void AddHit(const Int_t &id, const Double_t &time, const Double_t &charge);
   
   virtual void Init(void);
   virtual TCatUHodoPl* GetHit(const Int_t &idx);
private:
   ClassDef(TCatUHodo,1);
};
#endif // end of #ifdef TCATUHODO_H
