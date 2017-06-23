/* $Id:$ */
/**
 * @file   TCatCmd.cc
 * @date   Created : Feb 06, 2012 10:06:48 JST
 *   Last Modified : 2016-10-21 17:28:10 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TCatCmd.h"

#include <TObjArray.h>
#include <TObjString.h>

#include "TCatHistManager.h"

ClassImp(TCatCmd);

const TString TCatCmd::kRangeDefault = ".";

TCatCmd::TCatCmd()
   : TFolder("TCatCmdAbc","TCatCmd Abstract Class") 
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

void TCatCmd::GetRange(const TString &arg, Int_t &id1, Int_t &id2, TString delim)
{
   if (!arg.CompareTo(kRangeDefault)) {
      const Int_t id = TCatHistManager::Instance()->GetId();
      id1 = id2 = id;
      return;
   }

   TObjArray *ids = arg.Tokenize(delim);
   if (!((TObjString*)ids->At(0))->GetString().IsDec()) {
      id1 = id2 = -1;
      return;
   }
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
   printf(" Help is under construction for %s.\n",GetName());
}

Int_t TCatCmd::Compare(const TObject *obj) const 
{
   if (!obj) return 1;
   return TString(GetName()).CompareTo(obj->GetName());
}
