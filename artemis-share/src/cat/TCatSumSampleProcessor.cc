/* $Id:$ */
/**
 * @file   TCatSumSampleProcessor.cc
 * @date   Created : Mar 11, 2013 05:11:49 JST
 *   Last Modified : Jan 27, 2014 18:07:06 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#include "TCatSumSampleProcessor.h"
#include <TClonesArray.h>

#include <TCatPulseShape.h>

using art::TCatSumSampleProcessor;

TCatSumSampleProcessor::TCatSumSampleProcessor()
   : fCatHit(NULL), fCatHitOut(NULL)
{
   StringVec_t defInput;
   defInput.push_back("atrawsub");
   RegisterInputCollection("InputCollection","pedestal subtracted pulse (TCatPulseShape)",
                           fInputColName,defInput);
   RegisterOutputCollection("OutputCollection","output collection name (TCatRadcRawData)",
                            fOutputColName,TString("atpulse"));
   RegisterProcessorParameter("Sparse","hit will be inserted at the index corresponding to its id if sparse is 1, otherwize hit will be added ordinaly (default: 0)",fIsSparse,1);
   RegisterProcessorParameter("Overwrite","Overwriting the charge information and ignore OutputCollection if 1 (default), otherwise create new output",fOverwrite,1);

}
TCatSumSampleProcessor::~TCatSumSampleProcessor()
{
   if (fCatHitOut) delete fCatHitOut;
   fCatHitOut = NULL;
}

void TCatSumSampleProcessor::Init(TEventCollection *col)
{
   TEventObject *obj = NULL;
   if (!(obj = col->Get(fInputColName.front()))) {
      printf("[TCatSumSampleProcessor]: No such input %s\n",fInputColName.front().Data());
      return;
   }
   fCatHit = (TClonesArray**) obj->GetObjectRef();
   if (fVerboseLevel) {
      Info("Init","Input : %s, Output : %s, IsSparse = %d",
           fInputColName.front().Data(),fOutputColName.Data(),fIsSparse);
   }
   if (!fOverwrite) {
      fCatHitOut = new TClonesArray("art::TCatPulseShape");
      if (!fCatHitOut || ! fCatHitOut->GetClass() ) {
         Error("Init","Faild to initialize output collection");
         return;
      }
      fCatHitOut->SetName(fOutputColName);
      col->Add(fOutputColName.Data(),fCatHitOut,fOutputIsTransparent);
   }
}

void TCatSumSampleProcessor::Process()
{
   if (!fOverwrite) {
      fCatHitOut->Clear("C");
      TClonesArray &arr = **fCatHit;
      Int_t nHits = arr.GetEntriesFast();
      for (Int_t iHit = 0; iHit != nHits; iHit++) {
         TCatPulseShape *hit = (TCatPulseShape*)arr.At(iHit);
         TCatPulseShape *dest = NULL;
         if (!fIsSparse) {
            dest = (TCatPulseShape*)fCatHitOut->ConstructedAt(fCatHitOut->GetEntriesFast());
         } else {
            dest = (TCatPulseShape*)fCatHitOut->ConstructedAt(hit->GetID());
         }
         Double_t charge = 0.;
         Int_t nSample = hit->GetNumSample();
         dest->SetID(hit->GetID());
         dest->SetTime(hit->GetTime());
         dest->SetGeo(hit->GetGeo());
         dest->SetCh(hit->GetCh());
         dest->SetOffset(hit->GetOffset());
         for (Int_t i=0; i!=nSample; i++) {
            dest->AddSample((Float_t)(*hit)[i],(*hit)(i));
            charge += (*hit)[i];
         }
         dest->SetCharge(charge);
         if (charge > 0) {
            dest->ResetQualityBit(TCatPulseShape::kInvalid);
         }
      }
      for (Int_t i=0; i!=fCatHitOut->GetEntriesFast(); i++) {
         // to avoid NULL dereference
         if (fCatHitOut->UncheckedAt(i)) continue;
         fCatHitOut->ConstructedAt(i);
      }
   } else {
      TClonesArray &arr = **fCatHit;
      Int_t nHits = arr.GetEntriesFast();
      for (Int_t iHit = 0; iHit != nHits; iHit++) {
         TCatPulseShape *hit  = (TCatPulseShape*)arr.At(iHit);
         Int_t nSample = hit->GetNumSample();
         Double_t charge = 0.;
         for (Int_t i=0; i!=nSample; i++) {
            charge += (*hit)[i];
         }
         hit->SetCharge(charge);
      }
   }
}
