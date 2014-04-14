/* $Id:$ */
/**
 * @file   TLoopManager.cc
 * @date   Created : Jul 10, 2013 17:10:36 JST
 *   Last Modified : Mar 16, 2014 04:53:13 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#include "TLoopManager.h"

#include <TSystem.h>
#include <TFolder.h>
#include <TROOT.h>

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

   const TString dirname = gSystem->DirName(filename);
   const TString basename = gSystem->BaseName(filename);

   TLoop *loop = new TLoop;
   std::list <Long_t> loaded;
   TFolder *topfolder = (TFolder*)gROOT->FindObject("/artemis");
   TFolder *folder = (TFolder*)topfolder->FindObject("loops");
   if (!folder) {
      folder = topfolder->AddFolder("loops","container for loops");
   }
   TString name = TString::Format("loop%d",fLoops->GetEntries());
   folder->AddFolder(name,filename);

   fLoops->Add(loop);
   loop->Load(dirname, basename, &loaded);
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
   TLoop* l =  (TLoop*) fLoops->At(i);
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
   TLoop * obj = (TLoop*) fLoops->At(i);
   Suspend(i);
   while (obj->GetCondition()->IsSet(TLoop::kRunning)) {
      gSystem->Sleep(100);
   }
   fLoops->Remove(obj);
   TFolder *topfolder = (TFolder*)gROOT->FindObject("/artemis/loops");
   TFolder *folder = (TFolder*)topfolder->FindObject(TString::Format("loop%d",i));
   topfolder->Remove(folder);
   delete folder;

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


