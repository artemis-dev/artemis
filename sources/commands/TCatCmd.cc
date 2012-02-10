/* $Id:$ */
/**
 * @file   TCatCmd.cc
 * @date   Created : Feb 06, 2012 10:06:48 JST
 *   Last Modified : Feb 08, 2012 00:18:48 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TCatCmd.h"

#include <TObjArray.h>
#include <TObjString.h>

ClassImp(TCatCmd);

TCatCmd::TCatCmd()
   : TNamed("TCatCmdAbc","TCatCmd Abstract Class") 
{
}

TCatCmd::~TCatCmd()
{
}

Long_t TCatCmd::Exec(TString &line) 
{
   TObjArray *arr = line.Tokenize(" ");
   TIter it(arr);
   vector<TString> tokens;
   TObjString *str;
   while ((str = (TObjString*)it.Next())) {
      tokens.push_back(str->GetString());
   }
   delete arr;
   return Cmd(tokens);
}

void TCatCmd::GetRange(TString &arg, Int_t &id1, Int_t &id2, TString delim)
{
   TObjArray *ids = arg.Tokenize(delim);
   if (ids->GetEntries() == 1) {
      id1 = id2 = arg.Atoi();
   } else {
      id1 = ((TObjString*)ids->At(0))->GetString().Atoi();
      id2 = ((TObjString*)ids->At(1))->GetString().Atoi();
   }
   if (id1 > id2) {
      Int_t tmp = id1;
      id1 = id2;
      id2 = tmp;
   }
}

void TCatCmd::Help() 
{
}

