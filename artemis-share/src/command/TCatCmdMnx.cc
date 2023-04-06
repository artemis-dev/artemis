/* $Id:$ */
/**
 * @file   TCatCmdMnx.cc
 * @date   Created : Feb 09, 2012 22:09:06 JST
 *   Last Modified : Feb 09, 2012 22:29:42 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TCatCmdMnx.h"

TCatCmdMnx::TCatCmdMnx()
   : TCatCmdMn(kX)
{
   SetName("mnx");
   SetTitle("profile to x axis");
}
TCatCmdMnx::~TCatCmdMnx()
{
}

TCatCmdMnx* TCatCmdMnx::Instance()
{
   static TCatCmdMnx instance;
   return &instance;
}
