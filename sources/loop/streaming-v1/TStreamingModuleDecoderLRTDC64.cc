/*
 * Created       : 2023-02-11 11:54:42 JST
 * Last Modified : 2023-02-22 16:42:42 JST
 */

#include "TStreamingModuleDecoderLRTDC64.h"
#include "TStreamingHeartBeatDelimiter.h"
#include "TStreamingSpillDelimiter.h"

#include "TRawDataTimingCharge.h"
#include "TClonesArray.h"
#include "TObjArray.h"

// const std::string art::TStreamingModuleDecoderLRTDC64::fgName = "art::TStreamingModuleDecoderLRTDC64";

int art::TStreamingModuleDecoderLRTDC64::fgID = art::TStreamingModuleDecoderLRTDC64::kID;

art::TStreamingModuleDecoderLRTDC64::TStreamingModuleDecoderLRTDC64()
   : art::TStreamingModuleDecoderImpl<art::TStreamingModuleDecoderLRTDC64>()
{
   fTDC = new TClonesArray(art::TRawDataTimingCharge::Class());
   fHBD = new TClonesArray(art::TStreamingHeartBeatDelimiter::Class());
   fSD  = new TClonesArray(art::TStreamingSpillDelimiter::Class());
}

art::TStreamingModuleDecoderLRTDC64::~TStreamingModuleDecoderLRTDC64()
{
   Clear();
   delete fTDC;
   delete fHBD;
   delete fSD;
}

void art::TStreamingModuleDecoderLRTDC64::Clear()
{
   fTDC->Clear("C");
   fHBD->Clear("C");
   fSD->Clear("C");
}


