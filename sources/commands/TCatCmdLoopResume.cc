/* $Id:$ */
/**
 * @file   TCatCmdLoopResume.cc
 * @date   Created : May 02, 2012 16:02:06 JST
 *   Last Modified : Sep 16, 2013 13:05:43 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TCatCmdLoopResume.h"
#include "TLoopManager.h"

ClassImp(TCatCmdLoopResume);

TCatCmdLoopResume::TCatCmdLoopResume()
{
   SetName("resume");
   SetTitle("resume loop (only first loop is resumed currently)");
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
   art::TLoopManager *lm = art::TLoopManager::Instance();
   switch (args.size()) {
   case 1:
      lm->Resume(0);
      break;
   default:
      break;
   }
   return 1;
}

void TCatCmdLoopResume::Help()
{
   
}
