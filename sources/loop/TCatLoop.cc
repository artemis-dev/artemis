/* $Id:$ */
/**
 * @file   TCatLoop.cc
 * @date   Created : Apr 26, 2012 20:26:47 JST
 *   Last Modified : Mar 20, 2013 18:06:09 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TCatLoop.h"

#include <TArtCore.h>

#include <TClass.h>
#include <fstream>
#include <TROOT.h>
#include <TFile.h>

#include <TCatOstream.h>
#include <TProcessID.h>
#include <TApplication.h>
#include <TRint.h>
using namespace std;

TCatLoop::TCatLoop()
{
   fEvtNum = 0;
   fIsOnline = kTRUE;
   fInitialized = kFALSE;
   fEventStore = NULL;
   fEventCollection = new TCatEventCollection;
}

TCatLoop::~TCatLoop()
{
   delete fEventCollection;
//   delete fEventStore;
}


Bool_t TCatLoop::AddInputFile(const char *inputfile)
{
   Warning("AddInputFile","may not use");
   return kFALSE;
}

Bool_t TCatLoop::AddProcess(const char *name, const char *procname)
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


Bool_t TCatLoop::AddProcess(const char *name, TCatProcessor *proc)
{
   if (!proc) {
      return kFALSE;
   }
   proc->SetName(name);
   proc->SetWidget(fWidget);
   fProcessors.push_back(proc);
   return kTRUE;
}

Bool_t TCatLoop::Load(const char* filename)
{

}

Bool_t TCatLoop::Init()
{
   list<TCatProcessor*>::iterator itr;
   list<TCatProcessor*>::iterator itrBegin = fProcessors.begin();
   list<TCatProcessor*>::iterator itrEnd   = fProcessors.end();
   // initialization function
   fWidget->Info("prepare output");
   if (!fEventStore) {
      Warning("Init","EventStore is not set");
      return kFALSE;
   }
   if (fEventStore->IsPrepared()) {
      if (!IsInitialized()) {
         fEventCollection->Init();
         fEventCollection->Add(fEventStore->GetListOfObjects(),kTRUE); // output is transparent
         fWidget->Info("event store objects added");
         for (itr = itrBegin; itr !=itrEnd; itr++) {
            (*itr)->Init(fEventCollection);
         }
         fEventCollection->CreateOutput("test","hoge");
         fWidget->Info("output is prepared");
         fWidget->Info("processors are initialized");
         fInitialized = kTRUE;
      }
      fWidget->Info("initialized");
   }
   fEventCollection->cd();
#if DEBUG   
   fWidget->Info(TString::Format("%p",gDirectory));
   fWidget->Info(TString::Format("gROOT = %p",gROOT));
   gDirectory = fEventCollection->GetDir();
   fWidget->Info(TString::Format("%p",gDirectory));
#endif
   return kTRUE;
}

Bool_t TCatLoop::Resume()
{
   // do nothing if loop is terminated or running 
   fWidget->Info(TString::Format("Start loop %d",fProcStatus));

   if (IsTerminated() || IsRunning()) return kFALSE;
   if (fProcessors.size() == 0) return kFALSE;
   // set status running 
   SetStatus(kRunning);
   // get loop iterator
   list<TCatProcessor*>::iterator itr;
   list<TCatProcessor*>::iterator itrBegin = fProcessors.begin();
   list<TCatProcessor*>::iterator itrEnd   = fProcessors.end();

   fEventCollection->RemapBranch();
   // do while there are something to be analyzed
   // begin of run
   // idle loop if failed in the begin of run
   while (fEventStore->IsPrepared()) {
      // for now, Linux only
      TString filename = 
         ((TObjString*)fEventStore->GetCurrentInputName().Tokenize("/")->Last())->GetString();
         ((TRint*)gApplication)->SetPrompt(TString::Format("artemis (%s) [%s]# ",filename.Data(),"%d"));
      if (fEventStore->IsBeginOfRun()) {
         for (itr = itrBegin; itr !=itrEnd; itr++) {
            (*itr)->BeginOfRun();
         }
      }

      while (1) {
         if (fEventStore->GetNextEvent()) {
            Int_t ObjectNumber = TProcessID::GetObjectCount();
            for (itr = itrBegin; itr != itrEnd; itr++) {
               (*itr)->Process();
//            if (!(*itr)->Cut()) break;
            }
            TProcessID::SetObjectCount(ObjectNumber);
            fEventCollection->Fill();
         } else if(fEventStore->IsEndOfRun()) {
            break;
         }
         if (IsSuspending()) {
            fMutex.Lock();
            SetStatus(kSuspended);
            fMutex.UnLock();
            return kTRUE;
         }
         if (IsTerminated()) {
            return kTRUE;
         }
      }
      
      for (itr = itrBegin; itr !=itrEnd; itr++) {
         (*itr)->EndOfRun();
      }
      if (IsSuspending()) {
         SetStatus(kSuspended);
         return kTRUE;
      }
      if (IsTerminated()) {
         return kTRUE;
      }
   }
   ((TRint*)gApplication)->SetPrompt("artemis [%d] ");

   fWidget->Info("End Loop");
   // analysis is finished
   SetStatus(kIdle);
   
   
   return kTRUE;
}

Bool_t TCatLoop::Suspend()
{
   // set status to be suspend only when the loop is running
   if (IsRunning()) {
      fWidget->Info("Suspended");
      SetStatus(kSuspending);
    }
   while (IsSuspending()) { usleep(100000);};
   fEventCollection->ResetBranch();
   return kTRUE;
}


void TCatLoop::ShowLog()
{
}

Bool_t TCatLoop::Terminate()
{
   // status can be set to be terminated anytime
   SetStatus(kTerminated);
   fWidget->Info("Terminated");
   return kTRUE;
}



