/* $Id:$ */
/**
 * @file   TParameterStrings.cc
 * @date   Created : May 18, 2012 14:18:07 JST
 *   Last Modified : 2016-07-22 08:38:50 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *
 *    Copyright (C)2012
 */
#include "TParameterStrings.h"

#include <vector>
#include <yaml-cpp/yaml.h>

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

void art::TParameterStrings::GetValue(const char* name, DoubleVec_t& param)
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

void art::TParameterStrings::GetValue(const char* name, Long_t& param)
{
   if (fParamMap[name].size()==0) return;
   param = fParamMap[name][0].Atoll();
}

void art::TParameterStrings::GetValue(const char* name, Float_t& param)
{
   if (fParamMap[name].size()==0) return;
   param = fParamMap[name][0].Atof();
}

void art::TParameterStrings::GetValue(const char* name, Double_t& param)
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

void operator >> (const YAML::Node &node, art::TParameterStrings *str) {
   std::string name,value;
   for (YAML::Iterator it = node.begin(); it != node.end(); ++it) {
      std::vector <TString> prm;
      it.first() >> name;
      if (it.second().Type() == YAML::NodeType::Scalar) {
	 it.second() >> value;
	 prm.push_back(value);
      } else if (it.second().Type() == YAML::NodeType::Sequence) {
	 for (YAML::Iterator itv = it.second().begin(); itv != it.second().end(); ++itv) {
	    if (itv->Type() == YAML::NodeType::Scalar) {
	       *itv >> value;
	       prm.push_back(value);
	    } else if (itv->Type() == YAML::NodeType::Sequence) {
               for (YAML::Iterator itv2 = itv->begin(); itv2 != itv->end(); ++itv2) {
                  if (itv2->Type() == YAML::NodeType::Scalar) {
                     *itv2 >> value;
		     prm.push_back(value);
                  } else {
                     // NOT SUPPORTED
                  }
               }
	    }
	 }
      }
      str->Add(name.data(),prm);
   }
}
