/**
 * @file   TUpdateParticleMomentumByBrho.h
 * @brief  update particle momentum by using brho value
 *
 * @date   Created       : 2018-08-07 19:00:47 JST
 *         Last Modified : 2018-08-07 19:30:42 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2018 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_E0DAAF8F_C762_4B2B_907F_FC7E36970E40
#define INCLUDE_GUARD_UUID_E0DAAF8F_C762_4B2B_907F_FC7E36970E40

#include "TProcessor.h"

namespace art {
   class TUpdateParticleMomentumByBrho;
}

class art::TUpdateParticleMomentumByBrho : public TProcessor {
public:
   TUpdateParticleMomentumByBrho();
   virtual ~TUpdateParticleMomentumByBrho();

   TUpdateParticleMomentumByBrho(const TUpdateParticleMomentumByBrho& rhs);
   TUpdateParticleMomentumByBrho& operator=(const TUpdateParticleMomentumByBrho& rhs);

   virtual void Process();

protected:
   TString fInputName; // name of input
   TString fBrhoName; // name of input brho

   TClonesArray **fInput; //! input TClonesArray to be updated
   TClonesArray **fBrho; //! input Brho TClonesArray
   

private:

   ClassDef(TUpdateParticleMomentumByBrho,1) // update particle momentum by using brho value
};

#endif // INCLUDE_GUARD_UUID_E0DAAF8F_C762_4B2B_907F_FC7E36970E40
