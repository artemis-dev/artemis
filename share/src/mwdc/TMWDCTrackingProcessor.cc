/**
 * @file   TMWDCTrackingProcessor.cc
 * @brief
 *
 * @date   Created:       2013-10-19 17:01:56
 *         Last Modified: 2015-04-16 06:05:43 JST (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved.
 */

#include "TMWDCTrackingProcessor.h"
#include "TMWDCTrackingResult.h"
#include "TMWDCParameter.h"
#include "TMWDCPlaneInfo.h"
#include "TMWDCHitData.h"
#include "constant.h"

#include <numeric>
#include <iterator>

#include <TStopwatch.h>
#include <TClonesArray.h>
#include <TMath.h>
#include <TMatrixD.h>

using art::TMWDCTrackingProcessor;

ClassImp(art::TMWDCTrackingProcessor);

// Default constructor
TMWDCTrackingProcessor::TMWDCTrackingProcessor()
   : fPlaneIn(NULL), fPlaneInfo(NULL), fMWDCTrackOut(NULL),
     fGMatrix(NULL), fHMatrix(NULL), fCovariantMatrix(NULL),
     fPlaneData(NULL), fCos(NULL), fSin(NULL),
     fZ(NULL), fX(NULL), fY(NULL), fStopwatch(NULL)
{
   StringVec_t defInput;
   defInput.push_back("mwdc1_x");
   defInput.push_back("mwdc1_u");
   defInput.push_back("mwdc1_v");
   RegisterInputCollection("InputCollection","input",
			   fInputColName,defInput);
   RegisterOutputCollection("OutputCollection","output",
			    fOutputColName,TString("mwdc1_track"),
			    &fMWDCTrackOut, "TClonesArray", "art::TMWDCTrackingResult");
   RegisterProcessorParameter("UseLocalPos","default: 0",
			      fUseLocalPos,0);
   RegisterProcessorParameter("UseAllCombination","use all the combination",
			      fUseAllCombination,kFALSE);
   RegisterProcessorParameter("SignificanceLevel","significance level",
			      fSignificanceLevel,0.05F);
   RegisterProcessorParameter("MaxSSR","maximum limit for SSR",
			      fSSRMax,1e+1F);

   FloatVec_t defGate;
   defGate.push_back(-1e6);
   defGate.push_back(1e6);
   RegisterProcessorParameter("TimingGate","timing gate",fTimingGate,defGate);
   RegisterProcessorParameter("ChargeGate","charge gate",fChargeGate,defGate);
}

TMWDCTrackingProcessor::~TMWDCTrackingProcessor()
{
   delete fPlaneIn;
   delete fPlaneInfo;
   delete fMWDCTrackOut;
   delete fGMatrix;
   delete fHMatrix;
   delete fCovariantMatrix;
   delete [] fPlaneData;
   delete [] fCos;
   delete [] fSin;
   delete [] fZ;
   delete [] fX;
   delete [] fY;
   delete fStopwatch;
}

