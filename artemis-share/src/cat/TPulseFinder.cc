/* $Id:$ */
/**
 * @file   TPulseFinder.cc
 * @date   Created : Feb 02, 2014 16:02:27 JST
 *   Last Modified : 2021-02-10 14:18:33 JST (ota)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *
 *    Copyright (C)2014
 */
#include "TPulseFinder.h"
#include <TCatPulseShape.h>
#include <TClonesArray.h>
#include <algorithm>
#include <numeric>
#include <functional>
#include <TArtSystemOfUnit.h>
#include <TF1.h>

ClassImp(art::TPulseFinder);

using art::TPulseFinder;

TPulseFinder::TPulseFinder()
   : fFunChargeRiseCompensation(NULL)
{
   StringVec_t def;
   def.push_back("at");
   RegisterInputCollection("InputCollection","pulse data (TCatPulseShape)",
                           fInputColName, def);
   RegisterOutputCollection("OutputCollection","output pulse data (TCatPulseShape)",
                            fOutputColName,TString("atpulse"));
   RegisterProcessorParameter("RisingThreshold","threshold for rising edge",
                              fRisingThreshold,(Float_t)10.);
   RegisterProcessorParameter("FallingThreshold","threshold for falling edge",
                              fFallingThreshold,(Float_t)10.);
   RegisterProcessorParameter("SavePulse","flag to save pulse",
                              fSavePulse,kFALSE);
   RegisterProcessorParameter("MaxLength","maximum length to be valid",
                              fMaxLength,150);
   RegisterProcessorParameter("PrePulse","the number of sampling for pre-pulse",
                              fPrePulse,10);
   RegisterProcessorParameter("PostPulse","the number of sampling for pre-pulse",
                              fPostPulse,10);
   RegisterProcessorParameter("Sparse","sparse mode (first hist is stored)",
                              fIsSparse,kFALSE);
   RegisterProcessorParameter("ClockPeriod","clock priod in ns",fClockPeriod,20.);
   RegisterProcessorParameter("Time2Drift",
                              "type of time to calculate drift length (0: offset, 1: cfd (0.5), 2: peak",
                              fTime2Drift,0);
   RegisterProcessorParameter("ChargeRiseCompensation","3 parameters for charge compensation with rise time",
                              fChargeRiseCompensation,DoubleVec_t(0));
   RegisterProcessorParameter("NumSampleForBaseline","the number of sample for the calculation of baseline",fNumSampleForBaseline,10);
   RegisterProcessorParameter("DoKeepInvalid","keep invalid pulse shape if 1 (default 0)",fDoKeepInvalid,kFALSE);
   FloatVec_t range;
   range.push_back(-10000.);
   range.push_back(10000.);
   RegisterProcessorParameter("OffsetRange","minimum and maximum range of offset",
                              fOffsetRange,range);

   Register(fNumAvgPoints("NumAvgPoints","number of average points",5));
   Register(fTpcGasProp("TpcGasProp","name of tpc static gas property","tpcGasProperty"));
   fGainShiftPrm.SetDoAuto(false);
   Register(fGainShiftPrm("GainShiftPrm","Correction parameter of gain shift",""));
}
TPulseFinder::~TPulseFinder()
{
}


void TPulseFinder::Init(TEventCollection *col)
{
   TEventObject *obj = NULL;
   if (fChargeRiseCompensation.size() > 0 && fChargeRiseCompensation.size() != 3) {
      SetStateError(TString::Format("3 parameters required for charge compensation with rise time : %zu parameters are given",
                                    fChargeRiseCompensation.size()));
      return;
   } 
      
                    
   
   if (!(obj = col->Get(fInputColName.front()))) {
      Info("Init","No such input %s",fInputColName.front().Data());
      return;
   }
   fInput = (TClonesArray**) obj->GetObjectRef();
   fOutput = new TClonesArray("art::TCatPulseShape");
   col->Add(fOutputColName,fOutput,fOutputIsTransparent);

   if (!fGainShiftPrm.Value().IsNull()) {
      void** obj = col->GetInfoRef(fGainShiftPrm.Value());
      if (!obj) {
         SetStateError(Form("No such object %s",fGainShiftPrm.Value().Data()));
         return;
      }
      fGainShiftPrm.SetData(reinterpret_cast<TClonesArray**>(obj));
   }
   
   if (fChargeRiseCompensation.size() == 3) {
      fFunChargeRiseCompensation = new TF1("funChargeRiseCompensation",
                                           "[2]-1/(1+TMath::Power(x/[0],[1]))",
                                           0.,100.);
      fFunChargeRiseCompensation->SetParameters(&fChargeRiseCompensation[0]);
   }

}



