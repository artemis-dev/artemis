/*
 * Created       : 2023-02-11 11:54:42 JST
 * Last Modified : 2023-02-22 16:42:42 JST
 */

#include "TStreamingModuleDecoderLRTDC.h"
#include "TStreamingHeartBeatDelimiter.h"
#include "TStreamingSpillDelimiter.h"

#include "TRawTimingWithEdge.h"
#include "TClonesArray.h"
#include "TObjArray.h"

const std::string art::TStreamingModuleDecoderLRTDC::fgName = "art::TStreamingModuleDecoderLRTDC";

int art::TStreamingModuleDecoderLRTDC::fgID = art::TStreamingModuleDecoderLRTDC::kID;

art::TStreamingModuleDecoderLRTDC::TStreamingModuleDecoderLRTDC()
   : art::TStreamingModuleDecoderImpl<art::TStreamingModuleDecoderLRTDC>()
{
   fTDC = new TClonesArray(art::TRawTimingWithEdge::Class());
   fHBD = new TClonesArray(art::TStreamingHeartBeatDelimiter::Class());
   fSD  = new TClonesArray(art::TStreamingSpillDelimiter::Class());
}

art::TStreamingModuleDecoderLRTDC::~TStreamingModuleDecoderLRTDC()
{
   Clear();
   delete fTDC;
   delete fHBD;
   delete fSD;
}

void art::TStreamingModuleDecoderLRTDC::Clear()
{
   fTDC->Clear("C");
   fHBD->Clear("C");
   fSD->Clear("C");
}


int art::TStreamingModuleDecoderLRTDC::Decode(char *buf, const int& size, TObjArray *seg, int femid)
{
   static TStreamingSpillDelimiter* savedSPND = NULL; // spill on 
   static TStreamingSpillDelimiter* savedSPFD = NULL; // spill off
   static TStreamingHeartBeatDelimiter* savedHBD  = NULL; // heartbeat
   // assume 64 bit data
   ULong64_t *evtdata = reinterpret_cast<ULong64_t*>(buf);
   UInt_t evtsize = size/sizeof(ULong64_t);
   

   Int_t ih = 0;

   for (UInt_t iw = 0; iw < evtsize; ++iw) {
      ULong64_t& data = evtdata[iw];
      ih = ((data >> kShiftHeader) & kHeaderMask);

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

         odatal->Set(time);
         odatat->Set(time + tot);
         odatal->SetEdge(TRawTimingWithEdge::kLeading);
         odatal->SetEdge(TRawTimingWithEdge::kTrailing);

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
         UInt_t spill = DecodeBits(data,kShiftFlag,kMaskFlag);
         UInt_t hb = DecodeBits(data,kShiftFlag,kMaskFlag);
         if (!savedSPND) {
            datatype* odata = static_cast<datatype*>(fSD->ConstructedAt(fSD->GetEntriesFast()));
            odata->SetCh(fgChannelSD);
            odata->SetGeo(femid);
            odata->SetFlag(flag);
            odata->SetSpillNumber(spill);
            odata->SetHeartBeatFrameNumber(hb);
            odata->SetOnOff(datatype::kON);
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
         UInt_t spill = DecodeBits(data,kShiftFlag,kMaskFlag);
         UInt_t hb = DecodeBits(data,kShiftFlag,kMaskFlag);
         if (!savedSPFD) {
            datatype *odata = static_cast<datatype*>(fSD->ConstructedAt(fSD->GetEntriesFast()));
            odata->SetCh(fgChannelSD);
            odata->SetGeo(femid);
            odata->SetFlag(flag);
            odata->SetSpillNumber(spill);
            odata->SetHeartBeatFrameNumber(hb);
            odata->SetOnOff(datatype::kOFF);
            savedSPFD = odata;
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

