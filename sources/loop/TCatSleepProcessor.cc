/* $Id:$ */
/**
 * @file   TCatSleepProcessor.cc
 * @date   Created : Apr 27, 2012 19:27:49 JST
 *   Last Modified : May 19, 2012 17:23:25 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TCatSleepProcessor.h"

TCatSleepProcessor::TCatSleepProcessor()
   : fSleepTime(1)
{
   RegisterOptionalParameter("SleepTime","Time to sleep",fSleepTime,fSleepTime);
   printf("%d\n",fSleepTime);
}
TCatSleepProcessor::~TCatSleepProcessor()
{
}

void TCatSleepProcessor::Process()
{
   Info(TString::Format("%s processing",GetName()));
   sleep(fSleepTime);
}

