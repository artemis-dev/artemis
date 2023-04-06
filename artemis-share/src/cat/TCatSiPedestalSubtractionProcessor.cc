/* $Id:$ */
/**
 * @file   TCatSiCommonNoiseSubtractionProcessor.h
 * @date   Created : Oct 27, 2013 17:39:00 JST
 * @author Motonobu Takaki <takaki@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#include "TCatSiPedestalSubtractionProcessor.h"
#include <TCategorizedData.h>
#include <TRawDataFadc.h>
#include "TCatFadcRawData.h"
#include "TCatPulseShape.h"
#include <TH2F.h>
#include <TMath.h>

art::TCatSiPedestalSubtractionProcessor::TCatSiPedestalSubtractionProcessor()
  : fhSiPedMean(NULL), fhSiPedSigma(NULL)
{
  StringVec_t defInput;
  defInput.push_back("sicomsub");
  RegisterInputCollection("InputCollection","common noise subtracted fadc data (TCatPulseShape)", fInputColName,defInput);
  RegisterOutputCollection("OutputCollection","output collection name (TCatPulseShape)",
			   fOutputColName,TString("sisub"));

  fSiHitOut = new TClonesArray("TCatPulseShape");
}

art::TCatSiPedestalSubtractionProcessor::~TCatSiPedestalSubtractionProcessor()
{
  delete fSiHitOut;
}

void art::TCatSiPedestalSubtractionProcessor::Init(art::TEventCollection *col)
{
  if (col->Get(fInputColName.front())){
      fSiHit = (TClonesArray**)(col->Get(fInputColName.front())->GetObjectRef());
      printf("<TCatSiPedestalSubtractionProcessor> Input: %s, Output: %s\n",
	     fInputColName.front().Data(),fOutputColName.Data());
  }
  
  if (!fSiHit) {
    printf("ERROR: collection '%s' does not exist\n", fInputColName.front().Data());
    return;
  }
  fSiHitOut->Clear("C");
  fSiHitOut->SetName(fOutputColName);
  col->Add(fOutputColName,fSiHitOut,fOutputIsTransparent);
  if (fhSiPedMean) delete fhSiPedMean;
  if (fhSiPedSigma) delete fhSiPedSigma;
  fhSiPedMean = new TH2F("hSiPedMean", "hSiPedMean", 32,-0.5,31.5,200,-200.,200.);
  fhSiPedSigma = new TH2F("hSiPedSigma","hSiPedSigma",32,-0.5,31.5,100,0.,100.);
}

void art::TCatSiPedestalSubtractionProcessor::Process()
{
  fSiHitOut->Clear("C");
  Int_t nHit = (*fSiHit)->GetEntriesFast();
  for (Int_t iHit = 0; iHit != nHit; iHit++){
    TCatPulseShape *hit = (TCatPulseShape*)((*fSiHit)->At(iHit));
    TCatPulseShape *dest = (TCatPulseShape*)fSiHitOut->ConstructedAt(fSiHitOut->GetEntriesFast());
    dest->SetID(hit->GetID());
    dest->SetTime(hit->GetTime());
    dest->SetGeo(hit->GetGeo());
    dest->SetCh(hit->GetCh());
    dest->SetOffset(hit->GetOffset());


    Int_t nSample = hit->GetNumSample();
    Double_t nValid = 0;
    Double_t ped = 0;
    Double_t ped2 = 0;

    for (Int_t i=0; i<50; i++){
      if (TMath::Abs((*hit)[i])<100.){
	ped += (*hit)[i];
	ped2 += (*hit)[i]*(*hit)[i];
	nValid++;
      }
    }
    ped /= nValid;
    ped2 /= nValid;
    ped2 -= ped*ped;
    ped2 = TMath::Sqrt(ped2);
    fhSiPedMean->Fill(hit->GetID(),ped);
    fhSiPedSigma->Fill(hit->GetID(),ped2);
    for (Int_t i=0; i!=nSample; i++) {
      dest->AddSample((Float_t)(*hit)[i] - ped,(*hit)(i));
    }
  }
}



