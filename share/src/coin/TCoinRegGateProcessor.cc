/**
 * @file   TCoinRegGateProcessor.cc
 * @brief
 *
 * @date   Created:       2014-01-15 13:05:04
 *         Last Modified: 2014-06-18 17:29:34 JST (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2014 KAWASE Shoichiro All rights reserved.
 */

#include "TCoinRegGateProcessor.h"

#include <TCoinRegData.h>

using art::TCoinRegGateProcessor;

ClassImp(TCoinRegGateProcessor)

// Default constructor
TCoinRegGateProcessor::TCoinRegGateProcessor()
   : fRegHit(0)
{
   RegisterInputCollection("InputCollection","TClonesArray returned by TCoinRegMappingProcessor",
                           fInputColName,TString("coin"),
			   &fRegHit,"art::TCoinRegData");

   IntVec_t defCoinID;
   defCoinID.push_back(0);
   defCoinID.push_back(1);
   defCoinID.push_back(2);
   RegisterProcessorParameter("GateCoincidenceID","continue analysis only if CoinReg is among these ID",
			      fCoinID,defCoinID);
}

TCoinRegGateProcessor::~TCoinRegGateProcessor() {}

void TCoinRegGateProcessor::Init(TEventCollection*)
{
   Info("Init","CoinReg: %s, GateCoincidenceID: ",fInputColName.Data());

   for (Int_t i = 0, n = fCoinID.size(); i != n; ++i) {
      printf("%d ",fCoinID[i]);
   }
   printf("\n");
}

void TCoinRegGateProcessor::Process()
{
   for (Int_t i = 0, n = fCoinID.size(); i != n; ++i) {
      if ((*fRegHit)->Test(fCoinID[i])) return;
   }

   SetStopEvent();
}
