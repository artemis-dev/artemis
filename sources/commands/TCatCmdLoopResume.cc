/* $Id:$ */
/**
 * @file   TCatCmdLoopResume.cc
 * @date   Created : May 02, 2012 16:02:06 JST
 *   Last Modified : May 02, 2012 17:28:30 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TCatCmdLoopResume.h"
#include "TCatLoopManager.h"

ClassImp(TCatCmdLoopResume);

TCatCmdLoopResume::TCatCmdLoopResume()
{
   SetName("resume");
   SetTitle("resume loop");
}
TCatCmdLoopResume::~TCatCmdLoopResume()
{
}


TCatCmdLoopResume* TCatCmdLoopResume::Instance()
{
   static TCatCmdLoopResume instance;
   return &instance;
}

Long_t TCatCmdLoopResume::Cmd(vector<TString> args)
{
   TCatLoopManager *lm = TCatLoopManager::Instance();
   switch (args.size()) {
   case 1:
      lm->Resume();
      break;
   default:
      break;
   }
   return 1;
}

void TCatCmdLoopResume::Help()
{
   
}
