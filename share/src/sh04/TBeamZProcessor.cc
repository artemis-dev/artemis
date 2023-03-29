/**
 * @file   TBeamZProcessor.cc
 * @brief
 *
 * @date   Created:       2014-02-25 10:49:25
 *         Last Modified: 2015-04-10 15:27:15 JST (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2014 KAWASE Shoichiro All rights reserved.
 */

#include "TBeamZProcessor.h"
#include <TClonesArray.h>
#include <TClass.h>
#include <TMath.h>

#include "TDetectorGeometry.h"
#include "TBeamZ.h"
#include "TMWDCTrackingResult.h"
#include "constant.h"

using art::sh04::TBeamZProcessor;

ClassImp(TBeamZProcessor)

// Default constructor
TBeamZProcessor::TBeamZProcessor()
   : fTrackL(NULL), fTrackR(NULL) {
   RegisterInputCollection("TrackL","TMWDCTrackingResult for SMWDC-L",
			   fTrackLName,TString("smwdc_l"),
			   &fTrackL, TClonesArray::Class_Name(),TMWDCTrackingResult::Class_Name());
   RegisterInputCollection("TrackR","TMWDCTrackingResult for SMWDC-L",
			   fTrackRName,TString("smwdc_r"),
			   &fTrackR, TClonesArray::Class_Name(),TMWDCTrackingResult::Class_Name());
   RegisterProcessorParameter("Geometry","Name of Geometry Configuration",
			      fGeometryName, TString("geometry"));
   RegisterProcessorParameter("Averaging Method","0: use better one, 1: arithmetic avg., 2: weighted avg.",
			      fAveragingMethod, 2);
   RegisterOutputCollection("OutputCollection","output",
			    fOutputColName,TString("BeamZ"),
			    &fBeamZ,TClonesArray::Class_Name(),TBeamZ::Class_Name());
}

TBeamZProcessor::~TBeamZProcessor()
{
   delete fBeamZ;
}

void TBeamZProcessor::Init(TEventCollection *col)
{
   { /* Load Geometry */
      const TDetectorGeometry *const geo =
	 reinterpret_cast<TDetectorGeometry*>(col->GetInfo(fGeometryName));
      if(!geo) {
	 SetStateError(TString::Format("geometry not found: %s",fGeometryName.Data()));
	 return;
      }
      fDistance[0] = geo->GetMWDCDistance(0);
      fDistance[1] = geo->GetMWDCDistance(1);
      fAngle[0] = geo->GetMWDCAngle(0);
      fAngle[1] = geo->GetMWDCAngle(1);
   }

   Info("Init","Track L: %s, R: %s, Geometry: %s, BeamZ: %s",
	fTrackLName.Data(),fTrackRName.Data(),
	fGeometryName.Data(),fOutputColName.Data());
}

void TBeamZProcessor::Process()
{
   /* initialization */
   fBeamZ->Clear("C");

   if(!(*fTrackL)->GetEntriesFast() || !(*fTrackR)->GetEntriesFast()) {
      return;
   }

   const TMWDCTrackingResult *const trackL =
      static_cast<TMWDCTrackingResult*>((*fTrackL)->At(0));
   const TMWDCTrackingResult *const trackR =
      static_cast<TMWDCTrackingResult*>((*fTrackR)->At(0));

   TBeamZ *const beamZ = static_cast<TBeamZ*>(fBeamZ->ConstructedAt(0));

   Double_t beamZL, beamZR, errL, errR;
   CalcBeamZ(fDistance[0], fAngle[0], trackL->X(), trackL->A(),
	     trackL->GetSigma(0),trackL->GetSigma(1), &beamZL, &errL);
   CalcBeamZ(fDistance[1], fAngle[1], trackR->X(), trackR->A(),
	     trackR->GetSigma(0),trackR->GetSigma(1), &beamZR, &errR);
   beamZ->SetL(beamZL);
   beamZ->SetR(beamZR);

   const Double_t average = CalcAverage(beamZL, beamZR, errL, errR);
   beamZ->SetAverage(average);
}

void TBeamZProcessor::CalcBeamZ(Double_t distance, Double_t angle,
	       Double_t x, Double_t a, Double_t xerr, Double_t aerr,
				Double_t *beamZ, Double_t *error) const
{
   const Double_t c = TMath::Cos(angle * TMath::DegToRad());
   const Double_t s = TMath::Sin(angle * TMath::DegToRad());

   const Double_t alpha = 1./(a*c-s);
   const Double_t xi    = x*c - distance*s;

   *beamZ = distance*c + x*s - alpha * xi * (c + a*s);
   const Double_t dzdx = s - c * alpha;
   const Double_t dzda = alpha * xi * (alpha * c * (c + a*s) - s);

   *error = TMath::Sqrt( (dzdx * dzdx * xerr * xerr)
			 + (dzda * dzda * aerr * aerr));
}

Double_t TBeamZProcessor::CalcAverage(Double_t l, Double_t r, Double_t le, Double_t re)
{
   switch(fAveragingMethod) {
      case 0: // take better one
	 return (le < re) ? l : r;
      case 1: // arithmetic average
	 return (l + r) * 0.5;
      case 2: // weighted average
	 {
	 const Double_t lw = le * le; // inverse of weight
	 const Double_t rw = re * re;
	 return (l*rw + r*lw)/(lw+rw);
	 }
      default:
	 return kInvalidD;
   }
}