int art::TStreamingModuleDecoderLRTDC64::Decode(char *buf, const int& size, TObjArray *seg, int femid)
{
  static std::map<int,TStreamingSpillDelimiter*> savedSPNDmap; // spill on 
  static std::map<int,TStreamingSpillDelimiter*> savedSPFDmap; // spill on 
  static std::map<int,TStreamingHeartBeatDelimiter*> savedHBDmap; // heartbeat

  TStreamingSpillDelimiter*& savedSPND = savedSPNDmap[femid];
  TStreamingSpillDelimiter*& savedSPFD = savedSPFDmap[femid];
  TStreamingHeartBeatDelimiter*& savedHBD = savedHBDmap[femid];


   // assume 64 bit data
   ULong64_t *evtdata = reinterpret_cast<ULong64_t*>(buf);
   UInt_t evtsize = size/sizeof(ULong64_t);
   
   Int_t ih = 0;

   for (UInt_t iw = 0; iw < evtsize; ++iw) {
      ULong64_t& data = evtdata[iw];
      ih = ((data >> kShiftHeader) & kHeaderMask);

      if (savedHBD && ih != kHeaderHBD) {
         printf("Error in TStreamingModuleDecoderHRTDC : no second kHeaderHBD\n");
         savedHBD = NULL;
         return (iw+1) * sizeof(ULong64_t);
      } 
      
      if (savedSPND && ih != kHeaderSPND) {
         printf("Error in TStreamingModuleDecoderHRTDC : no second kHeaderSPND\n");
         savedSPND = NULL;
      }

      if (savedSPFD && ih != kHeaderSPFD) {
         printf("Error in TStreamingModuleDecoderHRTDC : no second kHeaderSPFD\n");
         savedSPFD = NULL;
         return (iw+1) * sizeof(ULong64_t);
      }

      switch (ih) {
      case kHeaderTDC:
      {
         UInt_t ch = DecodeBits(data,kShiftChannel,kMaskChannel);
         UInt_t tot = DecodeBits(data,kShiftTOT,kMaskTOT);
         UInt_t time = DecodeBits(data,kShiftTime,kMaskTime);

         TRawDataTimingCharge *odatal = static_cast<TRawDataTimingCharge*>(fTDC->ConstructedAt(fTDC->GetEntriesFast()));
         // printf("decoding %lx LR-TDC segid = %d, femid = %x, ch = %d, tdc = %d, tot = %d\n",data, seg->GetUniqueID(),femid,ch,time,tot);

         odatal->SetSegInfo(seg->GetUniqueID(),femid,ch);

         odatal->SetTiming(time);
         odatal->SetCharge(tot);
         seg->Add(odatal);
      }
      break;
      case kHeaderTDCT:
      {
         UInt_t ch = DecodeBits(data,kShiftChannel,kMaskChannel);
         UInt_t tot = DecodeBits(data,kShiftTOT,kMaskTOT);
         UInt_t time = DecodeBits(data,kShiftTime,kMaskTime);

         TRawDataTimingCharge *odatal = static_cast<TRawDataTimingCharge*>(fTDC->ConstructedAt(fTDC->GetEntriesFast()));

         odatal->SetSegInfo(seg->GetUniqueID(),femid,ch);

         odatal->SetTiming(time);
         odatal->SetCharge(tot);
         seg->Add(odatal);
      }
      break;
      case kHeaderTDCT1S:
      {
         UInt_t ch = DecodeBits(data,kShiftChannel,kMaskChannel);
         UInt_t tot = DecodeBits(data,kShiftTOT,kMaskTOT);
         UInt_t time = DecodeBits(data,kShiftTime,kMaskTime);

         TRawDataTimingCharge *odatal = static_cast<TRawDataTimingCharge*>(fTDC->ConstructedAt(fTDC->GetEntriesFast()));

         odatal->SetSegInfo(seg->GetUniqueID(),femid,ch);

         odatal->SetTiming(time);
         odatal->SetCharge(tot);
         seg->Add(odatal);
      }
      break;
      case kHeaderTDCT1E:
      {
         UInt_t ch = DecodeBits(data,kShiftChannel,kMaskChannel);
         UInt_t tot = DecodeBits(data,kShiftTOT,kMaskTOT);
         UInt_t time = DecodeBits(data,kShiftTime,kMaskTime);

         TRawDataTimingCharge *odatal = static_cast<TRawDataTimingCharge*>(fTDC->ConstructedAt(fTDC->GetEntriesFast()));

         odatal->SetSegInfo(seg->GetUniqueID(),femid,ch);

         odatal->SetTiming(time);
         odatal->SetCharge(tot);
         seg->Add(odatal);
      }
      break;
      case kHeaderTDCT2S:
      {
         UInt_t ch = DecodeBits(data,kShiftChannel,kMaskChannel);
         UInt_t tot = DecodeBits(data,kShiftTOT,kMaskTOT);
         UInt_t time = DecodeBits(data,kShiftTime,kMaskTime);

         TRawDataTimingCharge *odatal = static_cast<TRawDataTimingCharge*>(fTDC->ConstructedAt(fTDC->GetEntriesFast()));

         odatal->SetSegInfo(seg->GetUniqueID(),femid,ch);

         odatal->SetTiming(time);
         odatal->SetCharge(tot);
         seg->Add(odatal);
      }
      break;
      case kHeaderTDCT2E:
      {
         UInt_t ch = DecodeBits(data,kShiftChannel,kMaskChannel);
         UInt_t tot = DecodeBits(data,kShiftTOT,kMaskTOT);
         UInt_t time = DecodeBits(data,kShiftTime,kMaskTime);

         TRawDataTimingCharge *odatal = static_cast<TRawDataTimingCharge*>(fTDC->ConstructedAt(fTDC->GetEntriesFast()));

         odatal->SetSegInfo(seg->GetUniqueID(),femid,ch);

         odatal->SetTiming(time);
         odatal->SetCharge(tot);
         seg->Add(odatal);
      }
      break;
      case kHeaderHBD:
      {
         using datatype = TStreamingHeartBeatDelimiter;
         UInt_t flag = DecodeBits(data,kShiftFlag,kMaskFlag);
         UInt_t spill = DecodeBits(data,kShiftSN,kMaskSN);
         UInt_t hb = DecodeBits(data,kShiftHB,kMaskHB);
         
         datatype *hbd = static_cast<datatype*>(fHBD->ConstructedAt(fHBD->GetEntriesFast()));
         hbd->SetSegInfo(seg->GetUniqueID(),femid,fgChannelHBD);
         hbd->SetFlag(flag);
         hbd->SetSpillNumber(spill);
         hbd->SetHeartBeatFrameNumber(hb);
         if (savedHBD) {
            savedHBD = NULL;
            return (iw+1) * sizeof(ULong64_t);
         } else {
            savedHBD = hbd;
         }
         seg->Add(hbd);
      }
      break;
      case kHeaderSPND:
      {
         using datatype = TStreamingSpillDelimiter;
         UInt_t flag = DecodeBits(data,kShiftFlag,kMaskFlag);
         UInt_t spill = DecodeBits(data,kShiftSN,kMaskSN);
         UInt_t hb = DecodeBits(data,kShiftHB,kMaskHB);
         if (!savedSPND) {
            datatype* odata = static_cast<datatype*>(fSD->ConstructedAt(fSD->GetEntriesFast()));
            odata->SetSegInfo(seg->GetUniqueID(),femid,fgChannelSD);
            odata->SetCh(fgChannelSD);
            odata->SetGeo(femid);
            odata->SetFlag(flag);
            odata->SetSpillNumber(spill);
            odata->SetHeartBeatFrameNumber(hb);
            odata->SetOnOff(datatype::kON);
	    seg->Add(odata);

            savedSPND = odata;
         } else {
            savedSPND->SetHeartBeatFrameCounter(hb);
            savedSPND = NULL;
         }
      }
      break;
      case kHeaderSPFD:
      {
         using datatype = TStreamingSpillDelimiter;
         UInt_t flag = DecodeBits(data,kShiftFlag,kMaskFlag);
         UInt_t spill = DecodeBits(data,kShiftSN,kMaskSN);
         UInt_t hb = DecodeBits(data,kShiftHB,kMaskHB);
         if (!savedSPFD) {
            datatype *odata = static_cast<datatype*>(fSD->ConstructedAt(fSD->GetEntriesFast()));
            odata->SetSegInfo(seg->GetUniqueID(),femid,fgChannelSD);
            odata->SetCh(fgChannelSD);
            odata->SetGeo(femid);
            odata->SetFlag(flag);
            odata->SetSpillNumber(spill);
            odata->SetHeartBeatFrameNumber(hb);
            odata->SetOnOff(datatype::kOFF);
            savedSPFD = odata;
	    seg->Add(odata);
         } else {
            savedSPFD->SetHeartBeatFrameCounter(hb);
            savedSPFD = NULL;
         }
      }
      break;
      }
   }
   return 0;
}

