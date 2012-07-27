/* $Id:$ */
/**
 * @file   TCatCmdMacro.cc
 * @date   Created : Jul 27, 2012 18:27:45 JST
 *   Last Modified : 
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TCatCmdMacro.h"

#include <TROOT.h>
#include <TApplication.h>

TCatCmdMacro::TCatCmdMacro()
{
   SetName("automacro");
   SetTitle("automatically perform macro loaded");
}
TCatCmdMacro::~TCatCmdMacro()
{
}

TCatCmdMacro* TCatCmdMacro::Instance()
{
   static TCatCmdMacro instance;
   return &instance;
}

Long_t TCatCmdMacro::Cmd(vector<TString> tokens)
{
   Int_t n = tokens.size();
   // this command is called automatically by artemis
   // if command name should be checked
   if (!tokens[0].EqualTo(GetName())) return 0;
//   printf("%d\n",tokens.size());
//   printf("%s\n",tokens[1].Data());
   if (n == 1 || !gROOT->GetListOfGlobalFunctions(kTRUE)->FindObject(tokens[1])) {
      return 0;
   }
   TString cmd = tokens[1];
   if (n==2) {
      // no argument
      cmd+= "()";
   } else {
      for (Int_t i=2; i!=n; i++) {
         if (i==2) {
            cmd += "(";
         } else {
            cmd += ",";
         }
         cmd += tokens[i];
      }
      cmd+=");";
   }
   gApplication->ProcessLine(cmd);
   return 1;
}
