/* $Id:$ */
/**
 * @file   TCoinRegMappingProcessor.cc
 * @date   Created : Oct 31, 2013 06:31:02 JST
 *   Last Modified : 2014-06-19 17:01:16 JST (kawase)
 * @author Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>
 *
 *
 *    Copyright (C)2013
 */
#include "TCoinRegMappingProcessor.h"

#include <TCategorizedData.h>
#include <TRawDataSimple.h>

#include "TCoinRegData.h"

using art::TCoinRegMappingProcessor;

ClassImp(TCoinRegMappingProcessor)

namespace {
   typedef art::TRawDataSimple<UShort_t> CoinReg_t;
}

TCoinRegMappingProcessor::TCoinRegMappingProcessor()
{
   RegisterInputCollection("InputCollection","rawdata object returned by TArtEventStore",
                           fInputColName,TString("catdata"),
			   &fCategorized,"art::TCategorizedData");
   RegisterOutputCollection("OutputCollection","output collection name",
                            fOutputColName,TString("coin"),
			    &fRegHit,"art::TCoinRegData");
   RegisterProcessorParameter("CatID","Category ID of coinsidence register",fCatID,1);
   RegisterProcessorParameter("DetID","Detector ID of coinsidence register",fDetID,0);
   RegisterProcessorParameter("Mask","register mask",fMask,0xffff);
}

TCoinRegMappingProcessor::~TCoinRegMappingProcessor(){}

void TCoinRegMappingProcessor::Process()
{
   fRegHit->Clear("C");
   TObjArray *const reg = (*fCategorized)->FindCategory(fCatID);
   if (!reg) return;
   for (Int_t idet = 0, ndet = reg->GetEntriesFast(); idet!=ndet; ++idet) {
      const TObjArray *const det = static_cast<TObjArray*>(reg->At(idet));
      if (!det) continue;
      const TRefArray *const types = static_cast<TRefArray*>(det->At(0));
      if (!types) continue;
      const CoinReg_t *const hit = static_cast<CoinReg_t*>(types->At(0));
      if (!hit) continue;
      const Int_t detID = hit->GetDetID();
      if (detID == fDetID) {
         UShort_t bits = (hit->Get()&fMask);
         for (Int_t i=0; i!= 16; ++i) {
            if (bits & (1<<i)) {
               fRegHit->SetQualityBit((1<<i));
               fRegHit->Add(i);
            }
         }
	 return;
      }
   }
}
