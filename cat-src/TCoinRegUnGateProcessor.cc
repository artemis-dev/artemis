/* $Id:$ */
/**
 * @file   TCoinRegUnGateProcessor.cc
 * @date   Created : 2016-01-29 16:53:13 JST
 *   Last Modified : 2016-01-29 17:38:36 JST by tokieda
 * @author Hiroshi TOKIEDA <tokieda@cns.s.u-tokyo.ac.jp>
 *    Originated from TCoinRegGateProcessor.cc
 *  
 *    Copyright (C)2016
 */

#include "TCoinRegUnGateProcessor.h"

#include <TCoinRegData.h>

using art::TCoinRegUnGateProcessor;

ClassImp(TCoinRegUnGateProcessor)

// Default constructor
TCoinRegUnGateProcessor::TCoinRegUnGateProcessor()
   : fRegHit(0)
{
   RegisterInputCollection("InputCollection","TClonesArray returned by TCoinRegUnMappingProcessor",
                           fInputColName,TString("coin"),
			   &fRegHit,"art::TCoinRegData");

   IntVec_t defUnGatedCoinID;
   defUnGatedCoinID.push_back(3);
   RegisterProcessorParameter("UnGateCoincidenceID","stop analysis only if CoinReg is among these ID",
			      fUnGatedCoinID,defUnGatedCoinID);
}

TCoinRegUnGateProcessor::~TCoinRegUnGateProcessor() {}

void TCoinRegUnGateProcessor::Init(TEventCollection*)
{
   Info("Init","CoinReg: %s, UnGateCoincidenceID: ",fInputColName.Data());

   for (Int_t i = 0, n = fUnGatedCoinID.size(); i != n; ++i) {
      printf("%d ",fUnGatedCoinID[i]);
   }
   printf("\n");
}

void TCoinRegUnGateProcessor::Process()
{
   for (Int_t i = 0, n = fUnGatedCoinID.size(); i != n; ++i) {
      if (!((*fRegHit)->Test(fUnGatedCoinID[i]))) return;
   }

   SetStopEvent();
}
