/* $Id:$ */
/**
 * @file   TPulseMovingAverageProcessor.cc
 * @date   Created : Feb 02, 2014 16:02:27 JST
 *   Last Modified : 2016-04-17 12:04:26 JST (nil)

 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *
 *    Copyright (C)2014
 */
#include "TPulseMovingAverageProcessor.h"
#include <TCatPulseShape.h>
#include <TClonesArray.h>
#include <algorithm>
#include <numeric>
#include <TArtSystemOfUnit.h>

ClassImp(art::TPulseMovingAverageProcessor)

using art::TPulseMovingAverageProcessor;

TPulseMovingAverageProcessor::TPulseMovingAverageProcessor()
{
   StringVec_t def;
   def.push_back("at");
   RegisterInputCollection("InputCollection","pulse data (TCatPulseShape)",
                           fInputColName, "icw",
                           &fInput,TClonesArray::Class_Name(),TCatPulseShape::Class_Name());
   RegisterOutputCollection("OutputCollection","output pulse data (TCatPulseShape)",
                            fOutputColName,TString("icwave"),
                            &fOutput,TClonesArray::Class_Name(),TCatPulseShape::Class_Name());
   RegisterProcessorParameter("NumAverage","number of sample to be averaged",
                              fNumAverage, 10);
}
TPulseMovingAverageProcessor::~TPulseMovingAverageProcessor()
{
}


void TPulseMovingAverageProcessor::Init(TEventCollection *col)
{
}


void TPulseMovingAverageProcessor::Process()
{
   fOutput->Clear("C");
   Int_t nHits = (*fInput)->GetEntriesFast();
   for (Int_t iHit = 0; iHit != nHits; iHit++) {
      TCatPulseShape *hit = (TCatPulseShape*) (*fInput)->At(iHit);
      TCatPulseShape *out = (TCatPulseShape*) (fOutput)->ConstructedAt(iHit);
      hit->Copy(*out);
      if (!hit->GetNumSample()) continue;
      
      const std::vector<Float_t>& sample = hit->GetSample();
      const std::vector<Float_t>::const_iterator itend = sample.end();
      std::vector<Float_t>& outs = out->GetSample();
      std::vector<Float_t>& outc = out->GetClock();
      outs.resize(hit->GetNumSample()-fNumAverage);
      outc.resize(hit->GetNumSample()-fNumAverage);
      out->SetOffset(out->GetOffset() + fNumAverage);
      Double_t sum = 0;
      Int_t count = 0;
      for (std::vector<Float_t>::const_iterator it = sample.begin();
           it < itend; ++it,++count) {
         if (count < fNumAverage) {
            sum += *it;
//            outs[count] = sum  / (count + 1);
         } else {
            sum += *it;
            sum -= *(it - fNumAverage);
            outs[count-fNumAverage] = sum  / (fNumAverage);
            outc[count-fNumAverage] = (double)(count-fNumAverage);
         }            
      }
   }
}
