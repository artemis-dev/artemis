/**
 * @file   TUpdateParticleMomentumByBrho.cc
 * @brief  update particle momentum by using brho
 *
 * @date   Created       : 2018-08-07 19:02:57 JST
 *         Last Modified : 2018-08-09 10:50:38 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2018 Shinsuke OTA
 */

#include "TUpdateParticleMomentumByBrho.h"
#include <TClonesArray.h>
#include <TArtParticle.h>
#include <TSimpleData.h>


using art::TUpdateParticleMomentumByBrho;

ClassImp(TUpdateParticleMomentumByBrho)

TUpdateParticleMomentumByBrho::TUpdateParticleMomentumByBrho()
{
   RegisterInputCollection("InputName","name of particle input",fInputName,TString("particle"),
                           &fInput,TClonesArray::Class_Name(),TArtParticle::Class_Name());
   RegisterInputCollection("BrhoName","name of brho input",fBrhoName,TString("br57"),
                           &fBrho,TClonesArray::Class_Name(),TSimpleData::Class_Name());
}

TUpdateParticleMomentumByBrho::~TUpdateParticleMomentumByBrho()
{
}

TUpdateParticleMomentumByBrho::TUpdateParticleMomentumByBrho(const TUpdateParticleMomentumByBrho& rhs)
{
}

TUpdateParticleMomentumByBrho& TUpdateParticleMomentumByBrho::operator=(const TUpdateParticleMomentumByBrho& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}


void TUpdateParticleMomentumByBrho::Process()
{
   if ( 0 == (*fInput)->GetEntries() || 0 == (*fBrho)->GetEntries()) return;

   for (Int_t i = 0, n = (*fInput)->GetEntries(); i < n; i++) {
      if ( i > (*fBrho)->GetEntries() - 1) break;
      TArtParticle *particle = static_cast<TArtParticle*>((*fInput)->UncheckedAt(i));
      TSimpleData *brho = static_cast<TSimpleData*>((*fBrho)->UncheckedAt(i));
      Double_t mass = particle->M();
      Int_t q = particle->Charge();
      Double_t momentum = brho->GetValue() * q * TMath::C() / 1e6; // in MeV
      Double_t tke = mass * (TMath::Sqrt( 1 + (momentum/mass) * (momentum/mass) ) - 1);
      if (fVerboseLevel > 1) {
         Info("Process","momentum = %f, TKE = %f, mass = %f\n",momentum, tke, mass);
      }
      particle->SetKineticEnergy(tke,mass);
   }
}
