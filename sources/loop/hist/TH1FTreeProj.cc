/**
 * @file   TH1FTreeProj.cc
 * @brief  Tree projection
 *
 * @date   Created       : 2014-03-04 13:21:56 JST
 *         Last Modified : 2019-11-25 17:59:37 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 Shinsuke OTA
 */

#include "TH1FTreeProj.h"

using art::TH1FTreeProj;

ClassImp(TH1FTreeProj)

TH1FTreeProj::TH1FTreeProj()
: TAttTreeProj(this)
{
}

TH1FTreeProj::TH1FTreeProj(const char *name, const char *title, const char *cut)
   : TH1F(name,title,100,0.,100.), TAttTreeProj(this,cut)
{
}

TH1FTreeProj::~TH1FTreeProj()
{
}

TH1FTreeProj::TH1FTreeProj(const TH1FTreeProj& rhs)
{
}

TH1FTreeProj& TH1FTreeProj::operator=(const TH1FTreeProj& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}
void TH1FTreeProj::Copy(TObject& obj) const
{
   TH1F* h2 = dynamic_cast<TH1F*>(&obj);
   TH1F::Copy(*h2);
   TAttTreeProj* proj = dynamic_cast<TAttTreeProj*>(&obj);
   if (proj) {
      TAttTreeProj::Copy(*proj);
   }
}
