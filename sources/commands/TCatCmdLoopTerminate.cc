/* $Id:$ */
/**
 * @file   TCatCmdLoopTerminate.cc
 * @date   Created : Sep 16, 2013 12:16:05 JST
 *   Last Modified : Sep 16, 2013 12:23:49 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#include "TCatCmdLoopTerminate.h"
#include "TLoopManager.h"

ClassImp(TCatCmdLoopTerminate);

TCatCmdLoopTerminate::TCatCmdLoopTerminate()
{
   SetName("terminate");
   SetTitle("rterminate loop (only first loop is resumed currently)");

}
TCatCmdLoopTerminate::~TCatCmdLoopTerminate()
{
}

TCatCmdLoopTerminate* TCatCmdLoopTerminate::Instance()
{
   static TCatCmdLoopTerminate instance;
   return &instance;
}

Long_t TCatCmdLoopTerminate::Cmd(vector<TString> args)
{
   art::TLoopManager *lm = art::TLoopManager::Instance();
   switch (args.size()) {
   case 1:
      lm->Terminate();
      break;
   default:
      break;
   }
   return 1;
}

void TCatCmdLoopTerminate::Help()
{
   
}


