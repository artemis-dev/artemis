/**
 * @file   TTimingChargeMappingProcessor.cc
 * @brief
 *
 * @date   Created:       2013-08-12 12:45:58
 *         Last Modified: 2023-03-14 13:49:14 JST
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved.
 */

#include "TTimingChargeMappingProcessor.h"
#include "TTimingChargeData.h"
#include <TCategorizedData.h>
#include <TRawTimingWithEdge.h>
#include <TRawDataTimingCharge.h>

#include "constant.h"

using art::TTimingChargeMappingProcessor;

ClassImp(art::TTimingChargeMappingProcessor)

// Default constructor
TTimingChargeMappingProcessor::TTimingChargeMappingProcessor()
   : fPlastic(NULL) {
   RegisterInputCollection("InputCollection","rawdata object returned by TRIDFEventStore",
			   fInputColName,TString("catdata"),
			   &fCategorizedData, "art::TCategorizedData");
   RegisterOutputCollection("OutputCollection","output collection name",
			    fOutputColName,TString("plastic_raw"),
			    &fPlastic, "TClonesArray", "art::TTimingChargeData");
   RegisterProcessorParameter("CatID","Category ID of plastic",fCatID,21);
   RegisterProcessorParameter("ChargeType","0: V1190 width (default), 1: QDC (V792 etc.), 2: paired time and charge (hinp)",
			      fChargeType,0);
   RegisterProcessorParameter("ChargeTypeID","data type id for charge information (valid when ChargeType != (0 or 2) , default: 2)",
			      fChargeTypeID,2);
   RegisterProcessorParameter("TimingTypeID","data type id for timing information (default: 1)",
			      fTimingTypeID,1);
   RegisterProcessorParameter("TrailingComesFirst","0: Leading->Trailing (default), 1: Trailing->Leading (for QTC etc.)",
			      fTrailingComesFirst,kFALSE);
   RegisterProcessorParameter("Sparse","hit will be inserted at the index corresponding to its id if sparse is 0, otherwize hit will be added ordinaly (default: 1)",
                              fIsSparse,kTRUE);
   Register(fDoRequireBoth("DoRequireBoth","0: require timing or charge, 1: require both (default: 1)",1));
}

TTimingChargeMappingProcessor::~TTimingChargeMappingProcessor()
{
   delete fPlastic;
}

void TTimingChargeMappingProcessor::Init(TEventCollection*)
{
   const TString &chargeTypeStr =
      TString::Format("ChargeType: %d",fChargeType) +
      (fChargeType ? TString::Format(", ChargeTypeID: %d",fChargeTypeID) : "");

   Info("Init","CatID: %d, TimingTypeID: %d => %s",
	fCatID,fTimingTypeID,fOutputColName.Data());
   Info("Init","%s",chargeTypeStr.Data());
   Info("Init","TrailingComesFirst: %d",fTrailingComesFirst);

   if (!fIsSparse) Info("Init","Not sparse.");
}

void TTimingChargeMappingProcessor::Process()
{
   fPlastic->Clear("C");

   const TObjArray *const cat = (*fCategorizedData)->FindCategory(fCatID);
   if (!cat) return;

   for(Int_t i = 0, n = cat->GetEntriesFast(); i != n ; ++i){
      const TObjArray *const det = static_cast<TObjArray*>(cat->At(i));
      const TObjArray *const tArray = static_cast<TObjArray*>(det->At(fTimingTypeID));
      if (fChargeType == kWIDTH) {
	 MapEdgedTime(tArray);
      } else if (fChargeType == kHINP) {
         MapPairedTimeCharge(tArray);
      } else /* if (fChargeType == kQDC) */ {
	 const TObjArray *const qArray = static_cast<TObjArray*>(det->At(fChargeTypeID));
	 MapTimeCharge(tArray,qArray);
      }
   }

   if (fIsSparse) {
      // sort data in the same event in ascending order of timing
      TTimingChargeData::SetSortType(TTimingChargeData::kTiming);
      TTimingChargeData::SetSortOrder(TTimingChargeData::kASC);
      fPlastic->Sort();
      fPlastic->Compress();
   } else {
      for (Int_t i = 0, n = fPlastic->GetEntriesFast(); i != n; ++i) {
         fPlastic->ConstructedAt(i);
      }
   }
}

