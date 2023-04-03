/**
 * @file   TCmdFiga.cc
 * @brief  fit with gauss
 *
 * @date   Created       : 2014-06-25 05:35:40 JST
 *         Last Modified : Jun 28, 2014 19:40:11 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 Shinsuke OTA
 */

#include "TCmdFiga.h"

#include <TCatHistManager.h>
#include <TH1.h>
#include <TF1.h>
#include <TROOT.h>
#include <TPad.h>
using art::TCmdFiga;

ClassImp(TCmdFiga)

TCmdFiga::TCmdFiga()
{
   SetName("figa");
   SetTitle("fit gauss to histogram");
}

TCmdFiga::~TCmdFiga()
{
}

TCmdFiga::TCmdFiga(const TCmdFiga& rhs)
{
}

TCmdFiga& TCmdFiga::operator=(const TCmdFiga& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}

Long_t TCmdFiga::Cmd(vector<TString> tokens)
{
   Int_t n = tokens.size() - 1;
   Int_t id1 = -1;
   Int_t id2 = -1;
   Double_t c, m, s;
   if (n == 0) {
      // no option
      TIter next(gROOT->GetSelectedPad()->GetListOfPrimitives());
      while (TObject *obj = next()) {
         if (obj->InheritsFrom("TH1") && ((TH1*)obj)->GetDimension() == 1) {
            Run((TH1*)obj,&c,&m,&s);
            return 1;
         }

      }
         //id1 = id2 = TCatHistManager::Instance()->GetId();
   } else {
      GetRange(tokens[1],id1,id2);
   }
   id2++;
   for (Int_t id = id1; id != id2; id++) {
      TH1* h = dynamic_cast<TH1*>(TCatHistManager::Instance()->GetObject(id));
      if (!h) {
         printf("no such histogram with id = %d\n",id);
         return 1;
      }
      Run(h,&c,&m,&s);
   }
   return 1;
}
Long_t TCmdFiga::Run(TH1*h, Double_t *c, Double_t *m, Double_t *s)
{
   h->Fit("gaus");
   *c = h->GetFunction("gaus")->GetParameter(0);
   *m = h->GetFunction("gaus")->GetParameter(1);
   *s = h->GetFunction("gaus")->GetParameter(2);
   return 1;
}

void TCmdFiga::Help()
{
   printf("figa [hid or hname]");
}
