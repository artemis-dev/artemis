/* $Id:$ */
/**
 * @file   TRawDataFadc.h
 * @date   Created : Jul 20, 2013 23:20:58 JST
 *   Last Modified : Oct 18, 2013 16:55:03 JST
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

   virtual void SetFadcInfo(const UInt_t time,Int_t offset, UInt_t pat) {
      fTimestamp = time;
      fOffset = offset;
      fPattern = pat;
   }

   virtual void Add(Int_t adc) {
      fADC[fNumSample] = adc;
      fClock[fNumSample] = fNumSample;
      fNumSample++;
   }

   virtual void Clear(const Option_t* opt = "") {
      fNumSample = 0;
      TRawDataObject::Clear(opt);
//      SetGeo(TRawDataObject::kInvalid);
//      SetCh(TRawDataObject::kInvalid);
   }

   virtual Int_t GetNumSample() { return fNumSample; }
   virtual UInt_t GetTimestamp() { return fTimestamp; }
   virtual Int_t GetOffset() { return fOffset; }
   virtual UInt_t GetPattern() { return fPattern; }

   UShort_t& operator[] (int idx) { return fADC[idx]; }
   
         

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
