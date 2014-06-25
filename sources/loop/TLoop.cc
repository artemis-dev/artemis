/* $Id:$ */
/**
 * @file   TLoop.cc
 * @date   Created : Apr 26, 2012 20:26:47 JST
 *   Last Modified : Jun 24, 2014 12:40:56 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TLoop.h"

#include <fstream>
#include <yaml-cpp/yaml.h>

#include <algorithm>

#include <TProcessID.h>
#include <TRint.h>
#include <TProcessor.h>

#include <TSystem.h>
#include <TString.h>
#include <TFolder.h>
#include <TROOT.h>

const char* art::TLoop::kConditionName = "condition";

ClassImp(art::TLoop);

art::TLoop::TLoop()
{
   fEventCollection = new TEventCollection;
   fCondition       = new TConditionBit;
}

art::TLoop::~TLoop()
{
   std::list<TProcessor*>::iterator itr;
   std::list<TProcessor*>::iterator itrBegin = fProcessors.begin();
   std::list<TProcessor*>::iterator itrEnd   = fProcessors.end();
   for (itr = itrBegin; itr != itrEnd; itr++) {
      delete *itr;
   }
//   fEventCollection->Delete();
   delete fEventCollection;
   delete fCondition;
}

Bool_t art::TLoop::Load(const char* dirname, const char* basename, std::list<Long_t> *loaded)
{
   const char *filename = gSystem->ConcatFileName(dirname, basename);
   FileStat_t fstat;
   gSystem->GetPathInfo(filename, fstat);

   for (std::list<Long_t>::iterator itr = loaded->begin(); itr != loaded->end(); itr++) {
      if (*itr == fstat.fIno) {
         std::cerr << "Include loop found: " << filename << std::endl;
         return kFALSE;
      }
   }

   try {
      std::ifstream fin(filename);
      YAML::Parser parser(fin);
      YAML::Node doc;
      std::string name, type, value;
      
      loaded->push_back(fstat.fIno);
      
      parser.GetNextDocument(doc);
      fin.close();
      const YAML::Node &node = doc["Processor"];
      // iterate for all the processors
      for (YAML::Iterator it = node.begin(); it != node.end(); it++) {
         if (const YAML::Node *include = (*it).FindValue("include")) {
            std::string name;
            *include >> name;
            if (!Load(dirname, name.c_str(), loaded))
               return kFALSE;
            continue;
         }
         TProcessor *proc = NULL;
         (*it) >> proc;
         if (!proc) return kFALSE;
         fProcessors.push_back(proc);
      }
   } catch (YAML::KeyNotFound& e) {
      std::cout << e.what() << std::endl;
      return kFALSE;
   } catch (YAML::Exception &e) {
      std::cout << "Error Occured while loading " << filename << std::endl;
      std::cout << e.what() << std::endl;
      return kFALSE;
   }
//   Init();
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
      TProcessor *proc = (*itr);
      proc->InitProc(fEventCollection);
      if (proc->IsError()) {
         Error("Init","\n\n Process '%s' (%s) %s\n",proc->GetName(),proc->GetTitle(),proc->GetErrorMessage());
         return kFALSE;
      }
   }
   TFolder *folder = (TFolder*) gROOT->FindObject(TString::Format("/artemis/loops/loop%d",fID));
   if (folder) {
      TFolder *conf = folder->AddFolder("Config","configuration");
      TIter *infoiter = fEventCollection->GetUserInfoIter();
      TEventObject *obj;
      while ((obj = (TEventObject*)infoiter->Next())) {
         conf->Add(*(TObject**)obj->GetObjectRef());
      }
      delete infoiter;
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



