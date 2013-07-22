/* $Id:$ */
/**
 * @file   TLoop.cc
 * @date   Created : Apr 26, 2012 20:26:47 JST
 *   Last Modified : Mar 20, 2013 18:06:09 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TLoop.h"

#include <fstream>
#include <yaml-cpp/yaml.h>

#include <TProcessID.h>
#include <TRint.h>
#include <TProcessor.h>

const char* art::TLoop::kConditionName = "condition";

ClassImp(art::TLoop);

art::TLoop::TLoop()
{
   fEventCollection = new TEventCollection;
   fCondition       = new TConditionBit;
}

art::TLoop::~TLoop()
{
   delete fEventCollection;
   delete fCondition;
}

#if 0
Bool_t art::TLoop::AddProcess(const char *name, const char *procname)
{
   TClass *cls = gROOT->GetClass(procname);
   if (!cls) {
      printf("not such processor : %s\n",procname);
      return kFALSE;
   }
   TCatProcessor *proc = (TCatProcessor*) cls->New();
   if (!proc) {
      return kFALSE;
   }
   proc->SetName(name);
   proc->SetWidget(fWidget);
   fProcessors.push_back(proc);
   return kTRUE;
}


Bool_t art::TLoop::AddProcess(const char *name, TCatProcessor *proc)
{
   if (!proc) {
      return kFALSE;
   }
   proc->SetName(name);
   proc->SetWidget(fWidget);
   fProcessors.push_back(proc);
   return kTRUE;
}
#endif
Bool_t art::TLoop::Load(const char* filename)
{
   std::ifstream fin(filename);
   YAML::Parser parser(fin);
   YAML::Node doc;
   std::string name, type, value;
   
   parser.GetNextDocument(doc);
   fin.close();
   try {
      const YAML::Node &node = doc["Processor"];
      // iterate for all the processors
      for (YAML::Iterator it = node.begin(); it != node.end(); it++) {
         TProcessor *proc = NULL;
         (*it) >> proc;
         if (!proc) return kFALSE;
         fProcessors.push_back(proc);
      }
   } catch (YAML::KeyNotFound& e) {
      std::cout << e.what() << std::endl;
   } catch (YAML::Exception &e) {
      std::cout << "Error Occured" << std::endl;
      std::cout << e.what() << std::endl;
      return kFALSE;
   }
   Init();
   return kTRUE;
}

Bool_t art::TLoop::Init()
{
   std::list<TProcessor*>::iterator itr;
   std::list<TProcessor*>::iterator itrBegin = fProcessors.begin();
   std::list<TProcessor*>::iterator itrEnd   = fProcessors.end();
   // initialization function
//   fEventCollection->Clear();
   fEventCollection->Add(kConditionName,fCondition,kTRUE);
   fCondition->Set(kBeginOfRun);
   for (itr = itrBegin; itr!=itrEnd; itr++) {
      (*itr)->InitProc(fEventCollection);
   }
   return kTRUE;
}

Bool_t art::TLoop::Resume()
{
   // get loop iterator
   std::list<TProcessor*>::iterator itr;
   std::list<TProcessor*>::iterator itrBegin = fProcessors.begin();
   std::list<TProcessor*>::iterator itrEnd   = fProcessors.end();
   // if the loop is already running, do nothing
   if (fCondition->IsSet(kRunning)) return kFALSE;
   // set status as running
   fCondition->Set(kRunning);
   // if the loop start at the first time, call BeginOfRun
   if (fCondition->IsSet(kBeginOfRun)) {
      for_each(itrBegin,itrEnd,std::mem_fun(&TProcessor::BeginOfRun));
      fCondition->Unset(kBeginOfRun);
   }
   // do prerun
   // For the special call to remap branch etc.
   for_each(itrBegin,itrEnd,std::mem_fun(&TProcessor::PreLoop));
   // start loop
   while (1) {
      Int_t objectNumber = TProcessID::GetObjectCount();
      for (itr = itrBegin; itr != itrEnd; itr++) {
         (*itr)->Process();
         if (fCondition->IsSet(kStopEvent)) {
            fCondition->Unset(kStopEvent);
            break;
         }
      }
      TProcessID::SetObjectCount(objectNumber);
      if (fCondition->IsSet(kStopLoop)) {
         // requested to stop run
         fCondition->Unset(kStopLoop);
         break;
      }
   }
   for_each(itrBegin,itrEnd,std::mem_fun(&TProcessor::PostLoop));
   if (fCondition->IsSet(kEndOfRun)) {
      fCondition->Unset(kEndOfRun);
      for_each(itrBegin,itrEnd,std::mem_fun(&TProcessor::EndOfRun));
   }
   fCondition->Unset(kRunning);
   ((TRint*)gApplication)->SetPrompt("artemis [%d] ");

   return kTRUE;
}



