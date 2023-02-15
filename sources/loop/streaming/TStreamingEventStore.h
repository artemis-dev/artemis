/**
 *   Created       : 2023-02-09 08:41:43 JST
 *   Last Modified : 2023-02-15 14:27:32 JST
 * @author Shinsuke OTA <ota@rcnp.osaka-u.ac.jp>
 */


#ifndef TSTREAMINGEVENTSTORE_H
#define TSTREAMINGEVENTSTORE_H


#include "TProcessor.h"
#include "IEventStore.h"


namespace art {
   class TStreamingEventStore;
   class TRunInfo;
   class TDataSource;
   class TSegmentedData;
   class TEventHeader;
}




class art::TStreamingEventStore : public TProcessor, public IEventStore {
public:
   TStreamingEventStore();
   virtual ~TStreamingEventStore();

   TStreamingEventStore(const TStreamingEventStore& rhs);
   TStreamingEventStore& operator=(const TStreamingEventStore& rhs);

   Int_t GetRunNumber() const {return 0;}
   const char* GetRunName() const {return "";}

   virtual void Init(TEventCollection *col);
   virtual void Process();
//   virtual void ProcessRawStremingData();
//   virtual void ProcessSubTimeFrame();
//   virtual void ProcessTimeFrame();
   virtual Bool_t ReadTimeFrame();
   virtual Bool_t ReadSubTimeFrame();
   virtual Bool_t ReadSubTimeFrameOnce();
   virtual Bool_t Open();
   virtual Bool_t ReadData(int length, int femid, int femtype);




protected:
   virtual TRunInfo* GetRunInfo() { ; }

   TDataSource      *fDataSource; //!
   char             *fBuffer; //!
   bool              fIsEOB; //!
   
   OutputData<TSegmentedData> fSegmentedData; //!
   OutputInfo<TList> fRunHeader; //!
   OutputData<TEventHeader> fEventHeader; //!
   Parameter<Int_t> fMaxFrames; //!
   Parameter<Int_t> fStartFrame; //!
   Parameter<StringVec_t> fFileName; //!
   Parameter<Int_t> fFEMType; //! for direct sink from sampler (without header)
   Parameter<Int_t> fFEMID; //! for direct sink from sapmler
   Parameter<Int_t> fDefaultLength; // for direct sink from sampler

   ClassDef(TStreamingEventStore,1) // streaming data event store
};

#endif // TSTREAMINGEVENTSTORE_H
