/**
 * @file   TPulsePoleZeroCancellationProcessor.cc
 * @brief  pole zero cancellation
 *
 * @date   Created       : 2018-02-09 16:32:20 JST
 *         Last Modified : 2018-02-09 17:05:51 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2018 Shinsuke OTA
 */

#include "TPulsePoleZeroCancellationProcessor.h"
#include <TClonesArray.h>
#include <TCatPulseShape.h>
#include <numeric>


using art::TPulsePoleZeroCancellationProcessor;

ClassImp(TPulsePoleZeroCancellationProcessor)

TPulsePoleZeroCancellationProcessor::TPulsePoleZeroCancellationProcessor()
{

   RegisterInputCollection("InputCollection","input array name", fNameInput, TString("atpulse"),
                           &fInput,TClonesArray::Class_Name(),TCatPulseShape::Class_Name());
   RegisterProcessorParameter("TimeConstantPZ","pole zero time constant",
                              fTimeConstantPZ,1.);
   
}

TPulsePoleZeroCancellationProcessor::~TPulsePoleZeroCancellationProcessor()
{
}

TPulsePoleZeroCancellationProcessor::TPulsePoleZeroCancellationProcessor(const TPulsePoleZeroCancellationProcessor& rhs)
{
}

TPulsePoleZeroCancellationProcessor& TPulsePoleZeroCancellationProcessor::operator=(const TPulsePoleZeroCancellationProcessor& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}

void TPulsePoleZeroCancellationProcessor::Process()
{
   Int_t nHits = (*fInput)->GetEntries();
   
   for (Int_t iHit = 0; iHit != nHits; iHit++) {
      TCatPulseShape *hit = (TCatPulseShape*)(*fInput)->At(iHit);
      Int_t nSample = hit->GetNumSample();
      std::vector<Float_t>& sample = hit->GetSample();
      std::vector<Float_t> sum(sample.size());
      std::partial_sum(sample.begin(),sample.end(),sum.begin());
      for (Int_t i = 1; i<nSample; ++i) {
         sample[i] = sum[i] - (1-fTimeConstantPZ)*sum[i-1]+ (-fTimeConstantPZ)*sample[i-1];
      }
   }   
   
}
