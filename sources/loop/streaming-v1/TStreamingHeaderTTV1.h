/**
 * @file   TStreamingHeaderTTV1.h
 * @brief  Streaming Header for Time Frame
 *
 * @date   Created       : 2023-02-16 14:34:16 JST
 *         Last Modified : 2024-05-16 06:45:56 JST
 * @author Shinsuke OTA <ota@rcnp.osaka-u.ac.jp>
 *
 *    (C) 2023 Shinsuke OTA
 */

#ifndef TSTREAMINGHEADERTTV1_H
#define TSTREAMINGHEADERTTV1_H

#include <cstdint>

#include "TStreamingHeader.h"

/*
   constexpr uint64_t MAGIC {0x004d5246454d4954};
   struct Header {
      uint64_t magic       {MAGIC};
      uint32_t length      {0};
      uint16_t hLength     {24};
      uint16_t type        {0};
      uint32_t timeFrameId {0};
       uint32_t numSource   {0};
   };
*/

namespace art
{
   namespace v1
   {
      constexpr uint64_t TTFMagic = 0x00454d4954475254;
      class TStreamingHeaderTT;
   }
}

class art::v1::TStreamingHeaderTT : public art::TStreamingHeader<art::v1::TTFMagic>
{
public:
   static bool IsHeaderTT(uint64_t magic) { return IsThis(magic); }

   //uint32_t GetTimeFrameID() { return fTimeFrameID; }
   //uint32_t GetNumSources() { return fNumSources; }

   inline bool ReadRestFrom(char *buffer) {
      return true;
   }
   inline virtual void Print() const;
   // virtual void Print(Option_t *opt = "") const;


protected:

   // ClassDef(TStreamingHeaderTF, 1) // Streaming Data Header for TimeFrame
};


void art::v1::TStreamingHeaderTT::Print() const {
   printf("==============================\n");
   printf("MAGIC        = %016llx\n", fMagic);
   printf("Length       = %d\n", fLength);
   printf("HeaderLength = %d\n", fHeaderLength);
   printf("Type         = %d\n", fType);
   printf("==============================\n");
}


#endif // TSTREAMINGHEADERTF_H
