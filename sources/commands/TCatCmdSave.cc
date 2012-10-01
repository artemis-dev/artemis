/* $Id:$ */
/**
 * @file   TCatCmdSave.cc
 * @date   Created : Oct 01, 2012 13:01:21 JST
 *   Last Modified : 
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TCatCmdSave.h"

#include "TCatPadManager.h"

TCatCmdSave::TCatCmdSave()
{
   SetName("save");
   SetTitle("save canvas to file");
}
TCatCmdSave::~TCatCmdSave()
{
}

TCatCmdSave* TCatCmdSave::Instance()
{
   static TCatCmdSave instance;
   return &instance;
}

Long_t TCatCmdSave::Cmd(vector<TString> tokens)
{
   if (tokens.size()!=2) {
      Help();
      return 1;
   }
   return Run(tokens[1]);
}

Long_t TCatCmdSave::Run(TString &filename)
{
   TCatPadManager::Instance()->GetCanvas()->SaveAs(filename);
   return 1;
}
