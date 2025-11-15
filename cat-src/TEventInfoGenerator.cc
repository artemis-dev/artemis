/**
 * @file   TEventInfoGenerator.cc
 * @brief  generate simulation infomation
 *
 * @date   Created       : 2018-05-23 23:16:37 JST
 *         Last Modified : 2018-05-24 17:37:03 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2018 Shinsuke OTA
 */

#include "TEventInfoGenerator.h"
#include <TEventHeader.h>
#include <TRunInfo.h>
#include <TTimeStamp.h>

using art::TEventInfoGenerator;

ClassImp(TEventInfoGenerator)

TEventInfoGenerator::TEventInfoGenerator()
: fRunHeader(NULL), fEventHeader(NULL)
{
   RegisterProcessorParameter("RunHeaderName","name of run header",
                              fRunHeaderName, TString("runheader"));
   RegisterOutputCollection("EventHeaderName","name of event header",
                            fEventHeaderName, TString("eventheader"),
                            &fEventHeader,TEventHeader::Class_Name());
   RegisterProcessorParameter("RunName","run name",fRunName,TString("run"));
   RegisterProcessorParameter("RunNumber","run number",fRunNumber,0);
   
}

TEventInfoGenerator::~TEventInfoGenerator()
{
   if (fRunHeader) delete fRunHeader;
}

TEventInfoGenerator::TEventInfoGenerator(const TEventInfoGenerator& rhs)
{
}

TEventInfoGenerator& TEventInfoGenerator::operator=(const TEventInfoGenerator& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}

void TEventInfoGenerator::Init(TEventCollection *col)
{
   // create run header
   fRunHeader = new TList;
   fRunHeader->SetName(fRunHeaderName);

}

void TEventInfoGenerator::BeginOfRun()
{
   TTimeStamp start;
   TString name = fRunName + (Long_t) fRunNumber;
   TRunInfo *runinfo = new TRunInfo(name,name);
   runinfo->SetRunName(fRunName);
   runinfo->SetRunNumber(fRunNumber);
   runinfo->SetStartTime(start.GetSec());
   fRunHeader->Add(runinfo);
   fEventHeader->SetRunName(name);
   fEventHeader->SetRunNumber(fRunNumber);
}

void TEventInfoGenerator::PostLoop()
{
   TTimeStamp stop;
   TRunInfo *runinfo = static_cast<TRunInfo*>(fRunHeader->Last());
   runinfo->SetStopTime(stop.GetSec());
}

void TEventInfoGenerator::Process()
{
   fEventHeader->IncrementEventNumber();
}
