/**
 * @file   TRecoilProcessor.cc
 * @brief
 *
 * @date   Created:       2013-12-09 17:21:25
 *         Last Modified: 2015-04-13 12:46:55 JST (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved.
 */

#include "sh04/TRecoilProcessor.h"

#include <fstream>
#include <algorithm>
#include <functional>

#include <TLorentzVector.h>
#include <TClonesArray.h>
#include <TSpline.h>

#include "ITiming.h"
#include "TTrack.h"
#include "constant.h"
#include "TDetectorGeometry.h"

using art::sh04::TRecoilProcessor;

ClassImp(art::sh04::TRecoilProcessor)

namespace {
   const Double_t kBrhoCoefficient = 1. / 299.792;
   const Double_t kMagnetField     = 0.064; // B_y [T] of polarization magnet
   const Double_t kFieldRadius     = 0.11;  // radius of magnet field
}

// Default constructor
TRecoilProcessor::TRecoilProcessor()
   : fRecoilOut(NULL), fMWDCDistance(kInvalidD), fMWDCAngle(kInvalidD), fMWDCYOffset(0.),
     fStopDetDistance(kInvalidD), fStopDetAngle(kInvalidD),
     fP(NULL), fStopDetNormal(NULL)
{
   RegisterOutputCollection("OutputCollection","output",
			    fOutputColName,TString("recoil"),
			    &fRecoilOut,TLorentzVector::Class_Name());
   RegisterInputCollection("TrackCollection","output from TMWDCTrackingProcessor",
			   fTrackColName,TString("track"),
			   &fTrack,TClonesArray::Class_Name(),TTrack::Class_Name());
   RegisterInputCollection("TOFCollection","tof between target and stop detector",
			   fTOFColName,TString("tof"),
			   &fTOF,TClonesArray::Class_Name(),ITiming::Class_Name());
   RegisterProcessorParameter("S0Track","output from TMWDCTrackingResult",
			      fS0TrackName,TString("s0track"));
   RegisterProcessorParameter("Geometry","TDetectorGeometry output from TParameterLoader",
			      fParameterName,TString("geometry"));
   RegisterProcessorParameter("Side","0: Left (default), non 0: Right",fSide,0);
   RegisterProcessorParameter("BeamZ","reaction point",fBeamZ,0.f);
   RegisterProcessorParameter("AngleShift","shift of scattering angle",fAngleShift,0.f);
   RegisterProcessorParameter("Vertex","reaction vertex",fVertexName,TString("vertex"));
   RegisterProcessorParameter("UseReactionPointCorrection","reaction point correction",
			      fUseReactionPointCorrection,kFALSE);
   RegisterProcessorParameter("UseIncidentAngleCorrection","incident angle correction",
			      fUseIncidentAngleCorrection,kFALSE);
   RegisterProcessorParameter("UsePolarizedMagnetCorrection","polarized magnet bending correction",
			      fUsePolarizedMagnetCorrection,kFALSE);
   RegisterProcessorParameter("UseTable","use table for TOF->E conversion",
			      fUseTable,kFALSE);
   RegisterProcessorParameter("TableFile","input file for TOF->E conversion table",
			      fTableFile,TString("path/to/file"));
}

TRecoilProcessor::~TRecoilProcessor()
{
   delete fRecoilOut;
   delete fP;
   delete fStopDetNormal;

   for (Int_t i = 0, n = fSplines.size(); i != n; ++i) {
      delete fSplines.at(i);
   }
}

