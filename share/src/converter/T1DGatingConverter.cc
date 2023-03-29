/**
 * @file   T1DGatingConverter.cc
 * @brief  converter for 1D gating
 *
 * @date   Created       : 2015-03-16 10:24:02 JST
 *         Last Modified : 2015-03-16 10:58:38 JST (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2015 KAWASE Shoichiro
 */

#include "T1DGatingConverter.h"

#include <utility>

#include <TROOT.h>
#include <TString.h>
#include <TClass.h>

#include <constant.h>

using art::T1DGatingConverter;

ClassImp(T1DGatingConverter)

T1DGatingConverter::T1DGatingConverter()
  : TFixedNumberParameter(2)
{
   fParam[0] = -TMath::Infinity();
   fParam[1] =  TMath::Infinity();
}

T1DGatingConverter::T1DGatingConverter(Double_t min, Double_t max)
  : TFixedNumberParameter(2)
{
   Set(min,max);
}

T1DGatingConverter::~T1DGatingConverter(){}

void T1DGatingConverter::Set(Double_t min, Double_t max)
{
   if (min > max) std::swap(min,max);
   fParam[0] = min;
   fParam[1] = max;
}

Double_t T1DGatingConverter::Convert(Double_t val) const
{
   return (val < fParam[0]) || (fParam[1] <= val) ? kInvalidD : val;
}

void T1DGatingConverter::Print(Option_t*) const
{
   const TString indent(' ', gROOT->GetDirLevel());
   printf("OBJ: %s\t%s\n", IsA()->GetName(), GetName());
   printf("%s [min, max] = [%lf, %lf]\n", indent.Data(), fParam[0], fParam[1]);
}
