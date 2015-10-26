/* $Id:$ */
/**
 * @file   TParameterStrings.h
 * @date   Created : May 18, 2012 14:18:49 JST
 *   Last Modified : Oct 26, 2015 17:30:17 EDT
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#ifndef TPARAMETERSTRINGS_H
#define TPARAMETERSTRINGS_H

#include <map>
#include <TString.h>
#include <TObject.h>

#include <TArtTypes.h>

namespace art {
   class TParameterStrings;
}
namespace YAML {
   class Node;
}

void operator >> (const YAML::Node &node, art::TParameterStrings *str);

class art::TParameterStrings : public TObject {

public:
   typedef std::map<TString, std::vector<TString> > map_t;
   TParameterStrings();
   ~TParameterStrings();

   void Add(const TString& key, const std::vector<TString>& values);
   void Clear(Option_t*opt = "");

   void Erase(const TString&key);

   void GetValue(const char* name, IntVec_t& param);
   void GetValue(const char* name, FloatVec_t& param);
   void GetValue(const char* name, StringVec_t& param);

   void GetValue(const char* name, Int_t& param);
   void GetValue(const char* name, Long_t& param);
   void GetValue(const char* name, Float_t& param);
   void GetValue(const char* name, Bool_t& param);
   void GetValue(const char* name, TString& param);

   Bool_t IsSet(const char* name);

   map_t::const_iterator Begin() { return (map_t::const_iterator) fParamMap.begin(); }
   map_t::const_iterator End() { return (map_t::const_iterator) fParamMap.end(); }

protected:
   std::map<TString, std::vector<TString> > fParamMap;
   
};
#endif // end of #ifdef TPARAMETERSTRINGS_H
