/* $Id:$ */
/**
 * @file   TCatScalerData.h
 * @date   Created : Feb 12, 2013 05:12:04 JST
 *   Last Modified : 2018-06-25 20:41:05 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#ifndef TCATSCALERDATA_H
#define TCATSCALERDATA_H

#include <TNamed.h>

namespace art {
  class TCatScalerData;
}

class art::TCatScalerData  : public TNamed {

public:
   TCatScalerData(Int_t num = 32);
   virtual ~TCatScalerData();

   void SetNumChannel(Int_t fNumChannel);
   Int_t GetNumChannel() const { return fNumChannel; }
   void SetValue(Int_t idx, UInt_t data) { if (idx>-1 && idx < fNumChannel) fData[idx] = data; }
   UInt_t GetValue(Int_t idx) { 
      if (idx>-1 && idx < fNumChannel) return fData[idx]; 
      return 0;
   }

   virtual void Clear(Option_t *opt = "") ;
   

private:
   Int_t   fNumChannel;
   UInt_t *fData; //[fNumChannel]
//   Int_t   fType; 

   ClassDef(TCatScalerData,1);
};
#endif // end of #ifdef TCATSCALERDATA_H
