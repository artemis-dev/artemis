/* $Id:$ */
/**
 * @file   TCatCmdBny.cc
 * @date   Created : Feb 09, 2012 19:09:35 JST
 *   Last Modified : Feb 09, 2012 19:28:24 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TCatCmdBny.h"

TCatCmdBny::TCatCmdBny()
   : TCatCmdBn(kY) {
   SetName("bny");
   SetTitle("band projection to y-axis");
}
TCatCmdBny::~TCatCmdBny()
{
}

TCatCmdBny* TCatCmdBny::Instance()
{
   static TCatCmdBny instance;
   return &instance;
}

