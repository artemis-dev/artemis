/* $Id:$ */
/**
 * @file   TCatEventCollectionManager.h
 * @date   Created : Feb 19, 2012 11:19:47 JST
 *   Last Modified : Apr 26, 2012 22:12:19 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#ifndef TCATEVENTCOLLECTIONMANAGER_H
#define TCATEVENTCOLLECTIONMANAGER_H

#include <TTree.h>
#include <TList.h>

#include <vector>
using namespace std;


class TCatEventCollectionManager  {

private:
   TCatEventCollectionManager();
public:
   virtual ~TCatEventCollectionManager();

   static TCatEventCollectionManager* Instance();

   virtual void Add(TNamed *obj, Bool_t isPassive = kTRUE);
   virtual void Clear(Option_t *opt = "");
   TTree *CreateTree(const char *name, const char* title);
   TObject* Find(const char *name) {
      return fObjects->FindObject(name);
   }
   void Load(TTree *tree);

   void GetEntry(Int_t iEntry) {
      TIter next(fInputTrees->MakeIterator());
      TTree *tree =NULL;
      while ((tree = (TTree*) next())) {
         tree->GetEntry(iEntry);
      }
   }
   void Fill() {
      fOutputTree->Fill();
   }


protected:
   TList *fObjects;
   TList *fOutputObjects;
   TList *fInputTrees;
   TTree *fOutputTree;
   vector<TObject**> fBranchAddresses;

};
#endif // end of #ifdef TCATEVENTCOLLECTIONMANAGER_H
