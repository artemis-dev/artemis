/**
 * @file   TPolynomialConverter.cc
 * @brief
 *
 * @date   Created:       2013-10-13 15:33:46
 *         Last Modified: 2014-06-30 16:25:20 JST (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved.
 */

#include "TPolynomialConverter.h"

#include <vector>
#include <numeric>
#include <iostream>

#include <TROOT.h>
#include <TMath.h>
#include <TObjString.h>
#include <TObjArray.h>
#include <TClass.h>
#include <TRegexp.h>

#include "constant.h"

using art::TPolynomialConverter;

ClassImp(art::TPolynomialConverter)

// Default constructor
TPolynomialConverter::TPolynomialConverter() : fCoefficient(NULL) {}

TPolynomialConverter::~TPolynomialConverter()
{
   delete fCoefficient;
}

void TPolynomialConverter::Set(const std::vector<Double_t> &val)
{
   if (fCoefficient) {
      fCoefficient->clear();
      fCoefficient->reserve(val.size());
      fCoefficient->assign(val.begin(),val.end());
   } else {
      fCoefficient = new std::vector<Double_t>(val);
   }
}

namespace {
   struct Sum {
      // functional object used only in TPolynomialConverter::Convert()
      // this cannot be defined in TPolynomialConverter
      // because local type cannot be a template argument in C++0x
      Sum(Double_t val) : fVal(val) {}
      Double_t operator()(Double_t sum, Double_t input) {
	 return sum * fVal + input;
      }
      Double_t fVal;
   };
}

Double_t TPolynomialConverter::Convert(Double_t val) const
{
   return fCoefficient ?
      std::accumulate(fCoefficient->rbegin(),fCoefficient->rend(),0.,Sum(val))
      : kInvalidD;
}

Bool_t TPolynomialConverter::LoadTextFile(std::istream &str)
{
   TString s;
   while (s.ReadLine(str)) {
      if (LoadString(s)) return kTRUE;
   }
   return kFALSE;
}

Bool_t TPolynomialConverter::LoadString(const TString &s)
{
   if (fCoefficient) {
      fCoefficient->clear();
   } else {
      fCoefficient = new std::vector<Double_t>;
   }

   const Int_t commentStartIndex = s.First('#');
   const TString &strWithoutComment =
      commentStartIndex == -1 ? TString(s) : TString(s).Replace(commentStartIndex,s.Length(),"");
   TObjArray *a = strWithoutComment.Tokenize(" ,\t");
   for (Int_t i = 0, n = a->GetEntriesFast(); i!=n ; ++i) {
      const TString &subs = static_cast<TObjString*>(a->UncheckedAt(i))->GetString();
      if(!subs.IsFloat()) break;
      fCoefficient->push_back(subs.Atof());
   }

   if (!fCoefficient->empty()){
      fLoaded = kTRUE;
      return kTRUE;
   } else {
      return kFALSE;
   }
}

void TPolynomialConverter::Print(Option_t*) const
{
   const TString indent(' ', gROOT->GetDirLevel());
   printf("OBJ: %s\t%s\n", IsA()->GetName(), GetName());
   if (!fCoefficient) return;
   if (fCoefficient->size()) {
      printf("%s f(x) = %e", indent.Data(), fCoefficient->at(0));
      for (Int_t i = 1, n = fCoefficient->size(); i < n; ++i) {
	 printf(" + %e * x^%d", fCoefficient->at(i), i);
      }
      printf("\n");
   }
}

void TPolynomialConverter::PrintDescriptionText() const
{
   if (!fCoefficient) return;
   for (Int_t i = 1, n = fCoefficient->size(); i < n; ++i) {
      printf("%lf ", fCoefficient->at(i));
   }
   printf("\n");
}
