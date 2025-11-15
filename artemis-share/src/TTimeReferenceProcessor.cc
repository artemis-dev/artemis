/**
 * @file   TTimeReferenceProcessor.cc
 * @brief
 *
 * @date   Created:       2013-10-21 09:26:54
 *         Last Modified: 2014-07-01 11:10:34 JST (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    Copyright (C) 2013 KAWASE Shoichiro All rights reserved.
 */

#include "TTimeReferenceProcessor.h"
#include <TSegmentedData.h>
#include <TRawTiming.h>

using art::TTimeReferenceProcessor;

ClassImp(art::TTimeReferenceProcessor)

// Default constructor
TTimeReferenceProcessor::TTimeReferenceProcessor()
{
   const TString  defInput("segdata");
   const IntVec_t defRefConfig(5,0); // {0,0,0,0,0}
   const IntVec_t defSegConfig(4,0); // {0,0,0,0}

   RegisterInputCollection("InputCollection","Segmented Data",
			   fInputColName,defInput,
			   &fSegData, "art::TSegmentedData");
   RegisterProcessorParameter("RefConfig","Configuration of Reference Timing",
			      fRefConfig,defRefConfig);
   RegisterProcessorParameter("SegConfig","Configuration of Segment to be sorted",
			      fSegConfig,defSegConfig);
}

TTimeReferenceProcessor::~TTimeReferenceProcessor()
{
}

void TTimeReferenceProcessor::Init(TEventCollection*)
{
   Info("Init","RefConfig = [%3d,%3d,%3d,%3d,%3d]",
	fRefConfig[0],fRefConfig[1],fRefConfig[2],fRefConfig[3],fRefConfig[4]);
   Info("Init","SegConfig = [%3d,%3d,%3d,%3d]",
	fSegConfig[0],fSegConfig[1],fSegConfig[2],fSegConfig[3]);
}

void TTimeReferenceProcessor::Process()
{
   if (const Int_t tref = GetTimeReference()) SetTimeReference(tref);
}

Int_t TTimeReferenceProcessor::GetTimeReference() const
{
   if (TObjArray *const refSeg = (*fSegData)->FindSegment(fRefConfig[0],fRefConfig[1],fRefConfig[2])) {
      for (Int_t iData = 0, nData = refSeg->GetEntriesFast();
	   iData != nData; ++iData) {
	 const TRawTiming *const obj = static_cast<const TRawTiming*>(refSeg->At(iData));
	 if (obj->GetGeo() != fRefConfig[3] || obj->GetCh() != fRefConfig[4]) continue;
	 return obj->GetRawTiming();
      }
   }
   return 0;
}

void TTimeReferenceProcessor::SetTimeReference(Int_t tref)
{
   if (TObjArray *const seg = (*fSegData)->FindSegment(fSegConfig[0],fSegConfig[1],fSegConfig[2])) {
      for (Int_t iData = 0, nData = seg->GetEntriesFast();
	   iData != nData; ++iData) {
	 TRawTiming *const obj = static_cast<TRawTiming*>(seg->At(iData));
	 if (obj->GetGeo() != fSegConfig[3]) continue;
	 obj->SetReference(tref);
      }
   }
}
