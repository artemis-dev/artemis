/* $Id:$ */
/**
 * @file   TCatSleepProcessor.cc
 * @date   Created : Apr 27, 2012 19:27:49 JST
 *   Last Modified : May 14, 2012 09:16:31 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TCatSleepProcessor.h"

TCatSleepProcessor::TCatSleepProcessor()
{
}
TCatSleepProcessor::~TCatSleepProcessor()
{
}

void TCatSleepProcessor::Process()
{
   Info(TString::Format("%s processing",GetName()));
   sleep(1);
}

