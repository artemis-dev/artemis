/* $Id:$ */
/**
 * @file   TCatLoop.h
 * @date   Created : Apr 26, 2012 19:26:12 JST
 *   Last Modified : Apr 27, 2012 00:17:31 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#ifndef TCATLOOP_H
#define TCATLOOP_H

#include <TCatEventCollection.h>

#include <TThreadPool.h>
#include <TList.h>
#include <list>
using namespace std;

enum EProc {start, clean};


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

   list<TString> fInputs;
   TString fOutput;
   Bool_t  fIsOnline;
   TList   fProcessors;
   TCatEventCollection *fEventCollection;
   Int_t   fEvtNum;
   Bool_t  fIsSuspend;
};
#endif // end of #ifdef TCATLOOP_H
