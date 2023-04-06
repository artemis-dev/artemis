/* $Id:$ */
/**
 * @file   TCatAtHitPatternProcessor.h
 * @date   Created : Feb 09, 2013 15:09:47 JST
 *   Last Modified : 2018-03-08 10:03:31 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#ifndef TCATATHITPATTERNPROCESSOR_H
#define TCATATHITPATTERNPROCESSOR_H

#include <TProcessor.h>

class TH2Poly;
class TCanvas;
class TText;
class TClonesArray;
namespace art {
  class TCategorizedData;
  class TCatAtHitPatternProcessor;
   class TCatReadoutPadArray;
   class TEventHeader;
}

class art::TCatAtHitPatternProcessor  : public art::TProcessor {

public:
   TCatAtHitPatternProcessor();
   ~TCatAtHitPatternProcessor();

   void Init(art::TEventCollection *col);
   void Process();

   static TH2Poly* DrawPad(Int_t &nPads, Double_t *&padX, Double_t *&padY);

   StringVec_t fInputColName;
   TString     fHistName;
   //   TCanvas    *canvas;

   TH2Poly* fHPad; //!
   TH2Poly* fHPadEvt; //!
   TClonesArray **fFADC; //!
   Int_t    fNPad;
   Double_t *fPadX; //!
   Double_t *fPadY; //!
   Int_t    fWaitTime;
   Double_t fWdX;
   Double_t fWdY;
   

   TCanvas *fCanvas; //!
   TString fReadoutPadName;   
   TCatReadoutPadArray **fPads; //!
   TText   *fText;

   TString        fEventHeaderName;
   TEventHeader **fEventHeader; //!

   Long64_t fAnalyzed;
   
   ClassDef(TCatAtHitPatternProcessor,1)
};
#endif // end of #ifdef TCATATHITPATTERNPROCESSOR_H
