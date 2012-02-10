/* $Id:$ */
/**
 * @file   TCatCmdHn.cc
 * @date   Created : Feb 06, 2012 23:06:15 JST
 *   Last Modified : Feb 07, 2012 12:32:30 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TCatCmdHn.h"

#include <TCatPadManager.h>
#include <TCatHistManager.h>

TCatCmdHn::TCatCmdHn()
{
   SetName("hn");
   SetTitle("draw next histgram");
}
TCatCmdHn::~TCatCmdHn()
{
}

TCatCmdHn* TCatCmdHn::Instance()
{
   static TCatCmdHn instance;
   return &instance;
}

Long_t TCatCmdHn::Cmd(vector<TString> tokens)
{
   TString opt("");
   opt = tokens.size()>1 ? tokens[1] : "";
   return Run(opt);
}

Long_t TCatCmdHn::Run(TString &opt)
{
   TCatPadManager::Instance()->Next();
   TCatHistManager::Instance()->DrawNext(opt);
   return 1;
}
