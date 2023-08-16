/* $Id:$ */
/**
 * @file   TTreeEventStore.cc
 * @date   Created : Jul 11, 2013 21:11:20 JST
 *   Last Modified : 2023-08-16 16:25:00 JST (fendo)
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
#include <TLeaf.h>
#include <TROOT.h>
#include <TClonesArray.h>
#include <TEventHeader.h>
#include "TSystem.h"
#include "TChain.h"
#include "TObjString.h"
ClassImp(art::TTreeEventStore);



art::TTreeEventStore::TTreeEventStore()
   : fTree(NULL),fEventHeader(NULL),fFile(NULL)
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
   // extract files to be analyzed by this process 
   std::vector<TString> files;
   TString filelist = gSystem->GetFromPipe(Form("ls -tr %s",fFileName.Data()));
   TObjArray *allfiles = (filelist.Tokenize("\n"));
   if (!allfiles) {
      Error("Init","No such files %s",fFileName.Data());
      SetStateError("Init");
      return;
   } 
#if USE_MPI
   int useMPI;
   MPI_Initialized(&useMPI);
   if (useMPI) {
      MPI_Comm_size(MPI_COMM_WORLD, &fNPE);
      MPI_Comm_rank(MPI_COMM_WORLD, &fRankID);
      TChain *chain = new TChain(fTreeName);
      for (Int_t i = fRankID, n = allfiles->GetEntriesFast(); i < n; i+=fNPE) {
         Info("Init","Add '%s' to Rank %03d",static_cast<TObjString*>(allfiles->At(i))->GetName(),fRankID);
         chain->Add(static_cast<TObjString*>(allfiles->At(i))->GetName());
      }
      fTree = chain;
   }   
#endif
   if (!fTree) {
      TChain *chain = new TChain(fTreeName);
      for (Int_t i = 0, n = allfiles->GetEntriesFast(); i < n; ++i) {
         Info("Init","Add '%s'",static_cast<TObjString*>(allfiles->At(i))->GetName());
         chain->Add(static_cast<TObjString*>(allfiles->At(i))->GetName());
      }      
      fTree = chain;
   }

   // TTreeが使える状況であるかのチェックをして、使えなければこのプロセスはストップするということにする。
   // 万が一プロセス数がファイルの数を超えるような場合でも問題はない。ということは 144 以上のプロセスでくんでもよい。
   // その場合は、Tree projection のマージで引っかからないように気をつけなければならない。か
   fEventNum = 0;
   fCondition = (TConditionBit**)(col->Get(TLoop::kConditionName)->GetObjectRef());
   TDirectory *saved = gDirectory;
//   fFile = TFile::Open(fFileNam);
//   saved->cd();
//   if (!fFile) {
//      (*fCondition)->Set(TLoop::kStopLoop);
//      return ;
//   }
//   Info("Init","Input tree = %s",fTreeName.Data());
   std::vector<TBranch*> useBranch;
//   fTree = (TTree*)fFile->Get(fTreeName);
   if (!fTree) {
      Error("Init","Input tree '%s' does not exist in '%s'",fTreeName.Data(),
            fFileName.Data());
      (*fCondition)->Set(TLoop::kStopLoop);
      return;
   }
   TIter next(fTree->GetListOfBranches());
   TBranch *br = NULL;
   while ((br =(TBranch*)next())) {
      TClass *cls = NULL;
      EDataType type;;
      if (br->GetExpectedType(cls,type)) {
         Warning("Init","Unresolved type for branch '%s'",br->GetName());
         continue;
      } else if (!cls) {
         // primitive types
         void *input = NULL;
         Int_t counter = 0;
         // get dimension
         TLeaf *leaf = br->GetLeaf(br->GetName())->GetLeafCounter(counter);
         if (leaf) counter = leaf->GetMaximum();
         switch (type) {
         case kInt_t:
            input = (void*) new Int_t[counter];
            break;
         case kFloat_t:
            input = (void*) new Float_t[counter];
            break;
         case kDouble_t:
            input = (void*) new Double_t[counter];
            break;
         case kULong64_t:
//            input = (void*) new ULong64_t[counter];
            break;
         default:
            break;
         }
         if (!input) {
//            SetStateError(TString::Format("branch: %s is unsupported type %s",br->GetName(),TDataType::GetTypeName(type)));
//            return;
         } else {
            useBranch.push_back(br);
            col->Add(new TEventObject(br->GetName(),input,TString(TDataType::GetTypeName(type)),NULL));
            fTree->SetBranchAddress(br->GetName(),*col->Get(br->GetName())->GetObjectRef());
            printf("branch : %s (%s) %p\n",br->GetName(),TDataType::GetTypeName(type),*col->Get(br->GetName())->GetObjectRef());
         }
      } else {
#if 1
         if (cls == TClonesArray::Class()) {
            TClonesArray *arr = NULL;
            fTree->SetBranchStatus("*",0);
            fTree->SetBranchStatus(br->GetName());
            fTree->SetBranchAddress(br->GetName(),&arr);
            fTree->GetEntry(0);
            TClass *realcls = arr->GetClass();
            if (!realcls->GetNew()) {
               cls = 0;
            }
            br->ResetAddress();
         } 
#endif         
         if (cls) {
            useBranch.push_back(br);
            // object known by ROOT
            col->Add(br->GetName(),(TObject*)cls->New(),kTRUE);
            fTree->SetBranchAddress(br->GetName(),(TObject**)col->Get(br->GetName())->GetObjectRef());
            printf("branch : %s\n",br->GetName());
         }
         if (cls == TEventHeader::Class()) {
            fEventHeader = reinterpret_cast<TEventHeader**>(col->GetObjectRef(br->GetName()));
         }
      }
   }
   TIter nextinfo(fTree->GetUserInfo());
   TObject *obj = NULL;
   while ((obj = nextinfo()) != NULL) {
      col->AddInfo(obj->GetName(),obj,kTRUE);
   }
   fTree->SetBranchStatus("*",0);
   for (Int_t i = 0, n = useBranch.size(); i < n; ++i) {
      fTree->SetBranchStatus(useBranch[i]->GetName());
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


Int_t art::TTreeEventStore::GetRunNumber() const
{
   if ( NULL == fEventHeader ) return 0;
   return (*fEventHeader)->GetRunNumber();
}

const char* art::TTreeEventStore::GetRunName() const
{
   if ( NULL == fEventHeader ) return "";
   return (*fEventHeader)->GetRunName();
}
