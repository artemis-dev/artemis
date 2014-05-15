/**
 * @file   TRDFEventStore.h
 * @brief  RDF Event store
 *
 * @date   Created       : 2014-03-30 09:26:04 JST
 *         Last Modified : Mar 30, 2014 13:37:07 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_5C7FF7BF_1C04_4946_87BE_F56A4DA5B6ED
#define INCLUDE_GUARD_UUID_5C7FF7BF_1C04_4946_87BE_F56A4DA5B6ED

#include "TProcessor.h"

namespace art {
   class TRDFEventStore;
}

class art::TRDFEventStore : public TProcessor {
public:
   TRDFEventStore();
   virtual ~TRDFEventStore();

   TRDFEventStore(const TRDFEventStore& rhs);
   TRDFEventStore& operator=(const TRDFEventStore& rhs);

protected:
   virtual Bool_t GetNextEvent();
   virtual Bool_t GetNextBlock();
   virtual Bool_t Open();

private:
   StringVec_t fFileName; // input file name
   TString     fNameSegmented; // segmented data
   TString     fNameRunHeaders; // name of run headers stored in UserInfo
   TString     fNameEventHeader; // name of event headers stored in Tree

   Long_t fMaxEventNum;
   Long_t fEventNum;
   Long_t fEventNumTotal;

   Int_t fMaxBufSize;
   Char_t *fBuffer;
   Int_t fOffset;
   Int_t fBlockSize;


   

   ClassDef(TRDFEventStore,1) // RDF Event store
};

#endif // INCLUDE_GUARD_UUID_5C7FF7BF_1C04_4946_87BE_F56A4DA5B6ED
