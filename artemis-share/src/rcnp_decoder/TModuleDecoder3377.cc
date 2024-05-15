/**
 * @file   TModuleDecoder3377.cc
 * @brief  Decorder Class for V7XX
 *
 * @date   Created:       2013-07-24 14:41:31
 *         Last Modified: 2024-03-23 19:11:10 JST
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved.
 */

#include "TModuleDecoder3377.h"

#include <TObjArray.h>

#include <TRawTiming.h>

using art::TModuleDecoder3377;
using art::TRawTiming;

typedef TRawTiming V7XXRaw_t;

TModuleDecoder3377::TModuleDecoder3377()
   : TModuleDecoder(kID, V7XXRaw_t::Class()){
//   fHitData = new TObjArray;
   fHitData = nullptr;
}

TModuleDecoder3377::~TModuleDecoder3377() {
   if (fHitData) delete fHitData;
   fHitData = NULL;
}

Int_t TModuleDecoder3377::Decode(char* buffer, const Int_t &size, TObjArray *seg){
   short *evtData = reinterpret_cast<short*>(buffer);
	 //3377は2byte単位のイベントなので、short単位で読み込む
   UInt_t  evtSize = size;//sizeに書かれているのは単にイベントの数
   Bool_t  evtFlag = kFALSE;
   Int_t   headerID, geoID, channel, idx;
	int type,id;

//   V7XXRaw_t *data;
	int ihit =0;
//printf("evtsize=%d\n",evtSize);
   // clear old hits
  // fHitData->Clear("C");
//   fHitData->Delete();
   
   for (Int_t i=0; i != evtSize; ++i) {
		short temp =((evtData[i]>>8)&0x00ff|(evtData[i]<<8)&0xff00);
		evtData[i]=temp;
	
      headerID = (evtData[i] & kHeaderMask)>>15;
      switch (headerID) {
	 case kHeader:
		type = (evtData[i]&kTypeMask)>>4 ;
		geoID=type;
		id = (evtData[i]&kIDMask)<<5;
	    evtFlag = kTRUE;
		 if(geoID>6||geoID==1||geoID==3||geoID==5)evtFlag=kFALSE;
	    break;
	 case kMeasure:
	 
	    if(evtFlag) {
	       channel = (evtData[i] & kMaskChannel) >> kShiftChannel;
	       idx     = id + channel;
//			 this->Size();
//				printf("entry = %d\n",seg->GetEntriesFast());
//				getchar();
//	       if (fHitData->GetEntriesFast() <= idx || !fHitData->At(idx)) {
//		  V7XXRaw_t *obj = static_cast<V7XXRaw_t*>(this->New());
			V7XXRaw_t *obj = new V7XXRaw_t();
//		  obj->SetSegInfo(seg->GetUniqueID(),geoID,channel);
		  obj->SetSegInfo(seg->GetUniqueID(),geoID,idx);
//		  fHitData->AddAtAndExpand(obj,idx);
//		  fHitData->AddAtAndExpand(obj,ihit);
//		  fHitData->Clear("C");
			
//	       }
//	       
//		  printf("geoid=%d,channel=%d,idx=%d, id =%d\n",geoID,channel,idx,id);
//	       data = static_cast<V7XXRaw_t*>(fHitData->At(idx));
//	       data = static_cast<V7XXRaw_t*>(fHitData->At(ihit));
			 short measure = evtData[i] & kMaskMeasure;
//				data->SetTiming(measure&0x03ff);
//				data->SetReference(0);
				obj->SetCatID(-100000000);
				obj->SetTiming(measure&0x03ff);
				obj->SetReference(0);
//	       fHitData->AddAt(NULL,ihit);//aaaakoituga
seg->Add(obj);
				 ihit++;
//				 delete obj;
	    }
	    break;
	    
//	 case kEOB:
//	    evtFlag = kFALSE;
//	    break;
      }
   }
//   fHitData->Delete();
 //  fHitData->Clear();
	//printf("ihit=%d\n",ihit);

   return 0;
}
