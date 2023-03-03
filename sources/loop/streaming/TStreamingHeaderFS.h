/**
 * @file   TStreamingHeaderFS.h
 * @brief  Streaming Header for File sink
 *
 * @date   Created       : 2023-02-16 18:34:48 JST
 *         Last Modified : 2023-03-01 18:57:26 JST
 * @author Shinsuke OTA <ota@rcnp.osaka-u.ac.jp>
 *
 *    (C) 2023 Shinsuke OTA
 */
#ifndef TSTREAMINGHEADERFS_H
#define TSTREAMINGHEADERFS_H

#include <cstdint>
#include <ctime>

#include "TNamed.h"

namespace art {
   class TStreamingHeaderFS;
}


class art::TStreamingHeaderFS : public TNamed {
public:

   static const uint64_t kMagic {0x444145482d534640};

   static bool IsHeaderFS(uint64_t data) { return (data == kMagic); }

   void ReadFrom(char *buffer);

   virtual void Print(Option_t* opt = "") const;

   template <typename T> static void Decode(char*& data, T& out) {
      out = *(T*)data;
      data += sizeof(T);
   }

private:
   std::uint64_t fHeaderSize; /* 64 bits */
   std::uint64_t fMagic;                   /* 64 bits */
   std::uint64_t fFairMQDeviceType;        /* 64 bits */
   std::uint64_t fRunNumber;               /* 64 bits */
   std::time_t   fStartUnixTime;           /* 64 bits */
   std::time_t   fStopUnixTime;            /* 64 bits */
   char          fComments[256];           /* 8 bits x 256 */


   ClassDef(TStreamingHeaderFS,1) // 
};

#endif // TSTREAMINGHEADERFS_H
