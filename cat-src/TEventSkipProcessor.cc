/**
 * @file   TEventSkipProcessor.cc
 * @brief  skip some events in the beging and/or at the end of run
 *
 * @date   Created       : 2015-06-03 16:58:05 JST
 *         Last Modified : 2016-07-08 16:10:28 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2015 Shinsuke OTA
 */

#include "TEventSkipProcessor.h"
#include <TMath.h>

using art::TEventSkipProcessor;

ClassImp(TEventSkipProcessor)

TEventSkipProcessor::TEventSkipProcessor()
: fStart(0), fMaxNumEvent(0), fNumEvent(0), fNumEventAnalyzed(0)
{
   RegisterProcessorParameter("Start","event number to start analysis : first event is 0",fStart,0);
   RegisterProcessorParameter("MaxNumEvent","maximum number of events (maximum = INT_MAX)",fMaxNumEvent,TMath::Limits<Int_t>::Max());
}

TEventSkipProcessor::~TEventSkipProcessor()
{
}

TEventSkipProcessor::TEventSkipProcessor(const TEventSkipProcessor& rhs)
{
}

TEventSkipProcessor& TEventSkipProcessor::operator=(const TEventSkipProcessor& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}


void TEventSkipProcessor::Process()
{

   ++fNumEvent;
   if (fStart > fNumEvent) {
      SetStopEvent();
      return;
   }
   ++fNumEventAnalyzed;
   if (fNumEventAnalyzed >= fMaxNumEvent) {
      SetStopLoop();
      SetEndOfRun();
      return;
   }
}


void TEventSkipProcessor::PostLoop()
{
   if (fVerboseLevel > 0) {
      Info("PostLoop","Events = %d, Analyzed = %d\n",fNumEvent,fNumEventAnalyzed);
   }
}
