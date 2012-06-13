/* $Id:$ */
/**
 * @file   TArtEventCollections.h
 * @date   Created : Sep 09, 2011 18:09:30 JST
 *   Last Modified : Oct 04, 2011 15:18:52 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2011
 */
#ifndef TARTEVENTCOLLECTIONS_H
#define TARTEVENTCOLLECTIONS_H

#include <TObject.h>
#include <TTree.h>
#include <TObjArray.h>
#include <TList.h>
// #include "TArtObjectsManager.h"


class TArtEventCollections  : public TObject {
public:
   virtual ~TArtEventCollections();

   TObject* GetObject(const char *name) {
      return fObjects->FindObject(name);
   }

   static void LoadObject(TTree *tree, const char *name) {
      TObjArray *brlist = tree->GetListOfBranches();
      TIterator *iter = brlist->MakeIterator();
      TBranch *br = NULL;
      TArtEventCollections *mng = TArtEventCollections::Instance();
      while ((br=(TBranch*)iter->Next())) {
         TObject **obj = new TObject*;
         *obj = NULL;
         printf("name : %s\n",br->GetName());
         if (!TClass::GetClass(br->GetClassName())) continue;
         tree->SetBranchAddress(br->GetName(),obj);
         ((TNamed*)*obj)->SetName(br->GetName());
         mng->AddObject((TNamed*) *obj);
      }
   }

   void AddObject(TNamed *obj, Bool_t isOutput = kTRUE) {
      fObjects->Add(obj);
      if (isOutput) fOutputObjects->Add(obj);
   }

   TTree *CreateTree(const char* name, const char* title) {
      TTree *tree = new TTree(name,title);
///      tree->Branch(fOutputObjects,256000,0);
      tree->Branch(fOutputObjects,256000,0);
      return tree;
   }

   static TArtEventCollections *Instance() {
      static TArtEventCollections instance;
      return &instance;
   }

   virtual void Print(Option_t * = "") const {
      fObjects->Print();
   }

      
protected:
   TList *fObjects;
   TList *fOutputObjects;

   TArtEventCollections(); 


   ClassDef(TArtEventCollections,1);
};
#endif // end of #ifdef TARTEVENTCOLLECTIONS_H
