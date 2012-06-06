/* $Id:$ */
/**
 * @file   TCatV1190Processor.cc
 * @date   Created : Jun 07, 2012 06:07:10 JST
 *   Last Modified : 
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2012
 */
#include "TCatV1190Processor.h"

TCatV1190Processor::TCatV1190Processor()
: fHits(NULL)
{
   fHits = new TClonesArray("TCatSortable");
   fNameOfRawData = "rawdata";
}
TCatV1190Processor::~TCatV1190Processor()
{
   delete fHits;
}

void TCatV1190Processor::Init(TCatEventCollection *col)
{
   fHits->Clear();
   fRaw = col->Get(fNameOfRawData);
}

void TCatV1190Processor::Process()
{
   TArtRawDataObject *refObj = NULL;
   Int_t nSeg = fRaw->GetNumSeg();
   for (Int_t iSeg = 0; iSeg != nSeg; iSeg ++) {
      TArtRawSegmentObject *seg = fRaw->GetSegment(iseg);
      if (seg->GetDevice() != fRefDevice) continue;
      if (seg->GetFp() != fRefFp) continue;
      if (seg->GetDetType() != fRefDetType) continue;

      Int_t nData = seg->GetNumData();
      for (Int_t iData = 0; iData != nData; iData++) {
         TArtRawDataObject *data = seg->GetData(iData);
         if (data->GetGeo() != fRefGeo) continue;
         if (data->GetCh() != fRefCh) continue;
         refObj = data;
         break;
      }
      if (refObj) break;
   }

   for (Int_t iSeg = 0; iSeg != nSeg; iSeg ++) {
      TArtRawSegmentObject *seg = fRaw->GetSegment(iseg);
      if (seg->GetDevice() != fDevice) continue;
      if (seg->GetFp() != fFp) continue;
      if (seg->GetDetType() != fDetType) continue;

      Int_t nData = seg->GetNumData();
      for (Int_t iData = 0; iData != nData; iData++) {
         TArtRawDataObject *data = seg->GetData(iData);
         if (data->GetGeo() != fGeo) continue;
         new ((*fHits)[fHits->GetEntries()]) TCatSortable(data,data->GetVal());
      }
      // treat only one segment
      break;
   }
   fHits->Sort();
   Int_t nData = fHits->GetEntries();
   vector<Int_t> idx;
   for (Int_t i=0; i!=nData; i++) {
      idx.push_back();
   }

   while (idx.size()) {
      vector<int>::iterator it  = idx.begin();
      vector<int>::iterator it2;
      TCatSortable *sortable = (TCatSortable*)v1190hits->At(*it);
      TArtRawDataObject *raw = sortable->GetObject();
      Int_t ch = raw->GetCh();
      Int_t offset = offsetin - tref[raw->GetGeo()];
      it2 = idx.erase(it);
      while (it2 != idx.end()) {
         TCatSortable *sortable2 = (TCatSortable*)v1190hits->At(*it2);
         TArtRawDataObject *raw2 = sortable2->GetObject();
         if (raw->GetGeo() != raw2->GetGeo()) {
            it2++;
            continue;
         } 

         Int_t ch2 = raw2->GetCh();
         TCatHitV1190::EStatus status = TCatHitV1190::kInvalid;
         if (ch == ch2) {
            // invalid data;
            if (ch < trailingOffset) {
               status = TCatHitV1190::kLeading;
               new ((*arr)[arr->GetEntries()]) 
                  TCatHitV1190(raw->GetCategoryID(),
                               raw->GetDetectorID(),
                               raw->GetVal()+offset,0,
                               TCatHitV1190::kLeading);
               break;
            } else {
               status = TCatHitV1190::kTrailing;
               // raw is trailing edge but no matching leading edge
               new ((*arr)[arr->GetEntries()]) 
                  TCatHitV1190(raw->GetCategoryID(),
                               raw->GetDetectorID(),
                               0,raw->GetVal()+offset,
                               TCatHitV1190::kTrailing);
               break;
            }
         } else if (ch == ch2 - trailingOffset) {
            // valid data
            new ((*arr)[arr->GetEntries()]) TCatHitV1190(raw->GetCategoryID(),
                                      raw->GetDetectorID(),
                                      raw->GetVal()+offset,
                                      raw2->GetVal()+offset,
                                      TCatHitV1190::kValid);
            idx.erase(it2);
            break;
         }
         it2++;
      }
      if (it2 == idx.end()) {
         //printf("no matching channel data\n");
         if (ch < trailingOffset) {
            // raw is leading edge but no matching trailing edge
            new ((*arr)[arr->GetEntries()]) 
               TCatHitV1190(raw->GetCategoryID(),
                                      raw->GetDetectorID(),
                                      raw->GetVal()+offset,0,
                                      TCatHitV1190::kLeading);
         } else {
            // raw is trailing edge but no matching leading edge
            new ((*arr)[arr->GetEntries()]) 
               TCatHitV1190(raw->GetCategoryID(),
                                      raw->GetDetectorID(),
                                      0,raw->GetVal()+offset,
                                      TCatHitV1190::kTrailing);
         }
      }
   }

}
