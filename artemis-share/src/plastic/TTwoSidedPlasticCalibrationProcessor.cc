/**
 * @file   TTwoSidedPlasticCalibrationProcessor.cc
 * @brief
 *
 * @date   Created:       2013-10-10 15:58:58
 *         Last Modified: 2015-04-09 14:23:23 JST (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved.
 */

#include "TTwoSidedPlasticCalibrationProcessor.h"
#include "TTwoSidedPlasticData.h"
#include "TMultiHitArray.h"
#include "TConverterBase.h"
#include "TConverterUtil.h"

#include <TSystem.h>
#include <TClonesArray.h>

using art::TTwoSidedPlasticCalibrationProcessor;
using std::vector;

ClassImp(art::TTwoSidedPlasticCalibrationProcessor);

TTwoSidedPlasticCalibrationProcessor::TTwoSidedPlasticCalibrationProcessor()
   : fPlasticIn1(NULL), fPlasticIn2(NULL), fPlasticOut(NULL),
     fHits1(NULL), fHits2(NULL), fPosConverterArray(NULL)
{
   const IntVec_t defDetIDRange(2,1); // [1,1]

   RegisterInputCollection("InputCollection1","input of 1st side",
			   fInputColName1,TString("plastic1"),
			   &fPlasticIn1,"TClonesArray","art::TTimingChargeData");
   RegisterInputCollection("InputCollection2","input of 2nd side",
			   fInputColName2,TString("plastic2"),
			   &fPlasticIn2,"TClonesArray","art::TTimingChargeData");
   RegisterOutputCollection("OutputCollection","output plastic calibrated array",
			    fOutputColName,TString("plastic_cal"),
			    &fPlasticOut,"TClonesArray","art::TTwoSidedPlasticData");
   RegisterProcessorParameter("DetIDRange","range of detector ID",
			      fDetIDRange,defDetIDRange);
   RegisterProcessorParameter("PositionConverterArray","name of pos converter array",
			      fPosConverterArrayName,TString("no_conversion"));
   RegisterProcessorParameter("QDiffType","0: qDiff = q1 - q2, 1: qDiff = log(q1/q2)",
			      fQDiffType,0);
   const FloatVec_t range(3,0.);
   RegisterProcessorParameter("ValidTimeRange","[min,max,offset] => Tmin = offset + min etc. ignored if min == max",
                              fValidTimeRange,range);

}

TTwoSidedPlasticCalibrationProcessor::~TTwoSidedPlasticCalibrationProcessor()
{
   delete fPlasticOut;
   delete fHits1;
   delete fHits2;
}

void TTwoSidedPlasticCalibrationProcessor::Init(TEventCollection *col)
{
   Info("Init","(%s, %s) => %s",
	fInputColName1.Data(),fInputColName2.Data(),fOutputColName.Data());

   // calculate valid time range
   if (TMath::Abs(fValidTimeRange[0]-fValidTimeRange[1]) < TMath::Limits<Float_t>::Epsilon()) {
      fValidTimeMin = fValidTimeMax = 0.;
   } else {
      if (fValidTimeRange[0] > fValidTimeRange[1]) {
         SetStateError("Time range : min > max");
         return;
      }
      fValidTimeMin = fValidTimeRange[2] + fValidTimeRange[0];
      fValidTimeMax = fValidTimeRange[2] + fValidTimeRange[1];
   }
   
// TODO: remove fDetIDRange

   fNDet = fDetIDRange[1] - fDetIDRange[0] + 1;
   if (fNDet>0) {
      Info("Init","ID range: [%d:%d], ndet = %d",fDetIDRange[0],fDetIDRange[1],fNDet);
   } else {
      SetStateError("invalid ID range!");
      return;
   }

   fHits1 = new TMultiHitArray;
   fHits2 = new TMultiHitArray;

   if(strcmp(fPosConverterArrayName.Data(),"no_conversion")) {
      TConverterUtil::SetConverterArray(&fPosConverterArray,fPosConverterArrayName,col);
      Info("Init","PositionConverterArray: %s",fPosConverterArrayName.Data());
      fPosConverterArraySize = fPosConverterArray->size();
   }
}

void TTwoSidedPlasticCalibrationProcessor::FillHitArray(Int_t id)
{
   TMultiHitArray *const hitArray    = id ? fHits2 : fHits1;
   const TClonesArray *const inArray = id ? *fPlasticIn2 : *fPlasticIn1;

   hitArray->Clear();
   for(Int_t iHit=0, nHit = inArray->GetEntriesFast(); iHit != nHit; ++iHit) {
      TTimingChargeData *const data = static_cast<TTimingChargeData*>(inArray->At(iHit));
      hitArray->Add(data);
   }
}

