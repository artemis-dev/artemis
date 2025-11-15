/**
 * @file   TSimpleDataMappingProcessor.cc
 * @brief  simple data mapper
 *
 * @date   Created       : 2014-05-19 00:26:01 JST
 *         Last Modified : 2018-01-23 01:48:44 JST (ota)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 KAWASE Shoichiro
 */

#include "TSimpleDataMappingProcessor.h"

#include <TRawDataSimple.h>
#include <TCategorizedData.h>
#include <TChargeData.h>

#include "constant.h"

using art::TSimpleDataMappingProcessor;

ClassImp(TSimpleDataMappingProcessor)

TSimpleDataMappingProcessor::TSimpleDataMappingProcessor()
  : fOutputArray(NULL) {
  RegisterInputCollection("InputCollection","rawdata object returned by TRIDFEventStore",
			  fInputColName,TString("catdata"),
			  reinterpret_cast<void***>(&fCategorizedData),
			  "art::TCategorizedData");
  RegisterOutputCollection("OutputCollection","output collection name",
			   fOutputColName,TString("simple"),
			   reinterpret_cast<void**>(&fOutputArray),
			   "TClonesArray","art::TChargeData");
  RegisterProcessorParameter("CatID","Category ID",fCatID,0);
  RegisterProcessorParameter("DataTypeID","data type id for input",
			     fDataTypeID,0);
  //  RegisterProcessorParameter("OutputType","not yet implemented (always output TChargeData)"
  //			     fOutputType,2);
  RegisterProcessorParameter("Sparse","hit will be inserted at the index corresponding to its id if sparse is 0, otherwise added ordinaly", fIsSparse,kTRUE);
}

TSimpleDataMappingProcessor::~TSimpleDataMappingProcessor()
{
  delete fOutputArray;
  fOutputArray = NULL;
}

void TSimpleDataMappingProcessor::Init(TEventCollection *col)
{
  Info("Init","CatID: %d, DataTypeID: %d => %s",
       fCatID, fDataTypeID, fOutputColName.Data());
  if (!fIsSparse) Info("Init","Not sparse.");
}

void TSimpleDataMappingProcessor::Process()
{
  if (!fCategorizedData) return;

  fOutputArray->Clear("C");

  const TObjArray *const cat = (*fCategorizedData)->FindCategory(fCatID);
  if (!cat) return;

  const Int_t n = cat->GetEntriesFast();
  for(Int_t i = 0 ; i != n ; ++i) {
    const TObjArray *const det = static_cast<TObjArray*>(cat->At(i));
    const TObjArray *const dataArray = static_cast<TObjArray*>(det->At(fDataTypeID));
    if (!dataArray) continue;

    const TRawDataSimple<UInt_t> *const hit = 
      dynamic_cast<TRawDataSimple<UInt_t>*>(dataArray->At(0));
    
//    const TRawDataSimple<UShort_t> *const hit = 
//      dynamic_cast<TRawDataSimple<UShort_t>*>(dataArray->At(0));
    if (!hit) continue;

    const Int_t detID = hit->GetDetID();
    const Int_t idx = fIsSparse ? fOutputArray->GetEntriesFast() : detID;
    TChargeData *const data =
      static_cast<TChargeData*>(fOutputArray->ConstructedAt(idx));
    if (!data) {
       Warning("Process","unexpected data type");
       break;
    }
//    if (IsValid(data->GetID())) return; // take only the first hit if not sparse
    data->SetID(detID);
    data->SetCharge(hit->Get());
   }

  if(fIsSparse) {
//    TChargeData::SetSortType( TChargeData::kCharge);
//    TChargeData::SetSortOrder(TChargeData::kASC);
//    fOutputArray->UnSort();
//    fOutputArray->Sort();
//    fOutputArray->Compress();
  } else {
    const Int_t n = fOutputArray->GetEntriesFast();
    for (Int_t i = 0; i!=n; ++i) {
      fOutputArray->ConstructedAt(i);
    }
  }
}
