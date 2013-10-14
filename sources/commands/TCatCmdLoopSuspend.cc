/* $Id:$ */
/**
 * @file   TCatCmdLoopSuspend.cc
 * @date   Created : May 02, 2012 16:02:02 JST
 *   Last Modified : Sep 16, 2013 13:05:54 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TCatCmdLoopSuspend.h"
#include "TLoopManager.h"

ClassImp(TCatCmdLoopSuspend);

TCatCmdLoopSuspend::TCatCmdLoopSuspend()
{
   SetName("suspend");
   SetTitle("suspend loop (onlye first loop is suspended currently)");
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
   art::TLoopManager *lm = art::TLoopManager::Instance();
   switch (args.size()) {
   case 1:
      lm->Suspend(0);
      break;
   default:
      break;
   }
   return 1;
}

void TCatCmdLoopSuspend::Help()
{
}
