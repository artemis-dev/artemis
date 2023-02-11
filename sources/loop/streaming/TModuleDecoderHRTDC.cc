/* $Id:$ */
/**
 * @file   TModuleDecoderHRTDC.h
 * @date   Created : Feb 07, 2023 11:34:00 JST
 *   Last Modified : May 12, 2014 23:26:04 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2023
 */
#include "TModuleDecoderHRTDC.h"

#include "TRawTimingWithEdge.h"
#include "TObjArray.h"


using art::TModuleDecoderHRTDC;
using art::TRawTimingWithEdge;

TModuleDecoderHRTDC::TModuleDecoderHRTDC()
   : TModuleDecoder(kID,TRawTimingWithEdge::Class()) {
}

TModuleDecoderHRTDC::TModuleDecoderHRTDC(Int_t id)
   : TModuleDecoder(id,TRawTimingWithEdge::Class()) {
}

TModuleDecoderHRTDC::~TModuleDecoderHRTDC()
{
}

Int_t TModuleDecoderHRTDC::Decode(char *buf, const int& size, TObjArray *seg)
{
   ULong64_t *evtdata = (ULong64_t*) buf;
   UInt_t evtsize = size/sizeof(ULong64_t);

   Int_t ih = 0;
   

   for (UInt_t iw = 0; iw < evtsize; ++iw) {
      ULong64_t &data = evtdata[iw];
      ih = ((data >> kShiftHeader) & kHeaderMask);


      switch(ih) {
      case kHeaderTDC:
      {
         UInt_t ch   = ((data >> kShiftChannel) & kMaskChannel);
         UInt_t tot  = ((data >> kShiftTOT) & kMaskTOT);
         UInt_t time = ((data >> kShiftChannel) & kMaskChannel);

         TRawTimingWithEdge *odatal = static_cast<TRawTimingWithEdge*>(this->New());
         TRawTimingWithEdge *odatat = static_cast<TRawTimingWithEdge*>(this->New());
         odatal->SetSegInfo(seg->GetUniqueID(),0,ch);
         odatat->SetSegInfo(seg->GetUniqueID(),0,ch);
         odatal->Set(time);
         odatal->SetEdge(TRawTimingWithEdge::kLeading);
         odatat->Set(time + tot);
         odatat->SetEdge(TRawTimingWithEdge::kTrailing);
         
         seg->Add(odatal);
         seg->Add(odatat);
      }
         break;
      case kHeaderHBD:
         break;
      case kHeaderSPND:
         break;
      case kHeaderSPFD:
         break;
      }
   }
   return 0;
}


