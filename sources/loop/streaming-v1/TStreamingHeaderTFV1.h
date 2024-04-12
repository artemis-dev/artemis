/**
 * @file   TStreamingHeaderTFV1.h
 * @brief  Streaming Header for Time Frame
 *
 * @date   Created       : 2023-02-16 14:34:16 JST
 *         Last Modified : 2024/04/12 23:59:47
 * @author Shinsuke OTA <ota@rcnp.osaka-u.ac.jp>
 *
 *    (C) 2023 Shinsuke OTA
 */

#ifndef TSTREAMINGHEADERTF_H
#define TSTREAMINGHEADERTF_H

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
      constexpr uint64_t MAGIC_TF{0x004d5246454d4954};
      class TStreamingHeaderTF;
   }
}

class art::v1::TStreamingHeaderTF : public art::TStreamingHeader<art::v1::MAGIC_TF>
{
public:
   static bool IsHeaderTF(uint64_t magic) { return IsThis(magic); }

   uint32_t GetTimeFrameID() { return fTimeFrameID; }
   uint32_t GetNumSources() { return fNumSources; }

   inline bool ReadRestFrom(char *buffer) {
      Decode(buffer,fTimeFrameID);
      Decode(buffer,fNumSources);
      return true;
   }

   // virtual void Print(Option_t *opt = "") const;


protected:
   uint32_t fTimeFrameID{0};
   uint32_t fNumSources{0};


   // ClassDef(TStreamingHeaderTF, 1) // Streaming Data Header for TimeFrame
};

#endif // TSTREAMINGHEADERTF_H
