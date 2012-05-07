/* $Id:$ */
/**
 * @file   TCatRIDFEventStore.cc
 * @date   Created : May 04, 2012 00:04:24 JST
 *   Last Modified : 
 * @author ota <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TCatRIDFEventStore.h"

TCatRIDFEventStore::TCatRIDFEventStore()
{
   fArtEventStore = new TArtEventStore;
   fObjects = new TList;
   fObjects->Add(fArtEventStore->GetRawEventObject());
}
TCatRIDFEventStore::~TCatRIDFEventStore()
{
   delete fArtEventStore;
   delete fObjects;
}

bool TCatRIDFEventStore::GetNextEvent()
{
   return fArtEventStore->GetNextEvent();
}

bool TCatRIDFEventStore::Open(Int_t sid)
{
   return fArtEventStore->Open(sid);
}

bool TCatRIDFEventStore::Open(const char *filename)
{
   return fArtEventStore->Open(filename);
}

void TCatRIDFEventStore::Clear()
{
   fArtEventStore->ClearData();
}
