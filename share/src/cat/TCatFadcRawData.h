/* $Id:$ */
/**
 * @file   TCatFadcRawData.h
 * @date   Created : Feb 09, 2013 03:09:20 JST
 *   Last Modified : Oct 22, 2013 00:23:18 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#ifndef TCATFADCRAWDATA_H
#define TCATFADCRAWDATA_H

#include <TObject.h>

class TCatFadcRawData  : public TObject {

public:
   TCatFadcRawData();
   ~TCatFadcRawData();

   //TCatFadcRawData(int id, unsigned int timestamp);

   void AddSample(int adc, int clock = -1){
      fADC[fNumSample] = adc;
      if (clock != -1) {
         fClock[fNumSample] = clock;
      } else {
         fClock[fNumSample] = fNumSample;
      }
      fNumSample++;
   }
   UShort_t &operator[] (int idx) { return fADC[idx]; }
   UShort_t &operator() (int idx) { return fClock[idx]; }
   void SetTimestamp(UInt_t timestamp) { fTimestamp = timestamp; }
   void SetID(Int_t id) { fID = id; }
   void SetPattern(UShort_t pat) { fPattern = pat; }
   Int_t GetID() { return fID; }
   Int_t GetGeo() { return fGeo; }
   void SetGeo(Int_t geo) { fGeo = geo;}
   void SetCh(Int_t ch) { fCh = ch; }
   void SetOffset(Int_t offset) { fOffset = offset; }
   Int_t GetCh() { return fCh; }
   Int_t     GetNumSample() { return fNumSample; }
   UShort_t GetTimestamp() { return fTimestamp; }
   UShort_t GetPattern() { return fPattern; }
   Int_t GetOffset() { return fOffset; }

   virtual void Clear(const Option_t* = "");
   
protected:
   static const int kMaxSample = 20000;
   Int_t fID;
   Int_t fCh;
   Int_t fGeo;
   UInt_t fTimestamp;
   Int_t fOffset;
   UShort_t fPattern;
   Int_t fNumSample;
   UShort_t *fADC; //[fNumSample]
   UShort_t *fClock; //[fNumSample]

   ClassDef(TCatFadcRawData,1);
};
#endif // end of #ifdef TCATFADCRAWDATA_H
