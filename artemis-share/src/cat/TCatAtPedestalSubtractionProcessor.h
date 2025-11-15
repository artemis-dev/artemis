/* $Id:$ */
/**
 * @file   TCatAtPedestalSubtractionProcessor.h
 * @date   Created : Mar 06, 2013 18:06:53 JST
 *   Last Modified : 2018-02-21 13:50:26 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#ifndef TCATATPEDESTALSUBTRACTIONPROCESSOR_H
#define TCATATPEDESTALSUBTRACTIONPROCESSOR_H

#include <TProcessor.h>
class TH2F;
class TClonesArray;
namespace art {
  class TCategorizedData;
  class TCatAtPedestalSubtractionProcessor;
}

class art::TCatAtPedestalSubtractionProcessor  : public art::TProcessor {

public:
   TCatAtPedestalSubtractionProcessor();
   ~TCatAtPedestalSubtractionProcessor();

   virtual void Init(art::TEventCollection *col);
   virtual void Process();

private:
   StringVec_t fInputColName;
   TString     fOutputColName;
   TClonesArray **fCATHit; //!
   TClonesArray *fCATHitOut; //!

   TH2F *fhPedMean; //!
   TH2F *fhPedSigma; //!

   Int_t fDoInvertPulse; // flag for pulse inversion: default 0 (not inverted)

   ClassDef(TCatAtPedestalSubtractionProcessor,1);
};
#endif // end of #ifdef TCATATPEDESTALSUBTRACTIONPROCESSOR_H
