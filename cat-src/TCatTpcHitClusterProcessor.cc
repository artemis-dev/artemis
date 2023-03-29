/**
 * @file   TCatTpcHitClusterProcessor.cc
 * @brief  make cluster of tpc hits
 *
 * @date   Created       : 2016-04-17 04:46:27 JST
 *         Last Modified : 2020-05-20 11:04:41 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2016 Shinsuke OTA
 */

#include "TCatTpcHitClusterProcessor.h"

#include <TClonesArray.h>

#include <TCatReadoutPadArray.h>
#include <TCatReadoutPad.h>
#include <TCatPulseShape.h>
#include <TCatTrackResult.h>
#include <TCatTrackResultPulse.h>
#include <vector>
#include <list>
#include <TCatTpcTrack.h>
#include <TMath.h>
#include <TArtSystemOfUnit.h>
#include "TTpcStaticGasProperty.h"
using art::TCatTpcHitClusterProcessor;

ClassImp(TCatTpcHitClusterProcessor)


namespace {
   // typedef's for container
   typedef std::vector<art::TCatPulseShape*> pulseVec_t;
   typedef std::vector<pulseVec_t> clusterVec_t;
   typedef std::list<pulseVec_t> clusterList_t;

   template <class T>
   bool compare_size(std::vector<T> &first,std::vector<T> &second) {
      return (first.size() > second.size());
   }

   bool concatClusters(clusterVec_t &clusters, Double_t radZX, Double_t radY) {
      Bool_t found = kFALSE;
      const clusterVec_t::iterator end = clusters.end();
      clusterVec_t::iterator it1 = clusters.begin();
      clusterVec_t::iterator it2;
      for (; it1 < end; ++it1) {
         for (it2 = it1 + 1;  it2 < end; ++it2) {
            for (pulseVec_t::iterator it3 = (*it1).begin(), end3 = (*it1).end();  it3 != end3; ++it3) {
               for (pulseVec_t::iterator it4 = (*it2).begin(), end4 = (*it2).end();  it4 != end4; ++it4) {
                  art::TCatPulseShape *p1 = *it3;
                  art::TCatPulseShape *p2 = *it4;
                  TVector3 diff = p1->GetPos() - p2->GetPos();
                  if (TMath::Sqrt(diff.X()*diff.X() + diff.Z() * diff.Z()) < radZX &&
                      TMath::Abs(diff.Y()) < radY) {
                     found = kTRUE;
                     break;
                  }
               }
               if (found) break;
            }
            if (found) break;
         }
         if (found) break;
      }
      if (!found) return kFALSE;
      pulseVec_t &cls1 = (*it1);
      pulseVec_t &cls2 = (*it2);
      std::copy(cls2.begin(),cls2.end(),std::back_inserter(cls1));
      clusters.erase(it2);
      concatClusters(clusters,radZX,radY);
      return kTRUE;
   }
}

TCatTpcHitClusterProcessor::TCatTpcHitClusterProcessor()
{

   DoubleVec_t defClusterRadius;
   defClusterRadius.push_back(8.1);
   defClusterRadius.push_back(21);
   DoubleVec_t defPulseLength(2);
   defPulseLength[0] = 0;
   defPulseLength[1] = 100;



   Register(fInput("InputCollection","input pulse collection (for a specific region)",TString("atpulse")));
   Register(fOutput("OutputCollection","output pulse collection as a candidate of recoil track",TString("atcluster")));
   Register(fReadoutPadArray("ReadoutPad","Name of readout pad array",TString("readoutPad")));
   Register(fTpcProperty("TpcProperty","Name of tpc gas property",TString("tpcGasProperty")));
   Register(fClusterRadius("ClusterRadius","radius of cluster",defClusterRadius));
   Register(fMinClusterSize("MinClusterSize","required minimum cluster size ",5.));
   Register(fIsSingle("IsSingle","select maximum size cluster if 1 (default: 0)",kTRUE));
   Register(fMinHeight("MinHeight","minimum pulse height",0.));
   Register(fPulseLengthRange("PulseLengthRange","range of pulse length [start,end]. ([0,100])",defPulseLength));
   Register(fSelectIf("SelectIf","select all if 0, inside if 1 and outside if 2. The region is indicated by SelectedRegion",0));
   Register(fSelectedRegion("SelectedRegion","selected region defined by box [x1,y1,x2,y2]",DoubleVec_t(4,0.)));
   Register(fDoTrackUpdate("DoTrackUpdate","flag for updating track",kFALSE));
   RegisterHelper(&fTrackUpdateHelper);
   fTrackUpdateHelper.SetUpdateTracks((TClonesArray**)fOutput.PtrRef());
}

