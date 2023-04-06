/**
 * @file   TPIDGateProcessor.h
 * @brief
 *
 * @date   Created:       2014-01-15 13:49:31
 *         Last Modified: 2014-03-19 14:24:39 JST (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2014 KAWASE Shoichiro All rights reserved
 */

#ifndef TPIDGATEPROCESSOR_H
#define TPIDGATEPROCESSOR_H

#include <TProcessor.h>

#include <ITiming.h>

namespace art {
   class TPIDGateProcessor;
   class TGateObject;
   class TEventCollection;
}

class TClonesArray;
class TH2D;
class TGraph;

class art::TPIDGateProcessor : public TProcessor {
public:
   // Default constructor
   TPIDGateProcessor();
   virtual ~TPIDGateProcessor();

   void Init(TEventCollection *col);
   void Process();

protected:
   /* input */
   TString        fTimingColName;
   Int_t          fTimingDetID;
   TClonesArray **fTimingArray;
   TString        fChargeColName;
   Int_t          fChargeDetID;
   TClonesArray **fChargeArray;

   /* output */
   TGateObject  **fGate;

   /* rectangle gate */
   StringVec_t    fRectangleParList;
   Int_t          fNRectGate;
   Double_t      *fTMin;
   Double_t      *fTMax;
   Double_t      *fQMin;
   Double_t      *fQMax;

   /* polygon gate */
   StringVec_t    fPolygonParList;
   Int_t          fNPolyGate;
   TGraph       **fGraph;

   /* output histogram */
   StringVec_t    fHistOption;
   TH2D          *fH;
   TH2D         **fHGated;

private:
   // Copy constructor (prohibited)
   TPIDGateProcessor(const TPIDGateProcessor&);
   // Assignment operator (prohibited)
   TPIDGateProcessor& operator=(const TPIDGateProcessor&);

   ClassDef(TPIDGateProcessor,0)
};

#endif // TPIDGATEPROCESSOR_H
