/* $Id:$ */
/**
 * @file   TPulseUnfoldLowPassProcessor.h
 * @date   Created : Feb 01, 2014 17:01:55 JST
 *   Last Modified : 2018-02-04 12:06:59 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2014
 */
#ifndef TPULSEUNFOLDLOWPASSPROCESSOR_H
#define TPULSEUNFOLDLOWPASSPROCESSOR_H

#include <TProcessor.h>

namespace art {
   class TPulseUnfoldLowPassProcessor;
}
class TClonesArray;

class art::TPulseUnfoldLowPassProcessor  : public TProcessor {

public:
   TPulseUnfoldLowPassProcessor();
   ~TPulseUnfoldLowPassProcessor();

   virtual void Init(TEventCollection *col);
   virtual void Process();

protected:
   StringVec_t fInputColName;
   TString     fOutputColName;
   Float_t     fTimeConstant;

   TClonesArray **fInput; //!
   TClonesArray  *fOutput; //!
   
   ClassDef(TPulseUnfoldLowPassProcessor,1);

};
#endif // end of #ifdef TPULSELOWPASSFILTERPROCESSOR_H
