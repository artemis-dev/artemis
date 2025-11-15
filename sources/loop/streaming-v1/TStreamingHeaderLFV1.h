/**
 * @file   TStreamingHeaderLFV1.h
 * @brief  Streaming Header for Time Frame
 *
 * @date   Created       : 2023-02-16 14:34:16 JST
 *         Last Modified : 2024-05-21 08:37:25 JST
 * @author Shinsuke OTA <ota@rcnp.osaka-u.ac.jp>
 *
 *    (C) 2023 Shinsuke OTA
 */

#ifndef TSTREAMINGHEADERLFV1_H
#define TSTREAMINGHEADERLFV1_H

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
      constexpr uint64_t LFFMagic = 0x0049474f4c544c46;
      class TStreamingHeaderLF;
   }
}

class art::v1::TStreamingHeaderLF : public art::TStreamingHeader<art::v1::LFFMagic>
{
public:
   static bool IsHeaderLF(uint64_t magic) { return IsThis(magic); }

   //uint32_t GetTimeFrameID() { return fTimeFrameID; }
   //uint32_t GetNumSources() { return fNumSources; }

   inline bool ReadRestFrom(char *buffer) {
      Decode(buffer, fTimeFrameID);
      Decode(buffer, fNumTrigs);
      Decode(buffer, fWorkerID);
      Decode(buffer, fNumMessages);
      Decode(buffer, fElapseTime);
      Decode(buffer, fReserved);
      Decode(buffer, fTimeSec);
      Decode(buffer, fTimeUSec);
      return true;
   }
   inline virtual void Print() const;
   // virtual void Print(Option_t *opt = "") const;


protected:
   uint32_t fTimeFrameID;
   uint32_t fNumTrigs;
   uint32_t fWorkerID;
   uint32_t fNumMessages;
   uint32_t fElapseTime;
   uint32_t fReserved;
   uint64_t fTimeSec;
   uint64_t fTimeUSec;

   // ClassDef(TStreamingHeaderTF, 1) // Streaming Data Header for TimeFrame
};


void art::v1::TStreamingHeaderLF::Print() const {
   printf("==============================\n");
   printf("MAGIC        = %016llx\n", fMagic);
   printf("Length       = %d\n", fLength);
   printf("HeaderLength = %d\n", fHeaderLength);
   printf("Type         = %d\n", fType);
   printf("==============================\n");
}


#endif // TSTREAMINGHEADERTF_H
