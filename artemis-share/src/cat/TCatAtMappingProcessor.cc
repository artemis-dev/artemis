/* $Id:$ */
/**
 * @file   TCatAtMappingProcessor.cc
 * @date   Created : Feb 06, 2013 14:06:38 JST
 *   Last Modified : 2022-09-19 21:55:16 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#include "TCatAtMappingProcessor.h"
#include <TCategorizedData.h>
#include <TRawDataFadc.h>
#include "TCatPulseShape.h"
#include <TCatReadoutPadArray.h>
#include <TCatReadoutPad.h>
#include <TClonesArray.h>

ClassImp(art::TCatAtMappingProcessor)

art::TCatAtMappingProcessor::TCatAtMappingProcessor()
: fCATHit(NULL)
{
   FloatVec_t beamRegion;
   beamRegion.push_back(-18);
   beamRegion.push_back(18);
   StringVec_t defInput; defInput.push_back("catdata");
   RegisterInputCollection("InputCollection","rawdata object returned by TArtEventStore",
                           fInputColName,defInput);
   RegisterOutputCollection("OutputCollection","output collection name",
                            fOutputColName,TString("at"));
   RegisterProcessorParameter("ReadoutPad","Name of Readout Pad",fReadoutPadName,
                              TString("readoutPad"));
   RegisterProcessorParameter("CatID","Category ID of hodoscope",fCatID,1);

   RegisterOptionalParameter("BeamRegion","Range of x position for beam region: default (-18,18)",
                             fBeamRegion,beamRegion);
   RegisterOptionalParameter("SelectIf","select all if 0 (default), recoil if 1, or beam if 2",fSelectIf,0);
   RegisterOptionalParameter("MaxDetID","Max ID of detector: default 415 (for type 2 readout pad)",fMaxDetID,415);
}
art::TCatAtMappingProcessor::~TCatAtMappingProcessor()
{
   if (fCATHit) delete fCATHit;
}

void art::TCatAtMappingProcessor::Init(art::TEventCollection* col)
{
   fCategorized = (TCategorizedData**)(col->Get(fInputColName.front())->GetObjectRef());
   if (!fCategorized) {
      printf("Initialization failed with %s\n",fInputColName.front().Data());
   }
   fCATHit = new TClonesArray("art::TCatPulseShape");
   if (!fCATHit) {
      Error("Init","Initialize failed\n");
      return;
   }
   Info("Init","MaxDetID = %d",fMaxDetID);

   fPads = (TCatReadoutPadArray*) col->GetInfo(fReadoutPadName);
   
   col->Add(fOutputColName,fCATHit,fOutputIsTransparent);
}

void art::TCatAtMappingProcessor::Process()
{
   if (!fCategorized) return;
   fCATHit->Clear("C");
   TObjArray *cat = (*fCategorized)->FindCategory(fCatID);
   if (!cat) {
      // not always error
      return;
   }
//   printf("catid = %d\n",cat->GetUniqueID());
//   printf("entries = %d\n",cat->GetEntriesFast());
   const Int_t &ndet = cat->GetEntriesFast();
   TObjArray *det = NULL;
   for (Int_t idet = 0; idet!=ndet; idet++) {
      det = (TObjArray*) cat->At(idet);
//      Int_t did = det->GetUniqueID();
      TRefArray *types = (TRefArray*)det->At(0); //only fadc object 
      if (!types) continue; // no hits found anyway
      Int_t nHit = types->GetLast()+1;
      for (Int_t iHit=0; iHit != nHit; iHit++) {
         TRawDataFadc* hitorg = (TRawDataFadc*) types->At(iHit);
         if (hitorg->GetDetID()>fMaxDetID) continue;
         TCatReadoutPad *pad =NULL;
         if (fPads) pad = (TCatReadoutPad*) fPads->At(hitorg->GetDetID());
         if (fSelectIf && fPads) {
            Bool_t isBeam = (fBeamRegion[0] < pad->X() && pad->X() < fBeamRegion[1]);
            if ((fSelectIf == 1 /* select recoil */ && isBeam) ||
                (fSelectIf == 2 /* select beam */   && !isBeam)) {
               continue;
            }
         }
         TCatPulseShape *hit = (TCatPulseShape*) fCATHit->ConstructedAt(fCATHit->GetEntriesFast());
         
         hit->SetID(hitorg->GetDetID());
         hit->SetAuxID(hitorg->GetUniqueID());
         hit->SetTime(hitorg->GetTimestamp());
         hit->SetGeo(hitorg->GetGeo());
         hit->SetCh(hitorg->GetCh());
         hit->SetOffset(hitorg->GetOffset());
         hit->Validate();
         if (pad && pad->IsBad()) {
            hit->SetQualityBit(TCatPulseShape::kInvalid);
            hit->SetQualityBit(TCatPulseShape::kBadPad);
         }
         
         
         if (pad) hit->SetXZ(pad->X(),pad->Y());
         
         // make the patter human readable taking not
         Int_t nsample = hitorg->GetNumSample();
         Short_t* adc = hitorg->GetSample();
         for (Int_t is = 0; is != nsample; is++) {
//            hit->AddSample((*hitorg)[is]);
            hit->AddSample((Short_t)adc[is]);
            
         }
      }
   }
}

