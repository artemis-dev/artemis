/**
 * @file   TMonotoneTableConverter.cc
 * @brief
 *
 * @date   Created:       2013-10-17 16:49:43
 *         Last Modified: 2014-06-19 16:35:54 JST (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved.
 */

#include "TMonotoneTableConverter.h"

#include <functional>
#include <cstdio>
#include <TClass.h>
#include <iostream>
#include <TRandom.h>

#include "constant.h"

using art::TMonotoneTableConverter;
using std::greater;

ClassImp(art::TMonotoneTableConverter)

// Default constructor
TMonotoneTableConverter::TMonotoneTableConverter() : fX(NULL), fY(NULL) {}

TMonotoneTableConverter::~TMonotoneTableConverter()
{
   delete fX;
   delete fY;
}

TMonotoneTableConverter::TMonotoneTableConverter(const TMonotoneTableConverter& rhs)
   : TConverterBase(rhs), fX(NULL), fY(NULL)
{
   if (rhs.fX) {
      fX = new std::vector<Double_t>(*rhs.fX);
      fY = new std::vector<Double_t>(*rhs.fY);
   }
}

TMonotoneTableConverter& TMonotoneTableConverter::operator=(const TMonotoneTableConverter& rhs)
{
   TConverterBase::operator=(rhs);
   if (this != &rhs) {
      if (fX && !rhs.fX) {
	 delete fX;
	 delete fY;
      } else if (!fX && rhs.fX) {
	 fX = new std::vector<Double_t>(*rhs.fX);
	 fY = new std::vector<Double_t>(*rhs.fY);
      } else {
	 *fX = *rhs.fX;
	 *fY = *rhs.fY;
      }
   }
   return *this;
}

void TMonotoneTableConverter::Set(const Double_t *const x, const Double_t *const y,
				  const Int_t nPoints)
{
   if (!fX) {
      fX = new std::vector<Double_t>(nPoints);
      fY = new std::vector<Double_t>(nPoints);
   } else {
      fX->resize(nPoints);
      fY->resize(nPoints);
   }
   fX->assign(x,x+nPoints);
   fY->assign(y,y+nPoints);
   fNPoints = nPoints;

   if (fX->front() > fX->back()) {
      std::reverse(fX->begin(),fX->end());
      std::reverse(fY->begin(),fY->end());
   }
}

Double_t TMonotoneTableConverter::Convert(const Double_t val) const
{
   if (!fX || val < fX->front() || fX->back() <= val) return kInvalidD;

//   const std::binder2nd <greater<Double_t> > gtVal(greater<Double_t>(),val);
   const Int_t idx = std::find_if(fX->begin(),fX->end(),std::bind(std::greater<Double_t>(),std::placeholders::_1,val)) - fX->begin();

   const Double_t lower = fY->at(idx - 1);
   const Double_t upper = fY->at(idx);
   return lower + (upper - lower) * gRandom->Uniform();
}

Bool_t TMonotoneTableConverter::LoadTextFile(std::istream &str)
{
   Int_t nPoints = 0;
   str >> nPoints;
   if (nPoints < 1) return kFALSE;

   if (!fX) {
      fX = new std::vector<Double_t>(nPoints);
      fY = new std::vector<Double_t>(nPoints);
   } else {
      fX->resize(nPoints);
      fY->resize(nPoints);
   }

   {
      Int_t i = 0;
      TString s;
      while (s.ReadLine(str)) {
	 if (2 != std::sscanf(s.Data(), "%lf %lf %*s", &(fX->at(i)), &(fY->at(i)))) {
	    continue;
	 }
	 ++i;
	 if (i == nPoints) break;
      }
      if (i != nPoints) {
	 nPoints = i;
	 fX->resize(i);
	 fY->resize(i);
      }
   }

   fNPoints = nPoints;
   return (Bool_t)fNPoints;
}

void TMonotoneTableConverter::Print(Option_t *) const
{
   printf("OBJ: %s\t%s\tnPoints = %d\n",
	  IsA()->GetName(), GetName(), fNPoints);
}

void TMonotoneTableConverter::PrintDescriptionText() const
{
   printf("%d # number of points\n", fNPoints);
   for (Int_t i = 0; i!=fNPoints; ++i) {
      printf("%lf %lf\n", fX->at(i), fY->at(i));
   }
}
