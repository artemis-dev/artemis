/* $Id: TArtParticle.h 44 2011-02-16 14:39:26Z sweetbluemoon $ */
/**
 * @file   TArtParticle.h
 * @date   Created : Jan 25, 2011 21:25:48 JST
 *   Last Modified : Aug 30, 2011 18:38:20 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2011
 */
#ifndef TARTPARTICLE_H
#define TARTPARTICLE_H
#include <TLorentzVector.h>


class TArtParticle : public TLorentzVector {
private:
   Double_t fM; // mass of particle
   TVector3 fBoost;
   Bool_t   fIsRandom;
   Double_t fDaughterMass[2];
   TArtParticle *fDaughter[2];
   TArtParticle *fDaughterAtRest[2];

public:
   TArtParticle();
   TArtParticle(TVector3 v, Double_t e) : TLorentzVector(v,e) {
      for (Int_t i=0; i<2; i++) {
         fDaughter[i] = NULL;
         fDaughterAtRest[i] = NULL;
      }
   }

   ~TArtParticle();
   
   void Decay(Double_t theta = 0., Double_t phi = 0.);
   void SetAngle(const Double_t &theta, const Double_t &phi);
   void SetKineticEnergy(const Double_t &kin, const Double_t &mass = 0.);
   void SetTwoBodyDecay(Double_t m1, Double_t m2, Bool_t isRandom = kTRUE);
   TArtParticle *GetDaughter(const Int_t &i) ;
   TArtParticle *GetDaughterAtRest(const Int_t &i) ;
   Double_t ThetaX(void) const  { return Pz()?TMath::ATan(Px()/Pz()):0.; }
   Double_t ThetaY(void) const { return Pz()?TMath::ATan(Py()/Pz()):0.; }

   Double_t TKE(void) const { return E()-M(); }

   inline TArtParticle  operator+ (const TArtParticle &) const;
   inline TArtParticle& operator+=(const TArtParticle &);
   inline TArtParticle& operator= (const TArtParticle &);
      

   static Double_t CalcMomentum(const Double_t &mass, const Double_t &kin) {
      if (mass == 0.) return kin;
      return TMath::Sqrt(kin*(2*mass+kin));
   }
   static Double_t CalcBeta(const Double_t &mass, const Double_t &kin) {
      return CalcMomentum(mass,kin) / (mass + kin);
   }

   ClassDef(TArtParticle,1);
};

inline TArtParticle TArtParticle::operator+ (const TArtParticle &q) const
{
   return TArtParticle(Vect()+q.Vect(),T()+q.T());
}

inline TArtParticle& TArtParticle::operator+= (const TArtParticle &q)
{
   SetVect(Vect()+q.Vect());
   SetE(T() + q.T());
   return *this;
}

inline TArtParticle& TArtParticle::operator= (const TArtParticle &q)
{
   SetVect(q.Vect());
   SetE(q.T());
   fBoost.SetXYZ(q.X(),q.Y(),q.Z());
   fIsRandom = q.fIsRandom;
   return *this;
}
#endif // end of #ifdef TARTPARTICLE_H

