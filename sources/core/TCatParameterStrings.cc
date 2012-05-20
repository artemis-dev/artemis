/* $Id:$ */
/**
 * @file   TCatParameterStrings.cc
 * @date   Created : May 18, 2012 14:18:07 JST
 *   Last Modified : May 19, 2012 18:10:41 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TCatParameterStrings.h"

TCatParameterStrings::TCatParameterStrings()
{
}
TCatParameterStrings::~TCatParameterStrings()
{
}

void TCatParameterStrings::Add(const TString& key,
                               const std::vector<TString>& value)
{
   for (UInt_t i = 0; i < value.size(); i++) {
      fParamMap[key].push_back(value[i]);
   }
}

void TCatParameterStrings::Clear(Option_t* /* opt */)
{
   fParamMap.clear();
}


void TCatParameterStrings::Erase(const TString& key)
{
   fParamMap.erase(key);
}

void TCatParameterStrings::GetValue(const char* name, IntVec_t& param)
{
   std::vector<TString>::iterator it;
   TString key(name);
   param.clear();
   for (it = fParamMap[key].begin(); it != fParamMap[key].end(); it++) {
      param.push_back((*it).Atoi());
   }
}

void TCatParameterStrings::GetValue(const char* name, FloatVec_t& param)
{
   std::vector<TString>::iterator it;
   TString key(name);
   param.clear();
   for (it = fParamMap[key].begin(); it != fParamMap[key].end(); it++) {
      param.push_back((*it).Atof());
   }
}

void TCatParameterStrings::GetValue(const char* name, StringVec_t& param)
{
   std::vector<TString>::iterator it;
   TString key(name);
   param.clear();
   for (it = fParamMap[key].begin(); it != fParamMap[key].end(); it++) {
      param.push_back((*it));
   }
}

void TCatParameterStrings::GetValue(const char* name, Int_t& param)
{
   if (fParamMap[name].size()==0) return;
   param = fParamMap[name][0].Atoi();
}

void TCatParameterStrings::GetValue(const char* name, Float_t& param)
{
   if (fParamMap[name].size()==0) return;
   param = fParamMap[name][0].Atof();
}

void TCatParameterStrings::GetValue(const char* name, Bool_t& param)
{
   if (fParamMap[name].size()==0) return;
   param = kFALSE;
   TString str = fParamMap[name][0];
   if (!str.CompareTo("true",TString::kIgnoreCase) || 
       !str.CompareTo("T",TString::kIgnoreCase) ||
       str.Atoi() == 1) {
      param = kTRUE;
   }
}

void TCatParameterStrings::GetValue(const char* name, TString& param)
{
   if (fParamMap[name].size()==0) return;
   param = fParamMap[name][0];
}


Bool_t TCatParameterStrings::IsSet(const char* name)
{
   TString key(name);
   return fParamMap[key].size() > 0;
}

