/* $Id:$ */
/**
 * @file   TCatCmdAvx.cc
 * @date   Created : Feb 09, 2012 22:09:06 JST
 *   Last Modified : Feb 09, 2012 22:29:42 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TCatCmdAvx.h"

TCatCmdAvx::TCatCmdAvx()
   : TCatCmdAv(kX)
{
   SetName("avx");
   SetTitle("Slice and fit by Gaussian (FitSlicesY)");
}
TCatCmdAvx::~TCatCmdAvx()
{
}

TCatCmdAvx* TCatCmdAvx::Instance()
{
   static TCatCmdAvx instance;
   return &instance;
}
