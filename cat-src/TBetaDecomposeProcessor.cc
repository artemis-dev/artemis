/**
 * @file   TBetaDecomposeProcessor.cc
 * @brief
 *
 * @date   Created       : 2015-04-14 08:15:44 JST
 *         Last Modified : May 05, 2015 21:42:11 JST
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2015
 */

#include "TBetaDecomposeProcessor.h"
#include <TMath.h>
#include <TClonesArray.h>
#include <TSimpleData.h>
#include <TTimeDifference.h>

using art::TBetaDecomposeProcessor;


ClassImp(TBetaDecomposeProcessor)

TBetaDecomposeProcessor::TBetaDecomposeProcessor()
{
   RegisterInputCollection("InputNameBrho1","name of input collection for Brho of the first section",
                           fInputNameBrho1,TString("brho35"),
                           &fInputBrho1,TClonesArray::Class_Name(),art::TSimpleData::Class_Name());
   RegisterInputCollection("InputNameBrho2","name of input collection for Brho of the second section",
                           fInputNameBrho2,TString("brho57"),
                           &fInputBrho2,TClonesArray::Class_Name(),art::TSimpleData::Class_Name());
   RegisterInputCollection("InputNameTOF","name of input collection for TOF",
                           fInputNameTOF,TString("tof37"),
                           &fInputTOF,TClonesArray::Class_Name(),art::TTimeDifference::Class_Name());
   RegisterProcessorParameter("Length1","length of the first section", fLength1,(Float_t)23.2832);
   RegisterProcessorParameter("Length2","length of the second section",fLength2,(Float_t)23.2832);

   RegisterOutputCollection("OutputNameBeta1","name of output collection for beata of the first section",
                            fOutputNameBeta1,TString("beta35"),
                            &fOutputBeta1,TClonesArray::Class_Name(),art::TSimpleData::Class_Name());
   RegisterOutputCollection("OutputNameBeta2","name of output collection for beata of the second section",
                            fOutputNameBeta2,TString("beta57"),
                            &fOutputBeta2,TClonesArray::Class_Name(),art::TSimpleData::Class_Name());
}

TBetaDecomposeProcessor::~TBetaDecomposeProcessor()
{
}

TBetaDecomposeProcessor::TBetaDecomposeProcessor(const TBetaDecomposeProcessor& rhs)
{
}

TBetaDecomposeProcessor& TBetaDecomposeProcessor::operator=(const TBetaDecomposeProcessor& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}

void TBetaDecomposeProcessor::Process()
{
   fOutputBeta1->Clear("C");
   fOutputBeta2->Clear("C");
   // local variables to be used to decompose beta
   Double_t t;  // (s) time of flight to be decomposed
   Double_t br1; // (Tm) brho at the first section
   Double_t br2; // (Tm) brho at the second section
   Double_t l1; // (m) length of the first section
   Double_t l2; // (m) length of the second section
   Double_t b1; // beta of the first section;
   Double_t b2; // beta of the second section;
   Double_t c = TMath::C(); // (m/s) speed of light

#if 0
   { // 132Sn
      t = 285.6e-9;
      br1 = 6.227;
      br2 = 4.655;
      l1 = 23.2832;
      l2 = 23.2832;
      // b1 = 0.604962
      // b2 = 0.49386
   }
#endif
   if ((*fInputBrho1)->GetEntriesFast() == 0) return;
   if ((*fInputBrho2)->GetEntriesFast() == 0) return;
   if ((*fInputTOF)->GetEntriesFast() == 0) return;
   t = ((TTimeDifference*)(*fInputTOF)->At(0))->GetTiming();
   t *= 1.e-9;
   br1 = ((TSimpleData*)(*fInputBrho1)->At(0))->GetValue();
   br2 = ((TSimpleData*)(*fInputBrho2)->At(0))->GetValue();
   // printf("%g %g %f %f\n",((TTimeDifference*)(*fInputTOF)->At(0))->GetTiming(),t,br1,br2);

   l1 = fLength1;
   l2 = fLength2;


   // temporary variables
   Double_t r = br1/br2; // ratio of the Brho's
   Double_t v1 = l1/c; 
   Double_t v2 = l2/c;

   Double_t tt = t * r * TMath::Sqrt(1 + (r-1) * (r+1) * (v2 - v1/r) * (v2 + v1/r) / t /t);

   b1 = (v1 * tt + v2 * r * r * t) / (t * tt + v1*v2*r*r - v1*v2);
   b2 = (v1 * tt + v2 * r * r * t) / (r*r*t*t - v1*v1*r*r + v1*v1);

   // printf("%f %f\n",b1,b2);

   ((TSimpleData*)fOutputBeta1->ConstructedAt(fOutputBeta1->GetEntriesFast()))->SetValue(b1);
   ((TSimpleData*)fOutputBeta2->ConstructedAt(fOutputBeta2->GetEntriesFast()))->SetValue(b2);
   
}

