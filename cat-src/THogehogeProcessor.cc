/**
 * @file   THogehogeProcessor.cc
 * @brief
 *
 * @date   Created       : 2015-02-06 06:53:17 JST
 *         Last Modified : Feb 10, 2015 05:30:10 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2015 Shinsuke OTA
 */

#include "THogehogeProcessor.h"

using art::THogehogeProcessor;

ClassImp(THogehogeProcessor)

THogehogeProcessor::THogehogeProcessor()
{
   RegisterInputCollection("InputNumRandom","name of input",fNumRandomName,TString("numrandom"),
                           &fNumRandom);
   RegisterInputCollection("InputRandom","name of input random",fRandomName,TString("uuu"),
                           &fRandom);
                                                                                
}

THogehogeProcessor::~THogehogeProcessor()
{
}

THogehogeProcessor::THogehogeProcessor(const THogehogeProcessor& rhs)
{
}

THogehogeProcessor& THogehogeProcessor::operator=(const THogehogeProcessor& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}


void THogehogeProcessor::Process()
{
   for (Int_t i=0; i!=**fNumRandom; ++i) {
      (*fRandom)[i] *= 10;
   }
}
