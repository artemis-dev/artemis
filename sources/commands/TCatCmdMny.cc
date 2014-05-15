/* $Id:$ */
/**
 * @file   TCatCmdMny.cc
 * @date   Created : Feb 09, 2012 22:09:41 JST
 *   Last Modified : Feb 10, 2012 20:16:02 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TCatCmdMny.h"

TCatCmdMny::TCatCmdMny()
   : TCatCmdMn(kY)
{
   SetName("mny");
   SetTitle("profile to y axis");
}
TCatCmdMny::~TCatCmdMny()
{
}

TCatCmdMny* TCatCmdMny::Instance()
{
   static TCatCmdMny instance;
   return &instance;
}
