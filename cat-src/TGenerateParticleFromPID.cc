/**
 * @file   TGenerateParticleFromPID.cc
 * @brief  generate particle from pid information
 *
 * @date   Created       : 2018-08-06 21:59:36 JST
 *         Last Modified : 2018-08-07 19:26:02 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2018 Shinsuke OTA
 */

#include "TGenerateParticleFromPID.h"
#include <TArtAtomicMassTable.h>
#include <TClonesArray.h>
#include <TArtParticle.h>
#include <TSimpleData.h>
#include <TArtMathUtil.h>
using art::TGenerateParticleFromPID;

ClassImp(TGenerateParticleFromPID)

TGenerateParticleFromPID::TGenerateParticleFromPID()
{
   RegisterInputCollection("InputX","name of input X",fInputXName,TString("aq"),
                           &fInputX,TClonesArray::Class_Name(),TSimpleData::Class_Name());
   RegisterInputCollection("InputY","name of input Y",fInputYName,TString("z"),
                           &fInputY,TClonesArray::Class_Name(),TSimpleData::Class_Name());
   RegisterProcessorParameter("PID2Particle","ranges of x and y for Zint and Aint [Zint, Aint, xmin, xmax, ymin, ymax]",
                              fPID2Particle,DoubleVec_t(0));
   RegisterOutputCollection("Output","name of output",fOutputName,TString("beam"),
                            &fOutput,TClonesArray::Class_Name(),TArtParticle::Class_Name());

}

TGenerateParticleFromPID::~TGenerateParticleFromPID()
{
}

TGenerateParticleFromPID::TGenerateParticleFromPID(const TGenerateParticleFromPID& rhs)
{
}

TGenerateParticleFromPID& TGenerateParticleFromPID::operator=(const TGenerateParticleFromPID& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}


void TGenerateParticleFromPID::Init(TEventCollection *col)
{
   if ( 0 == fPID2Particle.size() ||
        0 != (fPID2Particle.size() % kNumElementInPID2Particle)) {
      SetStateError(TString::Format("num of element in PID2Particle should be multiple of %d ( %zu elements given )",
                                    (Int_t)kNumElementInPID2Particle, fPID2Particle.size()));
   }
   fNumSet = (Int_t) (fPID2Particle.size() / kNumElementInPID2Particle);
}

void TGenerateParticleFromPID::Process()
{
   fOutput->Clear("C");

   for (Int_t i = 0, n = (*fInputX)->GetEntries(); i < n; ++i) {
      Double_t x = (static_cast<TSimpleData*>((*fInputX)->UncheckedAt(i)))->GetValue();
      Double_t y = (static_cast<TSimpleData*>((*fInputY)->UncheckedAt(i)))->GetValue();
      for (Int_t iSet = 0; iSet < fNumSet; ++iSet) {
         Double_t *pid2particle = & fPID2Particle[iSet * kNumElementInPID2Particle];
         if (TArtMathUtil::IsWithinRange(x,pid2particle[2],pid2particle[3]) &&
             TArtMathUtil::IsWithinRange(y,pid2particle[4],pid2particle[5])) {
            Int_t z = pid2particle[0];
            Int_t a = pid2particle[1];
            Double_t mass = gAtomicMassTable->GetNucleusMass(z,a);
            if (mass < -1) continue; // since unknown mass
               
            TArtParticle *particle = static_cast<TArtParticle*>(fOutput->ConstructedAt(fOutput->GetEntriesFast()));
            particle->SetAtomicNumber(z);
            particle->SetCharge(z);
            particle->SetMassNumber(a);
            particle->SetKineticEnergy(0,mass);
         }
      }
   }
}
