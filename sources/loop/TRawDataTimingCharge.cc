/**
 * @file   TRawDataTimingCharge.cc
 * @brief  timing charge raw data
 *
 * @date   Created       : 2015-10-04 17:59:25 JST
 *         Last Modified : 2015-10-04 18:24:24 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2015 Shinsuke OTA
 */

#include "TRawDataTimingCharge.h"

using art::TRawDataTimingCharge;

ClassImp(TRawDataTimingCharge)

TRawDataTimingCharge::TRawDataTimingCharge()
{
}

TRawDataTimingCharge::~TRawDataTimingCharge()
{
}

TRawDataTimingCharge::TRawDataTimingCharge(const TRawDataTimingCharge& rhs)
   : TRawDataObject(rhs)
{
}

TRawDataTimingCharge& TRawDataTimingCharge::operator=(const TRawDataTimingCharge& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}
