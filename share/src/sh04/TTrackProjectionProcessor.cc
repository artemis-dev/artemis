/**
 * @file   TTrackProjectionProcessor.cc
 * @brief  project track to another plane
 *
 * @date   Created       : 2014-12-08 19:24:47 JST
 *         Last Modified : 2015-04-10 15:28:24 JST (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 KAWASE Shoichiro
 */

#include "TTrackProjectionProcessor.h"

#include <TClonesArray.h>
#include <TMath.h>

#include <TMWDCTrackingResult.h>

using art::sh04::TTrackProjectionProcessor;

ClassImp(TTrackProjectionProcessor);

TTrackProjectionProcessor::TTrackProjectionProcessor()
   : fTrackIn(NULL), fZ(0.), fCenterX(0.), fTheta(0.)
{
   RegisterInputCollection("InputCollection","input track",
			   fInputColName,TString("track"),
			   &fTrackIn,TClonesArray::Class_Name(),TMWDCTrackingResult::Class_Name());
   RegisterOutputCollection("OutputCollection","output track",
			    fOutputColName,TString("track_out"),
			    &fTrackOut,TClonesArray::Class_Name(),TTrack::Class_Name());
   RegisterProcessorParameter("Z","dz",fZ,0.f);
   RegisterProcessorParameter("RotationCenterX","x",fCenterX,0.f);
   RegisterProcessorParameter("Theta","theta (deg.)",fTheta,0.f);
}

TTrackProjectionProcessor::~TTrackProjectionProcessor()
{
}

void TTrackProjectionProcessor::Init(TEventCollection*)
{
   fTan = TMath::Tan(fTheta * TMath::DegToRad());
}

void TTrackProjectionProcessor::Process()
{
   /* init */
   fTrackOut->Clear("C");

   const TMWDCTrackingResult *const track =
      static_cast<TMWDCTrackingResult*>((*fTrackIn)->At(0));
   if (!track) return;

   const Double_t x0 = track->X();
   const Double_t y0 = track->Y();
   const Double_t z0 = track->Z();
   const Double_t a0 = track->A();
   const Double_t b0 = track->B();
   const Double_t dx = track->X() - fCenterX;
   const Double_t dz = (dx * fTan + fZ) / (1 - a0 * fTan);

   const Double_t x = x0 + a0 * dz;
   const Double_t y = y0 + b0 * dz;
   const Double_t z = z0 + dz;

   TTrack *const out = static_cast<TTrack*>(fTrackOut->ConstructedAt(0));
   out->SetPos(x,y,z);
   out->SetAngle(a0,b0);
}
