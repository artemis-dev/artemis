/* $Id:$ */
/**
 * @file   TCatCmdHtp.cc
 * @date   Created : Feb 07, 2012 18:07:09 JST
 *   Last Modified : 2016-04-17 06:03:47 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TCatCmdHtp.h"

#include <TCatPadManager.h>
#include <TCatHistManager.h>

TCatCmdHtp::TCatCmdHtp()
{
   SetName("htp");
   SetTitle("draw current hist");
}
TCatCmdHtp::~TCatCmdHtp()
{
}

TCatCmdHtp* TCatCmdHtp::Instance()
{
   static TCatCmdHtp instance;
   return &instance;
}

Long_t TCatCmdHtp::Cmd(vector<TString> args)
{
   TString opt(args.size()>0?args[1]:"");
   return Run(opt);
}

Long_t TCatCmdHtp::Run(TString &opt)
{
   TCatPadManager::Instance()->Current();
   TCatHistManager::Instance()->DrawCurrent(opt);
   return 1;
}
