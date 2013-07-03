/* $Id:$ */
/**
 * @file   TCatCmdLg.cc
 * @date   Created : Jun 20, 2013 04:20:43 JST
 *   Last Modified : 
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#include "TCatCmdLg.h"

#include <TString.h>
#include <TMethodCall.h>
#include "TCatPadManager.h"

TCatCmdLg::TCatCmdLg(const EAxis axis, Bool_t value)
   : fAxis(axis), fValue(value)
{
   TString cmd = fValue ? "lg" : "ln";
   char caxis = 'x' + fAxis;
   cmd += caxis;
   SetName(cmd);
   SetTitle(TString::Format("Set %s scale for %c",(fValue?"Log":"Linear"),'X'+fAxis));
   fMethod = new TMethodCall(TVirtualPad::Class(),TString::Format("SetLog%c",'x'+fAxis),TString::Format("%d",fValue));
}
TCatCmdLg::~TCatCmdLg()
{
   delete fMethod;
}

Long_t TCatCmdLg::Cmd(vector<TString> tokens)
{
   Int_t padid1 = -1;
   Int_t padid2 = -1;
   Int_t n = tokens.size();
   for (Int_t i=1; i!=n; i++) {
      TString &token = tokens[i];
      switch(i) {
      case 1:
         GetRange(token,padid1,padid2);
         break;
      default:
         // nothing;
         break;
      }
   }
   return Run(padid1,padid2);
}

Long_t TCatCmdLg::Run(Int_t padid1, Int_t padid2)
{
   if (padid1 > padid2) {
      Int_t tmp = padid1;
      padid1 = padid2;
      padid2 = tmp;
   }
   if (padid1 == -1) {
      fMethod->Execute(TCatPadManager::Instance()->Current());
      return 1;
   } else if (padid2 == -1) {
      fMethod->Execute(TCatPadManager::Instance()->Get(padid1));
   } else {
      Int_t n = padid2 - padid1 + 1;
      for (Int_t i = 0; i != n; i++) {
         if (padid1+1 < 0) return 1;
         if (TCatPadManager::Instance()->Get(padid1+i)) {
            fMethod->Execute(TCatPadManager::Instance()->Get(padid1 + i));
         }
      }
   }
   return 1;
}



