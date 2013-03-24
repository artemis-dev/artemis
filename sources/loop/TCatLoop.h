
/* $Id:$ */
/**
 * @file   TCatLoop.h
 * @date   Created : Apr 26, 2012 19:26:12 JST
 *   Last Modified : Mar 20, 2013 18:03:41 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#ifndef TCATLOOP_H
#define TCATLOOP_H

#include <TCatEventCollection.h>
#include "TCatProcessor.h"
#include <TCatOstream.h>

#include <TThreadPool.h>
#include <TList.h>
#include <list>

#include <TCatLoopWidget.h>
#include <TCatEventStore.h>
#include <TCatEventCollection.h>
#include <TFile.h>

using namespace std;

enum EProc {kIdle, kRunning, kSuspended, kTerminated, kSuspending };

#include <TTask.h>



class TCatLoop  : public TThreadPoolTaskImp<TCatLoop,EProc>, public TTask {

public:
   TCatLoop();
   ~TCatLoop();

   bool runTask(EProc /* prm */) {
      Bool_t res = Resume();
      printf("end of run\n");
      return res;
   }

   Bool_t AddInputFile(const char *inputfile);
   Bool_t Init();
   Bool_t Resume();
   Bool_t Suspend();
   Bool_t Terminate();

   void   cd() { fEventCollection->cd(); }

   void   ShowLog();
   void   AddWidget(TCatLoopWidget *widget) { fWidget = widget; }

   Bool_t IsRunning() { return (fProcStatus == kRunning); }
   Bool_t IsSuspending() { return (fProcStatus == kSuspending); }
   Bool_t IsSuspended() { return (fProcStatus == kSuspended); }
   Bool_t IsTerminated() { return (fProcStatus == kTerminated); }
   Bool_t IsIdle() { return (fProcStatus == kIdle); }

   Bool_t IsInitialized() { return fInitialized; }
   Int_t  GetStatus() { return fProcStatus; }

   Bool_t AddProcess(const char *name, const char *procname);
   Bool_t AddProcess(const char *name, TCatProcessor *proc);

   Bool_t Load(const char* filename);

   void SetEventStore(TCatEventStore *store) { fEventStore = store; }

protected:
   void SetStatus(EProc stat) { fProcStatus = stat; }
   
private:
   Bool_t  fIsOnline;
   Bool_t  fIsOpen;
   list<TCatProcessor*> fProcessors;
   TCatEventStore *fEventStore;
   TCatEventCollection *fEventCollection;
   Int_t   fEvtNum;
   EProc   fProcStatus;
   Bool_t  fInitialized;
   TCatLoopWidget *fWidget;
   TString fSteeringFile;
   TMutex  fMutex;
};

class TCatLoopResume: public TThreadPoolTaskImp<TCatLoopResume,TCatLoop*> {
public:
   bool runTask(TCatLoop* loop) {
      bool res = loop->Resume();
      return res;
   }
};
class TCatLoopSuspend: public TThreadPoolTaskImp<TCatLoopSuspend,TCatLoop*> {
public:
   bool runTask(TCatLoop* loop) {
      bool res = loop->Suspend();
      return res;
   }
};

#endif // end of #ifdef TCATLOOP_H
