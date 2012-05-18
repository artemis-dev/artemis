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

class TArtEventStore;

class TCatEventStore  {
public:

   typedef enum {kIdle, kRunning, kEOF} EStatus_t;

   TCatEventStore();
   virtual ~TCatEventStore();


   virtual bool   AddInputFile(const char *filename);
   virtual bool   IsPrepared();
   virtual Bool_t IsBeginOfRun();
   virtual void   Close();
   virtual bool   Open(Int_t shmid = 0);
   virtual bool   Open(const char* filename);
   virtual bool   GetNextEvent();
   virtual TList* GetListOfObjects() { return fObjects; }
   virtual void   Clear();
protected:
   list<TString> fInputFiles;
   EStatus_t     fStatus;
   TString       fCurrentInput;
   Bool_t        fIsOnline;
   Bool_t        fIsBeginOfRun;

   TList   *fObjects;
   TArtEventStore *fArtEventStore;
};
#endif // end of #ifdef TCATEVENTSTORE_H
