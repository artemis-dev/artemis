/**
 * @file   TModuleDecoderV7XX.cc
 * @brief  Decorder Class for V7XX
 *
 * @date   Created:       2013-07-24 14:41:31
 *         Last Modified: 2013-08-01 11:15:17
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved.
 */

#include "TModuleDecoderV7XX.h"

#include <TObjArray.h>

#include <TRawDataSimple.h>

using art::TModuleDecoderV7XX;
using art::TRawDataSimple;

typedef TRawDataSimple<UInt_t> V7XXRaw_t;

TModuleDecoderV7XX::TModuleDecoderV7XX()
   : TModuleDecoder(kID, V7XXRaw_t::Class()){
   fHitData = new TObjArray;
}

TModuleDecoderV7XX::~TModuleDecoderV7XX() {
}

Int_t TModuleDecoderV7XX::Decode(char* buffer, const Int_t &size, TObjArray *seg){
   UInt_t *evtData = reinterpret_cast<UInt_t*>(buffer);
   UInt_t  evtSize = size/sizeof(UInt_t);
   Bool_t  evtFlag = kFALSE;
   Int_t   headerID, geoID, channel, idx;

   V7XXRaw_t *data;
   
   for (Int_t i=0; i != evtSize; ++i) {
      headerID = (evtData[i] & kHeaderMask);
      switch (headerID) {
	 case kHeader:
	    geoID   = (evtData[i] & kMaskGeometry) >> kShiftGeometry;
	    evtFlag = kTRUE;
	    break;
	    
	 case kMeasure:
	    if(evtFlag) {
	       channel = (evtData[i] & kMaskChannel) >> kShiftChannel;
	       idx     = geoID * 128 + channel;
	       
	       // check if the data object exists
	       if (fHitData->GetEntriesFast() <= idx || !fHitData->At(idx)) {
		  // if no data available, create one
		  V7XXRaw_t *obj = static_cast<V7XXRaw_t*>(this->New());
		  obj->SetSegInfo(seg->GetUniqueID(),geoID,channel);
		  fHitData->AddAtAndExpand(obj,idx);
		  seg->Add(obj);
	       }
	       
	       data = static_cast<V7XXRaw_t*>(fHitData->At(idx));
	       //           printf("[V7XX] evtData[%d] = %d\n",i, evtData[i]&0x1fff);
	       data->Set( evtData[i] & kMaskMeasure );
	    }
	    break;
	    
	 case kEOB:
	    evtFlag = kFALSE;
	    break;
      }
   }

   return 0;
}
