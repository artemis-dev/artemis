/* $Id:$ */
/**
 * @file   TRandomNumberEventStore.cc
 * @date   Created : Jul 11, 2013 18:11:52 JST
 *   Last Modified : Feb 01, 2015 03:37:49 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#include "TRandomNumberEventStore.h"

#include <TLoop.h>
#include <TRandom.h>
#include <TSimpleData.h>

ClassImp(art::TRandomNumberEventStore);

art::TRandomNumberEventStore::TRandomNumberEventStore()
   : fNumLoop(0)
{
   RegisterOutputCollection("OutputCollection","output name of random values",fOutputColName,TString("random"));
   RegisterProcessorParameter("MaxLoop","the maximum number of loop",fMaxLoop,100);
   RegisterProcessorParameter("Min","the minimum value",fMin,(float)0.);
   RegisterProcessorParameter("Max","the maximum value",fMax,(float)1.);
}
art::TRandomNumberEventStore::~TRandomNumberEventStore()
{
}


void art::TRandomNumberEventStore::Init(TEventCollection *col)
{
   fNumLoop = 0;
   fData = new TSimpleData;
   col->Add(fOutputColName,fData,fOutputIsTransparent);
}
void art::TRandomNumberEventStore::Process()
{
   fData->SetValue(gRandom->Uniform(fMin,fMax));
   fNumLoop++;

   if (fMaxLoop <= fNumLoop) {
      SetStopLoop();
      SetEndOfRun();
   }
}
