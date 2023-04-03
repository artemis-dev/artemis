/* $Id:$ */
/**
 * @file   TCatSiHitFindProcessor.cc
 * @date   Created : Oct 28, 2013 19:03:32 JST
 *
 * @author Motonobu Takaki <takaki@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 *
 */
#include "TCatSiHitFindProcessor.h"
#include <TClonesArray.h>
#include "TCatPulseShape.h"

art::TCatSiHitFindProcessor::TCatSiHitFindProcessor()
{
  StringVec_t defInput;
  defInput.push_back("siped");
  RegisterInputCollection("InputCollection","pedestal subtracted fadc data (TCatPulseShape)",fInputColName,defInput);
  RegisterOutputCollection("OutputCollection","output collection name (TCatPulseShape)",
			   fOutputColName,TString("sihit"));
  fSiHitOut = new TClonesArray("TCatPulseShape");

}
art::TCatSiHitFindProcessor::~TCatSiHitFindProcessor()
{
  delete fSiHitOut;
}

void art::TCatSiHitFindProcessor::Init(art::TEventCollection *col)
{
  if (col->Get(fInputColName.front())) {
    fSiHit = (TClonesArray**)(col->Get(fInputColName.front())->GetObjectRef());
    printf("<TCatSiHitFindProcessor> Input: %s, Output: %s\n",
	   fInputColName.front().Data(), fOutputColName.Data());
  }
  if (!fSiHit) {
    printf("<TCatSiHitFindProcessor> ERROR: collection %s does not exist\n",
	   fInputColName.front().Data());
    return;
  }
  fSiHitOut->Clear("C");
  fSiHitOut->SetName(fOutputColName);
  col->Add(fOutputColName,fSiHitOut,fOutputIsTransparent);
}

void art::TCatSiHitFindProcessor::Process()
{
  fSiHitOut->Clear("C");
  Int_t nHit = (*fSiHit)->GetEntriesFast();
  Float_t iThresholdL = 300.;
  Float_t iThresholdH = 3000.;
  Int_t nOverThreshold = 0;
  Bool_t bOverThreshold;
  for (Int_t iHit = 0; iHit!=nHit; iHit++) {
    TCatPulseShape *pulse = (TCatPulseShape*) (*fSiHit)->At(iHit);
    bOverThreshold = false;
    Int_t nSample = pulse->GetNumSample();
    Int_t nOverThresholdSample=0;
    for (Int_t i=0; i!=nSample; i++){
      if ((Float_t)(*pulse)[i] > iThresholdL && 
	  (Float_t)(*pulse)[i] < iThresholdH){
	// break;
	nOverThresholdSample++;
      }
      if (nOverThresholdSample > 100) {
	bOverThreshold = true;
      	break;
      }
    }


    if (bOverThreshold) {
      TCatPulseShape *dest = (TCatPulseShape*)fSiHitOut->ConstructedAt(fSiHitOut->GetEntriesFast());
      nOverThreshold++;
      dest->ResetQualityBit(TCatPulseShape::kInvalid);
      dest->SetID(pulse->GetID());
      dest->SetTime(pulse->GetTime());
      dest->SetGeo(pulse->GetGeo());
      dest->SetCh(pulse->GetCh());
      dest->SetOffset(pulse->GetOffset());
      // printf("hit ID: %d\n", pulse->GetID());
      for (Int_t i=0; i!=nSample; i++){
	dest->AddSample((Float_t)(*pulse)[i],(*pulse)(i));
      }
    } 
  }
  Bool_t Invalid = (nOverThreshold == 0);
  // Bool_t Invalid = ((nOverThreshold == 0) ||
  // 		    (nOverThreshold == 16) || 
  // 		    (nOverThreshold == 32));
  if (Invalid){
    // printf("<TCatSiHitFindProcessor> number of over threshold: %d\n", nOverThreshold);
    SetStopEvent();
    return;
  }
  // printf("<TCatSiHitFindProcessor> number of over threshold: %d\n", nOverThreshold);
}


	
