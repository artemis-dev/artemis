/* $Id:$ */
/**
 * @file   TTimerProcessor.cc
 * @date   Created : Jun 22, 2012 21:22:04 JST
 *   Last Modified : 
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TTimerProcessor.h"

#include <TBenchmark.h>

ClassImp(art::TTimerProcessor);

art::TTimerProcessor::TTimerProcessor()
{
}
art::TTimerProcessor::~TTimerProcessor()
{
}

void art::TTimerProcessor::BeginOfRun()
{
   gBenchmark->Reset();
   gBenchmark->Start("analysis");
}

void art::TTimerProcessor::EndOfRun()
{
   gBenchmark->Show("analysis");
}
