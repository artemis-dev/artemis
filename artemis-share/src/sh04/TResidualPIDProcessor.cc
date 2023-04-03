/**
 * @file   TResidualPIDProcessor.cc
 * @brief  processor for residual PID
 *
 * @date   Created       : 2014-12-25 16:39:51 JST
 *         Last Modified : 2015-04-10 15:30:48 JST (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 KAWASE Shoichiro
 */

#include "TResidualPIDProcessor.h"

#include <TMath.h>
#include <TClonesArray.h>

#include <TMWDCTrackingResult.h>
#include <TTimingChargeData.h>
#include <TParticleIdentifier.h>
#include <TGenericData.h>
#include <TBeamParameter.h>

using art::sh04::TResidualPIDProcessor;

ClassImp(TResidualPIDProcessor)

TResidualPIDProcessor::TResidualPIDProcessor()
{
   RegisterInputCollection("ChargeCollection","charge input",
			   fChargeColName,TString("nyoki_t"),
			   &fChargeArray,TClonesArray::Class_Name(),ICharge::Class_Name());
   RegisterInputCollection("TOFCollection","TOF input",
			   fTOFColName,TString("tof"),
			   &fTOFArray,TClonesArray::Class_Name(),ITiming::Class_Name());
   RegisterInputCollection("MWDCCollection","MWDC input",
			   fMWDCColName,TString("mwdc"),
			   &fMWDCArray,TClonesArray::Class_Name(),TMWDCTrackingResult::Class_Name());
   RegisterOutputCollection("PIDOut","output for pid",
			    fPIDName,TString("pid"),
			    &fPID,TParticleIdentifier::Class_Name());
   RegisterOutputCollection("FlightLengthOut","output for flight length",
			    fFlightLengthName,TString("fl"),
			    &fFlightLength,TGenericData::Class_Name());
   RegisterProcessorParameter("Parameter1","Z = beta * sqrt(L / \"p1\" - p2 * L)",fP1,1.f);
   RegisterProcessorParameter("Parameter2","Z = beta * sqrt(L / p1 - \"p2\" * L)",fP2,0.f);
   RegisterProcessorParameter("Dispersion","dispersion",fDispersion,-1000.f);
   RegisterProcessorParameter("BeamParameter","name of beam parameter",
			      fBeamParameterName,TString("beam"));
}

TResidualPIDProcessor::~TResidualPIDProcessor() {}

void TResidualPIDProcessor::Init(TEventCollection *col)
{
   { /* Load beam parameter fron UserInfo */
      const TBeamParameter *const beam = dynamic_cast<TBeamParameter*>(col->GetInfo(fBeamParameterName));
      if (!beam) {
	 SetStateError(TString::Format("No TBeamParameter named '%s' found", fBeamParameterName.Data()));
	 return;
      }
      fBrho = beam->GetBrho();
   }
}

namespace {
   const Double_t AMU = 931.5;           // atomic mass unit (MeV/c^2)
   const Double_t C = TMath::C() * 1e-6; // light velocity (mm/ns)
}

void TResidualPIDProcessor::Process()
{
   /* output initialization */
   fPID->Clear();
   fFlightLength->Clear();

   /* read input */
   if ( !(*fTOFArray)->GetEntriesFast() || !(*fChargeArray)->GetEntriesFast()) return;
   const Double_t tof = dynamic_cast<ITiming*>((*fTOFArray)->At(0))->GetTiming();
   const Double_t charge = dynamic_cast<ICharge*>((*fChargeArray)->At(0))->GetCharge();
   const Double_t s1x = static_cast<TMWDCTrackingResult*>((*fMWDCArray)->At(0))->X();

   /* flight length (mm) */
   const Double_t fl = 2.367e-4 * s1x * s1x - 4.049e-1 * s1x + 6.297e+3;

   /* Lorentz factors of residual particle */
   const Double_t beta = fl / C / tof;
   const Double_t gamma = 1. / TMath::Sqrt(1 - beta * beta);

   /* proton number */
   const Double_t z = beta * TMath::Sqrt(charge / (fP1 - fP2 * charge));

   /* brho (Tm) */
   const Double_t brho = fBrho * (1. + s1x / fDispersion);

   /* A/Q */
   const Double_t aoq = C * brho / AMU / gamma / beta;

   /* output */
   fPID->SetZ(z);
   fPID->SetBrho(brho);
   fPID->SetAOQ(aoq);
   fFlightLength->SetID(0);
   fFlightLength->Set(fl);
}
