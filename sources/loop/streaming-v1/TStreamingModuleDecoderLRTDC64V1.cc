/*
   * Created       : 2023-02-11 11:54:42 JST
   * Last Modified : 2024/04/13 02:49:00
   */

#include "TClonesArray.h"
#include "TObjArray.h"
#include "TRawDataTimingCharge.h"
#include "TStreamingHeartBeatDelimiterV1.h"
#include "TStreamingModuleDecoderLRTDC64V1.h"


    // const std::string art::TStreamingModuleDecoderLRTDC64::fgName =
    // "art::TStreamingModuleDecoderLRTDC64";

    using art::streaming::v1::TStreamingModuleDecoderLRTDC64;

int TStreamingModuleDecoderLRTDC64::fgID = TStreamingModuleDecoderLRTDC64::kID;

TStreamingModuleDecoderLRTDC64::TStreamingModuleDecoderLRTDC64()
    : art::TStreamingModuleDecoderImpl<art::streaming::v1::TStreamingModuleDecoderLRTDC64>() {
   fTDC = new TClonesArray(art::TRawDataTimingCharge::Class());
   fHBD = new TClonesArray(art::streaming::v1::TStreamingHeartBeatDelimiter::Class());
}

TStreamingModuleDecoderLRTDC64::~TStreamingModuleDecoderLRTDC64() {
   Clear();
   delete fTDC;
   delete fHBD;
}

void TStreamingModuleDecoderLRTDC64::Clear() {
   fTDC->Clear("C");
   fHBD->Clear("C");
}

int TStreamingModuleDecoderLRTDC64::Decode(char* buf, const int& size,
                                           TObjArray* seg, int femid) {
   // assume 64 bit data
   ULong64_t* evtdata = reinterpret_cast<ULong64_t*>(buf);
   UInt_t evtsize = size / sizeof(ULong64_t);

   Int_t ih = 0;

   for (UInt_t iw = 0; iw < evtsize; ++iw) {
      ULong64_t& data = evtdata[iw];
      ih = ((data >> kShiftHeader) & kHeaderMask);

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

            TRawDataTimingCharge* odatal = static_cast<TRawDataTimingCharge*>(
                fTDC->ConstructedAt(fTDC->GetEntriesFast()));
            // printf("decoding %lx LR-TDC segid = %d, femid = %x, ch = %d, tdc
            // = %d, tot = %d\n",data, seg->GetUniqueID(),femid,ch,time,tot);

            odatal->SetSegInfo(seg->GetUniqueID(), femid, ch);

            odatal->SetTiming(time);
            odatal->SetCharge(tot);
            seg->Add(odatal);
         } break;
         case kHeaderHBD: {
            using datatype = TStreamingHeartBeatDelimiter;
            UInt_t flag = DecodeBits(data, kShiftFlag, kMaskFlag);
            UInt_t spill = DecodeBits(data, kShiftSN, kMaskSN);
            UInt_t hb = DecodeBits(data, kShiftHB, kMaskHB);

            datatype* hbd = static_cast<datatype*>(
                fHBD->ConstructedAt(fHBD->GetEntriesFast()));
            hbd->Decode(data);
            hbd->SetSegInfo(seg->GetUniqueID(), femid, fgChannelHBD);
            if (hbd->IsDelim1(data)) {
               seg->Add(hbd);
            }
         } break;
      }
   }
   return 0;
}
