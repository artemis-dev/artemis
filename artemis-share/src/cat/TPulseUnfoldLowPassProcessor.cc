/* $Id:$ */
/**
 * @file   TPulseUnfoldLowPassProcessor.cc
 * @date   Created : Feb 01, 2014 17:01:43 JST
 *   Last Modified : Feb 01, 2014 19:05:19 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2014
 */
#include "TPulseUnfoldLowPassProcessor.h"
#include <TClonesArray.h>
#include <TCatPulseShape.h>

ClassImp(art::TPulseUnfoldLowPassProcessor);

using art::TPulseUnfoldLowPassProcessor;

TPulseUnfoldLowPassProcessor::TPulseUnfoldLowPassProcessor()
{
   StringVec_t def;
   def.push_back("at");
   RegisterInputCollection("InputCollection","pulse shape (TCatPulseShape)",
                           fInputColName,def);
   RegisterProcessorParameter("TimeConstant","time constant of amplifier which will be used for pole zero canceleration",
                              fTimeConstant,(Float_t)(1.));
}
TPulseUnfoldLowPassProcessor::~TPulseUnfoldLowPassProcessor()
{
}

void TPulseUnfoldLowPassProcessor::Init(TEventCollection *col)
{
   TEventObject *obj = NULL;
   if (!(obj = col->Get(fInputColName.front()))) {
      Info("Init","No such input %s",fInputColName.front().Data());
      return;
   }
   fInput = (TClonesArray**) obj->GetObjectRef();
   Info("Init","Input %s (overwritten)",fInputColName.front().Data());
   Info("Init","TimeConstant = %f",fTimeConstant);

}
void TPulseUnfoldLowPassProcessor::Process()
{
   Int_t nHits = (*fInput)->GetEntries();
   for (Int_t iHit = 0; iHit != nHits; iHit++) {
      TCatPulseShape *hit = (TCatPulseShape*)(*fInput)->At(iHit);
      Int_t nSample = hit->GetNumSample();
      Double_t sum = 0;
      for (Int_t is = 0; is != nSample; is++) {
         double qn = fTimeConstant * ((*hit)[is]-sum);
         sum = (*hit)[is];
         (*hit)[is] += qn;
         (*hit)[is] /= (fTimeConstant+1);
      }
      hit->SetQualityBit(TCatPulseShape::kLowPassUnfolded);
   }
}
