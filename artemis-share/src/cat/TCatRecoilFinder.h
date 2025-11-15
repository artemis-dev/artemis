/* $Id:$ */
/**
 * @file   TCatRecoilFinder.h
 * @date   Created : Nov 01, 2013 22:01:47 JST
 *   Last Modified : 2018-02-04 12:01:18 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#ifndef TCATRECOILFINDER_H
#define TCATRECOILFINDER_H

#include <TProcessor.h>

class TH2Poly;
class TH2F;
class TCanvas;
class TClonesArray;
namespace art {
   class TCatRecoilFinder;
}

class art::TCatRecoilFinder  : public art::TProcessor {

public:
   TCatRecoilFinder();
   virtual ~TCatRecoilFinder();

private:
   virtual void Init(TEventCollection *col);
   virtual void Process();
   virtual void PostLoop();

   TH2Poly * DrawPad(Int_t &npad, Double_t *&padx, Double_t *&pady);

   StringVec_t fInputColName;
   TClonesArray **fInput;  //!

   Int_t    fNPad; //!
   Double_t *fPadX; //!
   Double_t *fPadY; //!
   Int_t    fWaitTime;
   TCanvas *fCanvas; //!
   TH2F    *fHCharge; //!
   TH2Poly *fHPad; //!
   TH2Poly *fHPadTime; //!
   TH2Poly *fHPadCharge; //!
   Int_t    fNumEvt;
   
   ClassDef(TCatRecoilFinder,1);
};
#endif // end of #ifdef TCATRECOILFINDER_H
