/**
 * @file   TMissingMassProcessor.cc
 * @brief  missing mass in two body reaction
 *
 * @date   Created       : 2017-09-28 15:55:36 JST
 *         Last Modified : 2018-02-15 16:34:23 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2017 Shinsuke OTA
 */

#include <TArtParticle.h>
#include <TClonesArray.h>

#include "TMissingMassProcessor.h"

using art::TMissingMassProcessor;

ClassImp(TMissingMassProcessor)

TMissingMassProcessor::TMissingMassProcessor()
: fNameP1("P1"), fNameP2("P2"), fNameP3("P3"), fP1(NULL), fP2(NULL), fP3(NULL), fP4(NULL)
{
   RegisterInputCollection("Particle1","name of input particle 1 (P1)",fNameP1,"P1",
                           &fP1,TClonesArray::Class_Name(),TArtParticle::Class_Name());
   RegisterInputCollection("Particle2","name of input particle 2 (P2)",fNameP2,"P2",
                           &fP2,TClonesArray::Class_Name(),TArtParticle::Class_Name());
   RegisterInputCollection("Particle3","name of input particle 3 (P3)",fNameP3,"P3",
                           &fP3,TClonesArray::Class_Name(),TArtParticle::Class_Name());
   RegisterOutputCollection("Particle4","name of output particle 4 (P4)",fNameP4,"P4",
                           &fP4,TClonesArray::Class_Name(),TArtParticle::Class_Name());
   RegisterOutputCollection("Particle4CM","name of output particle 4 (P4) in cm frame",fNameP4CM,"P4CM",
                           &fP4CM,TClonesArray::Class_Name(),TArtParticle::Class_Name());
}

TMissingMassProcessor::~TMissingMassProcessor()
{
}

TMissingMassProcessor::TMissingMassProcessor(const TMissingMassProcessor& rhs)
{
}

TMissingMassProcessor& TMissingMassProcessor::operator=(const TMissingMassProcessor& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}

void TMissingMassProcessor::Process()
{
   (fP4)->Clear("C");
   (fP4CM)->Clear("C");
   if (!(*fP1)->GetEntriesFast() ||
       !(*fP2)->GetEntriesFast() ||
       !(*fP3)->GetEntriesFast()) return;
       
   TArtParticle *p1 = static_cast<TArtParticle*>((*fP1)->UncheckedAt(0));
   TArtParticle *p2 = static_cast<TArtParticle*>((*fP2)->UncheckedAt(0));
   TArtParticle *p3 = static_cast<TArtParticle*>((*fP3)->UncheckedAt(0));
   TArtParticle *p4 = static_cast<TArtParticle*>((fP4)->ConstructedAt(0));
   TArtParticle *p4CM = static_cast<TArtParticle*>((fP4CM)->ConstructedAt(0));
   
   p4->SetXYZT(0.,0.,1.,0.);
   p4CM->SetXYZT(0.,0.,1.,0.);
//   printf("*p1 = %p %p %p",*p1,*p2,*p3\n);
   if (p1 == NULL || p2 == NULL || p3 == NULL) return;
   TLorentzVector cm = static_cast<TLorentzVector>(*p1) + static_cast<TLorentzVector>(*p2);
   TLorentzVector scat = cm - static_cast<TLorentzVector>(*p3);
   p4->SetKineticEnergy(scat.E() - scat.M(),scat.M());
   p4->SetTheta(scat.Theta());
   p4->SetPhi(scat.Phi());
   p4->SetMassNumber((p1)->MassNumber() + (p2)->MassNumber() - (p3)->MassNumber());
   p4->SetAtomicNumber((p1)->AtomicNumber() + (p2)->AtomicNumber() - (p3)->AtomicNumber());
   
   scat.Boost(-cm.BoostVector());
   p4CM->SetKineticEnergy(scat.E() - scat.M(),scat.M());
   p4CM->SetTheta(scat.Theta());
   p4CM->SetPhi(scat.Phi());
   p4CM->SetMassNumber((p1)->MassNumber() + (p2)->MassNumber() - (p3)->MassNumber());
   p4CM->SetAtomicNumber((p1)->AtomicNumber() + (p2)->AtomicNumber() - (p3)->AtomicNumber());

}
