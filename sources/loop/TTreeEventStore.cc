/* $Id:$ */
/**
 * @file   TTreeEventStore.cc
 * @date   Created : Jul 11, 2013 21:11:20 JST
 *   Last Modified : Dec 02, 2014 12:52:08 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#include "TTreeEventStore.h"
#include <TFile.h>
#include <TList.h>
#include <TTree.h>
#include <TProcessor.h>
#include <TLoop.h>
#include <TConditionBit.h>

ClassImp(art::TTreeEventStore);



art::TTreeEventStore::TTreeEventStore()
{
   RegisterProcessorParameter("FileName","The name of output file",fFileName,TString("temp.root"));
   RegisterProcessorParameter("TreeName","The name of output tree",fTreeName,TString("tree"));
   RegisterProcessorParameter("MaxEventNum",
                              "The maximum event number to be analyzed."
                              " Analyze all the data if this is set to be 0",
                              fMaxEventNum,0L);
   fObjects = new TList;
}
art::TTreeEventStore::~TTreeEventStore()
{
   if (fFile) fFile->Close();
   fTree = NULL;
}


void art::TTreeEventStore::Init(TEventCollection *col)
{
   fEventNum = 0;
   fCondition = (TConditionBit**)(col->Get(TLoop::kConditionName)->GetObjectRef());
   Info("Init","Input file = %s",fFileName.Data());
   TDirectory *saved = gDirectory;
   fFile = TFile::Open(fFileName);
   saved->cd();
   if (!fFile) {
      (*fCondition)->Set(TLoop::kStopLoop);
      return ;
   }
   Info("Init","Input tree = %s",fTreeName.Data());
   fTree = (TTree*)fFile->Get(fTreeName);
   if (!fTree) {
      Error("Init","Input tree '%s' does not exist in '%s'",fTreeName.Data(),
            fFileName.Data());
      (*fCondition)->Set(TLoop::kStopLoop);
      return;
   }
   TIter next(fTree->GetListOfBranches());
   TBranch *br = NULL;
   while ((br =(TBranch*)next())) {
      col->Add(br->GetName(),(TObject*)TClass::GetClass(br->GetClassName())->New(),kTRUE);
      printf("branch : %s\n",br->GetName());
      fTree->SetBranchAddress(br->GetName(),(TObject**)col->Get(br->GetName())->GetObjectRef());
   }
   TIter nextinfo(fTree->GetUserInfo());
   TObject *obj = NULL;
   while ((obj = nextinfo()) != NULL) {
      col->AddInfo(obj->GetName(),obj,kTRUE);
   }
   fTree->LoadTree(0);
   fTree->GetEntry(0);
   if (!fMaxEventNum) fMaxEventNum = fTree->GetEntries();
}
void art::TTreeEventStore::Process()
{
   fTree->GetEntry(fEventNum);
   fEventNum++;
   if (fMaxEventNum == fEventNum) {
      SetStopLoop();
      SetEndOfRun();
   }
}
