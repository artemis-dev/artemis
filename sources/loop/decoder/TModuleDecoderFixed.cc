/**
 * @file   TModuleDecoderFixed.cc
 * @brief
 *
 * @date   Created:       2013-07-30 16:34:07
 *         Last Modified: 2013-07-31 15:14:12
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved.
 */

#include "TModuleDecoderFixed.h"

#include <TRawDataSimple.h>

using art::TModuleDecoderFixed;
using art::TRawDataSimple;

// Default constructor
template <typename T> TModuleDecoderFixed<T>::TModuleDecoderFixed(const Int_t id, const Int_t size)
   : TModuleDecoder(id, TRawDataSimple::Class()){
   fHitData = new TObjArray;
   CalcMaskMeasure(size);
}

// Default destructor
template <typename T> TModuleDecoderFixed<T>::~TModuleDecoderFixed() {
}

template <typename T> void TModuleDecoderFixed<T>::CalcMaskMeasure(const Int_t size) {
   Int_t shiftDigit = 8 * sizeof(T) - size;

   fMaskMeasure = static_cast<T>(-1);
   if(shiftDigit > 0){
      fMaskMeasure >>= shiftDigit;
   }
}

template <typename T> Int_t TModuleDecoderFixed<T>::Decode(char* buffer, const int &size, TObjArray *seg) {
   const Int_t kGeoID  = 0;
   UInt_t      evtSize = size / sizeof(T);
   T          *evtData = (T*) buffer;
   T           measure;

   TRawDataSimple *data;

   Clear();
   fHitData->Clear();

   // check if the data object exists
   for (Int_t iData = 0; iData != evtSize; ++iData){
      measure = evtData[iData] & fMaskMeasure;

      if (fHitData->GetEntriesFast() <= iData || !fHitData->At(iData)) {
	 // if no data available, create one
	 TObject *obj = New();
	 ((TRawDataSimple*)obj)->SetSegInfo(seg->GetUniqueID(), kGeoID, iData);
	 fHitData->AddAtAndExpand(obj,iData);
	 seg->Add(obj);
      }

      data = (TRawDataSimple*)fHitData->At(iData);
      data->Set(measure);
      fHitData->AddAt(NULL,iData);
   }

   return 0;
}

// explicit instantiations to limit T to be one of unsigned integer types.
template class TModuleDecoderFixed<UChar_t>;
template class TModuleDecoderFixed<UShort_t>;
template class TModuleDecoderFixed<UInt_t>;
//template class TModuleDecoderFixed<ULong64_t>;
