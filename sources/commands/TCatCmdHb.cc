/* $Id:$ */
/**
 * @file   TCatCmdHb.cc
 * @date   Created : Feb 06, 2012 23:06:15 JST
 *   Last Modified : Feb 06, 2012 23:21:13 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TCatCmdHb.h"

#include <TCatHistManager.h>

TCatCmdHb::TCatCmdHb()
{
   SetName("hb");
   SetTitle("draw next histgram");
}
TCatCmdHb::~TCatCmdHb()
{
}

TCatCmdHb* TCatCmdHb::Instance()
{
   static TCatCmdHb instance;
   return &instance;
}

Long_t TCatCmdHb::Cmd(vector<TString> tokens)
{
   TString opt("");
   opt = tokens.size()>1 ? tokens[1] : "";
   return Run(opt);
}

Long_t TCatCmdHb::Run(TString &opt)
{
   TCatHistManager::Instance()->DrawPrev(opt);
   return 1;
}
