/* $Id:$ */
/**
 * @file   TCatLoop.cc
 * @date   Created : Apr 26, 2012 20:26:47 JST
 *   Last Modified : May 18, 2012 08:57:31 JST
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

using namespace std;

TCatLoop::TCatLoop()
{
   fEvtNum = 0;
   fIsOnline = kTRUE;
   fInitialized = kFALSE;
   fOutput = "temp.root";
   fOut    = new TCatOstream;
   fEventStore = new TCatEventStore;
   fEventCollection = new TCatEventCollection;
}

TCatLoop::~TCatLoop()
{
   delete fOut;
   delete fEventCollection;
   delete fEventStore;
}


Bool_t TCatLoop::AddInputFile(const char *inputfile)
{
   return fEventStore->AddInputFile(inputfile);
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

Bool_t TCatLoop::Init()
{
   list<TCatProcessor*>::iterator itr;
   list<TCatProcessor*>::iterator itrBegin = fProcessors.begin();
   list<TCatProcessor*>::iterator itrEnd   = fProcessors.end();
   // initialization function
   fWidget->Info("prepare output");
   if (fEventStore->IsPrepared()) {
      if (!IsInitialized()) {
         fEventCollection->Init();
         fEventCollection->Add(fEventStore->GetListOfObjects(),kFALSE);
         fWidget->Info("event store objects added");
         for (itr = itrBegin; itr !=itrEnd; itr++) {
            (*itr)->Print();
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
   
   
   // do while there are something to be analyzed
   // begin of run
   // idle loop if failed in the begin of run
   while (fEventStore->IsPrepared()) {
      if (fEventStore->IsBeginOfRun()) {
         for (itr = itrBegin; itr !=itrEnd; itr++) {
            (*itr)->BeginOfRun();
         }
      }

      while (fEventStore->GetNextEvent()) {
         for (itr = itrBegin; itr != itrEnd; itr++) {
            (*itr)->Process();
//            if (!(*itr)->Cut()) break;
         }
         if (IsSuspended() || IsTerminated()) {
            return kTRUE;
         }
         fEventCollection->Fill();
      }
      
      for (itr = itrBegin; itr !=itrEnd; itr++) {
         (*itr)->EndOfRun();
      }
      if (IsSuspended() || IsTerminated()) {
         return kTRUE;
      }
   }
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
      SetStatus(kSuspended);
   }
   return kTRUE;
}


void TCatLoop::ShowLog()
{
   fOut->Print();
}

Bool_t TCatLoop::Terminate()
{
   // status can be set to be terminated anytime
   SetStatus(kTerminated);
   fWidget->Info("Terminated");
   return kTRUE;
}



