/* $Id:$ */
/**
 * @file   TTimerProcessor.h
 * @date   Created : Jun 22, 2012 21:22:45 JST
 *   Last Modified : Feb 04, 2014 16:33:20 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#ifndef TTIMERPROCESSOR_H
#define TTIMERPROCESSOR_H

#include "TProcessor.h"
#include <TStopwatch.h>
namespace art {
   class TTimerProcessor;
}



class art::TTimerProcessor  : public TProcessor {

public:
   TTimerProcessor();
   ~TTimerProcessor();

   virtual void Init(TEventCollection*col);
   virtual void Process();
   
   virtual void PreLoop();
   virtual void PostLoop();

protected:
   Long_t fEventNumber;

   TStopwatch fStopwatch;
   Double_t fRealTime;
   Double_t fCpuTime;

   ClassDef(TTimerProcessor,1);
};
#endif // end of #ifdef TTIMERPROCESSOR_H
