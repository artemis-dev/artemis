/**
 * @file   TVDCTrackingProcessor.h
 * @brief based on TMWDCTrackingProcessor
 *
 * @date   Created:       2023-03-02 19:01:37 JST
 *         Last Modified: 2023-03-03 00:09:00 JST
 * @author Shinsuke OTA <ota@rcnp.osaka-u.ac.jp>
 *
 *    Copyright (C) 2023 OTA Shinsuke All rights reserved
 */

#include "TVDCTrackingProcessor.h"
#include "TMWDCTrackingResult.h"
#include "TMWDCParameter.h"
#include "TMWDCPlaneInfo.h"
#include "TVDCCluster.h"


#include "TStopwatch.h"
#include "TClonesArray.h"
#include "TMath.h"
#include "TMatrixD.h"
#include "TVector3.h"

#include <numeric>

ClassImp(art::TVDCTrackingProcessor)

art::TVDCTrackingProcessor::TVDCTrackingProcessor()
{
   Register(fSearchTimeWidth("SearchTimeWidth","search window width of timestamp",10));
   Register(fTiltedAngleY("TiltedAngleY","tilted angle of whole system along y axis in deg",45));
}

art::TVDCTrackingProcessor::~TVDCTrackingProcessor()
{
}



void art::TVDCTrackingProcessor::Process()
{

//   Info("Process","========== New Event ==========");
   fStopwatch->Start();
   fMWDCTrackOut->Clear("C");


   std::vector<std::vector<TVDCCluster*>> planeSets;
   
   
   // find cluster combination
   for (int iPlane = 0; iPlane < fNPlane; ++iPlane) {
      TClonesArray* plane = *(fPlaneIn->at(iPlane));
      if (fVerboseLevel > 2) printf("=== %d clusters ===\n",plane->GetEntries());
      for (int iHit = 0, nHits = plane->GetEntriesFast(); iHit < nHits; ++iHit) {
         TVDCCluster* planeCluster = (TVDCCluster*)plane->At(iHit);
	 if (fVerboseLevel > 2)  printf("Hit Pos = %10f,  Timestamp = %20f\n",planeCluster->GetHitPos(),planeCluster->GetTimestamp());
         if (iPlane == 0) {
            // this is the first hit
            planeSets.push_back(std::vector<TVDCCluster*>(1,planeCluster));
            continue;
         }
         for (auto& aSet : planeSets) {
	   if (fVerboseLevel > 2)  printf("iPlane[%d] Timestamp = %20f %20f Width = %20f\n",iPlane,planeCluster->GetTimestamp(),aSet[0]->GetTimestamp(),fSearchTimeWidth.Value());
            if (abs(planeCluster->GetTimestamp() - aSet[0]->GetTimestamp()) < fSearchTimeWidth.Value()) {
               aSet.push_back(planeCluster);
               break;
            }
         }
      }
   }
   if (fVerboseLevel > 2) Info("Process","num of sets = %d",planeSets.size());
   
   // check required planes
   for (auto it = planeSets.begin(); it != planeSets.end(); ) {
//      Info("Process","size of sets = %d",it->size());
      if (it->size() != 4) {
         it = planeSets.erase(it);
      } else {
         ++it;
      }
   }

   for (auto& planes : planeSets) {
//      Info("Process","planes size = %d",planes.size());
      ProcessOne(planes);
   }

}


void art::TVDCTrackingProcessor::ProcessOne(const std::vector<TVDCCluster*>& planes)
{
   TMWDCTrackingResult *tr = (TMWDCTrackingResult*)fMWDCTrackOut->ConstructedAt(fMWDCTrackOut->GetEntriesFast());

   tr->Init(fNPlane);
   tr->SetNPlaneValid(fNPlane);
   tr->SetTimestamp(planes[0]->GetTimestamp());
   tr->SetNParameter(fNParameter);
   tr->SetNCombination(1);

   FindTrack(planes,tr,true);
   
}

void art::TVDCTrackingProcessor::FindTrack(const std::vector<TVDCCluster*>& planes, TMWDCTrackingResult *tr, bool doFillStat)
{
   std::vector<Double_t> pos(fNPlane), trackPrm(fNParameter);
   std::vector<Double_t> bestTrackPrm(fNParameter,0);
   std::vector<Double_t> residual(fNPlane,0);
   std::vector<Double_t> bestResidual(fNPlane,0);
   
   for (int iPlane = 0; iPlane != fNPlane; ++iPlane) {
      const Double_t cos = fCos[iPlane];
      const Double_t sin = fSin[iPlane];
      const Double_t x   = fX[iPlane];
      const Double_t y   = fY[iPlane];
      
      const TMWDCPlaneInfo &planeInfo = *fPlaneInfo->at(iPlane);
      const Double_t cellSize = planeInfo.GetCellSize();
      const Double_t center   = planeInfo.GetCenter();
      TVDCCluster* planeData = planes[iPlane];
      pos[iPlane] = (planeData->GetHitPos() - center) * cellSize + x * cos + y * sin;
   }
   const TMatrixD &g = *fGMatrix;
   for (Int_t iParameter = 0; iParameter!=fNParameter; ++iParameter) {
      // calculate pos[] \dot g[iParameter][]
      trackPrm[iParameter] = std::inner_product(pos.begin(),pos.end(),
                                                g[iParameter].GetPtr(),0.);
   }
   const Double_t SSR = CalcSSR(pos,trackPrm,residual);
   std::copy(trackPrm.begin(),trackPrm.end(),bestTrackPrm.begin());
   std::copy(residual.begin(),residual.end(),bestResidual.begin());
   

   if (fNParameter == 2) {
      tr->SetTrack(bestTrackPrm[0],bestTrackPrm[1],0,0,0);
   } else {

      TVector3 vec(bestTrackPrm[2],bestTrackPrm[3],1);
//      vec.Print();
      vec.RotateY(fTiltedAngleY*TMath::DegToRad());
//      vec.Print();
      
      tr->SetTrack(bestTrackPrm[0],bestTrackPrm[1],0,
                   vec.X()/vec.Z(),
                   vec.Y()/vec.Z());
   }
   tr->SetSSR(SSR);

   for(Int_t i=0;i!=fNPlane;++i){
      // tr->SetWireIDAdopted(i,->GetDetID());
      // tr->SetDriftLengthAdopted(i,fPlaneData[i]->GetDriftLength());
      // tr->SetResidual(i,bestResidual[i]);
   }

   if (SSR < fSSRMax) {
      tr->SetTrackingID(TMWDCTrackingResult::kGood);
      if (doFillStat) { /* fill statistical information */
	 const Int_t ndf = fNPlaneValid - fNParameter;
	 const Double_t standardCI = fNPlaneValid > CRITICAL_NPLANE + 1 ?  TMath::StudentQuantile(1.- fSignificanceLevel/2., ndf) : 0;
	 const Double_t usv = SSR/ndf; // sample variance
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
