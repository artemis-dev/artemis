/**
 * @file   TSMWDCTrackingProcessor.cc
 * @brief
 *
 * @date   Created:       2013-10-19 17:01:56
 *         Last Modified: 2015-04-16 06:58:57 JST (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved.
 */

#include "TSMWDCTrackingProcessor.h"

#include <TClonesArray.h>
#include <TMath.h>
#include <TStopwatch.h>
#include <TMatrixD.h>
#include <TClass.h>

#include <numeric>

#include "TMWDCTrackingResult.h"
#include "TMWDCParameter.h"
#include "TMWDCPlaneInfo.h"
#include "TMWDCHitData.h"
#include "TMultiHitCombinator.h"
#include "constant.h"

using art::sh04::TSMWDCTrackingProcessor;

ClassImp(art::sh04::TSMWDCTrackingProcessor)

// Default constructor
TSMWDCTrackingProcessor::TSMWDCTrackingProcessor()
   : fPlaneIn(NULL), fPlaneInfo(NULL), fTrackingResult(NULL),
     fPlaneData(NULL), fCos(NULL), fSin(NULL),
     fZ(NULL), fX(NULL), fY(NULL), fWeight(NULL),
     fDLOriginal(NULL), fDLCorrected(NULL), fStopwatch(NULL),
     fCombinator(NULL), fGMatrix(NULL), fHMatrix(NULL), fCovariantMatrix(NULL)
{
   StringVec_t defInput;
   defInput.push_back("smwdc_x1");
   defInput.push_back("smwdc_x2");
   defInput.push_back("smwdc_u1");
   defInput.push_back("smwdc_u2");
   defInput.push_back("smwdc_v1");
   defInput.push_back("smwdc_v2");
   RegisterInputCollection("InputCollection","input",
			   fInputColName,defInput);
   RegisterOutputCollection("OutputCollection","output",
			    fOutputColName,TString("smwdc_track"));
   RegisterProcessorParameter("NIteration",
			      "number of incident angle iteration. if zero, no incident angle correction will be done.",
			      fNIteration,0);
   RegisterProcessorParameter("UseAllCombination","use all the combination",
			      fUseAllCombination,kTRUE);
   RegisterProcessorParameter("SignificanceLevel","significance level",
			      fSignificanceLevel,0.05F);
   RegisterProcessorParameter("MAXSSR","maximum value of SSR",
			      fSSRMax,1e+1F);

   FloatVec_t defGate;
   defGate.push_back(-1e6);
   defGate.push_back(1e6);
   RegisterProcessorParameter("TimingGate","timing gate",fTimingGate,defGate);
   RegisterProcessorParameter("ChargeGate","charge gate",fChargeGate,defGate);
   RegisterProcessorParameter("AGate","x angle gate",fAGate,defGate);
   RegisterProcessorParameter("BGate","x angle gate",fBGate,defGate);
}

TSMWDCTrackingProcessor::~TSMWDCTrackingProcessor()
{
   delete fPlaneIn;
   delete fPlaneInfo;
   delete fTrackingResult;
   delete [] fPlaneData;
   delete [] fCos;
   delete [] fSin;
   delete [] fZ;
   delete [] fX;
   delete [] fY;
   delete [] fWeight;
   delete [] fDLOriginal;
   delete [] fDLCorrected;
   delete fStopwatch;
   delete fCombinator;
   if (fGMatrix) {
      for (Int_t i = 0; i!=fNPlane + 1; ++i) {
	 delete fGMatrix[i];
      }
      delete [] fGMatrix;
   }
   if (fHMatrix) {
      for (Int_t i = 0; i!=fNPlane + 1; ++i) {
	 delete fHMatrix[i];
      }
      delete [] fHMatrix;
   }
      if (fHMatrix) {
      for (Int_t i = 0; i!=fNPlane + 1; ++i) {
	 delete fCovariantMatrix[i];
      }
      delete [] fCovariantMatrix;
   }


}

