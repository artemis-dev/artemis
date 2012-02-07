/* $Id:$ */
/**
 * @file   TCatHistManager.cc
 * @date   Created : Feb 06, 2012 11:06:39 JST
 *   Last Modified : Feb 06, 2012 23:15:34 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TCatHistManager.h"
#include <TKey.h>
#include <TClass.h>
#include <TArtCore.h>

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
   if (!fObjs->GetEntries()) {
      Int_t n = fKeys->GetEntries();
      for (Int_t i=0; i<n; i++) {
         TKey *key = (TKey*) fKeys->At(i);
         if (fObjs->FindObject(key->GetName())) continue;
         if (TClass::GetClass(key->GetClassName())->InheritsFrom("TH1")) {
            key->ReadObj();
         }
      }
   }
}

void TCatHistManager::DrawObject(const Int_t &id, const Option_t *opt)
{
   if (!fObjs) Load();
   if (!fObjs) return;
   if (fObjs->At(id)) {
      fCurrentID = id;
      fObjs->At(id)->Draw(opt);
   }
}

void TCatHistManager::DrawCurrent(const Option_t *opt)
{
   if (!fObjs) Load();
   if (!fObjs) return;
   if (!fObjs->At(fCurrentID)) fCurrentID = 0;
   DrawObject(fCurrentID,opt);
}

void TCatHistManager::DrawNext(const Option_t *opt)
{
   if (!fObjs) Load();
   if (fObjs->At(fCurrentID+1)) {
      fCurrentID++;
   } else {
      TArtCore::Info("DrawNext","No more objects larger than oid = %d\n",fCurrentID);
   }
   DrawObject(fCurrentID,opt);
}

void TCatHistManager::DrawPrev(const Option_t *opt)
{
   if (!fObjs) Load();
   if (!fObjs) return;
   if (fObjs->At(fCurrentID-1)) {
      fCurrentID--;
   } else {
      TArtCore::Info("DrawNext","No more objects smaller than  oid = %d\n",fCurrentID);
   }
   DrawObject(fCurrentID,opt);
}

void TCatHistManager::ls(Option_t *) const 
{
   if (!fObjs) return;
   Int_t n = fObjs->GetEntries();
   printf("\n");
   for (Int_t i=0; i<n; i++) {
      TObject *obj = fObjs->At(i);
      printf("%5d %4s %10s %10s\n",i,
             obj->IsA()->GetName(),
             obj->GetName(),
             obj->GetTitle());
   }
   printf("\n");
}
