/**
 * @file   TStreamingHeaderTF.cc
 * @brief  Streaming Header for Subtime Frame
 *
 * @date   Created       : 2023-02-16 15:00:48 JST
 *         Last Modified : 2023-02-17 17:50:14 JST
 * @author Shinsuke OTA <ota@rcnp.osaka-u.ac.jp>
 *
 *    (C) 2023 Shinsuke OTA
 */


#include "TStreamingHeaderTF.h"


ClassImp(art::TStreamingHeaderTF)


art::TStreamingHeaderTF::TStreamingHeaderTF()
{
}

art::TStreamingHeaderTF::~TStreamingHeaderTF()
{
}

// static function
void art::TStreamingHeaderTF::ReadFrom(char *data)
{
   if (!data || !IsHeaderTF(*(uint64_t*)data)) {
      // this is not the subtime frame header
      return;
   }

   data += sizeof(kMagic);
   
   Decode(data,fTimeFrameID);
   Decode(data,fNumSources);
   Decode(data,fLength);

//   data += sizeof(kMagic);
//   fTimeFrameID = *(uint32_t*)data;
//   data += sizeof(fTimeFrameID);
//   fFEMType       = *(uint32_t*)data;
//   data += sizeof(fFEMType);
//   fFEMId         = *(uint32_t*)data;
//   data += sizeof(fFEMId);
//   fLength        = *(uint32_t*)data;
//   data += sizeof(fLength);
//   fNumMessages   = *(uint32_t*)data;
//   data += sizeof(fNumMessages);
   
}

void art::TStreamingHeaderTF::Print(Option_t*) const
{
   printf("==============================\n");
   printf("TimeFrameID = %d\n",fTimeFrameID);
   printf("NumSources  = %d\n",fNumSources);
   printf("Length      = %d\n",fLength);
   printf("==============================\n");
}
