/* $Id:$ */
/**
 * @file   TCatEventCountProcessor.cc
 * @date   Created : May 16, 2012 11:16:24 JST
 *   Last Modified : May 16, 2012 11:20:14 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TCatEventCountProcessor.h"

TCatEventCountProcessor::TCatEventCountProcessor()
{
   fEvtNum = 0;
}
TCatEventCountProcessor::~TCatEventCountProcessor()
{
}

void TCatEventCountProcessor::Process()
{
   fEvtNum++;
}

void TCatEventCountProcessor::EndOfRun()
{
   Info(TString::Format("Analyzed = %d\n",fEvtNum));
}
