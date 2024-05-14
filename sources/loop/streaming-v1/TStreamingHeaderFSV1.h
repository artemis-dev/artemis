/**
 * @file   TStreamingHeaderFS.h
 * @brief  Streaming Header for File sink
 *
 * @date   Created       : 2023-02-16 18:34:48 JST
 *         Last Modified : 2024/04/13 00:01:49
 * @author Shinsuke OTA <ota@rcnp.osaka-u.ac.jp>
 *
 *    (C) 2023 Shinsuke OTA
 */

/*
inline namespace v1 {
constexpr uint64_t MAGIC = {0x004b4e53454c4946};

struct Header {
    uint64_t magic               {MAGIC};
    union {
        uint64_t size            {0};
        struct {
            uint32_t length         ;
            uint16_t hLength        ;
            uint16_t type           ;
        };
    };
    uint64_t fairMQDeviceType    {0};
    uint64_t runNumber           {0};
    time_t   startUnixtime       {0};
    time_t   stopUnixtime        {0};
    char     comments[256];
};
} //namespace v0

*/
#ifndef TSTREAMINGHEADERFS_H
#define TSTREAMINGHEADERFS_H

#include <stdio.h>
#include <string.h>

#include <cstdint>
#include <ctime>

#include "TStreamingHeader.h"

namespace art {
namespace v1 {
constexpr uint64_t MAGIC_FS = {0x004b4e53454c4946};
class TStreamingHeaderFS;
}  // namespace v1
}  // namespace art

class art::v1::TStreamingHeaderFS
    : public art::TStreamingHeader<art::v1::MAGIC_FS> {
  public:
   static bool IsHeaderFS(uint64_t magic) { return IsThis(magic); }

   bool ReadRestFrom(char *buffer) {
      Decode(buffer, fFairMQDeviceType);
      Decode(buffer, fRunNumber);
      Decode(buffer, fStartUnixTime);
      Decode(buffer, fStopUnixTime);
      memcpy(fComments, buffer, sizeof(fComments));
      return true;
   }

   std::uint64_t GetRunNumber() const { return fRunNumber; }

   inline virtual void Print() const;

  private:
   std::uint64_t fFairMQDeviceType; /* 64 bits */
   std::uint64_t fRunNumber;        /* 64 bits */
   std::time_t fStartUnixTime;      /* 64 bits */
   std::time_t fStopUnixTime;       /* 64 bits */
   char fComments[256];             /* 8 bits x 256 */

   // ClassDef(TStreamingHeaderFS, 1) //
};

void art::v1::TStreamingHeaderFS::Print() const {
   printf("==============================\n");
   printf("MAGIC        = %016lx\n", fMagic);
   printf("Length       = %d\n", fLength);
   printf("HeaderLength = %d\n", fHeaderLength);
   printf("Type         = %d\n", fType);
   printf("RunNumber    = %lu\n", fRunNumber);
   printf("Start        = %ld\n", fStartUnixTime);
   printf("Stop         = %ld\n", fStopUnixTime);
   printf("Comments:\n");
   printf("%s\n", fComments);
   printf("==============================\n");
}

#endif  // TSTREAMINGHEADERFS_H
