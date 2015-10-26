/* $Id:$ */
/**
 * @file   TArtRint.cc
 * @date   Created : Feb 06, 2012 00:06:18 JST
 *   Last Modified : Oct 26, 2015 06:35:51 EDT
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
   
   // generate list of processors
   gClassTable->Init();
   Int_t nCls = gClassTable->Classes();
   for (Int_t iCls = 0; iCls != nCls; ++iCls) {
      TString name(gClassTable->Next());
      if (name.BeginsWith("vector")) continue;
      if (name.BeginsWith("pair")) continue;
      if (name.BeginsWith("reverse")) continue;
      if (name.BeginsWith("map")) continue;
      if (name.BeginsWith("list")) continue;
      if (name.EndsWith("Iter")) continue;
      TClass *cls = TClass::GetClass(name);
      // printf("cls : %s %d\n",name.Data(),cls->InheritsFrom(TProcessor::Class_Name()));
      if (cls && cls->IsLoaded() &&
          cls->InheritsFrom(TProcessor::Class_Name()) &&
          cls != TProcessor::Class()
         ) {
         gProcessors->Add(cls);
      }
   }
      

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
