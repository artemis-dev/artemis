/* $Id:$ */
/**
 * @file   TArtRint.cc
 * @date   Created : Feb 06, 2012 00:06:18 JST
 *   Last Modified : Feb 02, 2013 19:35:31 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TArtRint.h"
#include <TFolder.h>
#include <TROOT.h>

#include "TCatCmdFactory.h"
#include "TCatCmdMacro.h"
#include "TArtAtomicMassTable.h"
#include "TEnv.h"

// default value for CATLOGN
const char* kARTEMISLOGON_C = "artemislogon.C";

TArtRint::TArtRint(int* argc, char** argv, void* options, int numOptions, Bool_t noLogo)
   : TRint(fAppName, argc, argv, options, numOptions, noLogo)
{
   TRint::ProcessLine(".x artemislogon.C");
   TCatCmdFactory::Instance()->Register(TCatCmdMacro::Instance());

   // Preparation of folder for artemis
   TFolder *top = new TFolder("artemis","artemis");
   gROOT->GetListOfBrowsables()->Add(top);

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
   if (TCatCmdFactory::Instance()->ProcessLine(TString::Format("automacro %s",line))) {
      // macro was found
      return 1;
   } else
      if (TCatCmdFactory::Instance()->ProcessLine(line)) {
      return 1;
   }
   return TRint::ProcessLine(line,sync,error);
}
