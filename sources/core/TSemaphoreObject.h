/* $Id:$ */
/**
 * @file   TSemaphoreObject.h
 * @date   Created : Nov 29, 2013 21:29:29 JST
 *   Last Modified : 2017-06-06 22:12:55 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#ifndef TSEMAPHOREOBJECT_H
#define TSEMAPHOREOBJECT_H
#if defined (__CINT__)
#define _SYS_TYPES_H_
#endif

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#include <TObject.h>

namespace art {
   class TSemaphoreObject;
}

class art::TSemaphoreObject  : public TObject {

public:
   TSemaphoreObject(Int_t semkey);
   ~TSemaphoreObject();

   void Lock();
   void Unlock();
   bool IsValid() {return fIsValid; }
protected:
   int            fID;
   int            fKey;
   bool           fIsValid;
   //   union  semun   fSemUnion;
   struct sembuf  fSemaphore; //!
   
   ClassDef(TSemaphoreObject,1); // semaphore object
};
#endif // end of #ifdef TSEMAPHOREOBJECT_H