void TPulseFinder::Process()
{
   fOutput->Clear("C");
   Int_t nHits = (*fInput)->GetEntriesFast();
   const Double_t clockoffset[8] = { 0.000000,
0.780309,
0.435860,
0.363318,
0.456653,
-0.211933,
0.000000,
-0.668767
   };
   
   for (Int_t iHit = 0; iHit != nHits; iHit++) {
      TCatPulseShape *hit = (TCatPulseShape*) (*fInput)->At(iHit);

      if (!fDoKeepInvalid && !hit->IsValid()) continue;
      
      const std::vector<Float_t>& sample = hit->GetSample();
      std::vector<Float_t> smooth_sample(sample.size());
      for (Int_t i = 0, n = sample.size(); i < n; ++i) {
         std::vector<Float_t>::const_iterator start = sample.begin() + i;
         std::vector<Float_t>::const_iterator end = std::min(sample.end(),start + fNumAvgPoints);
         smooth_sample[i] = std::accumulate(start,end,0.)/(std::distance(start,end));
      }
      
      std::vector<Float_t>::const_iterator itbegin = smooth_sample.begin();
      std::vector<Float_t>::const_iterator itend = smooth_sample.end();
      std::vector<Float_t>::const_iterator it = smooth_sample.begin();
      std::vector<Float_t>::const_iterator itlead;
      std::vector<Float_t>::const_iterator itfirst;
      std::vector<Float_t>::const_iterator itlast;
      double risingThreshold = fRisingThreshold;
      double fallingThreshold = fFallingThreshold;
      TConverterBase *converter = NULL;
      if (fGainShiftPrm.Data()) {
         converter = static_cast<TConverterBase*>(fGainShiftPrm.Data()->UncheckedAt(hit->GetID()));
         risingThreshold = converter->Convert(risingThreshold);
         fallingThreshold = converter->Convert(fallingThreshold);
         
      }
      while (it != itend) {

         // find pulse over fRisingThreshold
         Bool_t invalidNoLeading = kFALSE;
         if (*it > risingThreshold) {
            invalidNoLeading = kTRUE;
         }
         
         itfirst = std::find_if(it,itend,std::bind(std::greater<Float_t>(),std::placeholders::_1,risingThreshold));
         it = itfirst;
         // break if no pulse is found
         if (itfirst == itend) break;
         itlead = itfirst = std::find_if(it,itend,std::bind(std::greater<Float_t>(),std::placeholders::_1,2.));

         // @todo remove magic number 100. (typical pulse length?)
         // find pulse end 
         itlast = std::find_if(itfirst + fNumAvgPoints,itend,std::bind(std::less<Float_t>(),std::placeholders::_1,fallingThreshold));
         Bool_t invalidNoTrailing = kFALSE;
         if (itlast == itend) {
            invalidNoTrailing = kTRUE;
         }
         // update first and last point
         itfirst = std::max(itfirst - fPrePulse,itbegin);
         itlast = std::min(itlast + fPostPulse,itend);

         // update present iterator
         it = itlast;

#if 0         
         if (std::distance(itfirst,itlast) < 10 + fPrePulse + fPostPulse) {
            // skip short pulse
            continue;
         }
#endif
         Double_t originalOffset = hit->GetOffset();
         Double_t pulseOffset = originalOffset+std::distance(itbegin,itfirst);
         Bool_t invalidLongPulse = (std::distance(itfirst,itlast) > fMaxLength);
         Bool_t invalidOutOfOffsetRange = (pulseOffset < fOffsetRange[0] || pulseOffset > fOffsetRange[1]);

         // proceed only for valid pulse
         std::vector<Float_t>::const_iterator itmax = max_element(itfirst,itend);
         Double_t baseline = std::accumulate(itfirst,itfirst+fNumSampleForBaseline,0.) / fNumSampleForBaseline;
         Double_t leadingOffset = originalOffset + std::distance(itbegin,itlead);
         Double_t riseTime = std::distance(itlead,itmax);
         Double_t maxSampleOffset = leadingOffset + riseTime;

         // recalculate baseline
         std::vector<Float_t> outputSample(itfirst,itlast);
         std::transform(outputSample.begin(),outputSample.end(),outputSample.begin(),
                        std::bind(std::minus<float>(),std::placeholders::_1,baseline));
         
         Double_t charge = std::accumulate(outputSample.begin(),outputSample.end(),0.);
         Double_t maxSample = *itmax - baseline;
         const Double_t fraction = 0.5;
         
         Double_t constantFraction = - clockoffset[hit->GetGeo()] + pulseOffset + 
            std::distance(outputSample.begin(),
                          std::find_if(outputSample.begin(),outputSample.end(),
                                       std::bind(std::greater<Float_t>(),std::placeholders::_1,maxSample * fraction)));

         Double_t posY = 0.;
         switch (fTime2Drift) {
         case 0:
            posY = fTpcGasProp->GetDriftLength(leadingOffset * fClockPeriod * TArtSystemOfUnit::ns);
            break;
         case 1:
            posY = fTpcGasProp->GetDriftLength(constantFraction * fClockPeriod * TArtSystemOfUnit::ns);
            break;
         case 2:
            posY = fTpcGasProp->GetDriftLength(maxSampleOffset * fClockPeriod * TArtSystemOfUnit::ns);
            break;
         }

         Bool_t invalidNegativeCharge = (charge < 0);
         if ((!fDoKeepInvalid) && 
             (invalidOutOfOffsetRange || invalidLongPulse ||
              invalidNegativeCharge || invalidNoTrailing || invalidNoLeading)) {
            continue;
         }

         // make output
         TCatPulseShape *out = NULL;
         if (fIsSparse) {
            out = (TCatPulseShape*) fOutput->ConstructedAt(hit->GetID());
//               if (hit->GetID() > fOutput->GetLast()) {
            for (Int_t i=0; i!=fOutput->GetEntriesFast(); i++) {
               // to avoid NULL dereference
               if (fOutput->UncheckedAt(i)) continue;
               (TCatPulseShape*) fOutput->ConstructedAt(i);
            }
//               }
            if (!out->TestQualityBit(TCatPulseShape::kInvalid)) break;
         } else {
            out = (TCatPulseShape*) fOutput->ConstructedAt(fOutput->GetEntriesFast());
         }

         // prepare output
         hit->Copy(*out);
#if 0            
         Double_t a = 1.,b = 0.;
         // calculate timing by using extrapolation
         {
            std::vector<Float_t> clock;
            std::vector<Float_t>::const_iterator max_it = max_element(itfirst,itlast);
            int nclock = std::distance(itfirst,max_it);
            if (nclock>3) nclock -= 3;
            for (int i = 0; i != nclock; i++) {
               clock.push_back(i);
            }
            Double_t xy = std::inner_product(itfirst,itfirst+nclock,clock.begin(),0.);
            Double_t xx = std::inner_product(clock.begin(),clock.end(),clock.begin(),0.);
            Double_t x  = std::accumulate(clock.begin(),clock.end(),0.);
            Double_t y  = std::accumulate(itfirst,itfirst+nclock,0.);
            a = (nclock * xy - x * y) / (nclock * xx - x*x);
            b = (xx*y - xy * x) / (nclock * xx - x*x);
         }
#endif            
         out->SetOffset(pulseOffset);
         out->SetLeadingEdgeOffset(leadingOffset);
         out->SetMaxSampleOffset(maxSampleOffset);
         out->SetRiseTime(maxSampleOffset - leadingOffset);
         out->SetMaxSample(maxSample);
         out->SetBaseline(baseline);
#if 0            
         out->SetBaselineRMS();
         out->AddMoment();
#endif
                                  
         out->GetPos().SetY(posY);
         out->SetCharge(charge);
         out->SetTiming(out->GetY()/fTpcGasProp->GetDriftVelocity());

         if (fFunChargeRiseCompensation) {
            const Double_t c = charge;
            const Double_t dt = maxSampleOffset - leadingOffset;
            out->SetCharge(c / fFunChargeRiseCompensation->Eval(dt));
         }
            
         out->ResetQualityBit(TCatPulseShape::kInvalid);

            
         if (invalidLongPulse) {
            out->SetQualityBit(TCatPulseShape::kLongPulse);
            out->SetQualityBit(TCatPulseShape::kInvalid);
         }
         if (invalidOutOfOffsetRange) {
            out->SetQualityBit(TCatPulseShape::kOutOfOffset);
            out->SetQualityBit(TCatPulseShape::kInvalid);
         }
         if (invalidNegativeCharge) {
            out->SetQualityBit(TCatPulseShape::kInvalid);
         }

         if (invalidNoTrailing|| invalidNoLeading) {
            out->SetQualityBit(TCatPulseShape::kInvalid);
         }
         int num = outputSample.size();
         out->SetNumSample(num);


         if (converter) {
            out->SetCharge(converter->Convert(out->GetCharge()));
            out->SetMaxSample(converter->Convert(out->GetMaxSample()));
         }
         // optional output
         if (fSavePulse) {
            //printf("%d %d %d %f %f\n",iHit,std::distance(itbegin,itfirst),std::distance(itbegin,itlast),hit->GetOffset(),out->GetOffset());
            out->GetSample().resize(num);
            out->GetClock().resize(num);
            std::copy(outputSample.begin(),outputSample.end(),out->GetSample().begin());
            if (converter) {
               std::transform(outputSample.begin(),outputSample.end(),outputSample.begin(),
                              std::bind(std::mem_fn(&TConverterBase::Convert),converter,std::placeholders::_1));
            }
            for (int i = 0; i!= num; i++) {
               out->GetClock()[i] = (double)i;
            }
         }
      }
   }
}