TCatTpcHitClusterProcessor::~TCatTpcHitClusterProcessor()
{
}

TCatTpcHitClusterProcessor::TCatTpcHitClusterProcessor(const TCatTpcHitClusterProcessor& rhs)
{
}

TCatTpcHitClusterProcessor& TCatTpcHitClusterProcessor::operator=(const TCatTpcHitClusterProcessor& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}


void TCatTpcHitClusterProcessor::Init(TEventCollection *col)
{
   //======================================================================
   // check the type of readout pad aray
   Int_t id = fReadoutPadArray->GetUniqueID() ;
   Info("Init","Readout Pad Type = %d",id);

   //======================================================================
   // cluster radii for XZ and Y, respectively
   if (((DoubleVec_t)fClusterRadius).size() != 2) {
      SetStateError(Form("ClusterRadius has %d components while ClusterRadius should have two values for ZX plane and Y direction, respectively\n",(Int_t)((DoubleVec_t)fClusterRadius).size()));
      return;
   }


   //======================================================================
   // cluster radii for XZ and Y, respectively
   DoubleVec_t &selectedRegion = fSelectedRegion.Value();
   if (fSelectIf) {
      if (selectedRegion.size() != 4) {
         SetStateError(Form("SelectedRegion should be defined by box [x1,y1,x2,y2] (size = %d)\n",(Int_t)selectedRegion.size()));
      }
      if (selectedRegion[0] > selectedRegion[2]) {
         Double_t tmp = selectedRegion[0] ;
         selectedRegion[0] = selectedRegion[2];
         selectedRegion[2] = tmp;
      }
      if (selectedRegion[1] > selectedRegion[3]) {
         Double_t tmp = selectedRegion[1] ;
         selectedRegion[1] = selectedRegion[3];
         selectedRegion[3] = tmp;
      }
   }
}

