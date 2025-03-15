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

#include "TSRPPACPlaneDataV2.h"
#include "constant.h"
using art::TSRPPACPlaneDataV2;

ClassImp(TSRPPACPlaneDataV2)

TSRPPACPlaneDataV2::TSRPPACPlaneDataV2()
: fPosition(kInvalidD), fConvCoeff(kInvalidD), fConvCoeffL(kInvalidD), fConvCoeffR(kInvalidD)
{
}

TSRPPACPlaneDataV2::~TSRPPACPlaneDataV2()
{
}

TSRPPACPlaneDataV2::TSRPPACPlaneDataV2(const TSRPPACPlaneDataV2& rhs)
{
}

TSRPPACPlaneDataV2& TSRPPACPlaneDataV2::operator=(const TSRPPACPlaneDataV2& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}

void TSRPPACPlaneDataV2::Clear(Option_t *opt){
     fPosition = kInvalidD;
     fConvCoeff = kInvalidD;
     fConvCoeffL = kInvalidD;
     fConvCoeffR = kInvalidD;
}

