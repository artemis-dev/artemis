/**
 * @file   TModuleDecoderMXDC32_rcnp.cc
 * @brief  Decoder Class for MADC32, MQDC32, MTDC32
 *
 * @date   Created       : 2015-10-08 14:30:23 JST
 *         Last Modified : 2015-10-16 18:10:22 JST (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2015 KAWASE Shoichiro
 */

#include "TModuleDecoderMXDC32_rcnp.h"

#include <TObjArray.h>

#include <TRawTiming.h>

using art::TModuleDecoderMXDC32_rcnp;
using art::TRawTiming;

typedef TRawTiming MXDC32_rcnpRaw_t;

ClassImp(TModuleDecoderMXDC32_rcnp)

TModuleDecoderMXDC32_rcnp::TModuleDecoderMXDC32_rcnp()
   : TModuleDecoder(kID, MXDC32_rcnpRaw_t::Class())
{
   fHitData = new TObjArray;
}

TModuleDecoderMXDC32_rcnp::TModuleDecoderMXDC32_rcnp(Int_t id)
   : TModuleDecoder(id, MXDC32_rcnpRaw_t::Class())
{
   fHitData = new TObjArray;
}

TModuleDecoderMXDC32_rcnp::~TModuleDecoderMXDC32_rcnp()
{
   delete fHitData;
   fHitData = NULL;
}

Int_t TModuleDecoderMXDC32_rcnp::Decode(char* buffer, const Int_t &size, TObjArray *seg)
{
   const UInt_t *const evtData = reinterpret_cast<UInt_t*>(buffer);
   const UInt_t        evtSize = size/sizeof(UInt_t);
   Bool_t              evtFlag = kFALSE;
   Int_t igeo;

   // clear old hits
   fHitData->Clear();

   for (UInt_t i = 0; i != evtSize; ++i) {

     UInt_t evtDataTmp = ((evtData[i]>>8)&0x000000ff|(evtData[i]<<8)&0x0000ff00|(evtData[i]>>8)&0x00ff0000|(evtData[i]<<8)&0xff000000);

     //const UInt_t headerID = (evtData[i] & kMaskHeader) >> kShiftHeader;
     const UInt_t headerID = (evtDataTmp & kMaskHeader) >> kShiftHeader;

      switch(headerID) {
	 case kHeader:
	   //igeo = (evtData[i] & kMaskGeometry) >> kShiftGeometry;
	    igeo = (evtDataTmp & kMaskGeometry) >> kShiftGeometry;
	    evtFlag = kTRUE;
	    break;
	 case kMeasure:
	    if (evtFlag) {
	      //const UInt_t channel = (evtData[i] & kMaskChannel) >> kShiftChannel;
	       const UInt_t channel = (evtDataTmp & kMaskChannel) >> kShiftChannel;
	       const UInt_t index   = igeo * kChannel + channel;

	       // check if the data object exists
	       if (fHitData->GetEntriesFast() <= (Int_t)index || !fHitData->At(index)) {
		  // if no data available, create one
		  MXDC32_rcnpRaw_t *const obj = static_cast<MXDC32_rcnpRaw_t*>(this->New());
		  obj->SetSegInfo(seg->GetUniqueID(),igeo,channel);
		  fHitData->AddAtAndExpand(obj,index);
		  seg->Add(obj);
	       }

	       MXDC32_rcnpRaw_t *const data = static_cast<MXDC32_rcnpRaw_t*>(fHitData->At(index));
	       //const UInt_t measure = (evtData[i] & kMaskMeasure);
	       const UInt_t measure = (evtDataTmp & kMaskMeasure);
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
