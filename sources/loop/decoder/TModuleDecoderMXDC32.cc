/**
 * @file   TModuleDecoderMXDC32.cc
 * @brief  Decoder Class for MADC32, MQDC32, MTDC32
 *
 * @date   Created       : 2015-10-08 14:30:23 JST
 *         Last Modified : 2015-10-16 18:10:22 JST (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2015 KAWASE Shoichiro
 */

#include "TModuleDecoderMXDC32.h"

#include <TObjArray.h>

#include <TRawTiming.h>

using art::TModuleDecoderMXDC32;
using art::TRawTiming;

typedef TRawTiming MXDC32Raw_t;

ClassImp(TModuleDecoderMXDC32)

TModuleDecoderMXDC32::TModuleDecoderMXDC32()
   : TModuleDecoder(kID, MXDC32Raw_t::Class())
{
   fHitData = new TObjArray;
}

TModuleDecoderMXDC32::TModuleDecoderMXDC32(Int_t id)
   : TModuleDecoder(id, MXDC32Raw_t::Class())
{
   fHitData = new TObjArray;
}

TModuleDecoderMXDC32::~TModuleDecoderMXDC32()
{
   delete fHitData;
   fHitData = NULL;
}

Int_t TModuleDecoderMXDC32::Decode(char* buffer, const Int_t &size, TObjArray *seg)
{
   const UInt_t *const evtData = reinterpret_cast<UInt_t*>(buffer);
   const UInt_t        evtSize = size/sizeof(UInt_t);
   Bool_t              evtFlag = kFALSE;
   Int_t igeo;

   // clear old hits
   fHitData->Clear();

   for (UInt_t i = 0; i != evtSize; ++i) {
      const UInt_t headerID = (evtData[i] & kMaskHeader) >> kShiftHeader;

      switch(headerID) {
	 case kHeader:
	    igeo = (evtData[i] & kMaskGeometry) >> kShiftGeometry;
	    evtFlag = kTRUE;
	    break;
	 case kMeasure:
	    if (evtFlag) {
	       const UInt_t channel = (evtData[i] & kMaskChannel) >> kShiftChannel;
	       const UInt_t index   = igeo * kChannel + channel;

	       // check if the data object exists
	       if (fHitData->GetEntriesFast() <= (Int_t)index || !fHitData->At(index)) {
		  // if no data available, create one
		  MXDC32Raw_t *const obj = static_cast<MXDC32Raw_t*>(this->New());
		  obj->SetSegInfo(seg->GetUniqueID(),igeo,channel);
		  fHitData->AddAtAndExpand(obj,index);
		  seg->Add(obj);
	       }

	       MXDC32Raw_t *const data = static_cast<MXDC32Raw_t*>(fHitData->At(index));
	       const UInt_t measure = (evtData[i] & kMaskMeasure);
	       data->Set( measure );
	       fHitData->AddAt(NULL,index);
	    }
	    break;
	 case kEOB:
	    evtFlag = kFALSE;
	    break;
      }
   }

   return 0;
}
