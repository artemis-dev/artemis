/* $Id:$ */
/**
 * @file   TCatPulseBitCorrectionProcessor.h
 * @date   Created : Nov 01, 2013 06:01:14 JST
 *   Last Modified : 2018-02-04 11:50:36 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#ifndef TCATPULSEBITCORRECTIONPROCESSOR_H
#define TCATPULSEBITCORRECTIONPROCESSOR_H

#include <TProcessor.h>

namespace art {
   class TCatPulseBitCorrectionProcessor;
}
class TClonesArray;

class art::TCatPulseBitCorrectionProcessor  : public art::TProcessor {

public:
   TCatPulseBitCorrectionProcessor();
   virtual ~TCatPulseBitCorrectionProcessor();

   virtual void Init(TEventCollection* col);
   virtual void Process();


protected:
   StringVec_t fInputColName;

   TClonesArray **fInput; //!

   Float_t fMaxADC;

   ClassDef(TCatPulseBitCorrectionProcessor,1);
};
#endif // end of #ifdef TCATPULSEBITCORRECTIONPROCESSOR_H
