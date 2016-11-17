/**
 * @file   TCmdFipo.cc
 * @brief  fit with pol functions
 *
 * @date   Created       : 2016-01-15 15:10:40 JST
 *         Last Modified : 2016-01-18 12:13:20 JST (kawase)
 * @author KAWASE Shoichiro <kawase@aees.kyushu-u.ac.jp>
 *
 *    (C) 2016 KAWASE Shoichiro
 */

#include "TCmdFipo.h"

#include <TCatHistManager.h>

#include <TROOT.h>
#include <TPad.h>
#include <TH1.h>
#include <TF1.h>
#include <TCollection.h>

using art::TCmdFipo;

ClassImp(TCmdFipo)

TCmdFipo::TCmdFipo()
{
   SetName("fipo");
   SetTitle("fit pol functions to histogram");
}

TCmdFipo::~TCmdFipo()
{
}

Long_t TCmdFipo::Cmd(vector<TString> tokens)
{
   const Int_t n = tokens.size() - 1;

   const TString range = n > 0 ? tokens[1] : TCatCmd::kRangeDefault;
   const Int_t d = n > 1 ? tokens[2].Atoi() : 0;
   const UInt_t degree = d > 0 ? d : 1;

   Int_t id1, id2;
   id1 = id2 = -1;
   GetRange(range,id1,id2);

   id2++;
   for (Int_t id = id1; id != id2; ++id) {
      TH1* h = dynamic_cast<TH1*>(TCatHistManager::Instance()->GetObject(id));
      if (!h) {
         printf("no such histogram with id = %d\n",id);
         return 1;
      }
      Run(h,degree);
   }
   return 1;
}

Long_t TCmdFipo::Run(TH1 *h, UInt_t degree, Double_t *params) const
{
   const TString &func = TString::Format("pol%d",degree);
   h->Fit(func);

   if (!params) {
      return 1;
   }

   if (TF1 *const f = h->GetFunction(func)) {
      f->GetParameters(params);
   }

   return 1;
}

void TCmdFipo::Help()
{
   printf("fipo [hid or hname] [degree of polN]");
}
