/* @file TCatClusterSumProcessor.cc
 * @brief sum up the hits along given axis
 *
 * @date Create        : 2019-11-07 21:47:48 JST
 *       Last Modified : 2019-11-07 22:58:22 JST (ota)
 * @author: Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 */


#include "TCatClusterSumProcessor.h"
#include "TCatTrackResultPulse.h"

ClassImp(art::TCatClusterSumProcessor)

using art::TCatClusterSumProcessor;

TCatClusterSumProcessor::TCatClusterSumProcessor()
   : fHitHelper(NULL)
{
   Register(fInput("Input","Name of input","atcluster"));
   Register(fOutput("Output","Name of output","atclustersum"));
   Register(fAxisType("AxisType","type of axis along  which the hits are sum",0));
   
}

TCatClusterSumProcessor::~TCatClusterSumProcessor()
{
   if (fHitHelper) {
      fHitHelper->Clear("C");
      delete fHitHelper;
   }
}

void TCatClusterSumProcessor::Init(TEventCollection *col)
{
   fHitHelper = new TClonesArray(TCatTrackResultPulse::Class());
}

void TCatClusterSumProcessor::Process()
{
   fOutput->Clear("C");
   fHitHelper->Clear("C");
   const Int_t nCls = fInput->GetEntriesFast();
   if (!nCls) return;
   for (Int_t iCls = 0; iCls < nCls; ++iCls) {
      TCatTpcTrack* aCls = static_cast<TCatTpcTrack*>(fInput->UncheckedAt(iCls));
      TCatTpcTrack* output = static_cast<TCatTpcTrack*>(fOutput->ConstructedAt(iCls));
      const Int_t nHits = aCls->GetHits().size();
      std::vector<TCatTrackResultPulse*> hits(nHits);
      std::vector<TCatPulseShape*> outputhits;
      for (Int_t iHit = 0; iHit < nHits; ++iHit) {
         hits[iHit] = static_cast<TCatTrackResultPulse*>(aCls->GetHits()[iHit]);
      }

      // only for CAT-M and only for x direction
      for (Int_t iHit = 0; iHit < nHits; ++iHit) {
         if (hits[iHit] == NULL) continue;
         Int_t id0 = TMath::FloorNint((hits[iHit]->GetID() % 2024) / 46);
         double charge = hits[iHit]->GetCharge();
         double maxSample = hits[iHit]->GetMaxSample();
         double x = hits[iHit]->GetX() * charge;
         double y = hits[iHit]->GetY() * charge;
         double z = hits[iHit]->GetZ() * charge;
         for (Int_t iHit1 = iHit + 1; iHit1 < nHits; ++iHit1) {
            if (hits[iHit1] == NULL) continue;
            
            Int_t id1 = TMath::FloorNint((hits[iHit1]->GetID() % 2024) / 46);
            if (id0 != id1) continue;
            double charge1 = hits[iHit1]->GetCharge();
            charge += charge1;
            maxSample += hits[iHit1]->GetMaxSample();
            x += hits[iHit1]->GetX() * charge1;
            y += hits[iHit1]->GetY() * charge1;
            z += hits[iHit1]->GetZ() * charge1;
            hits[iHit1] = NULL;
         }
         TCatTrackResultPulse* oHit = static_cast<TCatTrackResultPulse*>(fHitHelper->ConstructedAt(fHitHelper->GetEntriesFast()));
         hits[iHit]->Copy(*oHit);
         hits[iHit] = NULL;
         oHit->SetID(id0);
         oHit->SetCharge(charge);
         oHit->SetX(x/charge);
         oHit->SetY(y/charge);
         oHit->SetZ(z/charge);
         oHit->SetMaxSample(maxSample);
         outputhits.push_back(oHit);
      }

      output->Init();
      output->EstimateFromHits(outputhits);
      output->SetID(iCls);
   }
}
