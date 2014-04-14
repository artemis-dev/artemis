/* $Id:$ */
/**
 * @file   TTimerProcessor.cc
 * @date   Created : Jun 22, 2012 21:22:04 JST
 *   Last Modified : Feb 04, 2014 16:34:12 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TTimerProcessor.h"

#include <TBenchmark.h>
#include <TStopwatch.h>

ClassImp(art::TTimerProcessor);

art::TTimerProcessor::TTimerProcessor()
{
}
art::TTimerProcessor::~TTimerProcessor()
{
}

void art::TTimerProcessor::Init(TEventCollection *)
{
   fEventNumber = 0;
   fRealTime = 0;
   fCpuTime = 0;
}

void art::TTimerProcessor::Process()
{
   fEventNumber++;
}

void art::TTimerProcessor::PreLoop()
{
   fStopwatch.Start();
}

void art::TTimerProcessor::PostLoop()
{
   fStopwatch.Stop();
   fRealTime += fStopwatch.RealTime();
   fCpuTime  += fStopwatch.CpuTime();
   Info("PostLoop","real = %.2f, cpu = %.2f sec, total %d events, rate %.2f evts/sec",
        fRealTime,fCpuTime,fEventNumber,fEventNumber/fCpuTime);
}