void TSMWDCTrackingProcessor::Init(TEventCollection *col)
{

   fPlaneIn = new std::vector<TClonesArray**>;
   fPlaneInfo = new std::vector<TMWDCPlaneInfo*>;

   fNPlane = fInputColName.size();

   if(fNPlane < CRITICAL_NPLANE) {
      SetStateError("Number of plane is insufficient for tracking (min: 5 planes).");
      fNPlane = 0;
      return;
   }

   {
      TString mwdcnameCache("");
      for (Int_t i = 0;i!=fNPlane;++i) {
	 const TString &inputName = fInputColName[i];

	 TClonesArray** const ref = reinterpret_cast<TClonesArray**>(col->GetObjectRef(inputName.Data()));
	 if (!ref) {
	    SetStateError(TString::Format("branch '%s' not found",inputName.Data()));
	    return;
	 }

	 const Int_t delimIndex = inputName.Last('_');
	 const TString &mwdcname = inputName(0,delimIndex);
	 const TString &planename = inputName(delimIndex+1,inputName.Length());

	 TMWDCParameter *const mwdcprm =
	    reinterpret_cast<TMWDCParameter*>(col->GetInfo(mwdcname));

	 if (!mwdcprm) {
	    SetStateError(TString::Format("mwdc parameter object '%s' not found",mwdcname.Data()));
	    return;
	 }

	 TMWDCPlaneInfo *const info = mwdcprm->GetPlane(planename, kTRUE);
	 if (!info) {
	    SetStateError(TString::Format("info for plane '%s' not found",planename.Data()));
	    return;
	 }

	 if (mwdcnameCache.Length() && !mwdcnameCache.EqualTo(mwdcname)) {
	    Warning("Init","Planes from more than one MWDC are used.");
	 }
	 mwdcnameCache = mwdcname;

	 fPlaneIn->push_back(ref);
	 fPlaneInfo->push_back(info);
      }
   }

   fStopwatch = new TStopwatch;

   fTrackingResult = new TClonesArray(TMWDCTrackingResult::Class(),1);
   fTrackingResult->SetName(fOutputColName);
   col->Add(fOutputColName,fTrackingResult,fOutputIsTransparent);

   fCos = new Double_t[fNPlane];
   fSin = new Double_t[fNPlane];
   fZ   = new Double_t[fNPlane];
   fX   = new Double_t[fNPlane];
   fY   = new Double_t[fNPlane];

   fWeight = new Int_t [fNPlane];

   for(Int_t i = 0; i!=fNPlane; ++i) {
      const TMWDCPlaneInfo *const planeInfo = fPlaneInfo->at(i);
      fCos[i] = planeInfo->Cos();
      fSin[i] = planeInfo->Sin();
      fZ[i]   = planeInfo->GetZ();
   }

   GenerateMatrix();

   fPlaneData = new const TMWDCHitData*[fNPlane];
   fDLOriginal = new Double_t[fNPlane];
   fDLCorrected = new Double_t[fNPlane];

   const Bool_t allowNoHit = kTRUE;
   fCombinator = new TMultiHitCombinator(allowNoHit);

   TString outputStr = TString("(");
   for(Int_t i = 0; i!=fNPlane; ++i) {
      if(i) outputStr.Append(", ");
      outputStr.Append(fInputColName[i]);
   }
   outputStr.Append(") => ");
   outputStr.Append(fOutputColName);

   Info("Init","%s",outputStr.Data());
   Info("Init","UseAllCombination: %s",fUseAllCombination ? "true" : "false");
   Info("Init","NIteration: %d",fNIteration);
   Info("Init","Significance level: %.3f",fSignificanceLevel);
}

