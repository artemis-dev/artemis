/* $Id:$ */
/**
 * @file   TCatSiCommonNoiseSubtractionProcessor.cc
 * @date   Created : Oct 26, 2013 17:39:00 JST
 * @author Motonobu Takaki <takaki@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#include "TCatSiCommonNoiseSubtractionProcessor.h"
#include <TCategorizedData.h>
#include <TRawDataFadc.h>
#include "TCatFadcRawData.h"
#include "TCatPulseShape.h"
#include <TH2F.h>
#include <TMath.h>

art::TCatSiCommonNoiseSubtractionProcessor::TCatSiCommonNoiseSubtractionProcessor()
//  : fhSiPedMean(NULL), fhSiPedSigma(NULL)
{
  StringVec_t defInput;
  defInput.push_back("siraw");
  RegisterInputCollection("InputCollection","fadc rawdata (TCatPulseShape)",
			  fInputColName,defInput);
  RegisterOutputCollection("OutputCollection","output collection name (TcatFadcRawData)",
			   fOutputColName,TString("sirawsub"));
  fSiHitOut = new TClonesArray("TCatPulseShape");
}
art::TCatSiCommonNoiseSubtractionProcessor::~TCatSiCommonNoiseSubtractionProcessor()
{
  delete fSiHitOut;
}

void art::TCatSiCommonNoiseSubtractionProcessor::Init(art::TEventCollection *col)
{
  if (col->Get(fInputColName.front())) {
    fSiHit = (TClonesArray**)(col->Get(fInputColName.front())->GetObjectRef());
    printf("<TCatSiCommonNoiseSubtractionProcessor> Input: %s, Output: %s\n", 
	   fInputColName.front().Data(), fOutputColName.Data());
    printf("TCatSiCommonNoiseSubtractionProcessor: Output = %s\n", fOutputColName.Data());
  }
  if (!fSiHit) {
    printf("ERROR: collection '%s' does not exist\n",fInputColName.front().Data());
    return;
  }
  fSiHitOut->Clear("C");
  fSiHitOut->SetName(fOutputColName);
  col->Add(fOutputColName,fSiHitOut,fOutputIsTransparent);
  // if (fhSiPedMean) delete fhSiPedMean;
  // if (fhSiPedSigma) delete fhSiPedSigma;
  // fhSiPedMean = new TH2F("hSiPedMean","hSiPedMean",400,0.5,400.5,100,500.,1000.);
  // fhSiPedSigma = new TH2F("hSiPedSigma","hSiPedSigma",400,0.5,400.5,200,0.,200.);
}

void art::TCatSiCommonNoiseSubtractionProcessor::Process()
{
  fSiHitOut->Clear("C");
  Int_t nHit = (*fSiHit)->GetEntriesFast();
  //  Int_t nStrip = 32;
  Int_t iThreshold = 1000;
  Int_t nUnderthreshold = 0;
  Int_t iSumPed[800];
  // initialize for iSumPed
  for (Int_t i=0; i < 800; i++) {
    iSumPed[i] = 0;
  }
  for (Int_t iHit = 0; iHit != nHit; iHit++) {
    Bool_t bOverThreshold = false;
    TCatFadcRawData *hit = (TCatFadcRawData*) ((*fSiHit)->At(iHit));
    Int_t nSample = hit->GetNumSample();
    for (Int_t j=0; j!=nSample; j++){
      if ((*hit)[j] > iThreshold) {
	bOverThreshold = true;
	break;
      }
    }
    if (!bOverThreshold) {
      for (Int_t k=0; k!=nSample; k++){
	iSumPed[k] += (*hit)[k];
      }
      nUnderthreshold++;
    }
  }
  for (Int_t iHit=0; iHit!=nHit; iHit++) {
    TCatFadcRawData *hitstrip = (TCatFadcRawData*)((*fSiHit)->At(iHit));
    TCatPulseShape *dest = (TCatPulseShape*)fSiHitOut->ConstructedAt(fSiHitOut->GetEntriesFast());
    dest->SetID(hitstrip->GetID());
    dest->SetTime(hitstrip->GetTimestamp());
    dest->SetGeo(hitstrip->GetGeo());
    dest->SetCh(hitstrip->GetCh());
    dest->SetOffset(hitstrip->GetOffset());
    Int_t nSample = hitstrip->GetNumSample();
    for (Int_t l=0; l!=nSample; l++){
      dest->AddSample((Float_t)(*hitstrip)[l] - (Float_t)iSumPed[l]/nUnderthreshold,(*hitstrip)(l));
    }
  }
}


    
      








