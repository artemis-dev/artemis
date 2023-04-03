/* $Id:$ */
/**
 * @file   TCatAtSiHitPatternProcessor.h
 * @date   Created : Feb 09, 2013 15:09:47 JST
 *         Modified : Oct 19, 2013 00:52:36 JST
 *    Last Modified : 2018-02-04 11:52:11 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 * @modifier Motonobu Takaki <takaki@cns.s.u-tokyo.ac.jp> 
 *  
 *    Copyright (C)2013
 */
#ifndef TCATATSIHITPATTERNPROCESSOR_H
#define TCATATSIHITPATTERNPROCESSOR_H

#include <TProcessor.h>

class TH2Poly;
class TCanvas;
class TClonesArray;
namespace art {
  class TCategorizedData;
  class TCatAtSiHitPatternProcessor;
}

class art::TCatAtSiHitPatternProcessor  : public art::TProcessor {

public:
  TCatAtSiHitPatternProcessor();
  ~TCatAtSiHitPatternProcessor();

  void Init(art::TEventCollection *col);
  void Process();
  
  static TH2Poly* DrawPad(Int_t &nPads, Double_t *&padX, Double_t *&padY,
			  Int_t &nStrips, Double_t *&StripX, Double_t *&StripY);

  StringVec_t fInputColNameAt;
  StringVec_t fInputColNameSi;
  TString     fHistName;
   TCanvas    *canvas; //!

   TH2Poly* fHPad; //!
   TH2Poly* fHPadEvt; //!
   TClonesArray **fFADCAT; //!
   TClonesArray **fFADCSI;  //!
  Int_t    fNPad;
   Double_t *fPadX; //!
   Double_t *fPadY;  //!
  Int_t    fNStrip;
   Double_t *fStripX; //!
   Double_t *fStripZ; //!
  
   ClassDef(TCatAtSiHitPatternProcessor,1);
  
};
#endif // end of #ifdef TCATATHITPATTERNPROCESSOR_H
