/**
 * @file   TArtemisUtil.cc
 * @brief
 *
 * @date   Created       : 2018-07-26 16:33:05 JST
 *         Last Modified : 2018-09-05 13:57:25 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2018 Shinsuke OTA
 */

#include "TArtemisUtil.h"
#include <TString.h>
#include <TSystem.h>
#include <TFileMerger.h>
#include <iostream>
#include <TAnalysisInfo.h>
#include <TDirectory.h>
#include <TFile.h>
#include <TROOT.h>
#include <TFolder.h>
#include <TClass.h>
#include <TKey.h>
#include "TMath.h"
#include "TH1FTreeProj.h"
#include "TH2FTreeProj.h"
#include "TH3FTreeProj.h"

using namespace art;


Bool_t Util::PrepareDirectoryFor(const char *filename)
{
   TString pwd = gSystem->pwd();
   TString target = gSystem->DirName(filename);
   if (gSystem->ChangeDirectory(target)) {
      // target directory exists
      gSystem->ChangeDirectory(pwd);
      return kTRUE;
   } else {
      return ( 0 == gSystem->mkdir(target,kTRUE) );
   }
}


Bool_t Util::LoadAnalysisInformation(const char *infoName)
{
   TString name = (infoName) ? infoName : TAnalysisInfo::kDefaultAnalysInfoName;
   TDirectory *topdir = gDirectory->GetFile();
   TObject *cachedobj = NULL;
   TObject * obj = NULL;
   TFolder *topfolder = static_cast<TFolder*>(gROOT->FindObject("/artemis"));
   cachedobj = topfolder->FindObject(name);

   // find directory object
   
   if ( NULL == topdir ) {
      topdir = gROOT;
      obj = topdir->FindObject(name);
   } else {
      obj = topdir->Get(name);
   }

   if ( !obj && !cachedobj ) return kFALSE;
   if ( obj == cachedobj ) return kTRUE;
   if ( obj ) {
      topdir->Add(obj);
      if ( cachedobj ) {
         topfolder->Remove(cachedobj);
      }
      topfolder->Add(obj);
   }
      
   return kTRUE;
}


void Util::GetPolygonFromSide(Int_t n, Double_t side, Double_t angle, Double_t *vx, Double_t *vy, Double_t xoffset, Double_t yoffset)
{
   Double_t theta = 2. * TMath::Pi() / n;
   Double_t r = side * TMath::Abs(TMath::Sin( (TMath::Pi() - theta)/2. ) / TMath::Sin(theta));

   for (Int_t i = 0; i < n + 1; ++i) {
      Double_t t = theta * i + angle;
      Double_t sint = TMath::Sin(t);
      Double_t cost = TMath::Cos(t);
      vx[i] = r * cost + xoffset;
      vy[i] = r * sint + yoffset;
   }
}

Bool_t Util::WriteObjectsToFile(const char* fileName, const Option_t *opt)
{
   TDirectory *saved = gDirectory;
   TFile *file = TFile::Open(fileName,opt);
   if (!file || !file->IsOpen()) {
      std::cerr << "Util::WriteObjectsToFile : cannot open file " << fileName << std::endl;
      return kFALSE;
   }
   WriteObjectsRecursively(file,saved->GetList());
   file->Close();
   saved->cd();
   return kTRUE;
}

void Util::WriteObjectsRecursively(TDirectory* parent, TList* list)
{
   if (!parent || !list) return;
   Int_t nObj = list->GetEntries();
   for (Int_t i=0; i != nObj; i++) {
      TObject *obj = list->At(i);
      parent->cd();
      if (obj->InheritsFrom("TH1")) {
         if (obj->InheritsFrom("art::TH1FTreeProj")) {
            TH1F(*(art::TH1FTreeProj*)obj).Write();
         } else if (obj->InheritsFrom("art::TH2FTreeProj")) {
            TH2F(*(art::TH2FTreeProj*)obj).Write();
         } else if (obj->InheritsFrom("art::TH2FTreeProj")) {
            TH3F(*(art::TH3FTreeProj*)obj).Write();
         } else {
            obj->Write();
         }
      } else if (obj->InheritsFrom("TDirectory")) {
         TDirectory *dir    = (TDirectory*)obj;
         TDirectory *newdir = parent->mkdir(dir->GetName());
         newdir->SetTitle(dir->GetTitle());
         WriteObjectsRecursively(newdir,dir->GetList());
      } else {
         obj->Write();
      }
   }   
}


#ifdef USE_MPI
Bool_t Util::MPIFileMerger(const char* filename)
{
   int myrank, npe,useMPI;
   MPI_Initialized(&useMPI);
   if (!useMPI) return kTRUE;
   // treated file is already local 
   const Bool_t force = kTRUE;
   const Int_t newcomp = 0;
   const Int_t verbosity = 99;
   Bool_t status = kFALSE;

   MPI_Comm_size(MPI_COMM_WORLD, &npe);
   MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
   MPI_Barrier(MPI_COMM_WORLD);   
   if (npe > 0 && myrank == 0) {
      TFileMerger merger(kFALSE,kFALSE);
      merger.SetMsgPrefix("MPIFileMerger");
      merger.SetPrintLevel(verbosity - 1);
      if (!merger.OutputFile(filename,force, newcomp)) {
         std::cerr << "MPIFileMerger cannot open output file " << filename << std::endl;
         return kFALSE;
      }
      TString files;
      for (Int_t i = 0; i < npe; ++i) {
         // ファイルの存在の確認はしたほうが良い？mergerがやってくれているか？
         merger.AddFile(Form("%s%d ",filename,i));
      }
      status = merger.Merge();
      for (Int_t i = 0; i < npe; ++i) {
         TString file = TString::Format("%s%d",filename,i);
         if (NULL != gSystem->FindFile(".",file)) {
            gSystem->Exec(Form("rm %s",file.Data()));
         }
      }
   }
   MPI_Barrier(MPI_COMM_WORLD);
   return status;
}
#endif
