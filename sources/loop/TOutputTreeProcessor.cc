/* $Id:$ */
/**
 * @file   TOutputTreeProcessor.cc
 * @date   Created : Jul 11, 2013 17:11:41 JST
 *   Last Modified : Jul 22, 2013 18:47:20 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#include "TOutputTreeProcessor.h"
#include <TEventObject.h>
#include <TDirectory.h>

ClassImp(art::TOutputTreeProcessor);

art::TOutputTreeProcessor::TOutputTreeProcessor()
   : fFile(NULL), fTree(NULL) {
   RegisterProcessorParameter("FileName","The name of output file",fFileName,TString("temp.root"));
   RegisterProcessorParameter("TreeName","The name of output tree",fTreeName,TString("tree"));
   fObjects = new TList;
}
art::TOutputTreeProcessor::~TOutputTreeProcessor()
{
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
      fTree->Branch(obj->GetName(),obj->GetClass()->GetName(),obj->GetObjectRef(),3200000,0);
      fObjects->Add(obj);
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
      ((TBranch*)br)->SetAddress(obj->GetObjectRef());
   }
}
void art::TOutputTreeProcessor::PostLoop()
{
   TObject *obj = 0;
   TIter next(fTree->GetListOfBranches());
   while ((obj = next())) {
      ((TBranch*)obj)->ResetAddress();
   }
   TDirectory *saved = gDirectory;
   fFile->cd();
   fTree->Write(0,TFile::kOverwrite);
   saved->cd();
}
