/* $Id:$ */
/**
 * @file   TPulseUnfoldHighPassProcessor.cc
 * @date   Created : Jan 31, 2014 13:31:12 JST
 *   Last Modified : Feb 19, 2014 15:41:42 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2014
 */
#include "TPulseUnfoldHighPassProcessor.h"
#include <TCatPulseShape.h>
#include <TClonesArray.h>

#include <algorithm>
#include <functional>
#include <numeric>

ClassImp(art::TPulseUnfoldHighPassProcessor);

using art::TPulseUnfoldHighPassProcessor;

TPulseUnfoldHighPassProcessor::TPulseUnfoldHighPassProcessor()
{
   StringVec_t def;
   def.push_back("at");
   RegisterInputCollection("InputCollection","pulse shape (TCatPulseShape)",
                           fInputColName,def);
   RegisterProcessorParameter("TimeConstant","time constant of amplifier which will be used for pole zero canceleration",
                              fTimeConstant,(Float_t)(16./20.));
}
TPulseUnfoldHighPassProcessor::~TPulseUnfoldHighPassProcessor()
{
}

void TPulseUnfoldHighPassProcessor::Init(TEventCollection *col)
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

void TPulseUnfoldHighPassProcessor::Process()
{
   Int_t nHits = (*fInput)->GetEntries();
   const Float_t kM = 1./(fTimeConstant+1);
   for (Int_t iHit = 0; iHit != nHits; iHit++) {
      TCatPulseShape *hit = (TCatPulseShape*)(*fInput)->At(iHit);
      std::vector<Float_t>& sample = hit->GetSample();
      std::vector<Float_t> sum(sample.size());
      std::partial_sum(sample.begin(),sample.end(),sum.begin());
      std::transform(sample.begin(),sample.end(),sample.begin(),
                     std::bind(std::multiplies<Float_t>(),std::placeholders::_1,fTimeConstant));
      std::transform(sample.begin(),sample.end(),sum.begin(),sample.begin(),std::plus<Float_t>());
      std::transform(sample.begin(),sample.end(),sample.begin(),std::bind(std::multiplies<Float_t>(),std::placeholders::_1,kM));
      hit->SetQualityBit(TCatPulseShape::kHighPassUnfolded);
   }
}
