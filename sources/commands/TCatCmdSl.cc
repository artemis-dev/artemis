/* $Id:$ */
/**
 * @file   TCatCmdSl.cc
 * @date   Created : Feb 09, 2012 23:09:34 JST
 *   Last Modified : Jul 22, 2013 14:16:02 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TCatCmdSl.h"

#include <TCatHistManager.h>
#include <TDirectory.h>
#include <TH2.h>

TCatCmdSl::TCatCmdSl(const EAxis& axis)
   : fAxis(axis)
{
}
TCatCmdSl::~TCatCmdSl()
{
}

Long_t TCatCmdSl::Cmd(vector<TString> args)
{
   Int_t id1, id2;
   id1 = id2 = -1;
   // check arguments
   if (args.size() == 1) {
      id1 = id2 = TCatHistManager::Instance()->GetId();
   } else if (args.size() == 2) {
      GetRange(args[1],id1,id2);
   } else {
      Help();
      return 1;
   }
   if (id1 < 0) return 1;
   Int_t n = id2-id1+1;
   for (Int_t i=0; i<n; i++) {
      TObject *obj = TCatHistManager::Instance()->GetObject(id1+i);
      if (!obj) continue;
      if (!obj->InheritsFrom(TH2::Class())) {
         // TArtCore::Info(GetName(),"%s is not 2D histogram",obj->GetName());
         continue;
      }
      Run((TH2*)obj);

   }
   return 1;
}

TH1* TCatCmdSl::Run(TH2* h2)
{
   TH1 *hnew = NULL;
   TString namesuffix, titlesuffix;
   TAxis *axis = NULL;
   Int_t ibin,nbins;
   TDirectory* dir;
   switch (fAxis) {
   case kX:
      axis = h2->GetYaxis();
      nbins = axis->GetNbins();
      dir = gDirectory->mkdir(
         TString::Format("%s_slx",h2->GetName()));
      for (ibin = 1; ibin <= nbins; ibin++) {
         hnew = h2->ProjectionX("",ibin,ibin);
         hnew->SetName(TString::Format("%s_slx%d",(h2->GetName()),ibin));
         hnew->SetTitle(TString::Format("%s (slx%d)",
                                        (h2->GetTitle()),ibin));
         hnew->SetDirectory(dir);
      }
      break;
   case kY:
      axis = h2->GetXaxis();
      nbins = axis->GetNbins();
      dir = gDirectory->mkdir(
         TString::Format("%s_sly",h2->GetName()));
      for (ibin = 1; ibin <= nbins; ibin++) {
         hnew = h2->ProjectionY("",ibin,ibin);
         hnew->SetName(TString::Format("%s_sly%d",(h2->GetName()),ibin));
         hnew->SetTitle(TString::Format("%s (sly%d)",
                                        (h2->GetTitle()),ibin));
         hnew->SetDirectory(dir);
      }
      break;
   }      
   return NULL;
}