void TMWDCTrackingProcessor::Init(TEventCollection *col)
{
   fPlaneIn = new std::vector<TClonesArray**>;
   fPlaneInfo = new std::vector<TMWDCPlaneInfo*>;

   fNPlane = fInputColName.size();

   if(fNPlane < MIN_NPLANE) {
      SetStateError("Number of plane is insufficient for tracking (min: 3 planes).");
      fNPlane = 0;
      return;
   }

   {
      TString mwdcnameCache("");
      for (Int_t i = 0;i!=fNPlane;++i) {
	 const TString &inputName = fInputColName[i];

	 TClonesArray** const ref = reinterpret_cast<TClonesArray**>(col->GetObjectRef(inputName.Data()));

	 const Int_t delimIndex = inputName.Last('_');
	 const TString &mwdcname = inputName(0,delimIndex);
	 const TString &planename = inputName(delimIndex+1,inputName.Length());

	 TMWDCParameter *const mwdcprm =
	    reinterpret_cast<TMWDCParameter*>(col->GetInfo(mwdcname));

	 if (!mwdcprm) {
	    SetStateError(TString::Format("mwdc parameter object '%s' not found",mwdcname.Data()));
	    delete fPlaneIn;
	    delete fPlaneInfo;
	    fPlaneIn = NULL;
	    fPlaneInfo = NULL;
	    return;
	 }

	 TMWDCPlaneInfo *const info = mwdcprm->GetPlane(planename, fUseLocalPos);
	 if (!info) {
	    SetStateError(TString::Format("info for plane '%s' not found",planename.Data()));
	 }

	 if (fUseLocalPos) {
	    if (mwdcnameCache.Length() && !mwdcnameCache.EqualTo(mwdcname)) {
	       Warning("Init","Planes from more than one MWDC are used. fUseLocalPos setting will be ignored.");
	       fUseLocalPos = kFALSE;
	    }
	    mwdcnameCache = mwdcname;
	 }

	 if (!ref || !info) {
	    delete fPlaneIn;
	    delete fPlaneInfo;
	    fPlaneIn = NULL;
	    fPlaneInfo = NULL;
	    return;
	 }

	 fPlaneIn->push_back(ref);
	 fPlaneInfo->push_back(info);
      }
   }

   fCos = new Double_t[fNPlane];
   fSin = new Double_t[fNPlane];
   fZ   = new Double_t[fNPlane];
   fX   = new Double_t[fNPlane];
   fY   = new Double_t[fNPlane];

   for(Int_t i = 0; i!=fNPlane; ++i) {
      const TMWDCPlaneInfo *const planeInfo = fPlaneInfo->at(i);
      fCos[i] = planeInfo->Cos();
      fSin[i] = planeInfo->Sin();
      fZ[i]   = planeInfo->GetZ();
      fX[i]   = planeInfo->GetX();
      fY[i]   = planeInfo->GetY();
   }

   fNParameter = (fNPlane < CRITICAL_NPLANE) ? 2 : 4;
   GenerateMatrix();

   fPlaneData = new const TMWDCHitData*[fNPlane];

   const Bool_t allowNoHit = kFALSE;
   fCombinator = new TMultiHitCombinator(allowNoHit);

   TString outputStr = TString("(");
   for(Int_t i = 0; i!=fNPlane; ++i) {
      if(i) outputStr.Append(", ");
      outputStr.Append(fInputColName[i]);
   }
   outputStr.Append(") => ");
   outputStr.Append(fOutputColName);

   Info("Init","%s",outputStr.Data());

   fStopwatch = new TStopwatch;
}

void TMWDCTrackingProcessor::GenerateMatrix()
{
   fGMatrix         = new TMatrixD(fNParameter,fNPlane);
   fHMatrix         = new TMatrixD(fNPlane,fNParameter);
   fCovariantMatrix = new TMatrixD(fNParameter,fNParameter);

   TMatrixD &h   = *fHMatrix;
   for (Int_t iPlane = 0; iPlane != fNPlane; ++iPlane) {
      const Double_t cos = fCos[iPlane];
      const Double_t sin = fSin[iPlane];
      h[iPlane][0] = cos;
      h[iPlane][1] = sin;
      if (fNParameter == 4) {
	 const Double_t z = fZ[iPlane];
	 h[iPlane][2] = z * cos;
	 h[iPlane][3] = z * sin;
      }
   }

   TMatrixD &cov = *fCovariantMatrix;
   cov.TMult(h,h);

   if (TMath::AreEqualAbs(cov.Determinant(),0.,1e-5)) {
      SetStateError("Matrix is singular. Check plane configuration.");
      return;
   }

   cov.InvertFast();
   TMatrixD &g   = *fGMatrix;
   g.MultT(cov,h);
}

void TMWDCTrackingProcessor::Process()
{
   {/* initialization */
      fStopwatch->Start();
      fMWDCTrackOut->Clear("C");
   }

   std::vector<std::vector<std::vector<TMWDCHitData*> > > eventSet;
   std::vector<std::vector<TMWDCHitData*> > planeSet;
   std::vector<double> timestamp;
   

   for (int iplane = 0; iplane < fNPlane; ++iplane) {
      TClonesArray* aPlane = *fPlaneIn->at(iplane);
      std::vector<TMWDCHitData*> hitSet;
      for (int iHit = 0; iHit < aPlane->GetEntriesFast(); ++iHit) {
         TMWDCHitData *const aHit = (TMWDCHitData*)aPlane->At(iHit);
         // printf("%f\n",aHit->GetTimestamp());
         
         if (DataIsValid(aHit)) {
            hitSet.push_back(aHit);
         }
      }
      planeSet.push_back(hitSet);
      // printf("planeSet[%d].size() = %d\n",iplane,planeSet[iplane].size());
   }

   for (int iPlane = 0; iPlane < fNPlane; ++iPlane) {
      std::vector<TMWDCHitData*>& aPlane = planeSet[iPlane];
      std::vector<TMWDCHitData*>::iterator it = aPlane.begin();      
      while ( it < aPlane.end() ) {
         std::vector<double>::iterator itt = timestamp.begin();
         while (itt < timestamp.end()) {
            if (TMath::Abs((*it)->GetTimestamp() - *itt) < TMath::Limits<Double_t>::Epsilon()) {
               break;
            }
            ++itt;
         }
         int index = std::distance(timestamp.begin(),itt);
         // printf("iplane = %d, index = %d,eventSet.size() = %d\n",iPlane, index,eventSet.size());
         if ( index >= eventSet.size()) {
            // create a new data set because any stored timestamp were not match the hit
            eventSet.push_back( std::vector<std::vector<TMWDCHitData*> >(fNPlane,std::vector<TMWDCHitData*>()));
            eventSet[index][iPlane].push_back(*it);
            // push back the timestamp since this hit is the first hit for this timestamp
            timestamp.push_back((*it)->GetTimestamp());
            // printf("%f\n",((*it)->GetTimestamp()));
            // erase the hits from the first event
            it = aPlane.erase(it);
            // printf("next\n");
            
         } else if (index >= 0) {
            // move this hits, the timestamp corresponding to this hit was found in other than the first event
            eventSet[index][iPlane].push_back(*it);
            it = aPlane.erase(it);
         } else {
            ++it;
         }
      }
   }
   // printf("eventSet.size() = %d\n",eventSet.size());
   
   for (int ievt = 0; ievt < eventSet.size(); ++ievt) {
      // printf("plane.size for event %d: %d\n",ievt, eventSet[ievt].size());
      
      ProcessOne(eventSet[ievt]);
   }
}

