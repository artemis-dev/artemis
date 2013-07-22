/* $Id:$ */
/**
 * @file   TCounterEventStore.cc
 * @date   Created : Jul 11, 2013 16:11:52 JST
 *   Last Modified : 
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#include "TCounterEventStore.h"
#include <TLoop.h>

ClassImp(art::TCounterEventStore);


art::TCounterEventStore::TCounterEventStore()
   : fNumLoop(0)
{
   RegisterProcessorParameter("MaxLoop","the number of loop",fMaxLoop,100);
}
art::TCounterEventStore::~TCounterEventStore()
{
}

void art::TCounterEventStore::Init(TEventCollection *col)
{
   fCondition = (TConditionBit**)(col->Get(TLoop::kConditionName)->GetObjectRef());
}

void art::TCounterEventStore::Process()
{
   fNumLoop++;
   if (fMaxLoop <=  fNumLoop) {
      (*fCondition)->Set(TLoop::kStopLoop);
      (*fCondition)->Set(TLoop::kEndOfRun);
   }
}
