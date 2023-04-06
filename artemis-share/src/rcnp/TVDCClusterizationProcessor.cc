/**
 * @file   TVDCClusterizationProcessor.cc
 * @brief VDC Clustering initiated by Y. Watanabe. 
 *
 * @date   Created:       2013-10-18 13:01:05
 *         Last Modified: 2023-03-03 17:04:34 JST
 * @author Shinsuke OTA <ota@rcnp.osaka-u.ac.jp>
 *
 *    Copyright (C) 2023 Shinsuke OTA All rights reserved.
 */

#include <algorithm>

#include "TVDCClusterizationProcessor.h"
#include "TMWDCParameter.h"
#include "TMWDCPlaneInfo.h"
#include "TTimingChargeData.h"
#include "TMWDCHitData.h"
#include "TVDCCluster.h"
#include "TVDCTrackingResult.h"
#include "TConverterBase.h"
#include "TConverterUtil.h"

#include "constant.h"

#include <TClass.h>
#include <TClonesArray.h>
#include <numeric>

using art::TVDCClusterizationProcessor;

ClassImp(art::TVDCClusterizationProcessor);

// Default constructor
  TVDCClusterizationProcessor::TVDCClusterizationProcessor()
: fClusterOut(NULL), fNPlane(kInvalidI), fNData(NULL), fConverterArray(NULL)
{

  //RegisterInputCollection("InputCollection","input",
  //	fInputColName,TString("mwdc1_raw"));
  //	&fMWDCIn,"TClonesArray","art::TMWDCHitData");
  RegisterInputCollection("InputCollection","input",
		fInputColName,TString("mwdc1_raw"),
		&fMWDCIn,"TClonesArray","art::TTimingChargeData");
  RegisterProcessorParameter("OutputCollection","name of mwdc",
		fOutputName,TString("mwdc1"));
  RegisterProcessorParameter("plane type","0: x, 1: u, 2: v",
		fPlaneType, 0);
  Register(fSearchTimeWidth("SearchTimeWidth","width of search time window for pre-clustering by timestamp",200));
  //
  //  RegisterOutputCollection("OutputCollection","output",
  //		fOutputColName,TString("mwdc1_track"));
  // const StringVec_t defConverterList(1,"dtdx");
  //  RegisterProcessorParameter("ConverterList","dt2dx conversion parameter",
  //		fConverterList,defConverterList);
  // RegisterProcessorParameter("SortType","0: ID, 1: Timing, 2: Charge (Default), 3: DriftLength",
  //		fSortTypeInput, (Int_t)kCharge);
  //  RegisterProcessorParameter("MAXDL","max drift length",
  //		fMAXDL, 10);
  // RegisterProcessorParameter("SortOrder","0: ascending, 1: descending (Default)",
  //		fSortOrderInput, 0);//(Int_t)kDESC);
  // RegisterProcessorParameter("V1190","0: V1190, 1:3377",
  //		fV1190, 0);//(Int_t)kDESC);
}

TVDCClusterizationProcessor::~TVDCClusterizationProcessor()
{
  //  delete fConverterArray;
  //  if(fPlaneOut) {
  //	 for (Int_t i = 0; i != fNPlane ; ++i) delete fPlaneOut[i];
  //	 delete [] fPlaneOut;
  //  }	
  if(fClusterOut){
	 delete fClusterOut;
  }

  //  delete [] fNData;
}

void TVDCClusterizationProcessor::Init(TEventCollection *col)
{
  fClusterOut = new TClonesArray(TVDCCluster::Class(),1);
  fClusterOut->SetName(fOutputName);

  col->Add(fOutputName,fClusterOut,fOutputIsTransparent);
}

