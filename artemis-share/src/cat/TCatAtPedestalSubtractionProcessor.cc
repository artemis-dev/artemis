/* $Id:$ */
/**
 * @file   TCatAtPedestalSubtractionProcessor.cc
 * @date   Created : Mar 06, 2013 18:06:03 JST
 *   Last Modified : 2018-07-09 14:00:33 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *  
 *  
 *    Copyright (C)2013
 */
#include "TCatAtPedestalSubtractionProcessor.h"
#include <TCategorizedData.h>
#include <TRawDataFadc.h>
#include "TCatFadcRawData.h"
#include "TCatPulseShape.h"
#include <TH2F.h>
#include <TMath.h>

#include <vector>
#include <functional>
#include <algorithm>
#include <numeric>

art::TCatAtPedestalSubtractionProcessor::TCatAtPedestalSubtractionProcessor()
   : fhPedMean(NULL), fhPedSigma(NULL), fCATHitOut(NULL)
{
   StringVec_t defInput;
   defInput.push_back("atraw");
   RegisterInputCollection("InputCollection","fadc rawdata (TCatPulseShape)",
                           fInputColName,defInput);
   RegisterOutputCollection("OutputCollection","output collection name (TCatRadcRawData)",
                            fOutputColName,TString("atrawsub"));
   RegisterOptionalParameter("DoInvertPulse","flag for pulse inversion: default 0 (not inverted)",fDoInvertPulse,0);
}
art::TCatAtPedestalSubtractionProcessor::~TCatAtPedestalSubtractionProcessor()
{
   if (fCATHitOut) delete fCATHitOut;   
}

void art::TCatAtPedestalSubtractionProcessor::Init(art::TEventCollection *col)
{
   if (col->Get(fInputColName.front())) {
      fCATHit = (TClonesArray**)(col->Get(fInputColName.front())->GetObjectRef());
   }
   printf("TCatAtPedestalSubtractionProcessor: Input = %s\n",fInputColName.front().Data());
   if (!fCATHit) {
      printf("ERROR: collection '%s' does not exist\n",fInputColName.front().Data());
      return;
   }
   fCATHitOut = new TClonesArray("art::TCatPulseShape");
   if (!fCATHitOut) {
      Error("Init","Faild to initialize output collection");
      return;
   }

   fCATHitOut->Clear("C");
   fCATHitOut->SetName(fOutputColName);
   col->Add(fOutputColName,fCATHitOut,fOutputIsTransparent);
   if (fhPedMean) delete fhPedMean;
   if (fhPedSigma) delete fhPedSigma;
   fhPedMean = new TH2F("hPedMean","hPedMean",400,0.5,400.5,100,2000.,2100.);
   fhPedSigma = new TH2F("hPedSigma","hPedSigma",400,0.5,400.5,200,0.,50.);
}

void art::TCatAtPedestalSubtractionProcessor::Process()
{
   fCATHitOut->Clear("C");
   Int_t nHit = (*fCATHit)->GetEntriesFast();
   for (Int_t iHit = 0; iHit != nHit; iHit++) {
      TCatPulseShape *hit = (TCatPulseShape*)((*fCATHit)->At(iHit));
      TCatPulseShape *dest = (TCatPulseShape*)fCATHitOut->ConstructedAt(fCATHitOut->GetEntriesFast());
      std::vector<Float_t> &inSample = hit->GetSample();
      std::vector<Float_t>  &outSample = dest->GetSample();
      std::vector<Float_t>  &outClock  = dest->GetClock();
      hit->Copy(*dest);
//      dest->SetID(hit->GetID());
//      dest->SetTime(hit->GetTime());
//      dest->SetGeo(hit->GetGeo());
//      dest->SetCh(hit->GetCh());
//      dest->SetOffset(hit->GetOffset());
//      dest->SetPattern(hit->GetPattern());
      Int_t nSample = hit->GetNumSample();
      Double_t ped = std::accumulate(inSample.begin(),inSample.begin()+10,0.);
      Double_t ped2 = std::inner_product(inSample.begin(),inSample.begin()+10,inSample.begin(),0.);
      Int_t nValid = 0;
      
//      for (Int_t i=0; i<10; i++) { 
//         if ((*hit)[i]<3800) {
//            ped += (*hit)[i];
//            ped2 += (*hit)[i]*(*hit)[i];
//            nValid++; 
//         }
//      }
//      ped2 /= nValid;
//      ped /= nValid;
      ped /= 10.;
      ped2/=10.;
      ped2 -= ped*ped;
      ped2 = TMath::Sqrt(ped2);
      fhPedMean->Fill(hit->GetID(),ped);
      fhPedSigma->Fill(hit->GetID(),ped2);
      int num = inSample.size();
      outSample.resize(inSample.size());
      std::transform(inSample.begin(),inSample.end(),outSample.begin(),
                     std::bind(std::minus<float>(),std::placeholders::_1,ped));

      if(fDoInvertPulse){
         std::transform(outSample.begin(),outSample.end(),outSample.begin(),
                        std::bind(std::multiplies<int>(),std::placeholders::_1,-1));
      }

      outClock.resize(num);
      dest->SetNumSample(num);
      for (Int_t i=0; i!=num; i++) {
         outClock[i] = i;
      }
   }
}
