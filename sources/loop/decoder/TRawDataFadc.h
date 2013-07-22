/* $Id:$ */
/**
 * @file   TRawDataFadc.h
 * @date   Created : Jul 20, 2013 23:20:58 JST
 *   Last Modified : Jul 22, 2013 09:14:13 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#ifndef TRAWDATAFADC_H
#define TRAWDATAFADC_H

#include <TRawDataObject.h>

namespace art {
   class TRawDataFadc;
}

class art::TRawDataFadc  : public TRawDataObject {

public:
   TRawDataFadc();
   virtual ~TRawDataFadc();

   virtual void Set(Int_t geo, Int_t ch, const UInt_t time,
                    Int_t offset, UInt_t pat) {
      SetGeo(geo);
      SetCh(ch);
      fTimestamp = time;
      fOffset = offset;
      fPattern = pat;
   }

   virtual void Add(Int_t adc) {
      if (fNumSample < 0 || fNumSample >= kMaxSample) {
         printf("Exceeds the maimum number of sample\n");
         return;
      }
      fADC[fNumSample] = adc;
      fClock[fNumSample] = fNumSample;
      fNumSample++;
   }

   virtual void Clear(const Option_t* opt = "") {
      fNumSample = 0;
      SetGeo(TRawDataObject::kInvalid);
      SetCh(TRawDataObject::kInvalid);
   }     
         

protected:
   static const Int_t kMaxSample = 1000;
   Int_t fNumSample;
   UShort_t fADC[kMaxSample]; //[fNumSample]
   UShort_t  fClock[kMaxSample]; //[fNumSample]
   UInt_t fTimestamp;
   Int_t          fOffset;
   UInt_t  fPattern;

   ClassDef(TRawDataFadc,1);
};
#endif // end of #ifdef TRAWDATAFADC_H
