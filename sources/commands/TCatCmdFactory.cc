/* $Id:$ */
/**
 * @file   TCatCmdFactory.cc
 * @date   Created : Feb 06, 2012 10:06:33 JST
 *   Last Modified : Feb 09, 2012 19:35:41 JST
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
   fCmds = new TObjArray;
}
TCatCmdFactory::~TCatCmdFactory()
{
   delete fCmds;
}

TCatCmdFactory* TCatCmdFactory::Instance()
{
   static TCatCmdFactory instance;
   return &instance;
}

Long_t TCatCmdFactory::ProcessLine(TString line)
{
   if (line == "") return 0;
   TString com = ((TObjString*)(line.Tokenize(" ")->At(0)))->GetString();
   TCatCmd *cmd = (TCatCmd*) fCmds->FindObject(com);
   if (!cmd) return 0;
   return cmd->Exec(line);
}

void TCatCmdFactory::Register(TCatCmd *cmd)
{
   if (!fCmds->FindObject(cmd)) {
      fCmds->Add(cmd);
   } else {
   }
}
