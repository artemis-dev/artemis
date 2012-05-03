/* $Id:$ */
/**
 * @file   TCatLoop.cc
 * @date   Created : Apr 26, 2012 20:26:47 JST
 *   Last Modified : May 03, 2012 17:51:21 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TCatLoop.h"

#include <TArtCore.h>

#include <TClass.h>
#include <fstream>

#include <TCatOstream.h>

using namespace std;

TCatLoop::TCatLoop()
{
   fEvtNum = 0;
   fIsOnline = kTRUE;
   fOutput = "temp.root";
   fOut    = new TCatOstream;
}
TCatLoop::~TCatLoop()
{
}


Bool_t TCatLoop::AddInputFile(const char *inputfile)
{
   ifstream fin;
   fin.open(inputfile);
   if (!fin) {
      printf("file not found : %s\n",inputfile);
      return kFALSE;
   }
   fIsOnline = kFALSE;
   fInputs.push_back(TString(inputfile));
   return kTRUE;
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
   fProcessors.push_back(proc);
   return kTRUE;
}

Bool_t TCatLoop::SetOutput(const char *outputfile)
{
   ifstream fin;
   fin.open(outputfile);
   if (!fin) {
      printf("file already exist : %s\n",outputfile);
      return kFALSE;
   }
   fOutput = outputfile;
   return kTRUE;
}

Bool_t TCatLoop::Resume()
{
   // do nothing if loop is terminated or running 
   if (IsTerminated() || IsRunning()) return kFALSE;
   if (fProcessors.size() == 0) return kFALSE;
   // set status running 
   SetStatus(kRunning);
   // get loop iterator
   list<TCatProcessor*>::iterator itr;
   list<TCatProcessor*>::iterator itrBegin = fProcessors.begin();
   list<TCatProcessor*>::iterator itrEnd   = fProcessors.end();

   *fOut << "TCatLoop::Resume " << "Starting loop" << endl;
   
   // do while there are something to be analyzed
   while (fIsOnline || fInputs.size()!=0) {
      // set status to be terminated if failed to open 
      if (!Open()) {
         TArtCore::Info("Resume","Catnnot open any inputs");
         SetStatus(kTerminated);
         return kFALSE;
      }
      // loop processor while new event exits
      while (fEventCollection->GetNextEvent()) {
         // check if the status is suspended
         if (IsSuspended() || IsTerminated()) return kTRUE;
         // call processors
         for (itr = itrBegin; itr != itrEnd; itr++) {
            (*itr)->Process();
         }
         fEvtNum++;
         *fOut << "fEvtNum = " << fEvtNum << endl;
         sleep(1);
      }
      // check if the status is suspended
      if (IsSuspended() || IsTerminated()) return kTRUE;
   }
   // after all the data are analyzed, status is set to be idle
   SetStatus(kIdle);
   return kTRUE;
}

Bool_t TCatLoop::Suspend()
{
   // set status to be suspend only when the loop is running
   if (IsRunning()) SetStatus(kSuspended);
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
   return kTRUE;
}


Bool_t TCatLoop::Open(Int_t shmid)
{
   if (IsSuspended()) return kTRUE;

   if (fIsOnline) {
      fEventCollection = new TCatEventCollection(shmid);
      return kTRUE;
   }
   if (fInputs.size() == 0) {
      return kFALSE;
   }
   TString filename = fInputs.front();
   fInputs.pop_front();
   fEventCollection = new TCatEventCollection(filename);
   return kTRUE;
}

