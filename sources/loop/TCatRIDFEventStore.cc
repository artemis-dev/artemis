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
   fObjects->Add(fArtEventStore->GetRawEventObject());
   fStatus = kIdle;
}
TCatRIDFEventStore::~TCatRIDFEventStore()
{
   delete fArtEventStore;
   delete fObjects;
}

Bool_t TCatRIDFEventStore::IsPrepared()
{
   TArtCore::Info("IsPrepared","fStatus = %d",fStatus);
   switch (fStatus) {
   case kEOF:
      Close();
      fStatus = kIdle;
   case kIdle:
      TArtCore::Info("IsPrepared","fInputFiles.size() = %d",fInputFiles.size());
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


Bool_t TCatRIDFEventStore::IsBeginOfRun()
{
   return fIsBeginOfRun;
}

Bool_t TCatRIDFEventStore::AddInputFile(const char *filename)
{
   ifstream fin(filename);
   if (!fin) {
      TArtCore::Info("AddInputFile","No such file %s",filename);
      return kFALSE;
   }
   fin.close();
   fInputFiles.push_back(filename);
   fIsOnline = kFALSE;
   return kTRUE;
}

bool TCatRIDFEventStore::Open(const char* filename)
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
bool TCatRIDFEventStore::Open(Int_t shmid)
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


bool TCatRIDFEventStore::GetNextEvent() {
   if(fArtEventStore->GetNextEvent()) {
      fIsBeginOfRun = kFALSE;
      return kTRUE;
   }
   // no more event exists
//   fArtEventStore->Close();
   fStatus = kEOF;
   return kFALSE;
}

void TCatRIDFEventStore::Clear()
{
   fArtEventStore->ClearData();
}

void TCatRIDFEventStore::Close()
{

}
