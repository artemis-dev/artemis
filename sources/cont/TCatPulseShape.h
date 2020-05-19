/* $Id:$ */
/**
 * @file   TCatPulseShape.h
 * @date   Created : Mar 10, 2013 18:10:59 JST
 *   Last Modified : 2019-05-15 11:21:56 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#ifndef TCATPULSESHAPE_H
#define TCATPULSESHAPE_H

#include "TDataObject.h"
#include "ICharge.h"
#include "ITiming.h"
#include "IPosition.h"
#include <vector>
#include "TVector3.h"
#include "TMath.h"


namespace art {
   class TCatPulseShape;
}

class TBuffer;

class art::TCatPulseShape  : public TDataObject, public ICharge, public ITiming, public IPosition {

public:
   static const UInt_t kWrongBitCorrected = 1<<16;
   static const UInt_t kHighPassUnfolded  = 1<<17;
   static const UInt_t kLowPassUnfolded   = 1<<18;
   static const UInt_t kLongPulse         = 1<<19;
   static const UInt_t kOutOfOffset       = 1<<20;
   static const UInt_t kBadPad            = 1<<21;

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
   const std::vector<Float_t>& GetSample() const { return fSample; }
   const std::vector<Float_t>& GetClock() const { return fClock; }
   Double_t GetX() const { return fPos.X(); }
   Double_t GetY() const { return fPos.Y(); }
   Double_t GetZ() const { return fPos.Z(); }
   double X() const { return GetX(); }
   double Y() const { return GetY(); }
   double Z() const { return GetZ(); }
   void SetX(double x) { fPos.SetX(x); }
   void SetY(double y) { fPos.SetY(y); }
   void SetZ(double z) { fPos.SetZ(z); }
   TVector3& GetPos() { return fPos; }
   const TVector3& GetPos() const { return fPos; }
   
   Float_t &operator[] (Int_t idx) { return fSample[idx]; }
   Float_t &operator() (Int_t idx) { return fClock[idx]; }

   void SetGeo(Int_t geo) { fGeo = geo; }
   void SetCh(Int_t ch) { fCh = ch; }
   void SetTime(Double_t time) { fTime = time; }
   void SetOffset(Float_t offset) { fOffset = offset; }
   void SetCharge(const ICharge &obj) { SetCharge(obj.GetCharge()); }
   void SetCharge(Double_t charge) { fCharge = charge; }
   void SetRiseTime(Float_t risetime) { fRiseTime = risetime; }
   void SetNumSample(Int_t n) { fNumSample = n; }
   // will be obsoluted
   void SetXZ(Double_t x, Double_t z) { fPos.SetX(x); fPos.SetZ(z); }

   
   void AddSample(Float_t sample,Float_t clock = -1) {
      fSample.push_back(sample);
      fClock.push_back((clock!=-1)?clock:fNumSample);
      fNumSample++;
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

   // for version > 2
   void SetMaxSample(Double_t max) { fMaxSample = max; }
   void SetMaxSampleOffset(Double_t offset) { fMaxSampleOffset = offset; }
   void SetBaseline(Double_t baseline) { fBaseline = baseline; }
   void SetBaselineRMS(Double_t rms) { fBaselineRMS = rms; }
   void SetLeadingEdgeOffset(Double_t offset) { fLeadingEdgeOffset = offset; }
   void AddMoment(Double_t moment) {
      fMoment.push_back(moment);
      fNumMoment = fMoment.size();
   }

   virtual Double_t GetMaxSample() const { return fMaxSample; }
   virtual Double_t GetMaxSampleOffset() const { return fMaxSampleOffset; }
   virtual Double_t GetBaseline() const { return fBaseline; }
   virtual Double_t GetBaselineRMS() const { return fBaselineRMS; }
   virtual Double_t GetLeadingEdgeOffset() const { return fLeadingEdgeOffset; }
   virtual Double_t GetMoment(Int_t i) const { return  (i < fNumMoment) ? fMoment[i] : TMath::QuietNaN(); }
   // pure virtual method in 
   virtual Double_t GetTiming() const { return fTime; }
   virtual void SetTiming(Double_t time) { fTime = time; }
   virtual void SetTiming(const ITiming& time) { fTime = time.GetTiming(); }



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
   TVector3 fPos;
   
   std::vector<Float_t> fSample;
   std::vector<Float_t> fClock;

   // information for the version above 3
   Double_t fMaxSample;
   Double_t fMaxSampleOffset;
   Double_t fBaseline;
   Double_t fBaselineRMS;
   Double_t fLeadingEdgeOffset;
   Int_t    fNumMoment;
   std::vector<Double_t> fMoment;
   

   ClassDef(TCatPulseShape,3)
};
#endif // end of #ifdef TCATPULSESHAPE_H
