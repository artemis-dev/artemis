/* $Id:$ */
/**
 * @file   TCatTwoBodyKinematics.cc
 * @date   Created : Oct 07, 2012 13:07:55 JST
 *   Last Modified : 
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TCatTwoBodyKinematics.h"

#include <TArtAtomicMassTAble.h>

TCatTwoBodyKinematics::TCatTwoBodyKinematics()
{
   for (Int_t i=0; i!= 4; i++) {
      fPLab[i] = NULL;
      fPCM[i] = NULL;
   }
   fCM = NULL;
}
TCatTwoBodyKinematics::~TCatTwoBodyKinematics()
{
   if (fPLab[0]) delete fPLab[0];
   if (fPLab[1]) delete fPLab[1];
   if (fCM) delete fCM;
}

TCatTwoBodyKinematics::TCatTwoBodyKinematics(Int_t a1, Int_t z1, Int_t a2, Int_t z2, Int_t a3, Int_t z3)
{
   Int_t a4 = a1 + a2 - a3;
   Int_t z4 = z1 + z2 - z3;
   if (a4 <= 0 || z4 < 0) return;
   Int_t a[4] = { a1, a2, a3, a4 };
   Int_t z[4] = { z1, z2, z3, z4 };
   fIncidentEnergyPerNucleon = 0.;
   for (Int_t i=0; i!=4 ; i++) {
      fMass[i] = gAtomicMassTable->GetNucleusMass(z[i],a[i]);
      fEx[i]   = 0.;
      fPLab[i] = NULL;
      fPCM[i]  = NULL;
      fA[i] = a[i];
      fZ[i] = z[i];
   }
   fPLab[0] = new TArtParticle;
   fPLab[1] = new TArtParticle;
   fPCM[0] = new TArtParticle;
   fPCM[1] = new TArtParticle;
   fCM      = new TArtParticle;
   fIsRandom = kFALSE;
   fThetaCM  = 0.;
   Build();
}


void TCatTwoBodyKinematics::SetIncidentEnergy(Double_t energyPerNucleon)
{
   fIncidentEnergyPerNucleon = energyPerNucleon * fA[0];
   Build();
}

void TCatTwoBodyKinematics::SetExcitationEnergy(Double_t ex, Int_t idx)
{
   if (idx < 0 || idx > 3) return;
   fEx[idx] = ex;
   Build();
}

void TCatTwoBodyKinematics::Build()
{
   fPLab[0]->SetKineticEnergy(fIncidentEnergyPerNucleon,fMass[0]+fEx[0]);
   fPLab[1]->SetKineticEnergy(0.,fMass[1]+fEx[1]);
   fPCM[0]->SetKineticEnergy(fIncidentEnergyPerNucleon,fMass[0]+fEx[0]);
   fPCM[1]->SetKineticEnergy(0.,fMass[1]+fEx[1]);
   *fCM = *fPLab[0] + *fPLab[1];
   fPCM[0]->Boost(-fCM->BoostVector());
   fPCM[1]->Boost(-fCM->BoostVector());
   fCM->SetTwoBodyDecay(fMass[2]+fEx[2],fMass[3]+fEx[3],fIsRandom);
   fPLab[2] = fCM->GetDaughter(0);
   fPLab[3] = fCM->GetDaughter(1);
   fPCM[2] = fCM->GetDaughterAtRest(0);
   fPCM[3] = fCM->GetDaughterAtRest(1);
   fCM->Decay(fThetaCM);
}

void TCatTwoBodyKinematics::SetTheta(Double_t thetaCM)
{
   fThetaCM = thetaCM;
   fCM->Decay(thetaCM);
}

const TArtParticle* TCatTwoBodyKinematics::GetParticle(Int_t index, Bool_t isCM)
{
   if (index < 0 || index > 3) return NULL;
   if (isCM) return fPCM[index];
   return fPLab[index];
}

void TCatTwoBodyKinematics::Print(Option_t * /* opt */) const
{
   for (Int_t i=0; i!= 4; i++) {
      printf("particle %d\n",i);
      printf(" mass = %10.3f\n",fMass[i]);
      printf(" ex   = %10.3f\n",fEx[i]);
      printf(" kin  = %10.3f\n",fPLab[i]->TKE());
   }
   printf("CM");
   printf(" mass = %10.3f\n",fCM->M());
   printf(" kin  = %10.3f\n",fCM->TKE());
}

Double_t TCatTwoBodyKinematics::GetDq()
{
   return (*(fPCM[3]) - *(fPCM[1])).P();
}