void TMWDCTrackingProcessor::ProcessOne(const std::vector<std::vector<TMWDCHitData*> >& planeData)
{
   fNPlaneValid = 0;
   fCombinator->Clear();
   for (Int_t iPlane = 0; iPlane != fNPlane; ++iPlane) {
      const std::vector<TMWDCHitData*>& aPlane = planeData[iPlane];
      for (Int_t iHit=0, nHit = aPlane.size(); iHit != nHit; ++iHit) {
	 TMWDCHitData *const planeData =aPlane[iHit];
	 if (DataIsValid(planeData)) fCombinator->Add(planeData,iPlane);
      }
   }
   fCombinator->Prepare();
   for (Int_t iPlane = 0 ; iPlane!=fNPlane; ++iPlane) {
      const TObjArray *const hits = fCombinator->GetHits(iPlane);
      if ( hits && !hits->IsEmpty() ) ++fNPlaneValid;
   }

   TMWDCTrackingResult *const tr =
      static_cast<TMWDCTrackingResult*>(fMWDCTrackOut->ConstructedAt(fMWDCTrackOut->GetEntriesFast()));

   tr->Init(fNPlane);
   tr->SetNPlaneValid(fNPlaneValid);
   if( fNPlaneValid != fNPlane ) return;
   tr->SetTimestamp(planeData[0][0]->GetTimestamp());
   
   tr->SetNParameter(fNParameter);
   tr->SetNCombination(fCombinator->GetNCombination());

   if (fUseAllCombination) {
      const Int_t nCombinationProcessed = FindBestCombination(tr);
      tr->SetNCombinationProcessed(nCombinationProcessed);
      if (!nCombinationProcessed) {
	 tr->SetTimeCost(fStopwatch->RealTime() * 1000.); // in msec
	 return;
      }
   } else {
      tr->SetNCombinationProcessed(1);
   }

   for (Int_t iPlane = 0; iPlane != fNPlane ; ++iPlane) {
	 fPlaneData[iPlane] =
	    static_cast<const TMWDCHitData*>(fCombinator->GetHit(iPlane));
   }

   /* track again with the best combination */
   FindTrack(tr, kTRUE);

   for (Int_t iPlane=0; iPlane!=fNPlane; ++iPlane) {
      tr->SetWireIDAdopted(iPlane,fPlaneData[iPlane]->GetDetID());
      tr->SetDriftLengthAdopted(iPlane,fPlaneData[iPlane]->GetDriftLength());
   }

   const Double_t timeCost = fStopwatch->RealTime() * 1000.; // in milliseconds
   tr->SetTimeCost(timeCost);
}

Int_t TMWDCTrackingProcessor::FindBestCombination(TMWDCTrackingResult *tr)
{
   Int_t nCombinationProcessed = 0;
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
	 fPlaneData[iPlane] =
	    static_cast<const TMWDCHitData*>(fCombinator->GetHit(iPlane));
      }
      if (!CombinationIsValid()) continue;

      ++nCombinationProcessed;
      FindTrack(tr,kFALSE);
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
   return nCombinationProcessed;
}

Bool_t TMWDCTrackingProcessor::DataIsValid(const TMWDCHitData *planeData) const
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

Bool_t TMWDCTrackingProcessor::CombinationIsValid() const
{
   return kTRUE;
}

