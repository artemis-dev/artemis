/**
 *   Created       : 2023-02-09 08:41:43 JST
 *   Last Modified : 2025-08-03 22:33:47 JST
 * @author Shinsuke OTA <ota@rcnp.osaka-u.ac.jp>
 */


#ifndef TSTREAMINGEVENTSTORE_H_V2
#define TSTREAMINGEVENTSTORE_H_V2


#include "TProcessor.h"
#include "IEventStore.h"
#include "TRunInfo.h"

namespace art {
   namespace v1 {
      class TStreamingEventStore;
      class TStreamingHeaderFS;
      class TStreamingHeaderSTF;
      class TStreamingHeaderTF;
      class TStreamingHeaderHB;
      // class TStreamingHeaderFLTCOIN;
   }
   namespace streaming {
      namespace v1 {
         class TStreamingHeartBeatDelimiter;
      }
   }
   class TRunInfo;
   class TDataSource;
   class TSegmentedData;
   class TEventHeader;
}




class art::v1::TStreamingEventStore : public TProcessor, public IEventStore {
public:
   TStreamingEventStore();
   virtual ~TStreamingEventStore();

   TStreamingEventStore(const TStreamingEventStore& rhs);
   TStreamingEventStore& operator=(const TStreamingEventStore& rhs);

   Int_t GetRunNumber() const {return fPresentRunInfo ? fPresentRunInfo->GetRunNumber() : 0;}
   const char* GetRunName() const {return fPresentRunInfo ?  fPresentRunInfo->GetRunName() : "online";}

   virtual void Init(TEventCollection *col);
   virtual void PreProcess();
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

#if HAVE_ZMQ_H
public:
   virtual bool OpenZmq();
   virtual bool GetZmqUri(const std::string& uri,const std::string& devid,
                                 const std::string& channel,const std::string& subChannel,
                                 std::string& zmqUri);

   Parameter<TString> fZmqURI;
   Parameter<int>     fIsOnline; // flag for online analysis (0 : offline)
#if HAVE_REDIS_H   
   Parameter<TString> fRedisURI;
   Parameter<TString> fDeviceID;
   Parameter<TString> fChannelName;
   Parameter<TString> fSubChannel;
#endif
#endif



protected:
   virtual TRunInfo* GetRunInfo() { return fPresentRunInfo; }
   virtual void NotifyEndOfRun();

   bool fIsMaster;

   TDataSource      *fDataSource; //!
   TRunInfo         *fPresentRunInfo; //!
   char             *fBuffer; //!
   bool              fIsEOB; //!
   std::map<ULong64_t,char*> fSTFBuffer; //!
   uint64_t           fNumSources; //!
   TStreamingHeaderFS *fHeaderFS; //!
   TStreamingHeaderSTF *fHeaderSTF; //!
   TStreamingHeaderTF *fHeaderTF; //!
   TStreamingHeaderHB *fHeaderHB; //!
   streaming::v1::TStreamingHeartBeatDelimiter *fHBD; //!
   // TStreamingHeaderFLTCOIN *fHeaderFLTCOIN; //!
   std::vector<char*> fSubTimeFrameBuffers; //!
   std::vector<int>   fSubTimeFrameSize; //!
   std::vector<TStreamingHeaderSTF*> fSubTimeFrameHeaders; //!
   // TClonesArray *fSTFClones; //!
   
   OutputData<TSegmentedData> fSegmentedData; //!
   OutputData<TEventHeader> fEventHeader; //!
   OutputInfo<TList> fRunHeader; //!
   OutputInfo<TList> fFileHeader; //! 
   Parameter<Int_t> fMaxFrames; //!
   Parameter<Int_t> fStartFrame; //!
   Parameter<StringVec_t> fFileName; //!
   Parameter<Int_t> fFEMType; //! for direct sink from sampler (without header)
   Parameter<Int_t> fFEMID; //! for direct sink from sapmler
   Parameter<Int_t> fDefaultLength; // for direct sink from sampler
   Parameter<Int_t> fIsStandAlone; // for direct sink from sampler

   ClassDef(TStreamingEventStore,1) // streaming data event store
};

#endif // TSTREAMINGEVENTSTORE_H
