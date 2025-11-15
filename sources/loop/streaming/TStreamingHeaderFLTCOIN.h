/**
 * @file   TStreamingHeaderFLTCOIN.h
 * @brief  Streaming Header for File sink
 *
 * @date   Created       : 2023-02-16 18:34:48 JST
 *         Last Modified : 2023-03-01 18:57:26 JST
 * @author Shinsuke OTA <ota@rcnp.osaka-u.ac.jp>
 *
 *    (C) 2023 Shinsuke OTA
 */
#ifndef TSTREAMINGHEADERFLTCOIN_H
#define TSTREAMINGHEADERFLTCOIN_H

#include <cstdint>
#include <ctime>

#include "TNamed.h"

namespace art {
   class TStreamingHeaderFLTCOIN;
}


class art::TStreamingHeaderFLTCOIN : public TNamed {
public:

   static const uint64_t kMagic {0x4e494f432d544c46};
   static const uint64_t kHeaderSize { sizeof(uint32_t)*12 };

   static bool IsHeaderFLTCOIN(uint64_t data) { return (data == kMagic); }

   void ReadFrom(char *buffer);

   virtual void Print(Option_t* opt = "") const;

   template <typename T> static void Decode(char*& data, T& out) {
     printf("%p \n",data);
      out = *(T*)data;
      data += sizeof(T);
   }

private:
   std::uint64_t fMagic;                   /* 64 bits */
   std::uint64_t fHeaderSize; /* 64 bits */
   std::uint32_t fNumTrigs;        /* 64 bits */
   std::uint32_t fWorkerID;               /* 64 bits */
   std::uint32_t fElapseTime;               /* 64 bits */
   std::uint32_t fDummy;               /* 64 bits */
   std::uint64_t fProcessTimeSec;               /* 64 bits */
   std::uint64_t fProcessTimeUsec;               /* 64 bits */


   ClassDef(TStreamingHeaderFLTCOIN,1) // 
};

#endif // TSTREAMINGHEADERFLTCOIN_H
