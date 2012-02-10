/* $Id:$ */
/**
 * @file   TArtRint.cc
 * @date   Created : Feb 06, 2012 00:06:18 JST
 *   Last Modified : Feb 10, 2012 00:32:18 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TArtRint.h"

#include <TCatCmdFactory.h>
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

TArtRint::TArtRint(int* argc, char** argv, void* options, int numOptions, Bool_t noLogo)
   : TRint(fAppName, argc, argv, options, numOptions, noLogo)
{
   TCatCmdFactory *cf = TCatCmdFactory::Instance();
   cf->Register(TCatCmdHt::Instance());
   cf->Register(TCatCmdHtp::Instance());
   cf->Register(TCatCmdHb::Instance());
   cf->Register(TCatCmdHn::Instance());
   cf->Register(TCatCmdZone::Instance());
   cf->Register(TCatCmdLs::Instance());
   cf->Register(TCatCmdCd::Instance());
   cf->Register(TCatCmdPrx::Instance());
   cf->Register(TCatCmdPry::Instance());
   cf->Register(TCatCmdBnx::Instance());
   cf->Register(TCatCmdBny::Instance());
   cf->Register(TCatCmdSly::Instance());
}
TArtRint::~TArtRint()
{
}

Long_t TArtRint::ProcessLine(const char* line, Bool_t sync, Int_t* error)
{
   if (TCatCmdFactory::Instance()->ProcessLine(line)) {
      return 1;
   }
   if (!strncmp(line,"hoge",4)) {
      printf("hoge\n");
      return 1;
   }
   return TRint::ProcessLine(line,sync,error);
}
