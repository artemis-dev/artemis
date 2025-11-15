/**
 * @File   TCatAtEventSelectionByRecoilHitPattern.h
 * @brief  event selection for tracking detector
 *
 * @date   Created       : 2014-03-08 09:31:23 JST
 *         Last Modified : 2018-02-04 13:09:59 JST (ota)
 * @author Hiroshi TOKIEDA <tokieda@cns.s.u-tokyo.ac.jp>
 *
 */

#ifndef TCATATEVENTSELECTIONBYRECOILHITPATTERN_H
#define TCATATEVENTSELECTIONBYRECOILHITPATTERN_H

#include <TProcessor.h>
#include <TH2F.h>

namespace art {
   class TCatAtEventSelectionByRecoilHitPattern;
}

class TClonesArray;

class art::TCatAtEventSelectionByRecoilHitPattern : public TProcessor {
public:
   TCatAtEventSelectionByRecoilHitPattern();
   virtual ~TCatAtEventSelectionByRecoilHitPattern();

   TCatAtEventSelectionByRecoilHitPattern(const TCatAtEventSelectionByRecoilHitPattern& rhs);
   TCatAtEventSelectionByRecoilHitPattern& operator=(const TCatAtEventSelectionByRecoilHitPattern& rhs);

   virtual void Init(TEventCollection *col);
   virtual void Process();

protected:
   
private:
   FILE         *fp; //!->
   TClonesArray **fInput; //!
   TClonesArray *fOutput; //!
   TClonesArray *fCandidatesClsOutput;//!
   TClonesArray *fExcludeEvtOutput; //!
   TClonesArray *fExcludeLessEvtOutput; //!
   TClonesArray *fExcludeMoreEvtOutput; //!
   StringVec_t   fInputColName;
   TString       fOutputColName;
   TString       fOutputExcludeEvtColName;
   TString       fOutputExcludeLessEvtColName;
   TString       fOutputExcludeMoreEvtColName;
   Int_t         fEvt;
   TString       fLogFilePath;
   TString       fLogFileName;
   Bool_t        fDebug;
   Bool_t        fDetailDebug;
   Bool_t        fSaveCandidatesCls;
   Bool_t        fSaveExcludeEvt;
   Bool_t        fSaveHist;
   Int_t         fSetNColRecoilRange;

//   TH1F         *fhNumZeroHits;
   TH1F         *fhNumCluster;
   TH1F         *fhColRecoilRange;
   TH1F         *fhNumOfGoodEvent;

   Int_t        fMinNHits;

   ClassDef(TCatAtEventSelectionByRecoilHitPattern,1)
};

#endif // TCATATEVENTSELECTIONBYRECOILHITPATTERN_H
