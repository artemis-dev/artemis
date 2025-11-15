/* $Id:$ */
/**
 * @file   TCatAtSaveMaxSamplePatternProcessor.h
 * @date   Created : 2016-04-28 16:37:12 JST
 *   Last Modified : 2016-05-15 03:24:53 JST by tokieda
 * @author Hiroshi TOKIEDA <tokieda@cns.s.u-tokyo.ac.jp>
 *   based on TCatAtHitPatternProcessor.h
 *  
 *    Copyright (C)2016
 */
#ifndef TCATATSAVEMAXSAMPLEPATTERNPROCESSOR_H
#define TCATATSAVEMAXSAMPLEPATTERNPROCESSOR_H

#include <TProcessor.h>
#include <TCanvas.h>
#include <TH2Poly.h>
#include <TText.h>
#include <TRunInfo.h>
#include <TList.h>
#include <TDatime.h>
#include <TH1.h>
#include <TF1.h>
#include <TSimpleData.h>
#include <TCatReadoutPadArray.h>

namespace art {
   class TCatAtSaveMaxSamplePatternProcessor;
   class TEventHeader;
}

class art::TCatAtSaveMaxSamplePatternProcessor  : public art::TProcessor {

public:
   TCatAtSaveMaxSamplePatternProcessor();
   ~TCatAtSaveMaxSamplePatternProcessor();

   void Init(art::TEventCollection *col);
   void Process();

   StringVec_t fInputColName;   
   TClonesArray **fInput; //!
   Int_t    fNPad;
   Double_t *fPadX; //!
   Double_t *fPadY; //!
   Int_t    fWaitTime;
   Int_t    fEvt;

   TCanvas  *fCanvas; //!
   Int_t    fCanvasX;
   Int_t    fCanvasY;
   Int_t    fCanvasH;
   Int_t    fCanvasW;
   TText    *t; //!
   TString fReadoutPadName;
   TCatReadoutPadArray** fPadArray;//!
   TH2Poly* fHistPad; //!

   Bool_t   fSaveFigFlag;
   Bool_t   fFillClsFlag;
   Bool_t   fNoColorFig;
   TString  fFigFile;
   TString  fFigFilePath;
   TString  fAddFigFilePath;
   Int_t    fSaveEvt;
   TString  fLogFilePath;
   TString  fLogFileName;
   FILE     *fp; //!->
   Bool_t   fDebug;
   Bool_t   fDetailDebug;
   Bool_t   fNoClear;

   TString  fNameRunHeaders;
   TList    **fRunHeaders; //!
   TString  fNameEventHeader;
   TEventHeader **fEventHeader; //!
   TRunInfo *fRunInfo; //!
   TString  fRunName;
   Int_t    fRunNumber;
   TDatime  *fDatime; //!
   UInt_t   kDiffTime;

   ClassDef(TCatAtSaveMaxSamplePatternProcessor,1)
};
#endif // end of #ifdef TCATATSAVEMAXSAMPLEPATTERNPROCESSOR_H
