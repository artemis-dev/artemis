/* $Id:$ */
/**
 * @file   TArtRint.cc
 * @date   Created : Feb 06, 2012 00:06:18 JST
 *   Last Modified : 2018-08-06 22:50:25 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TArtRint.h"
#include <TFolder.h>
#include <TROOT.h>
#include <TInterpreter.h>
#include "TLoopManager.h"
#include "TCatCmdFactory.h"
#include "TCatCmdMacro.h"
#include "TArtAtomicMassTable.h"
#include "TEnv.h"
#include <TClassTable.h>
#include "TProcessor.h"

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef HAVE_MPI_H
#ifdef USE_MPI
#include <mpi.h>
#include <TRandom.h>
#endif
#endif

using art::TArtRint;

TArtRint::TArtRint(int* argc, char** argv, void* options, int numOptions, Bool_t noLogo)
   : TRint(gAppName, argc, argv, options, numOptions, noLogo)
{
   // load macros
   // @TODO documantation: all the processors should be loaded in artemislogon.C
   TRint::ProcessLine(".x artemislogon.C");

   TProcessor::ListProcessors();

   // Preparation of folder for artemis
   TFolder *top = gROOT->GetRootFolder()->AddFolder("artemis","artemis top level folders");
   gROOT->GetListOfBrowsables()->Add(top);

   // set prompt
   SetPrompt("artemis [%d] ");
   
   // load mass table
   TString filepath;
   if (!gEnv) {
      printf("gEnv is not prepared\n");
      return;
   }
   filepath = gEnv->GetValue("Art.MassTable",filepath);
   if (filepath.IsNull()) {
      gAtomicMassTable->Build();
      printf("Use mass table inside root since Art.MassTable is not defined in .rootrc\n");
   } else {
      gAtomicMassTable->SetMassTable(filepath,40);
   }
#if USE_MPI
    int myrank, npe;
    int usempi;
    MPI_Initialized(&usempi);
    if (usempi) {
       MPI_Comm_size(MPI_COMM_WORLD, &npe);
       MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
       gRandom->SetSeed(myrank);
    }
#endif
   
   
}
TArtRint::~TArtRint()
{
}

Long_t TArtRint::ProcessLine(const char* line, Bool_t sync, Int_t* error)
{
//   printf("%s\n",line);
   if (sync) {
      return TRint::ProcessLine(line,sync,error);
   }
   // prepare command line for artemis
   {
      TString lines(line);
      TObjArray *arr = lines.Tokenize('\n');
      std::vector<TString> cmds;
      for (Int_t i = 0; i < arr->GetEntries(); ++i) {
         TString aCmd = ((TObjString*)arr->At(i))->GetString();
         if (!aCmd.BeginsWith("#")) {
            cmds.push_back(aCmd);
         }
      }
      TString cmdline;
      for (Int_t i=0; i<cmds.size(); ++i) {
         if (i>0) cmdline.Append("\n");
         cmdline.Append(cmds[i]);
      }
      if (TCatCmdFactory::Instance()->ProcessLine(cmdline)) {
         return 1;
      } else {
         return TRint::ProcessLine(line,sync,error);
      }
   }
   return 0;
}

void   TArtRint::Terminate(Int_t status)
{
   art::TLoopManager *lm = art::TLoopManager::Instance();
   Int_t n = lm->GetEntries();
   for (Int_t i=0; i!=n; i++) {
      lm->Terminate(i);
   }
#ifdef USE_MPI
   int useMPI;
   MPI_Initialized(&useMPI);
   if (useMPI) {
      MPI_Barrier(MPI_COMM_WORLD);
      MPI_Finalize();
   }
#endif
   TRint::Terminate(status);
}
