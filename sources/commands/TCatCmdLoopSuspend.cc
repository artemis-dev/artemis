/* $Id:$ */
/**
 * @file   TCatCmdLoopSuspend.cc
 * @date   Created : May 02, 2012 16:02:02 JST
 *   Last Modified : May 02, 2012 17:28:19 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TCatCmdLoopSuspend.h"
#include "TCatLoopManager.h"

ClassImp(TCatCmdLoopSuspend);

TCatCmdLoopSuspend::TCatCmdLoopSuspend()
{
   SetName("suspend");
   SetTitle("suspend loop");
}
TCatCmdLoopSuspend::~TCatCmdLoopSuspend()
{
}

TCatCmdLoopSuspend* TCatCmdLoopSuspend::Instance()
{
   static TCatCmdLoopSuspend instance;
   return &instance;
}

Long_t TCatCmdLoopSuspend::Cmd(vector<TString> args)
{
   TCatLoopManager *lm = TCatLoopManager::Instance();
   switch (args.size()) {
   case 1:
      lm->Suspend();
      break;
   default:
      break;
   }
   return 1;
}

void TCatCmdLoopSuspend::Help()
{
}
