/* $Id:$ */
/**
 * @file   TSimpleData.h
 * @date   Created : Jul 11, 2013 22:11:59 JST
 *   Last Modified : Apr 15, 2015 14:47:32 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#ifndef TSIMPLEDATA_H
#define TSIMPLEDATA_H

#include <TDataObject.h>

namespace art {
   template <class T> class TSimpleDataBase;
   class TSimpleData;
   class TSimpleDataLong;
   class TSimpleDataInt;
   class TSimpleDataDouble;
};

namespace art {
   template <class T> class TSimpleDataBase : public TDataObject {
   public:
      TSimpleDataBase() { Invalidate(); }
      ~TSimpleDataBase() {;}

      void SetValue(T value) {
         fValue = value;
         Validate();
      }
      T GetValue() const { return fValue; }
   protected:   
      T fValue;
      ClassDef(TSimpleDataBase,1)
   };
}

class art::TSimpleData  : public TSimpleDataBase<Double_t> {
   ClassDef(TSimpleData,1)  
};
class art::TSimpleDataInt  : public TSimpleDataBase<Int_t> {
   ClassDef(TSimpleDataInt,1)
};
class art::TSimpleDataLong  : public TSimpleDataBase<Long_t> {
   ClassDef(TSimpleDataLong, 1)
};
class art::TSimpleDataDouble : public TSimpleDataBase<Double_t> {
   ClassDef(TSimpleDataDouble, 1)
};


#endif // end of #ifdef TSIMPLEDATA_H
