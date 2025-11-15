/**
 * @file   TGenerateParticleFromPID.h
 * @brief  generate particle from pid information
 *
 * @date   Created       : 2018-08-06 21:53:10 JST
 *         Last Modified : 2018-08-06 22:57:42 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2018 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_D73D253A_A3E0_47A7_BE7E_5B86A0932A14
#define INCLUDE_GUARD_UUID_D73D253A_A3E0_47A7_BE7E_5B86A0932A14

#include "TProcessor.h"

namespace art {
   class TGenerateParticleFromPID;
}

class art::TGenerateParticleFromPID : public TProcessor {
public:
   static const Int_t kNumElementInPID2Particle = 6;
   TGenerateParticleFromPID();
   virtual ~TGenerateParticleFromPID();

   TGenerateParticleFromPID(const TGenerateParticleFromPID& rhs);
   TGenerateParticleFromPID& operator=(const TGenerateParticleFromPID& rhs);

   virtual void Init(TEventCollection *col);
   virtual void Process();

protected:
   TString fInputXName; // name of input a/q
   TString fInputYName; // name of input z
   TString fOutputName; // name of output particle

   TClonesArray **fInputX; //!
   TClonesArray **fInputY; //!
   TClonesArray *fOutput; //!

   DoubleVec_t fPID2Particle; // rectangular region of x and y for particle having Zint and Aint [Zint, Aint, Xmin, Xmax, Ymin, Ymax]
   Int_t fNumSet; // num ber of set of pid to partile

   std::vector<std::vector<Double_t> > fMassCache; //! cache of masss
   

private:

   ClassDef(TGenerateParticleFromPID,1) // generate particle from pid information
};

#endif // INCLUDE_GUARD_UUID_D73D253A_A3E0_47A7_BE7E_5B86A0932A14
