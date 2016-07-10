/* $Id:$ */
/**
 * @file   TCatCmdPr.cc
 * @date   Created : Feb 07, 2012 22:07:36 JST
 *   Last Modified : 2016-01-18 14:15:31 JST (kawase)
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

TCatCmdPr::TCatCmdPr(const EAxis& axis, Bool_t isProjection)
   : fAxis(axis), fIsProjection(isProjection)
{
   const TString op = isProjection ? "projection" : "profile";
   const Char_t  rf = isProjection ? 'r' : 'f';
   fAxisLabel = (axis == kX) ? 'x' :
                (axis == kY) ? 'y' :
                'z';

   SetName(TString::Format("p%c%c",rf,fAxisLabel));
   SetTitle(TString::Format("%s to %c axis",op.Data(),fAxisLabel));
}

TCatCmdPr::~TCatCmdPr()
{
}

Long_t TCatCmdPr::Cmd(vector<TString> args)
{
   const TString range = args.size() > 1 ? args[1] : TCatCmd::kRangeDefault;
   const TString opt = args.size() > 2 ? args[2] : "";

   Int_t id1 = -1;
   Int_t id2 = -1;
   GetRange(range,id1,id2);
   Run(id1,id2,opt);

   return 1;
}

Long_t TCatCmdPr::Run(Int_t id1,Int_t id2, Option_t *opt)
{
   if (id1 < 0 || id1 > id2) return 1;
   ++id2;
   for (Int_t id=id1; id != id2; id++) {
      TObject *obj = TCatHistManager::Instance()->GetObject(id);
      if (!obj) continue;
      if (!obj->InheritsFrom(TH2::Class())) {
         // TArtCore::Info("TCatCmdPr::Run","%s is not 2D histogram",
         //               obj->GetName());
         continue;
      }
      Run((TH2*) obj, opt);
   }
   return 1;
}

TH1* TCatCmdPr::Run(TH2 *h2, Option_t *opt)
{
   TH1 *hnew = NULL;

   const TString &namesuffix =
      TString::Format("p%s%c",fIsProjection ? "" : "f", fAxisLabel);
   const TString &namearg =
      TString::Format("_%s",namesuffix.Data());
   const TString &titlesuffix =
      TString::Format(" (p%c%c)",fIsProjection ? 'r' : 'f', fAxisLabel);

   switch (fAxis) {
      case kX:
	 hnew = fIsProjection ? (TH1*)h2->ProjectionX(namearg,0,-1,opt) : (TH1*)h2->ProfileX(namearg,0,-1,opt);
	 break;
      case kY:
	 hnew = fIsProjection ? (TH1*)h2->ProjectionY(namearg,0,-1,opt) : (TH1*)h2->ProfileY(namearg,0,-1,opt);
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
