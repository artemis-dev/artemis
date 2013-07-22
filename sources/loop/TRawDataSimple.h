/* $Id:$ */
/**
 * @file   TRawDataSimple.h
 * @date   Created : Jul 22, 2013 18:22:01 JST
 *   Last Modified : Jul 22, 2013 18:19:25 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#ifndef TRAWDATASIMPLE_H
#define TRAWDATASIMPLE_H

#include <TRawDataObject.h>

namespace art {
   class TRawDataSimple;
}

class art::TRawDataSimple  : public TRawDataObject  {

public:
   TRawDataSimple();
   ~TRawDataSimple();

   void Set(Int_t val) { fValue = val; }
   Int_t Get() { return fValue; }
      
private:
   Int_t fValue;

   ClassDef(TRawDataSimple,1);
};
#endif // end of #ifdef TRAWDATASIMPLE_H
