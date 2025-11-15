/**
 * @file   TRangeTableConditionProcessor.cc
 * @brief  range table condition
 *
 * @date   Created       : 2016-09-18 10:44:16 JST
 *         Last Modified : 2016-09-18 23:40:27 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2016 Shinsuke OTA
 */

#include "TRangeTableConditionProcessor.h"

using art::TRangeTableConditionProcessor;

ClassImp(TRangeTableConditionProcessor)

TRangeTableConditionProcessor::TRangeTableConditionProcessor()
{
   RegisterHelper(&fHelper);
}

TRangeTableConditionProcessor::~TRangeTableConditionProcessor()
{
}

TRangeTableConditionProcessor::TRangeTableConditionProcessor(const TRangeTableConditionProcessor& rhs)
{
}

TRangeTableConditionProcessor& TRangeTableConditionProcessor::operator=(const TRangeTableConditionProcessor& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}

