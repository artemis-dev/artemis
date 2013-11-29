/* $Id:$ */
/**
 * @file   TSharedMemory.h
 * @date   Created : Nov 29, 2013 21:29:50 JST
 *   Last Modified : Nov 29, 2013 21:53:14 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#ifndef TSHAREDMEMORY_H
#define TSHAREDMEMORY_H

#include <TObject.h>

namespace art {
   class TSharedMemory;
}

class art::TSharedMemory  : public TObject {

public:
   TSharedMemory(int shmkey, int size);
   ~TSharedMemory();

   int GetSize() { return fSize; }
   int Read(char *buf, int size, int offset = 0);
   int  Delete();
   bool IsValid() { return fIsValid; }
protected:
   int   fID;     // shared memory id
   int   fKey;    // shared memmory key
   int   fSize;   // shared memory size
   bool  fIsValid;// flag for validity of shared memory
   char* fSharedMemory; // pointer to shared memory       
   ClassDef(TSharedMemory,1); // shared memory wrapper class
};
#endif // end of #ifdef TSHAREDMEMORY_H
