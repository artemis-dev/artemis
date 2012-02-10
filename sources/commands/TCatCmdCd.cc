/* $Id:$ */
/**
 * @file   TCatCmdCd.cc
 * @date   Created : Feb 09, 2012 22:09:21 JST
 *   Last Modified : Feb 09, 2012 22:46:16 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TCatCmdCd.h"

#include <TCatHistManager.h>

TCatCmdCd::TCatCmdCd()
{
   SetName("cd");
   SetTitle("change directory");
}
TCatCmdCd::~TCatCmdCd()
{
}

TCatCmdCd* TCatCmdCd::Instance()
{
   static TCatCmdCd instance;
   return &instance;
}

Long_t TCatCmdCd::Cmd(vector<TString> args)
{
   if (args.size() == 1) {
      TCatHistManager::Instance()->cd("");
   } else {
      TCatHistManager::Instance()->cd(args[1]);
   }
   return 1;
}
