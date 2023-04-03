/**
 * @file   TCatTrackResultPulse.cc
 * @brief
 *
 * @date   Created       : 2015-08-28 17:53:40 JST
 *         Last Modified : 2018-04-09 23:05:03 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2015 Shinsuke OTA
 */

#include "TCatTrackResultPulse.h"

using art::TCatTrackResultPulse;

ClassImp(TCatTrackResultPulse)

TCatTrackResultPulse::TCatTrackResultPulse()
: TCatPulseShape(),fNumVertex(0)
{
}

TCatTrackResultPulse::~TCatTrackResultPulse()
{
   fNumVertex = 0;
   fVertex.clear();
}
#if 0
TCatTrackResultPulse::TCatTrackResultPulse(const TCatTrackResultPulse& rhs)
{
}

TCatTrackResultPulse& TCatTrackResultPulse::operator=(const TCatTrackResultPulse& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}
#endif

