/* $Id:$ */
/**
 * @file   TPulseMovingAverageProcessor.h
 * @date   Created : Feb 02, 2014 13:02:42 JST
 *   Last Modified : 2018-02-04 12:07:47 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2014
 */
#ifndef TPULSEMOVINGAVERAGEPROCESSOR_H
#define TPULSEMOVINGAVERAGEPROCESSOR_H

#include <TProcessor.h>

namespace art {
   class TPulseMovingAverageProcessor;
}

class TClonesArray;

class art::TPulseMovingAverageProcessor  : public TProcessor {

public:
   TPulseMovingAverageProcessor();
   ~TPulseMovingAverageProcessor();

   virtual void Init(TEventCollection *col);
   virtual void Process();

protected:
   TClonesArray **fInput; //!
   TClonesArray  *fOutput; //!

   TString fInputColName;
   TString     fOutputColName;

   Int_t fNumAverage;
   
   ClassDef(TPulseMovingAverageProcessor,1);
};
#endif // end of #ifdef TPULSEFINDER_H
