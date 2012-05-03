/* $Id:$ */
/**
 * @file   TCatLoopManager.h
 * @date   Created : Apr 21, 2012 17:21:44 JST
 *   Last Modified : May 02, 2012 17:52:47 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#ifndef TCATLOOPMANAGER_H
#define TCATLOOPMANAGER_H

#include "TCatLoop.h"

class TCatLoopManager  {
public:
   typedef TThreadPool<TCatLoop,EProc> Loop_t;
   static const Int_t kMaxLoop;
   static const Int_t kInvalid = -1;
protected:
   TCatLoopManager();
public:
   ~TCatLoopManager();

   static TCatLoopManager* Instance();
   
   TCatLoop* Add(const char *filename = "");
   Int_t Resume(Int_t i = kInvalid);
   Int_t Suspend(Int_t i = kInvalid);
   Int_t Terminate(Int_t i = kInvalid);
   TCatLoop* GetLoop(Int_t i = kInvalid);
   Int_t GetEntries() { return fLoops->GetEntries(); }
private:
   TList *fLoops;
   Loop_t *fThreadPool;

   Int_t fCurrent;
};
#endif // end of #ifdef TCATLOOPMANAGER_H
