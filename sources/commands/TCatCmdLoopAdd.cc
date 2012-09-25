/* $Id:$ */
/**
 * @file   TCatCmdLoopAdd.cc
 * @date   Created : May 02, 2012 16:02:35 JST
 *   Last Modified : May 02, 2012 17:28:12 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TCatCmdLoopAdd.h"
#include "TCatLoopManager.h"

ClassImp(TCatCmdLoopAdd);

TCatCmdLoopAdd::TCatCmdLoopAdd()
{
   SetName("add");
   SetTitle("Add loop");
}
TCatCmdLoopAdd::~TCatCmdLoopAdd()
{
}

TCatCmdLoopAdd* TCatCmdLoopAdd::Instance()
{
   static TCatCmdLoopAdd instance;
   return &instance;
}

Long_t TCatCmdLoopAdd::Cmd(vector<TString> args)
{
   if (args.size()!=2) {
      Help();
      return 1;
   }
   TString &filename = args[1];


   // yaml loader
   
   
   return 1;
}

void TCatCmdLoopAdd::Help()
{
}