void TVDCClusterizationProcessor::Process()
{
  int debug = 0;
  if (fVerboseLevel > 2)  printf("newevent---------------------------\n");
  fClusterOut->Clear("C");

  //input
  int hitid[400];
  int hitid_raw[400];
  int hitinplane;
  int hitinplane_raw;
  double timing[400];
  double timing_raw[400];
  double dl[400];
  double dl_raw[400];

  //	 TClonesArray *const Plane= *(input->at(0));

  if(debug)printf("1\n");
  //TVDCTrackingResult* GR= (TVDCTrackingResult*)(**ref)[0];
  // printf("entry=%d\n",(*fMWDCIn)->GetEntriesFast());

  int entry = (*fMWDCIn)->GetEntriesFast();
  int entry2=0;

  if (!(*fMWDCIn)->GetEntriesFast()) return ;


  // Clustering by timestamp
  TTimingChargeData::SetSortType(TTimingChargeData::kTiming);
  TTimingChargeData::SetSortOrder(TTimingChargeData::kASC);
  const int n = (*fMWDCIn)->GetEntriesFast();
  TObjArray arr(n);
  for (Int_t i = 0; i < n; ++i) {
     arr[i] = (*fMWDCIn)->UncheckedAt(i);
  }
  arr.Sort();

  if (fVerboseLevel > 2) printf("n = %d\n",n);

  using objvec = std::vector<TMWDCHitData*>;
  std::vector<objvec> timeClusters;
  objvec hits;
  for (Int_t i = 0; i < n; ++i) {
     bool isFound = false;
     TMWDCHitData* hit = (TMWDCHitData*)arr[i];
     if (fVerboseLevel > 2) printf("id = %d, timestamp = %f, timing = %f, driftLength = %f\n",hit->GetID(),hit->GetTimestamp(),hit->GetTiming(),hit->GetDriftLength());
     if (!hit) continue;
     if (hits.size() == 0) {
        hits.push_back(hit);
        timeClusters.push_back(hits);
        continue;
     }
     for (auto& aCls : timeClusters) {
        for (auto& aHit : aCls) {
           if (fVerboseLevel > 2)  printf("timestamp %f %f %f\n",aHit->GetTimestamp(),hit->GetTimestamp(),fSearchTimeWidth.Value());
           if (TMath::Abs(aHit->GetTimestamp() - hit->GetTimestamp()) < fSearchTimeWidth) {
              // add this hit since it is in the same time window
              aCls.push_back(hit);
              isFound = true;
              break;
           }
        }
        if (isFound) break;
     }
     objvec newhits(1,hit);
     timeClusters.push_back(newhits);
  }
  if (fVerboseLevel > 2)  {
     printf("num of timeClusters : %d\n",timeClusters.size());
     for (int i = 0; i < timeClusters.size(); ++i) {
        printf(" size of cls[%d] : %d \n",i,timeClusters[i].size());
     }
  }

  for (auto it = timeClusters.begin(); it != timeClusters.end();) {
     if (it->size() < 2 || it->size() > 6) {
        it = timeClusters.erase(it);
     } else {
        ++it;
     }
  }

  using objvec = std::vector<TMWDCHitData*>;
  std::vector<std::vector<objvec>> clusters;
  TMWDCHitData* last = NULL;
  std::vector<objvec> wires;
  objvec mhits;
  // find geometrical cluster in each time cluster
  for (auto& aCls : timeClusters) {
     std::sort(aCls.begin(),aCls.end(),[](TMWDCHitData* lhs, TMWDCHitData*  rhs) {
                                          return lhs->GetID() < rhs->GetID();
                                       });
     // continuity check (strong request)
     for (auto& aHit : aCls) {
       if (fVerboseLevel > 2) { printf("id = %d\n",aHit->GetID()); }
        if (!last) {
           // first hit
           mhits.push_back(aHit);
           last = aHit;
        } else if (last->GetID() == aHit->GetID()) {
           // accept the existing wire id for a multi hit
           mhits.push_back(aHit);
           last = aHit;
        } else if (last->GetID() + 1 == aHit->GetID()) {
           wires.push_back(mhits);
           mhits.clear();
           mhits.push_back(aHit);
           last = aHit;
	} else {
           // gap
           wires.push_back(mhits);
           clusters.push_back(wires);
           for (auto& aWire : wires) {
              aWire.clear();
           }
           wires.clear();
           mhits.clear();
           mhits.push_back(aHit);
           last = aHit;
        } 
     }
     wires.push_back(mhits);
     clusters.push_back(wires);
     for (auto& aWire : wires) {
       aWire.clear();
     }
     wires.clear();
     mhits.clear();
     last = NULL;
  }

#if 1
  if (fVerboseLevel > 2) {
     printf("num of clusters : %d\n",clusters.size());
     for (int i = 0; i < clusters.size(); ++i) {
        printf(" size of cls[%d] : %d \n",i,clusters[i].size());
        for (int j = 0; j < clusters[i].size(); j ++) {
           printf("  multiplicity [%d] : %d \n",j,clusters[i][j].size());
           for (int k = 0; k < clusters[i][j].size(); k ++) {
              printf("   id = %d\n",clusters[i][j][k]->GetID());
           }
        }
     }
  }
#endif

  for (int iCls = 0, nCls = clusters.size(); iCls < nCls; ++iCls) {
     ProcessCluster(clusters[iCls]);
  }

  
  if(debug)printf("2\n");
}


