/* $Id:$ */
/**
 * @file   TCatCmdAvy.cc
 * @date   Created : Feb 09, 2012 22:09:41 JST
 *   Last Modified : Feb 10, 2012 20:16:02 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TCatCmdAvy.h"

TCatCmdAvy::TCatCmdAvy()
   : TCatCmdAv(kY)
{
   SetName("avy");
   SetTitle("Slice and fit by Gaussian (FitSlicesX)");
}
TCatCmdAvy::~TCatCmdAvy()
{
}

TCatCmdAvy* TCatCmdAvy::Instance()
{
   static TCatCmdAvy instance;
   return &instance;
}
