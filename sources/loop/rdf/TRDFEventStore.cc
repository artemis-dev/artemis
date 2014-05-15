/**
 * @file   TRDFEventStore.cc
 * @brief  RDF Event Store
 *
 * @date   Created       : 2014-03-30 10:56:06 JST
 *         Last Modified : Mar 30, 2014 13:40:57 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 Shinsuke OTA
 */

#include "TRDFEventStore.h"

using art::TRDFEventStore;

ClassImp(TRDFEventStore)

TRDFEventStore::TRDFEventStore()
{
   StringVec_t dummy;
   RegisterInputCollection("InputFiles","The names of input files",fFileName,dummy);
   RegisterOutputCollection("SegmentedData","The name of output array for segmented data",
                            fNameSegmented,TString("segdata"));
   RegisterOutputCollection("RunHeadersName","the name of output array for run headers, run header will be stored once",
                            fNameRunHeaders,TString("runheader"));
   RegisterOutputCollection("EventHeaderName","the name of event header",
                            fNameEventHeader,TString("eventheader"));
}

TRDFEventStore::~TRDFEventStore()
{
}

TRDFEventStore::TRDFEventStore(const TRDFEventStore& rhs)
{
}

TRDFEventStore& TRDFEventStore::operator=(const TRDFEventStore& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}

void TRDFEventStore::Init(TEventCollection *col)
{
   Int_t n = fFileName.size();
   for (Int_t i=0; i!= n; i++) {
      printf("file = %s\n",fFileName[i].Data());
   }
}

void TRDFEventStore::Process()
{
   // clear segment data
   // loop over the events if exist
   while (!GetNextEvent()) {
   // get next block
      while (!GetNextBlock()) {
         if ((*fCondition)->IsSet(TLoop::kStopLoop)) {
            SetStopEvent();
            return;
         }
         // try to open data source if no block is available 
         if (!Open()) {
            // loop is end if no data source is available
            SetStopEvent();
            SetStopLoop();
            SetEndOfRun();
            return;
         }
      }
   }
}

Bool_t TRDFEventStore::GetNextEvent()
{
   return kFALSE;
}

Bool_t TRDFEventStore::GetNextBlock()
{
   return kFALSE;
}
Bool_t TRDFEventStore::Open()
{
   return kFALSE;
}
