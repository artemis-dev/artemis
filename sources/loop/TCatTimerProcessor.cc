/* $Id:$ */
/**
 * @file   TCatTimerProcessor.cc
 * @date   Created : Jun 22, 2012 21:22:04 JST
 *   Last Modified : 
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TCatTimerProcessor.h"

#include <TBenchmark.h>

TCatTimerProcessor::TCatTimerProcessor()
{
}
TCatTimerProcessor::~TCatTimerProcessor()
{
}

void TCatTimerProcessor::BeginOfRun()
{
   gBenchmark->Reset();
   gBenchmark->Start("analysis");
}

void TCatTimerProcessor::EndOfRun()
{
   gBenchmark->Show("analysis");
}
