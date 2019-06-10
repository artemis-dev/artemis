/**
 * @file   TObjectLoader.cc
 * @brief  load object from root file
 *
 * @date   Created       : 2018-06-27 18:46:58 JST
 *         Last Modified : 2018-07-30 08:48:38 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2018 Shinsuke OTA
 */

#include "TObjectLoader.h"
#include <TFile.h>
#include <TKey.h>
#include <TDirectory.h>
using art::TObjectLoader;

ClassImp(TObjectLoader)

TObjectLoader::TObjectLoader()
{
   RegisterProcessorParameter("ObjectLists","pairs of input file and object to be loaded. all objects are loaded if object name is * (\"*\")",
                              fObjectLists,StringVec_t(0));
}

TObjectLoader::~TObjectLoader()
{
}

TObjectLoader::TObjectLoader(const TObjectLoader& rhs)
{
}

TObjectLoader& TObjectLoader::operator=(const TObjectLoader& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}

void TObjectLoader::Init(TEventCollection *col)
{
   const Int_t kRequiredNumTokens = 2;
   Int_t nTokens = fObjectLists.size();
   if (nTokens % kRequiredNumTokens) {
      SetStateError(TString::Format("The number of components in ObjectLists should be muliple of %d",kRequiredNumTokens));
      return;
   }
   TDirectory *dirSaved = gDirectory;
   Info("Init","nTokens = %d",nTokens);
   
   for (Int_t i = 0; i < nTokens; i += kRequiredNumTokens) {
      TString filename = fObjectLists[i];
      TString objname =  fObjectLists[i+1];
      TFile *file = TFile::Open(filename);
      if (!file) {
         SetStateError(TString::Format("Cannot open file '%s'",filename.Data()));
         return;
      }
      if (objname.EqualTo('*')) {
         // load all file
         TList *fKeys = gDirectory->GetListOfKeys();
         Int_t n = fKeys->GetEntries();
         if (!n) {
            SetStateError(TString::Format("No object is found in '%s'",filename.Data()));
            return;
         }
         for (Int_t i=0; i<n; i++) {
            TKey *key = (TKey*) fKeys->At(i);
            TObject *obj = key->ReadObj();
            Info("Init","Object: %s",obj->GetName());
            col->AddInfo(obj->GetName(),obj,fOutputIsTransparent);
         }
      } else {
         TObject *obj = file->Get(objname);
         if (!obj) {
            SetStateError(TString::Format("Cannot find object '%s'",objname.Data()));
            return;
         }
         col->AddInfo(obj->GetName(),obj,fOutputIsTransparent);
      }
            
   }
   dirSaved->cd();
}
