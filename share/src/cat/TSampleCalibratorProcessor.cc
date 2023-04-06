/* $Id:$ */
/**
 * @file   TSampleCalibratorProcessor.cc
 * @date   Created : Feb 20, 2014 08:20:29 JST
 *   Last Modified : 2018-02-21 13:51:21 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2014
 */
#include "TSampleCalibratorProcessor.h"
#include <TCatPulseShape.h>
#include <TConverterBase.h>
#include <vector>
#include <TClonesArray.h>

using art::TSampleCalibratorProcessor;

ClassImp(TSampleCalibratorProcessor)

TSampleCalibratorProcessor::TSampleCalibratorProcessor()
   : fHits(NULL),fADCConverterArray(NULL)
{
   StringVec_t input;
   input.push_back("at");
   RegisterInputCollection("InputCollection","input collection",fInputColName,input);
   RegisterProcessorParameter("ADCConverterArrayName","converter array name for ADC",fADCConverterArrayName,
                              TString(""));
}
TSampleCalibratorProcessor::~TSampleCalibratorProcessor()
{
}

void TSampleCalibratorProcessor::Init(TEventCollection *col)
{
   fHits = (TClonesArray**)  col->GetObjectRef(fInputColName[0]);
   if (!fHits) {
      Error("Init","No such input '%s'",fInputColName[0].Data());
      return;
   }
   fADCConverterArray = (TClonesArray**) col->GetInfoRef(fADCConverterArrayName);
   if (!fADCConverterArray) {
      Error("Init","No such converter '%s'",fADCConverterArrayName.Data());
      return;
   }
}
   
void TSampleCalibratorProcessor::Process()
{
   if (!fHits) return;
   if (!fADCConverterArray) return;

   Int_t nHits = (*fHits)->GetEntriesFast();
   for (Int_t iHit = 0; iHit != nHits; iHit++) {
      TCatPulseShape *pulse = (TCatPulseShape*) (*fHits)->At(iHit);
      std::vector<Float_t>& sample = pulse->GetSample();
      Int_t nSample = pulse->GetNumSample();
      TConverterBase* converter = (TConverterBase*) (*fADCConverterArray)->At(pulse->GetID());
      if (converter) {
         for (Int_t i=0; i!=nSample; i++) {
            sample[i] = converter->Convert(sample[i]);
         }
      }
   }
}
                                         
