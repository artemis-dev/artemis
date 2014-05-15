/**
 * @file   TAxisTreeProj.cc
 * @brief  Axis for tree projection
 *
 * @date   Created       : 2014-03-03 23:23:48 JST
 *         Last Modified : Mar 05, 2014 15:09:36 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 Shinsuke OTA
 */

#include "TAxisTreeProj.h"

using art::TAxisTreeProj;

ClassImp(TAxisTreeProj)

TAxisTreeProj::TAxisTreeProj()
: TAttCut(), fVariable(NULL), fSelection(NULL)
{
}

TAxisTreeProj::TAxisTreeProj(const char* var, Int_t nbins, Double_t min, Double_t max, const char* cut)
   : TAxis(nbins,min,max),TAttCut(cut), fVariable(NULL), fSelection(NULL)
{
   SetTitle(var);
}

TAxisTreeProj::~TAxisTreeProj()
{
}

TAxisTreeProj::TAxisTreeProj(const TAxisTreeProj& rhs)
   : TAxis(rhs), TAttCut(rhs)
{
}

TAxisTreeProj& TAxisTreeProj::operator=(const TAxisTreeProj& rhs)
{
   if (this != &rhs) {
//      rhs.Copy(this);
   }
   return *this;
}


TObject* TAxisTreeProj::Clone(const char *newname) const
{
   TAxisTreeProj *newobj = new TAxisTreeProj;
   TAxis::Copy(*newobj);
   TAttCut::Copy(*newobj);
   if (fVariable) {
      newobj->fVariable = new TTreeFormula(fVariable->GetName(),
                                          fVariable->GetTitle(),
                                          fVariable->GetTree());
   }
   if (fSelection) {
      newobj->fSelection = new TTreeFormula(fSelection->GetName(),
                                          fSelection->GetTitle(),
                                          fSelection->GetTree());
   }
   return newobj;
}


void TAxisTreeProj::Print(Option_t *opt) const
{
   printf("%s %s\n",GetName(),GetTitle());
   if (fVariable) printf(" var : %s %s\n",fVariable->GetName(),fVariable->GetTitle());
   if (fSelection) printf(" sel : %s %s\n",fSelection->GetName(),fSelection->GetTitle());
}
