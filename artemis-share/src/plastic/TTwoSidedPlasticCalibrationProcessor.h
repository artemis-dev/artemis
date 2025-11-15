/**
 * @file   TTwoSidedPlasticCalibrationProcessor.h
 * @brief
 *
 * @date   Created:       2013-10-10 15:34:10
 *         Last Modified: 2014-06-23 14:26:07 JST (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved
 */

#ifndef TTWOSIDEDPLASTICCALIBRATIONPROCESSOR_H
#define TTWOSIDEDPLASTICCALIBRATIONPROCESSOR_H

#include <TProcessor.h>
#include <vector>

namespace art{
   class TTwoSidedPlasticCalibrationProcessor;
   class TConverterBase;
   class TMultiHitArray;
}

class TClonesArray;

class art::TTwoSidedPlasticCalibrationProcessor : public TProcessor {

public:
   TTwoSidedPlasticCalibrationProcessor();
   virtual ~TTwoSidedPlasticCalibrationProcessor();

   virtual void Init(TEventCollection *col);
   virtual void Process();

protected:
   TString        fInputColName1; //name of input collection of 1st side
   TString        fInputColName2; //name of input collection of 2st side
   TString        fOutputColName; //name of output collection
   TClonesArray **fPlasticIn1; //!input collection of 1st side
   TClonesArray **fPlasticIn2; //!input collection of 2nd side
   TClonesArray  *fPlasticOut; //!output collection

   IntVec_t        fDetIDRange;
   Int_t           fNDet;       // number of detectors
   TMultiHitArray *fHits1; //!
   TMultiHitArray *fHits2; //!

   static const Int_t HITID_ORIGIN = 1;

   std::vector<TConverterBase*> *fPosConverterArray; //!
   TString fPosConverterArrayName;
   Int_t fPosConverterArraySize;

   Int_t fQDiffType;

   FloatVec_t fValidTimeRange;
   Float_t fValidTimeMin;
   Float_t fValidTimeMax;

private:
   // Copy constructor (prohibited)
   TTwoSidedPlasticCalibrationProcessor(const TTwoSidedPlasticCalibrationProcessor&);
   // Assignment operator (prohibited)
   TTwoSidedPlasticCalibrationProcessor& operator=(const TTwoSidedPlasticCalibrationProcessor&);

   void FillHitArray(Int_t);

   void ProcessByIndex();
   void ProcessByTimeDiff();

   ClassDef(TTwoSidedPlasticCalibrationProcessor,0) // processor for two-sided plastic calibration
};

#endif // TTWOSIDEDPLASTICCALIBRATIONPROCESSOR_H
