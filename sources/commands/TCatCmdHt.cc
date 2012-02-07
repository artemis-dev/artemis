/* $Id:$ */
/**
 * @file   TCatCmdHt.cc
 * @date   Created : Feb 06, 2012 11:06:16 JST
 *   Last Modified : Feb 06, 2012 19:44:19 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TCatCmdHt.h"
#include <TObjArray.h>
#include <TObjString.h>
#include <TArtCore.h>
#include <TCanvas.h>
#include <TCatHistManager.h>
#include <TCatPadManager.h>

ClassImp(TCatCmdHt);

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

Long_t TCatCmdHt::Cmd(vector<TString> tokens)
{
   Int_t id1 = -1;
   Int_t id2 = -1;
   TString opt("");
   Int_t n = tokens.size();
   if (n==1) {
      TCatHistManager::Instance()->DrawCurrent();
      return 1;
   }
   for (Int_t i=1; i<n; i++) {
      TString &token = tokens[i];
      TObjArray *ids = token.Tokenize(":");
      switch (i) {
      case 1: // should be id or id1:id2
         if (ids->GetEntries()==1) {
            id1 = id2 = token.Atoi();
         } else {
            id1 = ((TObjString*)ids->At(0))->GetString().Atoi();
            id2 = ((TObjString*)ids->At(1))->GetString().Atoi();
         }
         break;
      case 2: // should be draw option
         opt = token;
         break;
      default:
         // do nothing
         break;
      }
      delete ids;
   }
   return Run(id1,id2,opt);
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
      if (!TCatPadManager::Instance()->Next()) {
         TArtCore::Info("TCatCmdHt","Not enough pads to draw");
         return 1;
      }
      TCatHistManager::Instance()->DrawObject(id1+i,opt);
   }
   return 1;
}
