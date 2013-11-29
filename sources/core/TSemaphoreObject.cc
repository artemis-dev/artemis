/* $Id:$ */
/**
 * @file   TSemaphoreObject.cc
 * @date   Created : Nov 29, 2013 21:29:49 JST
 *   Last Modified : Nov 29, 2013 23:50:39 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#include "TSemaphoreObject.h"

using art::TSemaphoreObject;

TSemaphoreObject::TSemaphoreObject(Int_t semkey)
   : fID(0), fKey(semkey), fIsValid(false)
{
   if ((fID = semget((key_t)semkey, 1, IPC_CREAT | 0666)) == -1) {
      SysError("TSemaphoreObject","Invalid");
      fIsValid = false;
      return;
   }
   fIsValid = true;
   fSemaphore.sem_num = 0;
   fSemaphore.sem_flg = SEM_UNDO;
}

TSemaphoreObject::~TSemaphoreObject()
{
}

/**
 * Lock semaphore
 */
void TSemaphoreObject::Lock()
{
   fSemaphore.sem_op = -1;
   semop(fID,&fSemaphore,1);
}

/**
 * Unlock semaphore
 */
void TSemaphoreObject::Unlock()
{
   fSemaphore.sem_op = 1;
   semop(fID,&fSemaphore,1);
}

