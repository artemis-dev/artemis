/* $Id:$ */
/**
 * @file   TParameterStrings.cc
 * @date   Created : May 18, 2012 14:18:07 JST
 *   Last Modified : May 19, 2012 18:10:41 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TParameterStrings.h"

art::TParameterStrings::TParameterStrings()
{
}
art::TParameterStrings::~TParameterStrings()
{
}

void art::TParameterStrings::Add(const TString& key,
                               const std::vector<TString>& value)
{
   for (UInt_t i = 0; i < value.size(); i++) {
      fParamMap[key].push_back(value[i]);
   }
}

void art::TParameterStrings::Clear(Option_t* /* opt */)
{
   fParamMap.clear();
}


void art::TParameterStrings::Erase(const TString& key)
{
   fParamMap.erase(key);
}

void art::TParameterStrings::GetValue(const char* name, IntVec_t& param)
{
   std::vector<TString>::iterator it;
   TString key(name);
   param.clear();
   for (it = fParamMap[key].begin(); it != fParamMap[key].end(); it++) {
      param.push_back((*it).Atoi());
   }
}

void art::TParameterStrings::GetValue(const char* name, FloatVec_t& param)
{
   std::vector<TString>::iterator it;
   TString key(name);
   param.clear();
   for (it = fParamMap[key].begin(); it != fParamMap[key].end(); it++) {
      param.push_back((*it).Atof());
   }
}

void art::TParameterStrings::GetValue(const char* name, StringVec_t& param)
{
   std::vector<TString>::iterator it;
   TString key(name);
   param.clear();
   for (it = fParamMap[key].begin(); it != fParamMap[key].end(); it++) {
      param.push_back((*it));
   }
}

void art::TParameterStrings::GetValue(const char* name, Int_t& param)
{
   if (fParamMap[name].size()==0) return;
   param = fParamMap[name][0].Atoi();
}

void art::TParameterStrings::GetValue(const char* name, Float_t& param)
{
   if (fParamMap[name].size()==0) return;
   param = fParamMap[name][0].Atof();
}

void art::TParameterStrings::GetValue(const char* name, Bool_t& param)
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

void art::TParameterStrings::GetValue(const char* name, TString& param)
{
   if (fParamMap[name].size()==0) return;
   param = fParamMap[name][0];
}


Bool_t art::TParameterStrings::IsSet(const char* name)
{
   TString key(name);
   return fParamMap[key].size() > 0;
}

