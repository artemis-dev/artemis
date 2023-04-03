/**
 * @file   TCatTPCRecoilCandidateRescanByDistanceProcessor.cc
 * @brief
 *
 * @date   Created       : 2016-05-20 12:11:57 JST
 *         Last Modified : 2016-07-20 11:27:21 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2016 Shinsuke OTA
 */

#include "TCatTPCRecoilCandidateRescanByDistanceProcessor.h"
#include <TClonesArray.h>
#include <TCatPulseShape.h>

using art::TCatTPCRecoilCandidateRescanByDistanceProcessor;

ClassImp(TCatTPCRecoilCandidateRescanByDistanceProcessor)

TCatTPCRecoilCandidateRescanByDistanceProcessor::TCatTPCRecoilCandidateRescanByDistanceProcessor()
{
   RegisterInputCollection("InputCandidateCollection","input pulse collection as a candidate of recoil track",
                           fInputCandidateName,TString("atrc"),
                           &fInputCandidate,TClonesArray::Class_Name(),TCatPulseShape::Class_Name());
}

TCatTPCRecoilCandidateRescanByDistanceProcessor::~TCatTPCRecoilCandidateRescanByDistanceProcessor()
{
}

TCatTPCRecoilCandidateRescanByDistanceProcessor::TCatTPCRecoilCandidateRescanByDistanceProcessor(const TCatTPCRecoilCandidateRescanByDistanceProcessor& rhs)
{
}

TCatTPCRecoilCandidateRescanByDistanceProcessor& TCatTPCRecoilCandidateRescanByDistanceProcessor::operator=(const TCatTPCRecoilCandidateRescanByDistanceProcessor& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}

void TCatTPCRecoilCandidateRescanByDistanceProcessor::Process()
{
   // clear
   fOutput->Clear("C");
   
   std::list<TCatPulseShape*> inputCandidates;
   std::list<TCatPulseShape*> inputPulses;
   std::list<TCatPulseShape*> cluster;

   Int_t nHits = (*fInput)->GetEntriesFast();
   // ignore beam particle
   for (Int_t iHit = 0; iHit != nHits; iHit++) {
      TCatPulseShape *pulse = (TCatPulseShape*) (*fInput)->UncheckedAt(iHit);
      if (pulse->GetX() < fBeamRegion[0] || fBeamRegion[1] < pulse->GetX()) {
         inputPulses.push_back(pulse);
      }        
   }
   nHits = (*fInputCandidate)->GetEntriesFast();
   // ignore beam particle
   for (Int_t iHit = 0; iHit != nHits; iHit++) {
      TCatPulseShape *pulse = (TCatPulseShape*) (*fInputCandidate)->UncheckedAt(iHit);
      if (pulse->GetX() < fBeamRegion[0] || fBeamRegion[1] < pulse->GetX()) {
         inputCandidates.push_back(pulse);
      }        
   }
   
   std::list<TCatPulseShape*>::iterator it1 = inputPulses.begin();
   std::list<TCatPulseShape*>::iterator itend = inputPulses.end();

   while (it1 != itend) {
      // loop over all the pulses
      std::list<TCatPulseShape*>::iterator itrc1 = inputCandidates.begin();
      std::list<TCatPulseShape*>::iterator itrcend = inputCandidates.end();
      while (itrc1 != itrcend) {
         if (((*it1)->GetPos() - (*itrc1)->GetPos()).Mag() < fClusterRadius[0]) {
            cluster.push_back(*it1);
            break;
         }
         ++itrc1;
      }
      ++it1;
   }

   std::list<TCatPulseShape*>::iterator it= cluster.begin();
   std::list<TCatPulseShape*>::iterator itEnd= cluster.end();
   for (; it!=itEnd; ++it) {
      TCatPulseShape *out = static_cast<TCatPulseShape*>(fOutput->ConstructedAt(fOutput->GetEntriesFast()));
      (*it)->Copy(*out);
   }
   
}