void TCatTpcHitClusterProcessor::Process()
{
   // clear 
   fOutput->Clear("C");

   // initialize local variables
   pulseVec_t input;
   clusterVec_t cluster;
   const DoubleVec_t& radius = fClusterRadius.Value();
   const DoubleVec_t& reg = fSelectedRegion.Value();

   Int_t nHits = fInput->GetEntriesFast();
   std::vector<bool> use(nHits,true);
   TCatPulseShape::SetSortType(TCatPulseShape::kID);
   TCatPulseShape::SetSortOrder(TCatPulseShape::kASC);
   fInput->Sort();
   fInput->Compress();
   

   // ignore bad pads, invalid pulse, outside/inside given region
   for (Int_t iHit = 0; iHit != nHits; iHit++) {
      TCatPulseShape *pulse = (TCatPulseShape*) fInput->UncheckedAt(iHit);
      TCatReadoutPad *pad = static_cast<TCatReadoutPad*>(fReadoutPadArray->UncheckedAt(pulse->GetID()));
      if (!pulse->IsValid() || pad->IsBad()) {
         use[iHit] = false;
         continue;
      }
      if (!fSelectIf) {
         // assume use is initialized to be true
         continue;
      }
      const Double_t padx = pulse->GetX();
      const Double_t pady = pulse->GetZ();
      const bool isInside = reg[0] < padx && padx < reg[2] && reg[1] < pady && pady < reg[3] ;
      use[iHit] = ((fSelectIf == 1) && isInside) || ((fSelectIf == 2) && !isInside);
   }

   for (Int_t iHit = 0; iHit != nHits; iHit++) {
      if (!use[iHit]) continue;
      input.push_back((TCatPulseShape*) fInput->UncheckedAt(iHit));
   }
   
//   printf("input size = %d\n",input.size());
   

   for (pulseVec_t::iterator it1 = input.begin(), itend = input.end(); it1 != itend; ++it1) {
      Bool_t used = kFALSE;      
      for (clusterVec_t::iterator itCluster = cluster.begin(), itClusterEnd = cluster.end();
           itCluster != itClusterEnd;
           ++itCluster) {
         for (pulseVec_t::iterator itHitInCls = (*itCluster).begin(), itEndHitInCls = (*itCluster).end();
              itHitInCls != itEndHitInCls;
              ++itHitInCls) {

            const TVector3& p1 = (*it1)->GetPos();
            const TVector3& p2 = (*itHitInCls)->GetPos();
            Double_t y1 = p1.Y();
            Double_t y2 = p2.Y();
            double dx = p2.X() - p1.X();
            double dz = p2.Z() - p1.Z();
            double mag = TMath::Sqrt(dx * dx + dz * dz);
            if (mag < radius[0] && TMath::Abs(y1-y2) < radius[1]) {
               (*itCluster).push_back(*it1);
               used = kTRUE;
               break;
            }
         }
         if (used) break;
      }
      if (!used) {
         cluster.resize(cluster.size() + 1);
         cluster.back().push_back(*it1);
      }
   }

   // find the neighbor clusters
   concatClusters(cluster,radius[0],radius[1]);
   
   clusterList_t candidates;
   for (clusterVec_t::iterator itCluster = cluster.begin(), itClusterEnd = cluster.end();
        itCluster != itClusterEnd; ++itCluster) {
//      printf("cluster[%d] size = %d\n",distance(itCluster,cluster.begin()),(*itCluster).size());
      if ((*itCluster).size() > fMinClusterSize) {
         candidates.push_back((*itCluster));
      }
   }

   candidates.sort(compare_size<TCatPulseShape*>);
   Int_t clusterID = 0;
   for (clusterList_t::iterator itc = candidates.begin(), itcEnd = candidates.end(); itc != itcEnd; ++itc,++clusterID) {
      TCatTpcTrack *track = static_cast<TCatTpcTrack*>(fOutput->ConstructedAt(fOutput->GetEntriesFast()));
      track->Init();
      track->EstimateFromHits(*itc);
      track->SetID(clusterID);
      track->SetTimestamp(track->Y() / fTpcProperty->GetDriftVelocity());
      if (fIsSingle) break;
   }

   fTrackUpdateHelper.StartCloseToOrigin();
   
   if (fDoTrackUpdate) {
      fTrackUpdateHelper.SetUpdateType(0,TCatTpcTrackUpdateHelper::kExtrapolateToOrigin);
      fTrackUpdateHelper.SetUpdateType(1,TCatTpcTrackUpdateHelper::kExtrapolateToOrigin);
      fTrackUpdateHelper.SetUpdateType(2,TCatTpcTrackUpdateHelper::kExtrapolateToOrigin);
      fTrackUpdateHelper.ExtrapolateToOrigin();
      fTrackUpdateHelper.SetUpdateType(0,TCatTpcTrackUpdateHelper::kNone);
      fTrackUpdateHelper.SetUpdateType(1,TCatTpcTrackUpdateHelper::kOffsetToOrigin);
      fTrackUpdateHelper.SetUpdateType(2,TCatTpcTrackUpdateHelper::kNone);
      fTrackUpdateHelper.OffsetToOrigin();
   }
#if 0
   for (Int_t i = 0, n = fOutput->GetEntriesFast(); i < n; ++i) {
      TCatTpcTrack *track = static_cast<TCatTpcTrack*>(fOutput->UncheckedAt(i));
      track->SetResults(track->GetHits(),track->GetStartPoint(),track->GetEndPoint(),&fChargeResponseHelper);
   }
#endif   
}
