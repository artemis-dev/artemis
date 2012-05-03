/* $Id:$ */
/**
 * @file   TCatSleepProcessor.cc
 * @date   Created : Apr 27, 2012 19:27:49 JST
 *   Last Modified : May 03, 2012 11:29:26 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TCatSleepProcessor.h"

TCatSleepProcessor::TCatSleepProcessor(std::ostream *out)
   : TCatProcessor(out)
{
   SetTitle("TCatSleepProcessor");
}
TCatSleepProcessor::~TCatSleepProcessor()
{
}

void TCatSleepProcessor::Process()
{
   sleep(1);
}

