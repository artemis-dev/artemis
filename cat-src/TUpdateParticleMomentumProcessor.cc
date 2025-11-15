/**
 * @file   TUpdateParticleMomentumProcessor.cc
 * @brief  update particle momentum using track information
 *
 * @date   Created       : 2017-09-26 19:35:22 JST
 *         Last Modified : 2018-02-24 19:53:58 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2017 Shinsuke OTA
 */

#include "TUpdateParticleMomentumProcessor.h"
#include "TRangeTableHelper.h"
#include <TArtParticle.h>
#include <TCatTpcTrack.h>
#include <TClonesArray.h>
#include <TGraph.h>

using art::TUpdateParticleMomentumProcessor;

ClassImp(TUpdateParticleMomentumProcessor)

TUpdateParticleMomentumProcessor::TUpdateParticleMomentumProcessor()
: fNameParticle("particle"), fNameTrack("track"), fParticle(NULL), fTrack(NULL), fIgnoreRange(0)
{
   RegisterInputCollection("Particle","name of particle",fNameParticle,"particle",
                           &fParticle,TClonesArray::Class_Name(),TArtParticle::Class_Name());
   RegisterInputCollection("Track","name of track",fNameTrack,"track",
                           &fTrack,TClonesArray::Class_Name(),TCatTpcTrack::Class_Name());
   RegisterInputInfo("RangeTable","name of range table",fNameRangeTable,"nameOfRangeTable");
   RegisterProcessorParameter("IgnoreRange","ignore range of track if non-zero (only angular information will be used)",
                           fIgnoreRange,0);
   
}

TUpdateParticleMomentumProcessor::~TUpdateParticleMomentumProcessor()
{
}

TUpdateParticleMomentumProcessor::TUpdateParticleMomentumProcessor(const TUpdateParticleMomentumProcessor& rhs)
{
}

TUpdateParticleMomentumProcessor& TUpdateParticleMomentumProcessor::operator=(const TUpdateParticleMomentumProcessor& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}

void TUpdateParticleMomentumProcessor::Init(TEventCollection *col)
{
   if (fIgnoreRange) return;
   if (!col->GetInfo(fNameRangeTable)) {
      SetStateError(Form("No such range table is found : %s (or please set IgnoreRange not to use range table)",
                         fNameRangeTable.Data()));
      return;
   }
   fRangeTableHelper = reinterpret_cast<TRangeTableHelper**>(col->GetInfoRef(fNameRangeTable));

}

void TUpdateParticleMomentumProcessor::Process()
{
   if (!(*fParticle)->GetEntriesFast()) return;
   if (!(*fTrack)->GetEntriesFast()) return;
   TArtParticle *particle = static_cast<TArtParticle*>((*fParticle)->UncheckedAt(0));
   TCatTpcTrack *track = static_cast<TCatTpcTrack*>((*fTrack)->UncheckedAt(0));
   Double_t mass = particle->M();
   Double_t tke = particle->TKE();
   if (!fIgnoreRange) {
      tke = (*fRangeTableHelper)->GetR2E()->Eval(track->GetRange());
   }
   particle->SetKineticEnergy(tke,mass);
   particle->SetTheta(track->GetDirection().Theta());
   particle->SetPhi(track->GetDirection().Phi());
}
