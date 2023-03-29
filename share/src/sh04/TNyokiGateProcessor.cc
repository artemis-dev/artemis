/**
 * @file   TNyokiGateProcessor.cc
 * @brief  nyoki gate
 *
 * @date   Created       : 2014-09-26 10:54:15 JST
 *         Last Modified : 2014-09-26 11:30:26 JST (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 KAWASE Shoichiro
 */

#include "TNyokiGateProcessor.h"

#include <TClonesArray.h>

#include "constant.h"
#include "TTimingChargeData.h"
#include "TGateObject.h"
#include "TGateArray.h"

using art::sh04::TNyokiGateProcessor;

ClassImp(TNyokiGateProcessor)

namespace {
   const TString DEFAULT_NYOKIARRAY("nyoki");
}

TNyokiGateProcessor::TNyokiGateProcessor()
   : fGate(NULL) {
   RegisterInputCollection("NyokiArray","array containing nyoki information",
			   fNyokiArrayName,DEFAULT_NYOKIARRAY,
			   &fNyokiArray,TClonesArray::Class_Name(),TTimingChargeData::Class_Name());
   FloatVec_t defChargeGate;
   defChargeGate.push_back(-10000.);
   defChargeGate.push_back(10000.);
   RegisterProcessorParameter("ChargeGate","charge window",
			      fChargeGate,defChargeGate);
}

TNyokiGateProcessor::~TNyokiGateProcessor()
{
}

void TNyokiGateProcessor::Init(TEventCollection *col)
{
   TGateArray *const *const gateArray =
      reinterpret_cast<TGateArray *const *>(col->GetObjectRef("gate"));
   if(!gateArray) {
      SetStateError("gate array not found. Run TGateArrayInitializer before this.");
      return;
   }

   fGate = (*gateArray)->New("nyoki_charge","Nyoki Charge Gate");
   Info("Init","new charge gate: %s, q[%g:%g]","nyoki_charge",fChargeGate[0],fChargeGate[1]);
}


void TNyokiGateProcessor::Process()
{
   for (Int_t iData = 0, nData = (*fNyokiArray)->GetEntriesFast();
	iData != nData; ++iData) {
      const TTimingChargeData *const data =
	 static_cast<const TTimingChargeData*>((*fNyokiArray)->At(iData));

      const Double_t charge = data->GetCharge();
      if (!IsValid(charge)) continue;

      if (fChargeGate[0] < charge && charge < fChargeGate[1]) {
	 fGate->SetState(kTRUE);
	 return;
      }
   }
}
