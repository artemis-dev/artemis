/**
 * @file   TCatAtRoughEventCounter.h
 * @brief  Lough-Event Counter for tracking detector
 *
 * @date   Created       : 2015-02-17 17:46:31 JST
 *         Last Modified : 2016-07-08 16:35:01 JST by tokieda
 * @author Hiroshi TOKIEDA <tokieda@cns.s.u-tokyo.ac.jp>
 *
 *    
 */

#ifndef TCATATLOUGHEVENTCOUNTER_H
#define TCATATLOUGHEVENTCOUNTER_H

#include <TProcessor.h>
#include "TPulseDrawer.h"
#include <TGraph.h>
#include <TF1.h>
#include <TH2F.h>
#include <TH2Poly.h>
#include <TCatReadoutPadArray.h>
#include <stdio.h>
#include <TCanvas.h>
#include <TText.h>
#include <TList.h>
#include <TRunInfo.h>

namespace art {
   class TCatAtRoughEventCounter;
   class TCatReaoutPadArray;
   class TEventHeader;
}

class art::TCatAtRoughEventCounter : public TProcessor {
public:
   TCatAtRoughEventCounter();
   virtual ~TCatAtRoughEventCounter();

   TCatAtRoughEventCounter(const TCatAtRoughEventCounter& rhs);
   TCatAtRoughEventCounter& operator=(const TCatAtRoughEventCounter& rhs);

   virtual void Init(TEventCollection *col);
   virtual void Process();

   static TH2Poly* DrawPad(Int_t &nPads, Double_t &padX, Double_t &padY);

protected:
   
private:
   FILE         *fp; //!->
   TClonesArray **fInput; //!
   TClonesArray *fOutput;
   TClonesArray *fOutputBeam;
   TClonesArray *fOutputRecoil;
   StringVec_t  fInputColName;
   TString      fOutputColName;
   TString      fHistName;
   Int_t        fEvt;
   Int_t        fWaitTime;
   TString      fLogFilePath;
   TString      fLogFileName;
   Bool_t       fDrawFig;
   Bool_t       fDrawPulse;
   Bool_t       fSaveFig;
   Bool_t       fDebug;
   Bool_t       fDetailDebug;
   Bool_t       fSaveHist;
   Bool_t       fNoColorFig;
   TString      fFigFile;
   TString      fFigFilePath;
   TString      fAddFigFilePath;
   Int_t        fSaveEvt;
   Int_t        fSelectCondition;
   Float_t      SetRecoilRange;
   Bool_t       fShowBeam;
   Bool_t       fShowRecoil;
   Float_t      fAngle_lowth;
   Float_t      fAngle_hith;
   Int_t        fCutRecoilHits;
   Bool_t       fLowPressure;

   TH1F         *fhAngle;
   TH2F         *fhAngleRecoilE;
   TH2F         *fhAngleRecoilE_ex;
   TH2F         *fhAngleRecoilTKE;
   TH2F         *fhAngleRecoilRange;
   TH1F         *fhCMAngle;
   TH1F         *fhEx;
   TH2F         *fhAngleEx;
   TH2F         *fhRecoilNRange;
   TH2F         *fhRecoilNAngle;

   Bool_t       fLSM; // Beam Fitting using 1: Least Square Method, 0: TGraph pol1
   TGraph       *fhBeamHits;
   TF1          *fhBeamRay;
   TGraph       *fhBeamYHits;
   TF1          *fhBeamYRay;
   TGraph       *fhRecoilHits;
   TF1          *fhRecoilRay;
   TGraph       *fhRecoilYHits;
   TF1          *fhRecoilYRay;

   TCanvas      *fCanvas;
   TCanvas      *fCanvas2;
   TText        *t;
   TH2Poly      *fHPadEvt;
   Int_t        fNPad;
   Double_t     fPadX;
   Double_t     fPadY;

   TCatReadoutPadArray *fPads;
   TString      fReadoutPadName;
   TH2Poly      *fhPad;

   TString      fNameRunHeaders;
   TList        **fRunHeaders; //!
   TRunInfo     *fRunInfo;
   TString      fRunName;
   Int_t        fRunNumber;
   TString  fNameEventHeader;
   TEventHeader **fEventHeader; //!

   TPulseDrawer *fPulseDrawer;
   Int_t        fCanvasX;
   Int_t        fCanvasY;
   Int_t        fCanvasH;
   Int_t        fCanvasW;

   Float_t      fDriftVelocity;
   Float_t      fBeamMass; // MeV
   Float_t      fBeamAtomicMass; // amu
   Float_t      fBeamVelocity; // MeV/u

   // [#825] for clustering beam and recoil with non continuous hits
   Bool_t fIgnoreDistanceToBeam; // flag to ignore distance between recoil and beam clusters
   Bool_t fUseAllRecoilCluster; // flag to use all the recoil clusters

   ClassDef(TCatAtRoughEventCounter,1) // rough-event counter for tracking detector
};

#endif // TCATATLOUGHEVENTCOUNTER_H
