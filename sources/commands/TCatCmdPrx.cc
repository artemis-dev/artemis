/* $Id:$ */
/**
 * @file   TCatCmdPrx.cc
 * @date   Created : Feb 09, 2012 22:09:06 JST
 *   Last Modified : Feb 09, 2012 22:29:42 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TCatCmdPrx.h"

TCatCmdPrx::TCatCmdPrx()
   : TCatCmdPr(kX)
{
   SetName("prx");
   SetTitle("projection to x axis");
}
TCatCmdPrx::~TCatCmdPrx()
{
}

TCatCmdPrx* TCatCmdPrx::Instance()
{
   static TCatCmdPrx instance;
   return &instance;
}
