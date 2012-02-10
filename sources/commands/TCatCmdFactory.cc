/* $Id:$ */
/**
 * @file   TCatCmdFactory.cc
 * @date   Created : Feb 06, 2012 10:06:33 JST
 *   Last Modified : Feb 10, 2012 20:45:54 JST
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
      cmd->Print();
      fCmds->Add(cmd);
   } 
   fCmds->Sort();
}

void TCatCmdFactory::Help(TString cmd)
{

   TObject *obj;
   if ((obj = fCmds->FindObject(cmd))) {
      printf("\n");
      printf("Command : %s\n",obj->GetName());
      printf("Summary : %s\n",obj->GetTitle());
      printf("\n");
      ((TCatCmd*)obj)->Help();
   } else {
      TIter it(fCmds);
      while ((obj = it.Next())) {
         printf("   %-10s %s\n",obj->GetName(),obj->GetTitle());
      }
   }
}
