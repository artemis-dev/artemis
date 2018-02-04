/**
 * @file   TCmdHdel.cc
 * @brief  delete objects in the current directory
 *
 * @date   Created       : 2015-05-06 10:45:24 JST
 *         Last Modified : May 06, 2015 10:55:25 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2015 Shinsuke OTA
 */

#include "TCmdHdel.h"
#include <TCatHistManager.h>
#include <TH1.h>
#include <TObjArray.h>

using art::TCmdHdel;

ClassImp(TCmdHdel)

TCmdHdel::TCmdHdel()
{
   SetName("hdel");
   SetTitle("delete objects in the current directory");
}

TCmdHdel::~TCmdHdel()
{
}

TCmdHdel::TCmdHdel(const TCmdHdel& rhs)
{
}

TCmdHdel& TCmdHdel::operator=(const TCmdHdel& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}

Long_t TCmdHdel::Cmd(vector<TString> tokens)
{
   Int_t id1 = -1;
   Int_t id2 = -1;
   TString opt("");
   Int_t n = tokens.size();
   for (Int_t i=1; i<n; i++) {
      TString &token = tokens[i];
      switch (i) {
      case 1: // should be id or id1:id2
         GetRange(token,id1,id2);
         break;
      default:
         // do nothing
         break;
      }
   }
   return Run(id1,id2);
}

Long_t TCmdHdel::Run(Int_t id1, Int_t id2)
{
   if (id1 > id2) {
      Int_t tmp = id1;
      id1 = id2;
      id2 = tmp;
   }
   Int_t n = id2-id1+1;
   TObjArray arr;
   for (Int_t i=0; i<n; i++) {
      if (id1+i < 0) return 1;
      TObject *obj = TCatHistManager::Instance()->GetObject(id1+i);
      if (obj->InheritsFrom(TH1::Class())) {
         arr.Add(obj);
      }
   }
   while (TObject *obj = arr.Last()) {
      arr.Remove(obj);
      obj->Delete();
   }
      

   return 1;
}
