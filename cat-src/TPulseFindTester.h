/* $Id:$ */
/**
 * @file   TPulseFindTester.h
 * @date   Created : 2015-03-29 17:41:26 JST
 *   Last Modified : 2015-05-19 17:30:00 JST by tokieda
 * @author Hiroshi TOKIEDA <tokieda@cns.s.u-tokyo.ac.jp>
 *  Added from share/src/cat/TPulseFinder.h   
 *  
 *    Copyright (C)2015
 */
#ifndef TPULSEFINDTESTER_H
#define TPULSEFINDTESTER_H

#include <TProcessor.h>
#include "TPulseDrawer.h"
#include <TClonesArray.h>
#include <TList.h>
#include <TRunInfo.h>
#include <TH2.h>

namespace art {
   class TPulseFindTester;
}

class TClonesArray;

class art::TPulseFindTester  : public TProcessor {

public:
   TPulseFindTester();
   ~TPulseFindTester();

   virtual void Init(TEventCollection *col);
   virtual void Process();

protected:
   TClonesArray **fInput; //!
   TClonesArray  *fOutput;

   StringVec_t fInputColName;
   TString     fOutputColName;

   Bool_t      fSavePulse;
   Float_t fRisingThreshold;
   Float_t fFallingThreshold;
   Int_t   fMaxLength;
   Int_t   fPrePulse;
   Int_t   fPostPulse;
   Bool_t  fIsSparse;
   FloatVec_t fOffsetRange;
   
   Int_t        fEvt;
   Bool_t       fDebug;
   Bool_t       fDetailDebug;
   FILE         *fp; //!->
   TString      fLogFilePath;
   TString      fLogFileName;
   Bool_t       fSaveHist;
   Bool_t       fDrawFig;
   Bool_t       fSaveFig;
   Int_t        fCanvasX;
   Int_t        fCanvasY;
   Int_t        fCanvasH;
   Int_t        fCanvasW;
   Int_t        fWaitTime;
   TString      fFigFile;
   TString      fFigFilePath;
   TString      fAddFigFilePath;
   Int_t        fSaveEvt;
   Int_t        fSaveHit;
   Int_t        fSaveID;
   Bool_t       fCutNegativeCharge;
   Float_t      fCutLowCharge;
   Float_t      fCutHighCharge;
   Int_t        fNChargeZeroHits;

   TString      fNameRunHeaders;
   TList        **fRunHeaders; //!
   TRunInfo     *fRunInfo;
   TString      fRunName;
   Int_t        fRunNumber;

   TPulseDrawer *fPulseDrawer;

   Float_t      fCutLowOffset;
   Float_t      fCutHighOffset;
   Float_t      fGraphXmin;
   Float_t      fGraphXmax;
   Float_t      fGraphYmin;
   Float_t      fGraphYmax;

   TH2F         *fhYAbsYAbsY;
   TH2F         *fhSigmaYAbsY;
   TH2F         *fhSigmaYAbsY2;
   TH1F         *fhCharge;
   TH1F         *fhOffset;
   TH2F         *fhOffsetCharge;
   TH2F         *fhSigmaCharge;

   StringVec_t  fSigmaColName;
   TClonesArray **fSigma; //!
   
   TGraph       *fhGraph2;

   ClassDef(TPulseFindTester,1);
};
#endif // end of #ifdef TPULSEFINDTESTER_H
