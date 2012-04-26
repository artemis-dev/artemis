/* $Id:$ */
/**
 * @file   TCatLoopManager.h
 * @date   Created : Apr 21, 2012 17:21:44 JST
 *   Last Modified : Apr 27, 2012 00:10:39 JST
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
protected:
   TCatLoopManager();
public:
   ~TCatLoopManager();

   static TCatLoopManager* Instance();
   
   Int_t AddLoop(const char *filename = "");
   Int_t ResumeAll();
   Int_t Resume(Int_t i);
   Int_t Suspend(Int_t i);
   Int_t Terminate(Int_t i);
   TCatLoop* GetLoop(Int_t i);
   Int_t GetEntries() { return fLoops->GetEntries(); }
private:
   TList *fLoops;
   Loop_t *fThreadPool;
};
#endif // end of #ifdef TCATLOOPMANAGER_H
