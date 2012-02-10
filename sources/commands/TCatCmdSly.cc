/* $Id:$ */
/**
 * @file   TCatCmdSly.cc
 * @date   Created : Feb 10, 2012 00:10:25 JST
 *   Last Modified : Feb 10, 2012 00:30:21 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TCatCmdSly.h"


TCatCmdSly::TCatCmdSly()
   : TCatCmdSl(kY)
{
   SetName("sly");
   SetTitle("sliced projection to y-axis");
}
TCatCmdSly::~TCatCmdSly()
{
}
TCatCmdSly* TCatCmdSly::Instance()
{
   static TCatCmdSly instance;
   return &instance;
}
