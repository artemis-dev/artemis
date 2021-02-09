/**
 * @file   TGetEventStore.h
 * @brief  get file event store
 *
 * @date   Created       : 2017-12-21 00:15:51 JST
 *         Last Modified : 2021-01-17 22:23:24 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2017 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_99BC0D11_CDD3_4A53_9F9A_9A6C29085D06
#define INCLUDE_GUARD_UUID_99BC0D11_CDD3_4A53_9F9A_9A6C29085D06

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef HAVE_MPI_H
#include <mpi.h>
#endif

#include "TProcessor.h"
#include "IEventStore.h"

namespace art {
   class TGetEventStore;
   class TSegmentedData;
   class TEventHeader;
   class TRunInfo;
}

class GETDecoder;
class GETBasicFrame;


class art::TGetEventStore : public TProcessor, public IEventStore {
public:
   static const Int_t kFPNIDs[4]; // 11, 22, 45, 56

   TGetEventStore();
   virtual ~TGetEventStore();

   TGetEventStore(const TGetEventStore& rhs);
   TGetEventStore& operator=(const TGetEventStore& rhs);

   Int_t GetRunNumber() const ;
   const char* GetRunName() const;
   
   

   virtual void Init(TEventCollection *col);
   virtual void Process();
   virtual void ProcessFullCobo();
   virtual void ProcessReducedCobo();
   virtual void ProcessAsAd(GETBasicFrame *asad);
   

protected:
   virtual TRunInfo* GetRunInfo();
   Parameter<bool> fIsReducedCobo;
   

#if USE_MPI
   Int_t fNPE;//!
   Int_t fRankID;//!
   Int_t fUseMPI;//!
#endif   

private:
   StringVec_t fFileName;
   TString fNameSegmented;
   TString fNameRunHeaders;
   TString fNameEventHeader;
   Int_t fMaxEventNum; // maximum number of event
   Int_t fRequireHitBit; // require hit bit if 1
   Bool_t fSubtractFPN; // flag for subtraction of FPN (default 0)
   IntVec_t fValidBucket; // range of valid time bucket (default [0,0])
   Int_t fStartEventNum; // start number of event

   GETDecoder *fGetDecoder; //! get decoder 
   TSegmentedData *fSegmentedData; //! segmented data
   TEventHeader *fEventHeader; //!
   TList *fRunHeaders; //! 
   TClonesArray *fData; //!

   Int_t fDataID; // current data id;

   Int_t fFPNID[68]; //! FPN ID for subtraction
   Int_t *fFPN[4]; //! FPN buffer

   ClassDef(TGetEventStore,1) // get file event store
};

#endif // INCLUDE_GUARD_UUID_99BC0D11_CDD3_4A53_9F9A_9A6C29085D06
