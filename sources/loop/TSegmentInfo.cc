/**
 * @file   TSegmentInfo.cc
 * @brief  segment infomation
 *
 * @date   Created       : 2014-05-17 18:53:47 JST
 *         Last Modified : 2023-01-28 22:58:30 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 Shinsuke OTA
 */

#include "TSegmentInfo.h"
#include <TModuleInfo.h>
#include <TROOT.h>
#include <yaml-cpp/yaml.h>

using art::TSegmentInfo;

ClassImp(TSegmentInfo)

TSegmentInfo::FormatType_t TSegmentInfo::fgFormatType = RIDF;

TSegmentInfo::TSegmentInfo()
{
}

TSegmentInfo::~TSegmentInfo()
{
}

TSegmentInfo::TSegmentInfo(const TSegmentInfo& rhs)
{
}

TSegmentInfo& TSegmentInfo::operator=(const TSegmentInfo& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}

Bool_t TSegmentInfo::LoadYAMLNode(const YAML::Node& node)
{
   try {
      fModuleType = node["type"].as<std::string>().c_str();
      switch (fgFormatType) {
      case RIDF:
         for (YAML::const_iterator it = node["segid"].begin(); it != node["segid"].end(); it++) {
            fID.push_back((*it).as<int>());
         }
         if (fID.size() != 3) return kFALSE;
         // @TODO fID should be initialized here
         SetTitle(TString::Format("Dev = %d, FP = %d, DataType = %d",fID[0],fID[1],fID[2]));
         break;
      case RDF:
         fID.push_back(node["segid"].as<int>());
         SetTitle(TString::Format("SegID = %d",fID[0]));
         break;
         
      }
      for (YAML::const_iterator it = node["modules"].begin(); it != node["modules"].end(); it++) {
         TModuleInfo *info = new TModuleInfo;
         if (!info->LoadYAMLNode(*it)) {
            delete info;
         }
         fModules.push_back(info);
      }
   } catch (YAML::Exception e) {
      Error("LoadYAMLNode","%s",e.what());
      return kFALSE;
   }
   return kTRUE;
}


void TSegmentInfo::Print(Option_t *opt) const
{
   TString outs;
   TString indent(" ");
   for (Int_t i=0; i!=gROOT->GetDirLevel(); i++) {
      indent.Append(" ");
   }
   // first line is automatically indented
   outs.Append(GetName()).Append("\n");
   outs.Append(indent).Append(GetTitle()).Append("\n");
   outs.Append(indent).Append(TString::Format("Module = %s",fModuleType.Data())).Append("\n");
   gROOT->IncreaseDirLevel();
   printf("%s",outs.Data());
   for (UInt_t i=0; i!=fModules.size(); i++) {
      fModules[i]->Print();
   }
   gROOT->DecreaseDirLevel();
}
   
   
