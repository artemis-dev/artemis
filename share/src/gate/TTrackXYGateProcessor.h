/**
 * @file   TTrackXYGateProcessor.h
 * @brief
 *
 * @date   Created:       2014-01-30 11:05:46
 *         Last Modified: 2015-04-10 15:16:02 JST (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2014 KAWASE Shoichiro All rights reserved
 */

#ifndef TTRACKXYGATEPROCESSOR_H
#define TTRACKXYGATEPROCESSOR_H

#include <TProcessor.h>

namespace art {
   class TTrackXYGateProcessor;
   class TGateObject;
}

class TClonesArray;
class TH2D;
class TGraph;

class art::TTrackXYGateProcessor : public TProcessor {
public:
   // Default constructor
   TTrackXYGateProcessor();
   virtual ~TTrackXYGateProcessor();

   void Init(TEventCollection *col);
   void Process();

protected:
   /* input */
   TString        fTrackColName;
   TClonesArray **fTrackArray;

   /* output */
   TGateObject  **fGate;

   /* circle gate */
   StringVec_t    fCircleParList;
   Int_t          fNCircle;
   Double_t      *fCenterX;
   Double_t      *fCenterY;
   Double_t      *fRadius;

   /* polygon gate */
   StringVec_t    fPolygonParList;
   Int_t          fNPolygon;
   TGraph       **fGraph;

   /* output histogram */
   StringVec_t    fHistOption;
   TH2D          *fH;
   TH2D         **fHGated;

private:
   // Copy constructor (prohibited)
   TTrackXYGateProcessor(const TTrackXYGateProcessor&);
   // Assignment operator (prohibited)
   TTrackXYGateProcessor& operator=(const TTrackXYGateProcessor&);

   Bool_t CheckHistOption() const;

   ClassDef(TTrackXYGateProcessor,0)
};

#endif // TTRACKXYGATEPROCESSOR_H
