/**
 * @file   TScalerMappingProcessor.cc
 * @brief  event-by-event scaler mapping processor
 *
 * @date   Created       : 2018-06-27 17:39:22 JST
 *         Last Modified : 2018-06-29 19:14:11 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2018 Shinsuke OTA
 */

#include "TScalerMappingProcessor.h"
#include <TScalerData.h>
#include <TCategorizedData.h>
#include <TRawDataSimple.h>

using art::TScalerMappingProcessor;

ClassImp(TScalerMappingProcessor)

TScalerMappingProcessor::TScalerMappingProcessor()
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

TScalerMappingProcessor::~TScalerMappingProcessor()
{
   if (fScalerTotal) delete fScalerTotal;
   if (fScalerDiff) delete fScalerDiff;
   if (fScalerSaved) delete fScalerSaved;
}

TScalerMappingProcessor::TScalerMappingProcessor(const TScalerMappingProcessor& rhs)
{
}

TScalerMappingProcessor& TScalerMappingProcessor::operator=(const TScalerMappingProcessor& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}

void art::TScalerMappingProcessor::Init(TEventCollection *col)
{
  if (col->Get(fInputColName.front()) == NULL) {
    printf("No such collection %s\n",fInputColName.front().Data());
    return;
  }

  fCategorized = (TCategorizedData**)(col->Get(fInputColName.front())->GetObjectRef());
  if (!fCategorized) {
    printf("Initialization failed with %s\n",fInputColName.front().Data());
  }
  fScalerTotal = new TScalerData(fNumChannel);
  fScalerDiff = new TScalerData(fNumChannel);
  fScalerSaved = new TScalerData(fNumChannel);
  fScalerTotal->SetName(fOutputColNameTotal);
  fScalerDiff->SetName(fOutputColNameDiff);
  col->Add(fOutputColNameTotal,fScalerTotal,fOutputIsTransparent);
  col->Add(fOutputColNameDiff,fScalerDiff,fOutputIsTransparent);
}

void art::TScalerMappingProcessor::BeginOfRun()
{
   for (Int_t i=0; i!=fNumChannel; i++) {
      fScalerSaved->SetValue(i,0);
      fScalerTotal->SetValue(i,0);
      fScalerDiff->SetValue(i,0);
   }
}

void art::TScalerMappingProcessor::Process()
{
  if (!fCategorized) return;
  fScalerTotal->Clear("C");
  fScalerDiff->Clear("C");
  TObjArray *cat = (*fCategorized)->FindCategory(fCatID);
  if (!cat) {
     if (fVerboseLevel) {
        Warning("Process","no such category %d",fCatID);
     }
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
