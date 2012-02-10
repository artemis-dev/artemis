/* $Id:$ */
/**
 * @file   TCatCmdPry.cc
 * @date   Created : Feb 09, 2012 22:09:41 JST
 *   Last Modified : Feb 10, 2012 20:16:02 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TCatCmdPry.h"

TCatCmdPry::TCatCmdPry()
   : TCatCmdPr(kY)
{
   SetName("pry");
   SetTitle("projection to y-axis");
}
TCatCmdPry::~TCatCmdPry()
{
}

TCatCmdPry* TCatCmdPry::Instance()
{
   static TCatCmdPry instance;
   return &instance;
}
