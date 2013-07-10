/* $Id:$ */
/**
 * @file   TCatTimerProcessor.h
 * @date   Created : Jun 22, 2012 21:22:45 JST
 *   Last Modified : 
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#ifndef TCATTIMERPROCESSOR_H
#define TCATTIMERPROCESSOR_H

#include "TCatProcessor.h"

class TCatTimerProcessor  : public TCatProcessor {

public:
   TCatTimerProcessor();
   ~TCatTimerProcessor();

   virtual void BeginOfRun();
   virtual void EndOfRun();

protected:
};
#endif // end of #ifdef TCATTIMERPROCESSOR_H
