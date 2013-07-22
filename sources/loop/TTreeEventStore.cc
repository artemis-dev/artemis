/* $Id:$ */
/**
 * @file   TTreeEventStore.cc
 * @date   Created : Jul 11, 2013 21:11:20 JST
 *   Last Modified : 
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
}


void art::TTreeEventStore::Init(TEventCollection *col)
{
   fEventNum = 0;
   fCondition = (TConditionBit**)(col->Get(TLoop::kConditionName)->GetObjectRef());
   printf("fFileName = %s\n",fFileName.Data());
   fFile = TFile::Open(fFileName);
   if (!fFile) {
      (*fCondition)->Set(TLoop::kStopLoop);
      return ;
   }
   printf("fTreeName = %s\n",fTreeName.Data());
   fTree = (TTree*)fFile->Get(fTreeName);
   if (!fTree) {
      (*fCondition)->Set(TLoop::kStopLoop);
      return;
   }
   TIter next(fTree->GetListOfBranches());
   TBranch *br = NULL;
   while ((br =(TBranch*)next())) {
      col->Add(br->GetName(),(TObject*)TClass::GetClass(br->GetClassName())->New(),kFALSE);
      printf("branch : %s",br->GetName());
      fTree->SetBranchAddress(br->GetName(),col->Get(br->GetName())->GetObjectRef());
   }
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
