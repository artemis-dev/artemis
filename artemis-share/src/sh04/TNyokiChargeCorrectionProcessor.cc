/**
 * @file   TNyokiChargeCorrectionProcessor.cc
 * @brief  processor for nyoki charge correction
 *
 * @date   Created       : 2015-04-16 06:36:37 JST
 *         Last Modified : 2015-04-25 18:11:02 JST (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2015 KAWASE Shoichiro
 */

#include "TNyokiChargeCorrectionProcessor.h"

#include <fstream>
#include <algorithm>
#include <TClonesArray.h>
#include <TSpline.h>
#include <TTimingChargeData.h>
#include <TMonotoneTableConverter.h>
#include <TSplineConverter.h>
#include <TTrack.h>

using art::sh04::TNyokiChargeCorrectionProcessor;

ClassImp(TNyokiChargeCorrectionProcessor)

const TString DEFAULT_PATH = "path/to/file";

TNyokiChargeCorrectionProcessor::TNyokiChargeCorrectionProcessor()
   : fOutputCollection(NULL)
{
   RegisterInputCollection("InputCollection","array containing nyoki information",
			   fInputCollectionName, "nyoki",
			   &fInputCollection, TClonesArray::Class_Name(), TTimingChargeData::Class_Name());
   RegisterInputCollection("Track","array containing S1 position",
			   fTrackCollectionName, "smwdc_s1",
			   &fTrackCollection, TClonesArray::Class_Name(), TTrack::Class_Name());
   RegisterOutputCollection("OutputCollection","array containing nyoki information",
			    fOutputCollectionName, "nyoki_corrected",
			    &fOutputCollection, TClonesArray::Class_Name(), TTimingChargeData::Class_Name());
   RegisterProcessorParameter("Pedestal","pedestal for each nyoki",
			      fPedestal, FloatVec_t(kNumNyoki,0.F));
   RegisterProcessorParameter("TableFiles", "files containing conversion table",
			      fTableFiles, StringVec_t(1,DEFAULT_PATH));
   RegisterProcessorParameter("YTableFiles","files containing Y conversion table",
			      fYTableFiles, StringVec_t(1,DEFAULT_PATH));
}

namespace {
struct DeletePtr {
   template <typename T> void operator() (T* ptr) { delete ptr; }
};
}

TNyokiChargeCorrectionProcessor::~TNyokiChargeCorrectionProcessor()
{
   delete fOutputCollection;
   std::for_each(fTable.begin(), fTable.end(), DeletePtr());
   std::for_each(fYTable.begin(), fYTable.end(), DeletePtr());
}

void TNyokiChargeCorrectionProcessor::Init(TEventCollection*)
{
   if (fTableFiles.size() != kNumNyoki) {
      SetStateError(TString::Format("Number of Table input (%lu) differs from kNumNyoki (%d)",fTableFiles.size(),kNumNyoki));
      return;
   }

   if (fYTableFiles.size() != kNumNyoki) {
      SetStateError(TString::Format("Number of YTable input (%lu) differs from kNumNyoki (%d)",fTableFiles.size(),kNumNyoki));
      return;
   }

   if (fPedestal.size() != kNumNyoki) {
      SetStateError(TString::Format("Number of pedestal input (%lu) differs from kNumNyoki (%d)",fPedestal.size(), kNumNyoki));
      return;
   }

   for (StringVec_t::const_iterator it = fTableFiles.begin();
	it != fTableFiles.end(); ++it) {
      Converter_t *const converter = new Converter_t;
      if (converter->LoadFile(*it)) {
	 fTable.push_back(converter);
      } else {
	 SetStateError(TString::Format("Failed to load Table file: %s", it->Data()));
	 delete converter;
	 return;
      }
   }

   for (StringVec_t::const_iterator it = fYTableFiles.begin();
	it != fYTableFiles.end(); ++it) {
      YConverter_t *const converter = new YConverter_t;
      if (converter->LoadFile(*it)) {
	 fYTable.push_back(converter);
      } else {
	 SetStateError(TString::Format("Failed to load YTable file: %s", it->Data()));
	 delete converter;
	 return;
      }
   }
}

void TNyokiChargeCorrectionProcessor::Process()
{
   fOutputCollection->Clear("C");

   TTrack *track = static_cast<TTrack*>((*fTrackCollection)->At(0));

   for (Int_t iData = 0, nData = (*fInputCollection)->GetEntriesFast();
	iData != nData; ++iData) {
      TTimingChargeData *in = static_cast<TTimingChargeData*>((*fInputCollection)->At(iData));

      const Int_t id = in->GetDetID();
      if (id < 0 || id >= kNumNyoki) {
	 Warning("Process","Invalid nyoki id: %d", id);
	 continue;
      }

      const Double_t t0 = in->GetTiming();
      const Double_t q0 = in->GetCharge();
      const Double_t q  = (q0 - fPedestal[id]) / (fTable[id]->Convert(t0) - fPedestal[id])
	                                       / (fYTable[id]->Convert(track->Y()));

      TTimingChargeData *out = static_cast<TTimingChargeData*>(fOutputCollection->ConstructedAt(iData));
      in->Copy(*out);
      out->SetCharge(q);
   }
}
