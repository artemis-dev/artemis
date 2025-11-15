/**
 * @file   TFixedNumberParameter.cc
 * @brief
 *
 * @date   Created       : 2014-06-20 15:14:52 JST
 *         Last Modified : 2018-02-04 11:46:08 JST (ota)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 KAWASE Shoichiro
 */

#include "TFixedNumberParameter.h"

#include <vector>
#include <iostream>

#include <TROOT.h>
#include <TRegexp.h>
#include <TObjArray.h>
#include <TObjString.h>
#include <TClass.h>

using art::TFixedNumberParameter;

ClassImp(TFixedNumberParameter)

TFixedNumberParameter& TFixedNumberParameter::operator=(const TFixedNumberParameter& rhs)
{
   TConverterBase::operator=(rhs);
   const std::vector<Float_t>& vec = rhs.fParam;
   fParam.reserve(vec.size());
   fParam.assign(vec.begin(),vec.end());
   return *this;
}

Bool_t TFixedNumberParameter::LoadTextFile(std::istream &str)
{
   TString s;
   while (s.ReadLine(str)) {
      if (LoadString(s)) return kTRUE;
   }
   return kFALSE;
}

Bool_t TFixedNumberParameter::LoadString(const TString &s)
{
   const Int_t commentStartIndex = s.First('#');
   const TString &strWithoutComment =
      commentStartIndex == -1 ? TString(s) : TString(s).Replace(commentStartIndex,s.Length(),"");
   TObjArray *a = strWithoutComment.Tokenize(" ,\t");
   const UInt_t nTokens = a->GetEntriesFast();
   if (!nTokens) {
      return kFALSE;
   } else if (nTokens != fParam.size()) {
      Error("LoadString","Number of parameter is %d (expected: %d)",nTokens,(UInt_t)fParam.size());
      return kFALSE;
   }

   for (Int_t i = 0, n = fParam.size(); i != n; ++i) {
      const TString &subs = static_cast<TObjString*>(a->UncheckedAt(i))->GetString();
      if(!subs.IsFloat()) return kFALSE;
      fParam[i] = subs.Atof();
   }

   fLoaded = kTRUE;
   return kTRUE;
}

void TFixedNumberParameter::Print(Option_t*) const
{
   const TString indent(' ', gROOT->GetDirLevel());
   printf("OBJ: %s\t%s\n", IsA()->GetName(), GetName());
   printf("%s [",indent.Data());
   for (Int_t i = 0, n = fParam.size(); i != n; ++i) {
      if (i) printf(", ");
      printf("%f",fParam[i]);
   }
   printf("]\n");
}

void TFixedNumberParameter::PrintDescriptionText() const
{
   TString output("");
   for (Int_t i = 0, n = fParam.size(); i != n; ++i) {
      output.Append(TString::Format("%f ",fParam[i]));
	 }
   output.Chop().Append("\n");
   printf("%s",output.Data());
}
