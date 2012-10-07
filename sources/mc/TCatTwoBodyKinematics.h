/* $Id:$ */
/**
 * @file   TCatTwoBodyKinematics.h
 * @date   Created : Oct 07, 2012 13:07:00 JST
 *   Last Modified : 
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#ifndef TCATTWOBODYKINEMATICS_H
#define TCATTWOBODYKINEMATICS_H

#include <TArtParticle.h>


class TCatTwoBodyKinematics  {

public:
   TCatTwoBodyKinematics();
   ~TCatTwoBodyKinematics();

   TCatTwoBodyKinematics(Int_t a1, Int_t z1, Int_t a2, Int_t z2, Int_t a3, Int_t z3);

   void SetIncidentEnergy(Double_t energyPerNucleon);

   void SetTheta(Double_t thetaCM);

   void SetExcitationEnergy(Double_t ex, Int_t idx = 2); // idx begins with 0

   const TArtParticle* GetParticle(Int_t index, Bool_t isCM = kFALSE);

   Double_t GetDq();

   void Build();

   void Print(Option_t *opt = "")  const;
   
protected:
   TArtParticle *fPLab[4];
   TArtParticle *fPCM[4];
   Int_t         fA[4];
   Int_t         fZ[4];
   TArtParticle *fCM;
   Double_t      fMass[4];
   Double_t      fEx[4];
   Double_t      fIncidentEnergyPerNucleon;
   Bool_t        fIsRandom;
   Double_t      fThetaCM;
   

};
#endif // end of #ifdef TCATTWOBODYKINEMATICS_H
