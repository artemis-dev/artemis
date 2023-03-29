/**
 * @file   TNyokiTimingProcessor.cc
 * @brief  nyoki timing
 *
 * @date   Created       : 2014-11-17 10:20:04 JST
 *         Last Modified : 2015-02-13 10:30:56 JST (kawase)
 * @author KAWASE Shoichiro <kawase@cns.s.u-tokyo.ac.jp>
 *
 *    (C) 2014 KAWASE Shoichiro
 */

#include "TNyokiTimingProcessor.h"

#include <TClonesArray.h>
#include <constant.h>
#include <TTimingChargeData.h>

using art::sh04::TNyokiTimingProcessor;

ClassImp(TNyokiTimingProcessor)

TNyokiTimingProcessor::TNyokiTimingProcessor()
   : fOutputCol(NULL) {
   RegisterInputCollection("InputCollection","array containing nyoki information",
			   fInputColName,"nyoki",
			   &fInputCol,TClonesArray::Class_Name(),TTimingChargeData::Class_Name());
   RegisterOutputCollection("OutputCollection","output collection name",
			    fOutputColName,"nyoki_t",
			    &fOutputCol,TClonesArray::Class_Name(),TTimingChargeData::Class_Name());
}

TNyokiTimingProcessor::~TNyokiTimingProcessor()
{
   delete fOutputCol;
}

void TNyokiTimingProcessor::Init(TEventCollection *)
{
}

void TNyokiTimingProcessor::Process()
{
   fOutputCol->Clear("C");
   
   Double_t                 maxCharge = 0;
   const TTimingChargeData *maxChargeData = NULL;

   for (Int_t iData = 0, nData = (*fInputCol)->GetEntriesFast(); iData != nData; ++iData) {
      const TTimingChargeData *const data =
	 static_cast<const TTimingChargeData*>((*fInputCol)->At(iData));

      const Double_t charge = data->GetCharge();
      if (!IsValid(charge)) continue;

      if (charge > maxCharge) {
	 maxCharge = charge;
	 maxChargeData = data;
      }
   }

   if (maxChargeData) {
      TTimingChargeData *out = static_cast<TTimingChargeData*>(fOutputCol->ConstructedAt(0));
      maxChargeData->Copy(*out);
      out->SetDetID(0);
   }
}
