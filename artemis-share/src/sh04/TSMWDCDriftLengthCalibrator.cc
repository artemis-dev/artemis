/**
 * @file   TSMWDCDriftLengthCalibrator.cc
 * @brief
 *
 * @date   Created:       2013-12-25 13:41:41
 *         Last Modified: 2015-04-10 15:28:53 JST (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved.
 */

#include "TSMWDCDriftLengthCalibrator.h"

#include <TClonesArray.h>
#include <TVector3.h>
#include <TMath.h>

#include "TMWDCTrackingResult.h"
#include "TMWDCHitData.h"
#include "TMWDCTrackingResult.h"
#include "TMWDCParameter.h"
#include "TMWDCPlaneInfo.h"
#include "constant.h"

using art::sh04::TSMWDCDriftLengthCalibrator;

ClassImp(art::sh04::TSMWDCDriftLengthCalibrator)

// Default constructor
TSMWDCDriftLengthCalibrator::TSMWDCDriftLengthCalibrator()
{
   RegisterInputCollection("InputCollection","TMWDCTrackingResult",
			   fInputColName,TString("track"),
                           &fTrackingResult,"TClonesArray","art::TMWDCTrackingResult");
   RegisterOutputCollection("OutputCollection","output",
			    fOutputColName,TString("smwdc_x1_c"));
   RegisterProcessorParameter("PlaneData","plane data",
			      fPlaneName,TString("smwdc_x1"));
}

TSMWDCDriftLengthCalibrator::~TSMWDCDriftLengthCalibrator()
{
   delete fPlaneDataOut;
   delete fPos;
}

void TSMWDCDriftLengthCalibrator::Init(TEventCollection *col)
{
/* load planeinfo */
   {
      const Int_t delimIndex = fPlaneName.Last('_');
      const TString &mwdcname = fPlaneName(0,delimIndex);
      const TString &planename = fPlaneName(delimIndex+1,fPlaneName.Length());

      TMWDCParameter *const mwdcprm =
	 reinterpret_cast<TMWDCParameter*>(col->GetInfo(mwdcname));

      if (!mwdcprm) {
	 Error("Init","mwdc parameter object '%s' not found",mwdcname.Data());
	 return;
      }

      TMWDCPlaneInfo *const planeInfo = mwdcprm->GetPlane(planename, kTRUE);

      if (!planeInfo) {
	 Error("Init","info for plane '%s' not found",planename.Data());
	 return;
      }

      fZ        = planeInfo->GetZ();
      fCos      = planeInfo->Cos();
      fSin      = planeInfo->Sin();
      fCenter   = planeInfo->GetCenter();
      fCellSize = planeInfo->GetCellSize();
   }

/* load planedata */
   fPlaneDataIn =
      reinterpret_cast<TClonesArray**>(col->GetObjectRef(fPlaneName));

/* new output TClonesArray */
   fPlaneDataOut = new TClonesArray(TMWDCHitData::Class());
   fPlaneDataOut->SetName(fOutputColName);
   col->Add(fOutputColName,fPlaneDataOut,fOutputIsTransparent);

   fPos = new TVector3;
}

void TSMWDCDriftLengthCalibrator::Process()
{
   /* initialization */
   fPlaneDataOut->Clear("C");

   Double_t angleMult = 1.;
   { /* calculate position at the plane */
      const Int_t nHit = (*fTrackingResult)->GetEntriesFast();
      if (!nHit) return;
      const TMWDCTrackingResult *track = static_cast<TMWDCTrackingResult*>((*fTrackingResult)->At(0));
      track->CalculatePosAt(fZ,fPos);
      const Double_t dot = track->GetA() * fCos + track->GetB() * fSin;
      angleMult /= TMath::Sqrt(1 + dot*dot);
   }

   /* calculate driftlength from the position and planeinfo */
   const Double_t x = fPos->X();
   const Double_t y = fPos->Y();

   const Double_t wireID  = (x * fCos + y * fSin) / fCellSize + fCenter;
   const Int_t    wireIDI = TMath::Nint(wireID);

   const Double_t dl = (wireID - wireIDI) * fCellSize;
   const Int_t   wID = wireIDI;

   /* fill drift time, wire ID, driftlength */
   const Int_t nHit = (*fPlaneDataIn)->GetEntriesFast();
   for (Int_t iHit = 0; iHit != nHit; ++iHit) {
      const TMWDCHitData *const hit =
	 static_cast<TMWDCHitData*>((*fPlaneDataIn)->At(iHit));
      const Int_t wireIDIn = hit->GetDetID();
      if (wireIDIn != wID) continue;
      if (hit->GetCharge() < 750) continue;

      {
	 const Int_t nOut = fPlaneDataOut->GetEntriesFast();
	 TMWDCHitData *const out =
	    static_cast<TMWDCHitData*>(fPlaneDataOut->ConstructedAt(nOut));
	 hit->Copy(*out);
//	 out->SetDriftLength(dl * angleMult);
	 out->SetDriftLength(dl);
      }
   }
}
