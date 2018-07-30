/**
 * @file   TAnalysisInfo.cc
 * @brief  analysis information
 *
 * @date   Created       : 2018-07-28 09:55:24 JST
 *         Last Modified : 2018-07-30 15:43:08 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2018 Shinsuke OTA
 */

#include "TAnalysisInfo.h"
#include <TROOT.h>
#include <algorithm>
#include <TCollection.h>
#include <iostream>

using art::TAnalysisInfo;

ClassImp(TAnalysisInfo)

const char* TAnalysisInfo::kDefaultAnalysInfoName = "analysisInfo";

TAnalysisInfo::TAnalysisInfo()
   : TNamed(kDefaultAnalysInfoName,""), fAnalyzedEventNumber(0)
{
}

TAnalysisInfo::~TAnalysisInfo()
{
}

TAnalysisInfo::TAnalysisInfo(const TAnalysisInfo& rhs)
{
}

TAnalysisInfo& TAnalysisInfo::operator=(const TAnalysisInfo& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}

void TAnalysisInfo::Print(Option_t *opt) const
{
   Info("","Analysis Information");
   Info("","Steering    : %s",fSteeringFileName.Data());
   Info("","Run         : %s%s", fRunName.Data(),fRunNumber.Data());
   Info("","# of Recorded Events : %lld",fAnalyzedEventNumber);
   Info("","Analysis begins at  %s",fAnalysisStartTime.Data());
   Info("","         ends   at  %s",fAnalysisEndTime.Data());
}

void TAnalysisInfo::ShowProcessors() const {
//   std::for_each(fProcessors.begin(),fProcessors.end(),std::mem_fun( &TProcessor::PrintDescriptionYAML ));
   std::cout << fProcessors << std::endl;
}


Long64_t TAnalysisInfo::Merge(TCollection *col)
{
   Long64_t totalEventNumber = fAnalyzedEventNumber;
   TDatime startTime(fAnalysisStartTime);
   TDatime endTime(fAnalysisEndTime);
   std::vector<Int_t> runs;
   runs.push_back(fRunNumber.Atoi());
   TIter next(col);
   TObject *obj = NULL;
   while ((obj = next()) != NULL) {
      obj->Print();
      TAnalysisInfo *info = static_cast<TAnalysisInfo*>(obj);
      
      totalEventNumber += info->GetAnalyzedEventNumber();
      TDatime startTimeNew (info->GetAnalysisStartTime());
      TDatime endTimeNew (info->GetAnalysisEndTime());
      if (startTime > startTimeNew) startTime = startTimeNew;
      if (endTime < endTimeNew) endTime = endTimeNew;

      runs.push_back(TString(info->GetRunNumber()).Atoi());
   }
   std::sort(runs.begin(),runs.end());
   runs.erase(std::unique(runs.begin(),runs.end()),runs.end());
   
   if (runs.size() > 1) {
      fRunNumber = TString::Format("%04d-%04d",runs[0],runs[runs.size()-1]);
   }
   // update analyzed event number 
   fAnalyzedEventNumber = totalEventNumber;
   fAnalysisStartTime = startTime.AsSQLString();
   fAnalysisEndTime = endTime.AsSQLString();

   return fAnalyzedEventNumber;
}

