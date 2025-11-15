/*
 * Created       : 2023-02-11 12:10:50 JST
 * Last Modified : 2023-02-15 10:28:51 JST
 */

#include "TStreamingSpillDelimiter.h"

ClassImp(art::TStreamingSpillDelimiter)

art::TStreamingSpillDelimiter::TStreamingSpillDelimiter()
{
}


art::TStreamingSpillDelimiter::~TStreamingSpillDelimiter()
{
}

void art::TStreamingSpillDelimiter::Copy(TObject& obj) const
{
   TRawDataObject::Copy(obj);
   TStreamingSpillDelimiter& out = (TStreamingSpillDelimiter&) obj;
   out.fFlag = fFlag;
   out.fSpillNumber = fSpillNumber;
   out.fHeartBeatFrameCounter = fHeartBeatFrameCounter;
   out.fHeartBeatFrameNumber = fHeartBeatFrameNumber;
   out.fOnOff = fOnOff;
}

