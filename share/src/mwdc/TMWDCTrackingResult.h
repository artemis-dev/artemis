/**
 * @file   TMWDCTrackingResult.h
 * @brief
 *
 * @date   Created:       2013-10-17 16:02:43
 *         Last Modified: 2018-07-13 22:31:34 (masuoka)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved
 */

#ifndef INCLUDE_GUARD_UUID_9590E0F6_1A59_480C_BFF2_B6A517F535B6
#define INCLUDE_GUARD_UUID_9590E0F6_1A59_480C_BFF2_B6A517F535B6

#include "TTrack.h"
#include "TString.h"

namespace art {
   class TMWDCTrackingResult;
}

class art::TMWDCTrackingResult : public TTrack {
public:
   // Default constructor
   TMWDCTrackingResult();
   virtual ~TMWDCTrackingResult();
   // Copy constructor
   TMWDCTrackingResult(const TMWDCTrackingResult& rhs);
   // Assignment operator
   TMWDCTrackingResult& operator=(const TMWDCTrackingResult& rhs);

   enum ETrackingID {kDefault, kGood = 1, kLargeSSR = -1, kLargeCombination = -2};

   /* override */
   virtual void Copy(TObject &obj) const;
   virtual void Clear(Option_t *opt="");
   /* override end */

   void SetTrackingID(Int_t val) { SetID(val); }
   Int_t GetTrackingID() const { return GetID(); }

   Bool_t IsGood() const {return (GetID() == kGood); }
   Bool_t IsProcessed() const {return (GetID() != kDefault); }

   void SetSSR(Double_t val) {fSSR = val;}
   Double_t GetSSR() const {return fSSR;}

   void SetTrack(Double_t x, Double_t y, Double_t z,
		 Double_t a, Double_t b) {
      SetPos(x,y,z); SetAngle(a,b);
   }
   TTrack* GetTrack() { return this; } // obsolete. left for compatibility
   const TTrack* GetTrack() const { return this; } // obsolete. left for compatibility

   void SetResidual(Int_t idx, Double_t val){
      fResidual[idx] = val;
   }

   Double_t GetResidual(Int_t idx){
      return fResidual[idx];
   }

   void Init(Int_t nPlane);
   
   Int_t GetNPlane() const { return fNPlane; }
//   void SetNPlane(Int_t val) const { fNPlane = val; }
   
   Int_t GetNParameter() const { return fNParameter; }
   void SetNParameter(Int_t val) { fNParameter = val; }

   Int_t GetNPlaneValid() const { return fNPlaneValid; }
   void SetNPlaneValid(Int_t val) { fNPlaneValid = val; }

   Int_t GetPlaneCombinationID() const { return fPlaneCombinationID; }
   void SetPlaneCombinationID(Int_t val) { fPlaneCombinationID = val; }

   Int_t GetNCombination() const { return fNCombination; }
   void SetNCombination(Int_t val) { fNCombination = val; }

   Int_t GetNCombinationProcessed() const { return fNCombinationProcessed; }
   void SetNCombinationProcessed(Int_t val) { fNCombinationProcessed = val; }

   Int_t GetWireIDAdopted(Int_t idx) const { return fWireIDAdopted[idx]; }
   void SetWireIDAdopted(Int_t idx, Int_t val) { fWireIDAdopted[idx] = val; }

   Double_t GetDriftLengthAdopted(Int_t idx) const { return fDriftLengthAdopted[idx]; }
   void SetDriftLengthAdopted(Int_t idx, Double_t val) { fDriftLengthAdopted[idx] = val; }

   Double_t GetSigma(Int_t idx) const { return fSigma[idx]; }
   void SetSigma(Int_t idx, Double_t val) { fSigma[idx] = val; }

   Double_t GetVar(Int_t idx) const { return fVar[idx]; }
   void SetVar(Int_t idx, Double_t val) { fVar[idx] = val; }

   Double_t GetCI(Int_t idx) const { return fCI[idx]; }
   void SetCI(Int_t idx, Double_t val) { fCI[idx] = val; }

   Double_t GetTimeCost() const { return fTimeCost; }
   void SetTimeCost(Double_t val) { fTimeCost = val; }

   TString GetPlaneName(Int_t idx) const { return fPlaneName[idx]; }
   void SetPlaneName(Int_t idx, TString val) { fPlaneName[idx] = val; }
   
   static const Int_t kMAXNPARAMETER = 4;

   virtual void Print(Option_t*) const;

protected:

   Int_t    fNPlane;
   Int_t    fNPlaneValid;

   Int_t    fPlaneCombinationID;
   Int_t    fNParameter;
   Int_t    fNCombination;
   Int_t    fNCombinationProcessed;

   std::vector<Int_t> fWireIDAdopted;      //
   std::vector<Double_t> fDriftLengthAdopted; //

   Double_t fSSR;                // sum of squared residuals
   std::vector<Double_t> fResidual;          //

   std::vector<Double_t> fSigma; // Standard Error of tracking parameter
   std::vector<Double_t> fVar;   // Unbiased variance of tracking parameter
   std::vector<Double_t> fCI;    // Confident Interval of tracking parameter
   std::vector<TString>  fPlaneName; //Plane name
   Double_t fTimeCost;                     // time cost in msec. obsolete

   ClassDef(TMWDCTrackingResult,3) // container for MWDC tracking result
};

#endif // INCLUDE_GUARD_UUID_9590E0F6_1A59_480C_BFF2_B6A517F535B6
