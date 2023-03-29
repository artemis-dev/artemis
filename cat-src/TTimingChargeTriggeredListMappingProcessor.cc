/**
 * @file   TTimingChargeTriggeredListMappingProcessor.cc
 * @brief
 *
 * @date   Created:       2013-08-12 12:45:58
 *         Last Modified: 2016-08-17 15:05:41 JST (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved.
 */

#include "TTimingChargeTriggeredListMappingProcessor.h"
#include "TTimingChargeData.h"
#include <TCategorizedData.h>
#include <TRawDataTriggeredList.h>

#include "constant.h"

using art::TTimingChargeTriggeredListMappingProcessor;

ClassImp(art::TTimingChargeTriggeredListMappingProcessor)

// Default constructor
TTimingChargeTriggeredListMappingProcessor::TTimingChargeTriggeredListMappingProcessor()
   : fPlastic(NULL) {
   RegisterInputCollection("InputCollection","rawdata object returned by TRIDFEventStore",
			   fInputColName,TString("catdata"),
			   &fCategorizedData, "art::TCategorizedData");
   RegisterOutputCollection("OutputCollection","output collection name",
			    fOutputColName,TString("plastic_raw"),
			    &fPlastic, "TClonesArray", "art::TTimingChargeData");
   RegisterProcessorParameter("CatID","Category ID of plastic",fCatID,21);
   RegisterProcessorParameter("ChargeType","hoge",
			      fChargeType,0);
   RegisterProcessorParameter("ChargeTypeID","data type id for charge information (valid when ChargeType != (0 or 2) , default: 2)",
			      fChargeTypeID,2);
   RegisterProcessorParameter("TimingTypeID","data type id for timing information (default: 1)",
			      fTimingTypeID,1);
   RegisterProcessorParameter("TrailingComesFirst","0: Leading->Trailing (default), 1: Trailing->Leading (for QTC etc.)",
			      fTrailingComesFirst,kFALSE);
   RegisterProcessorParameter("Sparse","hit will be inserted at the index corresponding to its id if sparse is 0, otherwize hit will be added ordinaly (default: 1)",
                              fIsSparse,kTRUE);
}

TTimingChargeTriggeredListMappingProcessor::~TTimingChargeTriggeredListMappingProcessor()
{
   delete fPlastic;
}

void TTimingChargeTriggeredListMappingProcessor::Init(TEventCollection*)
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

void TTimingChargeTriggeredListMappingProcessor::Process()
{
   fPlastic->Clear("C");

   const TObjArray *const cat = (*fCategorizedData)->FindCategory(fCatID);
   if (!cat) return;

   for(Int_t i = 0, n = cat->GetEntriesFast(); i != n ; ++i){
      const TObjArray *const det = static_cast<TObjArray*>(cat->At(i));
      const TObjArray *const qArray = static_cast<TObjArray*>(det->At(fTimingTypeID));
      if (!qArray || qArray->IsEmpty()) continue;
      ChargeWithTS(qArray);
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

void TTimingChargeTriggeredListMappingProcessor::ChargeWithTS(const TObjArray *qArray)
{
   if (!qArray || qArray->IsEmpty()) return;

   TTimingChargeData *data = NULL;
   for(Int_t iHit = 0, nHit = qArray->GetEntriesFast(); iHit != nHit; ++iHit) {
      const TRawDataTriggeredList *const hit
	 = static_cast<TRawDataTriggeredList*>(qArray->At(iHit));
      const Int_t detID = hit->GetDetID();
      const Int_t idx = fIsSparse ? fPlastic->GetEntriesFast() : detID;
      data = static_cast<TTimingChargeData*>(fPlastic->ConstructedAt(idx));
      //if(IsValid(data->GetDetID())) continue;
      //printf("detID %d \n", detID);

      data->SetDetID(detID);
      const Double_t tsi = hit->GetTSI();
      const Double_t adc = hit->GetADC();
      data->SetTiming(tsi);
      data->SetCharge(adc);
      data = NULL;
   }
}

