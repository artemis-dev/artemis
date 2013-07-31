/* $Id:$ */
/**
 * @file   TRawDataSimple.h
 * @date   Created : Jul 22, 2013 18:22:01 JST
 *   Last Modified : 2013-07-31 16:19:15
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#ifndef TRAWDATASIMPLE_H
#define TRAWDATASIMPLE_H

#include <TRawDataObject.h>

namespace art {

template <typename T> class TRawDataSimple  : public TRawDataObject  {

public:
   TRawDataSimple();
   ~TRawDataSimple();

   void Set(T val) { fValue = val; }
   T Get() { return fValue; }
      
private:
   T fValue;

   ClassDef(TRawDataSimple,1);
};
}
#endif // end of #ifdef TRAWDATASIMPLE_H
