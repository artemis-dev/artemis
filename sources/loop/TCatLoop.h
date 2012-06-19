
/* $Id:$ */
/**
 * @file   TCatLoop.h
 * @date   Created : Apr 26, 2012 19:26:12 JST
 *   Last Modified : May 18, 2012 09:48:42 JST
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

enum EProc {kIdle, kRunning, kSuspended, kTerminated };



class TCatLoop  : public TThreadPoolTaskImp<TCatLoop,EProc>, public TObject {

public:
   TCatLoop();
   ~TCatLoop();

   bool runTask(EProc /* prm */) {
      return Resume();
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
   Bool_t IsSuspended() { return (fProcStatus == kSuspended); }
   Bool_t IsTerminated() { return (fProcStatus == kTerminated); }
   Bool_t IsIdle() { return (fProcStatus == kIdle); }

   Bool_t IsInitialized() { return fInitialized; }
   Int_t  GetStatus() { return fProcStatus; }

   Bool_t AddProcess(const char *name, const char *procname);
   Bool_t AddProcess(const char *name, TCatProcessor *proc);

protected:
   void SetStatus(EProc stat) { fProcStatus = stat; }
   
private:
   list<TString> fInputs;
   TString fOutput;
   TFile  *fFile;
   Bool_t  fIsOnline;
   Bool_t  fIsOpen;
   list<TCatProcessor*> fProcessors;
   TCatEventStore *fEventStore;
   TCatEventCollection *fEventCollection;
   Int_t   fEvtNum;
   EProc   fProcStatus;
   Bool_t  fInitialized;
   TCatOstream *fOut;
   TCatLoopWidget *fWidget;
};
#endif // end of #ifdef TCATLOOP_H
