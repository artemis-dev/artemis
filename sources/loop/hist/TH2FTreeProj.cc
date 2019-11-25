/**
 * @file   TH2FTreeProj.cc
 * @brief  Tree projection 2-D historgram
 *
 * @date   Created       : 2014-03-07 08:35:22 JST
 *         Last Modified : 2019-11-25 17:53:35 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 Shinsuke OTA
 */

#include "TH2FTreeProj.h"

using art::TH2FTreeProj;

ClassImp(TH2FTreeProj)

TH2FTreeProj::TH2FTreeProj()
: TAttTreeProj(this)
{
}

TH2FTreeProj::TH2FTreeProj(const char *name, const char *title, const char *cut)
   : TH2F(name,title,100,0.,100.,100,0.,100.), TAttTreeProj(this,cut)
{
}

TH2FTreeProj::~TH2FTreeProj()
{
}

TH2FTreeProj::TH2FTreeProj(const TH2FTreeProj& rhs)
{
}

TH2FTreeProj& TH2FTreeProj::operator=(const TH2FTreeProj& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}


void TH2FTreeProj::Copy(TObject& obj) const
{
   TH2F* h2 = dynamic_cast<TH2F*>(&obj);
   TH2F::Copy(*h2);
   TAttTreeProj* proj = dynamic_cast<TAttTreeProj*>(&obj);
   if (proj) {
      TAttTreeProj::Copy(*proj);
   }
}
