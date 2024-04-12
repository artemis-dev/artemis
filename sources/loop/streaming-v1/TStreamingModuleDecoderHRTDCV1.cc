/*
 * Created       : 2023-02-11 11:54:42 JST
 * Last Modified : 2024/04/13 02:49:06
 */

#include "TStreamingModuleDecoderHRTDCV1.h"

#include "TClonesArray.h"
#include "TObjArray.h"
#include "TRawDataTimingCharge.h"
#include "TStreamingHeartBeatDelimiterV1.h"

using art::streaming::v1::TStreamingModuleDecoderHRTDC;

int TStreamingModuleDecoderHRTDC::fgID = TStreamingModuleDecoderHRTDC::kID;

TStreamingModuleDecoderHRTDC::TStreamingModuleDecoderHRTDC()
    : art::TStreamingModuleDecoderImpl<TStreamingModuleDecoderHRTDC>() {
   fTDC = new TClonesArray(TRawDataTimingCharge::Class());
   fHBD = new TClonesArray(TStreamingHeartBeatDelimiter::Class());
}

TStreamingModuleDecoderHRTDC::~TStreamingModuleDecoderHRTDC() {
   Clear();
   delete fTDC;
   delete fHBD;
}

void TStreamingModuleDecoderHRTDC::Clear() {
   fTDC->Clear("C");
   fHBD->Clear("C");
}

int TStreamingModuleDecoderHRTDC::Decode(char *buf, const int &size,
                                         TObjArray *seg, int femid) {
   // assume 64 bit data
   ULong64_t *evtdata = reinterpret_cast<ULong64_t *>(buf);
   UInt_t evtsize = size / sizeof(ULong64_t);

   // 何れにせよ HBD か SPFD
   // まで読むことになるので、そこまで読めたかどうかを返すようにすれば良さそう
   // 読めた場合は使った分の長さを返して、RearchAtEndDelimiter
   // とかで状態を教えるようにすればよいか？ EventStore
   // は使わなかった分もどってしまえばよい。

   Int_t ih = 0;

   for (UInt_t iw = 0; iw < evtsize; ++iw) {
      ULong64_t &data = evtdata[iw];
      // printf("%lx\n",data);
      ih = ((data >> kShiftHeader) & kHeaderMask);

      // printf("ih = %x iw %d evtsize %u\n",ih,iw,evtsize);

      switch (ih) {
         case kHeaderTDC:
         case kHeaderTDCT:
         case kHeaderTDCT1S:
         case kHeaderTDCT1E:
         case kHeaderTDCT2S:
         case kHeaderTDCT2E: {
            UInt_t ch = DecodeBits(data, kShiftChannel, kMaskChannel);
            UInt_t tot = DecodeBits(data, kShiftTOT, kMaskTOT);
            UInt_t time = DecodeBits(data, kShiftTime, kMaskTime);

            TRawDataTimingCharge *odatal = static_cast<TRawDataTimingCharge *>(
                fTDC->ConstructedAt(fTDC->GetEntriesFast()));

            odatal->SetSegInfo(seg->GetUniqueID(), femid, ch);

            // printf("decoding %lx HR-TDC segid = %d, femid = %x, ch = %d, tdc
            // = %d, tot = %d\n",data, seg->GetUniqueID(),femid,ch,time,tot);

            odatal->SetTiming(time);
            odatal->SetCharge(tot);
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
