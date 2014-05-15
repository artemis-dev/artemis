/**
 * @file   TH2FTreeProj.cc
 * @brief  Tree projection 2-D historgram
 *
 * @date   Created       : 2014-03-07 08:35:22 JST
 *         Last Modified : Mar 07, 2014 08:38:48 JST
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
