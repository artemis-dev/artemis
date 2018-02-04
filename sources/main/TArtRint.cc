/* $Id:$ */
/**
 * @file   TArtRint.cc
 * @date   Created : Feb 06, 2012 00:06:18 JST
 *   Last Modified : 2018-02-04 13:06:40 JST (ota)
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
   TRint::Terminate(status);
}
