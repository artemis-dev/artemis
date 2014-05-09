/**
 * @file   TModuleDecoderFixed.cc
 * @brief
 *
 * @date   Created:       2013-07-30 16:34:07
 *         Last Modified: 2013-10-18 17:45:01
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved.
 */

#include "TModuleDecoderFixed.h"

#include <TRawDataSimple.h>

using art::TModuleDecoderFixed;
using art::TRawDataSimple;

template <typename T> TModuleDecoderFixed<T>::TModuleDecoderFixed()
   : TModuleDecoder(0, TRawDataSimple<T>::Class())
{
}

template <typename T> TModuleDecoderFixed<T>::TModuleDecoderFixed(const Int_t id, const Int_t size)
   : TModuleDecoder(id, TRawDataSimple<T>::Class()){
   fHitData = new TObjArray;
   CalcMaskMeasure(size);
}

template <typename T> TModuleDecoderFixed<T>::~TModuleDecoderFixed() {
   if (fHitData) delete fHitData;
   fHitData = NULL;
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
   T          *evtData = reinterpret_cast<T*>(buffer);
   T           measure;

   TRawDataSimple<T> *data;

   fHitData->Clear();

   // check if the data object exists
   for (Int_t iData = 0; iData != (Int_t)evtSize; ++iData){
      measure = evtData[iData] & fMaskMeasure;

      if (fHitData->GetEntriesFast() <= iData || !fHitData->At(iData)) {
	 // if no data available, create one
	 TRawDataSimple<T>* obj = static_cast<TRawDataSimple<T>*>(this->New());
	 obj->SetSegInfo(seg->GetUniqueID(), kGeoID, iData);
	 fHitData->AddAtAndExpand(obj,iData);
	 seg->Add(obj);
      }

      data = static_cast<TRawDataSimple<T>*>(fHitData->At(iData));
      data->Set(measure);
      fHitData->AddAt(NULL,iData);
   }

   return 0;
}

// explicit instantiations to limit T to be one of unsigned integer types.
namespace art{
template class TModuleDecoderFixed<UChar_t>;
template class TModuleDecoderFixed<UShort_t>;
template class TModuleDecoderFixed<UInt_t>;
template class TModuleDecoderFixed<ULong64_t>;
}
