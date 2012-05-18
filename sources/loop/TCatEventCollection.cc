/* $Id:$ */
/**
 * @file   TCatEventCollection.cc
 * @date   Created : Apr 26, 2012 23:26:27 JST
 *   Last Modified : May 18, 2012 09:50:06 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TCatEventCollection.h"
#include <TList.h>

#include <TFile.h>
#include <TROOT.h>

const TString TCatEventCollection::kDefaultOutputName = "cattemp.root";

TCatEventCollection::TCatEventCollection()
{
   fObjects = new TList;
   fOutputObjects = new TList;
   fOutputTree = NULL;
   fOutputFileName = kDefaultOutputName;
}
TCatEventCollection::~TCatEventCollection()
{
   fObjects->Clear();
   fOutputObjects->Clear();
   fOutputFile->Write();
   fOutputFile->Close();
   delete fObjects;
   delete fOutputObjects;
}


void TCatEventCollection::Add(TNamed *obj, Bool_t isPassive)
{
   if (Get(obj->GetName())) {
      // object which has the same name exists
      fOutputObjects->Remove(obj);
      if (isPassive) return ;
      fOutputObjects->Add(obj);
      return;
   }

   fObjects->Add(obj);
   if (isPassive) return;
   fOutputObjects->Add(obj);
}

void TCatEventCollection::Add(TList *list, Bool_t isPassive)
{
   TObjLink *lnk = list->FirstLink();
   list->Print();
   while (lnk) {
      Add((TNamed*)lnk->GetObject(),isPassive);
      lnk = lnk->Next();
   }
}

TObject* TCatEventCollection::Get(const char *name) 
{
   return fObjects->FindObject(name);
}

void TCatEventCollection::CreateOutput(const char *name, const char *title,
                                       const char *filename, const char *opt)
{
   // if output objects
   fOutputFileName = filename;
   if (fOutputFileName.IsNull()) fOutputFileName = kDefaultOutputName;
   fOutputFile = TFile::Open(fOutputFileName,opt);
   if (!fOutputFile) {
      fOutputFileName = kDefaultOutputName;
      fOutputFile = TFile::Open(fOutputFileName,"RECREATE");
   }
   if (fOutputObjects->GetEntries()) {
      fOutputTree = new TTree(name,title);
      fOutputTree->Branch(fOutputObjects,32000,1);
   }
}

void TCatEventCollection::Init()
{
   
}
