/**
 * @file   TSRPPACPlaneData.cc
 * @brief  Data class for SR-PPAC
 *
 * @date   Created       : 2019-02-07 03:41:19 DST
 *         Last Modified : 2019-02-14 03:15:23 JST (ota)
 * @author Shoichiro Masuoka <masuoka@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2019 Shoichiro Masuoka
 */

#include "TSRPPACPlaneData.h"
#include "constant.h"
using art::TSRPPACPlaneData;

ClassImp(TSRPPACPlaneData)

TSRPPACPlaneData::TSRPPACPlaneData()
: fPosition(kInvalidD)
{
}

TSRPPACPlaneData::~TSRPPACPlaneData()
{
}

TSRPPACPlaneData::TSRPPACPlaneData(const TSRPPACPlaneData& rhs)
{
}

TSRPPACPlaneData& TSRPPACPlaneData::operator=(const TSRPPACPlaneData& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}
