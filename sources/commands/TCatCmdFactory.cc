/* $Id:$ */
/**
 * @file   TCatCmdFactory.cc
 * @date   Created : Feb 06, 2012 10:06:33 JST
 *   Last Modified : Feb 14, 2014 18:46:31 JST
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
   fFlagExactName = kFALSE;
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
   // short name can be used
   TIterator *itr = fCmds->MakeIterator();
   TObjArray list;
   TCatCmd *cmd;
   while ((cmd=(TCatCmd*)itr->Next())!=NULL) {
      if (TString(cmd->GetName()).EqualTo(com)) {
         list.Add(cmd);
         break;
      } else if (!fFlagExactName && TString(cmd->GetName()).BeginsWith(com)) {
         list.Add(cmd);
      }
   }
   if (list.GetEntries()==0) return 0;
   if (list.GetEntries()!=1) {
      // there are some candidates
      TIterator *itr = list.MakeIterator();
      printf("An ambiguous command. Candidates are\n");
      while ((cmd=(TCatCmd*)itr->Next())!=NULL) {
         cmd->Print();
      }
      return 1;
   }
   cmd = (TCatCmd*)list[0];
   return cmd->Exec(line);
}

void TCatCmdFactory::Register(TCatCmd *cmd, Bool_t replace)
{
   TObject *obj = fCmds->FindObject(cmd->GetName());
   if (!obj) {
//      cmd->Print();
      fCmds->Add(cmd);
   } else if (replace) {
      fCmds->Remove(obj);
      delete obj;
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
