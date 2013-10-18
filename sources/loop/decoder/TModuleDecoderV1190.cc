/*
 * @file TModuleDecoderV1190.cc
 * @date  Created : 2008/11/26 21:34:03 JST<BR>
 *  Last Modified : Jul 23, 2013 09:33:23 JST
 *--------------------------------------------------------
 *    Comment : 
 *              copied from anapaw source and renamed
 *    
 *--------------------------------------------------------
 *    Copyright (C)2008-2013 by Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 */
#include "TModuleDecoderV1190.h"

#include <TRawDataV1190.h>
#include <TObjArray.h>

art::TModuleDecoderV1190::TModuleDecoderV1190()
   : TModuleDecoder(kID,TRawDataV1190::Class()) {
   // array length should be 128 (maximum number of channel)
   fHitData = new TObjArray; 
}
art::TModuleDecoderV1190::~TModuleDecoderV1190()
{
   if (fHitData) delete fHitData;
   fHitData = NULL;
}

Int_t art::TModuleDecoderV1190::Decode(char* buf, const int &size, TObjArray *seg)
{
   unsigned int *evtdata = (unsigned int*) buf;
   unsigned int evtsize = size/sizeof(unsigned int);
   int ih, igeo, ich;
   int ghf, thf, bncid, evtid, edge, idx;
   TRawDataV1190 *data;
   Int_t measure;
   ghf = thf = 0;

   // clear old hits
   fHitData->Clear();
   
   for (int i=0; i<evtsize; i++) {
      ih = evtdata[i]&kHeaderMask;
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
            TObject *obj = New();
            ((TRawDataV1190*)obj)->SetSegInfo(seg->GetUniqueID(),igeo,ich);
            fHitData->AddAtAndExpand(obj,idx);
            seg->Add(obj);
         }
         
         data = (TRawDataV1190*)fHitData->At(idx);

         if (edge) {
            // incoming data is trailing edge
            data->SetTrailing(measure);
            fHitData->AddAt(NULL,idx);
         } else {
            // if the data is leading edge, just set value
            data->SetLeading(measure);
         }
         break;
      case kTDCTrailer:
         break;
      case kTDCError:
         printf("V1190 [TDC Error    ] : 0x%08x\n", evtdata[i]);
         break;
      case kGlobalTrailer:
         ghf = 0;
         break;
      }
   }

   return 0;
}
