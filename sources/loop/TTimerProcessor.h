/* $Id:$ */
/**
 * @file   TTimerProcessor.h
 * @date   Created : Jun 22, 2012 21:22:45 JST
 *   Last Modified : 
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#ifndef TTIMERPROCESSOR_H
#define TTIMERPROCESSOR_H

#include "TProcessor.h"
namespace art {
   class TTimerProcessor;
}

class art::TTimerProcessor  : public TProcessor {

public:
   TTimerProcessor();
   ~TTimerProcessor();

   virtual void BeginOfRun();
   virtual void EndOfRun();

protected:

   ClassDef(TTimerProcessor,1);
};
#endif // end of #ifdef TTIMERPROCESSOR_H
