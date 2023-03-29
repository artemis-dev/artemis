/**
 * @file   TModuleDecoderA3100.cc
 * @brief  A3100 decoder
 *
 * @date   Created       : 2014-05-18 23:53:50 JST
 *         Last Modified :
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 KAWASE Shoichiro
 */

#include "TModuleDecoderA3100.h"

#include <TObjArray.h>
#include <TRawDataSimple.h>

using art::TModuleDecoderA3100;

typedef art::TRawDataSimple<UInt_t> A3100Raw_t;

ClassImp(TModuleDecoderA3100)

TModuleDecoderA3100::TModuleDecoderA3100()
  : TModuleDecoder(kID, A3100Raw_t::Class()){
  fHitData = new TObjArray;
}

TModuleDecoderA3100::~TModuleDecoderA3100()
{
  delete fHitData;
  fHitData = NULL;
}

Int_t TModuleDecoderA3100::Decode(char* buf, const Int_t &size, TObjArray *seg)
{
  UInt_t *evtdata = reinterpret_cast<UInt_t*>(buf);
  UInt_t evtsize = size/sizeof(UInt_t);
  Int_t       igeo, ich;
  Int_t       measure;
  A3100Raw_t *data;

  // clear old hits
  fHitData->Clear();

  igeo = 0;
  for (Int_t i=0; i < evtsize; ++i) {
    if ((evtdata[i] & 0x60000000) == 0x60000000) {
      igeo = 1;
      continue;
    }

    ich = (evtdata[i] & 0x0003c000) >> 14;
    measure = evtdata[i] & 0x1fff;

    // check if the data object exists.
    if (fHitData->GetEntriesFast() <= ich || !fHitData->At(ich)) {
      // if no data object is available, create one
      A3100Raw_t *obj = static_cast<A3100Raw_t*>(this->New());
      obj->SetSegInfo(seg->GetUniqueID(),igeo,ich);
      fHitData->AddAtAndExpand(obj,ich);
      seg->Add(obj);
    }

    data = static_cast<A3100Raw_t*>(fHitData->At(ich));
    data->Set(measure);

    fHitData->AddAt(NULL,ich);
  }

  return 0;
}

//TModuleDecoderA3100::TModuleDecoderA3100(const TModuleDecoderA3100& rhs)
//{
//}
//
//TModuleDecoderA3100& TModuleDecoderA3100::operator=(const TModuleDecoderA3100& rhs)
//{
//  if (this != &rhs) {
//
//  }
//  return *this;
//}
