/* $Id:$ */
/**
 * @file   TCatEventCountProcessor.h
 * @date   Created : May 16, 2012 11:16:19 JST
 *   Last Modified : May 16, 2012 11:21:15 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#ifndef TCATEVENTCOUNTPROCESSOR_H
#define TCATEVENTCOUNTPROCESSOR_H

#include <TCatProcessor.h>

class TCatEventCountProcessor  : public TCatProcessor {

public:
   TCatEventCountProcessor();
   ~TCatEventCountProcessor();

   virtual void Process();
   virtual void EndOfRun();

private:
   Int_t fEvtNum;
};
#endif // end of #ifdef TCATEVENTCOUNTPROCESSOR_H
