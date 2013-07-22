/* $Id:$ */
/**
 * @file   TCatCmdBn.cc
 * @date   Created : Feb 09, 2012 10:09:53 JST
 *   Last Modified : Jul 22, 2013 14:14:58 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TCatCmdBn.h"

#include <TCatHistManager.h>
#include <TH2.h>

TCatCmdBn::TCatCmdBn(EAxis axis)
   : fAxis(axis) {

}
TCatCmdBn::~TCatCmdBn()
{
}

Long_t TCatCmdBn::Cmd(vector<TString> args)
{
   Int_t id1,id2;
   Float_t start, end;
   id1 = id2 = -1;
   // check arguments
   if (args.size() == 4) {
      GetRange(args[1],id1,id2);
      start = args[2].Atof();
      end   = args[3].Atof();
   } else {
      Help();
      return 1;
   }
   if (id1 < 0) return 1;
   Int_t n = id2 - id1 + 1;
   for (Int_t i=0; i<n; i++) {
      TObject *obj = TCatHistManager::Instance()->GetObject(id1+i);
      if (!obj) continue;
      if (!obj->InheritsFrom(TH2::Class())) {
         // TArtCore::Info(GetName(),"%s is not 2D histogram",obj->GetName());
         continue;
      }
      Run((TH2*) obj, start, end);
   }
   return 1;
}

TH1* TCatCmdBn::Run(TH2* h2, Float_t start, Float_t end)
{
   TH1 *hnew = NULL;
   TString namesuffix, titlesuffix;
   TAxis *axis = NULL;
   Int_t fbin, ebin;
   switch (fAxis) {
   case kX:
      axis = h2->GetYaxis();
      fbin = axis->FindBin(start);
      if (axis->GetBinUpEdge(fbin) < end) {
         ebin = axis->FindBin(end);
      } else {
         ebin = fbin;
      }
      hnew = h2->ProjectionX("",fbin,ebin);
      namesuffix = "_bnx";
      titlesuffix = TString::Format(" (bnx %f %f)",start,end);
      break;
   case kY:
      axis = h2->GetXaxis();
      fbin = axis->FindBin(start);
      if (axis->GetBinUpEdge(fbin) < end) {
         ebin = axis->FindBin(end);
      } else {
         ebin = fbin;
      }
      printf("fbin = %d, ebin = %d\n",fbin,ebin);
      hnew = h2->ProjectionY("",fbin,ebin);
      namesuffix = "_bny";
      titlesuffix = TString::Format(" (bny %f %f)",start,end);
      break;
   case kZ:
      break;
   }
   if (hnew) {
      hnew->SetName(TString(h2->GetName()) + namesuffix);
      hnew->SetTitle(TString(h2->GetTitle()) + titlesuffix);
   } 
   return hnew;
}
