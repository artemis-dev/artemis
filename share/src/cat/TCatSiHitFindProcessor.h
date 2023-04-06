/* $Id:$ */
/**
 * @file   TCatSiHitFindProcessor.h
 * @date   Created : Oct 28, 2013 19:03:32 JST
 *
 * @author Motonobu Takaki <takaki@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 *
 */
#ifndef TCATSIHITFINDPROCESSOR_H
#define TCATSIHITFINDPROCESSOR_H

#include <TProcessor.h>

namespace art {
  class TCategorizedData;
  class TCatSiHitFindProcessor;
}
class TClonesArray;
class art::TCatSiHitFindProcessor  : public art::TProcessor {
  
public:
  TCatSiHitFindProcessor();
  ~TCatSiHitFindProcessor();

  virtual void Init(art::TEventCollection *col);
  virtual void Process();
  
private:
  StringVec_t fInputColName;
  TString     fOutputColName;
   TClonesArray **fSiHit; //!
   TClonesArray *fSiHitOut;//!


  ClassDef(TCatSiHitFindProcessor,1);
};
#endif // end of #ifdef TCATHITFINDPROCESSOR_H

  