void TSMWDCTrackingProcessor::Process()
{
   if(!fTrackingResult) return;

   {  /* initialization */
      fStopwatch->Start();
      fTrackingResult->Clear("C");
      fNPlaneValid = 0;
      fCombinator->Clear();
      fPlaneCombinationID = fNPlane;
   }

   FillCombinator();

   for (Int_t iPlane = 0; iPlane != fNPlane; ++iPlane) {
      const TObjArray *const hits = fCombinator->GetHits(iPlane);
      if ( hits && !hits->IsEmpty() ) {
	 fWeight[iPlane] = 1;
	 ++fNPlaneValid;
      } else {
	 fWeight[iPlane] = 0;
	 fPlaneCombinationID = iPlane;
      }
   }

   TMWDCTrackingResult *const tr =
      static_cast<TMWDCTrackingResult*>(fTrackingResult->ConstructedAt(0));
   tr->Init(fNPlane);
   tr->SetNPlaneValid(fNPlaneValid);
   if (fNPlaneValid < CRITICAL_NPLANE) {
      tr->SetPlaneCombinationID(-1);
      return;
   }

   tr->SetPlaneCombinationID(fPlaneCombinationID);
   tr->SetNParameter(NPARAMETER);

   {
      const Int_t nCombination = fCombinator->GetNCombination();
      // fCombinator is set to hold the best combination
      const Int_t nCombinationProcessed = FindBestCombination(tr);

      tr->SetNCombination(nCombination);
      tr->SetNCombinationProcessed(nCombinationProcessed);
      if (!nCombinationProcessed) {
	 tr->SetTimeCost(fStopwatch->RealTime() * 1000.); // in msec
	 return;
      }
   }

   /* track again with the best combination */
   for (Int_t iPlane = 0; iPlane != fNPlane ; ++iPlane) {
      if (fPlaneCombinationID == iPlane) continue;
      fPlaneData[iPlane] =
	 static_cast<const TMWDCHitData*>(fCombinator->GetHit(iPlane));
      const Double_t driftLength = fPlaneData[iPlane]->GetDriftLength();
      fDLOriginal[iPlane] = driftLength;
      fDLCorrected[iPlane] = driftLength;
   }
   FindTrack(tr, kTRUE);

   /* Iteration for Incident Angle Correction */
   if (fNIteration && tr->IsGood()) {
      for (Int_t i = 0; i!= fNIteration; ++i) {
	 IncidentAngleCorrection(tr);
	 FindTrack(tr, kTRUE);
      }
   }

   for (Int_t iPlane=0; iPlane!=fNPlane; ++iPlane) {
      if (fPlaneCombinationID == iPlane || !fPlaneData[iPlane]) continue;
      tr->SetWireIDAdopted(iPlane,fPlaneData[iPlane]->GetDetID());
      tr->SetDriftLengthAdopted(iPlane,fDLCorrected[iPlane]);
   }

   tr->SetTimeCost(fStopwatch->RealTime() * 1000.); // in msec
}

void TSMWDCTrackingProcessor::FillCombinator()
{
   for (Int_t iPlane = 0; iPlane !=fNPlane; ++iPlane) {
      TClonesArray *const plane = *(fPlaneIn->at(iPlane));
      const Int_t nHit = plane->GetEntriesFast();
      if(!nHit) continue;

      if (!fUseAllCombination) {
	 TMWDCHitData::SetSortType(TMWDCHitData::kDriftLength);
	 TMWDCHitData::SetSortOrder(TMWDCHitData::kASC);
	 plane->UnSort(); // unflag plane->fSorted
	 plane->Sort();
	 plane->Compress();
      }

      for (Int_t iHit=0;iHit != nHit; ++iHit) {
	 TMWDCHitData *const planeData = static_cast<TMWDCHitData*>(plane->At(iHit));
	 if (DataIsValid(planeData)) fCombinator->Add(planeData,iPlane);
      }
   }
   fCombinator->Prepare();
}

Int_t TSMWDCTrackingProcessor::FindBestCombination(TMWDCTrackingResult *tr)
{
   fCombinator->Prepare();
   Int_t nCombinationProcessed = 0;
   if (fUseAllCombination) {
      {
	 const Int_t nCombination = fCombinator->GetNCombination();
	 if(nCombination > 1e+6) {
	    Warning("Process","large combination number: %d. skipped.",nCombination);
	    tr->SetTrackingID(TMWDCTrackingResult::kLargeCombination);
	    return 0;
	 }
      }
      Double_t bestSSR = fSSRMax;
      Int_t bestCombinationID = kInvalidI;
      while (fCombinator->Next()) {
	 for (Int_t iPlane = 0; iPlane != fNPlane ; ++iPlane) {
	    if (fPlaneCombinationID == iPlane) continue;
	    fPlaneData[iPlane] =
	       static_cast<const TMWDCHitData*>(fCombinator->GetHit(iPlane));
	    fDLCorrected[iPlane] = fPlaneData[iPlane]->GetDriftLength();
	 }
	 if (!CombinationIsValid()) continue;

	 ++nCombinationProcessed;
	 FindTrack(tr,kFALSE);

	 if (!ParametersAreValid(tr)) continue;

	 {
	    const Double_t SSR = tr->GetSSR();
	    if (SSR < bestSSR) {
	       bestSSR = SSR;
	       bestCombinationID = fCombinator->GetCombinationID();
	    }
	 }
      }
      if (IsValid(bestCombinationID)) {
	 fCombinator->SetCombinationByID(bestCombinationID);
      }
   }
   return nCombinationProcessed;
}

