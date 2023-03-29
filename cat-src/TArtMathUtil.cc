/**
 * @file   TArtMathUtil.cc
 * @brief  math util
 *
 * @date   Created       : 2018-05-24 19:38:01 JST
 *         Last Modified : 2018-05-24 19:55:32 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2018 Shinsuke OTA
 */

#include "TArtMathUtil.h"


using namespace art::TArtMathUtil;

Bool_t art::TArtMathUtil::IsWithinRange(Double_t val, Double_t min, Double_t max)
{
   if (val < min || max < val) return kFALSE;
   return kTRUE;
}
