/**
 * @file   TTimestampEventReconstructor.h
 * @brief  event reconstruction using timestamp
 *
 * @date   Created       : 2018-06-27 15:37:06 JST
 *         Last Modified : 2018-07-17 12:52:21 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2018 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_A98A6FF6_D9E2_469D_8E8A_6071D8D02856
#define INCLUDE_GUARD_UUID_A98A6FF6_D9E2_469D_8E8A_6071D8D02856

#include "TProcessor.h"
#include <deque>

namespace art {
   class TTimestampEventReconstructor;
   class TSimpleData;
   class TEventHeader;
   class TTimestampEventList;
}

class TFile;
class TH1F;

class art::TTimestampEventReconstructor : public TProcessor {
public:
   TTimestampEventReconstructor();
   virtual ~TTimestampEventReconstructor();

   TTimestampEventReconstructor(const TTimestampEventReconstructor& rhs);
   TTimestampEventReconstructor& operator=(const TTimestampEventReconstructor& rhs);

   virtual void Init(TEventCollection *col);
   virtual void BeginOfRun();
   virtual void Process();
   virtual void PostLoop();

protected:
   Int_t fNumLists;
   StringVec_t fInputLists; // vector of event lists
   StringVec_t fFileNames; // vector of file names
   StringVec_t fTimestampNames; // vector of timestamp names
   StringVec_t fEventHeaderNames; // vector of event header names
   DoubleVec_t fSearchWindowStarts; // vector of search window start
   DoubleVec_t fSearchWindowEnds; // vector of search window start
   StringVec_t fOutputListNames; // vector of file names

   Int_t fMaxAllCombination; // make all the combination
   Double_t fCombinationThreshold;

   TString fOutputFileName; // vector of file names

   Int_t fDoUpdateOffset; // update offset
   DoubleVec_t fAdditionalOffset;

   std::vector<TSimpleData**> fTimestamps;
   std::vector<TEventHeader**> fEventHeaders;

   TFile* fOutputFile; //! pointer to output files
   std::vector<TTimestampEventList*> fOutputEventLists; //! vector of output files

   std::vector<TH1F*> fTimestampHists; //! hists for reconstructed timestamps
   std::vector<TH1F*> fTimestampHistsAll; //! hists for all timestamps

   std::vector<std::deque<Long64_t> > fHeaderQueue;
   std::vector<std::deque<Double_t> > fTimestampQueue;
   
   virtual void Pop(Int_t idx) {
      if (idx < 0 || idx >= fNumLists) return;
      if (fHeaderQueue[idx].size() == 0) return;
      fHeaderQueue[idx].pop_front();
      fTimestampQueue[idx].pop_front();
   }
   
private:

   ClassDef(TTimestampEventReconstructor,1) // event reconstruction using timestamp
};

#endif // INCLUDE_GUARD_UUID_A98A6FF6_D9E2_469D_8E8A_6071D8D02856
