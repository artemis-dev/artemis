/**
 * @file   TAttCut.cc
 * @brief  Attribute to deal with a collection of TCut
 *
 * @date   Created       : 2014-03-03 16:21:48 JST
 *         Last Modified : Mar 05, 2014 14:31:58 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 Shinsuke OTA
 */

#include "TAttCut.h"
#include <TObjArray.h>

using art::TAttCut;

ClassImp(TAttCut)

TAttCut::TAttCut(const char* cut)
   : fCut(cut)
{
}

TAttCut::~TAttCut()
{
}

TAttCut::TAttCut(const TAttCut& rhs)
   : fCut(rhs.fCut)
{
}

TAttCut& TAttCut::operator=(const TAttCut& rhs)
{
   if (this != &rhs) {
      fCut = rhs.fCut;
   }
   return *this;
}


void TAttCut::Copy(TAttCut &att) const
{
   att.fCut = fCut;
}
