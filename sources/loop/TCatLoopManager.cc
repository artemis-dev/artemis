/* $Id:$ */
/**
 * @file   TCatLoopManager.cc
 * @date   Created : Apr 26, 2012 23:26:40 JST
 *   Last Modified : May 19, 2012 18:17:51 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TCatLoopManager.h"

#include <TArtCore.h>

#include <TGTab.h>

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

TCatLoop* TCatLoopManager::Add(const char *filename)
{
   static TGMainFrame *main = 0;
   static TGTab *tab = 0;
   TCatLoop *loop = new TCatLoop;
   fLoops->Add(loop);
   fCurrent = fLoops->GetEntries() - 1;
   if (!main && gClient) {
      main = new TGMainFrame(gClient->GetRoot());
      tab = new TGTab(main,300,300);
      main->AddFrame(tab,new TGLayoutHints(kLHintsCenterX,5,5,5,5));
   }
   TCatLoopWidget *widget = new TCatLoopWidget(main,fCurrent);
   if (main) {
      widget->CreateFrame(tab->AddTab(TString::Format("Loop%d",fCurrent)));
      tab->Resize(tab->GetDefaultSize());
      tab->MapSubwindows();
      tab->Layout();
      main->MapSubwindows();
      main->Resize(main->GetDefaultSize());
      main->MapWindow();
   }
   return loop;
}


TCatLoop* TCatLoopManager::GetLoop(Int_t i)
{
   if (i == kInvalid ) i = fCurrent;
   return (TCatLoop*) fLoops->At(i);
}

Int_t TCatLoopManager::Resume(Int_t i)
{
   if (!GetLoop(i)) {
      TArtCore::Info("TCatLoopManager::Resume",
                     "No such loop %d",i);
      return kFALSE;
   }

   fThreadPool->PushTask(*GetLoop(i),kIdle);
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
