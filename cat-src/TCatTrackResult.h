/**
 * @file   TCatTrackResult.h
 * @brief  container for tracking result
 *
 * @date   Created       : 2016-07-24 11:37:39 JST
 *         Last Modified : 2018-02-04 13:17:23 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2016 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_DF45729E_1BB8_49A2_A2ED_968FE7E7ED47
#define INCLUDE_GUARD_UUID_DF45729E_1BB8_49A2_A2ED_968FE7E7ED47

#include "TObject.h"
#include <TVector3.h>
#include <vector>
#include <TCatTrackResultPulse.h>

namespace art {
   class TCatTrackResult;
   class TCatTrackResultPulse;
   class TCatPulseShape;
   class TCatReadoutPadArray;
   class TCatDiffusedChargeResponseHelper;
}

class TClonesArray;

class art::TCatTrackResult : public TVector3 {
public:
   TCatTrackResult();
   virtual ~TCatTrackResult();

   TCatTrackResult(const TCatTrackResult& rhs);
   TCatTrackResult& operator=(const TCatTrackResult& rhs);

   void Clear(Option_t* opt = "");
   // initialize arrays
   void Init();

   TVector3& GetStartPoint() { return fStartPoint; }
   TVector3& GetEndPoint() {return fEndPoint; } 

   Double_t GetRange() {return fRange; }
   void CalculateResults(const std::vector<art::TCatPulseShape*>& input);
   void CalculateRange() { fRange = (fEndPoint -fStartPoint). Mag(); }

   void SetChi2(Double_t chi2) { fChi2 = chi2; }
   Double_t GetChi2() const { return fChi2; }

   TCatTrackResultPulse *CreateAndGetPulse();

   TClonesArray *GetHitsHelper() { return fHitsHelper; }

   const std::vector<art::TCatTrackResultPulse*>& GetHits() const { return fHits; }

   void SetDEDX10(Double_t dedx) { fDEDX10 = dedx; }
   void SetDEDX30(Double_t dedx) { fDEDX30 = dedx; }
   void SetCharge10(Double_t charge){ fCharge10 = charge; }
   void SetCharge30(Double_t charge){ fCharge30 = charge; }
   void SetRange10(Double_t range) { fRange10 = range; }
   void SetRange30(Double_t range) { fRange30 = range; }
   void SetDistanceToEnd(Double_t distance) { fDistanceToEnd = distance; }
   void SetStatus(Int_t status) { fStatus = status; }
   void SetEDM(Double_t edm) { fEDM = edm; }

   void SetResponseHelper(TCatDiffusedChargeResponseHelper *helper) { fResponseHelper = helper; }
   void SetReadoutPadArray(TCatReadoutPadArray* array) { fReadoutPadArray = array; }

   Double_t GetDEDX10() const { return fDEDX10; }
   Double_t GetDEDX30() const { return fDEDX30; }

protected:
   TVector3 fStartPoint;
   TVector3 fEndPoint;

   Double_t fDistanceToEnd; // distance between endpoint and closest pad in ZX place

   Double_t fDEDX10; // averaged dedx between endpoint and 10-mm before it
   Double_t fDEDX30; // averaged dedx between endpoint and 30-mm before it
   Double_t fCharge10;
   Double_t fCharge30;
   Double_t fRange10;
   Double_t fRange30;
   
   Double_t fRange;

   Double_t fChi2;

   std::vector<TCatTrackResultPulse*> fHits;
   
   TClonesArray *fHitsHelper; //-> tracking result hits

   TCatReadoutPadArray *fReadoutPadArray; //! readout pad array
   TCatDiffusedChargeResponseHelper *fResponseHelper; //! helper to calculate charges

   Int_t fStatus; // minimization status
   Double_t fEDM; // estimated distance to minimum

private:

   ClassDef(TCatTrackResult,1) // container for tracking result
};

#endif // INCLUDE_GUARD_UUID_DF45729E_1BB8_49A2_A2ED_968FE7E7ED47
