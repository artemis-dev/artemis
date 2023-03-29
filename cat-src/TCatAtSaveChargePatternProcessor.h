/* $Id:$ */
/**
 * @file   TCatAtSaveChargePatternProcessor.h
 * @date   Created : 2015-05-09 12:49:05 JST
 *   Last Modified : 2016-05-15 03:31:15 JST by tokieda
 * @author Hiroshi TOKIEDA <tokieda@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2015
 */
#ifndef TCATATSAVECHARGEPATTERNPROCESSOR_H
#define TCATATSAVECHARGEPATTERNPROCESSOR_H

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
  class TCatAtSaveChargePatternProcessor;
}

class art::TCatAtSaveChargePatternProcessor  : public art::TProcessor {

public:
   TCatAtSaveChargePatternProcessor();
   ~TCatAtSaveChargePatternProcessor();

   void Init(art::TEventCollection *col);
   void Process();

   StringVec_t fInputColName;   
   TClonesArray **fInput;
   Int_t    fNPad;
   Double_t *fPadX;
   Double_t *fPadY;
   Int_t    fWaitTime;
   Int_t    fEvt;

   TCanvas  *fCanvas;
   Int_t    fCanvasX;
   Int_t    fCanvasY;
   Int_t    fCanvasH;
   Int_t    fCanvasW;
   TText    *t;
   TString fReadoutPadName;
   TCatReadoutPadArray** fPadArray;
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
   TRunInfo *fRunInfo;
   TString  fRunName;
   Int_t    fRunNumber;
   TDatime  *fDatime;
   UInt_t   kDiffTime;

   ClassDef(TCatAtSaveChargePatternProcessor,1)
};
#endif // end of #ifdef TCATATSAVECHARGEPATTERNPROCESSOR_H
