/**
 * @file   TMWDCPlaneProcessor.cc
 * @brief
 *
 * @date   Created:       2013-10-18 13:01:05
 *         Last Modified: 2014-06-29 06:47:17 JST (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved.
 */

#include <algorithm>

#include "TMWDCPlaneProcessor.h"
#include "TMWDCParameter.h"
#include "TMWDCPlaneInfo.h"
#include "TTimingChargeData.h"
#include "TMWDCHitData.h"
#include "TConverterBase.h"
#include "TConverterUtil.h"

#include "constant.h"

#include <TClass.h>
#include <TClonesArray.h>

using art::TMWDCPlaneProcessor;

ClassImp(art::TMWDCPlaneProcessor);

// Default constructor
TMWDCPlaneProcessor::TMWDCPlaneProcessor()
   : fPlaneOut(NULL), fNPlane(kInvalidI), fNData(NULL), fConverterArray(NULL)
{
   RegisterInputCollection("InputCollection","input",
			   fInputColName,TString("mwdc1_raw"),
			   &fMWDCIn,"TClonesArray","art::TTimingChargeData");
   RegisterProcessorParameter("MWDCName","name of mwdc",
			      fMWDCName,TString("mwdc1"));
   const StringVec_t defConverterList(1,"dtdx");
   RegisterProcessorParameter("ConverterList","dt2dx conversion parameter",
			      fConverterList,defConverterList);
   RegisterProcessorParameter("SortType","0: ID, 1: Timing, 2: Charge (Default), 3: DriftLength",
			      fSortTypeInput, (Int_t)kCharge);
   RegisterProcessorParameter("SortOrder","0: ascending, 1: descending (Default)",
			      fSortOrderInput, (Int_t)kDESC);
}

TMWDCPlaneProcessor::~TMWDCPlaneProcessor()
{
   delete fConverterArray;
   if(fPlaneOut) {
      for (Int_t i = 0; i != fNPlane ; ++i) delete fPlaneOut[i];
      delete [] fPlaneOut;
   }
   delete [] fNData;
}

void TMWDCPlaneProcessor::Init(TEventCollection *col)
{
   LoadParameter(col);            // fNWireID, fNPlane are set
   if (!IsValid(fNPlane)) return; // fNPlane is not valid if the loading failed

   if (fNPlane == (Int_t)fConverterList.size()) {
      TConverterUtil::SetConverterArrayFromList(&fConverterArray,fConverterList,col);
   } else {
      Warning("Init","size of ConverterList is different from number of plane. ConverterList setting will be ignored.");
   }

   fPlaneOut = new TClonesArray*[fNPlane];

   fPlaneID.clear();
   fWireID.clear();
   fMaxDetID = DETID_ORIGIN - 1;

   for(Int_t iPlane=0; iPlane != fNPlane ; ++iPlane) {
      const TString &outputName = fOutputColName.at(iPlane);

      fPlaneOut[iPlane] = new TClonesArray(TMWDCHitData::Class());
      fPlaneOut[iPlane]->SetName(outputName);
      col->Add(outputName,fPlaneOut[iPlane],fOutputIsTransparent);

      const Int_t n = fNWireID[iPlane];
      fPlaneID.insert(fPlaneID.end(),n,iPlane);
      for(Int_t i = 0; i != n; ++i) {
	 fWireID.push_back(i );
      }

      const TString &converterStr =
	 fConverterArray ? TString::Format("(converter: %s)",fConverterList.at(iPlane).Data()) : "";
      Info("Init","%s (id %d:%d) => %s %s",
	   fInputColName.Data(),
	   fMaxDetID + 1, fMaxDetID + n,
	   outputName.Data(), converterStr.Data());

      fMaxDetID += n;
   }

   fNData = new Int_t[fNPlane];

   InitSortParameter();
}

void TMWDCPlaneProcessor::InitSortParameter()
{
   switch (fSortTypeInput) {
      case kID:
	 fSortType = TMWDCHitData::kID;
	 break;
      case kTiming:
	 fSortType = TMWDCHitData::kTiming;
	 break;
      case kCharge:
	 fSortType = TMWDCHitData::kCharge;
	 break;
      case kDriftLength:
	 fSortType = TMWDCHitData::kDriftLength;
	 break;
      default:
	 SetStateError(TString::Format("Invalid SortType option: %d",fSortTypeInput));
   }

   switch (fSortOrderInput) {
      case kASC:
	 fSortOrder = TMWDCHitData::kASC;
	 break;
      case kDESC:
	 fSortOrder = TMWDCHitData::kDESC;
	 break;
      default:
	 SetStateError(TString::Format("Invalid SortOrder option: %d",fSortOrderInput));
   }
}

Bool_t TMWDCPlaneProcessor::LoadParameter(TEventCollection *col)
{
   const TMWDCParameter *const mwdcprm =
      reinterpret_cast<TMWDCParameter*>(col->GetInfo(fMWDCName));
   if (!mwdcprm) {
      SetStateError(TString::Format("no parameter object for '%s' found.",fMWDCName.Data()));
      return kFALSE;
   }

   const TList *const planeList = mwdcprm->GetPlaneList();

   TIter next(planeList);
   while (const TMWDCPlaneInfo *const prm = reinterpret_cast<TMWDCPlaneInfo*>(next())) {
      const TString &outputName = TString::Format("%s_%s",fMWDCName.Data(),prm->GetName());
      fOutputColName.push_back(outputName);
      fNWireID.push_back(prm->GetNWire());
      fCellSize.push_back(prm->GetCellSize() * 0.5);
   }

   fNPlane = fOutputColName.size();
   return kTRUE;
}

void TMWDCPlaneProcessor::Process()
{
   /* initialization */
   for(Int_t iPlane=0; iPlane != fNPlane ; ++iPlane) {
      fPlaneOut[iPlane]->Clear("C");
   }

   std::fill(fNData,fNData+fNPlane,0.);

   /* get input */
   for(Int_t iHit=0, nHit = (*fMWDCIn)->GetEntriesFast(); iHit!=nHit; ++iHit) {
      const TTimingChargeData *const inData =
	 static_cast<TTimingChargeData*>((*fMWDCIn)->At(iHit));

      const Int_t detID = inData->GetDetID();
      if (detID < DETID_ORIGIN || detID > fMaxDetID) continue;

      const Int_t planeID =  fPlaneID[detID - DETID_ORIGIN];

      /* output */
      TMWDCHitData *const outData =
	 static_cast<TMWDCHitData*>(fPlaneOut[planeID]->ConstructedAt(fNData[planeID]++));

      {
	 const Int_t wireID = fWireID[detID - DETID_ORIGIN];
	 outData->SetDetID(wireID);
      }

      {
	 const Double_t timing = inData->GetTiming();
	 outData->SetTiming(timing);
	 outData->SetCharge(inData->GetCharge());
         outData->SetTimestamp(inData->GetTimestamp());
	 if (fConverterArray) {
	    const Double_t driftLength = fConverterArray->at(planeID)->Convert(timing);
	    outData->SetDriftLength( driftLength * fCellSize[planeID]);
         }
      }
   }

   TMWDCHitData::SetSortType(fSortType);
   TMWDCHitData::SetSortOrder(fSortOrder);
   for(Int_t iPlane=0; iPlane != fNPlane ; ++iPlane) {
      fPlaneOut[iPlane]->Sort();
      fPlaneOut[iPlane]->Compress();
      
   }
}
