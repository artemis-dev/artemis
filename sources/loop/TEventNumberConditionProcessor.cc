/* @file TEventNumberConditionProcessor.cc
 * @brief event number alignment for multiple event store
 *
 * @date Create        : 2019-06-09 14:29:05 JST
 *       Last Modified : 2019-05-15 11:38:43 JST (ota)
 * @author: Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 */


#include "TEventNumberConditionProcessor.h"
#include "TEventCollection.h"
#include "TSimpleData.h"

ClassImp(art::TEventNumberConditionProcessor)

using art::TEventNumberConditionProcessor;

TEventNumberConditionProcessor::TEventNumberConditionProcessor()
{
   Register(fOutput("Output","name of event number","eventNumber"));
}

TEventNumberConditionProcessor::~TEventNumberConditionProcessor()
{
}

void TEventNumberConditionProcessor::Init(TEventCollection *col)
{
   fOutput.SetData(new TSimpleDataLong);
   fOutput->SetValue(-1L);
   col->AddInfo(fOutput.Value(),fOutput.fData,true);
}

void TEventNumberConditionProcessor::Process()
{
   fOutput->SetValue(fOutput->GetValue() + 1);
}
