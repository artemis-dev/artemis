/**
 * @file   TRawDataMappingProcessor.h
 * @brief  Mapping rawdata 
 *
 * @date   Created       : 2023-02-13 10:45:49 JST
 *         Last Modified : 2023-02-13 17:05:47 JST
 * @author Shinsuke OTA <ota@rcnp.osaka-u.ac.jp>
 *
 *    (C) 2023 Shinsuke OTA
 */

#include "TRawDataMappingProcessor.h"

#include "TCategorizedData.h"



ClassImp(art::TRawDataMappingProcessor)


art::TRawDataMappingProcessor::TRawDataMappingProcessor()
{

   Register(fInputCatData("Input","The name of input categorized data","catdata"));
   Register(fOutput("Output","The name of output collection","odata"));
   Register(fOutputClass("OutputClass","The name of output class","art::TRawDataObject"));
   Register(fCatID("CatID","Category ID of rawdata",0));
   Register(fDataType("DataType","Data Type (column in map)",0));
}


art::TRawDataMappingProcessor::~TRawDataMappingProcessor()
{
   
}

void art::TRawDataMappingProcessor::Init(TEventCollection *col)
{
   if (!TClass::GetClass(fOutputClass.Value())) {
      SetStateError(Form("No such class %s is given",fOutputClass.Value().Data()));
      return;
   }
   printf("%s\n",fOutputClass.Value().Data());
   fOutput->SetClass(fOutputClass.Value().Data());
}

void art::TRawDataMappingProcessor::Process()
{
   fOutput->Clear("C");

   TObjArray *cat = fInputCatData->FindCategory(fCatID);
   if (!cat) {
      if (fVerboseLevel) {
         Warning("Process","no such category %d",fCatID.Value());
      }
      return;
   }
   for (Int_t idet = 0, ndet = cat->GetEntriesFast(); idet != ndet; idet++) {
      TObjArray *det = (TObjArray*) cat->At(idet);
      TObjArray *types = (TObjArray*) det->At(fDataType);
      if (!types) continue;
      for (Int_t idata = 0, ndata = types->GetEntriesFast(); idata < ndata; ++idata) {
         TObject *objin = types->At(idata);
         TObject *objout = fOutput->ConstructedAt(fOutput->GetEntriesFast());
//         *objout = *objin;
         objin->Copy(*objout);
      }
   }
      
}
