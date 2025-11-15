/*
 * Created       : 2023-02-11 11:54:42 JST
 * Last Modified : 2025-02-25 17:26:46 JST
 */

#include "TStreamingModuleDecoderHRTDC_Unpaired.h"

#include "TClonesArray.h"
#include "TObjArray.h"
#include "TRawTimingWithEdge.h"
#include "TStreamingHeartBeatDelimiterV1.h"

using art::streaming::v1::TStreamingModuleDecoderHRTDC_Unpaired;

int TStreamingModuleDecoderHRTDC_Unpaired::fgID = TStreamingModuleDecoderHRTDC_Unpaired::kID;

TStreamingModuleDecoderHRTDC_Unpaired::TStreamingModuleDecoderHRTDC_Unpaired()
    : art::TStreamingModuleDecoderImpl<TStreamingModuleDecoderHRTDC_Unpaired>() {
   fTDC = new TClonesArray(art::TRawTimingWithEdge::Class());
   fHBD = new TClonesArray(TStreamingHeartBeatDelimiter::Class());

}

TStreamingModuleDecoderHRTDC_Unpaired::~TStreamingModuleDecoderHRTDC_Unpaired() {
   Clear();
   delete fTDC;
   delete fHBD;
}

void TStreamingModuleDecoderHRTDC_Unpaired::Clear() {
   fTDC->Clear("C");
   fHBD->Clear("C");
}

int TStreamingModuleDecoderHRTDC_Unpaired::Decode(char *buf, const int &size,
                                         TObjArray *seg, int femid) {
   // assume 64 bit data
   ULong64_t *evtdata = reinterpret_cast<ULong64_t *>(buf);
   UInt_t evtsize = size / sizeof(ULong64_t);


   Int_t ih = 0;

   for (UInt_t iw = 0; iw < evtsize; ++iw) {
      ULong64_t &data = evtdata[iw];
      // printf("%lx\n",data);
      ih = ((data >> kShiftHeader) & kHeaderMask);

      // printf("ih = %x iw %d evtsize %u\n",ih,iw,evtsize);

      switch (ih) {
         case kHeaderTDCL:
         case kHeaderTDCT:
         case kHeaderTDCT1S:
         case kHeaderTDCT1E:
         case kHeaderTDCT2S:
         case kHeaderTDCT2E: {
            UInt_t ch = DecodeBits(data, kShiftChannel, kMaskChannel);
            UInt_t tot = DecodeBits(data, kShiftTOT, kMaskTOT);
            UInt_t time = DecodeBits(data, kShiftTime, kMaskTime);

            TRawTimingWithEdge *odatal = static_cast<TRawTimingWithEdge *>(
                fTDC->ConstructedAt(fTDC->GetEntriesFast()));

            odatal->SetSegInfo(seg->GetUniqueID(), femid, ch);

            // printf("decoding %lx HR-TDC segid = %d, femid = %x, ch = %d, tdc
            // = %d, tot = %d\n",data, seg->GetUniqueID(),femid,ch,time,tot);
            // printf("ch = %d, femid = %x, time = %d, edge = %x \n",ch,femid,time,ih);

            odatal->SetTiming(time);
            odatal->SetEdge(kHeaderTDCL == ih); // true if leading edge
            seg->Add(odatal);
         } break;
         case kHeaderHBD: {
            using datatype = TStreamingHeartBeatDelimiter;
            datatype *hbd = static_cast<datatype *>(
                fHBD->ConstructedAt(fHBD->GetEntriesFast()));
            hbd->SetSegInfo(seg->GetUniqueID(), femid, fgChannelHBD);
            hbd->Decode(data);
            // printf("decoding %lx HR-TDC segid = %d, femid = %x, ch = %d,spill
            // = %d, hb = %d\n",data,
            // seg->GetUniqueID(),femid,fgChannelHBD,spill,hb);
            if (hbd->IsDelim1(data)) {
               seg->Add(hbd);
            }
         } break;
      }
   }
   return 0;
}
