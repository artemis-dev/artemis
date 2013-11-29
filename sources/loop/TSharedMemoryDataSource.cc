/* $Id:$ */
/**
 * @file   TSharedMemoryDataSource.cc
 * @date   Created : Nov 29, 2013 21:29:51 JST
 *   Last Modified : Nov 30, 2013 00:02:29 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#include "TSharedMemoryDataSource.h"

#include <TSharedMemory.h>
#include <TSemaphoreObject.h>

using art::TSharedMemoryDataSource;

TSharedMemoryDataSource::TSharedMemoryDataSource(Int_t shmid, Int_t semkey, Int_t size)
   : fSharedMemory(NULL), fSemaphore(NULL),
     fShmID(shmid), fSemKey(semkey), fSize(size)
{
   fSharedMemory = new TSharedMemory(shmid,size);
   if (!fSharedMemory->IsValid()) {
      Close();
      return;
   }
   fSemaphore = new TSemaphoreObject(semkey);
   if (!fSemaphore->IsValid()) {
      Close();
      return;
   }
   fMemoryPos = 0;
   return;
}
TSharedMemoryDataSource::~TSharedMemoryDataSource()
{
}


void TSharedMemoryDataSource::Close()
{
   if (fSharedMemory) {
      delete fSharedMemory;
      fSharedMemory = 0;
   }
   if (fSemaphore) {
      delete fSemaphore;
      fSemaphore = 0;
   }
}

Int_t TSharedMemoryDataSource::Seek(Long_t offset, Int_t origin)
{
   switch (origin) {
   case SEEK_SET:
      fMemoryPos = offset;
      break;
   case SEEK_CUR:
      fMemoryPos += offset;
      break;
   case SEEK_END:
      fMemoryPos = fSharedMemory->GetSize() - offset;
      break;
   default:
      return -1;
   }
   return 0;
}

Int_t TSharedMemoryDataSource::Read(Char_t *buf, const Int_t& size)
{
   int len = fSharedMemory->Read(buf,size,fMemoryPos);
   fMemoryPos += size;
   return len;
}

Int_t TSharedMemoryDataSource::IsPrepared()
{
   if (fSharedMemory && fSemaphore) return kTRUE;
   return kFALSE;
}
