/**
 * @file   TParticleGenerateProcessor.h
 * @brief  particle at rest will be produced
 *
 * @date   Created       : 2017-09-26 18:33:08 JST
 *         Last Modified : 2018-02-15 16:35:50 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2017 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_BAC727E8_EB3F_41FE_93D3_91C0A15EF504
#define INCLUDE_GUARD_UUID_BAC727E8_EB3F_41FE_93D3_91C0A15EF504

#include "TProcessor.h"

namespace art {
   class TParticleGenerateProcessor;
}
class TClonesArray;

class art::TParticleGenerateProcessor : public TProcessor {
public:
   static const Double_t kAtomicMassUnit;
   static const Double_t kElectronMass;
   TParticleGenerateProcessor();
   virtual ~TParticleGenerateProcessor();

   TParticleGenerateProcessor(const TParticleGenerateProcessor& rhs);
   TParticleGenerateProcessor& operator=(const TParticleGenerateProcessor& rhs);

   virtual void Init(TEventCollection *col);
   virtual void Process();

protected:
   TString fNameParticle; // name of particle
   TClonesArray *fParticle; //! stored in UserInfo
   
   
   Int_t fMassNumber;
   Int_t fAtomicNumber;
   Double_t fKineticEnergy; // kinetic energy per nucleon

   Double_t fMass;
   
   FloatVec_t fThetaRange;
   FloatVec_t fPhiRange;
   
private:

   ClassDef(TParticleGenerateProcessor,1) // particle at rest will be produced
};

#endif // INCLUDE_GUARD_UUID_BAC727E8_EB3F_41FE_93D3_91C0A15EF504
