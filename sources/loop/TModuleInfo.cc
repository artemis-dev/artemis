/**
 * @file   TModuleInfo.cc
 * @brief  module information
 *
 * @date   Created       : 2014-05-17 16:42:49 JST
 *         Last Modified : Jun 01, 2014 12:50:30 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 Shinsuke OTA
 */

#include "TModuleInfo.h"
#include <TROOT.h>
#include <yaml-cpp/yaml.h>
using art::TModuleInfo;

ClassImp(TModuleInfo)

TModuleInfo::TModuleInfo()
: fID(-1), fType("")
{
   fHists = new TObjArray;
}

TModuleInfo::~TModuleInfo()
{
   fHists->Clear();
   delete fHists;
}

TModuleInfo::TModuleInfo(const TModuleInfo& rhs)
{
}

TModuleInfo& TModuleInfo::operator=(const TModuleInfo& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}

Bool_t TModuleInfo::LoadYAMLNode(const YAML::Node& node)
{
   const char *kMethodName = "LoadYAMLNode";
   const char *const kKeyType = "type";
   const char *const kKeyID = "id";
   const char *const kKeyRanges = "ranges";

   try {
      if (node.FindValue(kKeyType)) {
         fType =  node[kKeyType].to<std::string>().c_str();
      }
      fID =  (node)[kKeyID].to<Int_t>();
      if (const YAML::Node *ranges = node.FindValue(kKeyRanges)) {
         for (YAML::Iterator itRanges = ranges->begin(); itRanges != ranges->end(); itRanges++) {
            TString rangeName = itRanges.first().to<std::string>().c_str();
            Int_t nbins = itRanges.second()[0].to<Int_t>();
            Double_t xmin = itRanges.second()[1].to<Double_t>();
            Double_t xmax = itRanges.second()[2].to<Double_t>();;
            TAxis *axis = new TAxis(nbins,xmin,xmax);
            axis->SetName(rangeName);
            fRanges.push_back(axis);
         }
      }
   } catch(YAML::Exception &e) {
      Error(kMethodName,"%s",e.what());
      return kFALSE;
   }
   return kTRUE;
}

void TModuleInfo::Print(Option_t *opt) const
{
   TString outs;
   TString indent(" ");
   for (Int_t i=0; i!=gROOT->GetDirLevel(); i++) {
      indent.Append(" ");
   }
   outs.Append(indent).Append(TString::Format("id = %d",fID)).Append("\n");
   for (UInt_t i=0; i!= fRanges.size(); i++) {
      TAxis *axis = fRanges[i];
      outs.Append(indent).Append(TString::Format(" %-10s : %d, %g, %g\n",axis->GetName(),axis->GetNbins(),axis->GetXmin(),axis->GetXmax()));
   }
   printf("%s",outs.Data());
}