void TTwoSidedPlasticCalibrationProcessor::Process()
{
   fPlasticOut->Clear("C");

   FillHitArray(0);
   FillHitArray(1);
   if (fValidTimeMax-fValidTimeMin < TMath::Limits<Float_t>::Epsilon()) {
      ProcessByIndex();
   } else {
      ProcessByTimeDiff();
   }

}

void TTwoSidedPlasticCalibrationProcessor::ProcessByIndex()
{
   // data1,2 are should be filtered out by some timing, charge gate (TODO)
   for(Int_t iDet=0, nDets = fHits1->GetEntriesFast(); iDet!=nDets; ++iDet) {
      const TObjArray *const hitArray1 = fHits1->At(iDet);
      const Int_t size1 = hitArray1->GetEntriesFast();
      if (!size1) continue;
      const Int_t detID = static_cast<TDataObject*>(hitArray1->At(0))->GetID();

      const TObjArray *const hitArray2 = fHits2->GetHits(detID);
      if (!hitArray2) continue;
      const Int_t size2 = hitArray2->GetEntriesFast();

      for(Int_t iHit=0, nHit = std::max(size1,size2); iHit != nHit; ++iHit) {
	 TTwoSidedPlasticData *const output =
	    static_cast<TTwoSidedPlasticData*>(fPlasticOut->ConstructedAt(fPlasticOut->GetEntriesFast()));

	 output->SetDetID(detID);
	 output->SetHitID(iHit + HITID_ORIGIN);

	 /* process input for 1st side */
	 if (iHit < size1) {
	    TTimingChargeData* const hit1 =
	       static_cast<TTimingChargeData*>(hitArray1->At(iHit));
	    output->SetSide1(hit1);
	    if (IsValid(hit1->GetTiming())) {
	       output->ValidateHit(TTwoSidedPlasticData::kT1);
	       if (IsValid(hit1->GetCharge()))
		  output->ValidateHit(TTwoSidedPlasticData::kQ1);
	    }
	 }

	 /* process input for 2nd side */
	 if (iHit < size2) {
	    TTimingChargeData* const hit2 =
	       static_cast<TTimingChargeData*>(hitArray2->At(iHit));
	    output->SetSide2(hit2);
	    if (IsValid(hit2->GetTiming())) {
	       output->ValidateHit(TTwoSidedPlasticData::kT2);
	       if (IsValid(hit2->GetCharge()))
		  output->ValidateHit(TTwoSidedPlasticData::kQ2);
	    }
	 }

	 /* process timing information */
	 if (output->HasValidHit(TTwoSidedPlasticData::kT1)
	     && output->HasValidHit(TTwoSidedPlasticData::kT2)) {
	    const Double_t t1 = output->GetUncheckedT1();
	    const Double_t t2 = output->GetUncheckedT2();
	    const Double_t timing = (t1 + t2) * 0.5;
	    const Double_t tdiff  = t1 - t2;
            output->SetT1(output->GetUncheckedT1());
            output->SetT2(output->GetUncheckedT2());
	    output->SetTiming(timing);
	    output->SetTDiff(tdiff);

	    if (fPosConverterArray) {
	       const Int_t idx = detID - fDetIDRange[0];
	       if (idx >= 0 || idx < fPosConverterArraySize) {
		  output->SetPos(fPosConverterArray->at(idx)->Convert(tdiff));
	       } else {
		  Warning("Process","index out of range for PosConverterArray");
		  gSystem->Sleep(100);
	       }
	    }

	    /* process charge information */
	    if (output->HasValidHit(TTwoSidedPlasticData::kQ1)
		&& output->HasValidHit(TTwoSidedPlasticData::kQ2)) {
	       const Double_t q1 = output->GetUncheckedQ1();
	       const Double_t q2 = output->GetUncheckedQ2();
	       const Double_t charge = TMath::Sqrt(q1 * q2);
	       const Double_t qDiff = fQDiffType ? TMath::Log(q1 / q2) : q1 - q2;
               output->SetQ1(output->GetUncheckedQ1());
               output->SetQ2(output->GetUncheckedQ2());
	       output->SetCharge(charge);
	       output->SetQDiff(qDiff);
	    }
	 }
      }
   }
   
}

