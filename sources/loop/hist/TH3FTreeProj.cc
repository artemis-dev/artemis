/**
 * @file   TH3FTreeProj.cc
 * @brief  Tree projection 3-D histogram
 *
 * @date   Created       : 2014-03-07 14:29:56 JST
 *         Last Modified : 2019-11-25 18:00:43 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 Shinsuke OTA
 */

#include "TH3FTreeProj.h"

using art::TH3FTreeProj;

ClassImp(TH3FTreeProj)

TH3FTreeProj::TH3FTreeProj()
{
}

TH3FTreeProj::TH3FTreeProj(const char *name, const char *title, const char *cut)
   : TH3F(name,title,100,0.,100.,100,0.,100.,100,0.,100), TAttTreeProj(this,cut)
{
}

TH3FTreeProj::~TH3FTreeProj()
{
}

TH3FTreeProj::TH3FTreeProj(const TH3FTreeProj& rhs)
{
}

TH3FTreeProj& TH3FTreeProj::operator=(const TH3FTreeProj& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}


void TH3FTreeProj::Copy(TObject& obj) const
{
   TH3F* h2 = dynamic_cast<TH3F*>(&obj);
   TH3F::Copy(*h2);
   TAttTreeProj* proj = dynamic_cast<TAttTreeProj*>(&obj);
   if (proj) {
      TAttTreeProj::Copy(*proj);
   }
}
