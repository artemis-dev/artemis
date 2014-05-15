/**
 * @file   TH1FTreeProj.cc
 * @brief  Tree projection
 *
 * @date   Created       : 2014-03-04 13:21:56 JST
 *         Last Modified : Mar 07, 2014 09:02:54 JST
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
