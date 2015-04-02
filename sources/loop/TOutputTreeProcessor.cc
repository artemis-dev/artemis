/* $Id:$ */
/**
 * @file   TOutputTreeProcessor.cc
 * @date   Created : Jul 11, 2013 17:11:41 JST
 *   Last Modified : Feb 06, 2015 05:20:36 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#include "TOutputTreeProcessor.h"
#include <TEventObject.h>
#include <TDirectory.h>
#include <TROOT.h>

ClassImp(art::TOutputTreeProcessor);

art::TOutputTreeProcessor::TOutputTreeProcessor()
   : fFile(NULL), fTree(NULL) {
   RegisterProcessorParameter("FileName","The name of output file",fFileName,TString("temp.root"));
   RegisterProcessorParameter("TreeName","The name of output tree",fTreeName,TString("tree"));
   fObjects = new TList;
}
art::TOutputTreeProcessor::~TOutputTreeProcessor()
{
   if (fTree) fTree->GetUserInfo()->Clear();
   if (fFile) fFile->Close();
   delete fObjects;
}

void art::TOutputTreeProcessor::Init(TEventCollection *col)
{
   fFile = TFile::Open(fFileName,"RECREATE");
   fTree = new TTree(fTreeName,fTreeName);
   // assume all of the objects inherit from TObject
   TIter *iter = col->GetIter();
   TEventObject *obj;
   while ((obj = (TEventObject*)iter->Next())) {
      if (obj->IsPassive()) continue;
      if (!obj->IsObject()) {
         // primitivee class
         TString brNme = obj->GetName();
         TString leaflist;
         if (obj->GetLength().IsNull()) {
            leaflist = TString::Format("%s/%s",obj->GetName(),obj->GetType());
         } else {
            leaflist = TString::Format("%s[%s]/%s",obj->GetName(),obj->GetLength().Data(),obj->GetType());
         }
         fTree->Branch(obj->GetName(),*obj->GetObjectRef(),leaflist);
      } else {
         fTree->Branch(obj->GetName(),obj->GetClass()->GetName(),obj->GetObjectRef(),3200000,0);
      }
      fObjects->Add(obj);
   }

   TIter *infoiter = col->GetUserInfoIter();
   while ((obj = (TEventObject*)infoiter->Next())) {
      if (obj->IsPassive()) continue;
      fTree->GetUserInfo()->Add(*(TObject**)obj->GetObjectRef());
   }
}
void art::TOutputTreeProcessor::Process()
{
   fTree->Fill();
}
void art::TOutputTreeProcessor::PreLoop()
{
   TObjArray *list = fTree->GetListOfBranches();
   Int_t nBranch = list->GetEntriesFast();
   for (Int_t i = 0; i!= nBranch; i++) {
      TBranch *br = (TBranch*)list->At(i);
      TEventObject *obj = (TEventObject*) fObjects->FindObject(br->GetName());
      if (!obj) {
         printf("br : %s at %p\n",br->GetName(),obj);
         // something is wrong...
         continue;
      }
      if (!obj->IsObject()) {
         ((TBranch*)br)->SetAddress(*(void**)obj->GetObjectRef());
      } else {
         ((TBranch*)br)->SetAddress(obj->GetObjectRef());
      }
   }
}
void art::TOutputTreeProcessor::PostLoop()
{
   TObject *obj = 0;
   TIter next(fTree->GetListOfBranches());
   while ((obj = next())) {
      ((TBranch*)obj)->ResetAddress();
   }
   if (!gDirectory) gDirectory = gROOT;
   TDirectory *saved = gDirectory;
   fFile->cd();
   fTree->Write(0,TFile::kOverwrite);
   saved->cd();
}
