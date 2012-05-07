/* $Id:$ */
/**
 * @file   TCatEventStore.h
 * @date   Created : May 04, 2012 00:04:47 JST
 *   Last Modified : 
 * @author ota <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#ifndef TCATEVENTSTORE_H
#define TCATEVENTSTORE_H

#include <TList.h>

class TArtEventStore;

class TCatEventStore  {
public:
   TCatEventStore();
   ~TCatEventStore();

   virtual bool   Open(Int_t shmid = 0);
   virtual bool   Open(const char* filename);
   virtual void   Close();
   virtual bool   GetNextEvent();
   virtual TList* GetListOfObjects() { return fObjects; }
   virtual void   Clear();
protected:
   TList   *fObjects;
   TArtEventStore *fArtEventStore;
   TCatEventStore *fCatEventStore;
};
#endif // end of #ifdef TCATEVENTSTORE_H
