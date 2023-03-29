/**
 * @file   TSplineConverter.cc
 * @brief  converter using spline
 *
 * @date   Created       : 2015-04-20 12:10:09 JST
 *         Last Modified : 2018-02-04 11:56:40 JST (ota)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2015 KAWASE Shoichiro
 */

#include "TSplineConverter.h"

#include <cstdio>
#include <iostream>

#include <TClass.h>
#include <TSpline.h>

#include <constant.h>

using art::TSplineConverter;

ClassImp(TSplineConverter)

TSplineConverter::TSplineConverter() : fSpline(NULL) {}

TSplineConverter::~TSplineConverter()
{
   delete fSpline;
}

TSplineConverter::TSplineConverter(const TSplineConverter& rhs)
   : TConverterBase(rhs), fSpline(NULL)
{
   if (rhs.fSpline) {
      fSpline = new TSpline3(*rhs.fSpline);
   }
}

TSplineConverter& TSplineConverter::operator=(const TSplineConverter& rhs)
{
   if (this != &rhs) {
      TConverterBase::operator=(rhs);
      if (fSpline && !rhs.fSpline) {
	 delete fSpline;
      } else if (!fSpline && rhs.fSpline) {
	 fSpline = new TSpline3(*rhs.fSpline);
      } else {
	 *fSpline = *rhs.fSpline;
      }
   }
   return *this;
}

void TSplineConverter::Set(Double_t *const x, Double_t *const y,
			   const Int_t nPoints)
{
   if (fSpline) delete fSpline;
   fSpline = new TSpline3("spline",x,y,nPoints);
}

Double_t TSplineConverter::Convert(const Double_t val) const
{
   return (IsValid(val) && fSpline) ? fSpline->Eval(val) : kInvalidD;
}

Bool_t TSplineConverter::LoadTextFile(std::istream &str)
{
   std::vector<Double_t> xvec, yvec;
   {
      Double_t x,y;
      TString s;
      while (s.ReadLine(str)) {
	 if (2 != std::sscanf(s.Data(), "%lf %lf %*s", &x, &y)) {
	    continue;
	 }
	 xvec.push_back(x);
	 yvec.push_back(y);
      }
   }

   Set(&xvec.front(),&yvec.front(),xvec.size());

   return (Bool_t)xvec.size();
}

void TSplineConverter::Print(Option_t *) const
{
   printf("OBJ: %s\t%s\tnPoints = %d\n",
	  IsA()->GetName(), GetName(), fSpline ? fSpline->GetNp() : 0);
}

void TSplineConverter::PrintDescriptionText() const
{
   if (!fSpline) {
      return;
   }

   printf("# TSplineConverter\n# number of points: %d\n# x y\n", fSpline->GetNp());
   Double_t x,y;
   for (Int_t i = 0; i!=fSpline->GetNp(); ++i) {
      fSpline->GetKnot(i,x,y);
      printf("%lf %lf\n", x,y);
   }
}
