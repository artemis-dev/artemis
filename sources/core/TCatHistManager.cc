/* $Id:$ */
/**
 * @file   TCatHistManager.cc
 * @date   Created : Feb 06, 2012 11:06:39 JST
 *   Last Modified : 2018-08-07 17:13:09 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TCatHistManager.h"
#include <TKey.h>
#include <TClass.h>
#include <TROOT.h>

ClassImp(TCatHistManager);

TCatHistManager::TCatHistManager()
{
}
TCatHistManager::~TCatHistManager()
{
}

TCatHistManager* TCatHistManager::Instance() 
{
   static TCatHistManager instance;
   instance.Load();
   return &instance;
}

void TCatHistManager::Load() 
{
   fObjs = gDirectory->GetList();
   fKeys = gDirectory->GetListOfKeys();
   if (!fObjs || !fKeys) return;
   Int_t n = fKeys->GetEntries();
   for (Int_t i=0; i<n; i++) {
      TKey *key = (TKey*) fKeys->At(i);
      if (fObjs->FindObject(key->GetName())) continue;
      if (TClass::GetClass(key->GetClassName())->InheritsFrom("TH1") |
          TClass::GetClass(key->GetClassName())->InheritsFrom("TDirectory")
         ) {
         key->ReadObj();
      }
   }
}

void TCatHistManager::DrawObject(const Int_t &id, const Option_t *opt)
{
   if (TObject *obj = GetObject(id)) {
      fCurrentID = id;
      obj->Draw(opt);
   }
}

void TCatHistManager::DrawCurrent(const Option_t *opt)
{
   if (!GetCurrent()) fCurrentID = 0;
   DrawObject(fCurrentID,opt);
}

void TCatHistManager::DrawNext(const Option_t *opt)
{
   TObject *obj = GetNext();
   if (obj) obj->Draw(opt);
}

void TCatHistManager::DrawPrev(const Option_t *opt)
{
   TObject *obj = GetPrev();
   if (obj) obj->Draw(opt);
}

void TCatHistManager::ls(Option_t *opt) const
{
   if (!fObjs) return;
   Int_t n = fObjs->GetEntries();
   printf("\n");
   printf(" %s\n",gDirectory->GetName());
   for (Int_t i=0; i<n; i++) {
      TObject *obj = fObjs->At(i);
      printf("%c%4d %4s %-15s %-10s\n",
	     i == fCurrentID ? '>' : ' ',
	     i,
             obj->IsA()->GetName(),
             obj->GetName(),
             obj->GetTitle());
   }

   printf("\n");
}

Bool_t TCatHistManager::cd(TString opt) const
{
   if (opt == "") {
      return gROOT->cd();
   } else if (opt == "..") {
      return gDirectory->cd("..");
   } else if (opt.IsDigit()) {
      Int_t id = opt.Atoi();
      return cd(id);
   } else {
      return gDirectory->cd(opt);
   }
}

Bool_t TCatHistManager::cd(const Int_t& id) const
{
   if (!fObjs) return kFALSE;
   if (fObjs->At(id) && 
       fObjs->At(id)->InheritsFrom("TDirectory")) {
      return ((TDirectory*)fObjs->At(id))->cd();
   }
   return kFALSE;
}

TObject* TCatHistManager::GetObject(const char* name)
{
   if (!fObjs) Load();
//   printf("id = %d at %p (size = %d)\n",id,fObjs->At(id),fObjs->GetEntries());
   return fObjs->FindObject(name);
}
TObject* TCatHistManager::GetObject(const Int_t &id)
{
   if (!fObjs) Load();
//   printf("id = %d at %p (size = %d)\n",id,fObjs->At(id),fObjs->GetEntries());
   return (id < 0) ? NULL : fObjs->At(id);
}
TObject* TCatHistManager::GetCurrent()
{
   return GetObject(fCurrentID);
}
TObject* TCatHistManager::GetNext()
{
   TObject *obj = NULL;
   if ((obj = GetObject(fCurrentID+1))) {
      fCurrentID++;
   } 
   return obj;
}
TObject* TCatHistManager::GetPrev()
{
   TObject *obj = NULL;
   if ((obj = GetObject(fCurrentID-1))) {
      fCurrentID--;
   } 
   return obj;
}

void TCatHistManager::SetId(const Int_t &id)
{
   if (GetObject(id)) {
      fCurrentID = id;
   } 
}
