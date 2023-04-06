/* $Id:$ */
/**
 * @file   TCatCmdMn.cc
 * @date   Created : Feb 07, 2012 22:07:36 JST
 *   Last Modified : Jan 08, 2014 16:02:28 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TCatCmdMn.h"
#include <TCatHistManager.h>

#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <TProfile.h>

TCatCmdMn::TCatCmdMn(const EAxis& axis)
   : fAxis(axis)
{
}
TCatCmdMn::~TCatCmdMn()
{
}

Long_t TCatCmdMn::Cmd(vector<TString> args)
{
   Int_t id1,id2;
   id1 = id2 = -1;
   if (args.size()>1) GetRange(args[1],id1,id2);
   return Run(id1,id2);
}

Long_t TCatCmdMn::Run(Int_t id1,Int_t id2)
{
   Int_t n = id2 - id1 + 1;
   if (id1 == -1 && id2 == -1) {
      TObject *obj = TCatHistManager::Instance()->GetCurrent();
      if (!obj) return 1;
      if (!obj->InheritsFrom(TH2::Class())) {
//         TArtCore::Info("TCatCmdMn::Run","%s is not 2D histogram",
//                        obj->GetName());
         return 1;
      }
      Run((TH2*)obj);
      return 1;
   }
   if (id1 < 0) return 1;
   
   for (Int_t i=0; i<n; i++) {
      TObject *obj = TCatHistManager::Instance()->GetObject(id1+i);
      if (!obj) continue;
      if (!obj->InheritsFrom(TH2::Class())) {
//         TArtCore::Info("TCatCmdMn::Run","%s is not 2D histogram",
//                        obj->GetName());
         continue;
      } 
      Run((TH2*) obj);
   }
   return 1;
}

TProfile* TCatCmdMn::Run(TH2 *h2)
{
   TProfile *hnew = NULL;
   TString namesuffix, titlesuffix;
   switch (fAxis) {
   case kX:
      hnew = h2->ProfileX();
      namesuffix = "mx";
      titlesuffix = " (mnx)";
      break;
   case kY:
      hnew = h2->ProfileY();
      namesuffix = "my";
      titlesuffix = " (mny)";
      break;
   default:
      break;
   }
   if (hnew) {
      hnew->SetName(TString(h2->GetName()) + namesuffix);
      hnew->SetTitle(TString(h2->GetTitle()) + titlesuffix);
   }
   return hnew;
}
