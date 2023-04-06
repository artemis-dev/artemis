/**
 * @file   TModuleDecoderV830.cc
 * @brief  Decorder Class for V830
 *
 * @date   Created:       2013-07-24 14:41:31
 *         Last Modified: 2013-10-28 13:00:01
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved.
 */

#include "TModuleDecoderV830.h"

#include <TObjArray.h>

#include <TRawTiming.h>    
using art::TModuleDecoderV830;
using art::TRawTiming;     

typedef TRawTiming V830Raw_t;

TModuleDecoderV830::TModuleDecoderV830()
   : TModuleDecoder(kID, V830Raw_t::Class()){
   fHitData = new TObjArray;
}

TModuleDecoderV830::~TModuleDecoderV830() {
   if (fHitData) delete fHitData;
   fHitData = NULL;
}

Int_t TModuleDecoderV830::Decode(char* buffer, const Int_t &size, TObjArray *seg){
   UInt_t *evtData = reinterpret_cast<UInt_t*>(buffer);
   UInt_t  evtSize = size/sizeof(UInt_t);
   Bool_t  evtFlag = kFALSE;
   Int_t   geoID=0, channel, idx;
   UInt_t   order[8];
   Int_t   tens = 1;
   //   Int_t   scaler = 0;
   UInt_t measure;
   
   V830Raw_t *data;

   // clear old hits
   fHitData->Clear();
   
   for (Int_t i=0; i != evtSize; ++i) {
     evtData[i]=(evtData[i]>>16|evtData[i]<<16);
     channel = i;
     idx = geoID*128 + i;
     for (int j=0; j<8; ++j){
       order[j] = (evtData[i]>>(j*4))&0xf;
       measure=order[j]*tens+measure;
       tens *= 10;
       //if(i==6) printf("%d ",order[i]);
       //if(i==6) printf("%d ",(evtData[i]>>(j*4))&0xf);
     }
     measure=order[0]+order[1]*10+order[2]*100+order[3]*1000+order[4]*10000+order[5]*100000+order[6]*1000000+order[7]*10000000;
     //if(i==6) printf("   %d    %d\n",evtData[i],measure);
     // check if the tata object exists
     if (fHitData->GetEntriesFast() <= idx || !fHitData->At(idx)) {
       // if no data available, create one
       V830Raw_t *obj = static_cast<V830Raw_t*>(this->New());
       obj->SetSegInfo(seg->GetUniqueID(),geoID,channel);
       fHitData->AddAtAndExpand(obj,idx);
       seg->Add(obj);
     }
     
     data = static_cast<V830Raw_t*>(fHitData->At(idx));
     //printf("[V830] evtData[%d] = %d,geo = %d ch =%d \n",i, evtData[i]&0x1fff,geoID,channel);
     data->Set( measure );
     fHitData->AddAt(NULL,idx);
     measure=0;
   }
   
   return 0;
}
