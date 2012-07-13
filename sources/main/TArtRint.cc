/* $Id:$ */
/**
 * @file   TArtRint.cc
 * @date   Created : Feb 06, 2012 00:06:18 JST
 *   Last Modified : May 02, 2012 16:35:41 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TArtRint.h"

#include <TCatCmdFactory.h>
#include <TCatCmdHelp.h>
#include <TCatCmdHt.h>
#include <TCatCmdHn.h>
#include <TCatCmdHb.h>
#include <TCatCmdHtp.h>
#include <TCatCmdZone.h>
#include <TCatCmdLs.h>
#include <TCatCmdCd.h>
#include <TCatCmdPrx.h>
#include <TCatCmdPry.h>
#include <TCatCmdBnx.h>
#include <TCatCmdBny.h>
#include <TCatCmdSly.h>
#include <TCatCmdLoopAdd.h>
#include <TCatCmdLoopSuspend.h>
#include <TCatCmdLoopResume.h>

TArtRint::TArtRint(int* argc, char** argv, void* options, int numOptions, Bool_t noLogo)
   : TRint(fAppName, argc, argv, options, numOptions, noLogo)
{
   TRint::ProcessLine(".x artemislogon.C");
}
TArtRint::~TArtRint()
{
}

Long_t TArtRint::ProcessLine(const char* line, Bool_t sync, Int_t* error)
{
   if (TCatCmdFactory::Instance()->ProcessLine(line)) {
      return 1;
   }
   return TRint::ProcessLine(line,sync,error);
}
