/**
 * @file   TCatTPCRecoilCandidateSelectProcessor.cc
 * @brief  select recoil candidate
 *
 * @date   Created       : 2016-04-17 04:46:27 JST
 *         Last Modified : 2018-02-04 13:16:13 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2016 Shinsuke OTA
 */

#include "TCatTPCRecoilCandidateSelectProcessor.h"

#include <TClonesArray.h>

#include <TCatReadoutPadArray.h>
#include <TCatPulseShape.h>
#include <vector>
#include <list>
#include <TMatrixD.h>
#include <TMatrixDSym.h>
#include <TMatrixDSymEigen.h>
#include <TVector3.h>
#include <TMath.h>

using art::TCatTPCRecoilCandidateSelectProcessor;

ClassImp(TCatTPCRecoilCandidateSelectProcessor)


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

TCatTPCRecoilCandidateSelectProcessor::TCatTPCRecoilCandidateSelectProcessor()
: fInputName(""), fInput(NULL), fOutputName(""), fOutput(NULL), fReadoutPadArrayName(""), fReadoutPadArray(NULL)
{
   FloatVec_t beamRegion;
   beamRegion.push_back(-18);
   beamRegion.push_back(18);

   FloatVec_t defClusterRadius;
   defClusterRadius.push_back(8.1);
   defClusterRadius.push_back(21);

   RegisterInputCollection("InputCollection","input pulse collection",fInputName,TString("atpulse"),
                           &fInput,TClonesArray::Class_Name(),TCatPulseShape::Class_Name());
   RegisterOutputCollection("OutputCollection","output pulse collection as a candidate of recoil track",fOutputName,TString("atrc"),
                           &fOutput,TClonesArray::Class_Name(),TCatPulseShape::Class_Name());
   RegisterOutputCollection("NameInertia","output moment of inertia",fNameInertia,TString("inertia"),
                            "D",&fInertia);
   RegisterOutputCollection("NamePrincipalAxis","output name of principal axis",fNamePrincipalAxis,TString("principalAxis"),
                            &fPrincipalAxis,TVector3::Class_Name());
   RegisterOutputCollection("NameDeviation","output name of deviation around principal axis",fNameDeviation,TString("deviation"),
                            &fDeviation,TVector3::Class_Name());
   RegisterInputInfo("ReadoutPad","Name of readout pad array",fReadoutPadArrayName,TString("readoutPad"),
                     &fReadoutPadArray,TCatReadoutPadArray::Class_Name());
   RegisterProcessorParameter("ClusterRadius","radius of cluster",fClusterRadius,defClusterRadius);
   RegisterProcessorParameter("MinClusterSize","required minimum cluster size ",fMinClusterSize,(Float_t)5);
   RegisterOptionalParameter("BeamRegion","Range of x position for beam region: default (-18,18)",
                             fBeamRegion,beamRegion);
                     
}

TCatTPCRecoilCandidateSelectProcessor::~TCatTPCRecoilCandidateSelectProcessor()
{
}

TCatTPCRecoilCandidateSelectProcessor::TCatTPCRecoilCandidateSelectProcessor(const TCatTPCRecoilCandidateSelectProcessor& rhs)
{
}

TCatTPCRecoilCandidateSelectProcessor& TCatTPCRecoilCandidateSelectProcessor::operator=(const TCatTPCRecoilCandidateSelectProcessor& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}


void TCatTPCRecoilCandidateSelectProcessor::Init(TEventCollection *col)
{
   //======================================================================
   // check the type of readout pad aray
   Int_t id;
   if ((id = (*fReadoutPadArray)->GetUniqueID()) != 2) {
      SetStateError(Form("Invadlid readout pad type %d. (This processor is currently valid for type 2)",id));
      return;
   } else {
      Info("Init","Type = %d",id);
   }
   //======================================================================
   // sort the x position of beam region
   if (fBeamRegion[0] > fBeamRegion[1]) {
      Float_t tmp = fBeamRegion[0];
      fBeamRegion[0] = fBeamRegion[1];
      fBeamRegion[1] = tmp;
   }
   if (fClusterRadius.size() != 2) {
      SetStateError(Form("ClusterRadius has %d components while ClusterRadius should have two values for ZX plane and Y direction, respectively\n",(Int_t)fClusterRadius.size()));
      return;
   }
}

