/**
 * @file   TCatTpcTrack.h
 * @brief  container for tpc track
 *
 * @date   Created       : 2016-07-24 11:37:39 JST
 *         Last Modified : 2020-07-19 05:34:57 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2016 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_DF45729E_1BB8_49A2_A2ED_968FE7E7ED47_1
#define INCLUDE_GUARD_UUID_DF45729E_1BB8_49A2_A2ED_968FE7E7ED47_1

#include "TObject.h"
#include <TVector3.h>
#include <TDataObject.h>
#include <vector>
#include <TCatTrackResultPulse.h>
#include "ITrack.h"

namespace art {
   class TCatTpcTrack;
//   class TCatTrackResultPulse;
   class TCatPulseShape;
   class TCatReadoutPadArray;
   class TCatDiffusedChargeResponseHelper;
}

class TClonesArray;

class art::TCatTpcTrack : public TDataObject, public ITrack {
public:
   typedef TCatTrackResultPulse TrackHit_t;
   typedef std::vector<TrackHit_t*> TrackHitVec_t;
   
   TCatTpcTrack();
   virtual ~TCatTpcTrack();

   TCatTpcTrack(const TCatTpcTrack& rhs);
   TCatTpcTrack& operator=(const TCatTpcTrack& rhs);

   virtual void Copy(TObject& obj) const;

   void Clear(Option_t* opt = "");
   // initialize arrays
   void Init();


   // estimate properties from given hit cluster using charge-weighted centroid tracking
   void EstimateFromHits(const std::vector<art::TCatPulseShape*>& cluster);

   // calculate properties from given track
   void SetResults(const std::vector<art::TCatPulseShape*>& input, const TVector3& start, const TVector3& end,
                   TCatDiffusedChargeResponseHelper *helper);
   void SetPrincipalAxis(const TVector3& axis) { fPrincipalAxis = axis; }
   void SetDeviation(const TVector3& deviation) { fDeviation = deviation; }
   void SetInertia(const TVector3& inertia) { fInertia = inertia; }
   void SetTrack(const TVector3& start, const TVector3& end) {
      SetTrack(start[0],start[1],start[2],end[0],end[1],end[2]);
   }

   void SetTrack(double x1, double y1, double z1, double x2, double y2, double z2) {
      fStartPoint.SetXYZ(x1,y1,z1);
      fEndPoint.SetXYZ(x2,y2,z2);
      fDirection.SetXYZ(x2 - x1, y2 - y1, z2 - z1);
      fRange = fDirection.Mag();
   }

   void SetCharge(double charge) { fCharge = charge; }
   void SetEnergyDeposit(double energyDeposit) { fEnergyDeposit = energyDeposit; }
   void SetChi2(Double_t chi2) { fChi2 = chi2; }
   void SetStatus(Int_t status) { fStatus = status; }
   void SetEDM(Double_t edm) { fEDM = edm; }
   void SetChargeWeight(Double_t power) { fChargeWeightPower = power; }

   void UpdateRange() { fRange = (fEndPoint-fStartPoint).Mag(); }
   TVector3& GetOriginalStartPoint() { return fOriginalStartPoint; }
   TVector3& GetOriginalEndPoint() { return fOriginalEndPoint; }
   TVector3& GetStartPoint() { return fStartPoint; }
   TVector3& GetEndPoint() {return fEndPoint; }
   TVector3& GetDirection() { return fDirection; }
   const TVector3& GetOriginalStartPoint() const { return fOriginalStartPoint; }
   const TVector3& GetOriginalEndPoint() const { return fOriginalEndPoint; }
   const TVector3& GetStartPoint() const { return fStartPoint; }
   const TVector3& GetEndPoint()   const {return fEndPoint; }
   const TVector3& GetDirection()  const { return fDirection; }
   const TVector3& GetPrincipalAxis() const { return fPrincipalAxis; }
   const TVector3& GetDeviation() const { return fDeviation; }
   void GetStartPoint(double& x, double& y, double& z) const { x = fStartPoint[0]; y = fStartPoint[1]; z = fStartPoint[2]; }
   void GetEndPoint(double& x, double& y, double& z) const { x = fEndPoint[0]; y = fEndPoint[1]; z = fEndPoint[2]; }
   void GetDirection(double& x, double& y, double& z) const { x = fDirection[0]; y = fDirection[1]; z = fDirection[2]; }
   Double_t GetCharge() const { return fCharge; }
   Double_t GetRange() const {return fRange; }
   Double_t GetChi2() const { return fChi2; }
   const TVector3& GetInertia() const { return fInertia; }
   Double_t GetEnergyDeposit() const { return fEnergyDeposit; }
   const TrackHitVec_t& GetHits() const { return fHits; }
   TrackHitVec_t& GetHits() { return fHits;}

   Int_t GetStatus() const { return fStatus; }
   Double_t GetEDM() const { return fEDM; }

   virtual Double_t X(Double_t z=0) const { return fStartPoint.X() + (z - fStartPoint.Z())/fDirection.Z() * fDirection.X(); }
   virtual Double_t Y(Double_t z=0) const { return fStartPoint.Y() + (z - fStartPoint.Z())/fDirection.Z() * fDirection.Y(); }
   virtual Double_t A() const { return fDirection.X()/fDirection.Z(); }
   virtual Double_t B() const { return fDirection.Y()/fDirection.Z(); }
   


   TCatTrackResultPulse* NewHit() ;
protected:
   
   TVector3 fOriginalStartPoint; // track start point before update
   TVector3 fOriginalEndPoint; // 
    
   TVector3 fStartPoint; // track start point
   TVector3 fEndPoint; // track end point
   TVector3 fDirection; 
   
   TVector3 fPrincipalAxis; // principal axis of hits
   TVector3 fDeviation; // deviation of position from the principal axis
   TVector3 fProjectedLength; // length projected onto each principal axis
    

   Double_t fCharge; // sum of charge
   Double_t fEnergyDeposit; // sum of energy deposit

   Double_t fRange; // range 
   Double_t fChi2; // chi2
   TVector3 fInertia; // inertia

   Bool_t fIsInitialized; // initialized

   std::vector<TCatTrackResultPulse*> fHits;
   
   TClonesArray *fHitsHelper; //!-> tracking result hits

   Int_t fStatus; // minimization status
   Double_t fEDM; // estimated distance to minimum

   Double_t fChargeWeightPower; //!

private:

   ClassDef(TCatTpcTrack,3) // container for tracking result
};

#endif // INCLUDE_GUARD_UUID_DF45729E_1BB8_49A2_A2ED_968FE7E7ED47
