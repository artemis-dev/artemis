/**
 * @file   TAsynchronousRunEndProcessor.cc
 * @brief  stop loop asynchronously by checking run status of each event store
 *
 * @date   Created       : 2018-06-29 18:18:55 JST
 *         Last Modified : 2018-06-29 18:29:17 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2018 Shinsuke OTA
 */

#include "TAsynchronousRunEndProcessor.h"

using art::TAsynchronousRunEndProcessor;

ClassImp(TAsynchronousRunEndProcessor)

TAsynchronousRunEndProcessor::TAsynchronousRunEndProcessor()
{
   RegisterProcessorParameter("RunStatusNames","list of run ststus names",fRunStatusNames,StringVec_t(0));
}

TAsynchronousRunEndProcessor::~TAsynchronousRunEndProcessor()
{
}

TAsynchronousRunEndProcessor::TAsynchronousRunEndProcessor(const TAsynchronousRunEndProcessor& rhs)
{
}

TAsynchronousRunEndProcessor& TAsynchronousRunEndProcessor::operator=(const TAsynchronousRunEndProcessor& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}

void TAsynchronousRunEndProcessor::Init(TEventCollection *col) {
   for (Int_t i = 0, n = fRunStatusNames.size(); i < n; ++i) {
      TConditionBit *status = reinterpret_cast<TConditionBit*>(col->GetInfo(fRunStatusNames[i]));
      if (!status) {
         SetStateError(TString::Format("No such run status: %s",fRunStatusNames[i].Data()));
         return;
      }
      fRunStatus.push_back(status);
   }
}

void TAsynchronousRunEndProcessor::Process()
{
   for (Int_t i = 0, n = fRunStatus.size(); i < n; ++i) {
      if (!fRunStatus[i]->IsSet(TLoop::kEndOfRun)) return;
   }

   SetStopEvent();
   SetStopLoop();
   SetEndOfRun();
}
