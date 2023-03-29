/* $Id:$ */
/**
 * @file   TCatDiffSampleProcessor.cc
 * @date   Created : Mar 11, 2013 05:11:49 JST
 *   Last Modified : Jan 31, 2014 18:24:46 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#include "TCatDiffSampleProcessor.h"

#include <TCatPulseShape.h>
#include <TClonesArray.h>

ClassImp(art::TCatDiffSampleProcessor);

using art::TCatDiffSampleProcessor;

TCatDiffSampleProcessor::TCatDiffSampleProcessor()
   : fCatHit(NULL), fCatHitOut(NULL)
{
   StringVec_t defInput;
   defInput.push_back("atrawsub");
   RegisterInputCollection("InputCollection","pedestal subtracted pulse (TCatPulseShape)",
                           fInputColName,defInput);
   RegisterOutputCollection("OutputCollection","output collection name (TCatRadcRawData)",
                            fOutputColName,TString("atpulse"));
   RegisterProcessorParameter("Sparse","hit will be inserted at the index corresponding to its id if sparse is 0, otherwize hit will be added ordinaly (default: 1)",fIsSparse,1);


}
TCatDiffSampleProcessor::~TCatDiffSampleProcessor()
{
   if (fCatHitOut) delete fCatHitOut;
   fCatHitOut = NULL;
}

void TCatDiffSampleProcessor::Init(TEventCollection *col)
{
   TEventObject *obj = NULL;
   if (!(obj = col->Get(fInputColName.front()))) {
      Info("Init","No such input %s",fInputColName.front().Data());
      return;
   }
   fCatHit = (TClonesArray**) obj->GetObjectRef();
   printf("Input :%s, Output: %s\n",fInputColName.front().Data(),
          fOutputColName.Data());
   printf("fIsSparse = %d\n",fIsSparse);

   if (fInputColName.front() == fOutputColName) {
      fCatHitOut = *fCatHit;
      return;
   }
   if (!fCatHitOut) {
      fCatHitOut = new TClonesArray("art::TCatPulseShape");
   }
   fCatHitOut->SetName(fOutputColName);
   col->Add(fOutputColName.Data(),fCatHitOut,fOutputIsTransparent);
}

void TCatDiffSampleProcessor::Process()
{
   if (fCatHitOut != *fCatHit) {
      TIter next(*fCatHit);
      TObject *obj = NULL;
      fCatHitOut->Clear("C");
      while ((obj = next())) {
         TCatPulseShape *hit  = (TCatPulseShape*)obj;
         TCatPulseShape *dest;
         if (fIsSparse) {
            dest = (TCatPulseShape*)fCatHitOut->ConstructedAt(fCatHitOut->GetEntriesFast());
         } else {
            dest = (TCatPulseShape*)fCatHitOut->ConstructedAt(hit->GetID());
         }
         Double_t charge = 0;
         Int_t nSample = hit->GetNumSample();
         dest->SetID(hit->GetID());
         dest->SetTime(hit->GetTime());
         dest->SetGeo(hit->GetGeo());
         dest->SetCh(hit->GetCh());
         dest->SetOffset(hit->GetOffset());
         for (Int_t i=1; i!=nSample; i++) {
            dest->AddSample((Float_t)((*hit)[i]-(*hit)[i-1]));
         }
         dest->ResetQualityBit(TCatPulseShape::kInvalid);
      }
      for (Int_t i=0; i!=fCatHitOut->GetEntriesFast(); i++) {
         fCatHitOut->ConstructedAt(i);
      }
   } else {
      TIter next(*fCatHit);
      TObject *obj;
      while ((obj = next())) {
         TCatPulseShape *hit  = (TCatPulseShape*)obj;
         Int_t nSample = hit->GetNumSample();
         Double_t charge;
         for (Int_t i=0; i!=nSample; i++) {
            charge += (*hit)[i];
         }
         hit->SetCharge(charge);
      }
   }
}
