/* $Id:$ */
/**
 * @file   TLoopManager.cc
 * @date   Created : Jul 10, 2013 17:10:36 JST
 *   Last Modified : Oct 21, 2013 15:09:41 JST
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
   if (fLoops->GetEntries() > 0) {
      printf("currently not supported to perform many loops at the same time\n");
      printf("please terminate before continue.\n");
      return NULL;
   }
      
   TLoop *loop = new TLoop;
   std::vector <const char*> loaded;
   fLoops->Add(loop);
   loop->Load(filename, &loaded);
   loop->Init();
   return loop;
}

Int_t art::TLoopManager::Resume(Int_t i)
{
   TLoop *l = GetLoop(i);
   if (l) {
      TLoopControl *task = new TLoopControl;
      task->Resume();
      fLoopControl->PushTask(*task,l);
   } else {
      printf("No loop is ready\n");
   }
   return kTRUE;
}
Int_t art::TLoopManager::Suspend(Int_t i)
{
   TLoop* l = GetLoop(i);
   if (l) {
      TLoopControl *task = new TLoopControl;
      task->Suspend();
      fLoopControl->PushTask(*task,l);
   } else {
      printf("No loop is ready\n");
   }
    return kTRUE;

}
Int_t art::TLoopManager::Terminate(Int_t i)
{
   TObject * obj = fLoops->At(i);
   fLoops->Remove(obj);
   delete obj;
   return kTRUE;
}
art::TLoop* art::TLoopManager::GetLoop(Int_t i)
{
   if (i >= 0 && i < fLoops->GetEntries()) {
      return (TLoop*)fLoops->At(i);
   }
   return NULL;
}


