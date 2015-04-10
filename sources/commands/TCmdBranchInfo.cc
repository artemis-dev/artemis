/**
 * @file   TCmdBranchInfo.cc
 * @brief  display branches
 *
 * @date   Created       : 2015-04-09 14:37:54 JST
 *         Last Modified : Apr 10, 2015 18:28:29 JST
 * @author  Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 * branchinfo 
 *   display the branches in the tree found first in the current directory
 * branchinfo [branch]
 *   Displays the class information of branch. If the class is TClonesArray, the contained class information will be displayed.
 *
 *    (C) 2015
 */

#include "TCmdBranchInfo.h"
#include <TDirectory.h>
#include <TTree.h>
#include <TClonesArray.h>
#include <TKey.h>
#include <TROOT.h>
#include <TCmdClassInfo.h>

using art::TCmdBranchInfo;

ClassImp(TCmdBranchInfo)

TCmdBranchInfo::TCmdBranchInfo()
{
   SetName("branchinfo");
   SetTitle("show branches and their classes or show class information of a branch");
}

TCmdBranchInfo::~TCmdBranchInfo()
{
}

TCmdBranchInfo::TCmdBranchInfo(const TCmdBranchInfo& rhs)
{
}

TCmdBranchInfo& TCmdBranchInfo::operator=(const TCmdBranchInfo& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}

Long_t TCmdBranchInfo::Cmd(vector<TString> args)
{
   if (args.size() > 2) {
      Help();
   } 
   TTree* tree = GetTree();
   TString brname;
   if (!tree) {
      Warning("Cmd","No TTree is found");
      return 1;
   }
   if (args.size() == 2) {
      brname = args[1];
      Bool_t isClonesArray = kFALSE;
      TClass *cls = GetConcreteClass(tree,tree->GetBranch(brname),&isClonesArray);
      if (!cls) {
         Error("Cmd","No such branch '%s' or unexpected",brname.Data());
         return 1;
      }
      TCmdClassInfo *info = new TCmdClassInfo;
      info->Run(cls->GetName());
      delete info;
      return 1;
   }
   Run(tree);
   return 1;
}

Long_t TCmdBranchInfo::Run(TTree *tree)
{
   if (!tree) {
      Warning("Run","Tree is NULL\n");
      return 1;
   }
   TObjArray *branches = tree->GetListOfBranches();
   TIterator *it = branches->MakeIterator();
   while (TBranch *br = (TBranch*)it->Next()) {
      Bool_t isClonesArray = kFALSE;
      TClass *cls = GetConcreteClass(tree,br,&isClonesArray);
      if (!cls) continue;
      TString clsname = cls->GetName();
      if (isClonesArray) {
         clsname = TString(TClonesArray::Class_Name());
         clsname.Append("(").Append(cls->GetName()).Append(")");
      }
      printf("%-20s %-20s\n",br->GetName(),clsname.Data());
   }

   return 1;
}

void TCmdBranchInfo::Help()
{
   TString help;
   help.Append("SYNOPSIS\n");
   help.Append(" branchinfo [branch]\n");
   help.Append("\n");
   help.Append("DISCRIPTION\n");
   help.Append("   Display list of branches in a first TTree in the current directory\n");
   help.Append("   or show class information of branch\n");
   help.Append("\n");
   printf("%s",help.Data());
}

TTree* TCmdBranchInfo::GetTree()
{
   TObject *ret = NULL;
   { // check object first
      TIter it(gDirectory->GetList());
      while (TObject *obj = it()) {
         if (obj->InheritsFrom("TTree")) {
            ret = obj;
            break;
         }
      }
   }
   if (!ret) {
      TIter it(gDirectory->GetListOfKeys());
      while (TKey *key = (TKey*)it()) {
         TObject *obj = key->ReadObj();
         if (obj->InheritsFrom("TTree")) {
            ret = obj;
            break;
         }
      }
   }
   return (TTree*)ret;
}

TClass* TCmdBranchInfo::GetConcreteClass(TTree* tree, TBranch*br, Bool_t* isClonesArray)
{
   TClass *cls = NULL;
   EDataType type;;
   *isClonesArray = kFALSE;
   if (!br) return NULL;
   if (br->GetExpectedType(cls,type)) {
      Warning("Init","Unresolved type for branch '%s'",br->GetName());
      return NULL;
   }
   TString clsname = cls->GetName();
   if (cls == TClonesArray::Class()) {
      TClonesArray *arr = NULL;
      tree->SetBranchAddress(br->GetName(),&arr);
      tree->GetEntry(0);
      cls = arr->GetClass();
      *isClonesArray = kTRUE;
      br->ResetAddress();
   }
   return cls;
}
