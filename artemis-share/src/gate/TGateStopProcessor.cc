/**
 * @file   TGateStopProcessor.cc
 * @brief  stop by gate id
 *
 * @date   Created       : 2014-03-19 16:15:54 JST
 *         Last Modified : 2020-03-06 13:37:31 JST (ota)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 KAWASE Shoichiro
 */

#include "TGateStopProcessor.h"
#include "TGateArray.h"
#include "TGateObject.h"

using art::TGateStopProcessor;

ClassImp(TGateStopProcessor)

TGateStopProcessor::TGateStopProcessor()
   : fGate(NULL)
{
   RegisterProcessorParameter("GateName","name of gate to test",
			      fGateName,TString("gate"));
   RegisterProcessorParameter("StopIf","stop analysis if gate is (0) false, (1) true",
			      fStopIf,0);
}

TGateStopProcessor::~TGateStopProcessor() {}

void TGateStopProcessor::Init(TEventCollection *col)
{
   TGateArray **const gateArray = reinterpret_cast<TGateArray**>(col->GetObjectRef("gate"));
   if(!gateArray) {
      SetStateError("gate array not found. Run TGateArrayInitializer before this.");
      return;
   }
   TGateObject **pGate = (*gateArray)->GetObjectRef(fGateName);
   if (pGate && *pGate) {
      fGate = *pGate;
   } else {
      SetStateError(TString::Format("gate named \'%s\' not found.",fGateName.Data()));
   }
}

void TGateStopProcessor::Process()
{
   if (fGate->Test(fStopIf)) SetStopEvent();
}
