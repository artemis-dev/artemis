/**
 *   Created       : 2023-02-09 08:41:43 JST
 *   Last Modified : 2024/04/12 23:34:33
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
      // class TStreamingHeaderFLTCOIN;
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

   Int_t GetRunNumber() const {return fPresentRunInfo->GetRunNumber();}
   const char* GetRunName() const {return fPresentRunInfo->GetRunName();}

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



protected:
   virtual TRunInfo* GetRunInfo() { return fPresentRunInfo; }
   virtual void NotifyEndOfRun();

   bool fIsMaster;

   TDataSource      *fDataSource; //!
   TRunInfo         *fPresentRunInfo; //!
   char             *fBuffer; //!
   bool              fIsEOB; //!
   std::map<ULong64_t,char*> fSTFBuffer; //!
   int               fNumSources; //!
   TStreamingHeaderFS *fHeaderFS; //!
   TStreamingHeaderSTF *fHeaderSTF; //!
   TStreamingHeaderTF *fHeaderTF; //!
   // TStreamingHeaderFLTCOIN *fHeaderFLTCOIN; //!
   std::vector<char*> fSubTimeFrameBuffers; //!
   std::vector<int>   fSubTimeFrameSize; //!
   std::vector<TStreamingHeaderSTF*> fSubTimeFrameHeaders; //!
   // TClonesArray *fSTFClones; //!
   
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
   Parameter<Int_t> fIsStandAlone; // for direct sink from sampler

   ClassDef(TStreamingEventStore,1) // streaming data event store
};

#endif // TSTREAMINGEVENTSTORE_H
