/* $Id:$ */
/**
 * @file   TCatCmd.cc
 * @date   Created : Feb 06, 2012 10:06:48 JST
 *   Last Modified : Feb 06, 2012 19:44:14 JST
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

void TCatCmd::Help() 
{
}