void TCatTPCRecoilCandidateSelectProcessor::Process()
{
   
   // clear 
   fOutput->Clear("C");
   fDeviation->SetXYZ(TMath::QuietNaN(),TMath::QuietNaN(),TMath::QuietNaN());
   fPrincipalAxis->SetXYZ(TMath::QuietNaN(),TMath::QuietNaN(),TMath::QuietNaN());
   (*fInertia) = TMath::QuietNaN();

   // initialize local variables
   pulseVec_t input;
   clusterVec_t cluster;

   Int_t nHits = (*fInput)->GetEntriesFast();
   TCatPulseShape::SetSortType(TCatPulseShape::kID);
   TCatPulseShape::SetSortOrder(TCatPulseShape::kASC);
   (*fInput)->Sort();

   // ignore beam particle
   for (Int_t iHit = 0; iHit != nHits; iHit++) {
      TCatPulseShape *pulse = (TCatPulseShape*) (*fInput)->UncheckedAt(iHit);
      if (pulse->GetX() < fBeamRegion[0] || fBeamRegion[1] < pulse->GetX()) {
         input.push_back(pulse);
      }        
   }

   for (pulseVec_t::iterator it1 = input.begin(), itend = input.end(); it1 != itend; ++it1) {
      Bool_t used = kFALSE;      
      for (clusterVec_t::iterator itCluster = cluster.begin(), itClusterEnd = cluster.end();
           itCluster != itClusterEnd;
           ++itCluster) {
         for (pulseVec_t::iterator itHitInCls = (*itCluster).begin(), itEndHitInCls = (*itCluster).end();
              itHitInCls != itEndHitInCls;
              ++itHitInCls) {

            TVector3 p1 = (*it1)->GetPos();
            TVector3 p2 = (*itHitInCls)->GetPos();
            Double_t y1 = p1.Y(); p1.SetY(0);
            Double_t y2 = p2.Y(); p2.SetY(0);

            if ((p1-p2).Mag() < fClusterRadius[0] && TMath::Abs(y1-y2) < fClusterRadius[1]) {
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
   concatClusters(cluster,fClusterRadius[0],fClusterRadius[1]);
   
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
      pulseVec_t::iterator it= (*itc).begin();
      pulseVec_t::iterator itEnd= (*itc).end();
      for (; it!=itEnd; ++it) {
         TCatPulseShape *out = static_cast<TCatPulseShape*>(fOutput->ConstructedAt(fOutput->GetEntriesFast()));
         (*it)->Copy(*out);
         out->SetAuxID(clusterID);
      }
      // only largest 
      break;
   }

   nHits = fOutput->GetEntriesFast();
   if (!nHits) return;
   Double_t sumX = 0;
   Double_t sumY = 0;
   Double_t sumZ = 0;
   Double_t sumXX = 0;
   Double_t sumYY = 0;
   Double_t sumZZ = 0;
   Double_t sumXY = 0;
   Double_t sumYZ = 0;
   Double_t sumZX = 0;
   Double_t sum = 0;
   Double_t mx, my,mz;
             
   for (Int_t i=0; i!=nHits; i++) {
      TCatPulseShape *out = static_cast<TCatPulseShape*>(fOutput->UncheckedAt(i));
      const Double_t &c = out->GetCharge();
      sum += c;
      sumX += c * out->GetX();
      sumY += c * out->GetY();
      sumZ += c * out->GetZ();
   }
   mx = sumX/sum;
   my = sumY/sum;
   mz = sumZ/sum;
   for (Int_t i=0; i!=nHits; i++) {
      TCatPulseShape *out = static_cast<TCatPulseShape*>(fOutput->UncheckedAt(i));
      const Double_t &c = out->GetCharge();
      Double_t dx = out->GetX() - mx;
      Double_t dy = out->GetY() - my;
      Double_t dz = out->GetZ() - mz;
      sumXX += c * dx * dx;
      sumXY += c * dx * dy;
      sumYY += c * dy * dy;
      sumYZ += c * dy * dz;
      sumZZ += c * dz * dz;
      sumZX += c * dz * dx;
   }

   Double_t e[9] = {
      sumYY+sumZZ, -sumXY, -sumZX,
      -sumXY, sumZZ+sumXX,-sumYZ,
      -sumZX, -sumYZ, sumXX+sumYY
   };

   TMatrixDSym mat(3,e);
   TMatrixDSymEigen me(mat);

   TVectorD eigenval = me.GetEigenValues();
   TMatrixD eigenvec = me.GetEigenVectors();

   (*fInertia) = eigenval[2];
   (fPrincipalAxis)->SetXYZ(eigenvec[0][2],eigenvec[1][2],eigenvec[2][2]);
   // calculate rms of XY and XZ distributions
   // y = y0 + p1 * (x - x0)
   Double_t varx = 0; // variance in yz
   Double_t vary = 0; // variance in zx
   Double_t varz = 0; // variance in xy
   Double_t vx = fPrincipalAxis->X();
   Double_t vy = fPrincipalAxis->Y();
   Double_t vz = fPrincipalAxis->Z();
   for (Int_t i=0; i!=nHits; i++) {
      TCatPulseShape *out = static_cast<TCatPulseShape*>(fOutput->UncheckedAt(i));
      const Double_t &c = out->GetCharge();
      Double_t x = out->GetX();
      Double_t y = out->GetY();
      Double_t z = out->GetZ();
      Double_t dxy = TMath::Abs(vy * x - vx * y - vy * mx + vx * my) / TMath::Sqrt(vy * vy + vx * vx);
      Double_t dyz = TMath::Abs(vz * y - vy * z - vz * my + vy * mz) / TMath::Sqrt(vz * vz + vy * vy);
      Double_t dzx = TMath::Abs(vx * z - vz * x - vx * mz + vz * mx) / TMath::Sqrt(vx * vx + vz * vz);
      varx += c * dyz * dyz;
      vary += c * dzx * dzx;
      varz += c * dxy * dxy;
   }
   vary = TMath::Sqrt(vary/sum);
   varz = TMath::Sqrt(varz/sum);
   varx = TMath::Sqrt(varx/sum);
   fDeviation->SetXYZ(varx,vary,varz);
}
