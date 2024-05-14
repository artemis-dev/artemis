/**
 * @file   TModuleDecoderFERET.cc
 * @brief  Decorder Class for V7XX
 *
 * @date   Created:       2013-07-24 14:41:31
 *         Last Modified: 2024-03-23 20:10:00 JST
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved.
 */

#include "TModuleDecoderFERET.h"

#include <TObjArray.h>

#include <TRawTiming.h>

using art::TModuleDecoderFERET;
using art::TRawTiming;

typedef TRawTiming V7XXRaw_t;

TModuleDecoderFERET::TModuleDecoderFERET()
   : TModuleDecoder(kID, V7XXRaw_t::Class()){
   // fHitData = new TObjArray;
   fHitData = nullptr;
}

TModuleDecoderFERET::~TModuleDecoderFERET() {
   if (fHitData) delete fHitData;
   fHitData = NULL;
}

Int_t TModuleDecoderFERET::Decode(char* buffer, const Int_t &size, TObjArray *seg){
   short *evtData = reinterpret_cast<short*>(buffer);
	 //FERETは2byte単位のイベントなので、short単位で読み込む
   UInt_t  evtSize = size;//sizeに書かれているのは単にイベントの数
   Bool_t  evtFlag = kFALSE;
   Int_t   headerID, geoID, channel, idx,mid;

   V7XXRaw_t *data;

//printf("evtsize=%d\n",evtSize);
   // clear old hits
//   fHitData->Clear();
   
   for (Int_t i=0; i != evtSize; ++i) {
//	 printf("%X\n",evtData[i]);
		short temp =((evtData[i]>>8)&0x00ff|(evtData[i]<<8)&0xff00);
		evtData[i]=temp;
//	 printf("%X\n",evtData[i]);
//	 }
//	 getchar();
      headerID = (evtData[i] & kHeaderMask);
      switch (headerID) {
	 case kHeader:
	    //geoID   = ((evtData[i] & kMaskGeometry) >> kShiftGeometry)&0x000f;//GEOIDの代わりにVSNを入れよう
//	    geoID   = ((evtData[i] & kMaskGeometry) >> kShiftGeometry)&0x000f;//GEOIDの代わりにVSNを入れよう, 0x00n0のnの桁にはTDCかADCかの区別が入っているが、decoderに入る前に区別してしまっているので、ここではこの情報を読まないことにする。
	    mid = ((evtData[i] & kMaskGeometry) >> kShiftGeometry)&0x000f;//GEOIDの代わりにVSNを入れよう, 0x00n0のnの桁にはTDCかADCかの区別が入っているが、decoderに入る前に区別してしまっているので、ここではこの情報を読まないことにする。
	    geoID = ((evtData[i] & 0x0070) >> 4);//GEOIDの代わりにVSNを入れよう, 0x00n0のnの桁にはTDCかADCかの区別が入っているが、decoderに入る前に区別してしまっているので、ここではこの情報を読まないことにする。
//		 geoID = geoID&0x000f;
	    evtFlag = kTRUE;
	    break;
	 case kMeasure:
	    if(evtFlag) {
	       channel = (evtData[i] & kMaskChannel) >> kShiftChannel;
//	       idx     = (geoID-1) * 16 + channel;
	       idx     = (mid-1) * 16 + channel;
//	       idx     =  channel;
	       // check if the data object exists
	     //  if (fHitData->GetEntriesFast() <= idx || !fHitData->At(idx)) {
		  // if no data available, create one
		  V7XXRaw_t *obj = static_cast<V7XXRaw_t*>(this->New());
//		  V7XXRaw_t *obj = new V7XXRaw_t();
		  obj->SetSegInfo(seg->GetUniqueID(),geoID,idx);
//		  obj->SetSegInfo(seg->GetUniqueID(),geoID,channel);
//		  fHitData->AddAtAndExpand(obj,idx);
	      // }
		  obj->Set( evtData[i] & kMaskMeasure );  
		  obj->SetCatID(-100000000);
		  obj->SetReference(0);
//	       data = static_cast<V7XXRaw_t*>(fHitData->At(idx));
//	                  printf("[V7XX] evtData[%d] = %d\n",i, evtData[i]&0x1fff);
//	       data->Set( evtData[i] & kMaskMeasure );
//	       fHitData->AddAt(NULL,idx);
//				 printf("FERET geo = %d,mid=%d, ch=%d, data = %d\n",geoID,mid,idx,evtData[i] & kMaskMeasure);
		  seg->Add(obj);
	    }
	    break;
	    
	 case kEOB:
	    evtFlag = kFALSE;
	    break;
      }
   }

   return 0;
}
