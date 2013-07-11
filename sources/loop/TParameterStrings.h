/* $Id:$ */
/**
 * @file   TParameterStrings.h
 * @date   Created : May 18, 2012 14:18:49 JST
 *   Last Modified : May 19, 2012 17:52:34 JST
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

#include <TTypes.h>

namespace art {
   class TParameterStrings;
}
namespace YAML {
   class Node;
}

void operator >> (const YAML::Node &node, art::TParameterStrings *&str);

class art::TParameterStrings : public TObject {

public:
   TParameterStrings();
   ~TParameterStrings();

   void Add(const TString& key, const std::vector<TString>& values);
   void Clear(Option_t*opt = "");

   void Erase(const TString&key);

   void GetValue(const char* name, IntVec_t& param);
   void GetValue(const char* name, FloatVec_t& param);
   void GetValue(const char* name, StringVec_t& param);

   void GetValue(const char* name, Int_t& param);
   void GetValue(const char* name, Float_t& param);
   void GetValue(const char* name, Bool_t& param);
   void GetValue(const char* name, TString& param);

   Bool_t IsSet(const char* name);


protected:
   std::map<TString, std::vector<TString> > fParamMap;
   
};
#endif // end of #ifdef TPARAMETERSTRINGS_H
