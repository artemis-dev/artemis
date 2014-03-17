/*
 * @file TModuleDecoderV1190.cc
 * @date  Created : 2008/11/26 21:34:03 JST<BR>
 *  Last Modified : Mar 17, 2014 13:05:56 JST
 *--------------------------------------------------------
 *    Comment : 
 *              copied from anapaw source and renamed
 *    
 *--------------------------------------------------------
 *    Copyright (C)2008-2013 by Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 */
#include "TModuleDecoderV1190.h"

#include <TRawTimingWithEdge.h>
#include <TObjArray.h>

using art::TModuleDecoderV1190;
using art::TRawTimingWithEdge;

typedef TRawTimingWithEdge V1190Raw_t;

TModuleDecoderV1190::TModuleDecoderV1190()
   : TModuleDecoder(kID,V1190Raw_t::Class()) {
   fHitData = new TObjArray; 
}
TModuleDecoderV1190::~TModuleDecoderV1190()
{
   if (fHitData) delete fHitData;
   fHitData = NULL;
}

Int_t TModuleDecoderV1190::Decode(char* buf, const int &size, TObjArray *seg)
{
   UInt_t *evtdata = (UInt_t*) buf;
   UInt_t evtsize = size/sizeof(UInt_t);
   Int_t ih, igeo, ich;
   Int_t ghf, thf, bncid, evtid, edge, idx;
   V1190Raw_t *data;
   Int_t measure;
   ghf = thf = 0;

   // clear old hits
   fHitData->Clear();
   
   for (Int_t i=0; i<evtsize; ++i) {
      ih = evtdata[i]&kHeaderMask;
      if ((evtdata[i]&kHeaderMask) == kGlobalTrailer) {
         ghf = 0;
      } else  {
         switch (ih) {
         case kGlobalHeader:
            ghf = 1;
            igeo = (evtdata[i]&kMaskGeometry)>>kShiftGeometry;
            break;
         case kTDCHeader:
            if (ghf!=1) return 0;
            bncid = (evtdata[i]&kMaskBunchID)>>kShiftBunchID;
            evtid = (evtdata[i]&kMaskEventCounter)>>kShiftEventCounter;
            break;
         case kTDCMeasurement:
            ich = (evtdata[i]&kMaskChannel) >> kShiftChannel;
            edge = (evtdata[i]&kMaskEdgeType) >> kShiftEdgeType;
            idx = igeo * 128 + ich;
            measure = (evtdata[i]&kMaskMeasure) >> kShiftMeasure;
            
            // check if the data object exists.
            if (fHitData->GetEntriesFast() <= idx || !fHitData->At(idx)) {
               // if no data object is available, create one
               V1190Raw_t *obj = static_cast<V1190Raw_t*>(this->New());
               obj->SetSegInfo(seg->GetUniqueID(),igeo,ich);
               fHitData->AddAtAndExpand(obj,idx);
               seg->Add(obj);
            }
            
            data = static_cast<V1190Raw_t*>(fHitData->At(idx));
            
            data->Set(measure);
            data->SetEdge(!edge); // definition of edge is opposite to that in TRawTimingWithEdge
            fHitData->AddAt(NULL,idx);
            
            break;
         case kTDCTrailer:
            break;
         case kTDCError:
            printf("V1190 [TDC Error    ] : 0x%08x\n", evtdata[i]);
            break;
         }
      }
   }

   return 0;
}
