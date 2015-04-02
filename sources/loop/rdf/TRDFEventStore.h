/**
 * @file   TRDFEventStore.h
 * @brief  RDF Event store
 *
 * @date   Created       : 2014-03-30 09:26:04 JST
 *         Last Modified : Jun 22, 2014 16:11:35 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 Shinsuke OTA
 */

#ifndef INCLUDE_GUARD_UUID_5C7FF7BF_1C04_4946_87BE_F56A4DA5B6ED
#define INCLUDE_GUARD_UUID_5C7FF7BF_1C04_4946_87BE_F56A4DA5B6ED

#include "TProcessor.h"
#include <TClonesArray.h>

namespace art {
   class TRDFEventStore;
   class TDataSource;
   class TModuleDecoderFactory;
   class TSegmentedData;
   class TEventHeader;
}

class art::TRDFEventStore : public TProcessor {
public:
   static const Int_t kMaxBufSize; // default size is 1 MB
   static const Int_t kBlockSize; // 16384 bytes for 1 block

   
   TRDFEventStore();
   virtual ~TRDFEventStore();

   TRDFEventStore(const TRDFEventStore& rhs);
   TRDFEventStore& operator=(const TRDFEventStore& rhs);

   virtual void Init(TEventCollection *col);
   virtual void Process();

protected:
   virtual Bool_t GetNextEvent();
   virtual Bool_t GetNextBlock();
   virtual Bool_t Open();

private:
   Bool_t fIsOnline; // should be false 
   TDataSource *fDataSource;   //! data source

   // for processor parameters
   StringVec_t fFileName; // input file name
   TString     fNameSegmented; // segmented data
   TString     fNameRunHeaders; // name of run headers stored in UserInfo
   TString     fNameEventHeader; // name of event headers stored in Tree


   Long_t fMaxEventNum;
   Long_t fEventNum;
   Long_t fEventNumTotal;

   Char_t *fBuffer; //!
   Int_t fOffset;
   Bool_t fIsEOB;
   
   TString fSourceName;
   TString fSearchPath;


   TString       fSegmentInfoName; // name of segment information
   TClonesArray **fSegmentInfoArray; //-> segment information
   TModuleDecoderFactory *fDecoderFactory; //!
   TObjArray      *fDecoders; //!
   TString       fModuleListName; // name of module information
   TClonesArray **fModuleTypeArray; //-> module information

   TSegmentedData *fSegmentedData; //! segmented data
   TEventHeader   *fEventHeader; //! event header
   TList *fRunHeaders;

   ClassDef(TRDFEventStore,1) // RDF Event store
};

#endif // INCLUDE_GUARD_UUID_5C7FF7BF_1C04_4946_87BE_F56A4DA5B6ED
