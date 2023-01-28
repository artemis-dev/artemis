/* $Id:$ */
/**
 * @file   TCounterEventStore.h
 * @date   Created : Jul 11, 2013 16:11:11 JST
 *   Last Modified : 2020-11-27 00:06:39 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#ifndef TCOUNTEREVENTSTORE_H
#define TCOUNTEREVENTSTORE_H

#include "TProcessor.h"
#include "TConditionBit.h"

namespace art {
   class TCounterEventStore;
}

class art::TCounterEventStore  : public TProcessor {

public:
   TCounterEventStore();
   ~TCounterEventStore();

   virtual void Init(TEventCollection *col);
   virtual void Process();

private:
   TConditionBit **fCondition; //!
   Int_t           fMaxLoop;
   Int_t           fNumLoop;


   ClassDef(TCounterEventStore,1);
};
#endif // end of #ifdef TCOUNTEREVENTSTORE_H