Bool_t TSMWDCTrackingProcessor::DataIsValid(const TMWDCHitData *planeData) const
{
   {
      const Double_t charge = planeData->GetCharge();
      if (!IsValid(charge)) return kFALSE;
      if (charge < fChargeGate[0] || charge > fChargeGate[1]) return kFALSE;
   }
   {
      const Double_t timing = planeData->GetTiming();
      if (timing < fTimingGate[0] || timing > fTimingGate[1]) return kFALSE;
   }
   {
      const Double_t dl     = planeData->GetDriftLength();
      if (!IsValid(dl)) return kFALSE;
   }

   return kTRUE;
}

Bool_t TSMWDCTrackingProcessor::ParametersAreValid(const TMWDCTrackingResult *track) const
{
   const Double_t a = track->A();
   const Double_t b = track->B();
   if (a < fAGate[0] || a > fAGate[1]) return kFALSE;
   if (b < fBGate[0] || b > fBGate[1]) return kFALSE;
   return kTRUE;
}

Bool_t TSMWDCTrackingProcessor::CombinationIsValid() const
{
   const Double_t epsilon = 1e-3;
   for (Int_t iPlane = 0; iPlane != fNPlane - 1; ++iPlane) {
      if (TMath::Abs(fSin[iPlane] - fSin[iPlane+1]) > epsilon) continue; // enough for smwdc
      if (fPlaneCombinationID == iPlane
	  || fPlaneCombinationID == iPlane + 1) continue;
      {
	 const Int_t wireID1 = fPlaneData[iPlane]->GetDetID();
	 const Int_t wireID2 = fPlaneData[iPlane+1]->GetDetID();
	 if (TMath::Abs(wireID1 - wireID2) > 1) return kFALSE;
      }
   }
   return kTRUE;
}

inline void TSMWDCTrackingProcessor::IncidentAngleCorrection(TMWDCTrackingResult *track)
{
   const Double_t a = track->GetA();
   const Double_t b = track->GetB();

   for (Int_t i = 0; i != fNPlane; ++i) {
      const Double_t dot = a*fCos[i] + b*fSin[i];
      fDLCorrected[i] = fDLOriginal[i] * TMath::Sqrt(1 + dot*dot);
   }
}

void TSMWDCTrackingProcessor::GenerateMatrix() {
   if (!fPlaneInfo) return;

   fGMatrix         = new TMatrixD*[fNPlane+1];
   fHMatrix         = new TMatrixD*[fNPlane+1];
   fCovariantMatrix = new TMatrixD*[fNPlane+1];
   for (Int_t iCombination = 0; iCombination!=fNPlane+1; ++iCombination) {
      fGMatrix[iCombination]         = new TMatrixD(NPARAMETER,fNPlane);
      fHMatrix[iCombination]         = new TMatrixD(fNPlane,NPARAMETER);
      fCovariantMatrix[iCombination] = new TMatrixD(NPARAMETER,NPARAMETER);

      TMatrixD &h   = *fHMatrix[iCombination];
      for (Int_t iPlane = 0; iPlane != fNPlane; ++iPlane) {
	 const Double_t cos = fCos[iPlane];
	 const Double_t sin = fSin[iPlane];
	 const Double_t z   = fZ[iPlane];
	 const Double_t w   = (iCombination == iPlane) ? 0. : 1.;
	 h[iPlane][0] = w * cos;
	 h[iPlane][1] = w * z * cos;
	 h[iPlane][2] = w * sin;
	 h[iPlane][3] = w * z * sin;
      }

      TMatrixD &cov = *fCovariantMatrix[iCombination];
      cov.TMult(h,h);
      cov.InvertFast();
      TMatrixD &g   = *fGMatrix[iCombination];
      g.MultT(cov,h);
   }
}

