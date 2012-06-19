/* $Id:$ */
/**
 * @file   TCatEventStore.cc
 * @date   Created : May 04, 2012 00:04:01 JST
 *   Last Modified : May 16, 2012 15:26:56 JST
 * @author ota <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TCatEventStore.h"

#include <TArtEventStore.h>

TCatEventStore::TCatEventStore()
{
   fArtEventStore = new TArtEventStore;
   fObjects = new TList;
   fIsOnline = kTRUE;
   fStatus = kIdle;
}
TCatEventStore::~TCatEventStore()
{
}


Bool_t TCatEventStore::AddInputFile(const char *filename)
{
   ifstream fin(filename);
   if (!fin) return kFALSE;
   fin.close();
   fInputFiles.push_back(filename);
   fIsOnline = kFALSE;
   return kTRUE;
}


Bool_t TCatEventStore::IsBeginOfRun()
{
   return fIsBeginOfRun;
}

bool TCatEventStore::IsPrepared()
{
   switch (fStatus) {
   case kEOF:
      Close();
      fStatus = kIdle;
   case kIdle:
      if (fInputFiles.size()) {
         fCurrentInput = fInputFiles.front();
         fInputFiles.pop_front();
         if (Open(fCurrentInput)) {
            fStatus = kRunning;
         } else {
            fStatus = kIdle;
         }
      } else if (fIsOnline) {
         if (Open(0)) {
            fStatus = kRunning;
         } else {
            fStatus = kIdle;
         }
      } else {
         fCurrentInput = "";
         fStatus = kIdle;
         return kFALSE;
      }
      if (fStatus == kRunning) return kTRUE;
      return kFALSE;
      break;
   case kRunning:
      return kTRUE;
      break;
   default:
      return kFALSE;
      break;
   }
}

bool TCatEventStore::Open(Int_t shmid)
{
   if (fArtEventStore->Open(shmid)) {
      TNamed *obj =  fArtEventStore->GetRawEventObject();
      fObjects->Remove(fObjects->FindObject("rawdata"));
      obj->SetName("rawdata");
      fObjects->Add(obj);
      fIsOnline = kTRUE;
      fIsBeginOfRun = kTRUE;
      return kTRUE;
   }
   return kFALSE;
}

bool TCatEventStore::Open(const char* filename)
{
   if (fArtEventStore->Open(filename)) {
      TNamed *obj =  fArtEventStore->GetRawEventObject();
//      printf("obj = %p\n",obj);
//      printf("old = %p\n",fObjects->FindObject("rawdata"));
      fObjects->Remove(fObjects->FindObject("rawdata"));
      obj->SetName("rawdata");
      fObjects->Add(obj);
      fIsOnline = kFALSE;
      fIsBeginOfRun = kTRUE;
      return kTRUE;
   }
   return kFALSE;
}

bool TCatEventStore::GetNextEvent() {
   if(fArtEventStore->GetNextEvent()) return kTRUE;
   // no more event exists
//   fArtEventStore->Close();
   fStatus = kEOF;
   return kFALSE;
}

void TCatEventStore::Clear()
{
   fArtEventStore->ClearData();
}

void TCatEventStore::Close()
{

}
