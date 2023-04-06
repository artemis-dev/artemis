/**
 * @file   TTimingSubtractionProcessor.cc
 * @brief
 *
 * @date   Created:       2013-11-07 15:26:17
 *         Last Modified: 2014-07-01 10:55:11 JST (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved.
 */

#include "TTimingSubtractionProcessor.h"
#include "TTimeDifference.h"

#include <TClonesArray.h>
#include <TObjArray.h>
#include <TClass.h>
#include <TSystem.h>

#include "constant.h"

using art::TTimingSubtractionProcessor;

ClassImp(art::TTimingSubtractionProcessor)

// Default constructor
TTimingSubtractionProcessor::TTimingSubtractionProcessor()
   : fInData(NULL), fOutData(NULL) , fStartTimingData(NULL),
     fHitIDArray(NULL), fStartTiming(NULL)
{
   RegisterInputCollection("InputCollection",
			   "collection including timing information (inherits from ITiming)",
			   fInputColName,TString("plastic"),
			   &fInData,"TClonesArray","art::ITiming");
   RegisterInputCollection("StartTimingCollection",
			   "collection including reference timing information",
			   fStartTimingColName,TString("start_timing"),
			   &fStartTimingData,"TClonesArray","art::ITiming");
   RegisterOutputCollection("OutputCollection","output collection name",
			    fOutputColName,TString("plastic_tof"));
   RegisterProcessorParameter("StartTimingDetID","detID of reference timing source (if -1 use any id)",
                              fStartTimingDetID,1);
   RegisterProcessorParameter("UseMultipleStartTiming",
			      "enables multihit start timing treatment. If non-zero, output class is art::TTimeDiffernce, else, it is identical to input. In the latter case, both Get/SetTiming() must be available in that class.",
                              fUseMultipleStartTiming,0);
   RegisterProcessorParameter("MaxDetID","maximum detID of input",
                              fMaxDetID,512);
   const DoubleVec_t range(3,0.);
   RegisterProcessorParameter("ValidTimeRange","[min,max,offset] => Tmin = offset + min etc. ignored if min == max",
                              fValidTimeRange,range);
   RegisterOptionalParameter("UseStartAsTimestamp","timestamp is start (if 1) or end (if 0)",fUseStart,1);
}

TTimingSubtractionProcessor::~TTimingSubtractionProcessor()
{
   delete fOutData;
   delete fStartTiming;
   delete fHitIDArray;
}

void TTimingSubtractionProcessor::Init(TEventCollection *col)
{
   Info("Init","%s => %s",fInputColName.Data(),fOutputColName.Data());

   {
      const TClass *const inClass = (*fInData)->GetClass();
      if (!inClass->InheritsFrom(art::TDataObject::Class())) {
         SetStateError(Form("Input class does not inherit from TDataObject"));
         return;
      } else if (!inClass->InheritsFrom("art::ITiming")) {
         SetStateError(Form("Input class does not inherit from ITiming"));
         return;
      }
      const TClass *const outClass =
	 fUseMultipleStartTiming ? TTimeDifference::Class() : inClass;
      fOutData = new TClonesArray(outClass);
   }

   if (TMath::Abs(fValidTimeRange[0]-fValidTimeRange[1]) < TMath::Limits<Float_t>::Epsilon()) {
      fValidTimeMin = fValidTimeMax = 0.;
   } else {
      if (fValidTimeRange[0] > fValidTimeRange[1]) {
         SetStateError("Time range : min > max");
         return;
      }
      fValidTimeMin = fValidTimeRange[2] + fValidTimeRange[0];
      fValidTimeMax = fValidTimeRange[2] + fValidTimeRange[1];
      if (fOutData) delete fOutData;
      fOutData = new TClonesArray((*fInData)->GetClass());
   }

   fOutData->SetName(fOutputColName);
   col->Add(fOutputColName,fOutData,fOutputIsTransparent);

   fStartTiming = new TObjArray;
   fHitIDArray  = new std::vector<Int_t>(fMaxDetID + 1);
}

