/* $Id:$ */
/**
 * @file   TCatAtBeamInRecoilRegionCounter.h
 * @date   Created : Feb 15, 2016 14:15:37 JST
 *   Last Modified : 2018-02-04 13:17:51 JST (ota)
 * @author Hiroshi TOKIEDA <tokieda@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2016
 */
#ifndef TCATATBEAMINRECOILREGIONCOUNTER_H
#define TCATATBEAMINRECOILREGIONCOUNTER_H

#include <TProcessor.h>
#include <TH2F.h>

namespace art {
   class TCatAtBeamInRecoilRegionCounter;
}
class TClonesArray;

class art::TCatAtBeamInRecoilRegionCounter  : public TProcessor {

public:
   TCatAtBeamInRecoilRegionCounter();
   virtual ~TCatAtBeamInRecoilRegionCounter();

   TCatAtBeamInRecoilRegionCounter(const TCatAtBeamInRecoilRegionCounter& rhs);
   TCatAtBeamInRecoilRegionCounter& operator=(const TCatAtBeamInRecoilRegionCounter& rhs);

   virtual void Init(TEventCollection *col);
   virtual void Process();

protected:
   
private:
   FILE         *fp; //!->
   TClonesArray **fInput; //!
   TClonesArray *fOutput; //!
   TClonesArray *fExcludeEvtOutput;//!
   StringVec_t   fInputColName;
   TString       fOutputColName;
   TString       fOutputExcludeEvtColName;
   Int_t         fEvt;
   TString       fLogFilePath;
   TString       fLogFileName;
   Bool_t        fDebug;
   Bool_t        fDetailDebug;
   Bool_t        fSaveExcludeEvt;
   Bool_t        fSaveHist;

   TH1F         *fhRowRecoilRange; //!

   ClassDef(TCatAtBeamInRecoilRegionCounter,1)
};
#endif // end of #ifdef TCATATBEAMINRECOILREGIONCOUNTER_H
