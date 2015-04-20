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
   class TSimpleData;
}

class art::TSimpleData  : public TDataObject {

public:
   TSimpleData(){ Invalidate();}
   ~TSimpleData(){;}

   void SetValue(Double_t value) { 
      fValue = value; 
      Validate();
   }
   Double_t GetValue() const { return fValue; }


   Double_t fValue;
   ClassDef(TSimpleData,1);
};


#endif // end of #ifdef TSIMPLEDATA_H
