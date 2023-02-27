/*
 * Created       : 2023-02-11 11:54:42 JST
 * Last Modified : 2023-02-18 16:08:57 JST
 */

#include "TStreamingModuleDecoderHRTDC.h"
#include "TStreamingHeartBeatDelimiter.h"
#include "TStreamingSpillDelimiter.h"

#include "TRawTimingWithEdge.h"
#include "TClonesArray.h"
#include "TObjArray.h"

const std::string art::TStreamingModuleDecoderHRTDC::fgName = "art::TStreamingModuleDecoderHRTDC";
//const int art::TStreamingModuleDecoderHRTDC::fgChannelHBD = 128;
//const int art::TStreamingModuleDecoderHRTDC::fgChannelSD = 129;

int art::TStreamingModuleDecoderHRTDC::fgID = art::TStreamingModuleDecoderHRTDC::kID;

art::TStreamingModuleDecoderHRTDC::TStreamingModuleDecoderHRTDC()
   : art::TStreamingModuleDecoderImpl<art::TStreamingModuleDecoderHRTDC>()
{
   fTDC = new TClonesArray(art::TRawTimingWithEdge::Class());
   fHBD = new TClonesArray(art::TStreamingHeartBeatDelimiter::Class());
   fSD  = new TClonesArray(art::TStreamingSpillDelimiter::Class());
}

art::TStreamingModuleDecoderHRTDC::~TStreamingModuleDecoderHRTDC()
{
   Clear();
   delete fTDC;
   delete fHBD;
   delete fSD;
}

void art::TStreamingModuleDecoderHRTDC::Clear()
{
   fTDC->Clear("C");
   fHBD->Clear("C");
   fSD->Clear("C");
}


int art::TStreamingModuleDecoderHRTDC::Decode(char *buf, const int& size, TObjArray *seg, int femid)
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

   // 何れにせよ HBD か SPFD まで読むことになるので、そこまで読めたかどうかを返すようにすれば良さそう
   // 読めた場合は使った分の長さを返して、RearchAtEndDelimiter とかで状態を教えるようにすればよいか？
   // EventStore は使わなかった分もどってしまえばよい。

   Int_t ih = 0;

   for (UInt_t iw = 0; iw < evtsize; ++iw) {
      ULong64_t& data = evtdata[iw];
      // printf("%lx\n",data);
      ih = ((data >> kShiftHeader) & kHeaderMask);

//      printf("ih = %x iw %d evtsize %u\n",ih,iw,evtsize);

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

         TRawTimingWithEdge *odatal = static_cast<TRawTimingWithEdge*>(fTDC->ConstructedAt(fTDC->GetEntriesFast()));
         TRawTimingWithEdge *odatat = static_cast<TRawTimingWithEdge*>(fTDC->ConstructedAt(fTDC->GetEntriesFast()));

         odatal->SetSegInfo(seg->GetUniqueID(),femid,ch);
         odatat->SetSegInfo(seg->GetUniqueID(),femid,ch);

         // printf("decoding %lx HR-TDC segid = %d, femid = %x, ch = %d, tdc = %d, tot = %d\n",data, seg->GetUniqueID(),femid,ch,time,tot);
         

         odatal->Set(time);
         odatat->Set(time + tot);
         odatal->SetEdge(TRawTimingWithEdge::kLeading);
         odatat->SetEdge(TRawTimingWithEdge::kTrailing);

         seg->Add(odatal);
         seg->Add(odatat);
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
            savedSPND = odata;
            seg->Add(odata);
#if 0            
            printf("decoding %lx, flag = %d, spill = %d, hbfn = %d, onoff = %d\n",
                   data,
                   flag, spill, hb, datatype::kON);
#endif            
         } else {
#if 0            
            printf("decoding %lx, hbfc = %d\n",data,hb);
#endif            
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

