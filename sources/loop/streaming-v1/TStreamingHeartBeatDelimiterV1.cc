/*
 * Created       : 2023-02-11 11:54:42 JST
 * Last Modified : 2024/04/13 02:23:15
 */

#include "TStreamingHeartBeatDelimiterV1.h"

#include <cstdint>

ClassImp(art::streaming::v1::TStreamingHeartBeatDelimiter)

    using art::streaming::v1::TStreamingHeartBeatDelimiter;

TStreamingHeartBeatDelimiter::TStreamingHeartBeatDelimiter() {}

TStreamingHeartBeatDelimiter::~TStreamingHeartBeatDelimiter() {}

void TStreamingHeartBeatDelimiter::Copy(TObject &obj) const {
   TRawDataObject::Copy(obj);
   TStreamingHeartBeatDelimiter &out = (TStreamingHeartBeatDelimiter &)obj;
   out.fFlag = fFlag;
   out.fHeartBeatFrameNumber = fHeartBeatFrameNumber;
   out.fFlag = fFlag;                  // flag
   out.fUserReg = fUserReg;               // flag
   out.fTimeOffset = fTimeOffset;            // time offset
   out.fHeartBeatFrameNumber = fHeartBeatFrameNumber;  // heart beat frame number
   out.fGeneratedDataSize = fGeneratedDataSize;
   out.fTransferedDataSize = fTransferedDataSize;
}

void TStreamingHeartBeatDelimiter::Decode(uint64_t data) {
   if (IsDelim1(data)) {
      fFlag = ((data >> 40) & 0xffff);
      fTimeOffset = ((data >> 24) & 0xffff);
      fHeartBeatFrameNumber = ((data & 0xffffff));
   }

   if (IsDelim2(data)) {
      fUserReg = ((data >> 40) & 0xffff);
      fGeneratedDataSize = ((data >> 20) & 0xfffff);
      fTransferedDataSize = (data & 0xfffff);
   }
}
