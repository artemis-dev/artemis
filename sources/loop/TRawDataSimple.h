/* $Id:$ */
/**
 * @file   TRawDataSimple.h
 * @date   Created : Jul 22, 2013 18:22:01 JST
 *   Last Modified : Jun 01, 2014 22:23:42 JST
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
   virtual ~TRawDataSimple();

   void Set(const T val) { fValue = val; }
   T Get() const { return fValue; }

   virtual Int_t GetNumValues() const { return 1; }
   virtual Double_t GetValue(Int_t /* idx */) const { return (Double_t)Get(); }

   void Clear(Option_t *opt="") {
      TRawDataObject::Clear(opt);
      fValue = art::TRawDataObject::kInvalid;
   }
      
private:
   T fValue;

   ClassDef(TRawDataSimple,1);
};
}
#endif // end of #ifdef TRAWDATASIMPLE_H
