/**
 *   Created       : 2023-02-09 08:41:43 JST
 *   Last Modified : 2023-02-18 15:16:43 JST
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
   class TStreamingHeaderFS;
   class TStreamingHeaderSTF;
   class TStreamingHeaderTF;
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
   virtual void PreLoop();
   virtual void Process();
//   virtual void ProcessRawStremingData();
//   virtual void ProcessSubTimeFrame();
//   virtual void ProcessTimeFrame();
   virtual Bool_t GetTimeFrame();
   virtual Bool_t GetSubTimeFrame();
   virtual Bool_t GetHeartBeatFrame();
   
#if 0
   virtual Bool_t ReadTimeFrame();
   virtual Bool_t ReadSubTimeFrame();
   virtual Bool_t ReadSubTimeFrameOnce();
#endif   
   virtual Bool_t Open();
//   virtual Bool_t ReadData(int length, int femid, int femtype);



protected:
   virtual TRunInfo* GetRunInfo() { ; }
   virtual void NotifyEndOfRun();

   TDataSource      *fDataSource; //!
   char             *fBuffer; //!
   bool              fIsEOB; //!
   std::map<ULong64_t,char*> fSTFBuffer; //!
   int               fNumSources; //!
   TStreamingHeaderFS *fHeaderFS; //!
   TStreamingHeaderSTF *fHeaderSTF; //!
   TStreamingHeaderTF *fHeaderTF; //!
   std::vector<char*> fSubTimeFrameBuffers; //!
   std::vector<int>   fSubTimeFrameSize; //!
   std::vector<TStreamingHeaderSTF*> fSubTimeFrameHeaders; //!
   TClonesArray *fSTFClones; //!
   
   OutputData<TSegmentedData> fSegmentedData; //!
   OutputInfo<TList> fRunHeader; //!
   OutputData<TEventHeader> fEventHeader; //!
   OutputInfo<TList> fFileHeader; //! 
   Parameter<Int_t> fMaxFrames; //!
   Parameter<Int_t> fStartFrame; //!
   Parameter<StringVec_t> fFileName; //!
   Parameter<Int_t> fFEMType; //! for direct sink from sampler (without header)
   Parameter<Int_t> fFEMID; //! for direct sink from sapmler
   Parameter<Int_t> fDefaultLength; // for direct sink from sampler

   ClassDef(TStreamingEventStore,1) // streaming data event store
};

#endif // TSTREAMINGEVENTSTORE_H
