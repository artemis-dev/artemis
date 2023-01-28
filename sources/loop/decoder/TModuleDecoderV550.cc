/**
 * @file   TModuleDecoderV550.cc
 * @brief  V550 decoder
 *
 * @date   Created       : 2014-05-18 23:53:50 JST
 *         Last Modified :
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 KAWASE Shoichiro
 */

#include "TModuleDecoderV550.h"

#include <TObjArray.h>
#include <TRawDataSimple.h>

//#define DB_PRINTF 1

using art::TModuleDecoderV550;

typedef art::TRawDataSimple<UInt_t> V550Raw_t;

ClassImp(TModuleDecoderV550)

TModuleDecoderV550::TModuleDecoderV550()
  : TModuleDecoder(kID, V550Raw_t::Class()){
  fHitData = new TObjArray;
}

TModuleDecoderV550::~TModuleDecoderV550()
{
  delete fHitData;
  fHitData = NULL;
}

Int_t TModuleDecoderV550::Decode(char* buf, const Int_t &size, TObjArray *seg)
{
  UInt_t *evtdata = reinterpret_cast<UInt_t*>(buf);
  UInt_t evtsize = size/sizeof(UInt_t);
  Int_t       igeo, ich;
  Int_t       measure;
  V550Raw_t *data;
  Int_t nChannel, fifoid;

  // clear old hits
  fHitData->Clear();
#ifdef DB_PRINTF
  printf("evtsize %d\n", evtsize);
#endif
  for (int i=0,j=0; i<evtsize; i++,j--) {
    if (!j) {
      igeo = (evtdata[i]&kGeoMask)>>kGeoShift;
      j    = (evtdata[i]&kNDataMask)>>kNDataShift;
      ++j;
      nChannel = ((evtdata[i]&kNChannelMask)>>kNChannelShift) * kDCNUnit; 
#ifdef DB_PRINTF
      printf("igeo %d j %d nChannel%d\n", igeo, j, nChannel);
#endif
    } else {
      ich = (evtdata[i]&kChannelMask)>>kChannelShift;
      fifoid = (evtdata[i]&kFifoIdMask)>>kFifoIdShift;
      if (fifoid==1) ich += nChannel;
      measure = (evtdata[i]&kAdcMask)>>kAdcShift;
      // check if the data object exists.
      if (fHitData->GetEntriesFast() <= ich || !fHitData->At(ich)) {
        // if no data object is available, create one
        V550Raw_t *obj = static_cast<V550Raw_t*>(this->New());
        obj->SetSegInfo(seg->GetUniqueID(),igeo,ich);
        fHitData->AddAtAndExpand(obj,ich);
        seg->Add(obj);
      }

      data = static_cast<V550Raw_t*>(fHitData->At(ich));
      data->Set(measure);

#ifdef DB_PRINTF
      printf("  ich %d fifoid %d measure %d\n", ich, fifoid, measure);
#endif
    
      fHitData->AddAt(NULL,ich);
      }
    }
  return 0;
}

//TModuleDecoderV550::TModuleDecoderV550(const TModuleDecoderV550& rhs)
//{
//}
//
//TModuleDecoderV550& TModuleDecoderV550::operator=(const TModuleDecoderV550& rhs)
//{
//  if (this != &rhs) {
//
//  }
//  return *this;
//}
