/**
 * @file   TSMWDCTrackingProcessor.h
 * @brief
 *
 * @date   Created:       2013-10-19 16:59:46
 *         Last Modified: 2015-03-25 14:11:59 JST (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved
 */

#ifndef TSMWDCTRACKINGPROCESSOR_H
#define TSMWDCTRACKINGPROCESSOR_H

#include <TProcessor.h>

#include <vector>

namespace art {
   namespace sh04 {
      class TSMWDCTrackingProcessor;
   }
   class TMWDCPlaneInfo;
   class TMWDCHitData;
   class TMWDCTrackingResult;
   class TMultiHitCombinator;
}

class TClonesArray;
class TStopwatch;
template <typename T> class TMatrixT;
typedef TMatrixT<double> TMatrixD;

class art::sh04::TSMWDCTrackingProcessor : public TProcessor {
public:
   // Default constructor
   TSMWDCTrackingProcessor();
   virtual ~TSMWDCTrackingProcessor();

   virtual void Init(TEventCollection *col);
   virtual void Process();

protected:
   StringVec_t                   fInputColName;
   TString                       fOutputColName;
   std::vector<TClonesArray**>  *fPlaneIn; //!
   std::vector<TMWDCPlaneInfo*> *fPlaneInfo; //!
   TClonesArray                 *fTrackingResult;
   Int_t                         fNPlane;
   TMWDCHitData          const **fPlaneData; //!
   Int_t                         fUseLocalPos; // obsolete

   Double_t *fCos; //[fNPlane]
   Double_t *fSin; //[fNPlane]
   Double_t *fZ;   //[fNPlane]
   Double_t *fX;   //[fNPlane]
   Double_t *fY;   //[fNPlane]

   Int_t     fNPlaneValid;
   Bool_t    fUseAllCombination;
   Int_t    *fWeight; //[fNPlane]
   Double_t *fDLOriginal; //[fNPlane]
   Double_t *fDLCorrected; //[fNPlane]

   Int_t     fNIteration;
   Float_t   fSignificanceLevel;

   Float_t  fSSRMax;

   TStopwatch *fStopwatch; //!

   FloatVec_t fTimingGate;
   FloatVec_t fChargeGate;

   FloatVec_t fAGate;
   FloatVec_t fBGate;

   TMultiHitCombinator *fCombinator;
   Int_t fPlaneCombinationID;

   TMatrixD **fGMatrix;         //!
   TMatrixD **fHMatrix;         //!
   TMatrixD **fCovariantMatrix; //!

   void GenerateMatrix();
   void FindTrack(TMWDCTrackingResult *tr, Bool_t fillStat);
   Double_t CalcSSR(const std::vector<Double_t> &pos,
		    const std::vector<Double_t> &trackPrm,
		    std::vector<Double_t> &residual) const;

   static const Int_t CRITICAL_NPLANE = 5;
   static const Int_t NPARAMETER = 4;

private:
   void IncidentAngleCorrection(TMWDCTrackingResult*);
   Bool_t CombinationIsValid() const;
   Bool_t DataIsValid(const TMWDCHitData*) const;
   Bool_t ParametersAreValid(const TMWDCTrackingResult*) const;
   void FillCombinator();
   Int_t FindBestCombination(TMWDCTrackingResult *tr);

   // Copy constructor (prohibited)
   TSMWDCTrackingProcessor(const TSMWDCTrackingProcessor&);
   // Assignment operator (prohibited)
   TSMWDCTrackingProcessor& operator=(const TSMWDCTrackingProcessor&);

   ClassDef(TSMWDCTrackingProcessor,0) // processor for tracking using SMWDC in SHARAQ04
};

#endif // TSMWDCTRACKINGPROCESSOR_H
