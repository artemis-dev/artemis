/**
 * @file   TTimingChargeCalibrationProcessor.cc
 * @brief
 *
 * @date   Created:       2013-11-06 20:36:15
 *         Last Modified: 2014-06-29 07:14:00 JST (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved.
 */

#include "TTimingChargeCalibrationProcessor.h"

#include "ITiming.h"
#include "ICharge.h"
#include "TConverterBase.h"
#include "TConverterUtil.h"
#include "constant.h"
#include "TDataObject.h"

#include <TClonesArray.h>
#include <TRandom.h>
#include <TClass.h>

using art::TTimingChargeCalibrationProcessor;

namespace {
   const TString DEFAULT_CONV_NAME("no_conversion");
}

ClassImp(art::TTimingChargeCalibrationProcessor)

// Default constructor
TTimingChargeCalibrationProcessor::TTimingChargeCalibrationProcessor()
   : fInData(NULL), fOutData(NULL),
   fTimingConverterArray(NULL), fChargeConverterArray(NULL),
   fInputHasTiming(kFALSE), fInputHasCharge(kFALSE)
{
   RegisterInputCollection("InputCollection",
			   "array of objects inheriting from art::ITiming and/or art::ICharge",
			   fInputColName,TString("plastic_raw"));
   RegisterOutputCollection("OutputCollection","output class will be the same as input",
			    fOutputColName,TString("plastic"));
   RegisterProcessorParameter("TimingConverterArray",
			      "normally output of TAffineConverterArrayGenerator",
			      fTimingConverterArrayName,DEFAULT_CONV_NAME);
   RegisterProcessorParameter("ChargeConverterArray",
			      "normally output of TAffineConverterArrayGenerator",
			      fChargeConverterArrayName,DEFAULT_CONV_NAME);
   RegisterProcessorParameter("InputIsDigital","whether input is digital or not",
			      fInputIsDigital,kTRUE);
}

TTimingChargeCalibrationProcessor::~TTimingChargeCalibrationProcessor()
{
   delete fOutData;
   delete fTimingConverterArray;
   delete fChargeConverterArray;
}

void TTimingChargeCalibrationProcessor::Init(TEventCollection *col)
{
   Info("Init","%s => %s",fInputColName.Data(),fOutputColName.Data());
   fInData = reinterpret_cast<TClonesArray**>(col->GetObjectRef(fInputColName.Data()));
   if (!fInData) {
      SetStateError(TString::Format("input not found: %s",fInputColName.Data()));
      return;
   }

   const TClass *const cl = (*fInData)->GetClass();
   fInputHasTiming = cl->InheritsFrom(art::ITiming::Class());
   fInputHasCharge = cl->InheritsFrom(art::ICharge::Class());

   if(!fInputHasTiming && !fInputHasCharge){
      SetStateError("contents of input array must inherit from art::ITiming and/or art::ICharge");
      return;
   }

   if(fInputHasTiming && fTimingConverterArrayName.CompareTo(DEFAULT_CONV_NAME)) {
      TConverterUtil::SetConverterArray(&fTimingConverterArray,fTimingConverterArrayName,col);
   }

   if(fInputHasCharge && fChargeConverterArrayName.CompareTo(DEFAULT_CONV_NAME)) {
      TConverterUtil::SetConverterArray(&fChargeConverterArray,fChargeConverterArrayName,col);
   }

   if(!fTimingConverterArray && !fChargeConverterArray){
      SetStateError("no converter registered.");
      return;
   }

   fOutData = new TClonesArray(cl);
   fOutData->SetName(fOutputColName);
   col->Add(fOutputColName,fOutData,fOutputIsTransparent);
}

void TTimingChargeCalibrationProcessor::Process()
{
   fOutData->Clear("C");

   for(Int_t iData = 0, nData = (*fInData)->GetEntriesFast();
       iData != nData; ++iData) {

      const TDataObject *const inData = static_cast<TDataObject*>((*fInData)->At(iData));
      TObject *const outData = fOutData->ConstructedAt(iData);
      const Int_t detID = inData->GetID();
      if(!IsValid(detID)) continue;
      inData->Copy(*outData);

      if (fInputHasTiming) {
	 const ITiming *const inDataT = dynamic_cast<const ITiming*>(inData);
	 const Double_t timing = inDataT->GetTiming() + (fInputIsDigital ? gRandom->Uniform() : 0);
	 if ( fTimingConverterArray && detID >= (Int_t)fTimingConverterArray->size()) {
	    SetStateError(TString::Format("size of TimingConverterArray (%d) is smaller than detID (%d)",(Int_t)fTimingConverterArray->size(), detID));
	    return;
	 }
	 const Double_t outputT =
	    fTimingConverterArray ? fTimingConverterArray->at(detID)->Convert(timing) : timing;
	 ITiming *const outDataT = dynamic_cast<ITiming*>(outData);
	 outDataT->SetTiming( outputT );
      }

      if (fInputHasCharge) {
	 const ICharge *const inDataQ = dynamic_cast<const ICharge*>(inData);
	 const Double_t charge = inDataQ->GetCharge() + (fInputIsDigital ? gRandom->Uniform() : 0);
	 if ( fChargeConverterArray && detID >= (Int_t)fChargeConverterArray->size()) {
	    SetStateError(TString::Format("size of ChargeConverterArray (%d) is smaller than detID (%d)",(Int_t)fChargeConverterArray->size(), detID));
	    return;
	 }
	 const Double_t outputQ =
	    fChargeConverterArray ? fChargeConverterArray->at(detID)->Convert(charge) : charge;
	 ICharge *const outDataQ = dynamic_cast<ICharge*>(outData);
	 outDataQ->SetCharge( outputQ );
      }
   }
}
