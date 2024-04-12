/*
 * Created       : 2023-02-11 11:54:42 JST
 * Last Modified : 2023-02-15 14:40:29 JST
 */

#include "TStreamingHeartBeatDelimiter.h"

ClassImp(art::TStreamingHeartBeatDelimiter)

art::TStreamingHeartBeatDelimiter::TStreamingHeartBeatDelimiter()
{
}


art::TStreamingHeartBeatDelimiter::~TStreamingHeartBeatDelimiter()
{
}

void art::TStreamingHeartBeatDelimiter::Copy(TObject& obj) const
{
   TRawDataObject::Copy(obj);
   TStreamingHeartBeatDelimiter& out = (TStreamingHeartBeatDelimiter&) obj;
   out.fFlag = fFlag;
   out.fSpillNumber = fSpillNumber;
   out.fHeartBeatFrameNumber = fHeartBeatFrameNumber;
}
