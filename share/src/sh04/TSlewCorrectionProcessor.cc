/**
 * @file   TSlewCorrectionProcessor.cc
 * @brief  slew correction for tof
 *
 * @date   Created       : 2014-11-07 14:20:18 JST
 *         Last Modified : 2015-03-06 12:11:53 JST (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 KAWASE Shoichiro
 */

#include "TSlewCorrectionProcessor.h"

#include <TClonesArray.h>

#include <TTimingData.h>
#include <TTwoSidedPlasticData.h>

using art::sh04::TSlewCorrectionProcessor;

ClassImp(TSlewCorrectionProcessor)

TSlewCorrectionProcessor::TSlewCorrectionProcessor()
   : fOutputCol(NULL) {
   RegisterInputCollection("InputCollection","TOF Input collection",
			   fInputColName,TString("tof"),
			   &fInputCol,TClonesArray::Class_Name(),TTimingData::Class_Name());
   RegisterInputCollection("ChargeCollection","Charge Input Collection",
			   fChargeColName,TString("plastic"),
			   &fChargeCol,TClonesArray::Class_Name(),TTwoSidedPlasticData::Class_Name());
   RegisterOutputCollection("OutputCollection","TOF Output Collection",
			    fOutputColName,TString("tof_corr"));
//			    &fOutputCol,TClonesArray::Class_Name(),TTimingData::Class_Name());
   RegisterProcessorParameter("ChargeDetID","Detector ID for Charge Input",
			      fChargeDetID,0);
   RegisterProcessorParameter("Parameter1f","Parameter1f",fParameter1f, 0.f);
   RegisterProcessorParameter("Parameter1b","Parameter1b",fParameter1b, 0.f);
   RegisterProcessorParameter("Parameter2f","Parameter2f",fParameter2f, 0.f);
   RegisterProcessorParameter("Parameter2b","Parameter2b",fParameter2b, 0.f);
}

TSlewCorrectionProcessor::~TSlewCorrectionProcessor()
{
   delete fOutputCol;
}


void TSlewCorrectionProcessor::Init(TEventCollection* col)
{
   fOutputCol = new TClonesArray(TTimingData::Class(),1);
   fOutputCol->SetName(fOutputColName);
   col->Add(fOutputColName,fOutputCol,fOutputIsTransparent);
}

namespace {
inline const art::TTwoSidedPlasticData * GetPlastic(const TClonesArray *const ca,const Int_t detID) {
   const Int_t n = ca->GetEntriesFast();
   for(Int_t i = 0; i!=n;++i) {
      const art::TTwoSidedPlasticData *const obj =
	 static_cast<art::TTwoSidedPlasticData*>(ca->At(i));
      if(obj->GetDetID() == detID) return obj;
   }
   return NULL;
}

inline Double_t CalcCorrection(Double_t q, Double_t p1, Double_t p2)
{
   const Double_t q0 = 500.;
   return - p1 * (1. - TMath::TanH(p2 *(q/q0)));
}
}

void TSlewCorrectionProcessor::Process()
{
   fOutputCol->Clear("C");
   if (!(*fInputCol)->GetEntriesFast()) return;
   const TTimingData *const tof = static_cast<TTimingData*>((*fInputCol)->At(0));
   const TTwoSidedPlasticData *const pla = GetPlastic(*fChargeCol,fChargeDetID);
   if (!pla) return;
   const Double_t t  = tof->GetTiming();
   const Double_t q1 = pla->GetQ1();
   const Double_t q2 = pla->GetQ2();
   const Double_t t0 = t + CalcCorrection(q1, fParameter1f, fParameter2f)
                         + CalcCorrection(q2, fParameter1b, fParameter2b);

   TTimingData *const tofC = static_cast<TTimingData*>(fOutputCol->ConstructedAt(0));
   tofC->SetTiming(t0);
   tofC->SetDetID(0);
}
