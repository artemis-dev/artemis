/**
 * @file   TBeamZProcessor.h
 * @brief
 *
 * @date   Created:       2014-02-24 17:31:09
 *         Last Modified: 2014-03-25 17:44:31 JST (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2014 KAWASE Shoichiro All rights reserved
 */

#ifndef TBEAMZPROCESSOR_H
#define TBEAMZPROCESSOR_H

#include <TProcessor.h>

namespace art {
   namespace sh04 {
      class TBeamZProcessor;
   }
}

class TClonesArray;

class art::sh04::TBeamZProcessor : public TProcessor {
public:
   // Default constructor
   TBeamZProcessor();
   virtual ~TBeamZProcessor();

   void Init(TEventCollection *col);
   void Process();

protected:
   /* input */
   TString        fTrackLName;
   TClonesArray **fTrackL; //!
   TString        fTrackRName;
   TClonesArray **fTrackR; //!

   /* parameter */
   TString  fGeometryName;
   Double_t fDistance[2];
   Double_t fAngle[2];

   /* output */
   TString       fOutputColName;
   TClonesArray *fBeamZ; //!

   /* switch */
   Int_t fAveragingMethod;

private:
   void CalcBeamZ(Double_t, Double_t, Double_t, Double_t, Double_t, Double_t,
		  Double_t*, Double_t*) const;
   Double_t CalcAverage(Double_t, Double_t, Double_t, Double_t);
   
// Copy constructor (prohibited)
   TBeamZProcessor(const TBeamZProcessor&);
   // Assignment operator (prohibited)
   TBeamZProcessor& operator=(const TBeamZProcessor&);

   ClassDef(TBeamZProcessor,0) // processor for BeamZ calculation
};

#endif // TBEAMZPROCESSOR_H
