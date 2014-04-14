/* $Id:$ */
/**
 * @file   TSemaphoreObject.h
 * @date   Created : Nov 29, 2013 21:29:29 JST
 *   Last Modified : Nov 29, 2013 23:50:21 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#ifndef TSEMAPHOREOBJECT_H
#define TSEMAPHOREOBJECT_H
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
