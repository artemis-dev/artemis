/* $Id:$ */
/**
 * @file   TCatSiCommonNoiseSubtractionProcessor.h
 * @date   Created : Oct 27, 2013 17:39:00 JST
 * @author Motonobu Takaki <takaki@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#ifndef TCATSIPEDESTALSUBTRACTIONPPROCESSOR_H
#define TCATSIPEDESTALSUBTRACTIONPPROCESSOR_H

#include <TProcessor.h>
class TH2F;

namespace art {
  class TCategorizedData;
  class TCatSiPedestalSubtractionProcessor;
}
class TClonesArray;
class art::TCatSiPedestalSubtractionProcessor  : public art::TProcessor {
  
public:
  TCatSiPedestalSubtractionProcessor();
  ~TCatSiPedestalSubtractionProcessor();
  
  virtual void Init(art::TEventCollection *col);
  virtual void Process();
  
private:
  StringVec_t fInputColName;
  TString     fOutputColName;
   TClonesArray **fSiHit; //!
   TClonesArray *fSiHitOut; //!
   TH2F *fhSiPedMean; //!
   TH2F *fhSiPedSigma; //!
  


  ClassDef(TCatSiPedestalSubtractionProcessor,1);
  
};
#endif // end of #ifdef TCATSIPEDESTALSUBTRACTIONPROCESSOR_H
