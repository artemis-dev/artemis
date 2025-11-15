/**
 * @file   TReactionVertexProcessor.cc
 * @brief  calculate reaction vertex in SHARAQ03
 *
 * @date   Created       : 2014-03-25 17:02:40 JST
 *         Last Modified : 2015-04-10 15:32:04 JST (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 KAWASE Shoichiro
 */

#include "TReactionVertexProcessor.h"

#include <TClass.h>
#include <TClonesArray.h>
#include <TVector2.h>
#include <TMath.h>

#include "TMWDCTrackingResult.h"
#include "TDetectorGeometry.h"

using art::sh04::TReactionVertexProcessor;

ClassImp(TReactionVertexProcessor)

TReactionVertexProcessor::TReactionVertexProcessor()
   : fTrackL(NULL), fTrackR(NULL), fS0Track(NULL), fVertex(NULL) {
   RegisterInputCollection("TrackL",
			   "TMWDCTrackingResult for Recoil L",
			   fTrackLName,TString("smwdc_l"),
			   &fTrackL,TClonesArray::Class_Name(),TMWDCTrackingResult::Class_Name());
   RegisterInputCollection("TrackR",
			   "TMWDCTrackingResult for Recoil R",
			   fTrackRName,TString("smwdc_r"),
			   &fTrackR,TClonesArray::Class_Name(),TMWDCTrackingResult::Class_Name());
   RegisterInputCollection("S0Track",
			   "TMWDCTrackingResult for S0 image",
			   fS0TrackName,TString("s0_image"),
   			   &fS0Track,TClonesArray::Class_Name(),TMWDCTrackingResult::Class_Name());
   RegisterProcessorParameter("Geometry","Name of Geometry Configuration",
			      fGeometryName, TString("geometry"));
   RegisterOutputCollection("OutputCollection","output",
			    fOutputColName,TString("vertex"),
			    &fVertex,TClonesArray::Class_Name(),TTrack::Class_Name());
}

TReactionVertexProcessor::~TReactionVertexProcessor()
{
   delete fVertex;
}

void TReactionVertexProcessor::Init(TEventCollection *col)
{
   { /* Load Geometry */
      const TDetectorGeometry *const geo =
	 reinterpret_cast<TDetectorGeometry*>(col->GetInfo(fGeometryName));
      if(!geo) {
	 Error("Init","geometry not found: %s",fGeometryName.Data());
	 return;
      }
      fDistance[0] = geo->GetMWDCDistance(0);
      fDistance[1] = geo->GetMWDCDistance(1);
      fAngle[0] = geo->GetMWDCAngle(0);
      fAngle[1] = geo->GetMWDCAngle(1);
   }

   Info("Init","Track: %s, %s, S0: %s, Geometry: %s, Vertex: %s",
	fTrackLName.Data(),fTrackRName.Data(),fS0TrackName.Data(),
	fGeometryName.Data(),fOutputColName.Data());
}

namespace {
inline Double_t Cross(const TVector2 &a, const TVector2 &b) {
   return a.X() * b.Y() - a.Y() * b.X();
}

inline Double_t CalcZ(Double_t x, Double_t a, Double_t da,
	       Double_t x0, Double_t a0,
	       Double_t distance, Double_t angle, Double_t *w)
{
   TVector2 xr = TVector2(distance,x).Rotate(angle * TMath::DegToRad());
   const TVector2 pr = TVector2(1.,a).Rotate(angle * TMath::DegToRad());

   const TVector2 pb(1.,-a0); // BLD coordinate is left-handed
   const TVector2 xb(0.,-x0);
   xr -= xb;

   const Double_t beamz = Cross(pr,xr) / Cross(pr,pb);

   const Double_t dz = distance / Cross(pr,pb) * da;
   *w = dz * dz;

   return beamz;
}
}

void TReactionVertexProcessor::Process()
{
   /* initialization */
   fVertex->Clear("C");

   if(!(*fTrackL)->GetEntriesFast()
      || !(*fTrackR)->GetEntriesFast()
      || !(*fS0Track)->GetEntriesFast()) {
      return;
   }

   const TMWDCTrackingResult *const trackL =
      static_cast<TMWDCTrackingResult*>((*fTrackL)->At(0));
   const TMWDCTrackingResult *const trackR =
      static_cast<TMWDCTrackingResult*>((*fTrackR)->At(0));
   const TMWDCTrackingResult *const trackS0 =
      static_cast<TMWDCTrackingResult*>((*fS0Track)->At(0));

   TTrack *const vertex = static_cast<TTrack*>(fVertex->ConstructedAt(0));

   Double_t lw, rw;
   const Double_t beamz_l = CalcZ(trackL->X(), trackL->A(), trackL->GetSigma(1),
				  trackS0->X(), trackS0->A(),
				  fDistance[0], fAngle[0], &lw);
   const Double_t beamz_r = CalcZ(trackR->X(), trackR->A(), trackR->GetSigma(1),
				  trackS0->X(), trackS0->A(),
				  fDistance[1], fAngle[1], &rw);

   const Double_t beamz = (beamz_l * rw + beamz_r * lw)/(lw + rw);

   vertex->SetPos(- trackS0->X() - beamz * trackS0->A(), // BLD coordinate is left-handed
		  trackS0->Y() + beamz * trackS0->B(),
		  beamz);
   vertex->SetAngle(-trackS0->A(), trackS0->B()); // BLD coordinate is left-handed
}
