/* $Id:$ */
/**
 * @file   TCatCmdBnx.cc
 * @date   Created : Feb 09, 2012 19:09:51 JST
 *   Last Modified : Feb 09, 2012 19:24:58 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TCatCmdBnx.h"

TCatCmdBnx::TCatCmdBnx()
   : TCatCmdBn(kX) {
   SetName("bnx");
   SetTitle("band projection");
}
TCatCmdBnx::~TCatCmdBnx()
{
}
