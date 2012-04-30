/* $Id:$ */
/**
 * @file   TCatSleepProcessor.h
 * @date   Created : Apr 27, 2012 19:27:30 JST
 *   Last Modified : Apr 27, 2012 19:20:39 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#ifndef TCATSLEEPPROCESSOR_H
#define TCATSLEEPPROCESSOR_H

#include "TCatProcessor.h"

class TCatSleepProcessor  : public TCatProcessor {

public:
   TCatSleepProcessor();
   ~TCatSleepProcessor();

   virtual void Process();

};
#endif // end of #ifdef TCATSLEEPPROCESSOR_H
