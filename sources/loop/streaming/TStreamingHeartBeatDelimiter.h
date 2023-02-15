/*
 * Created       : 2023-02-11 11:54:42 JST
 * Last Modified : 2023-02-15 14:39:24 JST
 */

#ifndef TSTREAMINGHEARTBEATDELIMITER_H
#define TSTREAMINGHEARTBEATDELIMITER_H

#include "TRawDataObject.h"

namespace art {
   class TStreamingHeartBeatDelimiter;
}

class art::TStreamingHeartBeatDelimiter : public TRawDataObject {

public:
   TStreamingHeartBeatDelimiter();
   virtual ~TStreamingHeartBeatDelimiter();

   virtual Int_t GetFlag() { return fFlag; }
   virtual Int_t GetSpillNumber() { return fSpillNumber; }
   virtual Int_t GetHeartBeatFrameNumber() { return fHeartBeatFrameNumber; }

   void  SetFlag(int flag ) { fFlag = flag; }
   void  SetSpillNumber(int number) { fSpillNumber = number; }
   void  SetHeartBeatFrameNumber(int number) { fHeartBeatFrameNumber = number; }

   virtual void Copy(TObject& obj) const;
   
private:
   Int_t fFlag; // flag
   Int_t fSpillNumber; // what is the general purpose of spill and its number?
   Int_t fHeartBeatFrameNumber; // number of heartbeat frame
   
   ClassDef(TStreamingHeartBeatDelimiter,1) // streaming data heartbeat delimiter
};



#endif
