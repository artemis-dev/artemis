/**
 * @file   TStreamingHeaderFS.cc
 * @brief  Streaming Header for File sink
 *
 * @date   Created       : 2023-02-16 22:08:56 JST
 *         Last Modified : 2023-02-16 22:28:00 JST
 * @author Shinsuke OTA <ota@rcnp.osaka-u.ac.jp>
 *
 *    (C) 2023 Shinsuke OTA
 */


#include "TStreamingHeaderFS.h"
ClassImp(art::TStreamingHeaderFS)


void art::TStreamingHeaderFS::ReadFrom(char* data)
{
   Decode(data,fHeaderSize);
   Decode(data,fMagic);
   Decode(data,fFairMQDeviceType);
   Decode(data,fRunNumber);
   Decode(data,fStartUnixTime);
   Decode(data,fStopUnixTime);
   memcpy(fComments,data,sizeof(fComments));
}


void art::TStreamingHeaderFS::Print(Option_t*) const
{
   printf("==============================\n");
   printf("HeaderSize = %d\n",fHeaderSize);
   printf("RunNumber  = %d\n",fRunNumber);
   printf("Start      = %d\n",fStartUnixTime);
   printf("Stop       = %d\n",fStopUnixTime);
   printf("Comments:\n");
   printf("%s\n",fComments);
   printf("==============================\n");
}
