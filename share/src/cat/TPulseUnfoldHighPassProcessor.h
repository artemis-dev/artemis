/* $Id:$ */
/**
 * @file   TPulseUnfoldHighPassProcessor.h
 * @date   Created : Jan 31, 2014 13:31:47 JST
 *   Last Modified : Feb 01, 2014 19:04:03 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2014
 */
#ifndef TPULSEUNFOLDHIGHPASSPROCESSOR_H
#define TPULSEUNFOLDHIGHPASSPROCESSOR_H

#include <TProcessor.h>

namespace art {
   class TPulseUnfoldHighPassProcessor;
}

class TClonesArray;
class TString;

class art::TPulseUnfoldHighPassProcessor  : public TProcessor {

public:
   TPulseUnfoldHighPassProcessor();
   ~TPulseUnfoldHighPassProcessor();

   virtual void Init(TEventCollection *col);
   virtual void Process();

protected:
   StringVec_t  fInputColName;

   TClonesArray **fInput; //!

   Float_t fTimeConstant; 
   
   ClassDef(TPulseUnfoldHighPassProcessor,1);
};
#endif // end of #ifdef TPULSEUNFOLDHIGHPASSPROCESSOR_H
