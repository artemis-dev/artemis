/* $Id:$ */
/**
 * @file   TCatLoop.h
 * @date   Created : Apr 26, 2012 19:26:12 JST
 *   Last Modified : May 02, 2012 17:20:57 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#ifndef TCATLOOP_H
#define TCATLOOP_H

#include <TCatEventCollection.h>
#include "TCatProcessor.h"
#include <TThreadPool.h>
#include <TList.h>
#include <list>
using namespace std;

enum EProc {kIdle, kRunning, kSuspended, kTerminated };


class TCatLoop  : public TThreadPoolTaskImp<TCatLoop,EProc>, public TObject {

public:
   TCatLoop();
   ~TCatLoop();

   bool runTask(EProc /* prm */) {
      return Resume();
   }

   Bool_t AddInputFile(const char *inputfile);
   Bool_t SetOutput(const char *outputfile);
   Bool_t Open(Int_t shmid = 0);
   Bool_t Resume();
   Bool_t Suspend();
   Bool_t Terminate();

   Bool_t IsRunning() { return (fProcStatus == kRunning); }
   Bool_t IsSuspended() { return (fProcStatus == kSuspended); }
   Bool_t IsTerminated() { return (fProcStatus == kTerminated); }
   Bool_t IsIdle() { return (fProcStatus == kIdle); }
   Int_t  GetStatus() { return fProcStatus; }

   Bool_t AddProcess(const char *name, const char *procname);
   Bool_t AddProcess(const char *name, TCatProcessor *proc) { return kTRUE; }

protected:
   void SetStatus(EProc stat) { fProcStatus = stat; }
   
private:
   list<TString> fInputs;
   TString fOutput;
   Bool_t  fIsOnline;
   Bool_t  fIsOpen;
   list<TCatProcessor*>   fProcessors;
   TCatEventCollection *fEventCollection;
   Int_t   fEvtNum;
   EProc   fProcStatus;
};
#endif // end of #ifdef TCATLOOP_H
