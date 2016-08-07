/**
 * @file   IProcessorHelper.cc
 * @brief  interface of the helper for processors
 *
 * @date   Created       : 2016-07-21 23:03:28 JST
 *         Last Modified : 2016-07-21 23:14:02 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2016 Shinsuke OTA
 */

#include "IProcessorHelper.h"

using art::IProcessorHelper;

ClassImp(IProcessorHelper)

IProcessorHelper::IProcessorHelper()
{
}

IProcessorHelper::~IProcessorHelper()
{
}

IProcessorHelper::IProcessorHelper(const IProcessorHelper& rhs)
{
}

IProcessorHelper& IProcessorHelper::operator=(const IProcessorHelper& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}

