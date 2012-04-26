/* $Id:$ */
/**
 * @file   TCatLoopManager.cc
 * @date   Created : Apr 26, 2012 23:26:40 JST
 *   Last Modified : Apr 27, 2012 00:14:08 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TCatLoopManager.h"

const Int_t TCatLoopManager::kMaxLoop = 10;

TCatLoopManager::TCatLoopManager()
{
   fLoops = new TList;
   fThreadPool = new Loop_t(kMaxLoop,kFALSE);
}
TCatLoopManager::~TCatLoopManager()
{
   delete fLoops;
   delete fThreadPool;
}


TCatLoopManager *TCatLoopManager::Instance()
{
   static TCatLoopManager instance;
   return &instance;
}

Int_t TCatLoopManager::AddLoop(const char *filename)
{
   fLoops->Add(new TCatLoop);
   return fLoops->GetEntries();
}


TCatLoop* TCatLoopManager::GetLoop(Int_t i)
{
   return (TCatLoop*) fLoops->At(i);
}

Int_t TCatLoopManager::Resume(Int_t i)
{
   fThreadPool->PushTask(*(TCatLoop*)fLoops->At(i),start);
   return kTRUE;
}

Int_t TCatLoopManager::Suspend(Int_t i)
{
   TCatLoop *loop = GetLoop(i);
   if (!loop) return 0;
   loop->Suspend();
   return 1;
}

Int_t TCatLoopManager::Terminate(Int_t i)
{
   TCatLoop *loop = GetLoop(i);
   if (!loop) return 0;
   loop->Terminate();
   return 1;
}
