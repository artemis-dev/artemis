/**
 * @file   TNArrayLoader.cc
 * @brief  TNArray loader
 *
 * @date   Created       : 2018-04-26 12:00:26 JST
 *         Last Modified : 2020-08-28 08:26:03 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2018 Shinsuke OTA
 */

#include "TNArrayLoader.h"
#include <TNArray.h>
#include <TFile.h>

using art::TNArrayLoader;

ClassImp(TNArrayLoader)

TNArrayLoader::TNArrayLoader()
: fFileName(""), fArrayName(""), fOutputName(""), fFile(NULL), fArray(NULL)
{
   RegisterProcessorParameter("FileName","name of input root file",fFileName,TString("path/to/rootfile"));
   RegisterProcessorParameter("ArrayName","name of TNArray",fArrayName,TString("table"));
   RegisterProcessorParameter("Output", "name of output",fOutputName,TString("table"));
}

TNArrayLoader::~TNArrayLoader()
{
   if (fFile) fFile->Close();
   if (fArray) fArray->Delete();
   fFile = NULL;
   fArray = NULL;
}

TNArrayLoader::TNArrayLoader(const TNArrayLoader& rhs)
{
}

TNArrayLoader& TNArrayLoader::operator=(const TNArrayLoader& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}

void TNArrayLoader::Init(TEventCollection *col) {
   TDirectory *save = gDirectory;
   TFile *fFile = TFile::Open(fFileName);
   if (!fFile) {
      SetStateError(TString::Format("file does not exist '%s'",fFileName.Data()));
      return;
   }
   fArray = dynamic_cast<TNArray*>(fFile->Get(fArrayName));
   if (!fArray) {
      SetStateError(TString::Format("array does not exist '%s'",fArrayName.Data()));
      return;
   }
   if (!fArray->Load()) {
      SetStateError("Error while loading TNArray");
      return;
   }
   
   col->AddInfo(fOutputName,fArray);
   save->cd();
}
