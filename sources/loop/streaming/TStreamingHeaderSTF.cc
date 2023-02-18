/**
 * @file   TStreamingHeaderSTF.cc
 * @brief  Streaming Header for Subtime Frame
 *
 * @date   Created       : 2023-02-16 15:00:48 JST
 *         Last Modified : 2023-02-17 19:53:21 JST
 * @author Shinsuke OTA <ota@rcnp.osaka-u.ac.jp>
 *
 *    (C) 2023 Shinsuke OTA
 */


#include "TStreamingHeaderSTF.h"


ClassImp(art::TStreamingHeaderSTF)


art::TStreamingHeaderSTF::TStreamingHeaderSTF()
{
}

art::TStreamingHeaderSTF::~TStreamingHeaderSTF()
{
}

// static function
void art::TStreamingHeaderSTF::ReadFrom(char *data)
{
   if (!data || !IsHeaderSTF(*(uint64_t*)data)) {
      // this is not the subtime frame header
      return;
   }

   data += sizeof(kMagic);
   
   Decode(data,fTimeFrameID);
   Decode(data,fReserved);
   Decode(data,fFEMType);
   Decode(data,fFEMId);
   Decode(data,fLength);
   Decode(data,fNumMessages);
   Decode(data,fTimeSec);
   Decode(data,fTimeUsec);

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

void art::TStreamingHeaderSTF::Print(Option_t*) const
{
   printf("==============================\n");
   printf("TimeFrameID = %d\n",fTimeFrameID);
   printf("Reserved    = %d\n",fReserved);
   printf("FEMType     = %d\n",fFEMType);
   printf("FEMId       = %x\n",fFEMId);
   printf("Length      = %d\n",fLength);
   printf("NumMessages = %d\n",fNumMessages);
   printf("TimeSec     = %d\n",fTimeSec);
   printf("TimeUsec    = %d\n",fTimeUsec);
   printf("==============================\n");
}
