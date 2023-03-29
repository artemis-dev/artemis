/**
 * @file   TParticleGenerateProcessor.cc
 * @brief  particle at rest will be created
 *
 * @date   Created       : 2017-09-26 18:35:38 JST
 *         Last Modified : 2018-02-15 16:36:01 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2017 Shinsuke OTA
 */

#include "TParticleGenerateProcessor.h"
#include <TArtParticle.h>
#include <TGeoManager.h>
#include <TGeoElement.h>
#include <TRandom.h>
#include <TClonesArray.h>

using art::TParticleGenerateProcessor;

ClassImp(TParticleGenerateProcessor)

const Double_t TParticleGenerateProcessor::kAtomicMassUnit = 931.494061;
const Double_t TParticleGenerateProcessor::kElectronMass   = 0.510999;

TParticleGenerateProcessor::TParticleGenerateProcessor()
: fNameParticle("particle"), fParticle(NULL),fMassNumber(0),fAtomicNumber(0)
{
   RegisterOutputCollection("ParticleName","name of particle",fNameParticle,"particle",
                              &fParticle, TClonesArray::Class_Name(),TArtParticle::Class_Name());
   RegisterProcessorParameter("MassNumber","mass number of particle (0)",fMassNumber,0);
   RegisterProcessorParameter("AtomicNumber","atomic number of particle (0)",fAtomicNumber,0);
   RegisterOptionalParameter("KineticEnergy","energy per nucleon (0 MeV)",fKineticEnergy,0.);
   RegisterOptionalParameter("ThetaRange","range of theta (deg) distribution (default [0,0])",fThetaRange,FloatVec_t(2,0));
   RegisterOptionalParameter("PhiRange","range of phi (deg) distribution (default [0,0])",fPhiRange,FloatVec_t(2,0));
   
}

TParticleGenerateProcessor::~TParticleGenerateProcessor()
{
   delete fParticle;
   fParticle = NULL;
}

TParticleGenerateProcessor::TParticleGenerateProcessor(const TParticleGenerateProcessor& rhs)
{
}

TParticleGenerateProcessor& TParticleGenerateProcessor::operator=(const TParticleGenerateProcessor& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}

void TParticleGenerateProcessor::Init(TEventCollection *col)
{
   if (!gGeoManager) new TGeoManager;
   TGeoElementRN *element = gGeoManager->GetElementTable()->GetElementRN(fMassNumber,fAtomicNumber);
   if (!element) {
      SetStateError(Form("Unknown particle with A = %d, Z = %d\n",fMassNumber,fAtomicNumber));
      return;
   }
   fMass =  kAtomicMassUnit * fMassNumber + element->MassEx() - kElectronMass * fAtomicNumber;

}

void TParticleGenerateProcessor::Process()
{
   (fParticle)->Clear("C");
   TArtParticle * particle = static_cast<TArtParticle*> ((fParticle)->ConstructedAt(0));
   particle->SetXYZT(0.,0.,0.,0.);
   particle->SetMassNumber(fMassNumber);
   particle->SetAtomicNumber(fAtomicNumber);
   particle->SetKineticEnergy(fKineticEnergy*fMassNumber,fMass);
   Double_t theta = 0.;
   Double_t phi = 0.;
   Double_t deg = TMath::Pi()/180.;
   if (fThetaRange.size() > 1 &&
       TMath::Abs(fThetaRange[0] - fThetaRange[1]) > TMath::Limits<Float_t>::Epsilon()) {
      theta = TMath::ACos(gRandom->Uniform(TMath::Cos(fThetaRange[0]*deg),
                                           TMath::Cos(fThetaRange[1]*deg)));
   }
   if (fPhiRange.size() > 1 &&
       TMath::Abs(fPhiRange[0] - fPhiRange[1]) > TMath::Limits<Float_t>::Epsilon()) {
      phi = gRandom->Uniform(fPhiRange[0]*deg,fPhiRange[1]*deg);
   }
   particle->SetTheta(theta);
   particle->SetPhi(phi);
}

