/* $Id:$ */
/**
 * @file   TCatAtPedestalSubtractTester.h
 * @date   Created : 2015-03-12 21:08:33 JST
 *   Last Modified : 2015-05-19 17:13:06 JST by tokieda
 * @author Hiroshi TOKIEDA <tokieda@cns.s.u-tokyo.ac.jp>
 *  Added from share/src/cat/TCatAtPedestalSubtractProcessor.h
 *  
 *    Copyright (C)2015
 */
#ifndef TCATATPEDESTALSUBTRACTTESTER_H
#define TCATATPEDESTALSUBTRACTTESTER_H

#include <TProcessor.h>
#include "TPulseDrawer.h"
#include <TClonesArray.h>
#include <TList.h>
#include <TRunInfo.h>
#include <TH2F.h>

namespace art {
  class TCatAtPedestalSubtractTester;
}

class art::TCatAtPedestalSubtractTester  : public art::TProcessor {

public:
   TCatAtPedestalSubtractTester();
   ~TCatAtPedestalSubtractTester();

   virtual void Init(art::TEventCollection *col);
   virtual void Process();

private:
   StringVec_t  fInputColName;
   TString      fOutputColName;
   TString      fOutputSigmaColName;
   TClonesArray **fCATHit; //!
   TClonesArray *fCATHitOut;
   TClonesArray *fSigma;

   Int_t        fEvt;
   Bool_t       fDebug;
   Bool_t       fDetailDebug;
   FILE         *fp; //!->
   TString      fLogFilePath;
   TString      fLogFileName;
   Bool_t       fSaveHist;
   Bool_t       fDrawFig;
   Bool_t       fSaveFig;
   Int_t        fWaitTime;
   Int_t        fCanvasX;
   Int_t        fCanvasY;
   Int_t        fCanvasH;
   Int_t        fCanvasW;
   TString      fFigFile;
   TString      fFigFilePath;
   TString      fAddFigFilePath;
   Int_t        fSaveEvt;
   Int_t        fSaveHit;
   Int_t        fViewID;
   
   TString      fNameRunHeaders;
   TList        **fRunHeaders; //!
   TRunInfo     *fRunInfo;
   TString      fRunName;
   Int_t        fRunNumber;

   Bool_t       fOutputInvalid;
   Bool_t       fCutSigma;

   TH2F         *fhPedMean;
   TH2F         *fhPedSigma;
   TH1F         *fh1DPedSigma;
   TH1F         *fhNSamples;
   TH2F         *fhNSamplesSigma;

   Float_t      fLowSigma;
   Float_t      fHighSigma;

   TPulseDrawer *fPulseDrawer;

   ClassDef(TCatAtPedestalSubtractTester,1);
};
#endif // end of #ifdef TCATATPEDESTALSUBTRACTTESTER_H
