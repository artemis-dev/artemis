/* $Id:$ */
/**
 * @file   TCatPulseShape.h
 * @date   Created : Mar 10, 2013 18:10:59 JST
 *   Last Modified : Mar 29, 2014 23:50:22 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#ifndef TCATPULSESHAPE_H
#define TCATPULSESHAPE_H

#include <TDataObject.h>
#include <vector>

#define VEC

namespace art {
   class TCatPulseShape;
}

class art::TCatPulseShape  : public TDataObject {

public:
   static const UInt_t kWrongBitCorrected = 1<<16;
   static const UInt_t kHighPassUnfolded  = 1<<17;
   static const UInt_t kLowPassUnfolded   = 1<<18;
   static const UInt_t kLongPulse         = 1<<19;
   static const UInt_t kOutOfOffset       = 1<<20;

   TCatPulseShape();
   ~TCatPulseShape();

   virtual void Copy(TObject &pulse) const;
   
   Int_t GetCh() const { return fCh; }
   Int_t GetGeo() const { return fGeo; }
   Double_t GetTime() const  { return fTime; }
   Float_t GetOffset() const { return fOffset; }
   Int_t   GetNumSample() const { return fNumSample; }
   Double_t GetCharge() const { return fCharge; }
   Float_t GetRiseTime() const { return fRiseTime; }
   std::vector<Float_t>& GetSample() { return fSample; }
   std::vector<Float_t>& GetClock() { return fClock; }
   Double_t GetX() const { return fX; }
   Double_t GetZ() const { return fZ; }
   
   
   Float_t &operator[] (Int_t idx) { return fSample[idx]; }
   Float_t &operator() (Int_t idx) { return fClock[idx]; }

   void SetGeo(Int_t geo) { fGeo = geo; }
   void SetCh(Int_t ch) { fCh = ch; }
   void SetTime(Double_t time) { fTime = time; }
   void SetOffset(Float_t offset) { fOffset = offset; }
   void SetCharge(Double_t charge) { fCharge = charge; }
   void SetRiseTime(Float_t risetime) { fRiseTime = risetime; }
   void SetNumSample(Int_t n) { fNumSample = n; }
   void SetXZ(Double_t x, Double_t z) { fX = x; fZ = z; }
   
   void AddSample(Float_t sample,Float_t clock = -1) {
#ifndef VEC
      fSample[fNumSample] = sample;
      if (clock == -1) {
         fClock[fNumSample] = fNumSample;
      } else {
         fClock[fNumSample] = clock;
      }
      fNumSample++;
#else
      fSample.push_back(sample);
      fClock.push_back((clock!=-1)?clock:fNumSample);
      fNumSample++;
#endif
                       
   }

   virtual void Clear(Option_t *);
   virtual Bool_t IsSortable() const { return kTRUE; }
   virtual Int_t Compare (const TObject *obj) const;
      
   static const int kMaxSample = 20000;
   typedef enum { kID, kTiming, kCharge } ESortType;
   typedef enum { kASC, kDESC } ESortOrder;

   static void SetSortType(ESortType type) { fgSortType = type; }
   static void SetSortOrder(ESortOrder order) {
      fgSortOrder = (order == kASC) ? 1 : -1; 
   }


protected:
   static ESortType fgSortType;
   static Int_t fgSortOrder;

   Int_t fCh;
   Int_t fGeo;
   Double_t fTime;
   Double_t fCharge;
   Float_t fRiseTime;
   Float_t fOffset;
   Int_t fNumSample;
   Double_t fX;
   Double_t fZ;
#ifndef VEC   
   Float_t *fSample; //[fNumSample]
   Float_t *fClock; //[fNumSample]
#else
   std::vector<Float_t> fSample;
   std::vector<Float_t> fClock;
#endif

   ClassDef(TCatPulseShape,1);
};
#endif // end of #ifdef TCATPULSESHAPE_H
