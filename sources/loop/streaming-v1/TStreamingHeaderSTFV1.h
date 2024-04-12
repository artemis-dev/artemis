/**
 * @file   TStreamingHeaderSTF.h
 * @brief  Streaming Header for Subtime Frame
 *
 * @date   Created       : 2023-02-16 14:34:16 JST
 *         Last Modified : 2024/04/13 00:01:07
 * @author Shinsuke OTA <ota@rcnp.osaka-u.ac.jp>
 *
 *    (C) 2023 Shinsuke OTA
 */

/*
struct Header {
    uint64_t magic        {MAGIC};
    uint32_t length       {0};
    uint16_t hLength      {40};
    uint16_t type         {0};
    uint32_t timeFrameId  {0};
    uint32_t femType      {0};
    uint32_t femId        {0};
    uint32_t numMessages  {0};
    uint64_t timeSec     {0};
    uint64_t timeUSec    {0};
};
*/

#ifndef TSTREAMINGHEADERSTF_H
#define TSTREAMINGHEADERSTF_H

#include <stdio.h>

#include <cstdint>

#include "TStreamingHeader.h"

namespace art {
namespace v1 {
constexpr uint64_t MAGIC_STF{0x00454d4954425553};
class TStreamingHeaderSTF;
}  // namespace v1
}  // namespace art

class art::v1::TStreamingHeaderSTF
    : public art::TStreamingHeader<art::v1::MAGIC_STF> {
  public:
   static bool IsHeaderSTF(uint64_t magic) { return IsThis(magic); }

   bool ReadRestFrom(char *buffer) {
      Decode(buffer, fTimeFrameID);
      Decode(buffer, fFEMType);
      Decode(buffer, fFEMId);
      Decode(buffer, fNumMessages);
      Decode(buffer, fTimeSec);
      Decode(buffer, fTimeUsec);
      return true;
   }

   inline virtual void Print() const;

   uint32_t GetTimeFrameID() const { return fTimeFrameID; }
   uint32_t GetReserved() const { return fReserved; }
   uint32_t GetFEMType() const { return fFEMType; }
   uint32_t GetFEMId() const { return fFEMId; }
   uint32_t GetLength() const { return fLength; }
   uint32_t GetNumMessages() const { return fNumMessages; }
   uint64_t GetTimeSec() const { return fTimeSec; }
   uint64_t GetTimeUsec() const { return fTimeUsec; }
   virtual uint16_t GetHeaderLength() const { return 48; }
  protected:
   template <typename T>
   static void Decode(char *&data, T &out) {
      out = *(T *)data;
      data += sizeof(T);
   }

  private:
   uint32_t fTimeFrameID;
   uint32_t fReserved;
   uint32_t fFEMType;
   uint32_t fFEMId;
   uint32_t fNumMessages;
   uint64_t fTimeSec;
   uint64_t fTimeUsec;

   // ClassDef(TStreamingHeaderSTF, 1) // Streaming Data Header for Subtime
   // Frame
};

void art::v1::TStreamingHeaderSTF::Print() const {
   printf("==============================\n");
   printf("TimeFrameID = %d\n", fTimeFrameID);
   printf("Reserved    = %d\n", fReserved);
   printf("FEMType     = %d\n", fFEMType);
   printf("FEMId       = %x\n", fFEMId);
   printf("HeaderLength= %d\n", fHeaderLength);
   printf("Length      = %d\n", fLength);
   printf("NumMessages = %d\n", fNumMessages);
   printf("TimeSec     = %llu\n", fTimeSec);
   printf("TimeUsec    = %llu\n", fTimeUsec);
   printf("==============================\n");
}

#endif  // TSTREAMINGHEADERSTF_H
