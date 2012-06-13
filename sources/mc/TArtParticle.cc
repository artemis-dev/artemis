/* $Id: TArtParticle.cc 44 2011-02-16 14:39:26Z sweetbluemoon $ */
/**
 * @file   TArtParticle.cc
 * @date   Created : Jan 25, 2011 21:25:33 JST
 *   Last Modified : Aug 18, 2011 09:19:17 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2011
 */
#include "TArtParticle.h"

#include <TRandom.h>

ClassImp(TArtParticle);

TArtParticle::TArtParticle()
{
   for (Int_t i=0; i<2; i++) {
      fDaughter[i] = NULL;
      fDaughterAtRest[i] = NULL;
   }
   
}
TArtParticle::~TArtParticle()
{
   for (Int_t i=0; i<2; i++) {
      if (fDaughter[i]) delete fDaughter[i];
      if (fDaughterAtRest[i]) delete fDaughterAtRest[i];
   }
}

TArtParticle* TArtParticle::GetDaughter(const Int_t &i)
{
   return fDaughter[i];
}

TArtParticle* TArtParticle::GetDaughterAtRest(const Int_t &i)
{
   return fDaughterAtRest[i];
}

void TArtParticle::SetAngle(const Double_t &theta, const Double_t &phi) 
{
   Double_t p = P();
   SetX(p * TMath::Sin(theta)*TMath::Cos(phi));
   SetY(p * TMath::Sin(theta)*TMath::Sin(phi));
   SetZ(p * TMath::Cos(theta));
}

void TArtParticle::SetKineticEnergy(const Double_t &kin, const Double_t &m) 
{
   Double_t mass = m;
   if (m == 0) mass = M();
   Double_t theta = Theta();
   Double_t phi   = Phi();
   Double_t e     = mass + kin;
   Double_t p     = CalcMomentum(mass,kin);
   SetXYZT(0.,0.,p,e);
   SetAngle(theta,phi);
}

void TArtParticle::SetTwoBodyDecay(Double_t m1,Double_t m2, Bool_t isRandom)
{
   fIsRandom = isRandom;
   fDaughterMass[0] = m1;
   fDaughterMass[1] = m2;
   for (Int_t i=0; i<2;i++) {
      if (fDaughter[i]==NULL) fDaughter[i] = new TArtParticle;
      if (fDaughterAtRest[i]==NULL) fDaughterAtRest[i] = new TArtParticle;
   }
//   printf("SetTwoBodyDecay %p %p\n",fDaughterAtRest[0],fDaughterAtRest[1]);
}

void TArtParticle::Decay(Double_t theta, Double_t phi)
{
   fM = M();
   fBoost = BoostVector();
   Double_t &m1 = fDaughterMass[0];
   Double_t &m2 = fDaughterMass[1];
   Double_t e1 = (fM + m1*m1/fM - m2*m2/fM)/2.;
   Double_t p1 = TMath::Sqrt((e1-m1)*(e1+m1));
   Double_t e2 = fM - e1;
   fDaughterAtRest[0]->SetPxPyPzE(0.,0.,p1,e1);
   fDaughterAtRest[1]->SetPxPyPzE(0.,0.,-p1,e2);
//   printf("fM = %f, e1 = %f, e2 = %f, p = %f\n",fM,e1,e2,p1);
   if (fIsRandom) {
      theta = TMath::ACos(2 * gRandom->Uniform() - 1.);
      phi   = gRandom->Uniform(0.,2.*TMath::Pi());
   }
   for (Int_t i=0; i<2; i++) {
      fDaughterAtRest[i]->RotateY(theta);
      fDaughterAtRest[i]->RotateZ(phi);
      fDaughter[i]->SetPxPyPzE(
         fDaughterAtRest[i]->Px(),
         fDaughterAtRest[i]->Py(),
         fDaughterAtRest[i]->Pz(),
         fDaughterAtRest[i]->E());
      fDaughter[i]->Boost(fBoost);
   }
}
   

#if 0
int main(void) {
   TArtTwoBodyKinematics *kinema = new TArtTwoBodyKinematics;
//...
   // incident particle
   kinema->SetParticle(0,z0,a0);
   // target particle
   kinema->SetParticle(1,z1,a1);
   // beam-like partcile
   kinema->SetPartciel(2,z2,a2,excitationEnergy2);
   // recoiled partcile
   kinema->SetPartciel(3,z3,a3,excitationEnergy3);

   kinema->SetIncidentEnergy(incidentEnergy);

   kinema->SetCMAngle(scatteringAngleInCM);

   TArtPartcile *recoil = kinema->GetParticle(3);
   recoil->SetNumDecayParticle(numberOfDecayParticle);
   TArtParticle *daughter = recoil->GetDaughter(0);

   

   
}
#endif
