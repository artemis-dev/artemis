/**
 * @file   TCmdGroup.cc
 * @brief  command group
 *
 * @date   Created       : 2016-08-17 16:54:40 JST
 *         Last Modified : 2016-08-19 16:11:40 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2016 Shinsuke OTA
 */

#include "TCmdGroup.h"
#include <TObjArray.h>
#include <TObjString.h>
#include <numeric>
#include <TList.h>
#include <TClass.h>

using art::TCmdGroup;
 
ClassImp(TCmdGroup);

Bool_t art::TCmdGroup::fgFlagExactName = kFALSE;

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
   fgFlagExactName = kFALSE;
}

TCmdGroup::~TCmdGroup()
{
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

TCatCmd* TCmdGroup::FindCommand(const char* name)
{
   TString path(name);
   while(path.BeginsWith("/")) {
      path.Remove(0,1);
   }
   
   TObjArray *arr = TString(name).Tokenize("/");
   TIterator *itr = GetListOfFolders()->MakeIterator();
   TCatCmd *cmd;
   TObjArray list;
   if (arr->GetEntries() == 0) return NULL;
   TString com = ((TObjString*)arr->UncheckedAt(0))->GetString();
   while ((cmd=(TCatCmd*)itr->Next())!=NULL) {
      if (TString(cmd->GetName()).EqualTo(com)) {
         list.Add(cmd);
         break;
      } else if (!fgFlagExactName && TString(cmd->GetName()).BeginsWith(com)) {
         if (arr->GetEntries() > 1) {
            // group
            if (cmd->InheritsFrom(TCmdGroup::Class())) list.Add(cmd);
         } else {
            if (!cmd->InheritsFrom(TCmdGroup::Class())) list.Add(cmd);
            // command
         }
      }
   }
   // too many candidates
   if (list.GetEntries()>1) {
      // there are some candidates
      TIterator *itr = list.MakeIterator();
      printf("An ambiguous command. Candidates are\n");
      while ((cmd=(TCatCmd*)itr->Next())!=NULL) {
         cmd->Print();
      }
      return NULL;
   }

   // command found 
   if (list.GetEntries() == 1) {
      cmd = static_cast<TCatCmd*>(list.UncheckedAt(0));
      
      if (cmd->InheritsFrom(TCmdGroup::Class())) {
         path.Remove(0,path.Index("/")+1);
         return static_cast<TCmdGroup*>(cmd)->FindCommand(path);
      } else {
         return cmd;
      }
   }

   // no candidates
   if (list.GetEntries()==0) {
      // try to search in all the directory
      itr =  GetListOfFolders()->MakeIterator();
      while ((cmd=(TCatCmd*)itr->Next())!=NULL) {
         if (!cmd->InheritsFrom(TCmdGroup::Class())) {
            continue;
         }
         if ((cmd = static_cast<TCmdGroup*>(cmd)->FindCommand(path))) {
            return cmd;
         }
      }
   }
   return cmd;
}

Long_t TCmdGroup::Cmd(vector<TString> tokens)
{
   TString com = tokens[0];
   TCatCmd *cmd = FindCommand(tokens[0]);
   if (!cmd) return 0;
   tokens[0] = cmd->GetName();
#if 0   
   TObjArray *arr = com.Tokenize("/");
   TIter it(arr);
   vector<TString> cmds;
   TObjString *str;
   while ((str = (TObjString*)it.Next())) {
      cmds.push_back(str->GetString());
   }
   if (arr) delete arr;
   
   com = cmds[0];


   TIterator *itr = GetListOfFolders()->MakeIterator();
   TObjArray list;
   TCatCmd *cmd;
   while ((cmd=(TCatCmd*)itr->Next())!=NULL) {
      if (TString(cmd->GetName()).EqualTo(com)) {
         list.Add(cmd);
         break;
      } else if (!fgFlagExactName && TString(cmd->GetName()).BeginsWith(com)) {
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
#endif

   TString line = std::accumulate(tokens.begin(),tokens.end(),
                                TString(""),join_with_separator(' '));
   
   return cmd->Exec(line);
}

void TCmdGroup::Register(TCatCmd *cmd, const char *path, const char* name, Bool_t replace)
{
   if (name != NULL) {
      cmd->SetName(name);
   }
   if (!TString(path).IsNull()) {
      TCmdGroup *group = static_cast<TCmdGroup*>(FindObjectAny(path));
      if (!group) {
         TObjArray *arr = TString(path).Tokenize("/");
         TCmdGroup *parent = this;
         for (Int_t i=0; i < arr->GetEntries(); ++i) {
            group = new TCmdGroup;
            group->SetName(((TObjString*)arr->UncheckedAt(i))->GetString());
            parent->Add(group);
            parent = group;
         }
      }
      group->Register(cmd);
      return;
   }

   TObject *obj = FindObjectAny(cmd->GetName());
   if (!obj) {
      Add(cmd);
   } else if (replace) {
      Remove(obj);
      delete obj;
      Add(cmd);
   }
   ((TList*)GetListOfFolders())->Sort();
}

void TCmdGroup::Help()
{
   printf("Subcommands for %s:\n",GetName());
   CmdHelp();
}


void TCmdGroup::CmdHelp(TString cmd)
{
//   Info("CmdHelp","cmd = %s",cmd.Data());
   TObject *obj;
   if (!cmd.IsNull() && ((obj = FindCommand(cmd)) || (obj = FindObjectAny(cmd)))) {
//      Info("CmdHelp","found %p",obj);
      
      printf("\n");
      printf("Command : %s\n",obj->GetName());
      printf("Summary : %s\n",obj->GetTitle());
      printf("\n");
      ((TCatCmd*)obj)->Help();
   } else {
//      Info("CmdHelp","not found");
      TIter it(GetListOfFolders());
      while ((obj = it.Next())) {
//         printf("%s\n",obj->IsA()->GetName());
         printf("   %-10s %s\n",obj->GetName(),obj->GetTitle());
      }
   }
//   Info("CmdHelp","done");
}
