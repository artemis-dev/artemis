/* $Id:$ */
/**
 * @file   TCatEventStore.h
 * @date   Created : May 04, 2012 00:04:47 JST
 *   Last Modified : May 12, 2012 09:30:43 JST
 * @author ota <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#ifndef TCATEVENTSTORE_H
#define TCATEVENTSTORE_H

#include <TList.h>
#include <list>
using namespace std;

class TCatEventStore  {
public:

   typedef enum {kIdle, kRunning, kEOF} EStatus_t;
   TCatEventStore();
   virtual ~TCatEventStore();
   virtual Bool_t IsPrepared() { return kFALSE; }
   virtual Bool_t IsBeginOfRun() {return kTRUE; }
   virtual Bool_t GetNextEvent() {return kFALSE;}
   virtual TList* GetListOfObjects() { return fObjects; }
   virtual void   Clear() {;}
protected:
   EStatus_t     fStatus;

   TList   *fObjects;
};
#endif // end of #ifdef TCATEVENTSTORE_H
