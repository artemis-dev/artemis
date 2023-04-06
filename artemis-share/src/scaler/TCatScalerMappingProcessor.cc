/* $Id:$ */
/**
 * @file   TCatScalerMappingProcessor.cc
 * @date   Created : Feb 13, 2013 20:13:55 JST
 *   Last Modified : 2018-06-25 21:21:40 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#include "TCatScalerMappingProcessor.h"
#include "TCatScalerData.h"
#include <TCategorizedData.h>
#include <TRawDataSimple.h>


art::TCatScalerMappingProcessor::TCatScalerMappingProcessor()
   : fCategorized(NULL), fScalerTotal(NULL), fScalerDiff(NULL),fScalerSaved(NULL)
{
   StringVec_t defInput; defInput.push_back("catdata");
   RegisterInputCollection("InputCollection","categorized input",
                           fInputColName,defInput);
   RegisterOutputCollection("OutputCollectionTotal","output collection name",
                            fOutputColNameTotal,TString("scatot"));
   RegisterOutputCollection("OutputCollectionDiff","output collection name",
                            fOutputColNameDiff,TString("scadiff"));
   RegisterProcessorParameter("CatID","Category ID of event scaler",fCatID,1);
   RegisterProcessorParameter("NumChannel","The Number of Scaler Channels",fNumChannel,32);
}
art::TCatScalerMappingProcessor::~TCatScalerMappingProcessor()
{
   if (fScalerTotal) delete fScalerTotal;
   if (fScalerDiff) delete fScalerDiff;
   if (fScalerSaved) delete fScalerSaved;
}

void art::TCatScalerMappingProcessor::Init(TEventCollection *col)
{
  if (col->Get(fInputColName.front()) == NULL) {
    printf("No such collection %s\n",fInputColName.front().Data());
    return;
  }

  fCategorized = (TCategorizedData**)(col->Get(fInputColName.front())->GetObjectRef());
  if (!fCategorized) {
    printf("Initialization failed with %s\n",fInputColName.front().Data());
  }
  fScalerTotal = new TCatScalerData(fNumChannel);
  fScalerDiff = new TCatScalerData(fNumChannel);
  fScalerSaved = new TCatScalerData(fNumChannel);
  fScalerTotal->SetName(fOutputColNameTotal);
  fScalerDiff->SetName(fOutputColNameDiff);
  col->Add(fOutputColNameTotal,fScalerTotal,fOutputIsTransparent);
  col->Add(fOutputColNameDiff,fScalerDiff,fOutputIsTransparent);
}

void art::TCatScalerMappingProcessor::BeginOfRun()
{
   for (Int_t i=0; i!=fNumChannel; i++) {
      fScalerSaved->SetValue(i,0);
      fScalerTotal->SetValue(i,0);
      fScalerDiff->SetValue(i,0);
   }
}

void art::TCatScalerMappingProcessor::Process()
{
  if (!fCategorized) return;
  fScalerTotal->Clear("C");
  fScalerDiff->Clear("C");
  TObjArray *cat = (*fCategorized)->FindCategory(fCatID);
  if (!cat) {
    printf("no such category %d\n",fCatID);
    return;
  }
  const Int_t &ndet = cat->GetEntriesFast();
  TObjArray *det = NULL;
  for (Int_t idet=0; idet!=ndet; idet++) {
    det = (TObjArray*) cat->At(idet);
    Int_t did = det->GetUniqueID();
    TRefArray *types = (TRefArray*) det->At(0);
    if (!types) continue;
    TRawDataSimple<UInt_t> *data = static_cast<TRawDataSimple<UInt_t>*>(types->At(0));
    // non-clear scaler
    fScalerTotal->SetValue(did,data->Get());
    if (fScalerSaved->GetValue(did)!=0) fScalerDiff->SetValue(did,(fScalerTotal->GetValue(did)-(fScalerSaved->GetValue(did))));
    fScalerSaved->SetValue(did,data->Get());
    //    printf("[did,fScalerTotal,fScalerDiff,fScalerSaved]=[%d,%d,%d,%d]\n",did,fScalerTotal->GetValue(did),fScalerDiff->GetValue(did),fScalerSaved->GetValue(did));
  }
}