void TSMWDCTrackingProcessor::FindTrack(TMWDCTrackingResult *tr, const Bool_t fillStat = kTRUE)
{
   std::vector<Double_t> pos(fNPlane), trackPrm(NPARAMETER);
   std::vector<Double_t> bestTrackPrm(NPARAMETER,kInvalidD);
   std::vector<Double_t> residual(fNPlane);
   std::vector<Double_t> bestResidual(fNPlane,kInvalidD);

   Double_t minSSR = fSSRMax;

   for(Int_t i = 0; i!= TMath::Nint(TMath::Power(2,fNPlaneValid)) ; ++i){
      {
	 Int_t shift = 0;
	 for(Int_t iPlane = 0; iPlane !=fNPlane ; ++iPlane){
	    if (!fWeight[iPlane]) {
	       pos[iPlane] = 0.;
	       continue;
	    }
	    const TMWDCPlaneInfo *const planeInfo = fPlaneInfo->at(iPlane);
	    const Double_t cellSize = planeInfo->GetCellSize();
	    const Double_t center   = planeInfo->GetCenter();
	    const Double_t wireID   = fPlaneData[iPlane]->GetDetID();
	    const Double_t dl       = fDLCorrected[iPlane];
	    const Int_t    lr       = ((i>>shift) & 1) ? 1 : -1;

	    pos[iPlane] = cellSize * (wireID - center) + dl * lr;
	    ++shift;
	 }
      }

      {
	 const TMatrixD &g = *fGMatrix[fPlaneCombinationID];
	 for (Int_t iParameter = 0; iParameter!=NPARAMETER; ++iParameter) {
	    // calculate pos[] \dot g[iParameter][]
	    trackPrm[iParameter] = std::inner_product(pos.begin(),pos.end(),
						      g[iParameter].GetPtr(),0.);
	 }
      }

      const Double_t SSR = CalcSSR(pos,trackPrm,residual);
      if (SSR < minSSR) {
         minSSR = SSR;
	 std::copy(trackPrm.begin(),trackPrm.end(),bestTrackPrm.begin());
	 std::copy(residual.begin(),residual.end(),bestResidual.begin());
      }
   }

   tr->SetTrack(bestTrackPrm[0],bestTrackPrm[2],0,
		bestTrackPrm[1],bestTrackPrm[3]);
   tr->SetSSR(minSSR);
   for(Int_t iPlane=0;iPlane!=fNPlane;++iPlane){
      if (fPlaneCombinationID != iPlane)
	 tr->SetResidual(iPlane,bestResidual[iPlane]);
   }

   {
      const Int_t trackingID =
	 (minSSR < fSSRMax) ? TMWDCTrackingResult::kGood : TMWDCTrackingResult::kLargeSSR;
      tr->SetTrackingID(trackingID);
   }

   if (fillStat) { /* fill statistical information */
      const Int_t ndf = fNPlaneValid - NPARAMETER;
      const Double_t standardCI = TMath::StudentQuantile(1.- fSignificanceLevel/2., ndf);
      const Double_t usv = minSSR/ndf; // unbiased sample variance
      const TMatrixD &cov = *fCovariantMatrix[fPlaneCombinationID];
      for (Int_t i = 0; i != NPARAMETER; ++i) {
	 const Double_t var   = cov[i][i] * usv;
	 const Double_t sigma = TMath::Sqrt(var);
	 const Double_t CI    = standardCI * sigma;
	 tr->SetVar(i,var);
	 tr->SetSigma(i,sigma);
	 tr->SetCI(i,CI);
      }
   }
}

inline Double_t TSMWDCTrackingProcessor::CalcSSR(const std::vector<Double_t> &pos,
						 const std::vector<Double_t> &trackPrm,
						 std::vector<Double_t> &residual) const
{
   Double_t SSR = 0.;
   for(Int_t iPlane = 0; iPlane !=fNPlane ; ++iPlane){
      if (!fWeight[iPlane]) continue;
      const Double_t trackedPosition =
	 fCos[iPlane] * (trackPrm[0] + trackPrm[1] * fZ[iPlane])
	 + fSin[iPlane] * (trackPrm[2] + trackPrm[3] * fZ[iPlane]);
      const Double_t r = residual[iPlane] = pos[iPlane] - trackedPosition;
      SSR += r * r;
      if (SSR > fSSRMax) return fSSRMax;
   }
   return SSR;
}
