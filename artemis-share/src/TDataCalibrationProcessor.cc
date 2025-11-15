/**
 * @file   TDataCalibrationProcessor.cc
 * @brief
 *
 * @date   Created:       2013-11-28 11:09:13
 *         Last Modified: 2014-06-18 11:40:36 JST (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved.
 */

#include "TDataCalibrationProcessor.h"

#include <TClonesArray.h>
#include <TRandom.h>
#include <TClass.h>
#include <TSystem.h>

#include "TDataObject.h"
#include "ITiming.h"
#include "ICharge.h"

#include "TConverterBase.h"
#include "TConverterUtil.h"

using art::TDataCalibrationProcessor;

namespace {
   const TString DEFAULT_CONV_NAME("no_conversion");
   typedef void (TDataCalibrationProcessor::*CalibFuncPtr)(art::TDataObject*) const;
}

ClassImp(art::TDataCalibrationProcessor)

inline void TDataCalibrationProcessor::SetCalibrationMethod()
{
   CalibrationMethod =
      fDataClassName.EqualTo("ITiming") ? (CalibFuncPtr)&TDataCalibrationProcessor::Calibrate<ITiming> :
      fDataClassName.EqualTo("ICharge") ? (CalibFuncPtr)&TDataCalibrationProcessor::Calibrate<ICharge> :
      NULL;
}

// Default constructor
TDataCalibrationProcessor::TDataCalibrationProcessor()
   : fInData(NULL), fOutData(NULL), fConverterArray(NULL), fOverwrite(kFALSE)
{
   const StringVec_t defInput(1,"data");
   RegisterInputCollection("InputCollection",
			   "collection of objects to be calibrated",
			   fInputColName,defInput);
   RegisterOutputCollection("OutputCollection","if blank, input will be overwritten",
			    fOutputColName,TString(""));
   RegisterProcessorParameter("DetectorIDOrigin","origin of detector ID",
			      fDetIDOrigin,0);
   RegisterProcessorParameter("ConverterArray",
			      "output of some converter array generator",
			      fConverterArrayName,DEFAULT_CONV_NAME);
   RegisterProcessorParameter("DataClass",
			      "choose from -> ITiming, ICharge",
			      fDataClassName,TString("ITiming"));
   RegisterProcessorParameter("InputIsDigital","whether input is digital(raw) or not",
			      fInputIsDigital,kTRUE);
}

TDataCalibrationProcessor::~TDataCalibrationProcessor()
{
   delete fOutData;
   delete fConverterArray;
}

void TDataCalibrationProcessor::Init(TEventCollection *col)
{
   fDataClass = TClass::GetClass(TString("art::") + fDataClassName);

   if (!fDataClass) {
      Error("Init","Invalid DataClass '%s'",fDataClassName.Data());
      return;
   }

   SetCalibrationMethod();

   if (!CalibrationMethod) {
      Error("Init","No implementation for DataClass '%s'",fDataClassName.Data());
      return;
   }

   Info("Init","%s => %s",fInputColName.front().Data(),fOutputColName.Data());
   fInData = reinterpret_cast<TClonesArray**>(col->GetObjectRef(fInputColName[0].Data()));
   if (!fInData) return;

   const TClass *const cl = (*fInData)->GetClass();

   if (!cl->InheritsFrom(fDataClass)) {
      Error("Init","Input doesn't inherits from '%s'",fDataClassName.Data());
      return;
   }

   if (fConverterArrayName.CompareTo(DEFAULT_CONV_NAME)) {
      TConverterUtil::SetConverterArray(&fConverterArray,fConverterArrayName,col);
   }

   if (!fConverterArray) {
      Error("Init","no converter registered.");
      return;
   }

   fConverterArraySize = fConverterArray->size();

   Info("Init","DataClass: %s, ConverterArray: %s",fDataClassName.Data(),fConverterArrayName.Data());

   if (!fOutputColName.IsNull()) {
      fOutData = new TClonesArray(cl);
      fOutData->SetName(fOutputColName);
      col->Add(fOutputColName,fOutData,fOutputIsTransparent);
   } else {
      fOverwrite = kTRUE;
   }
}

void TDataCalibrationProcessor::Process()
{
   if(!fOverwrite) {
      if(!fOutData) return;
      fOutData->Clear("C");
   } else if (!CalibrationMethod) {
      return;
   }

   for(Int_t iData = 0, nData = (*fInData)->GetEntriesFast();
       iData != nData; ++iData) {
      TDataObject *const inData = static_cast<TDataObject*>((*fInData)->At(iData));
      const Int_t detID = inData->GetID() - fDetIDOrigin;

      if (detID >= 0 && detID < fConverterArraySize) {
	 if (fOverwrite) {
	    (this->*CalibrationMethod)(inData);
	 } else {
	    TDataObject *const outData = static_cast<TDataObject*>(fOutData->ConstructedAt(iData));
	    inData->Copy(*outData);
	    (this->*CalibrationMethod)(outData);
	 }
      } else {
	 Error("Process","out of range (detID = %d, min = %d, max = %d)",
	       inData->GetID(), fDetIDOrigin, (Int_t)fConverterArray->size() + fDetIDOrigin - 1);
	 gSystem->Sleep(100);
      }
   }
}

template <typename T> void TDataCalibrationProcessor::Calibrate(TDataObject *data) const
{
   T *const dataT = dynamic_cast<T*>(data);
   const Int_t detID = data->GetID() - fDetIDOrigin;
   *dataT = ConvertVal(*dataT,detID);
}

inline Double_t TDataCalibrationProcessor::ConvertVal(Double_t val, Int_t detID) const
{
   if (fInputIsDigital) val += gRandom->Uniform();
   return fConverterArray->at(detID)->Convert(val);
}