double linearFit(const std::vector<double>& x, const std::vector<double>& y, double& a, double& b) {
    const auto n    = x.size();
    const auto s_x  = std::accumulate(x.begin(), x.end(), 0.0);
    const auto s_y  = std::accumulate(y.begin(), y.end(), 0.0);
    const auto s_xx = std::inner_product(x.begin(), x.end(), x.begin(), 0.0);
    const auto s_xy = std::inner_product(x.begin(), x.end(), y.begin(), 0.0);
    a    = (n * s_xy - s_x * s_y) / (n * s_xx - s_x * s_x);
    b    = (s_xx * s_y - s_x * s_xy) / (n * s_xx - s_x * s_x);
    double chi2 = 0;
    for (int i = 0; i < x.size(); ++i) {
       double arg = y[i] - a * x[i] - b;
       chi2 += arg * arg;
    }
    return chi2;
}

void TVDCClusterizationProcessor::ProcessCluster(std::vector<std::vector<TMWDCHitData*>> wires)
{
   int nComb = 1;
   int nWrs  = wires.size();
   if (nWrs < 3) return;
   std::vector<std::vector<TMWDCHitData*>> comb(1,std::vector<TMWDCHitData*>());
   for (int iWrs = 0; iWrs < nWrs; ++iWrs) {
      int nHits = wires[iWrs].size();
      std::vector<std::vector<TMWDCHitData*>> comb_orig = comb;
      for (int iHits = 1; iHits < nHits; ++iHits) {
         comb.insert(comb.end(),comb_orig.begin(),comb_orig.end());
      }
      for (int iHits = 0; iHits < nHits; ++iHits) {
         for (int iComb = 0; iComb < nComb; ++iComb) {
            comb[iComb+iHits*nComb].push_back(wires[iWrs][iHits]);
         }
      }
      nComb *= wires[iWrs].size();
   }

   int nPattern = nComb * (1<<nWrs);

   double bestChi2 = TMath::Limits<double>::Max();
   double bestPos;
   double bestSlope;
   double bestOffset;
   std::vector<double> dl(nWrs);
   std::vector<double> bestDL(nWrs);
   std::vector<double> id(nWrs);
   std::vector<double> bestID(nWrs);
   std::vector<double> ts(nWrs);
   std::vector<double> bestT(nWrs);

   for (int iComb = 0; iComb < nComb; ++iComb) {
      if (fVerboseLevel > 2) printf ("iComb = %d\n",iComb);
      std::vector<TMWDCHitData*>& hits = comb[iComb];
      for (auto aHit : hits) {
         if (fVerboseLevel > 2) printf("  id = %d, timing = %f, drift = %f\n",aHit->GetID(),aHit->GetTiming(),aHit->GetDriftLength());
      }
      for (int ip = 0, np = (1<<nWrs); ip < np; ++ip) {
         for (int iWrs = 0; iWrs < nWrs; ++iWrs) {
            int ud = ((ip >> iWrs)&0x1);
            TMWDCHitData *hit = comb[iComb][iWrs];
            double driftLength = hit->GetDriftLength();
            dl[iWrs] =  ud ? driftLength : -driftLength;
            ts[iWrs] = hit->GetTiming();
            id[iWrs] =  hit->GetID();
         }
         double slope = 0;
         double offset = 0;
         double chi2 = linearFit(id,dl,slope,offset);
         if (chi2 < bestChi2) {
            bestChi2 = chi2;
            bestOffset = offset;
            bestSlope = slope;
            for (int iwrs = 0; iwrs < nWrs; ++iwrs) {
               bestDL[iwrs] = dl[iwrs];
               bestID[iwrs] = id[iwrs];
               bestT[iwrs] = ts[iwrs];
            }
//            std::copy(dl.begin(),dl.end(),bestDL.begin());
//            std::copy(id.begin(),id.end(),bestID.begin());
//            std::copy(ts.begin(),ts.end(),bestT.begin());
         }
      }
      if (fVerboseLevel > 2) printf("slope = %f, offset = %f, chi2 = %f, pos = %f\n",bestSlope, bestOffset, bestChi2, - bestOffset / bestSlope);
   }
   TVDCCluster *output = (TVDCCluster*)(fClusterOut->ConstructedAt(fClusterOut->GetEntriesFast()));
   output->Init2(nWrs);
   output->SetTimestamp(wires[0][0]->GetTimestamp());
   output->SetHitPos(-bestOffset / bestSlope);
   output->SetHitAngle(((fPlaneType%2) == 0 ? -6 : -4)/10./bestSlope);
   output->SetClustID(0);
   output->SetClustSize(nWrs);
   output->SetClustnum(nWrs);
   output->SetClustnum2(nWrs);
   for (int i = 0; i < bestID.size(); i++) {
     if (fVerboseLevel > 2) printf("bestID[%d] = %f, bestDL[%d] = %f\n",i,bestID.at(i),i,bestDL.at(i));
      output->SetHitID(i,bestID[i]);
      output->SetHitDL(i,bestDL[i]);
      output->SetHitTiming(i,bestT[i]);
   }      
                       
}
