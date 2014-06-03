/**
 * @file   TModuleType.cc
 * @brief  module type definition
 *
 * @date   Created       : 2014-04-29 09:18:18 JST
 *         Last Modified : Apr 29, 2014 16:30:19 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 Shinsuke OTA
 */

#include "TModuleType.h"
#include <TROOT.h>
#include <yaml-cpp/yaml.h>

using art::TModuleType;

ClassImp(TModuleType)

TModuleType::TModuleType()
: fDecoderID(-1), fNch(0)
{
   fAxes.SetClass("TAxis");
}

TModuleType::~TModuleType()
{
   fAxes.Delete();
}

TModuleType::TModuleType(const TModuleType& rhs)
{
}

TModuleType& TModuleType::operator=(const TModuleType& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}

void TModuleType::SetValueRange(Int_t idx, const char* name, Int_t nbins, Double_t min, Double_t max)
{
   TAxis *axis = (TAxis*) fAxes.ConstructedAt(idx);
   axis->SetName(name);
   axis->SetTitle(name);
   axis->Set(nbins,min,max);
}

Bool_t TModuleType::LoadYAMLNode(const YAML::Node& node)
{
   try {
      node["id"] >> fDecoderID;
      node["ch"] >> fNch;
      const YAML::Node& values = node["values"];
      Int_t n = values.size();
      for (Int_t i=0; i!=n; i++) {
         std::string name;
         values[i].begin().first() >> name;
         const YAML::Node& range = values[i].begin().second();
         Int_t nbins;
         Double_t min,max;
         range[0] >> nbins;
         range[1] >> min;
         range[2] >> max;
         SetValueRange(i,name.c_str(),nbins,min,max);
         printf("%s %d [%f,%f]\n",name.c_str(),nbins,min,max);
      }
   } catch (YAML::Exception& e) {
      fprintf(stderr,"%s\n",e.what());
      return kFALSE;
   }
   return kTRUE;
}

void TModuleType::Print(Option_t *) const
{
//   TROOT::IndentLevel();
   printf("%s : id = %d, nch = %d\n",GetName(),fDecoderID,fNch);
   for (Int_t i=0; i!= fAxes.GetEntriesFast(); i++) {
      TAxis *axis = (TAxis*) fAxes[i];
      TROOT::IndentLevel();
      printf(" %-10s : %d, %g, %g\n",axis->GetName(),axis->GetNbins(),axis->GetXmin(),axis->GetXmax());
   }
}
