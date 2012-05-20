/* $Id:$ */
/**
 * @file   TCatSleepProcessor.h
 * @date   Created : Apr 27, 2012 19:27:30 JST
 *   Last Modified : May 19, 2012 15:59:35 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#ifndef TCATSLEEPPROCESSOR_H
#define TCATSLEEPPROCESSOR_H

#include "TCatProcessor.h"
#include "TCatOstream.h"

class TCatSleepProcessor  : public TCatProcessor {

public:
   TCatSleepProcessor();
   virtual ~TCatSleepProcessor();

   virtual void Process();


private:
   Int_t fSleepTime;

   ClassDef(TCatSleepProcessor,1);
};
#endif // end of #ifdef TCATSLEEPPROCESSOR_H
