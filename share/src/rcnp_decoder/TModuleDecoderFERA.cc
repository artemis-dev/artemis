/**
 * @file   TModuleDecoderFERA.cc
 * @brief  Decorder Class for V7XX
 *
 * @date   Created:       2013-07-24 14:41:31
 *         Last Modified: 2013-10-28 13:00:01
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved.
 */

#include "TModuleDecoderFERA.h"

#include <TObjArray.h>

#include <TRawTiming.h>

using art::TModuleDecoderFERA;
using art::TRawTiming;

typedef TRawTiming V7XXRaw_t;

TModuleDecoderFERA::TModuleDecoderFERA()
   : TModuleDecoder(kID, V7XXRaw_t::Class()){
   fHitData = new TObjArray;
}

TModuleDecoderFERA::~TModuleDecoderFERA() {
   if (fHitData) delete fHitData;
   fHitData = NULL;
}

Int_t TModuleDecoderFERA::Decode(char* buffer, const Int_t &size, TObjArray *seg){
   short *evtData = reinterpret_cast<short*>(buffer);
	 //FERAは2byte単位のイベントなので、short単位で読み込む
   UInt_t  evtSize = size;//sizeに書かれているのは単にイベントの数
   Bool_t  evtFlag = kFALSE;
   Int_t   headerID, geoID, channel, idx,mid;

   V7XXRaw_t *data;
	int ihit =0;
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
//	    geoID   = ((evtData[i] & kMaskGeometry) >> kShiftGeometry)&0x000f;//GEOIDの代わりにVSNを入れよう
	    //geoID   = ((evtData[i] & kMaskGeometry) >> kShiftGeometry)&0x000f;//GEOIDの代わりにVSNを入れよう
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
//				 printf("fera ch = %d\n",channel);
///				 printf("fera geo = %d, ch = %d\n",geoID,channel);
	       
	       // check if the data object exists
//	       if (fHitData->GetEntriesFast() <= idx || !fHitData->At(idx)) {
		  // if no data available, create one
//		  V7XXRaw_t *obj = static_cast<V7XXRaw_t*>(this->New());
		  V7XXRaw_t *obj = new V7XXRaw_t();
	
//		  obj->SetSegInfo(seg->GetUniqueID(),geoID,channel);
		  obj->SetSegInfo(seg->GetUniqueID(),geoID,idx);
//		  fHitData->AddAtAndExpand(obj,idx);
//		  seg->Add(obj);
//	       }
	       
//	       data = static_cast<V7XXRaw_t*>(fHitData->At(idx));
//	       data = static_cast<V7XXRaw_t*>(fHitData->At(ihit));
	 //      data = static_cast<V7XXRaw_t*>(fHitData->At(ch));
//	       data = static_cast<V7XXRaw_t*>(fHitData->At(fHitData->GetEntriesFast()));
	       //           printf("[V7XX] evtData[%d] = %d\n",i, evtData[i]&0x1fff);
//	       data->Set( evtData[i] & kMaskMeasure );
	       //data->SetTiming( evtData[i] & kMaskMeasure );
	       obj->SetTiming( evtData[i] & kMaskMeasure );
		  seg->Add(obj);
	 ///      fHitData->AddAt(NULL,idx);
//				 printf("FERA ch=%d, geo = %d, data = %d\n",idx,geoID,evtData[i] & kMaskMeasure);
				 ihit++;
	    }
	    break;
	    
	 case kEOB:
	    evtFlag = kFALSE;
	    break;
      }
   }

   return 0;
}