void TTimingChargeMappingProcessor::MapEdgedTime(const TObjArray *tArray)
{
   if (!tArray || tArray->IsEmpty()) return;

   TTimingChargeData *data = NULL;
   for(Int_t iHit = 0, nHit = tArray->GetEntriesFast(); iHit != nHit; ++iHit) {
      const TRawTimingWithEdge *const hit
	 = static_cast<TRawTimingWithEdge*>(tArray->At(iHit));
//      if (!hit) continue;

      if(hit->IsLeading() != fTrailingComesFirst) {
	 // "Leading" edge
	 const Int_t detID = hit->GetDetID();
	 const Int_t idx = fIsSparse ? fPlastic->GetEntriesFast() : detID;
	 data = static_cast<TTimingChargeData*>(fPlastic->ConstructedAt(idx));
	 if(IsValid(data->GetDetID())) continue;

	 data->SetDetID(detID);
	 const Double_t tLeading  = hit->GetTiming();
	 data->SetTiming(tLeading);
      } else if (data) {
	 // "Trailing" edge after "Leading" one
	 const Double_t tLeading  = data->GetTiming();
	 const Double_t tTrailing = hit->GetTiming();
	 const Double_t charge = tTrailing - tLeading;

	 data->SetCharge(charge);
	 data = NULL;
      } else {
	 // consecutive "Trailing" edge
	 // thought to be abnormal
      }
   }
}

void TTimingChargeMappingProcessor::MapTimeCharge(const TObjArray *tArray, const TObjArray *qArray)
{
   

   
   TTimingChargeData *data = nullptr;

   /* Timing */
   if (tArray && !tArray->IsEmpty()) {
      const TRawDataObject *const tHit = static_cast<TRawDataObject*>(tArray->At(0));
      const Int_t detID = tHit->GetDetID();
      const Int_t idx = fIsSparse ? fPlastic->GetEntriesFast() : detID;
      data = static_cast<TTimingChargeData*>(fPlastic->ConstructedAt(idx));
      if(IsValid(data->GetDetID())) return; // take only the first hit
      data->SetDetID(detID);
      data->SetTiming(tHit->GetValue());
   }

   if (qArray && !qArray->IsEmpty()) {
      const TRawTiming *const qHit = static_cast<TRawTiming*>(qArray->At(0));
      if (fDoRequireBoth.Value() == 0  && data == NULL) {
         const Int_t detID = qHit->GetDetID();
         const Int_t idx = fIsSparse ? fPlastic->GetEntriesFast() : detID;
         data = static_cast<TTimingChargeData*>(fPlastic->ConstructedAt(idx));
         data->SetDetID(qHit->GetDetID());
         data->SetTiming(TMath::QuietNaN());
      }
      if (data != NULL) {
         data->SetCharge(qHit->GetValue());
      }
   }
}


void TTimingChargeMappingProcessor::MapPairedTimeCharge(const TObjArray *tArray)
{
   if (!tArray || tArray->IsEmpty()) return;

   TTimingChargeData *data = NULL;
   for(Int_t iHit = 0, nHit = tArray->GetEntriesFast(); iHit != nHit; ++iHit) {
      const TRawDataTimingCharge *const hit
	 = static_cast<TRawDataTimingCharge*>(tArray->At(iHit));

      const Int_t detID = hit->GetDetID();
      const Int_t idx = fIsSparse ? fPlastic->GetEntriesFast() : detID;
      data = static_cast<TTimingChargeData*>(fPlastic->ConstructedAt(idx));
      if(IsValid(data->GetDetID())) continue;

      data->SetDetID(detID);
      const Double_t timing = hit->GetTiming();
      const Double_t charge = hit->GetCharge();
      data->SetTiming(timing);
      data->SetCharge(charge);
   }
}

