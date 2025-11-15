/* $Id:$ */
/**
 * @file   TCatSiCommonNoiseSubtractionProcessor.h
 * @date   Created : Oct 26, 2013 17:39:00 JST
 * @author Motonobu Takaki <takaki@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#ifndef TCATSICOMMONNOISESUBTRACTIONPROCESSOR_H
#define TCATSICOMMONNOISESUBTRACTIONPROCESSOR_H

#include <TProcessor.h>
class TH2F;

namespace art {
  class TCategorizedData;
  class TCatSiCommonNoiseSubtractionProcessor;
}
class TClonesArray;
class art::TCatSiCommonNoiseSubtractionProcessor  : public art::TProcessor {

public:
  TCatSiCommonNoiseSubtractionProcessor();
  ~TCatSiCommonNoiseSubtractionProcessor();

  virtual void Init(art::TEventCollection *col);
  virtual void Process();

private:
  StringVec_t fInputColName;
  TString     fOutputColName;
   TClonesArray **fSiHit; //!
   TClonesArray *fSiHitOut; // !



  ClassDef(TCatSiCommonNoiseSubtractionProcessor,1);
};
#endif // end of #ifdef TCATSICOMMONNOISESUBTRACTIONPROCESSOR_H

