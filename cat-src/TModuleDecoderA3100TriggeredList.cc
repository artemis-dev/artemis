/**
 * @file   TModuleDecoderA3100TriggeredList.cc
 * @brief  A3100 decoder for Triggered List mode
 *
 * @author Rin Yokoyama <yokoyama@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2016 Rin Yokoyama
 */

#include "TModuleDecoderA3100TriggeredList.h"

#include <TObjArray.h>
#include <TRawDataTriggeredList.h>

using art::TModuleDecoderA3100TriggeredList;

typedef art::TRawDataTriggeredList A3100Raw_t;

ClassImp(TModuleDecoderA3100TriggeredList)

TModuleDecoderA3100TriggeredList::TModuleDecoderA3100TriggeredList()
  : TModuleDecoder(kID, A3100Raw_t::Class()){
  fHitData = new TObjArray;
}

TModuleDecoderA3100TriggeredList::~TModuleDecoderA3100TriggeredList()
{
  delete fHitData;
  fHitData = NULL;
}

Int_t TModuleDecoderA3100TriggeredList::Decode(char* buf, const Int_t &size, TObjArray *seg)
{
  UInt_t *evtdata = reinterpret_cast<UInt_t*>(buf);
  UInt_t evtsize = size/sizeof(UInt_t);
  Int_t       igeo, ich;
  Int_t       adc;
  Int_t       tsi_hi;
  Int_t       tsi_lo;
  Int_t       evtcnt;
  A3100Raw_t *data;
//  Info("Decode","new event");
  // clear old hits
  fHitData->Clear();

  igeo = 0;
  for (Int_t i=0; i < evtsize; ++i) {
    if ((evtdata[i] & kHeaderMask) == kFirstWord) {
      adc = evtdata[i] & kADCMask;
      tsi_hi = ((evtdata[i] & kTSI_HiMask) >> kTSI_HiShift);
      ich = ((evtdata[i] & kChMask) >> kChShift);
      continue;
    }
    else if ((evtdata[i] & kHeaderMask) == kSecondWord) {
      tsi_lo = (evtdata[i] & kTSI_LoMask);
      continue;
    }
    else if ((evtdata[i] & kHeaderMask) == kThiredWord) {
      evtcnt = (evtdata[i] & kEventCountMask);
      igeo = 1;
    }
    else {
      continue;
    }

    // check if the data object exists.
//    if (fHitData->GetEntriesFast() <= ich || !fHitData->At(ich)) {
      // if no data object is available, create one
       //printf("new ich = %d # = %d %p\n",ich,fHitData->GetEntriesFast(),fHitData->At(ich));
      A3100Raw_t *obj = static_cast<A3100Raw_t*>(this->New());
      obj->SetSegInfo(seg->GetUniqueID(),igeo,ich);
//      fHitData->AddAtAndExpand(obj,ich);
      seg->Add(obj);
//    }

      data = obj;
      data->SetADC(adc);
      data->SetTSI(tsi_hi,tsi_lo);
      data->SetEventCount(evtcnt);
    //printf("ich %d adc %d tsi_hi %d tsi_lo %d evtcnt %d\n", ich, adc, tsi_hi, tsi_lo, evtcnt);

    fHitData->AddAt(NULL,ich);
  }

  return 0;
}

//TModuleDecoderA3100TriggeredList::TModuleDecoderA3100(const TModuleDecoderA3100& rhs)
//{
//}
//
//TModuleDecoderA3100TriggeredList& TModuleDecoderA3100::operator=(const TModuleDecoderA3100& rhs)
//{
//  if (this != &rhs) {
//
//  }
//  return *this;
//}
