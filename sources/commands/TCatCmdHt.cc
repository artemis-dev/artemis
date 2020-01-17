/* $Id:$ */
/**
 * @file   TCatCmdHt.cc
 * @date   Created : Feb 06, 2012 11:06:16 JST
 *   Last Modified : 2020-01-18 01:41:16 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TCatCmdHt.h"
#include <TObjArray.h>
#include <TObjString.h>
#include <TCanvas.h>
#include <TCatHistManager.h>
#include <TCatPadManager.h>
#include "TRegexp.h"
#include "TH1.h"


TCatCmdHt::TCatCmdHt()
{
   SetName("ht");
   SetTitle("draw histogram");
}
TCatCmdHt::~TCatCmdHt()
{
}

TCatCmdHt* TCatCmdHt::Instance()
{
   static TCatCmdHt instance;
   return &instance;
}


namespace {
   void GetObjects(TString id, TObjArray *objects) {
//      printf("id = %s\n",id.Data());

      if (id.IsDigit()) {
         TObject *obj = TCatHistManager::Instance()->GetObject(id.Atoi());
         if (obj->InheritsFrom("TH1")) {
            objects->Add(obj);
         } else {
            printf("GetObjects: '%s' does not inherits from TH1\n",id.Data());
         }
         return;
      } else if (id.Contains("/")) {
         Int_t idx = id.Index("/");
         TString dirname = id(0,idx);
         id.Remove(0,idx+1);
         if (dirname.IsDigit()) {
            TObject *obj = gDirectory->GetList()->At(dirname.Atoi());
            if (!obj || !obj->InheritsFrom(TDirectory::Class())) return;
            TDirectory *saved = gDirectory;
            static_cast<TDirectory*>(obj)->cd();
            TCatHistManager::Instance()->Load();
            GetObjects(id,objects);
            saved->cd();
            
            return;
            
         }
//         printf("id new = %s in direname %s\n",id.Data(),dirname.Data());
         
         TDirectory *saved = gDirectory;
         TRegexp dirnameexp(dirname.ReplaceAll("*",".*"));
         TCatHistManager::Instance()->Load();
         TIter next(gDirectory->GetList());
         while(TObject *obj = next()) {
            if (!obj->InheritsFrom(TDirectory::Class())) {
//               printf("%s is not a directory\n",obj->GetName());
               continue;               
            }
            TString name = obj->GetName();
//            printf("%s\n",name.Data());
            if (dirname.Contains("*") && name.Contains(dirnameexp)) {
               static_cast<TDirectory*>(obj)->cd();
               TCatHistManager::Instance()->Load();
               GetObjects(id,objects);
            } else if (!dirname.CompareTo(name)) {
               static_cast<TDirectory*>(obj)->cd();
               TCatHistManager::Instance()->Load();
               GetObjects(id,objects);
            }
         }
         saved->cd();
      } else if (id.Contains(":")) {
         Int_t id1, id2;
         TObjArray *array = id.Tokenize(":");
         id1 = ((TObjString*)array->At(0))->GetString().Atoi();
         id2 = ((TObjString*)array->At(1))->GetString().Atoi();
//         printf("%d %d\n",id1, id2);
         for (Int_t i = id1; i < id2+1; ++i) {
            GetObjects(Form("%d",i),objects);
         }
      } else  {
         // objects to be drawn
         if (id.Contains("*")) {
            TRegexp objname(id.ReplaceAll("*",".*"));
            TIter next(gDirectory->GetList());
            while (TObject *obj = next()) {
               if (!obj->InheritsFrom(TH1::Class())) continue;
               TString name = obj->GetName();
               if (name.Contains(objname)) {
                  objects->Add(obj);
               }
            }
         } else {
            TObject *obj = gDirectory->Get(id);
            if (obj) objects->Add(obj);
            
         }
      }
   }
};


Long_t TCatCmdHt::Cmd(vector<TString> tokens)
{
   Int_t id1 = -1;
   Int_t id2 = -1;
   TString opt("");
   Int_t n = tokens.size();
   TObject *obj = NULL;
   if (n==1) {
      TCatHistManager::Instance()->DrawCurrent();
      return 1;
   }
#if 1

   std::vector<TString> ids;
   TObjArray *drawObjects = new TObjArray;

   // analyze comma separator
   TObjArray *array = tokens[1].Tokenize(",");
   for (Int_t i = 0; i < array->GetEntries(); ++i) {
      TString id = ((TObjString*)array->At(i))->GetString();
//      printf("id = %s\n",id.Data());
      GetObjects(id,drawObjects);
    }

//   drawObjects->ls();
   
   for (Int_t i = 0; i < drawObjects->GetEntriesFast(); ++i) {
      TCatPadManager::Instance()->Next();
      drawObjects->At(i)->Draw(tokens.size() > 2 ? tokens[2] : "");
   }

   if (array->GetEntriesFast() == 1 &&
       tokens[1].IsDigit()) {
      TCatHistManager::Instance()->SetId(tokens[1].Atoi());
   }
   return 1;
   
#else 
   for (Int_t i=1; i<n; i++) {
      TString &token = tokens[i];
      switch (i) {
      case 1: // should be id or id1:id2
         GetRange(token,id1,id2);
         if (id1 == -1) {
            obj = TCatHistManager::Instance()->GetObject(token);
         }
         break;
      case 2: // should be draw option
         opt = token;
         break;
      default:
         // do nothing
         break;
      }
   }
   if (id1 == -1) {
      if (!TCatPadManager::Instance()->Next()) {
         // TArtCore::Info("TCatCmdHt","Not enough pads to draw");
         return 1;
      } else if (obj) {
         obj->Draw(opt);
      }
      return 1;
   }
   return Run(id1,id2,opt);
#endif   
}

Long_t TCatCmdHt::Run(Int_t id1, Int_t id2, TString& opt) 
{
   if (id1 > id2) {
      Int_t tmp = id1;
      id1 = id2;
      id2 = tmp;
   }
   Int_t n = id2-id1+1;
   for (Int_t i=0; i<n; i++) {
      if (id1+i < 0) return 1;
      if (!TCatPadManager::Instance()->Next()) {
         // TArtCore::Info("TCatCmdHt","Not enough pads to draw");
         return 1;
      }
      TCatHistManager::Instance()->DrawObject(id1+i,opt);
   }
   return 1;
}

#if 0
void TCatCmdHt::Help() {
   TString help = "";

   Info("Help","NAME");
   Info("Help"," ");
   Info("Help","     ht -- draws histograms");
   Info("Help","SYNOPSIS");
   Info("Help","     ht ids");
   Info("Help"," ");
   Info("Help","DESCRIPTION");
   Info("Help","     The command 'ht' draws histograms in artcanvas. ");
   Info("Help","     An argment ids are mandatory.");
   Info("Help","     Ids can cointain, colon (:), comma (,), and wild card (*), but not space");
   Info("Help","     Precedence of separators is comma, wild card and then colon.");
   Info("Help"," ");
}

#endif
