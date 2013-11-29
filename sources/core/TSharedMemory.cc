/* $Id:$ */
/**
 * @file   TSharedMemory.cc
 * @date   Created : Nov 29, 2013 21:29:50 JST
 *   Last Modified : Nov 29, 2013 21:53:51 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#include "TSharedMemory.h"


#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>

using art::TSharedMemory;

TSharedMemory::TSharedMemory(int shmkey, int size)
   : fID(0), fKey(shmkey), fSize(size), fIsValid(false), fSharedMemory(0)
{
   if ((fID = shmget(shmkey, size, IPC_CREAT | 0777)) == -1) {
      SysError("TSharedMemory","Tried to create");
      fIsValid = false;
      return;
   }
   fSharedMemory = (char*)shmat(fID,0,0);
   fIsValid = true;
   /* bzero(fSharedMemory,0x4000); */
}

TSharedMemory::~TSharedMemory()
{
   if (fSharedMemory) Delete();
}


int TSharedMemory::Read(char *buf, int size, int offset)
{
   if (fSize < offset+size) {
      Error("Read","bound check required offset+size = %d for fSize(%d)",offset+size,fSize);
      return -1;
   }
   memcpy(buf,fSharedMemory+offset, size);
   return size;
}

int TSharedMemory::Delete()
{
   shmdt(fSharedMemory);
   if (shmctl(fID, IPC_RMID, 0) < 0) return 0; // error
   fSharedMemory = 0;
   return 1;
}
               
