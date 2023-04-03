/* $Id:$ */
/**
 * @file   TCatAtSpillCounter.cc
 * @date   Created : Feb 12, 2013 23:12:29 JST
 *   Last Modified : Feb 13, 2013 01:00:01 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#include "TCatAtSpillCounter.h"
#include <TCategorizedData.h>
#include <TEventCollection.h>
#include <TString.h>
#include "TCatFadcRawData.h"
#include <TRawDataFadc.h>

art::TCatAtSpillCounter::TCatAtSpillCounter()
{
   StringVec_t defInput; defInput.push_back("categorized");
   RegisterInputCollection("InputCollection","categorized rawdata",
                           fInputColName,defInput);
   RegisterOutputCollection("OutputCollection","output collection name",
                            fOutputColName,TString("atspill"));
   RegisterProcessorParameter("CatID","Category ID of Spill Signal",fCatID,2);
}
art::TCatAtSpillCounter::~TCatAtSpillCounter()
{
}

void art::TCatAtSpillCounter::Init(art::TEventCollection* col)
{
  fCategorized = (TCategorizedData**)(col->Get(fInputColName.front())->GetObjectRef());
  if (!fCategorized) {
    printf("Initialization failed with %s\n",fInputColName.front().Data());
  }
  fCATHit = new TClonesArray("TCatFadcRawData");
  fCATHit->SetName(fOutputColName);
  col->Add(fOutputColName,fCATHit,fOutputIsTransparent);
}

void art::TCatAtSpillCounter::Process()
{
   fCATHit->Clear("C");
   TObjArray *cat = (*fCategorized)->FindCategory(fCatID);
   if (!cat) return;
   TIterator *itr = cat->MakeIterator();
   TObjArray *det = NULL;
   while ((det = (TObjArray*)itr->Next())) {
//      Int_t did = det->GetUniqueID(); // unused
      TRefArray *types = (TRefArray*)det->At(0); //only fadc object 
      if (!types) continue; // no hits found anyway
      Int_t nHit = types->GetLast()+1;
      for (Int_t iHit=0; iHit != nHit; iHit++) {
         TRawDataFadc* hitorg = (TRawDataFadc*) types->At(iHit);
         TCatFadcRawData *hit = (TCatFadcRawData*) fCATHit->ConstructedAt(fCATHit->GetEntriesFast());
         hit->SetID(hitorg->GetDetID());
         hit->SetTimestamp(hitorg->GetTimestamp());
         hit->SetGeo(hitorg->GetGeo());
         hit->SetCh(hitorg->GetCh());
      }
   }
}

