/* $Id:$ */
/**
 * @file   TCatCmdPr.cc
 * @date   Created : Feb 07, 2012 22:07:36 JST
 *   Last Modified : Jul 22, 2013 14:15:46 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TCatCmdPr.h"
#include <TCatHistManager.h>

#include <TH1.h>
#include <TH2.h>
#include <TH3.h>

TCatCmdPr::TCatCmdPr(const EAxis& axis)
   : fAxis(axis)
{
}
TCatCmdPr::~TCatCmdPr()
{
}

Long_t TCatCmdPr::Cmd(vector<TString> args)
{
   Int_t id1,id2;
   id1 = id2 = -1;
   if (args.size()>1) GetRange(args[1],id1,id2);
   return Run(id1,id2);
}

Long_t TCatCmdPr::Run(Int_t id1,Int_t id2)
{
   Int_t n = id2 - id1 + 1;
   if (id1 == -1 && id2 == -1) {
      TObject *obj = TCatHistManager::Instance()->GetCurrent();
      if (!obj) return 1;
      if (!obj->InheritsFrom(TH2::Class())) {
         // TArtCore::Info("TCatCmdPr::Run","%s is not 2D histogram",
         // obj->GetName());
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
         // TArtCore::Info("TCatCmdPr::Run","%s is not 2D histogram",
         //               obj->GetName());
         continue;
      } 
      Run((TH2*) obj);
   }
   return 1;
}

TH1* TCatCmdPr::Run(TH2 *h2)
{
   TH1 *hnew = NULL;
   TString namesuffix, titlesuffix;
   switch (fAxis) {
   case kX:
      hnew = h2->ProjectionX();
      namesuffix = "px";
      titlesuffix = " (prx)";
      break;
   case kY:
      hnew = h2->ProjectionY();
      namesuffix = "py";
      titlesuffix = " (pry)";
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