void TTimingSubtractionProcessor::Process()
{

   if (fValidTimeMax-fValidTimeMin > TMath::Limits<Float_t>::Epsilon()) {
      ProcessByTimeDiff();
      return;
   }
   
   
   /* initialization */
   fOutData->Clear("C");
   fStartTiming->Clear();
   for(Int_t i = 0, n = fHitIDArray->size(); i!=n; ++i) {
      fHitIDArray->at(i) = HITID_ORIGIN - 1;
   }

   /* get start timing */
   for(Int_t i = 0, n = (*fStartTimingData)->GetEntriesFast(); i != n; ++i) {
      TDataObject *const startData = static_cast<TDataObject*>((*fStartTimingData)->At(i));
      const ITiming *const startDataT = dynamic_cast<ITiming*>(startData);
      if ( startData
	   && (fStartTimingDetID < 0 || startData->GetID() == fStartTimingDetID)
	   && IsValid(startDataT->GetTiming()) ) {
	 fStartTiming->Add(startData);
      }
   }

   if (fStartTiming->IsEmpty()) return;

   /* get input timing */
   for(Int_t iData=0, nData = (*fInData)->GetEntriesFast();
       iData != nData; ++iData) {
      const TDataObject *const inData = static_cast<TDataObject*>((*fInData)->At(iData));
      const ITiming *const inDataT = dynamic_cast<const ITiming*>(inData);

      const Int_t detID = inData->GetID();
      if(!IsValid(detID)) continue;
      if(detID >= fMaxDetID) {
	 Warning("Process","detID is larger than MaxDetID. reconfigure MaxDetID in your yaml file.");
	 gSystem->Sleep(100);
	 return;
      }

      ++(fHitIDArray->at(detID));

      if (!IsValid(inDataT->GetTiming())) continue;

      if (fUseMultipleStartTiming) {
	 /* subtraction for all the conbination */
	 for(Int_t i = 0, n = fStartTiming->GetEntriesFast(); i != n; ++i ) {
	    TTimeDifference *const outData =
	       static_cast<TTimeDifference*>(fOutData->ConstructedAt(fOutData->GetEntriesFast()));
	    const TDataObject *const startData = static_cast<TDataObject*>(fStartTiming->At(i));
	    const ITiming *const startDataT = dynamic_cast<const ITiming*>(startData);

	    outData->TakeDifference(inDataT,startDataT);
	    outData->SetDetID(detID);
	    outData->SetDetID0(startData->GetID());
	    outData->SetHitID(fHitIDArray->at(detID));
	    outData->SetHitID0(i + HITID_ORIGIN);
	 }
      } else {
	 /* only the 1st datum of start timing will be used for the subtraction */
	 TObject *const outData = fOutData->ConstructedAt(fOutData->GetEntriesFast());
	 ITiming *const outDataT = dynamic_cast<ITiming*>(outData);
	 const ITiming *const startDataT = dynamic_cast<const ITiming*>(fStartTiming->At(0));
	 inData->Copy(*outData);
	 *outDataT -= *startDataT;
      }
   }
}

void TTimingSubtractionProcessor::ProcessByTimeDiff()
{
   fOutData->Clear("C");

   std::vector<ITiming*> start;
   std::vector<ITiming*> dataT;
   std::vector<TDataObject*> data;
   
   for (Int_t i = 0, n = (*fStartTimingData)->GetEntriesFast(); i < n; ++i) {
      TObject *obj = (*fStartTimingData)->UncheckedAt(i);
      ITiming *time = dynamic_cast<ITiming*>(obj);
      if (!time) continue;
      start.push_back(time);
   }
   // if start timing is empty just return
   if (start.empty()) return;

   for(Int_t iData=0, nData = (*fInData)->GetEntriesFast(); iData != nData; ++iData) {
      const TDataObject *const inData = static_cast<TDataObject*>((*fInData)->At(iData));
      const ITiming *const inDataT = dynamic_cast<const ITiming*>(inData);

      for (Int_t iStart = 0, nStart = start.size(); iStart < nStart; ++iStart) {
         Double_t tdiff = inDataT->GetTiming() - start[iStart]->GetTiming();
         if (fValidTimeMin < tdiff && tdiff <  fValidTimeMax) {
            TDataObject * outData = static_cast<TDataObject*>(fOutData->ConstructedAt(fOutData->GetEntriesFast()));
            ITiming *outT = dynamic_cast<ITiming*>(outData);
            inData->Copy(*outData);
            if (fUseStart) {
               outData->SetTimestamp(start[iStart]->GetTiming());
            } else {
               outData->SetTimestamp(inDataT->GetTiming());
            }
            outT->SetTiming(tdiff);
         }
      }
   }
}
