/* $Id:$ */
/**
 * @file   TCatCmdFactory.cc
 * @date   Created : Feb 06, 2012 10:06:33 JST
 *   Last Modified : 2016-08-17 17:16:04 JST (ota)
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
   return &instance;
}

Long_t TCatCmdFactory::ProcessLine(TString line)
{
   if (line == "") return 0;
   return Exec(line);
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
