/**
 * @file   TParticleIdentifier.cc
 * @brief  particle identifier
 *
 * @date   Created       : 2014-12-26 16:50:42 JST
 *         Last Modified : 2014-12-26 17:00:03 JST (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 KAWASE Shoichiro
 */

#include "TParticleIdentifier.h"
#include <constant.h>

using art::sh04::TParticleIdentifier;

ClassImp(TParticleIdentifier)

TParticleIdentifier::TParticleIdentifier()
   : fBrho(kInvalidD), fAOQ(kInvalidD), fZ(kInvalidD)
{
}

TParticleIdentifier::~TParticleIdentifier()
{
}

TParticleIdentifier::TParticleIdentifier(const TParticleIdentifier& rhs)
   : TDataObject(rhs), fBrho(rhs.fBrho), fAOQ(rhs.fAOQ), fZ(rhs.fZ) {}

TParticleIdentifier& TParticleIdentifier::operator=(const TParticleIdentifier& rhs) {
   if (this != &rhs) {
      ((TParticleIdentifier&)rhs).Copy(*this);
   }
   return *this;
}

void TParticleIdentifier::Copy(TObject &obj) const
{
   TDataObject::Copy(obj);
   TParticleIdentifier &cobj = (TParticleIdentifier&)obj;
   cobj.fBrho = fBrho;
   cobj.fAOQ  = fAOQ;
   cobj.fZ    = fZ;
}

void TParticleIdentifier::Clear(Option_t *opt) {
   TDataObject::Clear(opt);
   fBrho = kInvalidD;
   fAOQ  = kInvalidD;
   fZ    = kInvalidD;
}
