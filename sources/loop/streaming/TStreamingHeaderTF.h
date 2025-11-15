/**
 * @file   TStreamingHeaderTF.h
 * @brief  Streaming Header for Subtime Frame
 *
 * @date   Created       : 2023-02-16 14:34:16 JST
 *         Last Modified : 2023-02-18 11:30:46 JST
 * @author Shinsuke OTA <ota@rcnp.osaka-u.ac.jp>
 *
 *    (C) 2023 Shinsuke OTA
 */


#ifndef TSTREAMINGHEADERTF_H
#define TSTREAMINGHEADERTF_H

#include <cstdint>

#include "TNamed.h"

namespace art {
   class TStreamingHeaderTF;
}


class art::TStreamingHeaderTF : public TNamed {
public:

   TStreamingHeaderTF();
   ~TStreamingHeaderTF();
   
   // "DAEH-FT@" : little endian of "@TF-HEAD"
   static const uint64_t kMagic {0x444145482d465440};
   static const uint64_t kHeaderSize { sizeof(uint32_t)*6 };
   

   static bool IsHeaderTF(uint64_t data) { return (data == kMagic); }

   uint32_t GetTimeFrameID() { return fTimeFrameID; }
   uint32_t GetNumSources() { return fNumSources; }
   uint64_t GetLength() { return fLength; }

   void ReadFrom(char *buffer);

   virtual void Print(Option_t* opt = "") const;

protected:
   
   template <typename T> static void Decode(char*& data, T& out) {
      out = *(T*)data;
      data += sizeof(T);
   }
private:

   uint32_t fTimeFrameID {0} ;
   uint32_t fNumSources {0} ;
   uint64_t fLength {0} ;
   
   ClassDef(TStreamingHeaderTF,1) // Streaming Data Header for Subtime Frame
};



#endif // TSTREAMINGHEADERTF_H

