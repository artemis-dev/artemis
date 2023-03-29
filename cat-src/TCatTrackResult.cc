/**
 * @file   TCatTrackResult.cc
 * @brief  container for tracking result of CAT
 *
 * @date   Created       : 2016-07-24 12:41:44 JST
 *         Last Modified : 2018-07-18 23:55:21 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2016 Shinsuke OTA
 */

#include "TCatTrackResult.h"
#include <TClonesArray.h>
#include <TCatTrackResultPulse.h>
#include <TMath.h>
#include <TCatPulseShape.h>
#include <TCatReadoutPadArray.h>
#include <TCatReadoutPad.h>
#include <TCatDiffusedChargeResponseHelper.h>
#include <TRangeTableHelper.h>
#include <TGraph.h>

using art::TCatTrackResult;

ClassImp(TCatTrackResult)

TCatTrackResult::TCatTrackResult()
{
   fHitsHelper = NULL;
}

TCatTrackResult::~TCatTrackResult()
{
   if (fHitsHelper) delete fHitsHelper;
}

TCatTrackResult::TCatTrackResult(const TCatTrackResult& rhs)
{
}

TCatTrackResult& TCatTrackResult::operator=(const TCatTrackResult& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}

void TCatTrackResult::Init()
{
   if (!fHitsHelper) {
      fHitsHelper = new TClonesArray(TCatTrackResultPulse::Class_Name());
   }
}

void TCatTrackResult::Clear(Option_t *opt)
{
   fHits.clear();
   fHitsHelper->Clear(opt);
   fStartPoint.SetXYZ(TMath::QuietNaN(),TMath::QuietNaN(),TMath::QuietNaN());
   fEndPoint.SetXYZ(TMath::QuietNaN(),TMath::QuietNaN(),TMath::QuietNaN());
   fRange = TMath::QuietNaN();
   fChi2 = TMath::QuietNaN();
}

art::TCatTrackResultPulse* TCatTrackResult::CreateAndGetPulse()
{
   TCatTrackResultPulse *pulse = static_cast<TCatTrackResultPulse*>(fHitsHelper->ConstructedAt(fHitsHelper->GetEntriesFast()));
   fHits.push_back(pulse);
   return pulse;
}

void TCatTrackResult::CalculateResults(const std::vector<TCatPulseShape*>& input)
{
   if (!fResponseHelper) {
      Warning("CalculateResults","no response helper is ready");
      return;
   }

   if (!fReadoutPadArray) {
      Warning("CalculateResults","no readout pad is ready");
      return;
   }
   Double_t e10 = 0;
   Double_t e30 = 0;
   Double_t r10max = 0;
   Double_t r30max = 0;
   Double_t minRangeToEnd = TMath::Limits<Double_t>::Max();
   const TRangeTableHelper *fRangeTableHelper = fResponseHelper->GetRangeTableHelper();
   *(TVector3*)this = fEndPoint - fStartPoint;

   const Int_t nHits = input.size();
   for (Int_t iHit = 0; iHit < nHits; ++iHit) {
      Double_t charge;
      Double_t closest;
      Double_t angle;
      Double_t rangeToEnd;
      Double_t trackLength = 0.;
      Double_t dedx = TMath::QuietNaN();
      Int_t num;
      
      const TCatPulseShape* const pulse = input[iHit];
      const TCatReadoutPad* const pad = (TCatReadoutPad*) (fReadoutPadArray)->At(pulse->GetID());
      TCatTrackResultPulse *out = CreateAndGetPulse();
      const Int_t id = pad->GetID();
      std::vector<TVector3> intersection;
      fResponseHelper->GetResponse(id,charge,closest,angle, rangeToEnd, trackLength);
      pad->GetIntersection(fStartPoint,fEndPoint,intersection);
      num = intersection.size();

      if (rangeToEnd < minRangeToEnd) minRangeToEnd = rangeToEnd;
      switch (num) {
      case 2: 
         trackLength = (intersection[0]-intersection[1]).Mag();
         {
            Double_t r1 = (fEndPoint - intersection[0]).Mag();
            Double_t r2 = (fEndPoint - intersection[1]).Mag();
            dedx = TMath::Abs(fRangeTableHelper->GetR2E()->Eval(r1) - fRangeTableHelper->GetR2E()->Eval(r2));
            
            if (r1 < 10 || r2 < 10.) {
               if (r10max < r1) r10max = r1;
               if (r10max < r2) r10max = r2;
               e10 += charge;
            }
            if (r1 < 30 || r2 < 30.) {
               if (r30max < r1) r30max = r1;
               if (r30max < r2) r30max = r2;
               e30 += charge;
            }
         }
         break;
      case 1:
         if (!pad->IsInside(fEndPoint.X(),fEndPoint.Z())) break;
         trackLength = (intersection[0] - fEndPoint).Mag();
         dedx = fRangeTableHelper->GetR2E()->Eval(trackLength);
         if (r10max < trackLength) r10max = trackLength;
         if (r30max < trackLength) r30max = trackLength;
         e10 += charge;
         e30 += charge;
         break;
      default:
         break;
      }   
      pulse->Copy(*out);
      out->SetEnergyDeposit(charge);
      out->SetTrackLength(trackLength);
      out->SetDEDX(dedx);
      out->SetAngle(angle);
      out->SetDistance(closest);
      out->SetRange(rangeToEnd);
   }
   SetDEDX10(e10/r10max);
   SetDEDX30(e30/r30max);
   SetCharge10(e10);
   SetCharge30(e30);
   SetRange10(r10max);
   SetRange30(r30max);
   SetDistanceToEnd(minRangeToEnd);

}

