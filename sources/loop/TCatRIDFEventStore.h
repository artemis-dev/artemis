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

   virtual bool Open(Int_t sid = 0);
   virtual bool Open(const char *filename);
   virtual bool GetNextEvent();
   virtual void Clear();
private:
   TArtEventStore *fArtEventStore;
};
#endif // end of #ifdef TCATRIDFEVENTSTORE_H
