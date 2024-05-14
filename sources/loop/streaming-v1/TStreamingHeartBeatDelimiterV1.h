/*
 * Created       : 2023-02-11 11:54:42 JST
 * Last Modified : 2024/04/13 02:22:52
 */

#ifndef TSTREAMINGHEARTBEATDELIMITER_H
#define TSTREAMINGHEARTBEATDELIMITER_H

#include <cstdint>

#include "TRawDataObject.h"

namespace art {
namespace streaming {
namespace v1 {
class TStreamingHeartBeatDelimiter;
}
}  // namespace streaming
}  // namespace art

class art::streaming::v1::TStreamingHeartBeatDelimiter : public TRawDataObject {
  public:
   TStreamingHeartBeatDelimiter();
   virtual ~TStreamingHeartBeatDelimiter();

   static bool IsDelim1(uint64_t data) {
      return (((data >> 58) & 0x3f) == 0x1c);
   }
   static bool IsDelim2(uint64_t data) {
      return (((data >> 58) & 0x3f) == 0x1e);
   }

   void Decode(uint64_t data);

   virtual Int_t GetFlag() { return fFlag; }
   virtual Int_t GetHeartBeatFrameNumber() { return fHeartBeatFrameNumber; }

   void SetFlag(int flag) { fFlag = flag; }
   void SetHeartBeatFrameNumber(int number) { fHeartBeatFrameNumber = number; }

   virtual void Copy(TObject& obj) const;

  private:
   uint32_t fFlag;                  // flag
   uint32_t fUserReg;               // flag
   uint32_t fTimeOffset;            // time offset
   uint32_t fHeartBeatFrameNumber;  // heart beat frame number
   uint32_t fGeneratedDataSize;
   uint32_t fTransferedDataSize;

   ClassDef(TStreamingHeartBeatDelimiter,
            1)  // streaming data heartbeat delimiter
};

#endif
