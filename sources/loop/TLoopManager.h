/* $Id:$ */
/**
 * @file   TLoopManager.h
 * @date   Created : Apr 21, 2012 17:21:44 JST
 *   Last Modified : Mar 20, 2013 18:02:24 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#ifndef ART_TLOOPMANAGER_H
#define ART_TLOOPMANAGER_H

#include <TThreadPool.h>
#include <TLoopControl.h>
#include <TLoop.h>

namespace art {
   class TLoopManager;
}

class art::TLoopManager  {
public:
   typedef TThreadPool<TLoopControl,TLoop*> LoopControl_t;
   static const Int_t kMaxLoop = 10;
   static const Int_t kInvalid = -1;
protected:
   TLoopManager();
public:
   ~TLoopManager();
   
   static TLoopManager* Instance();
   
   TLoop* Add(const char *filename = "");
   Int_t Resume(Int_t i = kInvalid);
   Int_t Suspend(Int_t i = kInvalid);
   Int_t Terminate(Int_t i = kInvalid);
   TLoop* GetLoop(Int_t i = kInvalid);
   Int_t GetEntries() { return fLoops->GetEntries(); }
private:
   TList *fLoops;
   LoopControl_t *fLoopControl;
};
#endif // end of #ifdef TCATLOOPMANAGER_H
