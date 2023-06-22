/**
 * @file   TStreamingHeaderFLTCOIN.cc
 * @brief  Streaming Header for File sink
 *
 * @date   Created       : 2023-02-16 22:08:56 JST
 *         Last Modified : 2023-02-16 22:28:00 JST
 * @author Shinsuke OTA <ota@rcnp.osaka-u.ac.jp>
 *
 *    (C) 2023 Shinsuke OTA
 */


#include "TStreamingHeaderFLTCOIN.h"
ClassImp(art::TStreamingHeaderFLTCOIN)


void art::TStreamingHeaderFLTCOIN::ReadFrom(char* data)
{
  printf("%p\n",data);

  printf("1\n");
   Decode(data,fMagic);
  printf("2\n");
   Decode(data,fHeaderSize);
  printf("3\n");
   Decode(data,fNumTrigs);
  printf("4\n");
   Decode(data,fWorkerID);
  printf("5\n");
   Decode(data,fElapseTime);
  printf("6\n");
}


void art::TStreamingHeaderFLTCOIN::Print(Option_t*) const
{
   printf("==============================\n");
   printf("HeaderSize = %d\n",fHeaderSize);
   printf("NumTrigs   = %d\n",fNumTrigs);
   printf("WorkerID   = %d\n",fWorkerID);
   printf("ElapseTime = %d\n",fElapseTime);
   printf("==============================\n");
}
