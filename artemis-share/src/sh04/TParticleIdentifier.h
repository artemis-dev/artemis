/**
 * @file   TParticleIdentifier.h
 * @brief  particle identifier
 *
 * @date   Created       : 2014-12-26 13:13:22 JST
 *         Last Modified : 2014-12-26 16:55:35 JST (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 KAWASE Shoichiro
 */

#ifndef INCLUDE_GUARD_UUID_A53E257C_670E_46D6_9F5F_760E0C27A90F
#define INCLUDE_GUARD_UUID_A53E257C_670E_46D6_9F5F_760E0C27A90F

#include "TDataObject.h"

namespace art {
   namespace sh04 {
      class TParticleIdentifier;
   }
}

class art::sh04::TParticleIdentifier : public TDataObject {
public:
   TParticleIdentifier();
   virtual ~TParticleIdentifier();

   TParticleIdentifier(const TParticleIdentifier& rhs);
   TParticleIdentifier& operator=(const TParticleIdentifier& rhs);

   Double_t GetBrho() const {return fBrho;}
   Double_t GetAOQ() const {return fAOQ;}
   Double_t GetZ() const {return fZ;}

   void SetBrho(Double_t val) {fBrho = val;}
   void SetAOQ(Double_t val) {fAOQ = val;}
   void SetZ(Double_t val) {fZ = val;}

   void Copy(TObject &obj) const;
   void Clear(Option_t *opt = "");

private:
   Double_t fBrho;
   Double_t fAOQ;
   Double_t fZ;

   ClassDef(TParticleIdentifier,1) // particle identifier
};

#endif // INCLUDE_GUARD_UUID_A53E257C_670E_46D6_9F5F_760E0C27A90F
