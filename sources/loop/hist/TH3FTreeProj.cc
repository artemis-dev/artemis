/**
 * @file   TH3FTreeProj.cc
 * @brief  Tree projection 3-D histogram
 *
 * @date   Created       : 2014-03-07 14:29:56 JST
 *         Last Modified : Mar 07, 2014 14:32:51 JST
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
