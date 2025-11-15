/**
 * @file   TStreamingHeaderSTF.h
 * @brief  Streaming Header for Subtime Frame
 *
 * @date   Created       : 2023-02-16 14:34:16 JST
 *         Last Modified : 2023-02-17 20:41:42 JST
 * @author Shinsuke OTA <ota@rcnp.osaka-u.ac.jp>
 *
 *    (C) 2023 Shinsuke OTA
 */


#ifndef TSTREAMINGHEADERSTF_H
#define TSTREAMINGHEADERSTF_H

#include <cstdint>

#include "TNamed.h"

namespace art {
   class TStreamingHeaderSTF;
}


class art::TStreamingHeaderSTF : public TNamed {
public:

   TStreamingHeaderSTF();
   ~TStreamingHeaderSTF();
   
   // "DAEH-FTS" : little endian of "STF-HEAD"
   static const uint64_t kMagic {0x444145482d465453};
   static const uint64_t kHeaderSize { sizeof(uint32_t)*12 };
   

   static bool IsHeaderSTF(uint64_t data) { return (data == kMagic); }

   void ReadFrom(char *buffer);

   virtual void Print(Option_t* opt = "") const;


   uint32_t GetTimeFrameID() const { return fTimeFrameID;}
   uint32_t GetReserved() const { return fReserved ;}
   uint32_t GetFEMType() const { return fFEMType;}
   uint32_t GetFEMId() const { return fFEMId;}
   uint32_t GetLength() const { return fLength;}
   uint32_t GetNumMessages() const { return fNumMessages;}
   uint64_t GetTimeSec() const { return fTimeSec ;}
   uint64_t GetTimeUsec() const { return fTimeUsec;}
   

protected:
   
   template <typename T> static void Decode(char*& data, T& out) {
      out = *(T*)data;
      data += sizeof(T);
   }
private:

   uint32_t fTimeFrameID;
   uint32_t fReserved;
   uint32_t fFEMType;
   uint32_t fFEMId;
   uint32_t fLength;
   uint32_t fNumMessages;
   uint64_t fTimeSec;
   uint64_t fTimeUsec;
   
   ClassDef(TStreamingHeaderSTF,1) // Streaming Data Header for Subtime Frame
};



#endif // TSTREAMINGHEADERSTF_H

