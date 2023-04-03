/**
 * @file   TP2PKinematicsProcessor.cc
 * @brief
 *
 * @date   Created:       2013-12-11 15:02:12
 *         Last Modified: 2015-02-13 12:11:55 JST (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved.
 */

#include "TP2PKinematicsProcessor.h"

#include <fstream>
#include "yaml-cpp/yaml.h"

#include <TClonesArray.h>

#include "TP2PKinematicsData.h"
#include "TPPACData.h"
#include "TMWDCTrackingResult.h"
#include "constant.h"
#include "TBeamParameter.h"

using art::sh04::TP2PKinematicsProcessor;
using std::cout;
using std::endl;


ClassImp(art::sh04::TP2PKinematicsProcessor)

namespace{
   enum EResidualFrame {kTargetRest=0, kProtonRest=1, kCenterOfMass=2};
}

// Default constructor
TP2PKinematicsProcessor::TP2PKinematicsProcessor()
   : fKinematics(NULL), fBrho(kInvalidD), fA(kInvalidI), fZ(kInvalidI), fF6XDelta(kInvalidD), fResidualFrame(0) {
   RegisterOutputCollection("OutputCollection","output",
			    fOutputColName,TString("kinematics"),
			    &fKinematics, TP2PKinematicsData::Class_Name());
   RegisterInputCollection("RecoilL","output from TRecoilProcessor",
			   fRecoilLName,TString("recoilL"),
			   &fRecoilL, TLorentzVector::Class_Name());
   RegisterInputCollection("RecoilR","output from TRecoilProcessor",
			   fRecoilRName,TString("recoilR"),
			   &fRecoilR, TLorentzVector::Class_Name());
   RegisterProcessorParameter("PPAC","output from TPPACProcessor",
			      fF6PPACColName,TString("ppac"));
   RegisterProcessorParameter("PPACDetID","detectorID for F6PPAC",
			      fF6PPACDetID,0);
   RegisterProcessorParameter("BeamParameter","name of beam parameter",
			      fParameterName,TString("beam"));
   RegisterProcessorParameter("ResidualFrame","0: Target-Rest Frame, 1: Proton-Rest Frame",
			      fResidualFrame,(Int_t)kTargetRest);
/* // not impremented yet
   RegisterProcessorParameter("IsInverseKinematics","is inverse kinematics",
			      fIsInverseKinematics,kTRUE);
*/
   fIsInverseKinematics = kTRUE;

}

TP2PKinematicsProcessor::~TP2PKinematicsProcessor()
{
   delete fKinematics;
}

void TP2PKinematicsProcessor::Init(TEventCollection *col)
{
   if (fVerboseLevel) Info("Init","RecoilL: %s, RecoilR: %s",fRecoilLName.Data(),fRecoilRName.Data());

   fF6PPAC = reinterpret_cast<TClonesArray**>(col->GetObjectRef(fF6PPACColName.Data()));
   if (fF6PPAC) {
      if (fVerboseLevel) Info("Init","F6PPAC: %s (ID: %d)",fF6PPACColName.Data(),fF6PPACDetID);
   } else {
      Warning("Init","No information from F6PPAC will be used.");
   }

   { /* Load beam parameter fron UserInfo */
      const TBeamParameter *const beam = dynamic_cast<TBeamParameter*>(col->GetInfo(fParameterName));
      if (!beam) {
	 SetStateError(TString::Format("No TBeamParameter named '%s' found", fParameterName.Data()));
	 return;
      }
      fZ	= beam->GetZ();
      fA	= beam->GetA();
      fBrho	= beam->GetBrho();
      fF6XDelta = beam->GetF6XD();
   }

   if (fVerboseLevel) Info("Init","(A,Z): (%d,%d)",fA, fZ);

   {// TODO: refine assignment code
      fM = fZ * P_MASS + (fA-fZ) * N_MASS;
      if (fZ == 1 && fA == 1) {
	 fQvalue = 0.;
      } else if (fZ == 8) {
	 if (fA == 14) {
	    fM -= fA * 7.052;
	    fQvalue = 4.627;
	 } else if (fA == 22) {
	    fM -= fA * 7.365;
	    fQvalue = 2.326E+1;
	 } else if (fA == 24) {
	    fM -= fA * 7.040;
	    fQvalue = 2.71E+1;
	 }
      } else {
	 SetStateError("mass and qvalue are not defined for assigned (A,Z).");
	 return;
      }
   }

   if (fVerboseLevel) Info("Init","Output: %s",fOutputColName.Data());
}

namespace {
inline const art::TPPACData* GetPPAC(const TClonesArray *const ca, const Int_t detID)
{
   const Int_t n = ca->GetEntriesFast();
   for (Int_t i = 0; i != n; ++i){
      const art::TPPACData *const ppac = static_cast<art::TPPACData*>(ca->At(i));
      if (ppac->GetID() == detID) return ppac;
   }
   return NULL;
}

inline Double_t CalcKZ(const Double_t gamma, const Double_t beta,
		       const Double_t lpz, const Double_t rpz,
		       const Double_t m_target, const Double_t m_recoil,
		       const Double_t k_para2)
{
   return (lpz + rpz)/gamma - beta * (m_target - m_recoil - k_para2 / (2*m_recoil));
}
}

void TP2PKinematicsProcessor::Process()
{
   /* initialization */
   fKinematics->Clear();

   const TLorentzVector *const l = *fRecoilL;
   const TLorentzVector *const r = *fRecoilR;

   if (!l || !r) return;

   /* calculate incident */
   // this part should be divided into another processor
   TLorentzVector *const incident = fKinematics->GetIncident();
   {
      const TPPACData *const ppac = fF6PPAC ? GetPPAC(*fF6PPAC, fF6PPACDetID) : NULL;
      const Double_t colFactor = (ppac && IsValid(fF6XDelta)) ? ppac->GetX()/fF6XDelta : 0.;
      const Double_t brho = fBrho * (1. + colFactor);

      const Double_t alpha = TMath::Power(TMath::C() * 1e-6 * fZ * brho / fM,2);
      const Double_t beta  = TMath::Sqrt(alpha/(1+alpha));
      incident->SetPxPyPzE(0,0,0,fM);
      incident->Boost(0,0,beta);
   }

   fKinematics->SetRecoilL(l);
   fKinematics->SetRecoilR(r);

   const Double_t beta  = incident->Beta();
   const Double_t gamma = incident->Gamma();
   const Double_t sp0   = (1 - gamma) * P_MASS
      - gamma * (l->Energy() + r->Energy() - 2*P_MASS)
      + beta * gamma * (l->Pz() + r->Pz());

   TVector3 k(l->X() + r->X(), l->Y() + r->Y(), 0);
   Double_t sp = sp0;
   Double_t m_recoil = 0;

   for (Int_t i = 0; i < 10; ++i) {
      const Double_t sp_old = sp;
      m_recoil = fM + sp - P_MASS;
      sp = sp0 - k.Mag2()/(2*m_recoil);
      k.SetZ( CalcKZ(gamma, beta, l->Z(), r->Z(), fM, m_recoil, k.Mag2()) );

      if (abs(sp - sp_old) < 0.001) break;
   }
   fKinematics->SetSp(sp);
   fKinematics->SetEx(sp - fQvalue);

//   if ( IsValid(sp) && abs(sp - 20) < 30 ){
//      k.Print();
//      printf("Sp, dSp: %f, %f\n",sp, sp - sp0);
//   }

   /* residual */
   {
      TLorentzVector *const residual = fKinematics->GetResidual();
      residual->SetVectM(k, m_recoil);
   }
}
