/* $Id:$ */
/**
 * @file   TZmqDataSource.cc
 * @date   Created : 2024-06-08 23:11:23 JST
 *   Last Modified : 2024-06-09 21:36:24 JST
 * @author Shinsuke OTA <ota@rcnp.osaka-u.ac.jp>
 *  
 *  
 *    Copyright (C)2024
 */
#include "TZmqDataSource.h"

#include <thread>
#include <chrono>

#define DEBUG_ZMQDATASOURCE 0

#if DEBUG_ZMQDATASOURCE
#define DBPRINT(...) printf(__VA_ARGS__)
#else
#define DBPRINT(...) 
#endif

art::TZmqDataSource::TZmqDataSource()
   : fMemoryPos(0), fConnectionTimeout(1000)
{
   fSubscriber = new TZmqSubscriber();
}
art::TZmqDataSource::~TZmqDataSource()
{
   if (fSubscriber) delete fSubscriber;
   fSubscriber = nullptr;
}


int art::TZmqDataSource::Seek(long offset, int origin)
{
   switch (origin) {
   case SEEK_SET:
      fMemoryPos = offset;
      break;
   case SEEK_CUR:
      fMemoryPos += offset;
      break;
   case SEEK_END:
      fMemoryPos = fSubscriber->GetSize() - offset;
      break;
   default:
      return -1;
   }
   return 0;
}

int art::TZmqDataSource::Read(char *buf, const int&size)
{
   if (fMemoryPos+size > fSubscriber->GetSize()) {
      fStatus = kEOF;
      return 0;
   }
   int len = fSubscriber->Read(buf,size,fMemoryPos);
   if (len < 0) {
      fMemoryPos = 0;
      fStatus = kERROR;
      return len;
   }
   fMemoryPos += size;
   return len;
}

bool art::TZmqDataSource::Prepare()
{
   DBPRINT("Preparing %p\n",fSubscriber); 
   if (!fSubscriber) fSubscriber = new TZmqSubscriber;
   DBPRINT("URI %s with valid %d\n",fSubscriber->GetUri().c_str(), fSubscriber->IsValid());
   if (!fSubscriber->GetUri().length() || 
       !fSubscriber->IsValid()) {
      fSubscriber->Terminate();
      if (!fSubscriber->Connect()) {
         DBPRINT("Subscriber is NOT connected\n");
         std::this_thread::sleep_for(std::chrono::milliseconds(fConnectionTimeout));
         return false;
      }
      DBPRINT("Subscriber is connected\n");
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
   }
   if (fSubscriber->Recv() < 0) {
      DBPRINT("Subscriber is terminated \n");
      fSubscriber->Terminate();
      DBPRINT("Subscriber is valid ? %d \n",fSubscriber->IsValid());
      return false;
   }
   DBPRINT("Prepare : Received %d\n",fSubscriber->GetSize());
   fStatus = kReady;
   fMemoryPos = 0;
   return true;
}


Int_t art::TZmqDataSource::IsPrepared()
{
   return (fStatus == kReady);
}




      
