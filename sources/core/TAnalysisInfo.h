/**
 * @file   TAnalysisInfo.h
 * @brief  container for analysis information
 *
 * @date   Created       : 2018-07-28 09:02:06 JST
 *         Last Modified : 2020-07-30 21:13:49 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2018 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_5CA39B51_D066_4D5C_A8FE_63A1F71A2295
#define INCLUDE_GUARD_UUID_5CA39B51_D066_4D5C_A8FE_63A1F71A2295

#include "TNamed.h"
#include <map>

namespace art {
   class TAnalysisInfo;
}

class TDirectory;

class art::TAnalysisInfo : public TNamed {
public:
   static const char* kDefaultAnalysInfoName;
   
   TAnalysisInfo();
   virtual ~TAnalysisInfo();

   TAnalysisInfo(const TAnalysisInfo& rhs);
   TAnalysisInfo& operator=(const TAnalysisInfo& rhs);

   static Bool_t AddTo(TDirectory *);
   static Bool_t RemoveFrom(TDirectory *);

   virtual void Print(Option_t *option = "") const ;
   virtual void ShowProcessors() const;

   const char* GetSteeringFileName() const { return fSteeringFileName; }
   const char* GetRunNumber() const { return fRunNumber; }
   const char* GetRunName() const { return fRunName; }
   Long64_t GetAnalyzedEventNumber() const { return fAnalyzedEventNumber; }
   const char* GetAnalysisStartTime() const { return fAnalysisStartTime; }
   const char* GetAnalysisEndTime() const { return fAnalysisEndTime; }
   const char* GetProcessors() const { return fProcessors; }
   const char* GetStringData(const char* key) {
      if (fStringData.count(TString(key))) {
         return fStringData[TString(key)];
      } else {
         return "";
      }
   }

   void SetSteeringFileName(const char* fileName) { fSteeringFileName = fileName; }
   void IncrementEventNumber() { ++fAnalyzedEventNumber; }
   void SetRunNumber(const char* runNumber) { fRunNumber = runNumber; }
   void SetRunName(const char* runName) { fRunName = runName; }
   void SetProcessors(const char* proc) { fProcessors = proc; }
   void SetAnalysisStartTime(const char* timestamp) { fAnalysisStartTime = timestamp; }
   void SetAnalysisEndTime(const char* timestamp) { fAnalysisEndTime = timestamp; }
   void SetStringData(const char* key, const char* val) { fStringData.insert(std::make_pair(TString(key),TString(val))); }

   // for hadd
   virtual Long64_t Merge(TCollection *);
   virtual ROOT::MergeFunc_t GetMerge() { return NULL; }
protected:
   TString fSteeringFileName; // steering file
   Long64_t fAnalyzedEventNumber; // analyzed event number (counted by TLoop)
   TString fRunNumber; // run number or "online" (given by IEventStore)
   TString fRunName; // run name or empty for online (given by IEventStore)
   TString fAnalysisStartTime; // analysis start  timestamp
   TString fAnalysisEndTime; // analysis end timestamp
   TString fProcessors; // yaml
   std::map<TString,TString> fStringData; // string information
private:

   ClassDef(TAnalysisInfo,2) // container for analysis information
};

#endif // INCLUDE_GUARD_UUID_5CA39B51_D066_4D5C_A8FE_63A1F71A2295
