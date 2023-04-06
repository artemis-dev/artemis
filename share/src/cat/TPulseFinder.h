/* $Id:$ */
/**
 * @file   TPulseFinder.h
 * @date   Created : Feb 02, 2014 13:02:42 JST
 *   Last Modified : 2021-02-10 14:18:56 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2014
 */
#ifndef TPULSEFINDER_H
#define TPULSEFINDER_H

#include <TProcessor.h>
#include "TTpcStaticGasProperty.h"
#include "TConverterBase.h"

namespace art {
   class TPulseFinder;

}
class TF1;
class TClonesArray;

class art::TPulseFinder  : public TProcessor {

public:
   TPulseFinder();
   ~TPulseFinder();

   virtual void Init(TEventCollection *col);
   virtual void Process();

protected:
   TClonesArray **fInput; //!
   TClonesArray  *fOutput; //!

   StringVec_t fInputColName;
   TString     fOutputColName;

   InputInfo<TTpcStaticGasProperty> fTpcGasProp;
   InputInfo<TClonesArray,TConverterBase> fGainShiftPrm;

   Bool_t      fSavePulse;
   Float_t fRisingThreshold;
   Float_t fFallingThreshold;
   Int_t   fMaxLength;
   Int_t   fPrePulse;
   Int_t   fPostPulse;
   Bool_t  fIsSparse;
   FloatVec_t fOffsetRange;
   Double_t fClockPeriod;
   Int_t fTime2Drift;
   Int_t fNumSampleForBaseline;
   Bool_t fDoKeepInvalid;

   Parameter<Int_t> fNumAvgPoints;

   TF1 *fFunChargeRiseCompensation; //!
   DoubleVec_t fChargeRiseCompensation;
   
   ClassDef(TPulseFinder,1);
};
#endif // end of #ifdef TPULSEFINDER_H
