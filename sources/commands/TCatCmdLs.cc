/* $Id:$ */
/**
 * @file   TCatCmdLs.cc
 * @date   Created : Feb 07, 2012 12:07:01 JST
 *   Last Modified : Feb 10, 2012 20:06:39 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TCatCmdLs.h"

#include <TCatHistManager.h>

#include <TDirectory.h>

TCatCmdLs::TCatCmdLs()
{
   SetName("ls");
   SetTitle("show the list of objects");
}
TCatCmdLs::~TCatCmdLs()
{
}

TCatCmdLs* TCatCmdLs::Instance()
{
   static TCatCmdLs instance;
   return &instance;
}

Long_t TCatCmdLs::Cmd(vector <TString> tokens)
{
   TString opt(tokens.size()>1 ? tokens[1] : "");
   return Run(opt);
}

Long_t TCatCmdLs::Run(TString &opt)
{
   TDirectory *saved = gDirectory;
   if (opt != "") {
      TCatHistManager::Instance()->cd(opt);
   }
   TCatHistManager::Instance()->ls(opt);
   saved->cd();
   return 1;
}