void TTwoSidedPlasticCalibrationProcessor::ProcessByTimeDiff()
{
//   printf("ProcessByTimeDiff\n");
   // loop for all the detectors in array1
   for(Int_t iDet=0, nDets = fHits1->GetEntriesFast(); iDet!=nDets; ++iDet) {
      const TObjArray *const hitArray1 = fHits1->At(iDet);
      const Int_t size1 = hitArray1->GetEntriesFast();
      if (!size1) continue;
      // choose one id
      const Int_t detID = static_cast<TDataObject*>(hitArray1->At(0))->GetID();
      // get corresponding hit array from array2
      const TObjArray *const hitArray2 = fHits2->GetHits(detID);
      if (!hitArray2) continue;
      const Int_t size2 = hitArray2->GetEntriesFast();
      if (!size2) continue;

      std::list<TTimingChargeData*> list1;
      std::list<TTimingChargeData*> list2;
      for(Int_t iHit1=0, nHit = size1; iHit1 != nHit; ++iHit1) {
         list1.push_back(static_cast<TTimingChargeData*>(hitArray1->At(iHit1)));
      }
      for(Int_t iHit2=0, nHit = size2; iHit2 != nHit; ++iHit2) {
         list2.push_back(static_cast<TTimingChargeData*>(hitArray2->At(iHit2)));
      }
      
      TTimingChargeData *hit1 = NULL;
      TTimingChargeData *hit2 = NULL;
      TTimingChargeData *last1 = list1.back();
      Int_t iHit = 0;
      // loop for all the hits for 1st side
//      printf("loop1\n");
      while (list1.size()!=0) {
         hit1 = list1.front();
         list1.pop_front();
         // check the validity of the timing
//         printf("loop1 aaa\n");
         if (!IsValid(hit1->GetTiming())) {
            continue;
         }
//         printf("loop1 bbb\n");
         // loop for all the his for 2nd side
         TTimingChargeData *last2 = list2.back();
//         printf("loop2\n");
         while (list2.size() != 0) {
            hit2 = list2.front();
//            printf("list2 %p\n",hit2);
            list2.pop_front();
            // check the validity of the timing
            if (!IsValid(hit2->GetTiming())) {
               // skip this hit
               continue;
            }
//            printf("list2 %p aaa\n",hit2);
            // check the validity of time difference
//            printf("%f %f %f\n",hit1->GetTiming(),hit2->GetTiming(),fValidTimeDiff);
            Double_t tdiff = hit1->GetTiming() - hit2->GetTiming();
            if (tdiff < fValidTimeMin || fValidTimeMax < tdiff) {
               // time difference is not valid.
               list2.push_back(hit2);
               if (hit2 == last2) {
                  // last hit in array2
                  break;
               } else {
                  // continue next hit
                  continue;
               }
            }
//            printf("output\n");
            // create new output
            TTwoSidedPlasticData *const output =
               static_cast<TTwoSidedPlasticData*>(fPlasticOut->ConstructedAt(fPlasticOut->GetEntriesFast()));
            output->SetDetID(detID);
            output->SetHitID(iHit+ HITID_ORIGIN);
            ++iHit;
            output->SetSide1(hit1);
	    output->SetSide2(hit2);
            output->ValidateHit(TTwoSidedPlasticData::kT1);
            output->ValidateHit(TTwoSidedPlasticData::kT2);
	    const Double_t t1 = output->GetUncheckedT1();
	    const Double_t t2 = output->GetUncheckedT2();
	    const Double_t timing = (t1 + t2) * 0.5;
	    output->SetTiming(timing);
	    output->SetTDiff(tdiff);
            output->SetCharge(TMath::Sqrt(output->GetUncheckedQ1()*output->GetUncheckedQ2()));
            output->SetSide1(hit1);
            output->SetSide2(hit2);
            output->SetQ1(output->GetUncheckedQ1());
            output->SetQ2(output->GetUncheckedQ2());
            output->SetT1(output->GetUncheckedT1());
            output->SetT2(output->GetUncheckedT2());
            if (dynamic_cast<TTwoSidedPlasticData*>(hit1) &&
                dynamic_cast<TTwoSidedPlasticData*>(hit2)) {
               output->SetTDiff1(static_cast<TTwoSidedPlasticData*>(hit1)->GetTDiff());
               output->SetTDiff2(static_cast<TTwoSidedPlasticData*>(hit2)->GetTDiff());
            }

            if (IsValid(hit1->GetCharge()))
               output->ValidateHit(TTwoSidedPlasticData::kQ1);
            if (IsValid(hit2->GetCharge()))
               output->ValidateHit(TTwoSidedPlasticData::kQ2);
            // initialize hits to indicate the correct hits are created
            hit1 = NULL;
            hit2 = NULL;
            break;
         }
//         printf("loop2 done\n");
         if (hit1 == last1) {
            // last hits
            list1.push_back(hit1);
            break;
         }
      }
//      printf("loop1 done\n");
      list1.clear();
      list2.clear();
      // @todo fill unpaired hits or not
   }
}
