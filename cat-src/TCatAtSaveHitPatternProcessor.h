/* $Id:$ */
/**
 * @file   TCatAtSaveHitPatternProcessor.h
 * @date   Created : 
 *   Last Modified : 2018-02-04 13:09:33 JST (ota)
 * @author 
 *  
 *  
 *    Copyright (C)2013
 */
#ifndef TCATATSAVEHITPATTERNPROCESSOR_H
#define TCATATSAVEHITPATTERNPROCESSOR_H

#include <TProcessor.h>
#include <TText.h>
#include <TRunInfo.h>
#include <TList.h>
#include <TDatime.h>

class TH2Poly;
class TCanvas;
class TClonesArray;

namespace art {
   class TCatAtSaveHitPatternProcessor;
   class TEventHeader;
}

class art::TCatAtSaveHitPatternProcessor  : public art::TProcessor {

public:
   TCatAtSaveHitPatternProcessor();
   ~TCatAtSaveHitPatternProcessor();

   void Init(art::TEventCollection *col);
   void Process();

   static TH2Poly* DrawPad(Int_t &nPads, Double_t *&padX, Double_t *&padY);

   StringVec_t fInputColName;
   //TString     fHistName;

   TH2Poly* fHPadEvt; //!
   TClonesArray **fInput; //!
   Int_t    fNPad;
   Double_t *fPadX; //!
   Double_t *fPadY; //!
   Int_t    fWaitTime;
   Int_t    fEvt;

   TCanvas      *fCanvas; //!
   Int_t        fCanvasX;
   Int_t        fCanvasY;
   Int_t        fCanvasH;
   Int_t        fCanvasW;
   TText        *t; //!

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

   TString      fNameRunHeaders;
   TList        **fRunHeaders; //!
   TRunInfo     *fRunInfo;
   TString      fRunName;
   Int_t        fRunNumber;
   TString  fNameEventHeader;
   TEventHeader **fEventHeader; //!
   TDatime      *fDatime;
   UInt_t       kDiffTime;

   ClassDef(TCatAtSaveHitPatternProcessor,1)
};
#endif // end of #ifdef TCATATSAVEHITPATTERNPROCESSOR_H
