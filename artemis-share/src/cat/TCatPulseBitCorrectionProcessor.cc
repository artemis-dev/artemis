/* $Id:$ */
/**
 * @file   TCatPulseBitCorrectionProcessor.cc
 * @date   Created : Nov 01, 2013 06:01:07 JST
 *   Last Modified : Jan 31, 2014 13:10:14 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#include "TCatPulseBitCorrectionProcessor.h"

#include <TCatPulseShape.h>
#include <TClonesArray.h>

using art::TCatPulseBitCorrectionProcessor;

TCatPulseBitCorrectionProcessor::TCatPulseBitCorrectionProcessor()
{
   StringVec_t defInput;
   defInput.push_back("at");
   RegisterInputCollection("InputCollection","pulse shape (TCatPulseShape), which will be updated",
                           fInputColName,defInput);
   RegisterProcessorParameter("MaxADC","maximum ADC value",fMaxADC,(Float_t)2800.);
}
TCatPulseBitCorrectionProcessor::~TCatPulseBitCorrectionProcessor()
{
}


void TCatPulseBitCorrectionProcessor::Init(TEventCollection *col)
{
   TEventObject *obj = NULL;
   if (!(obj = col->Get(fInputColName.front()))) {
      printf("[TCatPulseBitCorrectionProcessor]: No such input %s\n",fInputColName.front().Data());
      return;
   }
   fInput = (TClonesArray**) obj->GetObjectRef();
   printf("<TCatPulseBitCorrectionProcessor> Input :%s will be overwitten\n",fInputColName.front().Data());
   printf("<TCatPulseBitCorrectionProcessor> MaxADC = %.0f\n",fMaxADC);
   
}

void TCatPulseBitCorrectionProcessor::Process()
{
   Int_t nHits = (*fInput)->GetEntriesFast();
   for (Int_t iHit = 0; iHit != nHits; iHit++) {
      TCatPulseShape *hit = (TCatPulseShape*)(*fInput)->At(iHit);
      Int_t nSample = hit->GetNumSample();
      for (Int_t is = 0; is < nSample+1; is++) {
         if (((*hit)[is] > fMaxADC)) {
            if (is == 0) {
               (*hit)[is] = (*hit)[is+1] - ((*hit)[is+2]-(*hit)[is+1])/2.;
            } else if (is >= nSample-1) {
               (*hit)[is] = (*hit)[is-1] + ((*hit)[is-2]-(*hit)[is-1])/2.;
            } else {
	      (*hit)[is] = ((*hit)[is-1] + (*hit)[is+1])/2.;
            } 
            hit->SetQualityBit(TCatPulseShape::kWrongBitCorrected);
         }
      }
   }
}
