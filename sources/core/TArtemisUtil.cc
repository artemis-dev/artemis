/**
 * @file   TArtemisUtil.cc
 * @brief
 *
 * @date   Created       : 2018-07-26 16:33:05 JST
 *         Last Modified : 2018-07-30 15:05:02 JST (ota)
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
   TObject *obj = NULL;
   if ( NULL == topdir ) {
      topdir = gROOT;
      obj = topdir->FindObject(name);
   } else {
      TFile *file = (TFile*)topdir;
      TIter next(file->GetListOfKeys());
      while (TKey *key = (TKey*) next()) {
         if (TClass::GetClass(key->GetClassName()) != TAnalysisInfo::Class()) continue;

         // analysis info was found
         obj = key->ReadObj();
         break;
      }
   }
   if ( NULL ==  obj ) return kFALSE;
   TFolder *topfolder = static_cast<TFolder*>(gROOT->FindObject("/artemis"));
   if (!topfolder) return kFALSE;
   topfolder->Add(obj);
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
   TFileMerger merger(kFALSE,kFALSE);
   merger.SetMsgPrefix("MPIFileMerger");
   merger.SetPrintLevel(verbosity - 1);
   if (!merger.OutputFile(filename,force, newcomp)) {
      std::cerr << "MPIFileMerger cannot open output file " << filename << std::endl;
      return kFALSE;
   }

   MPI_Comm_size(MPI_COMM_WORLD, &npe);
   MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
   MPI_Barrier(MPI_COMM_WORLD);   
   if (npe > 0 && myrank == 0) {
      TString files;
      for (Int_t i = 0; i < npe; ++i) {
         merger.AddFile(Form("%s%d ",filename,i));
      }
      status = merger.Merge();
   }
   MPI_Barrier(MPI_COMM_WORLD);
   return status;
}
#endif
