/* $Id:$ */
/**
 * @file   TMonitorMappingProcessor.cc
 * @date   Created : Jul 15, 2014 03:38:46 JST
 *   Last Modified : Jul 15, 2014 13:52:28 JST
 * @author Hiroshi TOKIEDA <tokieda@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2014
 */
#include "TMonitorMappingProcessor.h"
#include <TCategorizedData.h>
#include <TRawDataFadc.h>
#include "TCatPulseShape.h"
#include <TClonesArray.h>

ClassImp(art::TMonitorMappingProcessor)

art::TMonitorMappingProcessor::TMonitorMappingProcessor()
{
   StringVec_t defInput; defInput.push_back("catdata");
   RegisterInputCollection("InputCollection","rawdata object returned by TArtEventStore",
                           fInputColName,defInput);
   RegisterOutputCollection("OutputCollection","output collection name",
                            fOutputColName,TString("at"));
   RegisterProcessorParameter("CatID","Category ID of hodoscope",fCatID,1);
}
art::TMonitorMappingProcessor::~TMonitorMappingProcessor()
{
   delete fMoniHit;
}

void art::TMonitorMappingProcessor::Init(art::TEventCollection* col)
{
   fCategorized = (TCategorizedData**)(col->Get(fInputColName.front())->GetObjectRef());
   if (!fCategorized) {
      printf("Initialization failed with %s\n",fInputColName.front().Data());
   }
   fMoniHit = new TClonesArray("art::TCatPulseShape");
   if (!fMoniHit) {
      Error("Init","Initialize failed\n");
      return;
   }

   col->Add(fOutputColName,fMoniHit,fOutputIsTransparent);
}

void art::TMonitorMappingProcessor::Process()
{
   //if (!fCategorized) return;
   if (!fCategorized) {
      printf("no fCatecorized.\n");
      return;
   } 

   fMoniHit->Clear("C");
   TObjArray *cat = (*fCategorized)->FindCategory(fCatID);
   if (!cat) {
      // not always error
      printf("no cat.\n");
      return;
   }
   //printf("catid = %d\n",cat->GetUniqueID());
   //printf("entries = %d\n",cat->GetEntriesFast());
   const Int_t &ndet = cat->GetEntriesFast();
   TObjArray *det = NULL;
   for (Int_t idet = 0; idet!=ndet; idet++) {
      det = (TObjArray*) cat->At(idet);
//      Int_t did = det->GetUniqueID();
      TRefArray *types = (TRefArray*)det->At(0); //only fadc object 
      if (!types) continue; // no hits found anyway
      Int_t nHit = types->GetLast()+1;
      //printf("nHit = %d\n",nHit);
      for (Int_t iHit=0; iHit != nHit; iHit++) {
         TRawDataFadc* hitorg = (TRawDataFadc*) types->At(iHit);
         TCatPulseShape *hit = (TCatPulseShape*) fMoniHit->ConstructedAt(fMoniHit->GetEntriesFast());
         hit->SetID(hitorg->GetDetID());
         hit->SetGeo(hitorg->GetGeo());
         hit->SetCh(hitorg->GetCh());
         
         // make the patter human readable taking not
         Int_t nsample = hitorg->GetNumSample();
         Short_t* adc = hitorg->GetSample();
         for (Int_t is = 0; is != nsample; is++) {
//            hit->AddSample((*hitorg)[is]);
            hit->AddSample(adc[is]);
         }
      }
   }
}

