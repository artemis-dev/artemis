/**
 * @file   TModuleDecoderTS.cc
 * @brief  Decorder Class for V7XX
 *
 * @date   Created:       2013-07-24 14:41:31
 *         Last Modified: 2024-03-23 18:47:34 JST
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved.
 */

#include "TModuleDecoderTS.h"

#include <TObjArray.h>

#include <TRawTiming.h>

using art::TModuleDecoderTS;
using art::TRawTiming;

typedef TRawTiming V7XXRaw_t;

TModuleDecoderTS::TModuleDecoderTS()
   : TModuleDecoder(kID, V7XXRaw_t::Class()){
   fHitData = new TObjArray;
}

TModuleDecoderTS::~TModuleDecoderTS() {
   if (fHitData) delete fHitData;
   fHitData = NULL;
}

Int_t TModuleDecoderTS::Decode(char* buffer, const Int_t &size, TObjArray *seg){
   short *evtData = reinterpret_cast<short*>(buffer);
   short measure1 = evtData[0];
   short measure2 = evtData[1];
//time stampは8bitで abcdefghのとき、cdabghefの順にくっつければ正しい値になる
   int timestamp = ((((measure1<<8)&0xff00)|((measure1>>8)&0x00ff))<<16)|((((measure2<<8&0xff00))|((measure2>>8)&0x00ff)));
//			printf("timestamp = %08x\n",timestamp);
//			printf("%04x,%04x\n",measure1,measure2);
   V7XXRaw_t *obj = new V7XXRaw_t();
   obj->SetSegInfo(seg->GetUniqueID(),72,1);
   obj->SetCatID(-100000000);
//				obj->SetTiming(measure&0x03ff);
   obj->SetTiming(timestamp);
   obj->SetReference(0);
   seg->Add(obj);
   /*
   short *evtData = reinterpret_cast<short*>(buffer);
	 //TSは2byte単位のイベントなので、short単位で読み込む
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
	*/

   return 0;
}
