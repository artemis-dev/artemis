/**
 * @file   TProcessorParameter.cc
 * @brief  processor parameter
 *
 * @date   Created       : 2014-05-09 18:15:59 JST
 *         Last Modified : May 09, 2014 18:16:01 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 Shinsuke OTA
 */

#include "TProcessorParameter.h"

using art::TProcessorParameter;

ClassImp(TProcessorParameter)

TProcessorParameter::TProcessorParameter()
{
}

TProcessorParameter::~TProcessorParameter()
{
}

TProcessorParameter::TProcessorParameter(const TProcessorParameter& rhs)
{
}

TProcessorParameter& TProcessorParameter::operator=(const TProcessorParameter& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}
