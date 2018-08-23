/**
 * @file   TArtemisUtil.cc
 * @brief
 *
 * @date   Created       : 2018-07-26 16:33:05 JST
 *         Last Modified : 2018-08-07 17:07:09 JST (ota)
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
