/* $Id:$ */
/**
 * @file   TCatCmdAv.cc
 * @date   Created : Feb 07, 2012 22:07:36 JST
 *   Last Modified : Jan 08, 2014 16:02:39 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TCatCmdAv.h"
#include <TCatHistManager.h>

#include <TROOT.h>
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <TProfile.h>

TCatCmdAv::TCatCmdAv(const EAxis& axis)
   : fAxis(axis)
{
}
TCatCmdAv::~TCatCmdAv()
{
}

Long_t TCatCmdAv::Cmd(vector<TString> args)
{
   Int_t id1,id2;
   id1 = id2 = -1;
   if (args.size()>1) GetRange(args[1],id1,id2);
   return Run(id1,id2);
}

Long_t TCatCmdAv::Run(Int_t id1,Int_t id2)
{
   Int_t n = id2 - id1 + 1;
   if (id1 == -1 && id2 == -1) {
      TObject *obj = TCatHistManager::Instance()->GetCurrent();
      if (!obj) return 1;
      if (!obj->InheritsFrom(TH2::Class())) {
//         TArtCore::Info("TCatCmdAv::Run","%s is not 2D histogram",
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
//         TArtCore::Info("TCatCmdAv::Run","%s is not 2D histogram",
//                        obj->GetName());
         continue;
      } 
      Run((TH2*) obj);
   }
   return 1;
}

void* TCatCmdAv::Run(TH2 *h2)
{
  //TH1 *hnew = NULL;
   TString namesuffix1,namesuffix2,namesuffix3,namesuffix4;
   switch (fAxis) {
   case kX:
     h2->FitSlicesY();
     namesuffix1 = "avx";
     namesuffix2 = "constant_x";
     namesuffix3 = "sigma_x";
     namesuffix4 = "chi2_x";
     
     ((TH1D*)gROOT->FindObject(TString::Format("%s_1",h2->GetName())))
       ->SetName(TString(h2->GetName()) + namesuffix1);
     ((TH1D*)gROOT->FindObject(TString::Format("%s_0",h2->GetName())))
       ->SetName(TString(h2->GetName()) + namesuffix2);
     ((TH1D*)gROOT->FindObject(TString::Format("%s_2",h2->GetName())))
       ->SetName(TString(h2->GetName()) + namesuffix3);
     ((TH1D*)gROOT->FindObject(TString::Format("%s_chi2",h2->GetName())))
       ->SetName(TString(h2->GetName()) + namesuffix4);
     break;
   case kY:
     h2->FitSlicesY();
     namesuffix1 = "avy";
     namesuffix2 = "constant_y";
     namesuffix3 = "sigma_y";
     namesuffix4 = "chi2_y";
     //titlesuffix1 = "(avx)";
     ((TH1D*)gROOT->FindObject(TString::Format("%s_1",h2->GetName())))
       ->SetName(TString(h2->GetName()) + namesuffix1);
     ((TH1D*)gROOT->FindObject(TString::Format("%s_0",h2->GetName())))
       ->SetName(TString(h2->GetName()) + namesuffix2);
     ((TH1D*)gROOT->FindObject(TString::Format("%s_2",h2->GetName())))
       ->SetName(TString(h2->GetName()) + namesuffix3);
     ((TH1D*)gROOT->FindObject(TString::Format("%s_chi2",h2->GetName())))
       ->SetName(TString(h2->GetName()) + namesuffix4);
     break;
   default:
      break;
   }
   /*if (hnew) {
      hnew->SetName(TString(h2->GetName()) + namesuffix);
      hnew->SetTitle(TString(h2->GetTitle()) + titlesuffix);
      }*/
   //return hnew;
}
