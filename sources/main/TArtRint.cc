/* $Id:$ */
/**
 * @file   TArtRint.cc
 * @date   Created : Feb 06, 2012 00:06:18 JST
 *   Last Modified : Oct 27, 2015 09:39:48 EDT
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
      printf("Art.MassTable is not defined in .rootrc\n");
      return;
   }
   gAtomicMassTable->SetMassTable(filepath,40);
   
}
TArtRint::~TArtRint()
{
}

Long_t TArtRint::ProcessLine(const char* line, Bool_t sync, Int_t* error)
{
   if (sync) {
      return TRint::ProcessLine(line,sync,error);
   }
   if (TCatCmdFactory::Instance()->ProcessLine(TString(line))) {
      return 1;
   } else {
      return TRint::ProcessLine(line,sync,error);
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
   TRint::Terminate(status);
}
