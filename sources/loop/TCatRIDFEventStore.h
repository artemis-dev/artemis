/* $Id:$ */
/**
 * @file   TCatRIDFEventStore.h
 * @date   Created : May 04, 2012 00:04:50 JST
 *   Last Modified : 
 * @author ota <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#ifndef TCATRIDFEVENTSTORE_H
#define TCATRIDFEVENTSTORE_H

#include <TCatEventStore.h>

#include <TArtEventStore.h>

class TCatRIDFEventStore  : public TCatEventStore {

public:
   TCatRIDFEventStore();
   ~TCatRIDFEventStore();
   virtual Bool_t IsPrepared();
   virtual bool Open(Int_t sid = 0);
   virtual bool Open(const char *filename);
   virtual bool GetNextEvent();
   virtual void Clear();
   virtual void Close();
   virtual Bool_t IsBeginOfRun();
   virtual Bool_t AddInputFile(const char *filename);
private:
   list<TString>   fInputFiles;
   TArtEventStore *fArtEventStore;
   TString fCurrentInput;
   Bool_t fIsOnline;
   Bool_t fIsBeginOfRun;
};
#endif // end of #ifdef TCATRIDFEVENTSTORE_H