void TMWDCTrackingProcessor::FindTrack(TMWDCTrackingResult *tr, Bool_t fillStat)
{
   std::vector<Double_t> pos(fNPlane), trackPrm(fNParameter);
   std::vector<Double_t> bestTrackPrm(fNParameter,kInvalidD);
   std::vector<Double_t> residual(fNPlane,kInvalidD);
   std::vector<Double_t> bestResidual(fNPlane,kInvalidD);

   Double_t minSSR = fSSRMax;

   for(Int_t i = 0, N = TMath::Nint(TMath::Power(2,fNPlane)); i!= N ; ++i){
      for(Int_t iPlane = 0; iPlane !=fNPlane ; ++iPlane){
	 const Double_t cos = fCos[iPlane];
	 const Double_t sin = fSin[iPlane];
	 const Double_t x   = fX[iPlane];
	 const Double_t y   = fY[iPlane];

	 const TMWDCPlaneInfo &planeInfo = *fPlaneInfo->at(iPlane);
	 const Double_t cellSize = planeInfo.GetCellSize();
	 const Double_t center   = planeInfo.GetCenter();
	 const TMWDCHitData &planeData = *fPlaneData[iPlane];
	 const Double_t wireID   = planeData.GetDetID();
	 const Double_t dl       = planeData.GetDriftLength();
	 const Int_t    lr       = ((i >> iPlane) & 1) ? 1 : -1;
         pos[iPlane] = cellSize * (wireID - center) + dl * lr + x * cos + y * sin;
      }
      {
	 const TMatrixD &g = *fGMatrix;
	 for (Int_t iParameter = 0; iParameter!=fNParameter; ++iParameter) {
	    // calculate pos[] \dot g[iParameter][]
	    trackPrm[iParameter] = std::inner_product(pos.begin(),pos.end(),
						      g[iParameter].GetPtr(),0.);
	 }
      }
      const Double_t SSR = CalcSSR(pos,trackPrm,residual);
      if(SSR < minSSR){
         minSSR = SSR;
	 std::copy(trackPrm.begin(),trackPrm.end(),bestTrackPrm.begin());
	 std::copy(residual.begin(),residual.end(),bestResidual.begin());
      }
   }

   if (fNParameter == 2) {
      tr->SetTrack(bestTrackPrm[0],bestTrackPrm[1],0,0,0);
   } else {
      tr->SetTrack(bestTrackPrm[0],bestTrackPrm[1],0,
		   bestTrackPrm[2],bestTrackPrm[3]);
   }
   tr->SetSSR(minSSR);

   for(Int_t i=0;i!=fNPlane;++i){
      tr->SetWireIDAdopted(i,fPlaneData[i]->GetDetID());
      tr->SetDriftLengthAdopted(i,fPlaneData[i]->GetDriftLength());
      tr->SetResidual(i,bestResidual[i]);
   }

   if (minSSR < fSSRMax) {
      tr->SetTrackingID(TMWDCTrackingResult::kGood);
      if (fillStat) { /* fill statistical information */
	 const Int_t ndf = fNPlaneValid - fNParameter;
	 const Double_t standardCI = TMath::StudentQuantile(1.- fSignificanceLevel/2., ndf);
	 const Double_t usv = minSSR/ndf; // sample variance
	 const TMatrixD &cov = *fCovariantMatrix;
	 for (Int_t i = 0; i != fNParameter; ++i) {
	    const Double_t var   = cov[i][i] * usv;
	    const Double_t sigma = TMath::Sqrt(var);
	    const Double_t CI    = standardCI * sigma;
	    tr->SetSigma(i,sigma);
	    tr->SetVar(i,var);
	    tr->SetCI(i,CI);
	 }
      }
   } else {
      tr->SetTrackingID(TMWDCTrackingResult::kLargeSSR);
   }
}

Double_t TMWDCTrackingProcessor::CalcSSR(const std::vector<Double_t> &pos,
					 const std::vector<Double_t> &trackPrm,
					 std::vector<Double_t> &residual) const
{
   Double_t SSR = 0.;
   for(Int_t iPlane = 0; iPlane !=fNPlane ; ++iPlane){
      const Double_t trackedPosition =
	 fNParameter == 2 ?
	 fCos[iPlane] * trackPrm[0] + fSin[iPlane] * trackPrm[1]
	 : fCos[iPlane] * (trackPrm[0] + trackPrm[2] * fZ[iPlane])
	 + fSin[iPlane] * (trackPrm[1] + trackPrm[3] * fZ[iPlane]);
      const Double_t r = residual[iPlane] = pos[iPlane] - trackedPosition;
      SSR += r * r;
      if (SSR > fSSRMax) return fSSRMax;
   }
   return SSR;
}
