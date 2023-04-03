/**
 * @file   TPIDProcessor.cc
 * @brief  particle identification
 *
 * @date   Created       : 2015-04-16 01:53:40 JST
 *         Last Modified : Apr 17, 2015 01:02:19 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2015 Shinsuke OTA
 */

#include "TPIDProcessor.h"
#include <TSimpleData.h>
#include <TClonesArray.h>
#include <TMWDCTrackingResult.h>
#include <TMath.h>


using art::ms1502::TPIDProcessor;

ClassImp(TPIDProcessor)

TPIDProcessor::TPIDProcessor()
{

   FloatVec_t def; 
   RegisterInputCollection("InputNameBrho1","name of brho at the first section",
                           fInputNameBrho1,TString("brho35"),
                           &fInputBrho1,TClonesArray::Class_Name(),TSimpleData::Class_Name());
   RegisterInputCollection("InputNameBrho2","name of brho at the second section",
                           fInputNameBrho2,TString("brho57"),
                           &fInputBrho2,TClonesArray::Class_Name(),TSimpleData::Class_Name());
   RegisterInputCollection("InputNameBeta1","name of beta at the first section",
                           fInputNameBeta1,TString("beta35"),
                           &fInputBeta1,TClonesArray::Class_Name(),TSimpleData::Class_Name());
   RegisterInputCollection("InputNameBeta2","name of beta at the first section",
                           fInputNameBeta2,TString("beta57"),
                           &fInputBeta2,TClonesArray::Class_Name(),TSimpleData::Class_Name());
   RegisterInputCollection("InputNameDF","name of dispersive forcus where the degrader is set",
                           fInputNameDF,TString("f5"),
                           &fInputDF,TClonesArray::Class_Name(),TMWDCTrackingResult::Class_Name());

   RegisterOutputCollection("OutputNameAQ35","name of output A/Q from 35",
                            fOutputNameAQ35,TString("aq35"),
                            &fOutputAQ35,TClonesArray::Class_Name(),TSimpleData::Class_Name());
   RegisterOutputCollection("OutputNameAQ57","name of output A/Q from 57",
                            fOutputNameAQ57,TString("aq57"),
                            &fOutputAQ57,TClonesArray::Class_Name(),TSimpleData::Class_Name());
   RegisterOutputCollection("OutputNameZdeg","name of output atomic number",
                            fOutputNameZdeg,TString("Zdeg"),
                            &fOutputZdeg,TClonesArray::Class_Name(),TSimpleData::Class_Name());
   


   def.clear(); def.push_back(3.5); def.push_back(0.003);
   RegisterProcessorParameter("DegraderThickness","d = p0 + p1 * x",
                              fDegraderThickness,def);
   RegisterProcessorParameter("AtomicNumberNormalization","normalization factor to calculate atomic number",
                              fAtomicNumberNormalization,(Float_t)1.);

}

TPIDProcessor::~TPIDProcessor()
{
}

TPIDProcessor::TPIDProcessor(const TPIDProcessor& rhs)
{
}

TPIDProcessor& TPIDProcessor::operator=(const TPIDProcessor& rhs)
{
   if (this != &rhs) {

   }
   return *this;
}

void TPIDProcessor::Process()
{
   fOutputAQ35->Clear("C");
   fOutputAQ57->Clear("C");
   fOutputZdeg->Clear("C");
   if (!(*fInputBrho1)->GetEntriesFast()) return;
   if (!(*fInputBrho2)->GetEntriesFast()) return;
   if (!(*fInputBeta1)->GetEntriesFast()) return;
   if (!(*fInputBeta2)->GetEntriesFast()) return;
   if (!(*fInputDF)->GetEntriesFast()) return;

   Double_t c1 = fAtomicNumberNormalization; // const for atomic number
   Double_t x = ((TMWDCTrackingResult*)(*fInputDF)->At(0))->GetTrack()->X(); // [mm] position at f5
   Double_t d = fDegraderThickness[0] + fDegraderThickness[1] * x; // [mm] thickness of energy degrader
   Double_t r35 = ((TSimpleData*)(*fInputBrho1)->At(0))->GetValue(); // [Tm] brho35 
   Double_t r57 = ((TSimpleData*)(*fInputBrho2)->At(0))->GetValue(); // [Tm] brho57
   Double_t b35 = ((TSimpleData*)(*fInputBeta1)->At(0))->GetValue(); // b35
   Double_t b57 = ((TSimpleData*)(*fInputBeta2)->At(0))->GetValue(); // b57
   // printf("b35=%f b57=%f\n",b35,b57);
   Double_t g35 = 1/sqrt(1-b35*b35);
   Double_t g57 = 1/sqrt(1-b57*b57);
   Double_t c = TMath::C()/1e6;
   Double_t m = 931.494; // MeV
   Double_t aq35 = c * r35 / (b35*g35*m);
   Double_t aq57 = c * r57 / (b57*g57*m);
   Double_t z = c1 * (r35/b35 - r57/b57) * b35*b35 / d;
   // printf("br35=%f br57=%f aq35=%f aq57=%f z=%f\n",r35,r57,aq35,aq57,z);
   
   ((TSimpleData*)fOutputAQ35->ConstructedAt(fOutputAQ35->GetEntriesFast()))->SetValue(aq35);
   ((TSimpleData*)fOutputAQ57->ConstructedAt(fOutputAQ57->GetEntriesFast()))->SetValue(aq57);
   ((TSimpleData*)fOutputZdeg->ConstructedAt(fOutputZdeg->GetEntriesFast()))->SetValue(z);
   
   return ;
}
