#ifndef TSTREAMINGHEADER_H
#define TSTREAMINGHEADER_H

#include <cstdint>

namespace art
{
   template <uint64_t magic>
   class TStreamingHeader;
   namespace streaming
   {
      namespace v1
      {
         constexpr int HDR_BASE_LENGTH = {16};
      }
   }
}

template <uint64_t magic>
class art::TStreamingHeader
{
public:
   TStreamingHeader() {}
   virtual ~TStreamingHeader() {}

   static bool Magic() { return magic; }
   static bool IsThis(uint64_t data) { return (data == magic); }
   virtual uint64_t GetMagic() const { return fMagic; }
   virtual uint32_t GetLength() const { return fLength; }
   virtual uint16_t GetHeaderLength() const { return fHeaderLength; }
   virtual uint16_t GetType() const { return fType; }

   bool ReadFrom(char *buffer)
   {
      if (!IsThis(*(uint64_t *)buffer))
         return false;
      ReadBaseFrom(buffer);
      return ReadRestFrom(buffer+art::streaming::v1::HDR_BASE_LENGTH);
   }

   bool ReadBaseFrom(char *buffer)
   {
      Decode(buffer, fMagic);
      Decode(buffer, fLength);
      Decode(buffer, fHeaderLength);
      Decode(buffer, fType);
      return true;
   }
   virtual bool ReadRestFrom(char *buffer) = 0;

   template <typename T>
   static void Decode(char *&data, T &out)
   {
      out = *(T*)data;
      data += sizeof(T);
   }

   bool IsType(uint16_t type) const { return (fType == type); }

protected:
   uint64_t fMagic;
   uint32_t fLength;
   uint16_t fHeaderLength;
   uint16_t fType;
};

#endif // TSTREAMINGHEADER_H
