/**
 * @file   TArtemisUtil.cc
 * @brief
 *
 * @date   Created       : 2018-07-26 16:33:05 JST
 *         Last Modified : 2018-07-30 12:57:22 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2018 Shinsuke OTA
 */

#include "TArtemisUtil.h"
#include <TString.h>
#include <TSystem.h>
#include <TFileMerger.h>
#include <iostream>
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
