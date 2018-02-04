/**
 * @file   TGetEventStore.h
 * @brief  get file event store
 *
 * @date   Created       : 2017-12-21 00:15:51 JST
 *         Last Modified : 2017-12-25 19:24:06 JST (ota)
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

namespace art {
   class TGetEventStore;
   class TSegmentedData;
   class TEventHeader;
   class TRunInfo;
}

class GETDecoder;

class art::TGetEventStore : public TProcessor {
public:
   TGetEventStore();
   virtual ~TGetEventStore();

   TGetEventStore(const TGetEventStore& rhs);
   TGetEventStore& operator=(const TGetEventStore& rhs);

   virtual void Init(TEventCollection *col);
   virtual void Process();

protected:
   virtual TRunInfo* GetRunInfo();

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


   GETDecoder *fGetDecoder; //! get decoder 
   TSegmentedData *fSegmentedData; //! segmented data
   TEventHeader *fEventHeader; //!
   TList *fRunHeaders; //! 
   TClonesArray *fData; //!

   Int_t fDataID; // current data id;

   ClassDef(TGetEventStore,1) // get file event store
};

#endif // INCLUDE_GUARD_UUID_99BC0D11_CDD3_4A53_9F9A_9A6C29085D06
