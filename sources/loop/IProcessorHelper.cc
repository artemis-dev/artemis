/**
 * @file   IProcessorHelper.cc
 * @brief  interface of the helper for processors
 *
 * @date   Created       : 2016-07-21 23:03:28 JST
 *         Last Modified : 2023-08-16 16:26:00 JST (fendo)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2016 Shinsuke OTA
 */

#include "IProcessorHelper.h"
#include <TProcessor.h>

using art::IProcessorHelper;

ClassImp(IProcessorHelper)

IProcessorHelper::IProcessorHelper()
: fParent(NULL)
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

void IProcessorHelper::RegisterProcessor(TProcessor *proc)
{
   fParent = proc;
   Register(proc);
}


void IProcessorHelper::SetStateError(TString msg)
{
   if (fParent) {
      fParent->SetStateError(msg);
   } else {
      printf("%s",msg.Data());
   }
}
