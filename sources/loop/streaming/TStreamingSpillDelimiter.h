/*
 * Created       : 2023-02-11 11:54:42 JST
 * Last Modified : 2023-02-11 12:14:14 JST
 */


#ifndef TSTREAMINGSPILLDELIMITER_H
#define TSTREAMINGSPILLDELIMITER_H

#include "TRawDataObject.h"

namespace art {
   class TStreamingSpillDelimiter;
}

class art::TStreamingSpillDelimiter : public TRawDataObject {

public:
   TStreamingSpillDelimiter();
   virtual ~TStreamingSpillDelimiter();

   virtual Int_t GetFlag() { return fFlag; }
   virtual Int_t GetSpillNumber() { return fSpillNumber; }
   virtual Int_t GetHeartBeatFrameNumber() { return fHeartBeatFrameNumber; }
   virtual Int_t GetHeartBeatFrameCounter() { return fHeartBeatFrameCounter; }
   virtual Int_t GetOnOff() { return fOnOff; }

   void  SetFlag(int flag ) { fFlag = flag; }
   void  SetSpillNumber(int number) { fSpillNumber = number; }
   void  SetHeartBeatFrameNumber(int number) { fHeartBeatFrameNumber = number; }
   void  SetHeartBeatFrameCounter(int counter) { fHeartBeatFrameCounter = counter; }
   void SetOnOff(int onoff) { fOnOff = onoff; }

   static constexpr int kON = 1;
   static constexpr int kOFF = 0;
   

private:
   int fFlag;
   int fSpillNumber;
   int fHeartBeatFrameCounter;
   int fHeartBeatFrameNumber;
   int fOnOff;
   

   ClassDef(TStreamingSpillDelimiter,1) // spill dellimiter but what is the general purpose?
};


#endif // TSTREAMINGSPILLDELIMITER_H