void TRecoilProcessor::Init(TEventCollection *col)
{
   fS0Track = reinterpret_cast<TClonesArray**>(col->GetObjectRef(fS0TrackName.Data()));
   fVertex = reinterpret_cast<TClonesArray**>(col->GetObjectRef(fVertexName.Data()));

   { /* Load geometry from UserInfo */
      const TDetectorGeometry *const geo = dynamic_cast<TDetectorGeometry*>(col->GetInfo(fParameterName));
      if(!geo) {
	 SetStateError(TString::Format("No TDetectorGeometry named '%s' found", fParameterName.Data()));
	 return;
      }
      fMWDCDistance    = geo->GetMWDCDistance(fSide);
      fMWDCAngle       = geo->GetMWDCAngle(fSide);
      fMWDCYOffset     = geo->GetMWDCYOffset(fSide);
      fStopDetDistance = geo->GetPlasticDistance(fSide);
      fStopDetAngle    = geo->GetPlasticAngle(fSide);
   }

   if (fUseTable){
      LoadTable(fTableFile);
   } else {
      fStopDetNormal = new TVector3;
      const Double_t stopDetDistance = fStopDetDistance - fBeamZ * TMath::Cos(TMath::DegToRad() * fStopDetAngle);
      fStopDetNormal->SetMagThetaPhi(stopDetDistance,TMath::DegToRad() * fStopDetAngle, 0.);
   }

   fP = new TVector3;
   fM = P_MASS;
}

Bool_t TRecoilProcessor::LoadTable(const TString &filename)
{
   std::ifstream ifs(filename.Data());
   if (ifs.fail()) {
      Info("LoadTable","Failed to load file: %s",
	   filename.Data());
   }

   std::string str;

   Int_t nTheta = 0, nTOF = 0;
   // read number of theta, TOF
   while (ifs && std::getline(ifs,str,'\n')) {
      const Int_t nInput = std::sscanf(str.c_str(), "%d %d %*s", &nTheta, &nTOF);
      if (nInput == 2) break;
   }
   if (nTheta < 1 || nTOF < 1) {
      SetStateError("Invalid Input for nTheta, nTOF.");
      return kFALSE;
   }
   Info("LoadTable","nTheta, nTOF = %d, %d", nTheta, nTOF);


   std::vector<Double_t> xVec, yVec;
   for (Int_t iTheta = 0; iTheta != nTheta; ++iTheta) {
      Float_t theta = kInvalidD;

      while (ifs && std::getline(ifs,str,'\n')) {
	 const Int_t nInput = std::sscanf(str.c_str(), "%f %*s", &theta);
	 if (nInput == 1) break;
      }
      if (!ifs) {
	 SetStateError("Unexpected end of file.");
	 return kFALSE;
      }

      if (!fThetas.empty() && !(theta > fThetas.back())) {
	 SetStateError("TOF table should be monotonic on theta.");
	 return kFALSE;
      }
      fThetas.push_back(theta);

      xVec.clear();
      yVec.clear();
      while (ifs && std::getline(ifs,str,'\n')) {
	 Float_t tof, ke;
	 const Int_t nInput = std::sscanf(str.c_str(), "%f %f %*s", &tof, &ke);
	 if (nInput != 2) continue;
	 if (!xVec.empty() && !(tof > xVec.back())) {
	    SetStateError("TOF should be monotonically increasing.");
	    return kFALSE;
	 }
	 xVec.push_back(tof);
	 yVec.push_back(ke);
	 if ((Int_t)xVec.size() == nTOF) break;
      }
      if ((Int_t)xVec.size() < nTOF) {
	 SetStateError("Unexpected end of file.");
	 return kFALSE;
      }

      fSplines.push_back(new TSpline3(TString::Format("tof_ke_%d",(Int_t)theta).Data(),
				      &xVec.front(),&yVec.front(),nTOF));
   }

   return kTRUE;
}

