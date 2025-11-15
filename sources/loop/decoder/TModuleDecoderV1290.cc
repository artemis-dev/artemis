/*
 * @file TModuleDecoderV1290.cc
 * @date  Created : 2008/11/26 21:34:03 JST<BR>
 *  Last Modified : 2023-08-16 16:18:00 JST (fendo)
 *--------------------------------------------------------
 *    Comment :
 *              copied from anapaw source and renamed
 *
 *--------------------------------------------------------
 *    Copyright (C)2008-2013 by Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 */
#include "TModuleDecoderV1290.h"

#include <TRawTimingWithEdge.h>
#include <TObjArray.h>

using art::TModuleDecoderV1290;
using art::TRawTimingWithEdge;

typedef TRawTimingWithEdge V1290Raw_t;

TModuleDecoderV1290::TModuleDecoderV1290()
    : TModuleDecoder(kID, V1290Raw_t::Class())
{
   fHitData = new TObjArray;
}

TModuleDecoderV1290::TModuleDecoderV1290(Int_t id)
    : TModuleDecoder(id, V1290Raw_t::Class())
{
   fHitData = new TObjArray;
}

TModuleDecoderV1290::~TModuleDecoderV1290()
{
   if (fHitData)
      delete fHitData;
   fHitData = NULL;
}

Int_t TModuleDecoderV1290::Decode(char *buf, const int &size, TObjArray *seg)
{
   UInt_t *evtdata = (UInt_t *)buf;
   UInt_t evtsize = size / sizeof(UInt_t);
   Int_t ih, igeo, ich;
   Int_t ghf, thf, bncid, evtid, edge, idx;
   V1290Raw_t *data;
   Int_t measure;
   ghf = thf = 0;
   igeo = -1;

   // clear old hits
   fHitData->Clear();

   for (Int_t i = 0; i < evtsize; ++i)
   {
      ih = evtdata[i] & kHeaderMask;
      if ((evtdata[i] & kHeaderMask) == kGlobalTrailer)
      {
         ghf = 0;
      }
      else
      {
         switch (ih)
         {
         case kGlobalHeader:
            ghf = 1;
            igeo = (evtdata[i] & kMaskGeometry) >> kShiftGeometry;
            break;
         case kTDCHeader:
            if (ghf != 1)
               return 0;
            thf = 1;
            bncid = (evtdata[i] & kMaskBunchID) >> kShiftBunchID;
            evtid = (evtdata[i] & kMaskEventCounter) >> kShiftEventCounter;
            break;
         case kTDCMeasurement:
            if (!ghf || !thf)
               continue;
            ich = (evtdata[i] & kMaskChannel) >> kShiftChannel;
            edge = (evtdata[i] & kMaskEdgeType) >> kShiftEdgeType;
            idx = igeo * 32 + ich;
            if (idx < 0)
            {
               printf("[%03d] igeo = %d, ich = %d\n", i, igeo, ich);
            }
            measure = (evtdata[i] & kMaskMeasure) >> kShiftMeasure;
            // check if the data object exists.
            if (fHitData->GetEntriesFast() <= idx || !fHitData->At(idx))
            {
               // if no data object is available, create one
               V1290Raw_t *obj = static_cast<V1290Raw_t *>(this->New());
               obj->SetSegInfo(seg->GetUniqueID(), igeo, ich);
               fHitData->AddAtAndExpand(obj, idx);
               seg->Add(obj);
            }

            data = static_cast<V1290Raw_t *>(fHitData->At(idx));
            //	    printf("ich = %d geo = %d measure = %f\n",ich,igeo,(float)measure);
            data->Set(measure);
            data->SetEdge(!edge); // definition of edge is opposite to that in TRawTimingWithEdge
            fHitData->AddAt(NULL, idx);

            break;
         case kTDCTrailer:
            thf = 0;
            break;
         case kTDCError:
            // Set header flags to 0 (stop reading the segment)
            // if TDC error flags are fatal
            if (TdcErrorCheck(evtdata[i]))
               ghf = thf = 0;
            if (kWarning >= fVerboseLevel)
            {
               Warning("Decode", "V1290 [TDC Error    ] : 0x%08x", evtdata[i]);
            }
            break;
         }
      }
   }

   return 0;
}
