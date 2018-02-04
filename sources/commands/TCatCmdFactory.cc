/* $Id:$ */
/**
 * @file   TCatCmdFactory.cc
 * @date   Created : Feb 06, 2012 10:06:33 JST
 *   Last Modified : 2016-08-18 15:52:13 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TCatCmdFactory.h"

#include <TObjArray.h>
#include <TObjString.h>

ClassImp(TCatCmdFactory);

TCatCmdFactory::TCatCmdFactory()
{
}
TCatCmdFactory::~TCatCmdFactory()
{
}

TCatCmdFactory* TCatCmdFactory::Instance()
{
   static TCatCmdFactory instance;
   instance.SetName("factory");
   instance.SetTitle("root of commands");
   return &instance;
}

Long_t TCatCmdFactory::ProcessLine(TString line)
{
   if (line == "") return 0;
   return Exec(line);
}

