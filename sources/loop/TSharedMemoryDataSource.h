/* $Id:$ */
/**
 * @file   TSharedMemoryDataSource.h
 * @date   Created : Nov 29, 2013 21:29:10 JST
 *   Last Modified : Nov 30, 2013 00:58:50 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#ifndef TSHAREDMEMORYDATASOURCE_H
#define TSHAREDMEMORYDATASOURCE_H

#include <TDataSource.h>
#include <TSemaphoreObject.h>

namespace art {
   class TSharedMemoryDataSource;
   class TSharedMemory;
}

class art::TSharedMemoryDataSource  : public TDataSource {

public:
   TSharedMemoryDataSource(Int_t shmid, Int_t semkey, Int_t size);
   virtual ~TSharedMemoryDataSource();
   virtual Int_t IsPrepared();

   virtual Int_t Read(Char_t *buf, const Int_t &size);
   virtual Int_t Seek(Long_t offset, Int_t origin);
   virtual void Lock() { fSemaphore->Lock(); }
   virtual void Unlock() { fSemaphore->Unlock(); }
   

   virtual void Close();

protected:
   TSharedMemory *fSharedMemory;
   TSemaphoreObject    *fSemaphore;
   Int_t          fShmID;
   Int_t          fSemKey;
   Int_t          fSize;
   Int_t          fMemoryPos;
   
};
#endif // end of #ifdef TSHAREDMEMORYDATASOURCE_H
