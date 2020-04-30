/* $Id:$ */
/**
 * @file   TModuleDecoderSIS3820.cc
 * @date   Created : Oct 24, 2013 22:39:29 JST
 *   Last Modified : 2019-11-22 17:52:03 JST (ota)
 * @author Hiroshi TOKIEDA <tokieda@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#include <TModuleDecoderFactory.h>
#include <TObjArray.h>
#include <TRawDataSimple.h>

#include "TModuleDecoderSIS3820.h"


art::TModuleDecoderSIS3820::TModuleDecoderSIS3820(Int_t id)
  : TModuleDecoder(id,TRawDataSimple<UInt_t>::Class()) {
}

art::TModuleDecoderSIS3820::~TModuleDecoderSIS3820()
{
}

Int_t art::TModuleDecoderSIS3820::Decode(char* buffer, const int &size, TObjArray *seg)
{
   const Int_t kGeoID  = 0;
   Int_t evtSize = size / sizeof(UInt_t);
   //   printf("evtSize = %d\n",evtSize);
   UInt_t *evtData = (UInt_t*) buffer;
   UInt_t measure;

   // check if the data object exists
   for (Int_t iData = 0; iData != evtSize; ++iData){
     measure = evtData[iData] & fMaskMeasure;
     //     printf("measure[%d] = 0x%x\n",iData,measure);
     //     printf("measure[%d] = %d\n",iData,measure);
     
     // if no data available, create one
     TRawDataSimple<UInt_t>* data = static_cast<TRawDataSimple<UInt_t>*>(this->New());
     data->SetSegInfo(seg->GetUniqueID(), kGeoID, iData);
     seg->Add(data);
     data->Set(measure);
     //     printf("iData = %d, data = %d\n",iData,data->Get());
     if (!data) {
       printf("data([%d] = %d) is Null !",iData,data->Get());
     }
   }
   return 0;
}