void TRecoilProcessor::Process()
{
   /* initialization */
   fRecoilOut->SetXYZT(kInvalidD,kInvalidD,kInvalidD,kInvalidD);

   const TTrack *const track = static_cast<TTrack*>((*fTrack)->At(0));
   if ( !(*fTOF)->GetEntriesFast() ) return;
   const ITiming *const tof = dynamic_cast<ITiming*>((*fTOF)->At(0));

   if (!track || !tof || !IsValid(tof->GetTiming())) return;

   {
      const Double_t x = track->GetX();
      const Double_t y = track->GetY() + fMWDCYOffset;
      const Double_t z = fMWDCDistance;
      if (!IsValid(x+y+z)) return;
      fP->SetXYZ(x,y,z);
   }

   fP->RotateY(TMath::DegToRad() * fMWDCAngle); // rotation from the MWDC local to the Lab frame

   if (fVertex) {
      CorrectionWithVertex();
   } else if (fS0Track) {
      CorrectionWithS0Track();
   }

   const Double_t p =
      fUseTable ? CalcMomentumWithTable(tof,fP->Theta(),fP->Phi()) : CalcMomentum(tof);
   if(!IsValid(p)) return;

   fP->SetMag(p);

   fRecoilOut->SetVectM(*fP,P_MASS);

   /* polarized magnet bending correction */
   if (fUsePolarizedMagnetCorrection) {
      const Double_t pBrho = kBrhoCoefficient * P_MASS * fRecoilOut->Beta() * fRecoilOut->Gamma();
      const Double_t dTheta = kMagnetField * kFieldRadius / pBrho;
      fRecoilOut->RotateY(dTheta);
   }

   fRecoilOut->RotateY(fAngleShift);
}

void TRecoilProcessor::CorrectionWithS0Track()
{
   (*fP)[2] -= fBeamZ;

   const TTrack *const s0tr = static_cast<TTrack*>((*fS0Track)->At(0));

   /* reaction point correction */
   if (fUseReactionPointCorrection) {
      const Double_t s0x = -s0tr->GetX(); // BLD coordinate is left-handed
      const Double_t s0y =  s0tr->GetY();
      (*fP)[0] -= s0x;
      (*fP)[1] -= s0y;
   }

   /* incident angle correction (ignore 2nd order of a,b) */
   if (fUseIncidentAngleCorrection) {
      const Double_t s0a = -s0tr->GetA(); // BLD coordinate is left-handed
      const Double_t s0b =  s0tr->GetB();
      fP->RotateY(-s0a);
      fP->RotateX(s0b);
   }
}

void TRecoilProcessor::CorrectionWithVertex()
{
   const TTrack *const vertex = static_cast<TTrack*>((*fVertex)->At(0));

   /* reaction point correction */
   if (fUseReactionPointCorrection) {
      const Double_t x = vertex->GetX();
      const Double_t y = vertex->GetY();
      const Double_t z = vertex->GetZ();
      (*fP)[0] -= x;
      (*fP)[1] -= y;
      (*fP)[2] -= z;
   }

   /* incident angle correction (ignore 2nd order of a,b) */
   if (fUseIncidentAngleCorrection) {
      const Double_t a = vertex->GetA();
      const Double_t b = vertex->GetB();
      fP->RotateY(-a);
      fP->RotateX(b);
   }
}

Double_t TRecoilProcessor::CalcMomentum(const ITiming *tof)
{
   const Double_t fl    = fP->Mag() * fStopDetNormal->Mag2() / fP->Dot(*fStopDetNormal) * 1E-3; // meter
   const Double_t beta  = fl / tof->GetTiming() / TMath::C() * 1E+9; // * 1E+9 because timing is in nsec
   const Double_t gamma = 1. / TMath::Sqrt(1. - beta * beta);
   const Double_t p = fM * beta * gamma;
   return beta < 1 ? p : kInvalidD;
}

Double_t TRecoilProcessor::CalcMomentumWithTable(const ITiming *tof, Double_t theta, Double_t phi)
{
   // theta, phi should be in radian
   using TMath::Power;
   using TMath::Sin;

   const Double_t tof_c =
      tof->GetTiming() * TMath::Sqrt(1 - Power(Sin(theta),2) * Power(Sin(phi),2));
   const Double_t theta_deg = theta * TMath::RadToDeg();

   std::vector<Double_t> ke;
   for (Int_t i = 0, n = fThetas.size(); i!=n; ++i) {
      ke.push_back(fSplines[i]->Eval(tof_c));
   }

   TSpline3 sp("sp",&fThetas.front(),&ke.front(),fThetas.size());

   const Float_t k = sp.Eval(theta_deg);

   return TMath::Sqrt(k * (k + 2*fM));
}
