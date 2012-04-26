/* $Id:$ */
/**
 * @file   TCatEventCollectionManager.cc
 * @date   Created : Feb 19, 2012 11:19:24 JST
 *   Last Modified : Feb 19, 2012 15:00:58 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TCatEventCollectionManager.h"


TCatEventCollectionManager::TCatEventCollectionManager()
   : fOutputTree(NULL) {
   fObjects = new TList;
   fOutputObjects = new TList;
   fInputTrees = new TList;
}
TCatEventCollectionManager::~TCatEventCollectionManager()
{
   delete fObjects;
   delete fOutputObjects;
   delete fInputTrees;
//   for (UInt_t i=0; i< fBranchAddresses.size(); i++) {
//      delete fBranchAddresses[i];
//   }
//   delete fOutputTree;
}

void TCatEventCollectionManager::Clear(Option_t *opt)
{
   TObject *obj = NULL;
   {
      TIter next(fObjects->MakeIterator());
      while ((obj = next())) {
         obj->Clear();
      }
   }
   {
      TIter next(fOutputObjects->MakeIterator());
      while ((obj = next())) {
         obj->Clear();
      }
   }
}

void TCatEventCollectionManager::Load(TTree *tree)
{
   TObjArray *brlist = tree->GetListOfBranches();
   TIter next(brlist->MakeIterator());
   TBranch *br = NULL;
   if (!fInputTrees->FindObject(tree)) {
      printf(" InputTree : %s\n",tree->GetName());
      fInputTrees->Add(tree);
   }
   while ((br=(TBranch*)next())) {
      TObject **obj = new TObject*;
      *obj = NULL;
      printf("name : %s\n",br->GetName());
      if (!TClass::GetClass(br->GetClassName())) continue;
      tree->SetBranchAddress(br->GetName(),obj);
      ((TNamed*)*obj)->SetName(br->GetName());
      Add((TNamed*) *obj);
      fBranchAddresses.push_back(obj);
   }
}

void TCatEventCollectionManager::Add(TNamed *obj, Bool_t isPassive)
{
   if (!fObjects->FindObject(obj)) {
      fObjects->Add(obj);
   }
   if (isPassive) {
      if (fOutputObjects->FindObject(obj)) fOutputObjects->Remove(obj);
   } else {
      if (!fOutputObjects->FindObject(obj)) fOutputObjects->Add(obj);
   }
}

TTree* TCatEventCollectionManager::CreateTree(const char *name, const char *title)
{
   TIter next(fOutputObjects->MakeIterator());
   fOutputTree = new TTree(name,title);
   TNamed *obj = NULL;
   while ((obj = (TNamed*)next())) {
      fOutputTree->Branch(obj->GetName(),obj,256000,0);
   }
   return fOutputTree;
}

TCatEventCollectionManager* TCatEventCollectionManager::Instance()
{
   static TCatEventCollectionManager instance;
   return &instance;
}
