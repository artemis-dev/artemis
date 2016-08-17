/**
 * @file   TCmdGroup.cc
 * @brief  command group
 *
 * @date   Created       : 2016-08-17 16:54:40 JST
 *         Last Modified : 2016-08-17 17:30:32 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2016 Shinsuke OTA
 */

#include "TCmdGroup.h"
#include <TObjArray.h>
#include <TObjString.h>
#include <numeric>

using art::TCmdGroup;
 
ClassImp(TCmdGroup);

namespace {
   struct join_with_separator {
      join_with_separator(char separator = ' ') : fSeparator(separator) {}
      // call of TString constructor should be avoided but the readability is set above the speed
      TString operator()(const TString &a, const TString &b) {
         return TString::Format("%s%c%s",a.Data(),fSeparator,b.Data());
      }
      const char fSeparator;
   };
}

TCmdGroup::TCmdGroup()
{
   fCmds = new TObjArray;
   fFlagExactName = kFALSE;
}

TCmdGroup::~TCmdGroup()
{
   delete fCmds;
   
}

TCmdGroup::TCmdGroup(const TCmdGroup& rhs)
{
}

TCmdGroup& TCmdGroup::operator=(const TCmdGroup& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}

Long_t TCmdGroup::Cmd(vector<TString> tokens)
{
   
   TString com = tokens[0];
   TObjArray *arr = com.Tokenize("/");
   TIter it(arr);
   vector<TString> cmds;
   TObjString *str;
   while ((str = (TObjString*)it.Next())) {
      cmds.push_back(str->GetString());
   }
   delete arr;
   
   com = cmds[0];
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
   Int_t offset = 0;
   if (cmds.size() > 1) offset = 1;
   TString line = std::accumulate(cmds.begin()+offset,cmds.end(),
                                   TString(""),join_with_separator('/'));
   line.Append(std::accumulate(++tokens.begin(),tokens.end(),
                                TString(""),join_with_separator(' ')));
   return cmd->Exec(line);
}

void TCmdGroup::Register(TCatCmd *cmd, const char* name, Bool_t replace)
{
   if (name != NULL) {
      cmd->SetName(name);
   }
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
