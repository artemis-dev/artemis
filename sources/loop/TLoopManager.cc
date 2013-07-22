/* $Id:$ */
/**
 * @file   TLoopManager.cc
 * @date   Created : Jul 10, 2013 17:10:36 JST
 *   Last Modified : 
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#include "TLoopManager.h"

art::TLoopManager::TLoopManager()
{
   fLoops = new TList;
   fLoopControl = new LoopControl_t(kMaxLoop,kFALSE);
}
art::TLoopManager::~TLoopManager()
{
   delete fLoops;
   delete fLoopControl;
}

art::TLoopManager* art::TLoopManager::Instance()
{
   static TLoopManager instance;
   return & instance;
}

art::TLoop* art::TLoopManager::Add(const char *filename)
{
   TLoop *loop = new TLoop;
   fLoops->Add(loop);
   loop->Load(filename);
   return loop;
}

Int_t art::TLoopManager::Resume(Int_t i)
{
   TLoopControl task;
   fLoopControl->PushTask(task,GetLoop(i));
   return kTRUE;
}
Int_t art::TLoopManager::Suspend(Int_t i)
{
   TLoopControl task;
   task.Suspend();
   fLoopControl->PushTask(task,GetLoop(i));
   return kTRUE;

}
Int_t art::TLoopManager::Terminate(Int_t i)
{
   return kTRUE;
}
art::TLoop* art::TLoopManager::GetLoop(Int_t i)
{
   return (TLoop*)fLoops->At(i);
}


