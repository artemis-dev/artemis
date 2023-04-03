/* $Id:$ */
/**
 * @file   TCatSimpleTrackingByChargeDivision.h
 * @date   Created : Aug 25, 2016 16:25:54 JST
 *   Last Modified : 2020-05-27 15:03:11 JST (ota)
 * @author Hiroshi TOKIEDA <tokieda@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2016
 */
#ifndef TCATSIMPLETRACKINGBYCHARGEDIVISION_H
#define TCATSIMPLETRACKINGBYCHARGEDIVISION_H

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
   class TCatSimpleTrackingByChargeDivision;
   class TCatReaoutPadArray;
   class TEventHeader;
}

class art::TCatSimpleTrackingByChargeDivision  : public TProcessor {

public:
   TCatSimpleTrackingByChargeDivision();
   virtual ~TCatSimpleTrackingByChargeDivision();

   TCatSimpleTrackingByChargeDivision(const TCatSimpleTrackingByChargeDivision& rhs){}
   TCatSimpleTrackingByChargeDivision& operator=(const TCatSimpleTrackingByChargeDivision& rhs){return *this;}

   virtual void Init(TEventCollection *col);
   virtual void Process();

   static TH2Poly* DrawPad(Int_t &nPads, Double_t &padX, Double_t &padY);

protected:
   
private:
   FILE         *fp; //!->
   TClonesArray **fInput; //!
   TClonesArray **fInputBeam; //!
   TClonesArray *fOutput;
   TClonesArray *fOutputBeam;
   TClonesArray *fOutputRecoil;
   StringVec_t  fInputColName;
   StringVec_t  fInputBeamColName;
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

   TH1F         *fhLSMXZDistance;
   TH1F         *fhLSMXYDistance;
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

   ClassDef(TCatSimpleTrackingByChargeDivision,1) // rough-event counter for tracking detector
};

#endif // end of #ifdef TCATSIMPLETRACKINGBYCHARGEDIVISION_H
