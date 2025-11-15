/**
 * @file   THogeProcessor.cc
 * @brief  hoge
 *
 * @date   Created       : 2015-01-31 19:36:14 JST
 *         Last Modified : Feb 05, 2015 17:34:33 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2015 Shinsuke OTA
 */

#include "THogeProcessor.h"
#include <TRandom.h>

using art::THogeProcessor;

ClassImp(THogeProcessor)

THogeProcessor::THogeProcessor()
: fRandom(NULL), fRandom2(NULL), fNumRandom(NULL)
{
   RegisterOutputCollection("Random2","name of Random2",fOutputName2,TString("random2"),
                            "F",&fRandom2);
   RegisterOutputCollection("NumRandom","name of the number of random",fOutputName3,TString("numrandom"),
                            "I",&fNumRandom);
   RegisterOutputCollection("OutputCollection","name of output collection",fOutputName,TString("random"),
                            "D",&fRandom,10,&fOutputName3);
   
}

THogeProcessor::~THogeProcessor()
{
   if (fRandom) delete fRandom ;
   if (fRandom2) delete fRandom2;
   if (fNumRandom) delete fNumRandom; 
}

THogeProcessor::THogeProcessor(const THogeProcessor& rhs)
{
}

THogeProcessor& THogeProcessor::operator=(const THogeProcessor& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}

void THogeProcessor::Init(TEventCollection *col)
{
//   *(Double_t**)col->GetObjectRef(fOutputName) = fRandom =  new Double_t;
//   *((Float_t**)col->GetObjectRef(fOutputName2)) = fRandom2 = new Float_t;
//   *((Int_t**)col->GetObjectRef(fOutputName3)) = fNumRandom = new Int_t;
   printf("random === %p\n",fRandom);

}

void THogeProcessor::Process()
{
   static int flag = 0;
   if (!flag ) {
      printf("%p %p\n",fRandom,fNumRandom);
      flag = 1;
   }
   fRandom[0] = gRandom->Uniform() + 1;
   fRandom[1] = gRandom->Uniform() + 2;
   fRandom2[0]  = 0.5;
   fNumRandom[0] = 2;

//   printf("%f\n",*fRandom);
}
