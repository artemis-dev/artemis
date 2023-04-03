/**
 * @file   TS1PIDProcessor.cc
 * @brief  processor for S1 PID
 *
 * @date   Created       : 2015-04-24 11:01:03 JST
 *         Last Modified : 2015-04-25 18:14:30 JST (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2015 KAWASE Shoichiro
 */

#include "TS1PIDProcessor.h"

#include <TClonesArray.h>
#include <TMath.h>

#include <TTimingData.h>
#include <TTimingChargeData.h>
#include <TTrack.h>
#include <TParticleIdentifier.h>

using art::sh04::TS1PIDProcessor;

ClassImp(TS1PIDProcessor)

TS1PIDProcessor::TS1PIDProcessor()
   : fPIDOut(NULL), fTOFOut(NULL), fS1TrackOut(NULL) {
   RegisterInputCollection("S0Track","S0 track",
			   fS0TrackName,TString("s0_image"),
			   &fS0TrackArray,TClonesArray::Class_Name(),TTrack::Class_Name());
   RegisterInputCollection("S0DTrack","S0D track",
			   fS0DTrackName,TString("s0d_image"),
			   &fS0DTrackArray,TClonesArray::Class_Name(),TTrack::Class_Name());
   RegisterInputCollection("S1Track","S1 track",
			   fS1TrackName,TString("s1_image"),
			   &fS1TrackArray,TClonesArray::Class_Name(),TTrack::Class_Name());
   RegisterInputCollection("TOF","TOF collection",
			   fTOFColName,TString("tof_s0s1"),
			   &fTOFArray,TClonesArray::Class_Name(),ITiming::Class_Name());
   RegisterInputCollection("Nyoki","Nyoki collection (charge should be corrected to Z)",
			   fNyokiName,TString("nyoki_z"),
			   &fNyokiArray,TClonesArray::Class_Name(), TTimingChargeData::Class_Name());
   RegisterOutputCollection("PIDOut","output for pid",
			    fOutputPIDName,TString("pid_s1"),
			    &fPIDOut,TParticleIdentifier::Class_Name());
   RegisterOutputCollection("TOFOut","output for corrected tof",
			    fOutputTOFName,TString("tof_c"),
			    &fTOFOut,TTimingData::Class_Name());
   RegisterOutputCollection("S1TrackOut","output for corrected S1 track",
			    fOutputS1TrackName,TString("s1_image_c"),
			    &fS1TrackOut,TTrack::Class_Name());
}

TS1PIDProcessor::~TS1PIDProcessor()
{
   delete fPIDOut;
   delete fTOFOut;
   delete fS1TrackOut;
}

void TS1PIDProcessor::Init(TEventCollection *) {}

namespace {
   Double_t kTOF_S0DX  = -0.0044;
   Double_t kS1X_F     = -26.6;
   Double_t kS1X_ALPHA = -0.004874;
   Double_t kAOQ_TOF   = 5.275e-2;
   Double_t kAOQ_S1X   = -2.198e-3;
   Double_t kAOQ_CONST = 0.89;
   Double_t kTOF_ORIGIN = 35.; // used for Z-TOF correction
   Double_t kZ_TOF     = 0.002;
}

void TS1PIDProcessor::Process()
{
   /* output initialization */
   fPIDOut->Clear();
   fTOFOut->Clear();
   fS1TrackOut->Clear();

   /* read input */
   if ( !(*fTOFArray)->GetEntriesFast()
	|| !(*fS1TrackArray)->GetEntriesFast() ) {
      return;
   }
   const Double_t tof_s0s1 = dynamic_cast<ITiming*>((*fTOFArray)->At(0))->GetTiming();
   const Double_t s1x = static_cast<TTrack*>((*fS1TrackArray)->At(0))->X();

   const Double_t nyoki_z = static_cast<TTimingChargeData*>((*fNyokiArray)->At(0))->GetCharge();

   const TTrack *s0d =
      (*fS0DTrackArray)->GetEntriesFast() ? static_cast<TTrack*>((*fS0DTrackArray)->At(0)) : NULL;
   const Double_t s0dx = (s0d && TMath::Finite(s0d->X())) ? s0d->X() : 0.;

   /* calc corected values */
   const Double_t tof_corrected = tof_s0s1 + kTOF_S0DX * s0dx;
   const Double_t s1x_corrected = (s1x + kS1X_ALPHA * kS1X_F * s0dx) / (1 + kS1X_ALPHA * s0dx);

   const Double_t aoq = kAOQ_TOF * tof_corrected + kAOQ_S1X * s1x_corrected + kAOQ_CONST;
   const Double_t z_corrected = nyoki_z * (1 - (tof_corrected - kTOF_ORIGIN) * kZ_TOF * nyoki_z);

   /* output */
   fTOFOut->SetTiming(tof_corrected);
   fS1TrackOut->SetX(s1x_corrected);
   fPIDOut->SetAOQ(aoq);
   fPIDOut->SetZ(z_corrected);
}
